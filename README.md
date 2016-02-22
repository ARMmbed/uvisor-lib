# The `uvisor-lib` library

This is a yotta module that provides a packaged version of uVisor for use in ARM mbed OS.

It consists of two main components:

1. The uVisor binaries
2. The APIs to interact with the uVisor.

To learn more about the uVisor refer to its [documentation](https://github.com/ARMmbed/uvisor).

If you are building your application with yotta on a supported platform, then you will already be running on top of the uVisor. At this stage of development, though, it is disabled by default.

## Word of caution

This version of the uVisor is an early technology preview with an **incomplete implementation of the security features** of the final product. Future versions of uVisor will add these functions.

> For this release uVisor is disabled by default. Use `UVISOR_SET_MODE(UVISOR_ENABLED);` in the top level application to enable it.

Some of the open uVisor issues in progress are listed here:

* [Known issues](https://github.com/ARMmbed/uvisor/issues?q=is%3Aopen+is%3Aissue+label%3Aissue)
* [FIXMEs](https://github.com/ARMmbed/uvisor/search?utf8=%E2%9C%93&q=FIXME)

## Supported platforms

The following platforms are currently supported:

* [NXP FRDM-K64F](http://developer.mbed.org/platforms/FRDM-K64F/)
* [STMicorelectronics STM32F429I-DISCO](http://www.st.com/web/catalog/tools/FM116/SC959/SS1532/PF259090)

The uVisor pre-linked binary images are built with the Launchpad [GCC ARM Embedded](https://launchpad.net/gcc-arm-embedded) toolchain. Currently only applications built with the same toolchain are supported.

## Introduction

The security model of the uVisor is based on secure boxes. A *secure box* is a code domain for which finely grained permissions are granted, protecting data and functions from the rest of the system.

By default, each software component that is included in the build process of your application ends up in a unique untrusted box, referred to as the *main box*. All code and data in the main box share the same level of permissions.

If you need to perform secure data access or function execution you may want to configure a secure box. Secure boxes all run in unprivileged mode but are granted protection and isolation from other boxes.

Access to protected resources is only possible when the context of the box they belong to is active. By default, the active context is the one of the main box. A special mechanism, called the *secure gateway*, allows you to switch context to your own secure box. You cannot switch context to a different box from yours.

Protecting a resource against other boxes means having exclusive access over the memory section the resource is mapped to. This can be in SRAM (for variables, objects), in a secure storage (for protected keys) or can be mapped to a peripheral. Access Control Lists (ACLs) are used to describe the list of resources you want to protect.

The following sections will describe all these features in greater detail.

To use the uVisor features with mbed OS you need to do the following:

* Add `uvisor-lib` as a dependency in your application `module.json`:

```
  "dependencies": {
    "uvisor-lib": "^1.0.0",
    ...
  },
```

* Include its header file in your source code:

```C
#include "uvisor-lib/uvisor-lib.h"
```

## How to configure a secure box

Configuring a secure box means specifying the resources that you want to protect and creating the entry points to your private secure environment. In particular, this is what you can do:

* [Protect your resources](#protect-your-resources), using Access Control Lists (ACLs) and the private *box context*.
* Use the [secure gateway](#secure-function-call) to ensure critical functions are executed from the context of the secure box.
* Use the [uvisor-lib APIs](#low-level-apis) to access restricted features (interrupts, system calls).

### Protect your resources

Resources can be both memory assets (SRAM, secure storage) and memory-mapped peripherals.

If you want to secure data/variables in SRAM, you can use what we call the *box context*, a private memory section that can be optionally coupled to the secure box. The box context generally provides 1KB of memory, and can be populated with a struct that encapsulates all your private resources:

```C
typedef struct {
    uint32_t op1;
    uint32_t op2;
    bool initialized;
} BoxContext;
```

`uvisor_ctx` is the pointer, maintained by uVisor, that allows to access the box context:

```C
void main(void)
{
    ...
    /* Only possible from the context of the secure box.
     * Access from other boxes causes a memory exception. */
    if (!uvisor_ctx->initialized) {
        uvisor_ctx->op1 = 0x42;
        uvisor_ctx->op2 = 0x23;
    }
    ...
}
```

If you want to gain exclusive access to peripherals, you need to specify an ACLs for each of them. ACLs can be used to specify the pointer, the size and the permission level associated with the resource they represent, providing a high level of granularity.

```C
static const UvisorBoxAclItem g_box_acl[] = {
    {PORTB,  sizeof(*PORTB),  UVISOR_TACLDEF_PERIPH},
    {RTC,    sizeof(*RTC),    UVISOR_TACLDEF_PERIPH},
    {LPTMR0, sizeof(*LPTMR0), UVISOR_TACLDEF_PERIPH},
}
```

Now that all your reseources are set-up for protection, you only need to group them together to configure the secure box, also specifying the name of the box and the required stack size.

```C
#define BOX_STACK_SIZE 0x100

UVISOR_BOX_CONFIG(my_box_name, g_box_acl, BOX_STACK_SIZE, BoxContext);
```

Please note that configuring a secure box in a file or whole software module does not mean that the whole module itself is a secure box. All peripherals, constants, variables and object instantiations that are not explicitly included in the secure box through the mechanism just described are automatically included in the context of the main box, unprotected from the rest of the code. A secure box is a domain in which several restrictions apply, so choose carefully what to include in it.

### Secure function call

By default also code execution happens in the context of the main box. You are generally free to execute code designed for a secure box, but if that code accesses a protected resource a memory exception will be thrown and the uVisor will halt the system. Secure boxes are hence required to expose special entry points to other boxes so that a context switch can be triggered. We call these entry points *secure gateways*.

To create a secure gateway, we suggest you use a wrapper function that performs the secure function call:

```C
/* The box is configured here. */
...

/* The actual function */
UVISOR_EXTERN uint32_t __secure_sum(uint32_t op3)
{
    /* If this code is run without a context switch, access to the context
     * elements will fail. */
    return uvisor_ctx->op1 + uvisor_ctx->op2 + op3;
}

/* The gateway to the actual function. */
uint32_t secure_sum(uint32_t op3)
{
    return secure_gateway(my_box_name, __secure_sum, op3);
}
```

The secure gateway helps you exposing only the unsecure entry points to your secure box, which trigger gateways to your protected environment. API users do not need to know the details of this implementation, and you are only responsible for the level of security of your own module.

Secure gateways may be called with a maximum of four 32bit arguments, and may
return a single 32bit value.

### Low level APIs

Access to some system registers or features is only possible in privileged mode, so low level APIs are provided to interact with the uVisor to perform those accesses. Currently, only interrupt management is supported.

#### Interrupt management

A box can register for interrupts and gain exclusive access to the corresponding IRQn slot, so that it is the only one in charge of modifying it. To register an interrupt routine, you have to set a handler for it and enable the corresponding IRQn slot, as you would do with regular NVIC registers:

```C
#define MY_HANDLER_IRQn 42

void my_handler(void)
{
    ...
}

void some_function(void)
{
    ...

    vIRQ_SetVector(MY_HANDLER_IRQn, (uint32_t) &my_handler);
    vIRQ_EnableIRQ(MY_HANDLER_IRQn);

    ...
}
```

After registration of an interrupt, its IRQn is bounded to the box that registered it.

Please note:

1. Currently, interrupt ownership is exclusive, meaning that multiple boxes cannot register for the same interrupt.
2. Registration works on a first-come-first-served basis.
3. Execution of an interrupt handler is always handled in unprivileged mode in the context of the box that registered for it.

## Full API documentation

For detailed documentation of all available macros and APIs, please refer to the [APIs documentation](DOCUMENTATION.md).

## Examples

We designed a yotta application, called [uvisor-helloworld](https://github.com/ARMmbed/uvisor-helloworld), that shows how to configure and use a secure box in mbed OS using the yotta build tools.

## Debugging

Failures and faults captured by the uVisor will trigger a system halt. Further debugging messages are silenced by default. If you want to enable them, you need to build you application in debug mode, using `yotta build -d`. This ensures that the debug version of uVisor is used.

Please refer to the [Debugging uVisor](https://github.com/ARMmbed/uvisor/blob/master/docs/DEBUGGING.md) guide for more details.
