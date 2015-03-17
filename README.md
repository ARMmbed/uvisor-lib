# uvisor-lib
uvisor-lib is a yotta module that provides a secure environment for modular
applications on mbed and the APIs to interact with it.

It consists of two main components:

1. the uVisor binaries, which set up a protected environment where isolated
   compartments are granted finely tuned permissions;
2. APIs to interact with the uVisor.

If you are building your application with yotta on a supported platform, then
you will be already running on top of the uVisor. At this stage of
development, though, it is disabled by default.

To learn more about the uVisor refer to its
[documentation](https://github.com/ARMmbed/uvisor).

Supported platforms:
- Freescale FRDM-K64F board (GCC ARM Embedded toolchain)

The current release version is 0.1.7.

## Introduction

The security model of the uVisor is based on secure boxes. A *secure box* is a
domain for which finely grained permissions are granted, protecting data and
functions from the rest of the code.

By default, each yotta module that is included in the build process of an mbed
application ends up in a unique unsecure box, referred to as the *main box*.
All code and data in the main box share the same level of permissions.

If you need to protect data or function execution you may want to configure a
secure box. Secure boxes all run in unprivileged mode but are granted
protection and isolation from other boxes.

Access to protected resources is only possible when the context of the box they
belong to is active. By default, the active context is the one of the main box.
A special mechanism, called the *secure gateway*, allows you to switch context
to your own secure box. You cannot switch context to a different box from
yours.

The next Sections will describe all these features in deeper detail.

## How to Configure a Secure Box

To configure a secure box:

- Add the module as a dependency in `module.json`:
```
  "dependencies": {
    "uvisor-lib": "ARMmbed/uvisor-lib#~0.1.7",
    "mbed": "ARMmbed/mbed-sdk",
    ...
  },
```

- Include its header file in your source code
```c
#include <uvisor-lib/uvisor-lib.h>
```

- Protect your data with `UVISOR_SECURE_CONST` and `UVISOR_SECURE_BSS` (see
  [here](#protect-your-data))

- Configure your secure box with `UVISOR_BOX_CONFIG(...)` (see
  [here](#configure-the-secure-box))

- Use the secure gateway to ensure critical functions are executed from the
  secure box context (see [here](#secure-function-call))

- Use the uvisor-lib APIs to access restricted features (interrupts, privileged
  registers) (see [here](#low-level-apis))

### Protect Your Data

You can secure data both in Flash and SRAM using dedicated macros:

```C
/* create private global constant */
UVISOR_SECURE_CONST char g_password[] = "password";

/* create private global variable */
UVISOR_SECURE_BSS char g_counter[];
```

You might use these macros as many times as you want, but to increase
performance and readability we suggest to group data in structs (one for Flash,
one for SRAM) and then secure the whole structs singularly. It will be much
faster for the uVisor to then ensure protection of a box context.

### Configure the Secure Box

Once restricted data as been made secure, you need to configure the secure box
itself. To do so you need to specify its name and provide an Access Control
List (ACL) specifying the data and peripherals for which you want to ensure
restricted and exclusive access. You can also specify the size of the protected
stack for the secure box.

The access control list can be defined as follows:

```C
/* create ACLs for the module */
static const UvBoxAclItem g_box_acl[] = {
    {&g_password, sizeof(g_data), UVISOR_TACL_SECURE_CONST}, /* some data */
    {&g_counter,  sizeof(g_data), UVISOR_TACL_SECURE_DATA},  /* some data */
    {UART0,       sizeof(*UART0), UVISOR_TACL_PERIPHERAL},   /* some devices */
};
```

And the secure box finally configured:

```C
/* required stack size */
#define BOX_STACK_SIZE 0x100

/* configure secure box */
UVISOR_BOX_CONFIG(my_box_name, g_box_acl, BOX_STACK_SIZE);
```

Please note that configuring a secure box in a yotta module does not mean that
the whole module itself is a secure box. A secure box is a domain in which
several restrictions apply, so choose carefully what to include in it. All
peripherals, constants, variables and object instantiations that are not
explicitly included in the secure box through the mechanism just described are
automatically included in the context of the main box, unprotected from the
rest of the code.

### Secure Function Call

Similarly to access to data and peripherals, code exectuion happens by default
in the context of the main box. If you want to ensure that a function is
executed in the context of your secure box, you need to use a secure gateway.

The secure gateway transforms your function call in a gateway to the protected
domain of your secure box. Without it, the direct execution of the target
function would fail at some point, accessing resources which are reserved to a
different context.

We suggest to create a wrapper function that does the actual secure function
call:

```C
/* the box is configured here */
...

/* the actual function */
uint32_t __secure_sum(uint32_t op1, uint32_t op2,
                      uint32_t op3, uint32_t op4)
{
    return op1 + op2 + op3 + op4;
}

/* the gateway to the secure function */
uint32_t secure_sum(uint32_t op1, uint32_t op2,
                    uint32_t op3, uint32_t op4)
{
    return secure_gateway(my_box_name, __secure_sum,
                          op1, op2, op3, op4);
}
```

### Low Level APIs

Access to some system registers or features is only possible in privileged
mode, so low level APIs are provided to interact with the uVisor to perform
those accesses. Currently, only interrupt management is supported.

#### Interrupt management

Boxes can register for interrupts and gain exclusive access to the
corresponding IRQn slot, so that they are the only one in charge of modifying
it. To register an interrupt routine, you have to set the handler and enable
the corresponding IRQn, as you would do with regular NVIC registers:

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
    uvisor_ena_irq(MY_HANDLER_IRQn);

    ...
}
```

After registration of an interrupt, its IRQn is bounded to the box that
registered it until ownership is releaed:

```C
    ...
    uvisor_let_isr(MY_HANDLER_IRQn);
    ...
```

An interrupt can be enabled and disabled repeatedly without the need of
registering and de-registering it all the time.

Currently, interrupt ownership is exclusive, meaning that multiple boxes cannot
register for the same interrupt. Registration works on a
first-come-first-served basis. Execution of an interrupt handler is always
handled in unprivileged mode in the context of the box that registered for it.

## Even More Documentation?

For a detailed documentation of all macros and APIs available, please see
[here](DOCUMENTATION.md).

## Limitations

For this release, uvisor-lib comes with the following restrictions:

- it is disabled by default. Use `UVISOR_SET_MODE(2);` in the top level yotta
  executable to enable it.
