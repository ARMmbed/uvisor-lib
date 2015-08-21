/*
 * Copyright (c) 2013-2015, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
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
    vIRQ_SetVectorX(TEST1_IRQn, (uint32_t) &test1_handler, 0);
    vIRQ_SetVectorX(TEST2_IRQn, (uint32_t) &test2_handler, 0);

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
    vIRQ_SetVectorX(TEST1_IRQn, 0, 0);
    vIRQ_SetVectorX(TEST2_IRQn, 0, 0);

}
