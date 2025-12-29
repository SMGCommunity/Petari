#ifndef BTE_PTIM_H
#define BTE_PTIM_H

/* Original source:
 * bluedroid <android.googlesource.com/platform/external/bluetooth/bluedroid>
 * bta/include/ptim.h
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

#include "gki.h"

/*******************************************************************************
 * types
 */

#ifdef __cplusplus
	extern "C" {
#endif

typedef struct
{
	TIMER_LIST_Q	timer_queue;	// size 0x0c, offset 0x00
	INT32			period;			// size 0x04, offset 0x0c
	UINT8			timer_id;		// size 0x01, offset 0x10
	/* 3 bytes padding */
} tPTIM_CB; // size 0x14

/*******************************************************************************
 * functions
 */

void ptim_init(tPTIM_CB *p_cb, UINT16 period, UINT8 timer_id);
void ptim_timer_update(tPTIM_CB *p_cb);
void ptim_start_timer(tPTIM_CB *p_cb, TIMER_LIST_ENT *p_tle, UINT16 type,
                      INT32 timeout);
void ptim_stop_timer(tPTIM_CB *p_cb, TIMER_LIST_ENT *p_tle);

#ifdef __cplusplus
	}
#endif

#endif // BTE_PTIM_H
