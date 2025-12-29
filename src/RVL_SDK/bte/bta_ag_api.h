#ifndef BTE_BTA_AG_API_H
#define BTE_BTA_AG_API_H

/* Original source:
 * bluedroid <android.googlesource.com/platform/external/bluetooth/bluedroid>
 * bta/include/bta_ag_api.h
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

/*******************************************************************************
 * types
 */

#ifdef __cplusplus
	extern "C" {
#endif

typedef struct
{
	char	*cind_info;		// size 0x04, offset 0x00
	INT32	conn_tout;		// size 0x04, offset 0x04
	UINT16	sco_pkt_types;	// size 0x02, offset 0x08
	/* 2 bytes padding */
} tBTA_AG_CFG; // size 0x0c

#ifdef __cplusplus
	}
#endif

#endif // BTE_BTA_AG_API_H
