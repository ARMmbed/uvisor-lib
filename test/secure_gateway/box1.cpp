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

/* create ACLs for secret data section */
static const UvisorBoxAclItem g_box0_acl[] = {
};

/* configure secure box compartnent */
UVISOR_BOX_CONFIG(box0, g_box0_acl, UVISOR_BOX_STACK_SIZE);

uint32_t box1_test_function(void)
{
    return secure_gateway(box0, __test_function0);
}

uint32_t box1_test_function(uint32_t a1)
{
    return secure_gateway(box0, __test_function1, a1);
}

uint32_t box1_test_function(uint32_t a1,
                            uint32_t a2)
{
    return secure_gateway(box0, __test_function2, a1, a2);
}

uint32_t box1_test_function(uint32_t a1,
                            uint32_t a2,
                            uint32_t a3)
{
    return secure_gateway(box0, __test_function3, a1, a2, a3);
}

uint32_t box1_test_function(uint32_t a1,
                            uint32_t a2,
                            uint32_t a3,
                            uint32_t a4)
{
    return secure_gateway(box0, __test_function4, a1, a2, a3, a4);
}
