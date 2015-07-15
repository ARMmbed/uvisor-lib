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
#include "nesting_patterns.h"
#include "box1.h"
#include "box2.h"
#include "box3.h"
#include "box4.h"

/* nesting depth counter: it is used to keep track of the nesting depth while
 * recursively triggering secure gateways */
int g_nest;

/* nested function call pattern: holds the sequence of box ids in the nesting
 * pattern */
int g_box[NESTING_MAX_DEPTH];

TEST_GROUP(SecureGatewayBox0BoxN)
{
};

/* this is the main function for the secure gateway testing; it follows the
 * nesting pattern provided by g_box triggering nested secure gateways (and
 * hence subsequently switching context to new boxes) until the context of the
 * latest box is not reached;
 *
 * the termination condition for recursion triggers a proper secure gateway to
 * the secure function call that computes the desired result; it is expected for
 * the result to propagate upwards in the nesting tree until all context
 * switches are resolved;
 *
 * note: this function is for testing purposes only; it uses the secure gateway
 * in a very different way from how we expect it to be used; in particular, the
 * following assumptions hold:
 *      - we are able to create multiple boxes in the context of a unique code
 *        base, inside a given module (this test module)
 *      - we are able to trigger secure gateways to different boxes as a
 *        consequence of the previous point
 * in a real world application it just would not make sense to do this */
UVISOR_EXTERN uint32_t boxM_boxN_nestX(uint32_t a0,
                                       uint32_t a1,
                                       uint32_t a2,
                                       uint32_t a3)
{
    uint32_t result;

    /* check recursion termination condition */
    if(g_nest == 0)
    {
        /* end of recursion: call the target function (will trigger another
         * context switch) */
        switch(g_box[g_nest])
        {
#if UVISOR_MAX_BOXES > 1
            case 1:
                return box1_test_function(a0, a1, a2, a3);
#endif
#if UVISOR_MAX_BOXES > 2
            case 2:
                return box2_test_function(a0, a1, a2, a3);
#endif
#if UVISOR_MAX_BOXES > 3
            case 3:
                return box3_test_function(a0, a1, a2, a3);
#endif
#if UVISOR_MAX_BOXES > 4
            case 4:
                return box4_test_function(a0, a1, a2, a3);
#endif
            default:
                FAIL("Box number not allowed");
                return TEST_ERR;
        }
    }
    else
    {
        /* recursive secure gateway to this function */
        /* note: assuming here that I can trigger a secure gateway to a
         * different box within the same file and that multiple boxes are
         * configured in the same module (at least for test purposes) */
        switch(g_box[g_nest])
        {
#if UVISOR_MAX_BOXES > 1
            case 1:
                --g_nest;
                result = secure_gateway(box1, boxM_boxN_nestX, a0, a1, a2, a3);
                return result;
#endif
#if UVISOR_MAX_BOXES > 2
            case 2:
                --g_nest;
                result = secure_gateway(box2, boxM_boxN_nestX, a0, a1, a2, a3);
                return result;
#endif
#if UVISOR_MAX_BOXES > 3
            case 3:
                --g_nest;
                result = secure_gateway(box3, boxM_boxN_nestX, a0, a1, a2, a3);
                return result;
#endif
#if UVISOR_MAX_BOXES > 4
            case 4:
                --g_nest;
                result = secure_gateway(box4, boxM_boxN_nestX, a0, a1, a2, a3);
                return result;
#endif
            default:
                FAIL("Box number not allowed");
                return TEST_ERR;
        }
    }
}

TEST(SecureGatewayBox0BoxN, box0_boxN_4args)
{
    int i;
    uint32_t result;

    /* iteratively test all provided nesting patterns */
    for(i = 0; i < g_nest_data_num; ++i)
    {
        /* copy nesting pattern for this iteration; copy is not needed in this
         * particular case but will be useful, for example, if testing dynamic
         * patterns in the future */
        memcpy(g_box, g_box_data[i], sizeof(int) * NESTING_MAX_DEPTH);

        /* nesting counter */
        g_nest = g_nest_data[i];

        /* calculate the result triggering the nesting pattern */
        result = boxM_boxN_nestX(TEST_ARG1, TEST_ARG2, TEST_ARG3, TEST_ARG4);
        CHECK_EQUAL(TEST_RES4, result);
    }
};
