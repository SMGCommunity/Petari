#ifndef BTE_BTA_HH_API_H
#define BTE_BTA_HH_API_H

/* Original source:
 * bluedroid <android.googlesource.com/platform/external/bluetooth/bluedroid>
 * bta/include/bta_hh_api.h
 */

/******************************************************************************
 *
 *  Copyright (C) 2002-2012 Broadcom Corporation
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

#include "bt_target.h"
#include "bt_types.h"
#include "data_types.h"

#include "bta_api.h"
#include "hiddefs.h"

/*******************************************************************************
 * macros
 */

#define BTA_HH_MAX_KNOWN		HID_HOST_MAX_DEVICES

#define BTA_HH_INVALID_HANDLE	0xff

/*******************************************************************************
 * types
 */

#ifdef __cplusplus
	extern "C" {
#endif

typedef UINT8 tBTA_HH_STATUS;
enum
{
	BTA_HH_OK				= 0,
	BTA_HH_HS_TRANS_NOT_SPT	= 3,
	BTA_HH_HS_ERROR			= 5,
	BTA_HH_ERR				= 6,
	BTA_HH_ERR_SDP			= 7,
	BTA_HH_ERR_PROTO		= 8,
	BTA_HH_ERR_DB_FULL		= 9,
	BTA_HH_ERR_NO_RES		= 11,
	BTA_HH_ERR_AUTH_FAILED	= 12,
	BTA_HH_ERR_HDL			= 13,
	BTA_HH_ERR_16			= 16,
	BTA_HH_ERR_TOD_UNSPT	= 255, // was 10
};

typedef UINT16 tBTA_HH_EVT;
enum
{
	BTA_HH_ENABLE_EVT		= 0,
	BTA_HH_DISABLE_EVT		= 1,
	BTA_HH_OPEN_EVT			= 2,
	BTA_HH_CLOSE_EVT		= 3,
	BTA_HH_GET_RPT_EVT		= 4,
	BTA_HH_SET_RPT_EVT		= 5,
	BTA_HH_GET_PROTO_EVT	= 6,
	BTA_HH_SET_PROTO_EVT	= 7,
	BTA_HH_GET_IDLE_EVT		= 8,
	BTA_HH_SET_IDLE_EVT		= 9,
	BTA_HH_GET_DSCP_EVT		= 10,
	BTA_HH_ADD_DEV_EVT		= 11,
	BTA_HH_RMV_DEV_EVT		= 12,
	BTA_HH_VC_UNPLUG_EVT	= 13,
	BTA_HH_GET_ACL_Q_EVT	= 15,
};

typedef UINT8 tBTA_HH_PROTO_MODE;
enum
{
	BTA_HH_PROTO_RPT_MODE	= 0,
	BTA_HH_PROTO_BOOT_MODE	= 1,

	BTA_HH_PROTO_UNKNOWN	= 255
};

typedef UINT8 tBTA_HH_BOOT_RPT_ID;
enum
{
	BTA_HH_KEYBD_RPT_ID = 1,
	BTA_HH_MOUSE_RPT_ID,
};

typedef UINT8 tBTA_HH_DEVT;
enum
{
	BTA_HH_DEVT_UNKNOWN	= 0,
};

typedef UINT16 tBTA_HH_ATTR_MASK;
#if 0
enum
{
	/* ... */
};
#endif

typedef UINT8 tBTA_HH_RPT_TYPE;
enum
{
	BTA_HH_RPTT_OUTPUT	= 2,
};

typedef UINT8 tBTA_HH_TRANS_CTRL_TYPE;
enum
{
	BTA_HH_CTRL_SUSPEND					= 3,
	BTA_HH_CTRL_EXIT_SUSPEND			= 4,
	BTA_HH_CTRL_VIRTUAL_CABLE_UNPLUG	= 5,
};

typedef struct
{
	tBTA_HH_DEVT	tod;	// size 0x01, offset 0x00
	UINT8			app_id;	// size 0x01, offset 0x01
} tBTA_HH_SPT_TOD; // size 0x02

typedef struct
{
	UINT8			max_devt_spt;	// size 0x01, offset 0x00
	/* 3 bytes padding */
	tBTA_HH_SPT_TOD	*p_devt_list;	// size 0x04, offset 0x04
	UINT16			sdp_db_size;	// size 0x02, offset 0x08
	/* 2 bytes padding */
} tBTA_HH_CFG; // size 0x0c

typedef struct
{
	BD_ADDR			bda;	// size 0x06, offset 0x00
	tBTA_HH_STATUS	status;	// size 0x01, offset 0x06
	UINT8			handle;	// size 0x01, offset 0x07
} tBTA_HH_CONN; // size 0x08

typedef tBTA_HH_CONN tBTA_HH_DEV_INFO;

typedef struct
{
	tBTA_HH_STATUS	status;	// size 0x01, offset 0x00
	UINT8			handle;	// size 0x01, offset 0x01
} tBTA_HH_CBDATA; // size 0x02

typedef struct
{
	UINT8	this_char;	// size 0x01, offset 0x00
	BOOLEAN	caps_lock;	// size 0x01, offset 0x01
	BOOLEAN	upper;		// size 0x01, offset 0x02
	BOOLEAN	ctrl;		// size 0x01, offset 0x03
	BOOLEAN	alt;		// size 0x01, offset 0x04
	BOOLEAN	num_lock;	// size 0x01, offset 0x05
} tBTA_HH_KEYBD_RPT; // size 0x06

typedef struct
{
	UINT8	mouse_button;	// size 0x01, offset 0x00
	INT8	delta_x;		// size 0x01, offset 0x01
	INT8	delta_y;		// size 0x01, offset 0x01
} tBTA_HH_MICE_RPT; // size 0x03

typedef struct
{
	tBTA_HH_BOOT_RPT_ID	dev_type;	// size 0x01, offset 0x00
	union
	{
		tBTA_HH_KEYBD_RPT	keybd_rpt;	// size 0x06
		tBTA_HH_MICE_RPT	mice_rpt;	// size 0x03
	} data_rpt; // size 0x06, offset 0x01
} tBTA_HH_BOOT_RPT; // size 0x07

typedef struct
{
	tBTA_HH_STATUS	status;	// size 0x01, offset 0x00
	UINT8			handle;	// size 0x01, offset 0x01
	union
	{
		tBTA_HH_PROTO_MODE	proto_mode;	// size 0x01
		BT_HDR				rpt_data;	// size 0x08
		UINT8				idle_rate;	// size 0x01
	} rsp_data; // size 0x08, offset 0x02
} tBTA_HH_HSDATA; // size 0x0a

typedef struct
{
	UINT8	handle;			// size 0x01, offset 0x00
	UINT16	num_queued;		// size 0x02, offset 0x02
	UINT16	num_not_acked;	// size 0x02, offset 0x04
} tBTA_HH_QUEUE_INFO; // size 0x6

typedef struct
{
	UINT16				num_avail_buffs;	// size 0x02, offset 0x00
	UINT16				num_buffs;			// size 0x02, offset 0x02
	UINT16				num_links;			// size 0x02, offset 0x04
	tBTA_HH_QUEUE_INFO	queue_info[4];		// size 0x18, offset 0x06
} tBTA_HH_ACL_QUEUE_INFO; // size 0x1e

typedef union
{
	tBTA_HH_DEV_INFO		dev_info;		// size 0x08
	tBTA_HH_CONN			conn;			// size 0x08
	tBTA_HH_CBDATA			dev_status;		// size 0x02
	tBTA_HH_STATUS			status;			// size 0x01
	tHID_DEV_DSCP_INFO		dscp_info;		// size 0x08
	tBTA_HH_HSDATA			hs_data;		// size 0x0a
	tBTA_HH_ACL_QUEUE_INFO	acl_queue_info;	// size 0x1e
} tBTA_HH;

typedef void tBTA_HH_CBACK(tBTA_HH_EVT event, tBTA_HH *p_data);

/*******************************************************************************
 * functions
 */

void BTA_HhEnable(tBTA_SEC sec_mask, tBTA_HH_CBACK *p_cback);
void BTA_HhDisable(void);
void BTA_HhClose(UINT8 dev_handle);
void BTA_HhOpen(BD_ADDR dev_bda, tBTA_HH_PROTO_MODE mode, tBTA_SEC sec_mask);

void BTA_HhSetReport(UINT8 dev_handle, tBTA_HH_RPT_TYPE r_type, BT_HDR *p_data);
void BTA_HhGetReport(UINT8 dev_handle, tBTA_HH_RPT_TYPE r_type, UINT8 rpt_id,
                     UINT16 buf_size);
void BTA_HhSetProtoMode(UINT8 dev_handle, tBTA_HH_PROTO_MODE p_type);
void BTA_HhGetProtoMode(UINT8 dev_handle);
void BTA_HhSetIdle(UINT8 dev_handle, UINT16 idle_rate);
void BTA_HhGetIdle(UINT8 dev_handle);
void BTA_HhSendCtrl(UINT8 dev_handle, tBTA_HH_TRANS_CTRL_TYPE c_type);
void BTA_HhSendData(UINT8 dev_handle, BT_HDR *p_data);
void BTA_HhGetDscpInfo(UINT8 dev_handle);
void BTA_HhAddDev(BD_ADDR bda, tBTA_HH_ATTR_MASK attr_mask, UINT8 sub_class,
                  UINT8 app_id, tHID_DEV_DSCP_INFO dscp_info);
void BTA_HhRemoveDev(UINT8 dev_handle);
void BTA_HhGetAclQueueInfo(void);
void BTA_HhParseBootRpt(tBTA_HH_BOOT_RPT *p_data, UINT8 *p_report,
                        UINT16 report_len);

#ifdef __cplusplus
	}
#endif

#endif // BTE_BTA_HH_API_H
