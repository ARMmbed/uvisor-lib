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

#define TEST_IRQn 42
#define TEST_PRIO 2
#define TEST_VAL  1

#define DFLT_PRIO 1

void test_handler(void)
{
    g_flag = TEST_VAL;
}

TEST_GROUP(IRQTestBasic)
{
};

TEST(IRQTestBasic, box0_isr_set_get)
{
    uint32_t hdlr;

    /* set ISR and check if it has been correctly set */
    uvisor_isr_set(TEST_IRQn, (uint32_t) &test_handler, 0);
    hdlr = uvisor_isr_get(TEST_IRQn);
    CHECK_EQUAL((uint32_t) &test_handler, hdlr);
}

TEST(IRQTestBasic, box0_irq_pending_clr_set_get)
{
    uint32_t pending;

    /* disable IRQ to make sure it does not get active */
    uvisor_irq_disable(TEST_IRQn);

    /* clear pending IRQn, if any, and check clearing */
    uvisor_irq_pending_clr(TEST_IRQn);
    pending = uvisor_irq_pending_get(TEST_IRQn);
    CHECK_EQUAL(0, pending);

    g_flag = 0;

    /* set pending IRQn and check that its ISR has not been executed */
    uvisor_irq_pending_set(TEST_IRQn);
    __DSB();
    __ISB();
    CHECK_EQUAL(0, g_flag);

    /* check the pending status and clear it  */
    pending = uvisor_irq_pending_get(TEST_IRQn);
    CHECK_EQUAL(1, pending);
    uvisor_irq_pending_clr(TEST_IRQn);
}

TEST(IRQTestBasic, box0_irq_ena_dis)
{
    g_flag = 0;

    /* configure the ISR and clear IRQn */
    uvisor_isr_set(TEST_IRQn, (uint32_t) &test_handler, 0);
    uvisor_irq_pending_clr(TEST_IRQn);

    /* enable and trigger IRQn */
    uvisor_irq_enable(TEST_IRQn);
    uvisor_irq_pending_set(TEST_IRQn);
    __DSB();
    __ISB();
    CHECK_EQUAL(TEST_VAL, g_flag);

    g_flag = 0;

    /* disable IRQn and check it does not get triggered again */
    uvisor_irq_disable(TEST_IRQn);
    uvisor_irq_pending_set(TEST_IRQn);
    __DSB();
    __ISB();
    CHECK_EQUAL(0, g_flag);
    uvisor_irq_pending_clr(TEST_IRQn);
}

TEST(IRQTestBasic, box0_irq_priority_set_get)
{
    uint32_t priority;

    /* set priority and check it has been set correctly */
    uvisor_irq_priority_set(TEST_IRQn, TEST_PRIO);
    priority = uvisor_irq_priority_get(TEST_IRQn);
    CHECK_EQUAL(TEST_PRIO, priority);

    /* restore default priority */
    uvisor_irq_priority_set(TEST_IRQn, DFLT_PRIO);
}
