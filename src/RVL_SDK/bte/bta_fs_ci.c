#include "bta_fs_ci.h"

/* Original source:
 * bluedroid <android.googlesource.com/platform/external/bluetooth/bluedroid>
 * bta/fs/bta_fs_ci.c
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

#include <stddef.h> // NULL

#include "data_types.h"

#include "bta_fs_co.h"
#include "bta_sys.h" // bta_sys_sendmsg
#include "gki.h" // GKI_getbuf

/*******************************************************************************
 * functions
 */

void bta_fs_ci_write(tBTA_FS_FD fd, tBTA_FS_CO_STATUS status, UINT16 evt)
{
	tBTA_FS_CI_WRITE_EVT *p_evt;

	if ((p_evt = GKI_getbuf(sizeof *p_evt)) != NULL)
	{
		p_evt->hdr.event = evt;
		p_evt->fd = fd;
		p_evt->status = status;

		bta_sys_sendmsg(p_evt);
	}
}

void bta_fs_ci_read(tBTA_FS_FD fd, UINT16 num_bytes_read,
                    tBTA_FS_CO_STATUS status, UINT16 evt)
{
	tBTA_FS_CI_READ_EVT *p_evt;

	if ((p_evt = GKI_getbuf(sizeof *p_evt)) != NULL)
	{
		p_evt->hdr.event = evt;
		p_evt->fd = fd;
		p_evt->status = status;
		p_evt->num_read = num_bytes_read;

		bta_sys_sendmsg(p_evt);
	}
}

void bta_fs_ci_open(tBTA_FS_FD fd, tBTA_FS_CO_STATUS status, UINT32 file_size,
                    UINT16 evt)
{
	tBTA_FS_CI_OPEN_EVT *p_evt;

	if ((p_evt = GKI_getbuf(sizeof *p_evt)) != NULL)
	{
		p_evt->hdr.event = evt;
		p_evt->fd = fd;
		p_evt->status = status;
		p_evt->file_size = file_size;

		bta_sys_sendmsg(p_evt);
	}
}

void bta_fs_ci_direntry(tBTA_FS_CO_STATUS status, UINT16 evt)
{
	tBTA_FS_CI_GETDIR_EVT *p_evt;

	if ((p_evt = GKI_getbuf(sizeof *p_evt)) != NULL)
	{
		p_evt->hdr.event = evt;
		p_evt->status = status;

		bta_sys_sendmsg(p_evt);
	}
}
