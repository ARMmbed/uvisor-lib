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
#ifndef __UVISOR_LIB_UVISOR_LIB_H__
#define __UVISOR_LIB_UVISOR_LIB_H__

/* This file includes all the uVisor library header files at once.
 * Some files are included depending on the whether uVisor is supported on the
 * target or not. */

/* Note: uvisor-lib/override.h should never be included automatically, but only
 *       when needed to explicitly override symbols in a source file. See the
 *       file's comments for further information. */

#if defined(YOTTA_CFG_UVISOR_PRESENT) && YOTTA_CFG_UVISOR_PRESENT == 1

#include "uvisor/api/inc/benchmark.h"
#include "uvisor/api/inc/box_config.h"
#include "uvisor/api/inc/debug.h"
#include "uvisor/api/inc/disabled.h"
#include "uvisor/api/inc/error.h"
#include "uvisor/api/inc/interrupts.h"
#include "uvisor/api/inc/register_gateway.h"
#include "uvisor/api/inc/secure_access.h"
#include "uvisor/api/inc/secure_gateway.h"

#else /* defined(YOTTA_CFG_UVISOR_PRESENT) && YOTTA_CFG_UVISOR_PRESENT == 1 */

#include "uvisor-lib/unsupported.h"

#endif /* defined(YOTTA_CFG_UVISOR_PRESENT) && YOTTA_CFG_UVISOR_PRESENT == 1 */

#include "uvisor/api/inc/debug_exports.h"
#include "uvisor/api/inc/halt_exports.h"
#include "uvisor/api/inc/svc_exports.h"
#include "uvisor/api/inc/svc_gw_exports.h"
#include "uvisor/api/inc/unvic_exports.h"
#include "uvisor/api/inc/uvisor_exports.h"
#include "uvisor/api/inc/vmpu_exports.h"

#endif /* __UVISOR_LIB_UVISOR_LIB_H__ */
