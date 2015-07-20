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
#include "uvisor-lib/uvisor-lib.h"
#include "common.h"
#include "boxN_acl.h"
#include "box4.h"

/* create ACLs for secret data section */
BOXN_ACL(g_box4_acl);

/* configure secure box compartnent */
UVISOR_BOX_CONFIG(box4, g_box4_acl, UVISOR_BOX_STACK_SIZE);

/* box secure function call */
uint32_t box4_test_function(uint32_t a1,
                            uint32_t a2,
                            uint32_t a3,
                            uint32_t a4)
{
    return secure_gateway(box4, __test_function4, a1, a2, a3, a4);
}
