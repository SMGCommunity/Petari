#ifndef BTE_BTA_API_H
#define BTE_BTA_API_H

/* Original source:
 * bluedroid <android.googlesource.com/platform/external/bluetooth/bluedroid>
 * bta/include/bta_api.h
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

#include "bt_types.h"
#include "data_types.h"

/*******************************************************************************
 * macros
 */

#define BTA_SERVICE_NAME_LEN	35

#define BTA_DM_PM_NUM_EVTS		9

#ifndef BTA_DM_PM_PARK_IDX
# define BTA_DM_PM_PARK_IDX		5
#endif

/*******************************************************************************
 * types
 */

#ifdef __cplusplus
	extern "C" {
#endif

typedef UINT8 tBTA_STATUS;
enum
{
	BTA_SUCCESS = 0,
	BTA_FAILURE = 1,
};

typedef UINT8 tBTA_SERVICE_ID;
enum
{
	BTA_MAX_SERVICE_ID	= 23, // MAX_SERVICE_ID is a limit, not a service id for a MAX service
};

typedef UINT32 tBTA_SERVICE_MASK;
#if 0
enum
{
	/* ... */
};
#endif

typedef UINT8 tBTA_SEC;
#if 0
enum
{
	/* ... */
};
#endif

typedef UINT8 tBTA_DM_DISC;
#if 0
enum
{
	/* ... */
};
#endif

typedef UINT8 tBTA_DM_CONN;
#if 0
enum
{
	/* ... */
};
#endif

typedef UINT8 tBTA_DM_INQ_MODE;
#if 0
enum
{
	/* ... */
};
#endif

typedef UINT8 tBTA_DM_INQ_FILT;
#if 0
enum
{
	/* ... */
};
#endif

typedef UINT8 tBTA_AUTH_RESP;
enum
{
	BTA_DM_AUTH_PERM	= 0,
	BTA_DM_NOT_AUTH		= 2,
};

typedef UINT8 tBTA_PREF_ROLES;
enum
{
	BTA_ANY_ROLE,
	BTA_MASTER_ROLE_PREF,
	BTA_MASTER_ROLE_ONLY,
};

// TODO: any associated type?
enum
{
	BTA_DM_NO_SCATTERNET		= 0,
	BTA_DM_PARTIAL_SCATTERNET	= 1,
};

typedef INT8 tBTA_DM_RSSI_VALUE;
typedef UINT8 tBTA_DM_LINK_QUALITY_VALUE;

typedef UINT8 tBTA_SIG_STRENGTH_MASK;
enum
{
	BTA_SIG_STRENGTH_RSSI_MASK = 1,
	BTA_SIG_STRENGTH_LINK_QUALITY_MASK,
};

typedef UINT8 tBTA_DM_SEC_EVT;
enum
{
	BTA_DM_DISABLE_EVT		= 1,
	BTA_DM_PIN_REQ_EVT		= 2,
	BTA_DM_AUTH_CMPL_EVT	= 3,
	BTA_DM_AUTHORIZE_EVT	= 4,
	BTA_DM_LINK_UP_EVT		= 5,
	BTA_DM_LINK_DOWN_EVT	= 6,
	BTA_DM_SIG_STRENGTH_EVT	= 7,
};

typedef UINT8 tBTA_DM_SEARCH_EVT;
enum
{
	BTA_DM_INQ_RES_EVT				= 0,
	BTA_DM_INQ_CMPL_EVT				= 1,
	BTA_DM_DISC_RES_EVT				= 2,
	BTA_DM_DISC_CMPL_EVT			= 3,
	BTA_DM_SEARCH_CANCEL_CMPL_EVT	= 4,
};

typedef UINT8 tBTA_DM_PM_ACTION;
enum
{
	BTA_DM_PM_NO_ACTION	= 0,
	BTA_DM_PM_PARK		= 1 << 0, // was 1 << 4
	BTA_DM_PM_SNIFF		= 1 << 1, // was 1 << 5
	BTA_DM_PM_ACTIVE	= 1 << 2, // was 1 << 6

	BTA_DM_PM_NO_PREF	= 1 << 4, // was 1 << 0
};

typedef void tBTA_APP_INFO_CBACK(tBTA_STATUS status);

typedef struct
{
	DEV_CLASS	dev_class;		// size 0x03, offset 0x00
	DEV_CLASS	dev_class_mask;	// size 0x03, offset 0x03
} tBTA_DM_COD_COND; // size 0x06

typedef union
{
	BD_ADDR				bd_addr;		// size 0x06, offset 0x00
	tBTA_DM_COD_COND	dev_class_cond;	// size 0x06, offset 0x06
} tBTA_DM_INQ_COND; // size 0x0c

typedef struct
{
	tBTA_DM_INQ_MODE	mode;			// size 0x01, offset 0x00
	UINT8				duration;		// size 0x01, offset 0x01
	UINT8				max_resps;		// size 0x01, offset 0x02
	tBTA_DM_INQ_FILT	filter_type;	// size 0x01, offset 0x03
	tBTA_DM_INQ_COND	filter_cond;	// size 0x0c, offset 0x04
} tBTA_DM_INQ; // size 0x10

typedef struct
{
	BD_ADDR	bd_addr;	// size 0x06, offset 0x00
} tBTA_DM_ENABLE; // size 0x06

typedef struct
{
	BD_ADDR		bd_addr;	// size 0x06, offset 0x00
	BD_NAME		bd_name;	// size 0xf8, offset 0x06
	DEV_CLASS	dev_class;	// size 0x03, offset 0xfe
} tBTA_DM_PIN_REQ; // size 0x101

typedef struct
{
	BD_ADDR		bd_addr;		// size 0x006, offset 0x000
	BD_NAME		bd_name;		// size 0x0f8, offset 0x006
	BOOLEAN		key_present;	// size 0x001, offset 0x0fe
	LINK_KEY	key;			// size 0x010, offset 0x0ff
	BOOLEAN		success;		// size 0x001, offset 0x10f
} tBTA_DM_AUTH_CMPL; // size 0x110

typedef struct
{
	BD_ADDR			bd_addr;	// size 0x06, offset 0x00
	BD_NAME			bd_name;	// size 0xf8, offset 0x06
	tBTA_SERVICE_ID	service;	// size 0x01, offset 0xfe
} tBTA_DM_AUTHORIZE; // size 0xff

typedef struct
{
	BD_ADDR	bd_addr;	// size 0x06, offset 0x00
} tBTA_DM_LINK_UP; // size 0x06

typedef struct
{
	BD_ADDR	bd_addr;	// size 0x06, offset 0x00
	UINT8	status;		// size 0x01, offset 0x06
} tBTA_DM_LINK_DOWN; // size 0x07

typedef struct
{
	BD_ADDR						bd_addr;			// size 0x06, offset 0x00
	tBTA_SIG_STRENGTH_MASK		mask;				// size 0x01, offset 0x06
	tBTA_DM_RSSI_VALUE			rssi_value;			// size 0x01, offset 0x07
	tBTA_DM_LINK_QUALITY_VALUE	link_quality_value;	// size 0x01, offset 0x08
} tBTA_DM_SIG_STRENGTH; // size 0x09

typedef struct
{
	UINT8	level;	// size 0x01, offset 0x00
} tBTA_DM_BUSY_LEVEL; // size 0x01

typedef union
{
	tBTA_DM_ENABLE			enable;			// size 0x006
	tBTA_DM_PIN_REQ			pin_req;		// size 0x101
	tBTA_DM_AUTH_CMPL		auth_cmpl;		// size 0x110
	tBTA_DM_AUTHORIZE		authorize;		// size 0x0ff
	tBTA_DM_LINK_UP			link_up;		// size 0x006
	tBTA_DM_LINK_DOWN		link_down;		// size 0x007
	tBTA_DM_SIG_STRENGTH	sig_strength;	// size 0x009
	tBTA_DM_BUSY_LEVEL		busy_level;		// size 0x001
} tBTA_DM_SEC; // size 0x110

typedef void tBTA_DM_SEC_CBACK(tBTA_DM_SEC_EVT event, tBTA_DM_SEC *p_data);

typedef struct
{
	BD_ADDR		bd_addr;				// size 0x06, offset 0x00
	DEV_CLASS	dev_class;				// size 0x03, offset 0x06
	INT8		rssi;					// size 0x01, offset 0x09
	BOOLEAN		remt_name_not_required;	// size 0x01, offset 0x0a
} tBTA_DM_INQ_RES; // size 0x0b

typedef struct
{
	UINT8	num_resps;	// size 0x01, offset 0x00
} tBTA_DM_INQ_CMPL; // size 0x01

typedef struct
{
	BD_ADDR				bd_addr;	// size 0x006, offset 0x000
	BD_NAME				bd_name;	// size 0x0f8, offset 0x006
	/* 2 bytes padding */
	tBTA_SERVICE_MASK	services;	// size 0x004, offset 0x100
	tBTA_STATUS			result;		// size 0x001, offset 0x104
	/* 3 bytes padding */
} tBTA_DM_DISC_RES; // size 0x108

typedef union
{
	tBTA_DM_INQ_RES		inq_res;	// size 0x00b
	tBTA_DM_INQ_CMPL	inq_cmpl;	// size 0x001
	tBTA_DM_DISC_RES	disc_res;	// size 0x108
} tBTA_DM_SEARCH; // size 0x108

typedef void tBTA_DM_SEARCH_CBACK(tBTA_DM_SEARCH_EVT event,
                                  tBTA_DM_SEARCH *p_data);

/*******************************************************************************
 * functions
 */

/* bta_dm_api.h */
tBTA_STATUS BTA_EnableBluetooth(tBTA_DM_SEC_CBACK *p_cback);
void BTA_DisableBluetooth(void);
BOOLEAN BTA_DmIsDeviceUp(void);
void BTA_DmSetDeviceName(char *p_name);
void BTA_DmSetVisibility(tBTA_DM_DISC disc_mode, tBTA_DM_CONN conn_mode);
void BTA_DmSearch(tBTA_DM_INQ *p_dm_inq, tBTA_SERVICE_MASK services,
                  tBTA_DM_SEARCH_CBACK *p_cback);
void BTA_DmSearchCancel(void);
void BTA_DmDiscover(BD_ADDR bd_addr, tBTA_SERVICE_MASK services,
                    tBTA_DM_SEARCH_CBACK *p_cback);
void BTA_DmBond(BD_ADDR bd_addr, UINT8 pin_len, PIN_CODE p_pin);
void BTA_DmPinReply(BD_ADDR bd_addr, BOOLEAN accept, UINT8 pin_len,
                    PIN_CODE p_pin);
tBTA_STATUS BTA_DmAddDevice(BD_ADDR bd_addr, LINK_KEY link_key,
                            UINT32 trusted_mask, BOOLEAN is_trusted);
tBTA_STATUS BTA_DmRemoveDevice(BD_ADDR bd_addr);
void BTA_DmAuthorizeReply(BD_ADDR bd_addr, tBTA_SERVICE_ID service,
                          tBTA_AUTH_RESP response);
void BTA_DmSignalStrength(tBTA_SIG_STRENGTH_MASK mask, UINT16 period,
                          BOOLEAN start);
void BTA_KeepACLLinksOnShutDown(BOOLEAN keep);
void BTA_DmSendHciReset(void);

/* bte_main.c */
tBTA_STATUS BTA_Init(void);
void BTA_CleanUp(tBTA_APP_INFO_CBACK *p_cb);

#ifdef __cplusplus
	}
#endif

#endif // BTE_BTA_API_H
