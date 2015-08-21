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
#include "boxN_acl.h"
#include "box1.h"

/* create ACLs for secret data section */
BOXN_ACL(g_box1_acl);

/* configure secure box compartnent */
UVISOR_BOX_CONFIG(box1, g_box1_acl, UVISOR_BOX_STACK_SIZE);

/* box secure function calls: for box 1 we overload the secure function call to
 * test all possible arguments passing */

uint32_t box1_test_function(void)
{
    return secure_gateway(box1, __test_function0);
}

uint32_t box1_test_function(uint32_t a1)
{
    return secure_gateway(box1, __test_function1, a1);
}

uint32_t box1_test_function(uint32_t a1,
                            uint32_t a2)
{
    return secure_gateway(box1, __test_function2, a1, a2);
}

uint32_t box1_test_function(uint32_t a1,
                            uint32_t a2,
                            uint32_t a3)
{
    return secure_gateway(box1, __test_function3, a1, a2, a3);
}

uint32_t box1_test_function(uint32_t a1,
                            uint32_t a2,
                            uint32_t a3,
                            uint32_t a4)
{
    return secure_gateway(box1, __test_function4, a1, a2, a3, a4);
}
