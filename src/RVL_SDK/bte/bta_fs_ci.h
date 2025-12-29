#ifndef BTE_BTA_FS_CI_H
#define BTE_BTA_FS_CI_H

/* Original source:
 * bluedroid <android.googlesource.com/platform/external/bluetooth/bluedroid>
 * bta/include/bta_fs_ci.h
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

#include "bt_types.h"
#include "data_types.h"

#include "bta_fs_co.h"

/*******************************************************************************
 * types
 */

#ifdef __cplusplus
	extern "C" {
#endif

// TODO: should the tBTA_FS_FD of fd fields be typedef'd?
typedef struct
{
	BT_HDR				hdr;	// size 0x08, offset 0x00
	tBTA_FS_FD			fd;		// size 0x04, offset 0x08
	tBTA_FS_CO_STATUS	status;	// size 0x02, offset 0x0c
	/* 2 bytes padding */
} tBTA_FS_CI_WRITE_EVT; // size 0x10

typedef struct
{
	BT_HDR				hdr;		// size 0x08, offset 0x00
	tBTA_FS_FD			fd;			// size 0x04, offset 0x08
	UINT16				num_read;	// size 0x02, offset 0x0c
	tBTA_FS_CO_STATUS	status;		// size 0x02, offset 0x0e
} tBTA_FS_CI_READ_EVT; // size 0x10

typedef struct
{
	BT_HDR				hdr;		// size 0x08, offset 0x00
	UINT32				file_size;	// size 0x04, offset 0x08
	tBTA_FS_FD			fd;			// size 0x04, offset 0x0c
	tBTA_FS_CO_STATUS	status;		// size 0x02, offset 0x10
	/* 2 bytes padding */
} tBTA_FS_CI_OPEN_EVT; // size 0x14

typedef struct
{
	BT_HDR				hdr;	// size 0x08, offset 0x00
	tBTA_FS_CO_STATUS	status;	// size 0x02, offset 0x08
} tBTA_FS_CI_GETDIR_EVT; // size 0x0a

/*******************************************************************************
 * functions
 */

void bta_fs_ci_write(tBTA_FS_FD fd, tBTA_FS_CO_STATUS status, UINT16 evt);
void bta_fs_ci_read(tBTA_FS_FD fd, UINT16 num_bytes_read,
                    tBTA_FS_CO_STATUS status, UINT16 evt);
void bta_fs_ci_open(tBTA_FS_FD fd, tBTA_FS_CO_STATUS status, UINT32 file_size,
                    UINT16 evt);
void bta_fs_ci_direntry(tBTA_FS_CO_STATUS status, UINT16 evt);

#ifdef __cplusplus
	}
#endif

#endif // BTE_BTA_FS_CI_H
