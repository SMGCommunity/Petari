#ifndef BTE_BTA_PBS_INT_H
#define BTE_BTA_PBS_INT_H

/* Original source:
 * bluedroid <android.googlesource.com/platform/external/bluetooth/bluedroid>
 * bta/pb/bta_pbs_int.h
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
	UINT8	realm_charset;			// size 0x01, offset 0x00
	BOOLEAN	userid_req;				// size 0x01, offset 0x01
	UINT8	supported_features;		// size 0x01, offset 0x02
	UINT8	supported_repositories;	// size 0x01, offset 0x03
} tBTA_PBS_CFG; // size 0x04

/*******************************************************************************
 * external globals
 */

extern tBTA_PBS_CFG const bta_pbs_cfg;
extern tBTA_PBS_CFG *p_bta_pbs_cfg;

#ifdef __cplusplus
	}
#endif

#endif // BTE_BTA_PBS_INT_H
