#ifndef BTE_BTA_SYS_INT_H
#define BTE_BTA_SYS_INT_H

/* Original source:
 * bluedroid <android.googlesource.com/platform/external/bluetooth/bluedroid>
 * bta/sys/bta_sys_int.h
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

#include "data_types.h"

#include "bta_sys.h"
#include "ptim.h"

/*******************************************************************************
 * types
 */

#ifdef __cplusplus
	extern "C" {
#endif

typedef struct
{
	tBTA_SYS_REG		*reg[BTA_ID_MAX];	// size 0x68, offset 0x00
	tPTIM_CB			ptim_cb;			// size 0x14, offset 0x68
	UINT8				timers_disabled;	// size 0x01, offset 0x7c
	UINT8				task_id;			// size 0x01, offset 0x7d
	UINT8				events_disabled;	// size 0x01, offset 0x7e
	tBTA_SYS_CONN_CBACK	*prm_cb;			// size 0x04, offset 0x80
	tBTA_SYS_CONN_CBACK	*ppm_cb;			// size 0x04, offset 0x84
	tBTA_SYS_CONN_CBACK	*compress_cb;		// size 0x04, offset 0x88
} tBTA_SYS_CB; // size 0x8c

/*******************************************************************************
 * external globals
 */

extern tBTA_SYS_CB bta_sys_cb;
extern tBTA_SYS_CFG *p_bta_sys_cfg;

/*******************************************************************************
 * functions
 */

#ifdef __cplusplus
	}
#endif

#endif // BTE_BTA_SYS_INT_H
