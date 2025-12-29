#ifndef BTE_BTA_FS_CO_H
#define BTE_BTA_FS_CO_H

/* Original source:
 * bluedroid <android.googlesource.com/platform/external/bluetooth/bluedroid>
 * bta/include/bta_fs_co.h
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

typedef UINT16 tBTA_FS_CO_STATUS;

typedef int tBTA_FS_OPEN_FLAGS;
enum
{
	BTA_FS_O_RDONLY	= 0,
};

typedef int tBTA_FS_FD;

/*******************************************************************************
 * functions
 */

/* WARNING: These callouts are not defined. */
extern void bta_fs_co_open(char const *p_path, tBTA_FS_OPEN_FLAGS oflags,
                           UINT32 size, UINT16 evt, UINT8 app_id);
extern tBTA_FS_CO_STATUS bta_fs_co_close(tBTA_FS_FD fd, UINT8 app_id);
extern void bta_fs_co_read(tBTA_FS_FD fd, UINT8 *p_buf, UINT16 nbytes,
                           UINT16 evt, UINT8 app_id);

#ifdef __cplusplus
	}
#endif

#endif // BTE_BTA_FS_CO_H
