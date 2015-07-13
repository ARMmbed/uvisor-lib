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
#include "mbed-cpputest/CppUTest/TestHarness.h"
#include "uvisor-lib/uvisor-lib.h"
#include "common.h"

void test_handler(void)
{
    g_flag = TEST1_VAL;
}

TEST_GROUP(IRQTestBasic)
{
};

TEST(IRQTestBasic, box0_isr_set_get)
{
    uint32_t hdlr;

    /* set ISR and check if it has been correctly set */
    vIRQ_SetVector(TEST1_IRQn, (uint32_t) &test_handler, 0);
    hdlr = vIRQ_GetVector(TEST1_IRQn);
    CHECK_EQUAL((uint32_t) &test_handler, hdlr);

    /* release ownership of ISR */
    vIRQ_SetVector(TEST1_IRQn, 0, 0);
}

TEST(IRQTestBasic, box0_irq_pending_clr_set_get)
{
    uint32_t pending;

    /* set ISR but do not enable it */
    vIRQ_SetVector(TEST1_IRQn, (uint32_t) &test_handler, 0);

    g_flag = 0;

    /* set pending IRQn and check that its ISR has not been executed */
    vIRQ_SetPendingIRQ(TEST1_IRQn);
    __DSB();
    __ISB();
    CHECK_EQUAL(0, g_flag);

    /* check the pending status, clear it and check clearing */
    pending = vIRQ_GetPendingIRQ(TEST1_IRQn);
    CHECK_EQUAL(1, pending);
    vIRQ_ClearPendingIRQ(TEST1_IRQn);
    pending = vIRQ_GetPendingIRQ(TEST1_IRQn);
    CHECK_EQUAL(0, pending);

    /* release ownership of ISR */
    vIRQ_SetVector(TEST1_IRQn, 0, 0);
}

TEST(IRQTestBasic, box0_irq_ena_dis)
{
    /* set ISR */
    vIRQ_SetVector(TEST1_IRQn, (uint32_t) &test_handler, 0);

    g_flag = 0;

    /* enable and trigger IRQn */
    vIRQ_EnableIRQ(TEST1_IRQn);
    vIRQ_SetPendingIRQ(TEST1_IRQn);
    __DSB();
    __ISB();
    CHECK_EQUAL(TEST1_VAL, g_flag);

    g_flag = 0;

    /* disable IRQn and check it does not get triggered again */
    vIRQ_DisableIRQ(TEST1_IRQn);
    vIRQ_SetPendingIRQ(TEST1_IRQn);
    __DSB();
    __ISB();
    CHECK_EQUAL(0, g_flag);
    vIRQ_ClearPendingIRQ(TEST1_IRQn);

    /* release ownership of ISR */
    vIRQ_SetVector(TEST1_IRQn, 0, 0);
}

TEST(IRQTestBasic, box0_irq_priority_set_get)
{
    uint32_t priority;

    /* set ISR */
    vIRQ_SetVector(TEST1_IRQn, (uint32_t) &test_handler, 0);

    /* set priority and check it has been set correctly */
    vIRQ_SetPriority(TEST1_IRQn, TEST1_PRIO);
    priority = vIRQ_GetPriority(TEST1_IRQn);
    CHECK_EQUAL(TEST1_PRIO, priority);

    /* reset priority and release ownership of ISR */
    vIRQ_SetPriority(TEST1_IRQn, DFLT_PRIO);
    vIRQ_SetVector(TEST1_IRQn, 0, 0);
}
