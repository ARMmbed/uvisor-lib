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

/* lowest priority ISR */
void test1_handler(void)
{
    g_flag = TEST1_VAL;

    /* trigger TEST2_IRQn: expected to be served due to priorities */
    vIRQ_SetPendingIRQ(TEST2_IRQn);
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
    vIRQ_SetPendingIRQ(TEST1_IRQn);
    __DSB();
    __ISB();
    CHECK_EQUAL(TEST1_VAL, g_flag);
    vIRQ_ClearPendingIRQ(TEST1_IRQn);

    g_flag = TEST2_VAL;
}

TEST_GROUP(IRQTestNested)
{
};

TEST(IRQTestNested, box0_irq_nested1)
{
    g_flag = 0;

    /* set ISRs */
    vIRQ_SetVector(TEST1_IRQn, (uint32_t) &test1_handler, 0);
    vIRQ_SetVector(TEST2_IRQn, (uint32_t) &test2_handler, 0);

    /* set priorities: TEST1_IRQn lowest */
    vIRQ_SetPriority(TEST1_IRQn, TEST1_PRIO);
    vIRQ_SetPriority(TEST2_IRQn, TEST2_PRIO);

    /* enable both IRQs and trigger TEST1_IRQn: in the end we expect TEST2_IRQn
     * is triggered in turn from within TEST1_IRQn */
    vIRQ_ClearPendingIRQ(TEST1_IRQn);
    vIRQ_ClearPendingIRQ(TEST2_IRQn);
    vIRQ_EnableIRQ(TEST1_IRQn);
    vIRQ_EnableIRQ(TEST2_IRQn);
    vIRQ_SetPendingIRQ(TEST1_IRQn);
    __DSB();
    __ISB();
    CHECK_EQUAL(TEST2_VAL, g_flag);

    /* disable and clear both IRQn */
    vIRQ_DisableIRQ(TEST1_IRQn);
    vIRQ_DisableIRQ(TEST2_IRQn);
    vIRQ_ClearPendingIRQ(TEST1_IRQn);
    vIRQ_ClearPendingIRQ(TEST2_IRQn);

    /* release ownership of both ISRs */
    vIRQ_SetVector(TEST1_IRQn, 0, 0);
    vIRQ_SetVector(TEST2_IRQn, 0, 0);

}
