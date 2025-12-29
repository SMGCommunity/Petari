#ifndef BTE_UTL_H
#define BTE_UTL_H

/* Original source:
 * bluedroid <android.googlesource.com/platform/external/bluetooth/bluedroid>
 * bta/include/utl.h
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

typedef UINT8 tUTL_SET_DEVICE_CLASS_CMD;
enum
{
	BTA_UTL_SET_COD_MAJOR_MINOR		= 1 << 0,
	BTA_UTL_SET_COD_SERVICE_CLASS	= 1 << 1,
	BTA_UTL_CLR_COD_SERVICE_CLASS	= 1 << 2,
	BTA_UTL_SET_COD_ALL				= 1 << 3,

	BTA_UTL_INIT_COD				=
		BTA_UTL_SET_COD_SERVICE_CLASS | BTA_UTL_SET_COD_ALL,
};

typedef struct
{
	UINT8	minor;		// size 0x01, offset 0x00
	UINT8	major;		// size 0x01, offset 0x01
	UINT16	service;	// size 0x02, offset 0x02
} tBTA_UTL_COD; // size 0x04

/*******************************************************************************
 * functions
 */

INT16 utl_str2int(char const *p_s);
int utl_strucmp(char const *p_s, char const *p_t);
UINT8 utl_itoa(UINT16 i, char *p_s);
void utl_freebuf(void **p);
BOOLEAN utl_set_device_class(tBTA_UTL_COD *p_cod,
                             tUTL_SET_DEVICE_CLASS_CMD cmd);

#ifdef __cplusplus
	}
#endif

#endif // BTE_UTL_H
