#ifndef BTE_BTA_HH_INT_H
#define BTE_BTA_HH_INT_H

/* Original source:
 * bluedroid <android.googlesource.com/platform/external/bluetooth/bluedroid>
 * bta/hh/bta_hh_int.h
 */

/******************************************************************************
 *
 *  Copyright (C) 2005-2012 Broadcom Corporation
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

#include "bta_api.h"
#include "bta_hh_api.h"
#include "bta_sys.h"
#include "hiddefs.h"
#include "sdp_api.h"

/*******************************************************************************
 * macros
 */

#define BTA_HH_MAX_RPT_CHARS		8

#define BTA_HH_FST_TRANS_CB_EVT		BTA_HH_GET_RPT_EVT
#define BTA_HH_FST_BTE_TRANS_EVT	HID_TRANS_GET_REPORT

/*******************************************************************************
 * types
 */

#ifdef __cplusplus
	extern "C" {
#endif

typedef UINT16 tBTA_HH_INT_EVT;
enum
{
	BTA_HH_API_OPEN_EVT = BTA_SYS_EVT_START(BTA_ID_HH),
	BTA_HH_API_CLOSE_EVT,
	BTA_HH_INT_OPEN_EVT,
	BTA_HH_INT_CLOSE_EVT,
	BTA_HH_INT_DATA_EVT,
	BTA_HH_INT_CTRL_DATA,
	BTA_HH_INT_HANDSK_EVT,
	BTA_HH_SDP_CMPL_EVT,
	BTA_HH_API_WRITE_DEV_EVT,
	BTA_HH_API_GET_DSCP_EVT,
	BTA_HH_API_MAINT_DEV_EVT,
	BTA_HH_OPEN_CMPL_EVT,
	BTA_HH_API_ENABLE_EVT,
	BTA_HH_API_DISABLE_EVT,
	BTA_HH_API_GET_ACL_Q_EVT,
	BTA_HH_DISC_CMPL_EVT,		// was 14
	BTA_HH_INVALID_EVT,			// was 15
};

typedef UINT8 tBTA_HH_STATE;
enum
{
	BTA_HH_NULL_ST,
	BTA_HH_IDLE_ST,
	BTA_HH_W4_CONN_ST,
	BTA_HH_CONN_ST,
};

typedef struct
{
	BT_HDR			hdr;									// size 0x08, offset 0x00
	UINT8			sec_mask;								// size 0x01, offset 0x08
	UINT8			service_name[BTA_SERVICE_NAME_LEN + 1];	// size 0x24, offset 0x09
	/* 3 bytes padding */
	tBTA_HH_CBACK	*p_cback;								// size 0x08, offset 0x30
} tBTA_HH_API_ENABLE; // size 0x34

typedef struct
{
	BT_HDR				hdr;		// size 0x08, offset 0x00
	BD_ADDR				bd_addr;	// size 0x06, offset 0x08
	UINT8				sec_mask;	// size 0x01, offset 0x0e
	tBTA_HH_PROTO_MODE	mode;		// size 0x01, offset 0x0f
} tBTA_HH_API_CONN; // size 0x10

typedef struct
{
	BT_HDR	hdr;		// size 0x08, offset 0x00
	UINT8	t_type;		// size 0x01, offset 0x08
	UINT8	param;		// size 0x01, offset 0x09
	UINT8	rpt_id;		// size 0x01, offset 0x0a
	/* 1 byte padding */
	UINT16	data;		// size 0x02, offset 0x0c
	/* 2 bytes padding */
	BT_HDR	*p_data;	// size 0x04, offset 0x10
} tBTA_HH_CMD_DATA; // size 0x14

typedef struct
{
	BT_HDR	hdr;		// size 0x08, offset 0x00
	UINT32	data;		// size 0x04, offset 0x08
	BT_HDR	*p_data;	// size 0x04, offset 0x0c
} tBTA_HH_CBACK_DATA; // size 0x10

typedef struct
{
	BT_HDR				hdr;		// size 0x08, offset 0x00
	BD_ADDR				bda;		// size 0x06, offset 0x08
	UINT16				attr_mask;	// size 0x02, offset 0x0e
	UINT16				sub_event;	// size 0x02, offset 0x10
	UINT8				sub_class;	// size 0x01, offset 0x12
	UINT8				app_id;		// size 0x01, offset 0x13
	tHID_DEV_DSCP_INFO	dscp_info;	// size 0x08, offset 0x14
} tBTA_HH_MAINT_DEV; // size 0x1c

typedef union
{
	BT_HDR				hdr;			// size 0x08
	tBTA_HH_API_ENABLE	api_enable;		// size 0x34
	tBTA_HH_API_CONN	api_conn;		// size 0x10
	tBTA_HH_CMD_DATA	api_sndcmd;		// size 0x14
	tBTA_HH_CBACK_DATA	hid_cback;		// size 0x10
	tBTA_HH_STATUS		status;			// size 0x01
	tBTA_HH_MAINT_DEV	api_maintdev;	// size 0x1c
} tBTA_HH_DATA;

typedef struct
{
	tHID_DEV_DSCP_INFO	dscp_info;		// size 0x08, offset 0x00
	BD_ADDR				addr;			// size 0x06, offset 0x08
	UINT16				attr_mask;		// size 0x02, offset 0x0e
	UINT16				w4_evt;			// size 0x02, offset 0x10
	UINT8				index;			// size 0x01, offset 0x12
	UINT8				sub_class;		// size 0x01, offset 0x13
	UINT8				sec_mask;		// size 0x01, offset 0x14
	UINT8				app_id;			// size 0x01, offset 0x15
	UINT8				hid_handle;		// size 0x01, offset 0x16
	BOOLEAN				vp;				// size 0x01, offset 0x17
	BOOLEAN				in_use;			// size 0x01, offset 0x18
	BOOLEAN				incoming_conn;	// size 0x01, offset 0x19
	BOOLEAN				opened;			// size 0x01, offset 0x1a
	tBTA_HH_PROTO_MODE	mode;			// size 0x01, offset 0x1b
	tBTA_HH_STATE		state;			// size 0x01, offset 0x1c
	/* 3 bytes padding */
} tBTA_HH_DEV_CB; // size 0x20

typedef struct
{
	BOOLEAN	upper;								// size 0x01, offset 0x00
	BOOLEAN	ctrl;								// size 0x01, offset 0x01
	BOOLEAN	alt;								// size 0x01, offset 0x02
	BOOLEAN	num_lock;							// size 0x01, offset 0x03
	BOOLEAN	caps_lock;							// size 0x01, offset 0x04
	UINT8	last_report[BTA_HH_MAX_RPT_CHARS];	// size 0x08, offset 0x05
} tBTA_HH_KB_CB; // size 0x0d

typedef struct
{
	tBTA_HH_KB_CB			kb_cb;						// size 0x00d, offset 0x000
	/* 3 bytes padding */
	tBTA_HH_DEV_CB			kdev[BTA_HH_MAX_KNOWN];		// size 0x200, offset 0x010
	tBTA_HH_DEV_CB			*p_cur;						// size 0x004, offset 0x210
	UINT8					cb_index[BTA_HH_MAX_KNOWN];	// size 0x010, offset 0x214
	tBTA_HH_CBACK			*p_cback;					// size 0x004, offset 0x224
	tSDP_DISCOVERY_DB		*p_disc_db;					// size 0x004, offset 0x228
	UINT8					trace_level;				// size 0x001, offset 0x22c
	UINT8					cnt_num;					// size 0x001, offset 0x22d
	BOOLEAN					w4_disable;					// size 0x001, offset 0x22e
	/* 1 byte padding */
} tBTA_HH_CB; // size 0x230

/*******************************************************************************
 * external globals
 */

extern tBTA_HH_CB bta_hh_cb;
extern tBTA_HH_CFG *p_bta_hh_cfg;

/*******************************************************************************
 * functions
 */

void bta_hh_disc_cmpl(void);
void bta_hh_sm_execute(tBTA_HH_DEV_CB *p_cb, UINT16 event,
                       tBTA_HH_DATA *p_data);
BOOLEAN bta_hh_tod_spt(tBTA_HH_DEV_CB *p_cb, UINT8 sub_class);
void bta_hh_add_device_to_list(tBTA_HH_DEV_CB *p_cb, UINT8 handle,
                               UINT16 attr_mask,
                               tHID_DEV_DSCP_INFO *p_dscp_info, UINT8 sub_class,
                               UINT8 app_id);
void bta_hh_clean_up_kdev(tBTA_HH_DEV_CB *p_cb);
void bta_hh_trace_dev_db(void);
BOOLEAN bta_hh_hdl_event(BT_HDR *p_msg);
void bta_hh_parse_keybd_rpt(tBTA_HH_BOOT_RPT *p_kb_data, UINT8 *p_report,
                            UINT16 report_len);
void bta_hh_parse_mice_rpt(tBTA_HH_BOOT_RPT *p_kb_data, UINT8 *p_report,
                           UINT16 report_len);
UINT8 bta_hh_find_cb(BD_ADDR bda);

// ---

void bta_hh_api_enable(tBTA_HH_DATA *p_data);
void bta_hh_api_disable(void);
void bta_hh_disc_cmpl(void);

void bta_hh_start_sdp(tBTA_HH_DEV_CB *p_cb, tBTA_HH_DATA *p_data);
void bta_hh_sdp_cmpl(tBTA_HH_DEV_CB *p_cb, tBTA_HH_DATA *p_data);
void bta_hh_api_disc_act(tBTA_HH_DEV_CB *p_cb, tBTA_HH_DATA *p_data);
void bta_hh_open_cmpl_act(tBTA_HH_DEV_CB *p_cb, tBTA_HH_DATA *p_data);
void bta_hh_open_act(tBTA_HH_DEV_CB *p_cb, tBTA_HH_DATA *p_data);
void bta_hh_data_act(tBTA_HH_DEV_CB *p_cb, tBTA_HH_DATA *p_data);
void bta_hh_handsk_act(tBTA_HH_DEV_CB *p_cb, tBTA_HH_DATA *p_data);
void bta_hh_ctrl_dat_act(tBTA_HH_DEV_CB *p_cb, tBTA_HH_DATA *p_data);
void bta_hh_close_act(tBTA_HH_DEV_CB *p_cb, tBTA_HH_DATA *p_data);
void bta_hh_get_dscp_act(tBTA_HH_DEV_CB *p_cb, tBTA_HH_DATA *p_data);
void bta_hh_maint_dev_act(tBTA_HH_DEV_CB *p_cb, tBTA_HH_DATA *p_data);
void bta_hh_get_acl_q_info(void);
void bta_hh_write_dev_act(tBTA_HH_DEV_CB *p_cb, tBTA_HH_DATA *p_data);

void bta_hh_sm_execute(tBTA_HH_DEV_CB *p_cb, UINT16 event,
                       tBTA_HH_DATA *p_data);
BOOLEAN bta_hh_hdl_event(BT_HDR *p_msg);

UINT8 bta_hh_find_cb(BD_ADDR bda);
void bta_hh_clean_up_kdev(tBTA_HH_DEV_CB *p_cb);
void bta_hh_add_device_to_list(tBTA_HH_DEV_CB *p_cb, UINT8 handle,
                               UINT16 attr_mask,
                               tHID_DEV_DSCP_INFO *p_dscp_info, UINT8 sub_class,
                               UINT8 app_id);
BOOLEAN bta_hh_tod_spt(tBTA_HH_DEV_CB *p_cb, UINT8 sub_class);
void bta_hh_parse_keybd_rpt(tBTA_HH_BOOT_RPT *p_kb_data, UINT8 *p_report,
                            UINT16 report_len);
void bta_hh_parse_mice_rpt(tBTA_HH_BOOT_RPT *p_mice_data, UINT8 *p_report,
                           UINT16 report_len);
void bta_hh_trace_dev_db(void);

#ifdef __cplusplus
	}
#endif

#endif // BTE_BTA_HH_INT_H
