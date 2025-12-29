#ifndef BTE_GKI_INT_H
#define BTE_GKI_INT_H

/* Original source:
 * bluedroid <android.googlesource.com/platform/external/bluetooth/bluedroid>
 * gki/ulinux/gki_int.h
 */

/******************************************************************************
 *
 *  Copyright (C) 1999-2012 Broadcom Corporation
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

#include "gki_common.h"

/*******************************************************************************
 * macros
 */

#define MAX_INT_STATE	20	/* name known from asserts */

/*******************************************************************************
 * types
 */

#ifdef __cplusplus
	extern "C" {
#endif

typedef struct
{
	UINT8	int_index;					// size 0x01, offset 0x00
	/* 3 bytes padding */
	int		int_state[MAX_INT_STATE];	// size 0x50, offset 0x04
} tGKI_OS; // size 0x54

typedef struct
{
	tGKI_OS		os;		// size 0x00054, offset 0x00000
	tGKI_COM_CB	com;	// size 0x28a8c, offset 0x00054
} tGKI_CB; // size 0x28ae0

/*******************************************************************************
 * external globals
 */

extern tGKI_CB gki_cb;

/*******************************************************************************
 * functions
 */

void gki_adjust_timer_count(INT32);

// ---

void gki_buffer_init(void);
/**/
BOOLEAN gki_chk_buf_damage(void *p_buf);

void gki_timers_init(void);
/**/
void gki_adjust_timer_count(INT32 ticks);

#ifdef __cplusplus
	}
#endif

#endif // BTE_GKI_INT_H
