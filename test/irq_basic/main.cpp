/***************************************************************
 * This confidential and  proprietary  software may be used only
 * as authorised  by  a licensing  agreement  from  ARM  Limited
 *
 *             (C) COPYRIGHT 2013-2015 ARM Limited
 *                      ALL RIGHTS RESERVED
 *
 *  The entire notice above must be reproduced on all authorised
 *  copies and copies  may only be made to the  extent permitted
 *  by a licensing agreement from ARM Limited.
 *
 ***************************************************************/
#include "mbed/test_env.h"
#include "uvisor-lib/uvisor-lib.h"

/* main box ACLs */
static const UvisorBoxAclItem g_main_acl[] = {
    {MCG,                  sizeof(*MCG),       UVISOR_TACLDEF_PERIPH},
    {UART0,                sizeof(*UART0),     UVISOR_TACLDEF_PERIPH},
    {PIT,                  sizeof(*PIT),       UVISOR_TACLDEF_PERIPH},
    {SIM,                  sizeof(*SIM),       UVISOR_TACLDEF_PERIPH},
    {PORTB,                sizeof(*PORTB),     UVISOR_TACLDEF_PERIPH},
};

/* enable uvisor */
UVISOR_SET_MODE_ACL(2, g_main_acl);

#define TEST_IRQn 42
#define TEST_VAL  1

volatile int g_flag;

void test_handler(void)
{
    printf("TEST_IRQn triggered\n");
    g_flag = TEST_VAL;
}

int main(void)
{
    MBED_HOSTTEST_TIMEOUT(5);
    MBED_HOSTTEST_SELECT(default);
    MBED_HOSTTEST_DESCRIPTION(IRQ_BASIC);
    MBED_HOSTTEST_START("MBED_A1");

    g_flag = 0;

    /* set the ISR */
    uvisor_isr_set(TEST_IRQn, (uint32_t) &test_handler, 0);
    if(uvisor_isr_get(TEST_IRQn) != (uint32_t) &test_handler)
    {
        MBED_HOSTTEST_RESULT(0);
    }
    printf("TEST_IRQn registered\n");

    /* enable and trigger the IRQ*/
    uvisor_irq_pending_clr(TEST_IRQn);
    uvisor_irq_enable(TEST_IRQn);
    printf("TEST_IRQn enabled\n");
    printf("Triggering TEST_IRQn... ");
    uvisor_irq_pending_set(TEST_IRQn);
    __DSB();
    __ISB();
    if(g_flag != TEST_VAL)
    {
        MBED_HOSTTEST_RESULT(0);
    }

    /* disable the IRQ and try to trigger it again */
    uvisor_irq_disable(TEST_IRQn);
    printf("TEST_IRQn disabled\n");
    printf("Triggering TEST_IRQn... ");
    uvisor_irq_pending_clr(TEST_IRQn);
    g_flag = 0;
    uvisor_irq_pending_set(TEST_IRQn);
    __DSB();
    __ISB();
    if(g_flag != 0)
    {
        MBED_HOSTTEST_RESULT(0);
    }
    uvisor_irq_pending_clr(TEST_IRQn);
    printf("Not served [OK] [TEST_IRQn cleared]\n");

    /* if the test got here, everything is fine */
    MBED_HOSTTEST_RESULT(1);
}
