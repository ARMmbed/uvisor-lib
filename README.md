# uvisor-lib
uvisor-lib is a yotta module that provides a secure environment for modular
applications on mbed and the APIs to interact with it.

It consists of two main components:

1. the uVisor, which sets up a protected environment where isolated
   compartments are granted finely tuned permissions;
2. APIs to interact with the uVisor.

If you are building your application with yotta on a supported platform, then
you will be already running on top of the uVisor. At this stage of
development, though, it is disabled by default.

To learn more about the uVisor refer to its
[documentation](https://github.com/ARMmbed/uvisor).

Supported platforms:
- Freescale FRDM-K64F board (GCC ARM Embedded toolchain)

The current release version is 0.1.2.

## HowTo
To access the uvisor-lib features:

- Add the module as a dependency to your yotta component in its `module.json`
  file:
```json
  "dependencies": {
    "uvisor-lib": "ARMmbed/uvisor-lib#~0.1.2",
    "mbed": "ARMmbed/mbed-sdk#3.0.0-alpha1",
    "other_dep": "link/to/other_dep"
  },
```
- Include its header file in your source code
```c
#include <uvisor-lib/uvisor-lib.h>
```

## Interfacing the uVisor

### APIs

Under construction.

### Pre-processor macros

```C
UVISOR_BOX_CONFIG(const UvBoxAclItem *module_acl_list,
                  uint32_t module_stack_size)
```

<table>
  <tr>
    <td>Description</td>
    <td colspan="3">Defines a configuration table for the module in Flash</td>
  </tr>
  <tr>
    <td>Type</td>
    <td colspan="2">C/C++ pre-processor macro (object declaration)</td>
  </tr>
  <tr>
    <td rowspan="2">Parameters</td>
    <td><pre>const UvBoxAclItem *module_acl_list<code></td>
    <td>List of ACLs for the module</td>
  </tr>
  <tr>
    <td><pre>uint32_t module_stack_size<code></td>
    <td>Required stack size</td>
  </tr>
</table>

Example:
```C
/* required stack size */
#define MODULE_STACK_SIZE 0x100

/* create ACLs for the module */
static const UvBoxAclItem g_box_acl[] = {
    {&g_data, sizeof(g_data), 0}, /* some data */
    {UART0,   sizeof(*UART0), 0}, /* some devices */
};

/* configure secure box compartment */
UVISOR_BOX_CONFIG(g_box_acl, MODULE_STACK_SIZE);
```

---
```C
UVISOR_SECURE_CONST var_type var_name = var_value
```

<table>
  <tr>
    <td>Description</td>
    <td colspan="2">A constant object is made private for the module</td>
  </tr>
  <tr>
    <td>Type</td>
    <td colspan="3">C/C++ pre-processor macro (attribute)</td>
  </tr>
</table>

Example:
```C
/* create private global constant */
UVISOR_SECURE_CONST char g_password[] = "password";
```

---
```C
UVISOR_SECURE_BSS var_type var_name [= var_value]
```

<table>
  <tr>
    <td>Description</td>
    <td colspan="2">A global variable is made private for the module</td>
  </tr>
  <tr>
    <td>Type</td>
    <td colspan="3">C/C++ pre-processor macro (attribute)</td>
  </tr>
</table>

Example:
```C
/* create private global variable */
UVISOR_SECURE_BSS char g_password[];
```

---
```C
UVISOR_SET_MODE(int uvisor_mode);
```

<table>
  <tr>
    <td>Description</td>
    <td colspan="2">[temporary] Set mode for the uVisor</td>
  </tr>
  <tr>
    <td>Type</td>
    <td colspan="3">C/C++ pre-processor macro (object declaration)</td>
  </tr>
  <tr>
    <td rowspan="3">Parameters</td>
    <td rowspan="3"><pre>int uvisor_mode<code></td>
    <td>0 = disabled [default]</td>
  </tr>
    <tr>
    <td>1 = permissive [n.a.]</td>
  </tr>
  <tr>
    <td>2 = enabled [currently same as permissive]</td>
  </tr>
</table>

Example:
```C
#include <uvisor-lib/uvisor-lib.h>

/* set uvisor mode (enable) */
UVISOR_SET_MODE(2);
```

### Type definitions

```C
typedef uint32_t UvBoxAcl;    /* permssions mask (under construction) */
```

---

```C
typedef struct
{
    const volatile void* start; /* start address of protected area */
    uint32_t length;            /* size of protected area */
    UvBoxAcl acl;               /* permissions for the protected area */
} UvBoxAclItem;
```

## Limitations

For this release, uvisor-lib comes with the following restrictions:

- it is disabled by default. Use `UVISOR_SET_MODE(2);` to enable it;
- it sets up the protected environment but does not enforce security (accesses
  are unprevented);
- it can logically manage only 1 secure box.

