#ifndef BTE_HID_CONN_H
#define BTE_HID_CONN_H

/* Original source:
 * bluedroid <android.googlesource.com/platform/external/bluetooth/bluedroid>
 * stack/hid/hid_conn.h
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

#include "data_types.h"

#include "gki.h"

/*******************************************************************************
 * types
 */

#ifdef __cplusplus
	extern "C" {
#endif

typedef UINT8 tHID_CONN_STATE;
enum
{
	HID_CONN_STATE_UNUSED,
	HID_CONN_STATE_CONNECTING_CTRL,
	HID_CONN_STATE_CONNECTING_INTR,
	HID_CONN_STATE_CONFIG,
	HID_CONN_STATE_CONNECTED,
	HID_CONN_STATE_DISCONNECTING,
	HID_CONN_STATE_SECURITY,
};

typedef UINT8 tHID_CONN_FLAGS;
enum
{
	HID_CONN_FLAGS_IS_ORIG				= 1 << 0,

	HID_CONN_FLAGS_THEIR_CTRL_CFG_DONE	= 1 << 1,
	HID_CONN_FLAGS_OUR_CTRL_CFG_DONE	= 1 << 2,
	HID_CONN_FLAGS_THEIR_INTR_CFG_DONE	= 1 << 3,
	HID_CONN_FLAGS_OUR_INTR_CFG_DONE	= 1 << 4,

	HID_CONN_FLAGS_ALL_CONFIGURED		= HID_CONN_FLAGS_THEIR_CTRL_CFG_DONE
										| HID_CONN_FLAGS_OUR_CTRL_CFG_DONE
										| HID_CONN_FLAGS_THEIR_INTR_CFG_DONE
										| HID_CONN_FLAGS_OUR_INTR_CFG_DONE,

	HID_CONN_FLAGS_CONGESTED			= 1 << 5,
	HID_CONN_FLAGS_INACTIVE				= 1 << 6,
};

enum
{
	HID_SEC_CHN		= 1,
	HID_NOSEC_CHN	= 2,

#define HID_SEC_CHN		(HID_SEC_CHN + 0)
#define HID_NOSEC_CHN	(HID_NOSEC_CHN + 0)
};


typedef struct hid_conn
{
	tHID_CONN_STATE	conn_state;		// size 0x01, offset 0x00
	UINT8			conn_flags;		// size 0x01, offset 0x01
	UINT8			ctrl_id;		// size 0x01, offset 0x02
	/* 1 byte padding */
	UINT16			ctrl_cid;		// size 0x02, offset 0x04
	UINT16			intr_cid;		// size 0x02, offset 0x06
	UINT16			rem_mtu_size;	// size 0x02, offset 0x08
	UINT16			disc_reason;	// size 0x02, offset 0x0a
	TIMER_LIST_ENT	timer_entry;	// size 0x18, offset 0x0c
} tHID_CONN; // size 0x24

#ifdef __cplusplus
	}
#endif

#endif // BTE_HID_CONN_H
