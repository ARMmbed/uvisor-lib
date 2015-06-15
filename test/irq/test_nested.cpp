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
#include "mbed/mbed.h"
#include "mbed/test_env.h"
#include "mbed-cpputest/CppUTest/TestHarness.h"
#include "uvisor-lib/uvisor-lib.h"
#include "common.h"

#define TEST1_IRQn 42
#define TEST1_PRIO 2
#define TEST1_VAL  1

#define TEST2_IRQn 43
#define TEST2_PRIO 1
#define TEST2_VAL  2

#define DFLT_PRIO 1

/* lowest priority ISR */
void test1_handler(void)
{
    g_flag = TEST1_VAL;

    /* trigger TEST2_IRQn: expected to be served due to priorities */
    uvisor_irq_pending_set(TEST2_IRQn);
    __DSB();
    __ISB();
    CHECK_EQUAL(TEST2_VAL, g_flag);
}

/* highest priority ISR: expected to be triggered only from inside the ISR for
 * TEST1_IRQn */
void test2_handler(void)
{
    /* check that this IRQn was actually served from within TEST1_IRQn */
    CHECK_EQUAL(TEST1_VAL, g_flag);

    /* try to trigger TEST1_IRQn from within here: expected not to be triggered
     * due to priorities */
    uvisor_irq_pending_set(TEST1_IRQn);
    __DSB();
    __ISB();
    CHECK_EQUAL(TEST1_VAL, g_flag);
    uvisor_irq_pending_clr(TEST1_IRQn);

    g_flag = TEST2_VAL;
}

TEST_GROUP(IRQTestNested)
{
};

TEST(IRQTestNested, box0_irq_nested_1)
{
    g_flag = 0;

    /* set ISRs */
    uvisor_isr_set(TEST1_IRQn, (uint32_t) &test1_handler, 0);
    uvisor_isr_set(TEST2_IRQn, (uint32_t) &test2_handler, 0);

    /* set priorities: TEST1_IRQn lowest */
    uvisor_irq_priority_set(TEST1_IRQn, TEST1_PRIO);
    uvisor_irq_priority_set(TEST2_IRQn, TEST2_PRIO);

    /* enable both IRQs and trigger TEST1_IRQn: in the end we expect TEST2_IRQn
     * is triggered in turn from within TEST1_IRQn */
    uvisor_irq_pending_clr(TEST1_IRQn);
    uvisor_irq_pending_clr(TEST2_IRQn);
    uvisor_irq_enable(TEST1_IRQn);
    uvisor_irq_enable(TEST2_IRQn);
    uvisor_irq_pending_set(TEST1_IRQn);
    __DSB();
    __ISB();
    CHECK_EQUAL(TEST2_VAL, g_flag);

    /* disable and clear both IRQn */
    uvisor_irq_disable(TEST1_IRQn);
    uvisor_irq_disable(TEST2_IRQn);
    uvisor_irq_pending_clr(TEST1_IRQn);
    uvisor_irq_pending_clr(TEST2_IRQn);
}
