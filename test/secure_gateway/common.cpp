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
