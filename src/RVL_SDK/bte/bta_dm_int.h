#ifndef BTE_BTA_DM_INT_H
#define BTE_BTA_DM_INT_H

/* Original source:
 * bluedroid <android.googlesource.com/platform/external/bluetooth/bluedroid>
 * bta/dm/bta_dm_int.h
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

#include <decomp.h>

#include "bt_target.h"
#include "bt_types.h"
#include "data_types.h"

#include "bta_api.h"
#include "bta_sys.h"
#include "btm_api.h"
#include "gki.h"
#include "hcidefs.h"
#include "sdp_api.h"

/*******************************************************************************
 * macros
 */

#define BTA_ALL_APP_ID				255

#define BTA_DM_NUM_PEER_DEVICE		7
#define BTA_DM_NUM_CONN_SRVS		5
#define BTA_DM_NUM_COMPRESS_SRVS	5
#define BTA_DM_NUM_PM_TIMER			3

#define BTA_COPY_DEVICE_CLASS(coddst, codsrc)				\
	do														\
	{														\
		((UINT8 *)(coddst))[0] = ((UINT8 *)(codsrc))[0];	\
		((UINT8 *)(coddst))[1] = ((UINT8 *)(codsrc))[1];	\
		((UINT8 *)(coddst))[2] = ((UINT8 *)(codsrc))[2];	\
	} while (FALSE)

#define BTA_SERVICE_ID_TO_SERVICE_MASK(id)	(1 << (id))

/*******************************************************************************
 * types
 */

