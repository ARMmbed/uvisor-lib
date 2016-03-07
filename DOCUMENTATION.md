# The uVisor API Documentation

Here you can find detailed documentation for:

1. [Configuration macros](#configuration-macros), to configure a secure box and protect data and peripherals.
2. [Secure function calls](#secure-function-call), to execute code in the context of a secure box.
3. [Low level APIs](#low-level-apis), to access uVisor functions that are not available to unprivileged code (interrupts, restricted system registers).
4. [Type definitions](#type-definitions).
5. [Error codes](#error-codes).

## Configuration macros

```C
UVISOR_BOX_CONFIG(box_name
                  const UvBoxAclItem *module_acl_list,
                  uint32_t module_stack_size,
                  [struct __your_context])
```

<table>
  <tr>
    <td>Description</td>
    <td colspan="2">Secure box configuration</td>
  </tr>
  <tr>
    <td>Type</td>
    <td colspan="2">C/C++ pre-processor macro (pseudo-function)</td>
  </tr>
  <tr>
    <td rowspan="4">Parameters</td>
    <td><pre>box_name<code></td>
    <td>Secure box name</td>
  </tr>
  <tr>
    <td><pre>const UvBoxAclItem *module_acl_list<code></td>
    <td>List of ACLs for the module</td>
  </tr>
  <tr>
    <td><pre>uint32_t module_stack_size<code></td>
    <td>Required stack size for the secure box</td>
  </tr>
  <tr>
    <td><pre>struct __your_context<code></td>
    <td>[optional] Type definition of the struct hosting the box context data</td>
  </tr>
</table>

Example:
```C
#include "uvisor-lib/uvisor-lib.h"

/* Required stack size */
#define BOX_STACK_SIZE 0x100

/* Define the box context. */
typedef struct {
    uint8_t secret[SECRET_SIZE];
    bool initialized;
    State_t current_state
} BoxContext;

/* Create the ACL list for the module. */
static const UvBoxAclItem g_box_acl[] = {
    {PORTB,  sizeof(*PORTB),  UVISOR_TACLDEF_PERIPH},
    {RTC,    sizeof(*RTC),    UVISOR_TACLDEF_PERIPH},
    {LPTMR0, sizeof(*LPTMR0), UVISOR_TACLDEF_PERIPH},
};

/* Configure the secure box compartment. */
UVISOR_BOX_CONFIG(my_box_name, g_box_acl, BOX_STACK_SIZE, BoxContext);
```

---

```C
UVISOR_SET_MODE(uvisor_mode);
```

<table>
  <tr>
    <td>Description</td>
    <td colspan="2">[temporary] Set mode for the uVisor</td>
  </tr>
  <tr>
    <td>Type</td>
    <td colspan="2">C/C++ pre-processor macro (object declaration)</td>
  </tr>
  <tr>
    <td rowspan="3">Parameters</td>
    <td rowspan="3"><pre>uvisor_mode<code></td>
    <td><pre>UVISOR_DISABLED<code> = disabled [default]</td>
  </tr>
    <tr>
    <td><pre>UVISOR_PERMISSIVE<code> = permissive [currently n.a.]</td>
  </tr>
  <tr>
    <td><pre>UVISOR_ENABLED<code> = enabled</td>
  </tr>
</table>

Example:
```C
#include "uvisor-lib/uvisor-lib.h"

/* Set the uVisor mode. */
UVISOR_SET_MODE(UVISOR_ENABLED);
```

**Note:**

1. This macro is only needed temporarily (uVisor disabled by default) and will be removed in the future.

2. This macro must be used only once in the top level yotta executable.

---

```C
UVISOR_SET_MODE_ACL(uvisor_mode, const UvBoxAcl *main_box_acl_list);
```

<table>
  <tr>
    <td>Description</td>
    <td colspan="2">[temporary] Set mode for the uVisor and provide background
                    ACLs for the main box
    </td>
  </tr>
  <tr>
    <td>Type</td>
    <td colspan="2">C/C++ pre-processor macro (object declaration)</td>
  </tr>
  <tr>
    <td rowspan="4">Parameters</td>
    <td rowspan="3"><pre>uvisor_mode<code></td>
    <td><pre>UVISOR_DISABLED<code> = disabled [default]</td>
  </tr>
    <tr>
    <td><pre>UVISOR_PERMISSIVE<code> = permissive [currently n.a.]</td>
  </tr>
  <tr>
    <td><pre>UVISOR_ENABLED<code> = enabled</td>
  <tr>
    <td><pre>const UvBoxAclItem *main_box_acl_list<code></td>
    <td>List of ACLs for the main box (background ACLs)</td>
  </tr>
</table>

Example:
```C
#include "uvisor-lib/uvisor-lib.h"

/* Create background ACLs for the main box. */
static const UvBoxAclItem g_background_acl[] = {
    {UART0,       sizeof(*UART0), UVISOR_TACL_PERIPHERAL},
    {UART1,       sizeof(*UART1), UVISOR_TACL_PERIPHERAL},
    {PIT,         sizeof(*PIT),   UVISOR_TACL_PERIPHERAL},
};

/* Set the uVisor mode. */
UVISOR_SET_MODE_ACL(UVISOR_ENABLED, g_background_acl);
```

**Note:**

1. This macro is only needed temporarily (uVisor disabled by default) and will be removed in the future.

2. This macro must be used only once in the top level yotta executable.

## Secure function call

```C
uint32_t secure_gateway(box_name, uint32_t target_fn, ...)
```

<table>
  <tr>
    <td>Description</td>
    <td colspan="2">Call a function using the context of a secure box</td>
  </tr>
  <tr>
    <td>Type</td>
    <td colspan="2">C/C++ pre-processor macro (pseudo-function)</td>
  </tr>
  <tr>
    <td>Return value</td>
    <td colspan="2">[optional] A maximum of one 32bit value</td>
  </tr>
  <tr>
    <td rowspan="3">Parameters</td>
    <td><pre>box_name<code></td>
    <td>Secure box name</td>
  </tr>
  <tr>
    <td><pre>uint32_t target_fn<code></td>
    <td>Function to execute in the secure context</td>
  </tr>
  <tr>
    <td><pre>...<code></td>
    <td>A maximum of four 32bit arguments</td>
  </tr>
</table>

Example:
```C
/* The box is configured here. */
...

/* The actual function implementation */
extern "C" uint32_t __secure_sum(uint32_t op1, uint32_t op2)
{
    return op1 + op2;op3 + op4;
}

/* The entry point to the actual function implementation */
uint32_t secure_sum(uint32_t op1, uint32_t op2)
{
    return secure_gateway(my_box_name, __secure_sum, op1, op2)
}
```

## Low-level APIs

Currently the following low level operations are permitted:

1. Interrupt management.

### Interrupt management

```C
void vIRQ_SetVectorX(uint32_t irqn, uint32_t vector, uint32_t flag)
```

<table>
  <tr>
    <td>Description</td>
    <td colspan="2">Register an ISR to the currently active box, with the ability to specify permissions associated with the IRQn slot</td>
  <tr>
    <td rowspan="3">Parameters</td>
    <td><pre>uint32_t irqn<code></td>
    <td>IRQn</td>
  </tr>
  <tr>
    <td><pre>uint32_t vector<code></td>
    <td>Interrupt handler; if 0 the IRQn slot is de-registered for the current
        box</td>
  </tr>
  <tr>
    <td><pre>uint32_t flag<code></td>
    <td>Permission flag (currently not implemented)</td>
  </tr>
</table>

---

```C
void vIRQ_SetVector(uint32_t irqn, uint32_t vector)
```

<table>
  <tr>
    <td>Description</td>
    <td colspan="2">Register an ISR to the currently active box</td>
  <tr>
    <td rowspan="2">Parameters</td>
    <td><pre>uint32_t irqn<code></td>
    <td>IRQn</td>
  </tr>
  <tr>
    <td><pre>uint32_t vector<code></td>
    <td>Interrupt handler; if 0 the IRQn slot is de-registered for the current
        box</td>
  </tr>
</table>

**Note:**

1. Currently `vIRQ_SetVectorX` and `vIRQ_SetVector` are mapped to the same function because the argument `flag` is not yet used to determine the permissions associated with the IRQn slot

2. `vIRQ_SetVector` is especially kept for backward compatibility with `NVIC_SetVector`

---

```C
uint32_t vIRQ_GetVector(uint32_t irqn)
```

<table>
  <tr>
    <td>Description</td>
    <td colspan="2">Get the ISR registered for IRQn</td>
  </tr>
  <tr>
    <td>Return value</td>
    <td colspan="2">The ISR registered for IRQn, if present; 0 otherwise</td>
  </tr>
  <tr>
    <td rowspan="1">Parameters</td>
    <td><pre>uint32_t irqn<code></td>
    <td>IRQn</td>
  </tr>
</table>

---

```C
void vIRQ_EnableIRQ(uint32_t irqn)
```

<table>
  <tr>
    <td>Description</td>
    <td colspan="2">Enable IRQn for the currently active box</td>
  </tr>
  <tr>
    <td rowspan="1">Parameters</td>
    <td><pre>uint32_t irqn<code></td>
    <td>IRQn</td>
  </tr>
</table>

---

```C
void vIRQ_DisableIRQ(uint32_t irqn)
```

<table>
  <tr>
    <td>Description</td>
    <td colspan="2">Disable IRQn for the currently active box</td>
  </tr>
  <tr>
    <td rowspan="1">Parameters</td>
    <td><pre>uint32_t irqn<code></td>
    <td>IRQn</td>
  </tr>
</table>

---

```C
void vIRQ_ClearPendingIRQ(uint32_t irqn)
```

<table>
  <tr>
    <td>Description</td>
    <td colspan="2">Clear pending status of IRQn</td>
  </tr>
  <tr>
    <td rowspan="2">Parameters</td>
    <td><pre>uint32_t irqn<code></td>
    <td>IRQn</td>
  </tr>
</table>

---

```C
void vIRQ_SetPendingIRQ(uint32_t irqn)
```

<table>
  <tr>
    <td>Description</td>
    <td colspan="2">Set pending status of IRQn</td>
  </tr>
  <tr>
    <td rowspan="2">Parameters</td>
    <td><pre>uint32_t irqn<code></td>
    <td>IRQn</td>
  </tr>
</table>

---

```C
uint32_t vIRQ_GetPendingIRQ(uint32_t irqn)
```

<table>
  <tr>
    <td>Description</td>
    <td colspan="2">Get pending status of IRQn</td>
  </tr>
  <tr>
    <td rowspan="1">Parameters</td>
    <td><pre>uint32_t irqn<code></td>
    <td>IRQn</td>
  </tr>
</table>

---

```C
void vIRQ_SetPriority(uint32_t irqn, uint32_t priority)
```

<table>
  <tr>
    <td>Description</td>
    <td colspan="2">Set priority level of IRQn</td>
  </tr>
  <tr>
    <td rowspan="2">Parameters</td>
    <td><pre>uint32_t irqn<code></td>
    <td>IRQn</td>
  </tr>
  <tr>
    <td><pre>uint32_t priority<code></td>
    <td>Priority level (minimum: 1)</td>
  </tr>
</table>

---

```C
uint32_t vIRQ_GetPriority(uint32_t irqn)
```

<table>
  <tr>
    <td>Description</td>
    <td colspan="2">Get priority level of IRQn</td>
  </tr>
  <tr>
    <td>Return value</td>
    <td colspan="2">The priority level of IRQn, if available; 0 otherwise</td>
  </tr>
  <tr>
    <td rowspan="1">Parameters</td>
    <td><pre>uint32_t irqn<code></td>
    <td>IRQn</td>
  </tr>
</table>

---

```C
int vIRQ_GetLevel(void)
```

<table>
  <tr>
    <td>Description</td>
    <td colspan="2">Get level of currently active IRQn, if any</td>
  </tr>
  <tr>
    <td>Return value</td>
    <td colspan="2">The priority level of the currently active IRQn, if any; -1 otherwise</td>
  </tr>
</table>

## Type definitions

```C
typedef uint32_t UvisroBoxAcl;    /* Permssion mask */
```

---

```C
typedef struct
{
    const volatile void* start;   /* Start address of the protected area */
    uint32_t length;              /* Size of the protected area */
    UvisorBoxAcl acl;             /* Permission mask for the protected area */
} UvisorBoxAclItem;
```
## Error codes

| Error reason          | Error code |
|-----------------------|------------|
| `PERMISSION_DENIED`   | 1          |
| `SANITY_CHECK_FAILED` | 2          |
| `NOT_IMPLEMENTED`     | 3          |
| `NOT_ALLOWED`         | 4          |
| `FAULT_MEMMANAGE`     | 5          |
| `FAULT_BUS`           | 6          |
| `FAULT_USAGE`         | 7          |
| `FAULT_HARD`          | 8          |
| `FAULT_DEBUG`         | 9          |
