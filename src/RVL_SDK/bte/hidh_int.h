#ifndef BTE_HID_HOST_INT_H
#define BTE_HID_HOST_INT_H

/* Original source:
 * bluedroid <android.googlesource.com/platform/external/bluetooth/bluedroid>
 * stack/hid/hidh_int.h
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

#include "gki.h"
#include "hid_conn.h"
#include "hiddefs.h"
#include "hidh_api.h"
#include "l2c_api.h"
#include "sdp_api.h"

/*******************************************************************************
 * macros
 */

#define MAX_SERVICE_DB_SIZE	4000

/*******************************************************************************
 * types
 */

#ifdef __cplusplus
	extern "C" {
#endif

enum
{
	HID_DEV_NO_CONN,
	HID_DEV_CONNECTED,
};

typedef struct per_device_ctb
{
	BOOLEAN		in_use;			// size 0x01, offset 0x00
	BD_ADDR		addr;			// size 0x06, offset 0x01
	/* 1 byte padding */
	UINT16		attr_mask;		// size 0x02, offset 0x08
	UINT8		state;			// size 0x01, offset 0x0a
	UINT8		conn_substate;	// size 0x01, offset 0x0b
	UINT8		conn_tries;		// size 0x01, offset 0x0c
	/* 3 bytes padding */
	tHID_CONN	conn;			// size 0x24, offset 0x10
} tHID_HOST_DEV_CTB; // size 0x34

typedef struct host_ctb
{
	tHID_HOST_DEV_CTB		devices[HID_HOST_MAX_DEVICES];	// size 0x340, offset 0x000
	tHID_HOST_DEV_CALLBACK	*callback;						// size 0x004, offset 0x340
	tL2CAP_CFG_INFO			l2cap_cfg;						// size 0x03c, offset 0x344
	BOOLEAN					sdp_busy;						// size 0x001, offset 0x380
	/* 3 bytes padding */
	tHID_HOST_SDP_CALLBACK	*sdp_cback;						// size 0x004, offset 0x384
	tSDP_DISCOVERY_DB		*p_sdp_db;						// size 0x004, offset 0x388
	tHID_DEV_SDP_INFO		sdp_rec;						// size 0x074, offset 0x38c
	BOOLEAN					reg_flag;						// size 0x001, offset 0x400
	UINT8					trace_level;					// size 0x001, offset 0x401
	/* 2 bytes padding */
} tHID_HOST_CTB; // size 0x404

/*******************************************************************************
 * external globals
 */

extern tHID_HOST_CTB hh_cb;

/*******************************************************************************
 * functions
 */

tHID_STATUS hidh_conn_reg(void);
void hidh_conn_dereg(void);
tHID_STATUS hidh_conn_initiate(UINT8 dhandle);
tHID_STATUS hidh_conn_snd_data(UINT8 dhandle, UINT8 trans_type, UINT8 param,
                               UINT16 data, UINT8 rpt_id, BT_HDR *buf);
tHID_STATUS hidh_conn_disconnect(UINT8 dhandle);
void hidh_proc_repage_timeout(TIMER_LIST_ENT *p_tle);

#ifdef __cplusplus
	}
#endif

#endif // BTE_HID_HOST_INT_H
