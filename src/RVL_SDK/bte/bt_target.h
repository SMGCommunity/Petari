#ifndef BTE_BT_TARGET_H
#define BTE_BT_TARGET_H

/* Original source:
 * bluedroid <android.googlesource.com/platform/external/bluetooth/bluedroid>
 * include/bt_target.h
 */

/******************************************************************************
 *
 *  Copyright (C) 1999-2012 Broadcom Corporation
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

#include "gki_target.h"

/*******************************************************************************
 * Configuration options, sorted as seen in bluedroid's bt_target.h
 */

/* Added from bluedroid buildcfg.h */

#ifndef BTA_RFC_MTU_SIZE
# define BTA_RFC_MTU_SIZE					(L2CAP_MTU_SIZE - L2CAP_MIN_OFFSET - RFCOMM_DATA_OVERHEAD)
#endif

#define BTA_DISABLE_DELAY					1000

#ifndef BTA_DISABLE_DELAY
# define BTA_DISABLE_DELAY					200
#endif

#define BTA_DM_REMOTE_DEVICE_NAME_LENGTH	32

#ifndef BTA_DM_REMOTE_DEVICE_NAME_LENGTH
# define BTA_DM_REMOTE_DEVICE_NAME_LENGTH	248
#endif

#define BTA_DM_SDP_DB_SIZE					250

#ifndef BTA_DM_SDP_DB_SIZE
# define BTA_DM_SDP_DB_SIZE					8000
#endif

/* GKI pools */

#ifndef HCI_CMD_POOL_ID
# define HCI_CMD_POOL_ID					GKI_POOL_ID_2
#endif

#define SDP_POOL_ID							GKI_POOL_ID_2

#ifndef SDP_POOL_ID
# define SDP_POOL_ID						GKI_POOL_ID_3
#endif

#ifndef RFCOMM_CMD_POOL_ID
# define RFCOMM_CMD_POOL_ID					GKI_POOL_ID_2
#endif

#ifndef RFCOMM_DATA_POOL_ID
# define RFCOMM_DATA_POOL_ID				GKI_POOL_ID_3
#endif

#ifndef RFCOMM_DATA_POOL_BUF_SIZE
# define RFCOMM_DATA_POOL_BUF_SIZE			GKI_BUF3_SIZE
#endif

#ifndef L2CAP_CMD_POOL_ID
# define L2CAP_CMD_POOL_ID					GKI_POOL_ID_2
#endif

#ifndef GAP_DATA_POOL_ID
# define GAP_DATA_POOL_ID					GKI_POOL_ID_3
#endif

/* Lower layer interface */

#define HCI_GET_CMD_BUF(paramlen)			GKI_getpoolbuf(HCI_CMD_POOL_ID)

/* HCI services */

#ifndef HCI_ACL_DATA_TO_LOWER
#if !defined(BTE_HCI_H)
void bte_hcisu_send(BT_HDR *p_msg, UINT16 event);
#endif
# define HCI_ACL_DATA_TO_LOWER(p)			bte_hcisu_send((BT_HDR *)(p), BT_EVT_TO_LM_HCI_ACL);
#endif

#ifndef BTU_CMD_CMPL_TIMEOUT
# define BTU_CMD_CMPL_TIMEOUT				8
#endif

#define BT_1SEC_TIMEOUT						1

#ifndef BT_1SEC_TIMEOUT
# define BT_1SEC_TIMEOUT					2
#endif

/* BTM */

#define BTM_INQ_DB_SIZE						12

#ifndef BTM_INQ_DB_SIZE
# define BTM_INQ_DB_SIZE					40
#endif

#ifndef BTM_DEFAULT_SCAN_TYPE
# define BTM_DEFAULT_SCAN_TYPE				BTM_SCAN_TYPE_INTERLACED
#endif

// CLEANUP: header pass, probably
#ifndef BTM_DEFAULT_CONN_WINDOW
# define BTM_DEFAULT_CONN_WINDOW			18
#endif

#ifndef BTM_DEFAULT_CONN_INTERVAL
# define BTM_DEFAULT_CONN_INTERVAL			2048
#endif

#ifndef BTM_DEFAULT_DISC_MODE
# define BTM_DEFAULT_DISC_MODE				BTM_GENERAL_DISCOVERABLE
#endif

// CLEANUP: header pass, probably
#ifndef BTM_DEFAULT_DISC_WINDOW
# define BTM_DEFAULT_DISC_WINDOW			18
#endif

#ifndef BTM_DEFAULT_DISC_INTERVAL
# define BTM_DEFAULT_DISC_INTERVAL			2048
#endif

/* Major service class: 0x400 - Telephony
 * Major  device class:     1 - Phone
 * Minor  device class:     1 - Cellular
 */
#define BTA_DM_COD							{0x40, 0x02, 0x04}

#ifndef BTA_DM_COD
# define BTA_DM_COD							{0x5a, 0x02, 0x0c}
#endif

#define BTM_MAX_SCO_LINKS					3

#ifndef BTM_MAX_SCO_LINKS
# define BTM_MAX_SCO_LINKS					2
#endif

