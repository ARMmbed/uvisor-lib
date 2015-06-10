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

#define TEST1_IRQn 42
#define TEST1_VAL  1

#define TEST2_IRQn 43
#define TEST2_VAL  2

volatile int g_flag;

/* lowest priority ISR */
void test1_handler(void)
{
    printf("TEST1_IRQn served\n");

    g_flag = TEST1_VAL;

    /* trigger TEST2_IRQn: expected to be served due to priorities */
    printf("Triggering TEST2_IRQn from TEST1_IRQn... ");
    uvisor_irq_pending_set(TEST2_IRQn);
    __DSB();
    __ISB();
    if(g_flag != TEST2_VAL)
    {
        MBED_HOSTTEST_RESULT(0);
    }
}

/* highest priority ISR: expected to be triggered only from inside the ISR for
 * TEST1_IRQn */
void test2_handler(void)
{
    printf("TEST2_IRQn served\n");

    /* check that this IRQn was actually served from within TEST1_IRQn */
    if(g_flag != TEST1_VAL)
    {
        MBED_HOSTTEST_RESULT(0);
    }
    g_flag = TEST2_VAL;

    /* try to trigger TEST1_IRQn from within here: expected not to be triggered
     * due to priorities */
    printf("Triggering TEST1_IRQn from TEST2_IRQn... ");
    uvisor_irq_pending_set(TEST1_IRQn);
    __DSB();
    __ISB();
    if(g_flag != TEST2_VAL)
    {
        MBED_HOSTTEST_RESULT(0);
    }
    uvisor_irq_pending_clr(TEST1_IRQn);
    printf("Not served [OK] [TEST1_IRQn cleared]\n");
}

int main(void)
{
    MBED_HOSTTEST_TIMEOUT(5);
    MBED_HOSTTEST_SELECT(default);
    MBED_HOSTTEST_DESCRIPTION(IRQ_NESTED);
    MBED_HOSTTEST_START("UVISOR_2");

    g_flag = 0;

    /* set the 2 ISRs */
    uvisor_isr_set(TEST1_IRQn, (uint32_t) &test1_handler, 0);
    uvisor_isr_set(TEST2_IRQn, (uint32_t) &test2_handler, 0);
    if((uvisor_isr_get(TEST1_IRQn) != (uint32_t) &test1_handler) ||
       (uvisor_isr_get(TEST2_IRQn) != (uint32_t) &test2_handler))
    {
        MBED_HOSTTEST_RESULT(0);
    }
    printf("TEST1_IRQn and TEST2_IRQn registered\n");

    /* set priorities: TEST1_IRQn lowest */
    uvisor_irq_priority_set(TEST1_IRQn, 2);
    uvisor_irq_priority_set(TEST2_IRQn, 1);
    if((uvisor_irq_priority_get(TEST1_IRQn) != 2) ||
       (uvisor_irq_priority_get(TEST2_IRQn) != 1))
    {
        MBED_HOSTTEST_RESULT(0);
    }
    printf("TEST1_IRQn and TEST2_IRQn priorities set\n");

    /* enable both IRQs and trigger TEST1_IRQn */
    uvisor_irq_pending_clr(TEST1_IRQn);
    uvisor_irq_pending_clr(TEST2_IRQn);
    uvisor_irq_enable(TEST1_IRQn);
    uvisor_irq_enable(TEST2_IRQn);
    printf("TEST1_IRQn and TEST2_IRQn enabled\n");
    printf("Triggering TEST1_IRQn................... ");
    uvisor_irq_pending_set(TEST1_IRQn);
    __DSB();
    __ISB();
    if(g_flag != TEST2_VAL)
    {
        MBED_HOSTTEST_RESULT(0);
    }

    /* disable all IRQ */
    uvisor_irq_disable(TEST1_IRQn);
    uvisor_irq_disable(TEST2_IRQn);
    printf("TEST1_IRQn and TEST2_IRQn disabled\n");

    /* if the test got here, everything is fine */
    MBED_HOSTTEST_RESULT(1);
}
