#ifndef BTE_GOEP_FS_H
#define BTE_GOEP_FS_H

/* Original source:
 * bluedroid <android.googlesource.com/platform/external/bluetooth/bluedroid>
 * stack/include/goep_fs.h
 */

/******************************************************************************
 *
 *  Copyright (C) 2000-2012 Broadcom Corporation
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

/*******************************************************************************
 * types
 */

#ifdef __cplusplus
	extern "C" {
#endif

typedef INT32 tGOEP_FD;

typedef UINT16 tGOEP_STATUS;
enum
{
	GOEP_OK,
	GOEP_FAIL,
	GOEP_EACCES,
	GOEP_ENOTEMPTY,
	GOEP_EOF,
	GOEP_EODIR,
	GOEP_ENOSPACE,
	GOEP_EIS_DIR,
	GOEP_RESUME,
	GOEP_NONE,
};

typedef struct
{
	BT_HDR			hdr;		// size 0x08, offset 0x00
	tGOEP_FD		fd;			// size 0x04, offset 0x08
	tGOEP_STATUS	status;		// size 0x02, offset 0x0c
	/* 2 bytes padding */
	UINT32			file_size;	// size 0x04, offset 0x10
} tGOEP_OPEN_RSP; // size 0x14

typedef struct
{
	BT_HDR			hdr;		// size 0x08, offset 0x00
	tGOEP_FD		fd;			// size 0x04, offset 0x08
	tGOEP_STATUS	status;		// size 0x02, offset 0x0c
	UINT16			bytes_read;	// size 0x02, offset 0x0e
} tGOEP_READ_RSP; // size 0x10

typedef struct
{
	BT_HDR			hdr;	// size 0x08, offset 0x00
	tGOEP_FD		fd;		// size 0x04, offset 0x08
	tGOEP_STATUS	status;	// size 0x02, offset 0x0c
	/* 2 bytes padding */
} tGOEP_WRITE_RSP; // size 0x10

typedef struct
{
	BT_HDR			hdr;	// size 0x08, offset 0x00
	tGOEP_STATUS	status;	// size 0x02, offset 0x08
} tGOEP_DIRENTRY_RSP; // size 0x0a

/*******************************************************************************
 * functions
 */

void GOEP_OpenRsp(tGOEP_FD fd, tGOEP_STATUS status, UINT32 file_size,
                  UINT16 event_id);
void GOEP_ReadRsp(tGOEP_FD fd, tGOEP_STATUS status, UINT16 bytes_read,
                  UINT16 event_id);
void GOEP_WriteRsp(tGOEP_FD fd, tGOEP_STATUS status, UINT16 event_id);
void GOEP_DirentryRsp(tGOEP_STATUS status, UINT16 event_id);

#ifdef __cplusplus
	}
#endif

#endif // BTE_GOEP_FS_H