#ifndef BTM_DEFAULT_SCO_MODE
# define BTM_DEFAULT_SCO_MODE				2
#endif

#define BTM_SEC_MAX_DEVICE_RECORDS			16

#ifndef BTM_SEC_MAX_DEVICE_RECORDS
# define BTM_SEC_MAX_DEVICE_RECORDS			100
#endif

#define BTM_SEC_MAX_SERVICE_RECORDS			24

#ifndef BTM_SEC_MAX_SERVICE_RECORDS
# define BTM_SEC_MAX_SERVICE_RECORDS		32
#endif

#define BTM_MAX_REM_BD_NAME_LEN				64

#ifndef BTM_MAX_REM_BD_NAME_LEN
# define BTM_MAX_REM_BD_NAME_LEN			248
#endif

#define BTM_MAX_LOC_BD_NAME_LEN				31

#ifndef BTM_MAX_LOC_BD_NAME_LEN
# define BTM_MAX_LOC_BD_NAME_LEN			248
#endif

#ifndef BTM_SEC_SERVICE_NAME_LEN
# define BTM_SEC_SERVICE_NAME_LEN			BT_MAX_SERVICE_NAME_LEN
#endif

#ifndef BT_MAX_SERVICE_NAME_LEN
# define BT_MAX_SERVICE_NAME_LEN			21
#endif

#ifndef BTM_PWR_MGR_INCLUDED
# define BTM_PWR_MGR_INCLUDED				TRUE
#endif

#ifndef BTM_MAX_PM_RECORDS
# define BTM_MAX_PM_RECORDS					2
#endif

/* L2CAP */

// TODO: could this be resolved by configuring MAX_ACL_CONNECTIONS?
#if 0
#define MAX_ACL_CONNECTIONS					4

#ifndef MAX_ACL_CONNECTIONS
# define MAX_L2CAP_LINKS					7
#else
# define MAX_L2CAP_LINKS					MAX_ACL_CONNECTIONS
#endif
#else
# define MAX_L2CAP_LINKS					4
#endif

#define MAX_L2CAP_CHANNELS					10

#ifndef MAX_L2CAP_CHANNELS
# define MAX_L2CAP_CHANNELS					16
#endif

#define MAX_L2CAP_CLIENTS					8

#ifndef MAX_L2CAP_CLIENTS
# define MAX_L2CAP_CLIENTS					15
#endif

#define L2CAP_LINK_INACTIVITY_TOUT			2

#ifndef L2CAP_LINK_INACTIVITY_TOUT
# define L2CAP_LINK_INACTIVITY_TOUT			4
#endif

#ifndef L2CAP_LINK_STARTUP_TOUT
# define L2CAP_LINK_STARTUP_TOUT			60
#endif

#ifndef L2CAP_MTU_SIZE
# define L2CAP_MTU_SIZE						1691
#endif

#ifndef L2CAP_HOST_FC_ACL_BUFS
# define L2CAP_HOST_FC_ACL_BUFS				20
#endif

#ifndef L2CAP_DESIRED_LINK_ROLE
# define L2CAP_DESIRED_LINK_ROLE			HCI_ROLE_SLAVE
#endif

/* SDP */

#ifndef SDP_MAX_RECORDS
# define SDP_MAX_RECORDS					20
#endif

#ifndef SDP_MAX_REC_ATTR
# define SDP_MAX_REC_ATTR					25
#endif

#define SDP_MAX_PAD_LEN						350

#ifndef SDP_MAX_PAD_LEN
# define SDP_MAX_PAD_LEN					600
#endif

#define SDP_MAX_ATTR_LEN					80

#ifndef SDP_MAX_ATTR_LEN
# if defined(HID_DEV_INCLUDED) && HID_DEV_INCLUDED == TRUE
// probably went here
#  define SDP_MAX_ATTR_LEN					80
# else
#  define SDP_MAX_ATTR_LEN					100
# endif
#endif

#define SDP_MAX_ATTR_FILTERS				12

#ifndef SDP_MAX_ATTR_FILTERS
# define SDP_MAX_ATTR_FILTERS				15
#endif

#ifndef SDP_MAX_UUID_FILTERS
# define SDP_MAX_UUID_FILTERS				3
#endif

#ifndef SDP_MAX_DISC_SERVER_RECS
# define SDP_MAX_DISC_SERVER_RECS			21
#endif

#define SDP_MAX_LIST_BYTE_COUNT				1000

#ifndef SDP_MAX_LIST_BYTE_COUNT
# define SDP_MAX_LIST_BYTE_COUNT			0x1000
#endif

#ifndef SDP_MAX_PROTOCOL_PARAMS
# define SDP_MAX_PROTOCOL_PARAMS			2
#endif

#ifndef SDP_MAX_CONNECTIONS
# define SDP_MAX_CONNECTIONS				4
#endif

#ifndef SDP_MTU_SIZE
# define SDP_MTU_SIZE						256
#endif

#ifndef SDP_FLUSH_TO
# define SDP_FLUSH_TO						0xffff
#endif

