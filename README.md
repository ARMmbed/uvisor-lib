# uvisor-lib

uvisor-lib is a yotta module that provides a secure environment for modular applications on mbed, and the APIs that interact with it.

It consists of two main components:

1. the uVisor binaries, which set up a protected environment where isolated compartments are granted finely tuned permissions.
2. APIs to interact with the uVisor.

If you are building your application with yotta on a supported platform, then you will already be running on top of the uVisor. At this stage of development, though, it is disabled by default.

To learn more about the uVisor refer to its [documentation](https://github.com/ARMmbed/uvisor-private).

Currently the only supported platform is the [Freescale FRDM-K64F](http://developer.mbed.org/platforms/FRDM-K64F/) ([GCC ARM Embedded toolchain](https://launchpad.net/gcc-arm-embedded)).

The current release version is 0.4.0.

## Introduction

The security model of the uVisor is based on secure boxes. A *secure box* is a code domain for which finely grained permissions are granted, protecting data and functions from the rest of the code.

By default, each yotta module that is included in the build process of an mbed application ends up in a unique unsecure box, referred to as the *main box*. All code and data in the main box share the same level of permissions.

If you need to protect data or function execution you may want to configure a secure box. Secure boxes all run in unprivileged mode but are granted protection and isolation from other boxes.

Access to protected resources is only possible when the context of the box they belong to is active. By default, the active context is the one of the main box. A special mechanism, called the *secure gateway*, allows you to switch context to your own secure box. You cannot switch context to a different box from yours.

The following sections will describe all these features in greater detail.

## How to Configure a Secure Box

To configure a secure box:

- Add uvisor-lib as a dependency in `module.json`:
```
  "dependencies": {
    "uvisor-lib": "ARMmbed/uvisor-lib#~0.2.0",
    ...
  },
```

- Include its header file in your source code:
```c
#include "uvisor-lib/uvisor-lib.h"
```

- [Protect your data](#protect-your-data) with `UVISOR_SECURE_CONST` and `UVISOR_SECURE_BSS`.

- [Configure your secure box](#configure-your-secure-box) with `UVISOR_BOX_CONFIG(...)`.

- Use the [secure gateway](#secure-function-call) to ensure critical functions are executed from the secure box context.

- Use the [uvisor-lib APIs](#low-level-apis) to access restricted features (interrupts, privileged registers).

### Protect Your Data

You can secure data both in flash and SRAM using dedicated macros:

```C
/* create private global constant */
UVISOR_SECURE_CONST char g_password[] = "password";

/* create private global variable */
UVISOR_SECURE_BSS int g_counter;
```

You may use these macros as many times as you want, but to increase performance and readability we suggest to group data in structs (one for flash, one for SRAM) and then secure the whole struct at once. It will be much faster for the uVisor to then ensure protection of a box context.

### Configure Your Secure Box

Once restricted data has been made secure, you need to configure the secure box itself. Specify its name and provide an Access Control List (ACL) indicating the data and peripherals for which you want to ensure restricted and exclusive access. You can also specify the size of the protected stack for the secure box.

The access control list can be defined as follows:

```C
/* create ACLs for the module */
static const UvBoxAclItem g_box_acl[] = {
    {&g_password, sizeof(g_data), UVISOR_TACL_SECURE_CONST}, /* some data */
    {&g_counter,  sizeof(g_data), UVISOR_TACL_SECURE_BSS},   /* some data */
    {UART0,       sizeof(*UART0), UVISOR_TACL_PERIPHERAL},   /* some devices */
};
```

And the secure box can finally be configured:

```C
/* required stack size */
#define BOX_STACK_SIZE 0x100

/* configure secure box */
UVISOR_BOX_CONFIG(my_box_name, g_box_acl, BOX_STACK_SIZE);
```

Please note that configuring a secure box in a yotta module does not mean that the whole module itself is a secure box. A secure box is a domain in which several restrictions apply, so choose carefully what to include in it. All peripherals, constants, variables and object instantiations that are not explicitly included in the secure box through the mechanism just described are automatically included in the context of the main box, unprotected from the rest of the code.

### Secure Function Call

The default code execution, like access to data and peripherals, happens in the context of the main box. But if you attempt to use a function from the main box in the protected domain of your secure box, the function's execution will fail when it attempts to access a resource that is reserved for the secure context. You must therefore use a secure gateway when calling the function, to give the function the relevant secure domain's context.

We suggest you create a wrapper function that does the actual secure function call:

```C
/* the box is configured here */
...

/* the actual function */
extern "C" uint32_t __secure_sum(uint32_t op1, uint32_t op2)
{
    return op1 + op2;
}

/* the gateway to the secure function */
uint32_t secure_sum(uint32_t op1, uint32_t op2)
{
    return secure_gateway(my_box_name, __secure_sum, op1, op2);
}
```

The secure gateway may be called with a maximum of four 32bit arguments.

### Low Level APIs

Access to some system registers or features is only possible in privileged mode, so low level APIs are provided to interact with the uVisor to perform those accesses. Currently, only interrupt management is supported.

#### Interrupt Management

A box can register for interrupts and gain exclusive access to the corresponding IRQn slot, so that it is the only one in charge of modifying it. To register an interrupt routine, you have to set a handler for it and enable the corresponding IRQn slot, as you would do with regular NVIC registers:

```C

#define MY_HANDLER_IRQn 42

void my_handler(void)
{
    ...
}

int some_function(void)
{
    ...

    uvisor_set_isr(MY_HANDLER_IRQn, (uint32_t) &my_handler);
    uvisor_enable_irq(MY_HANDLER_IRQn);

    ...
}
```

After registration of an interrupt, its IRQn is bounded to the box that registered it.

Please note:

1. Currently, interrupt ownership is exclusive, meaning that multiple boxes cannot register for the same interrupt. 
2. Registration works on a first-come-first-served basis. 
3. Execution of an interrupt handler is always handled in unprivileged mode in the context of the box that registered for it.

## Even More Documentation?

For detailed documentation of all available macros and APIs, please refer to the [APIs documentation](DOCUMENTATION.md).

## Limitations

For this release, uvisor-lib comes with the following restrictions:

- It is disabled by default. Use `UVISOR_SET_MODE(2);` in the top level yotta executable to enable it.
