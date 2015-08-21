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

/* TEST FUNCTIONS: these are meant to be executed as the target of a secure
 * gateway; we cannot use overloading because it has to be a uniquely defined
 * C function (limitation from the secure gateway) */

UVISOR_EXTERN uint32_t __test_function0(void)
{
    return TEST_RES0;
}

UVISOR_EXTERN uint32_t __test_function1(uint32_t a1)
{
    return a1;
}

UVISOR_EXTERN uint32_t __test_function2(uint32_t a1,
                                        uint32_t a2)
{
    return a1 + a2;
}

UVISOR_EXTERN uint32_t __test_function3(uint32_t a1,
                                        uint32_t a2,
                                        uint32_t a3)
{
    return a1 + a2 + a3;
}

UVISOR_EXTERN uint32_t __test_function4(uint32_t a1,
                                        uint32_t a2,
                                        uint32_t a3,
                                        uint32_t a4)
{
    return a1 + a2 + a3 + a4;
}
