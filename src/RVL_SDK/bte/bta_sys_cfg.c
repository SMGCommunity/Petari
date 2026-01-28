#include "bta_sys_int.h"

/* Original source:
 * bluedroid <android.googlesource.com/platform/external/bluetooth/bluedroid>
 * bta/sys/bta_sys_cfg.c
 */

/******************************************************************************
 *
 *  Copyright (C) 2003-2012 Broadcom Corporation
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at:
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 ******************************************************************************/

/* Includes changes by muff1n1634 */

/*******************************************************************************
 * headers
 */

#include "bt_trace.h"

#include "gki.h"

/*******************************************************************************
 * macros
 */

#ifndef BTA_MBOX_EVT
# define BTA_MBOX_EVT	TASK_MBOX_2_EVT_MASK
#endif

#ifndef BTA_MBOX
# define BTA_MBOX		TASK_MBOX_2
#endif

#ifndef BTA_TIMER
# define BTA_TIMER		TIMER_1
#endif

/*******************************************************************************
 * variables
 */

// .sdata2
tBTA_SYS_CFG const bta_sys_cfg =
{
	BTA_MBOX_EVT,
	BTA_MBOX,
	BTA_TIMER,
	APPL_INITIAL_TRACE_LEVEL
};

// .sdata
tBTA_SYS_CFG *p_bta_sys_cfg = (tBTA_SYS_CFG *)&bta_sys_cfg;
