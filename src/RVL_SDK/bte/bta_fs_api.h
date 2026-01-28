#ifndef BTE_BTA_FS_API_H
#define BTE_BTA_FS_API_H

/* Original source:
 * bluedroid <android.googlesource.com/platform/external/bluetooth/bluedroid>
 * bta/include/bta_fs_api.h
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
	UINT16			max_file_len;	// size 0x02, offset 0x00
	UINT16			max_path_len;	// size 0x02, offset 0x02
	char			path_separator;	// size 0x01, offset 0x04
	char unsigned	at_0x05[7];		// size 0x07, offset 0x05
} tBTA_FS_CFG; // size 0x0c

/*******************************************************************************
 * external globals
 */

extern tBTA_FS_CFG const bta_fs_cfg;
extern tBTA_FS_CFG *p_bta_fs_cfg;

#ifdef __cplusplus
	}
#endif

#endif // BTE_BTA_FS_API_H
