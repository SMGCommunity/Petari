#ifndef BTE_GAP_API_H
#define BTE_GAP_API_H

/* Original source:
 * bluedroid <android.googlesource.com/platform/external/bluetooth/bluedroid>
 * stack/include/gap_api.h
 *
 * NOTE: This file only appears in bluedroid since commit ead3cde.
 */

/******************************************************************************
 *
 *  Copyright (C) 2009-2013 Broadcom Corporation
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

#include "btm_api.h"
#include "profiles_api.h"

/*******************************************************************************
 * macros
 */

#define GAP_INVALID_HANDLE			0xffff

/*******************************************************************************
 * types
 */

#ifdef __cplusplus
	extern "C" {
#endif

enum
{
	GAP_UNSUPPORTED			= GAP_ERR_GROUP +  1,
	GAP_EOINQDB				= GAP_ERR_GROUP +  2,
	GAP_ERR_BUSY			= GAP_ERR_GROUP +  3,
	GAP_ERR_NO_CTRL_BLK		= GAP_ERR_GROUP +  4,
	// 5
	// 6
	GAP_ERR_ILL_MODE		= GAP_ERR_GROUP +  7,
	GAP_ERR_ILL_INQ_TIME	= GAP_ERR_GROUP +  8,
	GAP_ERR_ILL_PARM		= GAP_ERR_GROUP +  9,
	// 10
	GAP_CMD_INITIATED		= GAP_ERR_GROUP + 11,
	GAP_DEVICE_NOT_UP		= GAP_ERR_GROUP + 12,
	GAP_BAD_BD_ADDR			= GAP_ERR_GROUP + 13,
	GAP_ERR_BAD_HANDLE		= GAP_ERR_GROUP + 14,
	GAP_ERR_BUF_OFFSET		= GAP_ERR_GROUP + 15,
	GAP_ERR_BAD_STATE		= GAP_ERR_GROUP + 16,
	GAP_NO_DATA_AVAIL		= GAP_ERR_GROUP + 17,
	GAP_ERR_CONGESTED		= GAP_ERR_GROUP + 18,
	// 19
	GAP_ERR_PROCESSING		= GAP_ERR_GROUP + 20,
	GAP_ERR_TIMEOUT			= GAP_ERR_GROUP + 21,
};

enum
{
	GAP_EVT_INQUIRY_RESULTS		= 1,
	GAP_EVT_INQUIRY_COMPLETE	= 2,
	GAP_EVT_DISCOVERY_COMPLETE	= 3,
	GAP_EVT_REM_NAME_COMPLETE	= 4,
	GAP_EVT_FIND_ADDR_COMPLETE	= 5,

	// TODO: same enum group or no?
	GAP_EVT_CONN_OPENED			= 256,
	GAP_EVT_CONN_CLOSED			= 257,
	GAP_EVT_CONN_DATA_AVAIL		= 258,
	GAP_EVT_CONN_CONGESTED		= 259,
	GAP_EVT_CONN_UNCONGESTED	= 260,

#define GAP_EVT_CONN_CONGESTED		(GAP_EVT_CONN_CONGESTED + 0)
#define GAP_EVT_CONN_UNCONGESTED	(GAP_EVT_CONN_UNCONGESTED + 0)
};


enum
{
	GAP_GENERAL_INQUIRY	= BTM_GENERAL_INQUIRY,
	GAP_LIMITED_INQUIRY	= BTM_LIMITED_INQUIRY,
};

enum
{
	GAP_DISALLOW_PAIRING	= 0,
	GAP_ALLOW_PAIRING		= 1,
};

enum
{
	GAP_SET_COD_MAJOR_MINOR		= 1 << 0,
	GAP_SET_COD_SERVICE_CLASS	= 1 << 1,
	GAP_CLR_COD_SERVICE_CLASS	= 1 << 2,
	GAP_SET_COD_ALL				= 1 << 3,

	GAP_INIT_COD				= GAP_SET_COD_ALL | GAP_SET_COD_SERVICE_CLASS,
};

typedef void tGAP_CONN_CALLBACK(UINT16 gap_handle, UINT16 event);
typedef void tGAP_CALLBACK(UINT16 event, void *p_data);

typedef tBTM_REMOTE_DEV_NAME tGAP_REMOTE_DEV_NAME;
typedef tBTM_INQ_PARMS tGAP_INQ_PARMS;
typedef tBTM_INQ_RESULTS tGAP_INQ_RESULTS;

typedef struct
{
	UINT16	status;			// size 0x02, offset 0x00
	UINT8	num_results;	// size 0x01, offset 0x02
	/* 1 byte padding */
} tGAP_INQ_CMPL; // size 0x04

typedef struct
{
	UINT16			status;		// size 0x02, offset 0x00
	BD_ADDR			bd_addr;	// size 0x06, offset 0x02
	tBTM_BD_NAME	devname;	// size 0x41, offset 0x08
	/* 1 byte padding */
} tGAP_FINDADDR_RESULTS; // size 0x4a

typedef struct
{
	UINT8	minor;		// size 0x01, offset 0x00
	UINT8	major;		// size 0x01, offset 0x01
	UINT16	service;	// size 0x02, offset 0x02
} tGAP_COD; // size 0x04

typedef struct
{
	UINT8	lmp_version;	// size 0x01, offset 0x00
	/* 1 byte padding */
	UINT16	mfc_name;		// size 0x02, offset 0x02
	UINT16	lmp_subversion;	// size 0x02, offset 0x04
} tGAP_LMP_VERSION; // size 0x06

typedef struct
{
	UINT8	lmp_features[8];	// size 0x08, offset 0x00
} tGAP_LMP_FEATURES; // size 0x08

/*******************************************************************************
 * functions
 */

void GAP_Init(void);

#ifdef __cplusplus
	}
#endif

#endif // BTE_GAP_API_H
