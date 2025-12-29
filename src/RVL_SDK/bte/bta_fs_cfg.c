#include "bta_fs_api.h"

/* Original source:
 * bluedroid <android.googlesource.com/platform/external/bluetooth/bluedroid>
 * bta/fs/bta_fs_cfg.c
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
 * macros
 */

#ifndef BTA_FS_PATH_SEPARATOR
# define BTA_FS_PATH_SEPARATOR	((char)0x2f)
#endif

#ifndef BTA_FS_PATH_LEN
# define BTA_FS_PATH_LEN		294
#endif

#ifndef BTA_FS_FILE_LEN
# define BTA_FS_FILE_LEN		256
#endif

/*******************************************************************************
 * variables
 */

// .rodata
tBTA_FS_CFG const bta_fs_cfg =
{
	BTA_FS_FILE_LEN,
	BTA_FS_PATH_LEN,
	BTA_FS_PATH_SEPARATOR,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00
};

// .sdata
tBTA_FS_CFG *p_bta_fs_cfg = (tBTA_FS_CFG *)&bta_fs_cfg;
