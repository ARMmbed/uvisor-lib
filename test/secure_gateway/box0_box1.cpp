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
#include "box1.h"

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