#ifdef __cplusplus
	extern "C" {
#endif

enum
{
	BTA_DM_API_ENABLE_EVT = BTA_SYS_EVT_START(BTA_ID_DM),
	BTA_DM_API_DISABLE_EVT,
	BTA_DM_API_SET_NAME_EVT,
	BTA_DM_API_SET_VISIBILITY_EVT,
	BTA_DM_API_SIG_STRENGTH_EVT,	// was 5
	BTA_DM_ACL_CHANGE_EVT,			// was 8
	BTA_DM_API_BOND_EVT,			// was 10
	BTA_DM_API_PIN_REPLY_EVT,		// was 12
	BTA_DM_API_AUTH_REPLY_EVT,		// was 14
	BTA_DM_PM_BTM_STATUS_EVT,		// was 15
	BTA_DM_PM_TIMER_EVT,			// was 16
	BTA_DM_API_KEEP_ACL_LINKS,		// did not exist
	BTA_DM_API_RESET_HCI,			// did not exist

	BTA_DM_MAX_EVENT
};

enum
{
	BTA_DM_API_SEARCH_EVT = BTA_SYS_EVT_START(BTA_ID_DM_SEARCH),
	BTA_DM_API_SEARCH_CANCEL_EVT,
	BTA_DM_API_DISCOVER_EVT,
	BTA_DM_INQUIRY_CMPL_EVT,
	BTA_DM_REMT_NAME_EVT,
	BTA_DM_REMT_NAME_RESP_EVT,	// did not exist
	BTA_DM_SDP_RESULT_EVT,		// was 5
	BTA_DM_SEARCH_CMPL_EVT,		// was 6
	BTA_DM_DISCOVERY_RESULT_EVT,
};

typedef UINT8 tBTA_DM_CONN_STATE;
enum
{
	BTA_DM_CONNECTED	= 1,
	BTA_DM_UNPAIRING	= 2,
};

typedef UINT8 tBTA_DM_COMPRESS_STATE;
enum
{
	BTA_COMPRESS_STATE_0	= 0,
	BTA_COMPRESS_STATE_1	= 1,
};

enum
{
	BTA_DM_SEARCH_IDLE,
	BTA_DM_SEARCH_ACTIVE,
	BTA_DM_SEARCH_CANCELLING,
	BTA_DM_DISCOVER_ACTIVE,
};

typedef struct
{
	BT_HDR				hdr;			// size 0x08, offset 0x00
	tBTA_DM_SEC_CBACK	*p_sec_cback;	// size 0x04, offset 0x08
} tBTA_DM_API_ENABLE; // size 0x0c

typedef struct
{
	BT_HDR	hdr;									// size 0x08, offset 0x00
	char	name[BTA_DM_REMOTE_DEVICE_NAME_LENGTH];	// size 0x20, offset 0x08 // was BD_NAME_LEN (248)
} tBTA_DM_API_SET_NAME; // size 0x28

typedef struct
{
	BT_HDR			hdr;		// size 0x08, offset 0x00
	tBTA_DM_DISC	disc_mode;	// size 0x01, offset 0x08
	tBTA_DM_CONN	conn_mode;	// size 0x01, offset 0x09
} tBTA_DM_API_SET_VISIBILITY; // size 0x0a

typedef struct
{
	BT_HDR					hdr;		// size 0x08, offset 0x00
	tBTA_DM_INQ				inq_params;	// size 0x0a, offset 0x08
	/* 2 bytes padding */
	tBTA_SERVICE_MASK		services;	// size 0x04, offset 0x14
	tBTA_DM_SEARCH_CBACK	*p_cback;	// size 0x04, offset 0x18
} tBTA_DM_API_SEARCH; // size 0x1c

typedef struct
{
	BT_HDR					hdr;		// size 0x08, offset 0x00
	BD_ADDR					bd_addr;	// size 0x06, offset 0x08
	/* 2 bytes padding */
	tBTA_SERVICE_MASK		services;	// size 0x04, offset 0x10
	tBTA_DM_SEARCH_CBACK	*p_cback;	// size 0x04, offset 0x14
} tBTA_DM_API_DISCOVER; // size 0x18

typedef struct
{
	BT_HDR	hdr;					// size 0x08, offset 0x00
	BD_ADDR	bd_addr;				// size 0x06, offset 0x08
	UINT8	pin_len;				// size 0x01, offset 0x0e
	UINT8	p_pin[PIN_CODE_LEN];	// size 0x10, offset 0x0f
	/* 1 byte padding */
} tBTA_DM_API_BOND; // size 0x20

typedef struct
{
	BT_HDR	hdr;					// size 0x08, offset 0x00
	BD_ADDR	bd_addr;				// size 0x06, offset 0x08
	BOOLEAN	accept;					// size 0x01, offset 0x0e
	UINT8	pin_len;				// size 0x01, offset 0x0f
	UINT8	p_pin[PIN_CODE_LEN];	// size 0x10, offset 0x10
} tBTA_DM_API_PIN_REPLY; // size 0x20

typedef struct
{
	BT_HDR			hdr;		// size 0x08, offset 0x00
	BD_ADDR			bd_addr;	// size 0x06, offset 0x08
	tBTA_SERVICE_ID	service;	// size 0x01, offset 0x0e
	tBTA_AUTH_RESP	response;	// size 0x01, offset 0x0f
} tBTA_DM_API_AUTH_REPLY; // size 0x10

typedef struct
{
	BT_HDR			hdr;	// size 0x008, offset 0x000
	tBTA_DM_SEARCH	result;	// size 0x108, offset 0x008
} tBTA_DM_REM_NAME; // size 0x110

typedef struct
{
	BT_HDR			hdr;	// size 0x008, offset 0x000
	tBTA_DM_SEARCH	result;	// size 0x108, offset 0x008
} tBTA_DM_DISC_RESULT; // size 0x110

typedef struct
{
	BT_HDR	hdr;	// size 0x08, offset 0x00
	UINT8	num;	// size 0x01, offset 0x08
	/* 1 byte padding */
} tBTA_DM_INQUIRY_CMPL; // size 0x0a

typedef struct
{
	BT_HDR	hdr;		// size 0x08, offset 0x00
	UINT16	sdp_result;	// size 0x02, offset 0x08
} tBTA_DM_SDP_RESULT; // size 0x0a

typedef struct
{
	BT_HDR					hdr;	// size 0x08, offset 0x00
	tBTA_SIG_STRENGTH_MASK	mask;	// size 0x01, offset 0x08
	/* 1 byte padding */
	UINT16					period;	// size 0x02, offset 0x0a
	BOOLEAN					start;	// size 0x01, offset 0x0c
	/* 1 byte padding */
} tBTA_API_DM_SIG_STRENGTH; // size 0x0e

typedef struct
{
	BT_HDR			hdr;		// size 0x08, offset 0x00
	tBTM_BL_EVENT	event;		// size 0x01, offset 0x08
	UINT8			busy_level;	// size 0x01, offset 0x09
	BOOLEAN			is_new;		// size 0x01, offset 0x0a
	BD_ADDR			bd_addr;	// size 0x06, offset 0x0b
	/* 1 byte padding */
} tBTA_DM_ACL_CHANGE; // size 0x12

typedef struct
{
	BT_HDR			hdr;		// size 0x08, offset 0x00
	BD_ADDR			bd_addr;	// size 0x06, offset 0x08
	tBTM_PM_STATUS	status;		// size 0x01, offset 0x0e
	/* 1 byte padding */
	UINT16			value;		// size 0x02, offset 0x10
	tHCI_STATUS		hci_status;	// size 0x01, offset 0x12
	/* 1 byte padding */
} tBTA_DM_PM_BTM_STATUS; // size 0x14

typedef struct
{
	BT_HDR	hdr;		// size 0x08, offset 0x00
	BD_ADDR	bd_addr;	// size 0x06, offset 0x08
} tBTA_DM_PM_TIMER; // size 0x0e

typedef union
{
	BT_HDR						hdr;			// size 0x008
	tBTA_DM_API_ENABLE			enable;			// size 0x00c
	tBTA_DM_API_SET_NAME		set_name;		// size 0x028
	tBTA_DM_API_SET_VISIBILITY	set_visibility;	// size 0x00a
	tBTA_DM_API_SEARCH			search;			// size 0x01c
	tBTA_DM_API_DISCOVER		discover;		// size 0x018
	tBTA_DM_API_BOND			bond;			// size 0x020
	tBTA_DM_API_PIN_REPLY		pin_reply;		// size 0x020
	tBTA_DM_API_AUTH_REPLY		auth_reply;		// size 0x010
	tBTA_DM_REM_NAME			rem_name;		// size 0x110
	tBTA_DM_DISC_RESULT			disc_result;	// size 0x110
	tBTA_DM_INQUIRY_CMPL		inq_cmpl;		// size 0x00a
	tBTA_DM_SDP_RESULT			sdp_event;		// size 0x00a
	tBTA_API_DM_SIG_STRENGTH	sig_strength;	// size 0x00e
	tBTA_DM_ACL_CHANGE			acl_change;		// size 0x012
	tBTA_DM_PM_BTM_STATUS		pm_status;		// size 0x014
	tBTA_DM_PM_TIMER			pm_timer;		// size 0x00e
} tBTA_DM_MSG; // size 0x110

typedef struct
{
	BD_ADDR				peer_bdaddr;		// size 0x06, offset 0x00
	tBTA_DM_CONN_STATE	conn_state;			// size 0x01, offset 0x06
	tBTA_PREF_ROLES		pref_role;			// size 0x01, offset 0x07
	BOOLEAN				in_use;				// size 0x01, offset 0x08
	tBTA_DM_PM_ACTION	pm_mode_attempted;	// size 0x01, offset 0x09
	tBTA_DM_PM_ACTION	pm_mode_failed;		// size 0x01, offset 0x0a
} tBTA_DM_PEER_DEVICE; // size 0x0b

typedef struct
{
	tBTA_DM_PEER_DEVICE	peer_device[BTA_DM_NUM_PEER_DEVICE];	// size 0x4d, offset 0x00
	UINT8				count;									// size 0x01, offset 0x4d
} tBTA_DM_ACTIVE_LINK; // size 0x4e

typedef struct
{
	BD_ADDR					peer_bdaddr;	// size 0x06, offset 0x00
	tBTA_SYS_ID				id;				// size 0x01, offset 0x06
	UINT8					app_id;			// size 0x01, offset 0x07
	tBTA_SYS_CONN_STATUS	state;			// size 0x01, offset 0x08
} tBTA_DM_SRVCS; // size 0x09

typedef struct
{
	UINT8			count;								// size 0x01, offset 0x00
	tBTA_DM_SRVCS	conn_srvc[BTA_DM_NUM_CONN_SRVS];	// size 0x2d, offset 0x01
} tBTA_DM_CONNECTED_SRVCS; // size 0x2e

typedef struct
{
	BD_ADDR					peer_bdaddr;	// size 0x06, offset 0x00
	tBTA_SYS_ID				id;				// size 0x01, offset 0x06
	UINT8					app_id;			// size 0x01, offset 0x07
	tBTA_DM_COMPRESS_STATE	state;			// size 0x01, offset 0x08
} tBTA_DM_COMPRESS_SRVCS; // size 0x09

typedef struct
{
	TIMER_LIST_ENT	timer;			// size 0x18, offset 0x00
	BD_ADDR			peer_bdaddr;	// size 0x06, offset 0x18
	BOOLEAN			in_use;			// size 0x01, offset 0x1e
	/* 1 byte padding */
} tBTA_PM_TIMER; // size 0x20

typedef struct
{
	tBTA_DM_ACTIVE_LINK		device_list;					// size 0x04e, offset 0x000
	/* 2 bytes padding */
	tBTA_DM_SEC_CBACK		*p_sec_cback;					// size 0x004, offset 0x050
	TIMER_LIST_ENT			signal_strength_timer;			// size 0x018, offset 0x054
	tBTA_SIG_STRENGTH_MASK	signal_strength_mask;			// size 0x001, offset 0x06c
	/* 1 byte padding */
	UINT16					state;							// size 0x002, offset 0x06e
	UINT16					signal_strength_period;			// size 0x002, offset 0x070
	BOOLEAN					disabling;						// size 0x001, offset 0x072
	/* 1 byte padding */
	TIMER_LIST_ENT			disable_timer;					// size 0x018, offset 0x074
	UINT8					num_master_only;				// size 0x001, offset 0x08c
	/* 3 bytes padding */
	UINT32					wbt_sdp_handle;					// size 0x004, offset 0x090
	UINT8					wbt_scn;						// size 0x001, offset 0x094
	UINT8					pm_id;							// size 0x001, offset 0x095
	/* 2 bytes padding */
	tBTA_PM_TIMER			pm_timer[BTA_DM_NUM_PM_TIMER];	// size 0x060, offset 0x098
	BD_ADDR					pin_bd_addr;					// size 0x006, offset 0x0f8
	DEV_CLASS				pin_dev_class;					// size 0x003, offset 0x0fe
	BOOLEAN					keep_acl_on_shut_down;			// size 0x001, offset 0x101
	/* 2 bytes padding */
} tBTA_DM_CB; // size 0x104

typedef struct
{
	tBTA_DM_SEARCH_CBACK	*p_search_cback;								// size 0x04, offset 0x00
	tBTM_INQ_INFO			*p_btm_inq_info;								// size 0x04, offset 0x04
	tBTA_SERVICE_MASK		services;										// size 0x04, offset 0x08
	tBTA_SERVICE_MASK		services_to_search;								// size 0x04, offset 0x0c
	tBTA_SERVICE_MASK		services_found;									// size 0x04, offset 0x10
	tSDP_DISCOVERY_DB		*p_sdp_db;										// size 0x04, offset 0x14
	UINT16					state;											// size 0x02, offset 0x18
	BD_ADDR					peer_bdaddr;									// size 0x06, offset 0x1a
	char					peer_name[BTA_DM_REMOTE_DEVICE_NAME_LENGTH];	// size 0x20, offset 0x20 // was BD_NAME_LEN (248)
	TIMER_LIST_ENT			search_timer;									// size 0x18, offset 0x40
	TIMER_LIST_ENT			rnr_timer;										// size 0x18, offset 0x58
	UINT8					service_index;									// size 0x01, offset 0x70
	/* 3 bytes padding */
	tBTA_DM_MSG				*p_search_queue;								// size 0x04, offset 0x74
	BOOLEAN					wait_disc;										// size 0x01, offset 0x78
	BOOLEAN					sdp_results;									// size 0x01, offset 0x79
	/* 2 bytes padding */
} tBTA_DM_SEARCH_CB; // size 0x7c

typedef struct
{
	DEV_CLASS	dev_class;			// size 0x03, offset 0x00
	/* 1 byte padding */
	UINT16		policy_settings;	// size 0x02, offset 0x04
	UINT16		page_timeout;		// size 0x02, offset 0x06
	UINT16		link_timeout;		// size 0x02, offset 0x08
} tBTA_DM_CFG; // size 0x0a

typedef struct
{
	UINT8	id;		// size 0x01, offset 0x00
	UINT8	app_id;	// size 0x01, offset 0x01
	UINT8	cfg;	// size 0x01, offset 0x02
} tBTA_DM_RM; // size 0x03

typedef struct
{
	UINT8	id;			// size 0x01, offset 0x00
	UINT8	app_id;		// size 0x01, offset 0x01
	UINT8	spec_idx;	// size 0x01, offset 0x02
} tBTA_DM_PM_CFG; // size 0x03

typedef struct
{
	tBTA_DM_PM_ACTION	power_mode;	// size 0x01, offset 0x00
	/* 1 byte padding */
	UINT16				timeout;	// size 0x02, offset 0x02
} tBTA_DM_PM_ACTN; // size 0x04

typedef struct
{
	UINT8			allow_mask;							// size 0x01, offset 0x00
	/* 1 byte padding */
	tBTA_DM_PM_ACTN	actn_tbl[BTA_DM_PM_NUM_EVTS][2];	// size 0x48, offset 0x02
} tBTA_DM_PM_SPEC; // size 0x4a

typedef struct
{
	UINT8	id;		// size 0x01, offset 0x00
	UINT8	app_id;	// size 0x01, offset 0x01
	UINT8	mask;	// size 0x01, offset 0x02
} tBTA_DM_COMPRESS; // size 0x03

/*******************************************************************************
 * external globals
 */

/* bta_dm_act.c */

/* NOTE: bta_service_id_to_btm_srv_id_lkup_tbl is listed in DWARF info as being
 * an incomplete array type for those units without its definition
 */

extern UINT16 const bta_service_id_to_uuid_lkup_tbl[/* BTA_MAX_SERVICE_ID */];
extern UINT32 const
	bta_service_id_to_btm_srv_id_lkup_tbl[/* BTA_MAX_SERVICE_ID */];

// ---

extern tBTA_DM_CONNECTED_SRVCS bta_dm_conn_srvcs;

extern tBTA_DM_CB bta_dm_cb;
extern tBTA_DM_SEARCH_CB bta_dm_search_cb;

extern tBTA_DM_CFG const bta_dm_cfg;
extern tBTA_DM_PM_CFG *p_bta_dm_pm_cfg;

extern tBTA_DM_RM *p_bta_dm_rm_cfg;
extern tBTA_DM_PM_SPEC *p_bta_dm_pm_spec;
extern tBTM_PM_PWR_MD *p_bta_dm_pm_md;
extern tBTA_DM_COMPRESS *p_bta_dm_compress_cfg;

/*******************************************************************************
 * functions
 */

/* bta_dm_act.c */

void bta_dm_enable(tBTA_DM_MSG *p_data);
void bta_dm_disable(tBTA_DM_MSG *p_data);

void bta_dm_set_dev_name(tBTA_DM_MSG *p_data);
void bta_dm_set_visibility(tBTA_DM_MSG *p_data);
void bta_dm_bond(tBTA_DM_MSG *p_data);
void bta_dm_pin_reply(tBTA_DM_MSG *p_data);
void bta_dm_auth_reply(tBTA_DM_MSG *p_data);
void bta_dm_search_start(tBTA_DM_MSG *p_data);
void bta_dm_search_cancel(tBTA_DM_MSG *p_data);
void bta_dm_discover(tBTA_DM_MSG *p_data);
void bta_dm_inq_cmpl(tBTA_DM_MSG *p_data);
void bta_dm_rmt_name(tBTA_DM_MSG *p_data);
void bta_dm_disc_rmt_name(tBTA_DM_MSG *p_data);
void bta_dm_sdp_result(tBTA_DM_MSG *p_data);
void bta_dm_search_cmpl(tBTA_DM_MSG *p_data);
void bta_dm_disc_result(tBTA_DM_MSG *p_data);
void bta_dm_search_result(tBTA_DM_MSG *p_data);

void bta_dm_free_sdp_db(tBTA_DM_MSG *p_data);
void bta_dm_queue_search(tBTA_DM_MSG *p_data);
void bta_dm_queue_disc(tBTA_DM_MSG *p_data);
void bta_dm_search_clear_queue(tBTA_DM_MSG *p_data);
void bta_dm_search_cancel_cmpl(tBTA_DM_MSG *p_data);
void bta_dm_search_cancel_transac_cmpl(tBTA_DM_MSG *p_data);
void bta_dm_search_cancel_notify(tBTA_DM_MSG *p_data);

void bta_dm_cancel_rmt_name(tBTA_DM_MSG *p_data);

void bta_dm_signal_strength(tBTA_DM_MSG *p_data);

void bta_dm_acl_change(tBTA_DM_MSG *p_data);

BOOLEAN bta_sys_check_compress(unk_t, UINT8 app_id, BD_ADDR peer_addr);

void bta_dm_keep_acl(tBTA_DM_MSG *p_data);
void bta_dm_immediate_disable(void);
void bta_dm_reset_complete(void *p1);
void bta_dm_send_hci_reset(tBTA_DM_MSG *p_data);

// ---

/* bta_dm_main.c */
BOOLEAN bta_dm_sm_execute(BT_HDR *p_msg);
BOOLEAN bta_dm_search_sm_execute(BT_HDR *p_msg);

/* bta_dm_pm.c */
void bta_dm_init_pm(void);
void bta_dm_disable_pm(void);

void bta_dm_pm_btm_status(tBTA_DM_MSG *p_data);
void bta_dm_pm_timer(tBTA_DM_MSG *p_data);

#ifdef __cplusplus
	}
#endif

#endif // BTE_BTA_DM_INT_H