#ifndef SDP_SERVICE_NAME
# define SDP_SERVICE_NAME					"Service Discovery"
#endif

#ifndef SDP_SECURITY_LEVEL
# define SDP_SECURITY_LEVEL					BTM_SEC_NONE
#endif

/* RFCOMM */

#define MAX_RFC_PORTS						5

#ifndef MAX_RFC_PORTS
# define MAX_RFC_PORTS						30
#endif

// Well I guess not since this is different than MAX_L2CAP_LINKS
#if 0
#define MAX_ACL_CONNECTIONS					4

#ifndef MAX_ACL_CONNECTIONS
# define MAX_BD_CONNECTIONS					7
#else
# define MAX_BD_CONNECTIONS					MAX_ACL_CONNECTIONS
#endif
#else
# define MAX_BD_CONNECTIONS					1
#endif

#define PORT_RX_LOW_WM						5000

#ifndef PORT_RX_LOW_WM
# define PORT_RX_LOW_WM						(BTA_RFC_MTU_SIZE * PORT_RX_BUF_LOW_WM)
#endif

#define PORT_RX_HIGH_WM						8000

#ifndef PORT_RX_HIGH_WM
# define PORT_RX_HIGH_WM					(BTA_RFC_MTU_SIZE * PORT_RX_BUF_HIGH_WM)
#endif

#define PORT_RX_CRITICAL_WM					12000

#ifndef PORT_RX_CRITICAL_WM
# define PORT_RX_CRITICAL_WM				(BTA_RFC_MTU_SIZE * PORT_RX_BUF_CRITICAL_WM)
#endif

#define PORT_RX_BUF_LOW_WM					8

#ifndef PORT_RX_BUF_LOW_WM
# define PORT_RX_BUF_LOW_WM					4
#endif

#define PORT_RX_BUF_HIGH_WM					16

#ifndef PORT_RX_BUF_HIGH_WM
# define PORT_RX_BUF_HIGH_WM				10
#endif

#ifndef PORT_RX_BUF_CRITICAL_WM
# define PORT_RX_BUF_CRITICAL_WM			15
#endif

#define PORT_TX_HIGH_WM						8000

#ifndef PORT_TX_HIGH_WM
# define PORT_TX_HIGH_WM					(BTA_RFC_MTU_SIZE * PORT_TX_BUF_HIGH_WM)
#endif

#define PORT_TX_CRITICAL_WM					10000

#ifndef PORT_TX_CRITICAL_WM
# define PORT_TX_CRITICAL_WM				(BTA_RFC_MTU_SIZE * PORT_TX_BUF_CRITICAL_WM)
#endif

#define PORT_TX_BUF_HIGH_WM					16

#ifndef PORT_TX_BUF_HIGH_WM
# define PORT_TX_BUF_HIGH_WM				10
#endif

#define PORT_TX_BUF_CRITICAL_WM				22

#ifndef PORT_TX_BUF_CRITICAL_WM
# define PORT_TX_BUF_CRITICAL_WM			15
#endif

#ifndef PORT_FC_DEFAULT
# define PORT_FC_DEFAULT					PORT_FC_CREDIT
#endif

/* GAP */

#ifndef GAP_MAX_CONNECTIONS
# define GAP_MAX_CONNECTIONS				8
#endif

/* SPP */

#ifndef SDP_RAW_DATA_INCLUDED
# define SDP_RAW_DATA_INCLUDED				TRUE
#endif

/* HID */

#ifndef HID_CONTROL_POOL_ID
# define HID_CONTROL_POOL_ID				GKI_POOL_ID_2
#endif

#ifndef HID_INTERRUPT_POOL_ID
# define HID_INTERRUPT_POOL_ID				GKI_POOL_ID_2
#endif

/* HID Host & Device */

#ifndef HID_MAX_SVC_NAME_LEN
# define HID_MAX_SVC_NAME_LEN				32
#endif

#ifndef HID_MAX_SVC_DESCR_LEN
# define HID_MAX_SVC_DESCR_LEN				32
#endif

#ifndef HID_MAX_PROV_NAME_LEN
# define HID_MAX_PROV_NAME_LEN				32
#endif

/* HID Host */

#define HID_HOST_MAX_DEVICES				16

#ifndef HID_HOST_MAX_DEVICES
# define HID_HOST_MAX_DEVICES				7
#endif

#ifndef HID_HOST_MTU
# define HID_HOST_MTU						640
#endif

#ifndef HID_HOST_FLUSH_TO
# define HID_HOST_FLUSH_TO					0xffff
#endif

#define HID_HOST_MAX_CONN_RETRY				0

#ifndef HID_HOST_MAX_CONN_RETRY
# define HID_HOST_MAX_CONN_RETRY			3
#endif

#ifndef HID_HOST_REPAGE_WIN
# define HID_HOST_REPAGE_WIN				2
#endif

/* Application Task */

#define L2CAP_FEATURE_REQ_ID				73

#define L2CAP_FEATURE_RSP_ID				173

#endif // BTE_BT_TARGET_H
