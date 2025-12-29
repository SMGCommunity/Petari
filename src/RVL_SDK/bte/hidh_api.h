#ifndef BTE_HID_HOST_API_H
#define BTE_HID_HOST_API_H

/* Original source:
 * bluedroid <android.googlesource.com/platform/external/bluetooth/bluedroid>
 * stack/include/hidh_api.h
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

#include "bt_types.h"
#include "data_types.h"

#include "hiddefs.h"
#include "sdp_api.h"

/*******************************************************************************
 * types
 */

#ifdef __cplusplus
	extern "C" {
#endif

enum
{
	HID_SDP_NO_SERV_UUID		= SDP_ILLEGAL_PARAMETER + 1,
	HID_SDP_MANDATORY_MISSING,
};

enum
{
	HID_VIRTUAL_CABLE			= 1 <<  0,
	HID_NORMALLY_CONNECTABLE	= 1 <<  1,
	HID_RECONN_INIT				= 1 <<  2,
	HID_SDP_DISABLE				= 1 <<  3,
	HID_BATTERY_POWER			= 1 <<  4,
	HID_REMOTE_WAKE				= 1 <<  5,
	HID_SUP_TOUT_AVLBL			= 1 <<  6,
	HID_SSR_MAX_LATENCY			= 1 <<  7,
	HID_SSR_MIN_TOUT			= 1 <<  8,

	HID_SEC_REQUIRED			= 1 << 15,
};

enum
{
	HID_HDEV_EVT_OPEN,
	HID_HDEV_EVT_CLOSE,
	HID_HDEV_EVT_RETRYING,
	HID_HDEV_EVT_INTR_DATA,
	HID_HDEV_EVT_INTR_DATC,
	HID_HDEV_EVT_CTRL_DATA,
	HID_HDEV_EVT_CTRL_DATC,
	HID_HDEV_EVT_HANDSHAKE,
	HID_HDEV_EVT_VC_UNPLUG,
};

typedef void tHID_HOST_SDP_CALLBACK(UINT16 result, UINT16 attr_mask,
                                    tHID_DEV_SDP_INFO *sdp_rec);

typedef void tHID_HOST_DEV_CALLBACK(UINT8 dev_handle, UINT8 event, UINT32 data,
                                    BT_HDR *p_buf);

/*******************************************************************************
 * functions
 */

tHID_STATUS HID_HostGetSDPRecord(BD_ADDR addr, tSDP_DISCOVERY_DB *p_db,
                                 UINT32 db_len,
                                 tHID_HOST_SDP_CALLBACK *sdp_cback);

void HID_HostInit(void);
tHID_STATUS HID_HostRegister(tHID_HOST_DEV_CALLBACK *dev_cback);
tHID_STATUS HID_HostDeregister(void);
tHID_STATUS HID_HostAddDev(BD_ADDR addr, UINT16 attr_mask, UINT8 *handle);
tHID_STATUS HID_HostRemoveDev(UINT8 dev_handle);
tHID_STATUS HID_HostOpenDev(UINT8 dev_handle);
tHID_STATUS HID_HostWriteDev(UINT8 dev_handle, UINT8 t_type, UINT8 param,
                             UINT16 data, UINT8 report_id, BT_HDR *pbuf);
tHID_STATUS HID_HostCloseDev(UINT8 dev_handle);
tHID_STATUS HID_HostSetSecurityLevel(char *serv_name, UINT8 sec_lvl);

#ifdef __cplusplus
	}
#endif

#endif // BTE_HID_HOST_API_H
