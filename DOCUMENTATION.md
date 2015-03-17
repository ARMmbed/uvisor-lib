# uvisor-lib API documentation

Here you can find detailed documentation for:

1. [Configuration Macros](#configuration-macros), to configure a secure box and protect data and
   peripherals;
2. [Secure Function Call](#secure-function-call), to execute code in the context of a secure box
3. [Low Level APIs](#low-level-api), to access uVisor functions that are not available to
   unprivileged code (interrupts, restricted system registers).
4. [Type Definitions](#type-definitions)

## Configuration Macros

```C
UVISOR_SECURE_CONST var_type var_name = var_value
```

<table>
  <tr>
    <td>Description</td>
    <td colspan="2">Data in Flash is made private for the module</td>
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
    <td colspan="2">Data in the BSS (SRAM) is made private for the module</td>
  </tr>
  <tr>
    <td>Type</td>
    <td colspan="3">C/C++ pre-processor macro (attribute)</td>
  </tr>
</table>

Example:
```C
/* create private global variable */
UVISOR_SECURE_BSS char g_counter[];
```

---

```C
UVISOR_BOX_CONFIG(box_name
                  const UvBoxAclItem *module_acl_list,
                  uint32_t module_stack_size)
```

<table>
  <tr>
    <td>Description</td>
    <td colspan="3">Secure box configuration</td>
  </tr>
  <tr>
    <td>Type</td>
    <td colspan="2">C/C++ pre-processor macro (object declaration)</td>
  </tr>
  <tr>
    <td rowspan="3">Parameters</td>
    <td><pre>box_name<code></td>
    <td>Secure box name</td>
  </tr>
  <tr>
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
#define BOX_STACK_SIZE 0x100

/* create ACLs for the module */
static const UvBoxAclItem g_box_acl[] = {
    {&g_password, sizeof(g_data), UVISOR_TACL_SECURE_CONST}, /* some data */
    {&g_counter,  sizeof(g_data), UVISOR_TACL_SECURE_DATA},  /* some data */
    {UART0,       sizeof(*UART0), UVISOR_TACL_PERIPHERAL},   /* some devices */
};

/* configure secure box compartment */
UVISOR_BOX_CONFIG(my_box_name, g_box_acl, BOX_STACK_SIZE);
```

---

Note: this macro is only needed temporarily (uVisor disabled by default) and
will be removed in the future.

Note: this macro must be used only once in the top level yotta executable.

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
    <td>1 = permissive [n.a.] [currently same as enabled]</td>
  </tr>
  <tr>
    <td>2 = enabled</td>
  </tr>
</table>

Example:
```C
#include <uvisor-lib/uvisor-lib.h>

/* set uvisor mode (enable) */
UVISOR_SET_MODE(2);
```

## Secure Function Call

```C
uint32_t secure_gateway(box_name,
                        uint32_t target_fn,
                        uint32_t a0,
                        uint32_t a1,
                        uint32_t a2,
                        uint32_t a3)
```

<table>
  <tr>
    <td>Description</td>
    <td colspan="2">Call a function using the context of a secure box</td>
  </tr>
  <tr>
    <td>Type</td>
    <td colspan="3">C/C++ pre-processor macro (pseudo-function)</td>
  </tr>
  <tr>
    <td rowspan="6">Parameters</td>
    <td><pre>box_name<code></td>
    <td>Secure box name</td>
  </tr>
  <tr>
    <td><pre>uint32_t target_fn<code></td>
    <td>Function to execute in the secure context</td>
  </tr>
  <tr>
    <td><pre>uint32_t a0<code></td>
    <td>First 32bit argument</td>
  </tr>
  <tr>
    <td><pre>uint32_t a1<code></td>
    <td>Third 32bit argument</td>
  </tr>
  <tr>
    <td><pre>uint32_t a2<code></td>
    <td>Second 32bit argument</td>
  </tr>
  <tr>
    <td><pre>uint32_t a3<code></td>
    <td>Forth 32bit argument</td>
  </tr>
</table>

Example:
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

## Low Level APIs

Currently the following low level operations are permitted:

1. Interrupt management
2. Bitband access (temporary)

### Interrupt management

```C
void uvisor_set_isr(uint32_t irqn, uint32_t vector, uint32_t flag)
```

<table>
  <tr>
    <td>Description</td>
    <td colspan="2">Register an ISR to the currently active box</td>
  </tr>
    <td>Notes</td>
    <td colspan="3">Equivalent to <pre>NVIC_SetVector(irqn, vector)<code></td>
  </tr>
  <tr>
    <td rowspan="3">Parameters</td>
    <td><pre>uint32_t irqn<code></td>
    <td>IRQn</td>
  </tr>
  <tr>
    <td><pre>uint32_t vector<code></td>
    <td>Interrupt handler</td>
  </tr>
  <tr>
    <td><pre>uint32_t flag<code></td>
    <td>Permission flag (currently not implemented)</td>
  </tr>
</table>

---

```C
void uvisor_let_isr(uint32_t irqn)
```

<table>
  <tr>
    <td>Description</td>
    <td colspan="2">De-register an ISR for the currently active box</td>
  </tr>
  <tr>
    <td rowspan="1">Parameters</td>
    <td><pre>uint32_t irqn<code></td>
    <td>IRQn</td>
  </tr>
</table>

---

```C
uint32_t uvisor_get_isr(uint32_t irqn)
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
uint32_t uvisor_ena_irq(uint32_t irqn)
```

<table>
  <tr>
    <td>Description</td>
    <td colspan="2">Enable IRQn for the currently active box</td>
  </tr>
  <tr>
    <td>Notes</td>
    <td colspan="2">Equivalent to <pre>NVIC_EnableIRQ(irqn)<code></td>
  </tr>
  <tr>
    <td rowspan="1">Parameters</td>
    <td><pre>uint32_t irqn<code></td>
    <td>IRQn</td>
  </tr>
</table>

---

```C
uint32_t uvisor_dis_irq(uint32_t irqn)
```

<table>
  <tr>
    <td>Description</td>
    <td colspan="2">Disable IRQn for the currently active box</td>
  </tr>
  <tr>
    <td>Notes</td>
    <td colspan="2">Equivalent to <pre>NVIC_DisableIRQ(irqn)<code></td>
  </tr>
  <tr>
    <td rowspan="1">Parameters</td>
    <td><pre>uint32_t irqn<code></td>
    <td>IRQn</td>
  </tr>
</table>

---

```C
void uvisor_set_ena_isr(uint32_t irqn, uint32_t vector, uint32_t flag)
```

<table>
  <tr>
    <td>Description</td>
    <td colspan="2">Register an ISR to the currently active box and then enable
                    its corresponding IRQn</td>
  </tr>
    <td>Notes</td>
    <td colspan="3">Equivalent to <pre>NVIC_SetVector(irqn, vector); NVIC_EnableIRQ(irqn);<code></td>
  </tr>
  <tr>
    <td rowspan="3">Parameters</td>
    <td><pre>uint32_t irqn<code></td>
    <td>IRQn</td>
  </tr>
  <tr>
    <td><pre>uint32_t vector<code></td>
    <td>Interrupt handler</td>
  </tr>
  <tr>
    <td><pre>uint32_t flag<code></td>
    <td>Permission flag (currently not implemented)</td>
  </tr>
</table>

---

```C
void uvisor_dis_let_isr(uint32_t irqn)
```

<table>
  <tr>
    <td>Description</td>
    <td colspan="2">Disable IRQ for the currently active box and then
                    de-register the corresponding ISR</td>
  </tr>
  </tr>
    <td>Notes</td>
    <td colspan="3">Equivalent to <pre>NVIC_DisableIRQ(irqn)<code></td>
  </tr>
  <tr>
    <td rowspan="1">Parameters</td>
    <td><pre>uint32_t irqn<code></td>
    <td>IRQn</td>
  </tr>
</table>

### Bitband access (temporary)

```C
void uvisor_write_bitband(uint32_t addr, int32_t val)
```

<table>
  <tr>
    <td>Description</td>
    <td colspan="2">[temporary] Write to a bitband address when unprivileged
                    access is not permitted</td>
  </tr>
  <tr>
    <td rowspan="2">Parameters</td>
    <td><pre>uint32_t addr<code></td>
    <td>Bitband address</td>
  </tr>
  <tr>
    <td><pre>uint32_t val<code></td>
    <td>Value to write at bitband address</td>
  </tr>
</table>

## Type Definitions

```C
typedef uint32_t UvisroBoxAcl;    /* permssions mask */
```

---

```C
typedef struct
{
    const volatile void* start; /* start address of protected area */
    uint32_t length;            /* size of protected area */
    UvisorBoxAcl acl;           /* permissions for the protected area */
} UvisorBoxAclItem;
```
