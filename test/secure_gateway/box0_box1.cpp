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
#include "box1.h"

/* simple tests to trigger a unique secure gateway from box 0 to box 1 with a
 * variable number of arguments */

TEST_GROUP(SecureGatewayBox0Box1)
{
};

TEST(SecureGatewayBox0Box1, box0_box1_0args)
{
    uint32_t result;

    result = box1_test_function();
    CHECK_EQUAL(TEST_RES0, result);
};

TEST(SecureGatewayBox0Box1, box0_box1_1args)
{
    uint32_t result;

    result = box1_test_function(TEST_ARG1);
    CHECK_EQUAL(TEST_RES1, result);
};

TEST(SecureGatewayBox0Box1, box0_box1_2args)
{
    uint32_t result;

    result = box1_test_function(TEST_ARG1, TEST_ARG2);
    CHECK_EQUAL(TEST_RES2, result);
};

TEST(SecureGatewayBox0Box1, box0_box1_3args)
{
    uint32_t result;

    result = box1_test_function(TEST_ARG1, TEST_ARG2, TEST_ARG3);
    CHECK_EQUAL(TEST_RES3, result);
};

TEST(SecureGatewayBox0Box1, box0_box1_4args)
{
    uint32_t result;

    result = box1_test_function(TEST_ARG1, TEST_ARG2, TEST_ARG3, TEST_ARG4);
    CHECK_EQUAL(TEST_RES4, result);
};
