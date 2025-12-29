#ifndef BTE_BTM_INT_H
#define BTE_BTM_INT_H

/* Original source:
 * bluedroid <android.googlesource.com/platform/external/bluetooth/bluedroid>
 * stack/btm/btm_int.h
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

#include "bt_target.h"
#include "bt_trace.h"
#include "bt_types.h"
#include "data_types.h"

#include "btm_api.h"
#include "gki.h"
#include "hcidefs.h"
#include "rfcdefs.h"
#include "sdp_api.h"

/*******************************************************************************
 * macros
 */

#define BTM_MAX_SCN					PORT_MAX_RFC_PORTS

#define BTM_ACL_SUPPORTED_PKTS_MASK                                           \
	(HCI_PKT_TYPES_MASK_DM1 | HCI_PKT_TYPES_MASK_DH1 | HCI_PKT_TYPES_MASK_DM3 \
	 | HCI_PKT_TYPES_MASK_DH3 | HCI_PKT_TYPES_MASK_DM5                        \
	 | HCI_PKT_TYPES_MASK_DH5)
#define BTM_ACL_EXCEPTION_PKTS_MASK                              \
	(HCI_PKT_TYPES_MASK_NO_2_DH1 | HCI_PKT_TYPES_MASK_NO_3_DH1   \
	 | HCI_PKT_TYPES_MASK_NO_2_DH3 | HCI_PKT_TYPES_MASK_NO_3_DH3 \
	 | HCI_PKT_TYPES_MASK_NO_2_DH5 | HCI_PKT_TYPES_MASK_NO_3_DH5)
#define BTM_SCO_SUPPORTED_PKTS_MASK                              \
	(HCI_ESCO_PKT_TYPES_MASK_HV1 | HCI_ESCO_PKT_TYPES_MASK_HV2   \
	 | HCI_ESCO_PKT_TYPES_MASK_HV3 | HCI_ESCO_PKT_TYPES_MASK_EV3 \
	 | HCI_ESCO_PKT_TYPES_MASK_EV4 | HCI_ESCO_PKT_TYPES_MASK_EV5)
#define BTM_SCO_EXCEPTION_PKTS_MASK                                      \
	(HCI_ESCO_PKT_TYPES_MASK_NO_2_EV3 | HCI_ESCO_PKT_TYPES_MASK_NO_3_EV3 \
	 | HCI_ESCO_PKT_TYPES_MASK_NO_2_EV5 | HCI_ESCO_PKT_TYPES_MASK_NO_3_EV5)

#define BTM_MIN_INQUIRY_LENGTH		1
#define BTM_MAX_INQUIRY_LENGTH		HCI_MAX_INQUIRY_LENGTH

#define BTM_PER_INQ_MIN_MIN_PERIOD	HCI_PER_INQ_MIN_MIN_PERIOD
#define BTM_PER_INQ_MAX_MIN_PERIOD	HCI_PER_INQ_MAX_MIN_PERIOD

#define BTM_PER_INQ_MIN_MAX_PERIOD	HCI_PER_INQ_MIN_MAX_PERIOD
#define BTM_PER_INQ_MAX_MAX_PERIOD	HCI_PER_INQ_MAX_MAX_PERIOD

#define BTM_EXT_RMT_NAME_TIMEOUT	40

#define BTM_CLOCK_OFFSET_VALID		0x8000

#define BTM_SEC_INVALID_HANDLE		0xffff

#define BTM_ESCO_PKT_TYPE_MASK                                 \
	(HCI_ESCO_PKT_TYPES_MASK_HV1 | HCI_ESCO_PKT_TYPES_MASK_HV2 \
	 | HCI_ESCO_PKT_TYPES_MASK_HV3)

#define BTM_ESCO_2_SCO(escotype) \
	((UINT16)(((escotype) & BTM_ESCO_PKT_TYPE_MASK) << 5))

#define BTM_PM_REC_NOT_USED				0

#define BTM_SEC_MAX_LINK_KEY_CALLBACKS	2
#define BTM_SEC_MAX_RMT_NAME_CALLBACKS	2

#define BTM_DISC_DB_SIZE				4000

/*******************************************************************************
 * types
 */

#ifdef __cplusplus
	extern "C" {
#endif

typedef UINT8 tBTM_ACL_SWKEY_STATE;
enum
{
	BTM_ACL_SWKEY_STATE_IDLE,
	BTM_ACL_SWKEY_STATE_MODE_CHANGE,
	BTM_ACL_SWKEY_STATE_ENCRYPTION_OFF,
	BTM_ACL_SWKEY_STATE_SWITCHING,
	BTM_ACL_SWKEY_STATE_ENCRYPTION_ON,
	BTM_ACL_SWKEY_STATE_IN_PROGRESS,
};

enum
{
	TT_DEV_RESET	= 1,
	TT_DEV_RLN		= 2,
	TT_DEV_RLNKP	= 4,
};

typedef UINT8 tBTM_DEV_STATE;
enum
{
	BTM_DEV_STATE_WAIT_RESET_CMPLT	= 0,
	BTM_DEV_STATE_WAIT_AFTER_RESET	= 1,
	BTM_DEV_STATE_2					= 2,
	BTM_DEV_STATE_3					= 3,
	BTM_DEV_STATE_4					= 4,
	BTM_DEV_STATE_READY				= 5,
};

typedef UINT8 tBTM_RMT_NAME_ACTIVE_FLAGS;
enum
{
	BTM_RMT_NAME_INACTIVE	= 0,
	BTM_RMT_NAME_EXT		= 1 << 0,
	BTM_RMT_NAME_SEC		= 1 << 1,
	BTM_RMT_NAME_INQ		= 1 << 2,
};

typedef UINT8 tBTM_INQ_STATE;
enum
{
	BTM_INQ_INACTIVE_STATE	= 0,
	BTM_INQ_CLR_FILT_STATE	= 1,
	BTM_INQ_SET_FILT_STATE	= 2,
	BTM_INQ_ACTIVE_STATE	= 3,
	BTM_INQ_REMNAME_STATE	= 4,
};

enum
{
	BTM_SEC_AUTHORIZED			= BTM_SEC_FLAG_AUTHORIZED,
	BTM_SEC_AUTHENTICATED		= BTM_SEC_FLAG_AUTHENTICATED,
	BTM_SEC_ENCRYPTED			= BTM_SEC_FLAG_ENCRYPTED,
	BTM_SEC_NAME_KNOWN			= 0x08,
	BTM_SEC_LINK_KEY_KNOWN		= BTM_SEC_FLAG_LKEY_KNOWN,
	BTM_SEC_LINK_KEY_AUTHED		= 0x20,
	BTM_SEC_ROLE_SWITCHED		= 0x40,
	BTM_SEC_IN_USE				= 0x80,
};

enum
{
	BTM_SEC_STATE_IDLE				= 0,
	BTM_SEC_STATE_AUTHENTICATING	= 1,
	BTM_SEC_STATE_ENCRYPTING		= 2,
	BTM_SEC_STATE_GETTING_NAME		= 3,
	BTM_SEC_STATE_AUTHORIZING		= 4,
};

enum
{
    BTM_PM_ST_ACTIVE  = BTM_PM_STS_ACTIVE,
    BTM_PM_ST_PENDING = BTM_PM_STS_PENDING,
};

typedef char tBTM_LOC_BD_NAME[BTM_MAX_LOC_BD_NAME_LEN + 1];

typedef tBTM_SEC_CBACK tBTM_SEC_CALLBACK;

typedef void tBTM_SCO_IND_CBACK(UINT16);

typedef struct
{
	tHCI_HANDLE				hci_handle;			// size 0x002, offset 0x000
	UINT16					pkt_types_mask;		// size 0x002, offset 0x002
	UINT16					restore_pkt_types;	// size 0x002, offset 0x004
	UINT16					clock_offset;		// size 0x002, offset 0x006
	BD_ADDR					remote_addr;		// size 0x006, offset 0x008
	DEV_CLASS				remote_dc;			// size 0x003, offset 0x00e
	BD_NAME					remote_name;		// size 0x0f8, offset 0x011
	/* 1 byte padding */
	UINT16					manufacturer;		// size 0x002, offset 0x10a
	UINT16					lmp_subversion;		// size 0x002, offset 0x10c
	UINT16					link_super_tout;	// size 0x002, offset 0x10e
	BD_FEATURES				features;			// size 0x008, offset 0x110
	UINT8					lmp_version;		// size 0x001, offset 0x118
	BOOLEAN					in_use;				// size 0x001, offset 0x119
	tBTM_ROLE				link_role;			// size 0x001, offset 0x11a
	tBTM_ACL_SWKEY_STATE	switch_role_state;	// size 0x001, offset 0x11b
} tACL_CONN; // size 0x11c

typedef struct
{
	tBTM_DEV_STATUS_CB		*p_dev_status_cb;			// size 0x04, offset 0x00
	tBTM_VS_EVT_CB			*p_vend_spec_cb;			// size 0x04, offset 0x04
	tBTM_CMPL_CB			*p_stored_link_key_cmpl_cb;	// size 0x04, offset 0x08
	TIMER_LIST_ENT			reset_timer;				// size 0x18, offset 0x0c
	tBTM_CMPL_CB			*p_reset_cmpl_cb;			// size 0x04, offset 0x24
	TIMER_LIST_ENT			rln_timer;					// size 0x18, offset 0x28
	tBTM_CMPL_CB			*p_rln_cmpl_cb;				// size 0x04, offset 0x40
	TIMER_LIST_ENT			rlinkp_timer;				// size 0x18, offset 0x44
	tBTM_CMPL_CB			*p_rlinkp_cmpl_cb;			// size 0x04, offset 0x5c
	TIMER_LIST_ENT			rssi_timer;					// size 0x18, offset 0x60
	tBTM_CMPL_CB			*p_rssi_cmpl_cb;			// size 0x04, offset 0x78
	TIMER_LIST_ENT			lnk_quality_timer;			// size 0x18, offset 0x7c
	tBTM_CMPL_CB			*p_lnk_qual_cmpl_cb;		// size 0x04, offset 0x94
	TIMER_LIST_ENT			qossu_timer;				// size 0x18, offset 0x98
	tBTM_CMPL_CB			*p_qossu_cmpl_cb;			// size 0x04, offset 0xb0
	tBTM_CMPL_CB			*p_vsc_cmpl_cb;				// size 0x04, offset 0xb4
	tBTM_CMPL_CB			*p_reset_only_cmpl_cb;		// size 0x04, offset 0xb8
	tBTM_ROLE_SWITCH_CMPL	switch_role_ref_data;		// size 0x08, offset 0xbc
	tBTM_CMPL_CB			*p_switch_role_cb;			// size 0x04, offset 0xc4
	BD_ADDR					local_addr;					// size 0x06, offset 0xc8
	tBTM_VERSION_INFO		local_version;				// size 0x0a, offset 0xce
	BD_FEATURES				local_features;				// size 0x08, offset 0xd8
	DEV_CLASS				dev_class;					// size 0x03, offset 0xe0
	/* 1 byte padding */
	UINT16					page_timeout;				// size 0x02, offset 0xe4
	tBTM_DEV_STATE			state;						// size 0x01, offset 0xe6
	UINT8					retry_count;				// size 0x01, offset 0xe7
	UINT8					vsc_busy;					// size 0x01, offset 0xe8
	/* 3 bytes padding */
} tBTM_DEVCB; // size 0xec

typedef struct
{
	UINT32	inq_count;	// size 0x04, offset 0x00
	BD_ADDR	bd_addr;	// size 0x06, offset 0x04
	/* 2 bytes padding */
} tINQ_BDADDR; // size 0x0c

typedef struct
{
	UINT32			time_of_resp;	// size 0x04, offset 0x00
	UINT32			inq_count;		// size 0x04, offset 0x04
	tBTM_INQ_INFO	inq_info;		// size 0x12, offset 0x08
	BOOLEAN			in_use;			// size 0x01, offset 0x1a
	/* 1 byte padding */
} tINQ_DB_ENT; // size 0x1c

typedef struct
{
	tBTM_CMPL_CB				*p_remname_cmpl_cb;				// size 0x004, offset 0x000
	TIMER_LIST_ENT				rmt_name_timer_ent;				// size 0x018, offset 0x004
	tBTM_DISC_MODE				discoverable_mode;				// size 0x002, offset 0x01c
	tBTM_CONN_MODE				connectable_mode;				// size 0x002, offset 0x01e
	UINT16						page_scan_window;				// size 0x002, offset 0x020
	UINT16						page_scan_period;				// size 0x002, offset 0x022
	UINT16						inq_scan_window;				// size 0x002, offset 0x024
	UINT16						inq_scan_period;				// size 0x002, offset 0x026
	tBTM_INQ_SCAN_TYPE			inq_scan_type;					// size 0x002, offset 0x028
	tBTM_INQ_SCAN_TYPE			page_scan_type;					// size 0x002, offset 0x02a
	BD_ADDR						remname_bda;					// size 0x006, offset 0x02c
	tBTM_RMT_NAME_ACTIVE_FLAGS	remname_active;					// size 0x001, offset 0x032
	/* 1 byte padding */
	tBTM_CMPL_CB				*p_inq_cmpl_cb;					// size 0x004, offset 0x034
	tBTM_INQ_RESULTS_CB			*p_inq_results_cb;				// size 0x004, offset 0x038
	tBTM_CMPL_CB				*p_inqfilter_cmpl_cb;			// size 0x004, offset 0x03c
	tBTM_INQ_DB_CHANGE_CB		*p_inq_change_cb;				// size 0x004, offset 0x040
	UINT32						inq_counter;					// size 0x004, offset 0x044
	TIMER_LIST_ENT				inq_timer_ent;					// size 0x018, offset 0x048
	tINQ_BDADDR					*p_bd_db;						// size 0x004, offset 0x060
	UINT16						num_bd_entries;					// size 0x002, offset 0x064
	UINT16						max_bd_entries;					// size 0x002, offset 0x066
	tINQ_DB_ENT					inq_db[BTM_INQ_DB_SIZE];		// size 0x150, offset 0x068
	tBTM_INQ_PARMS				inqparms;						// size 0x00a, offset 0x1b8
	tBTM_INQUIRY_CMPL			inq_cmpl_info;					// size 0x002, offset 0x1c2
	UINT16						per_min_delay;					// size 0x002, offset 0x1c4
	UINT16						per_max_delay;					// size 0x002, offset 0x1c6
	BOOLEAN						inqfilt_active;					// size 0x001, offset 0x1c8
	UINT8						pending_filt_complete_event;	// size 0x001, offset 0x1c9
	tBTM_INQ_FILT_COND_TYPE		inqfilt_type;					// size 0x001, offset 0x1ca
	tBTM_INQ_STATE				state;							// size 0x001, offset 0x1cb
	tBTM_INQ_ACTIVE_STATUS		inq_active;						// size 0x001, offset 0x1cc
	/* 3 bytes padding */
} tBTM_INQUIRY_VAR_ST; // size 0x1d0

typedef struct
{
	tBTM_ESCO_CBACK		*p_esco_cback;	// size 0x04, offset 0x00
	tBTM_ESCO_PARAMS	setup;			// size 0x10, offset 0x04
	tBTM_ESCO_DATA		data;			// size 0x0e, offset 0x14
	tHCI_STATUS			hci_status;		// size 0x01, offset 0x22
	/* 1 byte padding */
} tBTM_ESCO_INFO; // size 0x24

typedef struct
{
	tBTM_SCO_CB		*p_conn_cb;		// size 0x04, offset 0x00
	tBTM_SCO_CB		*p_disc_cb;		// size 0x04, offset 0x04
	UINT16			state;			// size 0x02, offset 0x08
	tHCI_HANDLE		hci_handle;		// size 0x02, offset 0x0a
	BOOLEAN			is_orig;		// size 0x01, offset 0x0c
	BOOLEAN			rem_bd_known;	// size 0x01, offset 0x0d
	/* 2 bytes padding */
	tBTM_ESCO_INFO	esco;			// size 0x24, offset 0x10
} tSCO_CONN; // size 0x34

typedef struct
{
	tBTM_SCO_IND_CBACK	*app_sco_ind_cb;			// size 0x04, offset 0x00
	tSCO_CONN			sco_db[BTM_MAX_SCO_LINKS];	// size 0x9c, offset 0x04
	BD_ADDR				xfer_addr;					// size 0x06, offset 0xa0
	UINT16				sco_disc_reason;			// size 0x02, offset 0xa6
	tBTM_ESCO_PARAMS	def_esco_parms;				// size 0x10, offset 0xa8
	BOOLEAN				esco_supported;				// size 0x01, offset 0xb8
	tBTM_SCO_TYPE		desired_sco_mode;			// size 0x01, offset 0xb9
	tBTM_SCO_TYPE		xfer_sco_type;				// size 0x01, offset 0xba
	/* 1 byte padding */
} tSCO_CB; // size 0xbc

typedef struct
{
	UINT32	mx_proto_id;										// size 0x04, offset 0x00
	UINT32	orig_mx_chan_id;									// size 0x04, offset 0x04
	UINT32	term_mx_chan_id;									// size 0x04, offset 0x08
	UINT16	psm;												// size 0x02, offset 0x0c
	UINT8	security_flags;										// size 0x01, offset 0x0e
	UINT8	service_id;											// size 0x01, offset 0x0f
	UINT8	orig_service_name[BTM_SEC_SERVICE_NAME_LEN + 1];	// size 0x16, offset 0x10
	UINT8	term_service_name[BTM_SEC_SERVICE_NAME_LEN + 1];	// size 0x16, offset 0x26
} tBTM_SEC_SERV_REC; // size 0x3c

typedef struct
{
	tBTM_SEC_SERV_REC	*p_cur_service;								// size 0x04, offset 0x00
	tBTM_SEC_CALLBACK	*p_callback;								// size 0x04, offset 0x04
	void				*p_ref_data;								// size 0x04, offset 0x08
	UINT32				timestamp;									// size 0x04, offset 0x0c
	UINT32				trusted_mask[BTM_SEC_SERVICE_ARRAY_SIZE];	// size 0x08, offset 0x10
	UINT16				hci_handle;									// size 0x02, offset 0x18
	UINT16				clock_offset;								// size 0x02, offset 0x1a
	BD_ADDR				bd_addr;									// size 0x06, offset 0x1c
	DEV_CLASS			dev_class;									// size 0x03, offset 0x22
	LINK_KEY			link_key;									// size 0x10, offset 0x25
	tBTM_BD_NAME		sec_bd_name;								// size 0x41, offset 0x35
	UINT8				sec_flags;									// size 0x01, offset 0x76
	BD_FEATURES			features;									// size 0x08, offset 0x77
	UINT8				sec_state;									// size 0x01, offset 0x7f
	BOOLEAN				is_originator;								// size 0x01, offset 0x80
	UINT8				security_required;							// size 0x01, offset 0x81
	BOOLEAN				link_key_not_sent;							// size 0x01, offset 0x82
	UINT8				link_key_type;								// size 0x01, offset 0x83
	BOOLEAN				link_key_changed;							// size 0x01, offset 0x84
	BOOLEAN				sec_acl_connected;							// size 0x01, offset 0x85
	/* 2 bytes padding */
} tBTM_SEC_DEV_REC; // size 0x88

typedef struct
{
	tBTM_LOC_BD_NAME	bd_name;			// size 0x20, offset 0x00
	BOOLEAN				pin_type;			// size 0x01, offset 0x20
	UINT8				pin_code_len;		// size 0x01, offset 0x21
	PIN_CODE			pin_code;			// size 0x10, offset 0x22
	BOOLEAN				connectable;		// size 0x01, offset 0x32
	UINT8				def_inq_scan_mode;	// size 0x01, offset 0x33
} tBTM_CFG; // size 0x34

typedef struct
{
	tBTM_PM_PWR_MD	req_mode[BTM_MAX_PM_RECORDS];	// size 0x14, offset 0x00
	tBTM_PM_PWR_MD	set_mode;						// size 0x0a, offset 0x14
	UINT16			interval;						// size 0x02, offset 0x1e
	UINT8			state;							// size 0x01, offset 0x20
	UINT8			chg_ind;						// size 0x01, offset 0x21
} tBTM_PM_MCB; // size 0x22

typedef struct
{
	tBTM_PM_STATUS_CBACK	*cback;	// size 0x04, offset 0x00
	UINT8					mask;	// size 0x01, offset 0x04
} tBTM_PM_RCB; // size 0x08

// TODO: where should these go?
typedef tSDP_DISCOVERY_DB tBTM_DISCOVERY_DB;

typedef struct
{
	TIMER_LIST_ENT		disc_timer_ent;						// size 0x0018, offset 0x0000
	UINT16				num_uuid_filters;					// size 0x0002, offset 0x0018
	/* 2 bytes padding */
	tBT_UUID			uuid_filters[SDP_MAX_UUID_FILTERS];	// size 0x003c, offset 0x001c
	UINT16				num_attr_filters;					// size 0x0002, offset 0x0058
	UINT16				attr_filters[SDP_MAX_ATTR_FILTERS];	// size 0x0018, offset 0x005a
	/* 2 bytes padding */
	tBTM_INQ_INFO		*p_cur_ii;							// size 0x0004, offset 0x0074
	tBTM_CMPL_CB		*p_disc_cmpl_cb;					// size 0x0004, offset 0x0078 // TODO: is this actually something else? (probably not?)
	UINT8				btm_db_area[BTM_DISC_DB_SIZE];		// size 0x0fa0, offset 0x007c
	UINT8				disc_active;						// size 0x0001, offset 0x101c // tBTM_DISC_MODE
	/* 3 bytes padding */
} tBTM_DISCOVERY_VAR_ST; // size 0x1020

typedef struct
{
	tBTM_CFG				cfg;													// size 0x0034, offset 0x0000
	tACL_CONN				acl_db[MAX_L2CAP_LINKS];								// size 0x0470, offset 0x0034
	UINT8					btm_scn[BTM_MAX_SCN];									// size 0x001f, offset 0x04a4
	/* 1 byte padding */
	UINT16					btm_def_link_policy;									// size 0x0002, offset 0x04c4
	UINT16					btm_def_link_super_tout;								// size 0x0002, offset 0x04c6
	tBTM_ACL_DB_CHANGE_CB	*p_acl_changed_cb;										// size 0x0004, offset 0x04c8
	tBTM_PM_MCB				pm_mode_db[MAX_L2CAP_LINKS];							// size 0x0088, offset 0x04cc
	tBTM_PM_RCB				pm_reg_db[BTM_MAX_PM_RECORDS];							// size 0x0010, offset 0x0554
	UINT8					pm_pend_link;											// size 0x0001, offset 0x0564
	UINT8					pm_pend_id;												// size 0x0001, offset 0x0565
	/* 2 bytes padding */
	tBTM_DEVCB				devcb;													// size 0x00ec, offset 0x0568
	UINT16					btm_acl_pkt_types_supported;							// size 0x0002, offset 0x0654
	UINT16					btm_sco_pkt_types_supported;							// size 0x0002, offset 0x0656
	tBTM_DISCOVERY_VAR_ST	btm_disc_vars;											// size 0x1020, offset 0x0658
	tBTM_DISCOVERY_DB		*p_btm_ddb;												// size 0x0004, offset 0x1678
	tBTM_INQUIRY_VAR_ST		btm_inq_vars;											// size 0x01d0, offset 0x167c
	tBTM_FILTER_CB			*p_inq_filter_cb;										// size 0x0004, offset 0x184c
	tSCO_CB					sco_cb;													// size 0x00bc, offset 0x1850
	tBTM_APPL_INFO			api;													// size 0x0018, offset 0x190c
	tBTM_LINK_KEY_CALLBACK	*p_link_key_callback[BTM_SEC_MAX_LINK_KEY_CALLBACKS];	// size 0x0008, offset 0x1924
	tBTM_RMT_NAME_CALLBACK	*p_rmt_name_callback[BTM_SEC_MAX_RMT_NAME_CALLBACKS];	// size 0x0008, offset 0x192c
	tBTM_FILTER_CB			*p_conn_filter_cb;										// size 0x0004, offset 0x1934
	tBTM_SEC_DEV_REC		*p_collided_dev_rec;									// size 0x0004, offset 0x1938
	TIMER_LIST_ENT			sec_pin_code_req_tle;									// size 0x0018, offset 0x193c
	BD_ADDR					sec_pin_code_req_bd_addr;								// size 0x0006, offset 0x1954
	/* 2 bytes padding */
	TIMER_LIST_ENT			sec_collision_tle;										// size 0x0018, offset 0x195c
	UINT32					dev_rec_count;											// size 0x0004, offset 0x1974
	UINT8					security_mode;											// size 0x0001, offset 0x1978
	BOOLEAN					pairing_disabled;										// size 0x0001, offset 0x1979
	BOOLEAN					connect_only_paired;									// size 0x0001, offset 0x197a
	BOOLEAN					security_mode_changed;									// size 0x0001, offset 0x197b
	BOOLEAN					pin_type_changed;										// size 0x0001, offset 0x197c
	BOOLEAN					bond_created_connection;								// size 0x0001, offset 0x197d
	UINT8					disconnect_after_bond_reason;							// size 0x0001, offset 0x197e
	UINT8					pin_code_len;											// size 0x0001, offset 0x197f
	PIN_CODE				pin_code;												// size 0x0010, offset 0x1980
	tBTM_SEC_SERV_REC		sec_serv_rec[BTM_SEC_MAX_SERVICE_RECORDS];				// size 0x0600, offset 0x1990
	tBTM_SEC_DEV_REC		sec_dev_rec[BTM_SEC_MAX_DEVICE_RECORDS];				// size 0x0880, offset 0x1f90
	tBTM_MKEY_CALLBACK		*mkey_cback;											// size 0x0004, offset 0x27b0
	BD_ADDR					connecting_bda;											// size 0x0006, offset 0x27b4
	DEV_CLASS				connecting_dc;											// size 0x0003, offset 0x27ba
	tBTM_AFH_CHANNEL		first_disabled_channel;									// size 0x0001, offset 0x27bd
	tBTM_AFH_CHANNEL		last_disabled_channel;									// size 0x0001, offset 0x27be
	UINT8					acl_disc_reason;										// size 0x0001, offset 0x27bf
	tBT_TRACE_LEVEL			trace_level;											// size 0x0001, offset 0x27c0
	/* 3 bytes padding */
} tBTM_CB; // size 0x27c4

/*******************************************************************************
 * external globals
 */

extern tBTM_CB btm_cb;

/*******************************************************************************
 * functions
 */

void btm_sec_mx_access_request(BD_ADDR bd_addr, UINT16 psm,
                               BOOLEAN is_originator, UINT32 mx_proto_id,
                               UINT32 mx_chan_id, tBTM_SEC_CALLBACK *p_callback,
                               void *p_ref_data);
UINT16 btm_get_acl_disc_reason_code(void);
BOOLEAN btm_acl_notif_conn_collision(BD_ADDR bda);
unsigned char btm_sec_l2cap_access_req(
	BD_ADDR bd_addr, UINT16 psm, UINT16 handle, BOOLEAN is_originator,
	void (*p_callback)(unsigned char *, void *, unsigned char));
void btm_sec_abort_access_req(BD_ADDR bd_addr);
BOOLEAN btm_dev_support_switch(BD_ADDR bd_addr);
unsigned char btm_sec_is_bonding(unsigned short handle);
tBTM_SEC_DEV_REC *btm_find_dev(BD_ADDR bd_addr);
void btm_acl_created(BD_ADDR bda, DEV_CLASS dc, BD_NAME bdn, UINT16 hci_handle,
                     UINT8 link_role);
void btm_sco_acl_removed(BD_ADDR bda);
void btm_remove_sco_links(BD_ADDR bda);
void btm_acl_removed(BD_ADDR bda);
tBTM_STATUS btm_sec_disconnect(UINT16 handle, UINT8 reason);
BOOLEAN btm_is_sco_active_by_bdaddr(BD_ADDR remote_bda);
UINT16 btm_get_max_packet_size(BD_ADDR addr);
tBTM_STATUS btm_remove_acl(BD_ADDR bd_addr);
UINT32 *btm_read_trusted_mask(BD_ADDR bd_addr);
void btm_vendor_specific_evt(UINT8 *p, UINT8 evt_len);
void btm_process_inq_complete(UINT8 status);
void btm_process_inq_results(UINT8 *p, UINT8 inq_res_mode);
void btm_sec_connected(UINT8 *bda, UINT16 handle, UINT8 status, UINT8 enc_mode);
void btm_sco_connected(UINT8 hci_status, BD_ADDR bda, UINT16 hci_handle,
                       tBTM_ESCO_DATA *p_esco_data);
void btm_sec_conn_req(UINT8 *bda, UINT8 *dc);
void btm_sco_conn_req(BD_ADDR bda, DEV_CLASS dev_class, UINT8 link_type);
void btm_sco_removed(UINT16 hci_handle, UINT8 reason);
void btm_sec_disconnected(UINT16 handle, UINT8 reason);
void btm_sec_auth_complete(UINT16 handle, UINT8 status);
void btm_process_remote_name(BD_ADDR bda, BD_NAME name, UINT16 evt_len,
                             UINT8 hci_status);
void btm_sec_rmt_name_request_complete(UINT8 *bd_addr, UINT8 *bd_name,
                                       UINT8 status);
void btm_acl_encrypt_change(UINT16 handle, UINT8 status, UINT8 encr_enable);
void btm_sec_encrypt_change(UINT16 handle, UINT8 status, UINT8 encr_enable);
void btm_sec_mkey_comp_event(UINT16 handle, UINT8 status, UINT8 key_flg);
void btm_read_remote_features_complete(UINT8 *p);
void btm_read_remote_version_complete(UINT8 *p);
void btm_qos_setup_complete(UINT8 status, UINT16 handle, FLOW_SPEC *p_flow);
void btm_esco_proc_conn_chg(UINT8 status, UINT16 handle, UINT8 tx_interval,
                            UINT8 retrans_window, UINT16 rx_pkt_len,
                            UINT16 tx_pkt_len);
void btm_reset_complete(void);
void btm_event_filter_complete(UINT8 *p);
void btm_read_stored_link_key_complete(UINT8 *p);
void btm_write_stored_link_key_complete(UINT8 *p);
void btm_delete_stored_link_key_complete(UINT8 *p);
void btm_read_local_version_complete(UINT8 *p, UINT16 evt_len);
void btm_read_link_policy_complete(UINT8 *p);
void btm_read_hci_buf_size_complete(UINT8 *p, UINT16 evt_len);
void btm_read_local_features_complete(UINT8 *p, UINT16 evt_len);
void btm_read_local_name_complete(UINT8 *p, UINT16 evt_len);
void btm_read_local_addr_complete(UINT8 *p, UINT16 evt_len);
void btm_read_link_quality_complete(UINT8 *p);
void btm_read_rssi_complete(UINT8 *p);
void btm_vsc_complete(UINT8 *p, UINT16 cc_opcode, UINT16 evt_len);
void btm_pm_proc_cmd_status(UINT8 status);
void btm_acl_role_changed(UINT8 hci_status, BD_ADDR bd_addr, UINT8 new_role);
BOOLEAN btm_is_sco_active(UINT16 handle);
void btm_report_device_status(tBTM_DEV_STATUS status);
void btm_sco_chk_pend_unpark(UINT8 hci_status, UINT16 hci_handle, UINT8 mode);
void btm_pm_proc_mode_change(UINT8 hci_status, UINT16 hci_handle, UINT8 mode,
                             UINT16 interval);
void btm_return_link_keys_evt(tBTM_RETURN_LINK_KEYS_EVT *result);
void btm_sec_pin_code_request(UINT8 *p_bda);
void btm_sec_link_key_request(UINT8 *p_bda);
void btm_sec_link_key_notification(UINT8 *p_bda, UINT8 *p_link_key,
                                   UINT8 key_type);
void btm_process_clk_off_comp_evt(UINT16 hci_handle, UINT16 clock_offset);
void btm_sec_update_clock_offset(UINT16 handle, UINT16 clock_offset);
void btm_init(void);
void btm_discovery_db_init(void);
void btm_route_sco_data(BT_HDR *p_msg);
void btm_dev_timeout(TIMER_LIST_ENT *p_tle);
void btm_acl_timeout(TIMER_LIST_ENT *p_tle);
void btm_inq_rmt_name_failed(void);
void btm_discovery_timeout(void);
tBTM_SEC_DEV_REC *btm_find_dev_by_handle(UINT16 handle);
void btm_pm_sm_alloc(UINT8 ind);
tBTM_STATUS btm_set_packet_types(tACL_CONN *p, UINT16 pkt_types);
tBTM_SEC_DEV_REC *btm_sec_alloc_dev(BD_ADDR bd_addr);
void btm_sec_free_dev(tBTM_SEC_DEV_REC *p_dev_rec);
void btm_inq_db_reset(void);
void btm_discovery_db_reset(void);
void btm_pm_reset(void);
void btm_sec_dev_reset(void);
tBTM_STATUS btm_initiate_rem_name(BD_ADDR remote_bda, tBTM_INQ_INFO *p_cur,
                                  UINT8 origin, UINT32 timeout,
                                  tBTM_CMPL_CB *p_cb);
void btm_clr_inq_db(BD_ADDR p_bda);
void btm_sec_init(UINT8 sec_mode);
void btm_sco_init(void);
UINT8 btm_num_sco_links_active(void);
void btm_sco_flush_sco_data(UINT16 sco_inx);

// ---

void btm_acl_init(void);
tACL_CONN *btm_bda_to_acl(BD_ADDR bda);
UINT8 btm_handle_to_acl_index(UINT16 hci_handle);
void btm_acl_created(BD_ADDR bda, DEV_CLASS dc, BD_NAME bdn, UINT16 hci_handle,
                     UINT8 link_role);
void btm_acl_removed(BD_ADDR bda);
void btm_acl_device_down(void);
/**/
void btm_acl_encrypt_change(UINT16 handle, UINT8 status, BOOLEAN encr_enable);
/**/
void btm_read_link_policy_complete(UINT8 *p);
void btm_read_remote_version_complete(UINT8 *p);
void btm_read_remote_features_complete(UINT8 *p);
/**/
UINT16 btm_get_acl_disc_reason_code(void);
/**/
void btm_process_clk_off_comp_evt(UINT16 hci_handle, UINT16 clock_offset);
void btm_acl_role_changed(UINT8 hci_status, BD_ADDR bd_addr, UINT8 new_role);
/**/
void btm_acl_timeout(TIMER_LIST_ENT *p_tle);
tBTM_STATUS btm_set_packet_types(tACL_CONN *p, UINT16 pkt_types);
UINT16 btm_get_max_packet_size(BD_ADDR addr);
/**/
void btm_qos_setup_complete(UINT8 status, UINT16 handle, FLOW_SPEC *p_flow);
/**/
void btm_read_rssi_complete(UINT8 *p);
void btm_read_link_quality_complete(UINT8 *p);
tBTM_STATUS btm_remove_acl(BD_ADDR bd_addr);
/**/
void btm_chg_all_acl_pkt_types(BOOLEAN entering_sco);

tBTM_SEC_DEV_REC *btm_sec_alloc_dev(BD_ADDR bd_addr);
void btm_sec_free_dev(tBTM_SEC_DEV_REC *p_dev_rec);
tBTM_SEC_DEV_REC *btm_find_dev_by_handle(UINT16 handle);
tBTM_SEC_DEV_REC *btm_find_dev(BD_ADDR bd_addr);

void btm_dev_init(void);
/**/
void btm_dev_absent(void);
/**/
void btm_get_hci_buf_size(void);
void btm_get_local_version(void);
void btm_get_local_features(void);
void btm_dev_timeout(TIMER_LIST_ENT *p_tle);
void btm_reset_complete(void);
/**/
void btm_read_hci_buf_size_complete(UINT8 *p, UINT16 evt_len);
void btm_read_local_version_complete(UINT8 *p, UINT16 evt_len);
void btm_read_local_features_complete(UINT8 *p, UINT16 evt_len);
UINT8 btm_get_voice_coding_support(void);
/**/
void btm_read_local_name_complete(UINT8 *p, UINT16 evt_len);
/**/
void btm_read_local_addr_complete(UINT8 *p, UINT16 evt_len);
/**/
void btm_vsc_complete(UINT8 *p, UINT16 opcode, UINT16 evt_len);
/**/
void btm_vendor_specific_evt(UINT8 *p, UINT8 evt_len);
/**/
UINT8 btm_get_hci_version(void);
/**/
void btm_read_stored_link_key_complete(UINT8 *p);
void btm_write_stored_link_key_complete(UINT8 *p);
void btm_delete_stored_link_key_complete(UINT8 *p);
void btm_return_link_keys_evt(tBTM_RETURN_LINK_KEYS_EVT *result);
void btm_report_device_status(tBTM_DEV_STATUS status);

void btm_discovery_db_init(void);
void btm_discovery_db_reset(void);
void btm_discovery_timeout(void);

void btm_inq_db_reset(void);
void btm_inq_db_init(void);
void btm_clr_inq_db(BD_ADDR p_bda);
/**/
void btm_event_filter_complete(UINT8 *p);
/**/
void btm_process_inq_results(UINT8 *p, BOOLEAN with_rssi);
void btm_process_inq_complete(UINT8 status);
void btm_inq_check_ageing(void);
tBTM_STATUS btm_initiate_rem_name(BD_ADDR remote_bda, tBTM_INQ_INFO *p_cur,
                                  UINT8 origin, UINT32 timeout,
                                  tBTM_CMPL_CB *p_cb);
void btm_process_remote_name(BD_ADDR bda, BD_NAME bdn, UINT16 evt_len,
                             UINT8 hci_status);
void btm_inq_rmt_name_failed(void);

void btm_pm_reset(void);
void btm_pm_sm_alloc(UINT8 ind);
/**/
void btm_pm_proc_cmd_status(UINT8 status);
void btm_pm_proc_mode_change(UINT8 hci_status,
                             UINT16 hci_handle, UINT8 mode,
                             UINT16 interval);

void btm_sco_init(void);
/**/
void btm_set_sco_ind_cback(tBTM_SCO_IND_CBACK *sco_ind_cb);
void btm_accept_sco_link(UINT16 sco_inx, tBTM_ESCO_PARAMS *p_setup,
                         tBTM_SCO_CB *p_conn_cb, tBTM_SCO_CB *p_disc_cb);
void btm_reject_sco_link(UINT16 sco_inx);
/**/
void btm_sco_chk_pend_unpark(UINT8 hci_status, UINT16 hci_handle, UINT8 mode);
void btm_sco_conn_req(BD_ADDR bda, DEV_CLASS dev_class, UINT8 link_type);
void btm_sco_connected(UINT8 hci_status, BD_ADDR bda, UINT16 hci_handle,
                       tBTM_ESCO_DATA *p_esco_data);
/**/
void btm_remove_sco_links(BD_ADDR bda);
void btm_sco_removed(UINT16 hci_handle, UINT8 reason);
void btm_sco_acl_removed(BD_ADDR bda);
/**/
void btm_route_sco_data(BT_HDR *p_msg);
/**/
tBTM_SCO_TYPE btm_read_def_esco_mode(tBTM_ESCO_PARAMS *p_parms);
void btm_esco_proc_conn_chg(UINT8 status, UINT16 handle, UINT8 tx_interval,
                            UINT8 retrans_window, UINT16 rx_pkt_len,
                            UINT16 tx_pkt_len);
BOOLEAN btm_is_sco_active(UINT16 handle);
UINT8 btm_num_sco_links_active(void);
BOOLEAN btm_is_any_sco_active(void);
BOOLEAN btm_is_sco_active_by_bdaddr(BD_ADDR remote_bda);

tBTM_STATUS btm_sec_l2cap_access_req(BD_ADDR bd_addr, UINT16 psm, UINT16 handle,
                                     BOOLEAN is_originator,
                                     tBTM_SEC_CALLBACK *p_callback);
void btm_sec_mx_access_request(BD_ADDR bd_addr, UINT16 psm,
                               BOOLEAN is_originator, UINT32 mx_proto_id,
                               UINT32 mx_chan_id, tBTM_SEC_CALLBACK *p_callback,
                               void *p_ref_data);
void btm_sec_change_link_key(UINT16 handle);
void btm_sec_conn_req(BD_ADDR bda, DEV_CLASS dc);
void btm_sec_init(UINT8 sec_mode);
void btm_sec_dev_reset(void);
void btm_sec_abort_access_req(BD_ADDR bd_addr);
void btm_sec_rmt_name_request_complete(BD_ADDR_PTR p_bd_addr,
                                       BD_NAME_PTR p_bd_name, UINT8 status);
void btm_sec_auth_complete(UINT16 handle, UINT8 status);
void btm_sec_mkey_comp_event(UINT16 handle, UINT8 status, UINT8 key_flg);
void btm_sec_encrypt_change(UINT16 handle, UINT8 status, UINT8 encr_enable);
BOOLEAN btm_sec_is_bonding(UINT16 handle);
void btm_sec_connected(BD_ADDR bda, UINT16 handle, UINT8 status, UINT8 enc_mode);
tBTM_STATUS btm_sec_disconnect(UINT16 handle, UINT8 reason);
void btm_sec_disconnected(UINT16 handle, UINT8 reason);
void btm_sec_link_key_notification(BD_ADDR_PTR p_bda, UINT8 *p_link_key,
                                   UINT8 key_type);
void btm_sec_link_key_request(BD_ADDR_PTR p_bda);
void btm_sec_pin_code_request_timeout(void);
void btm_sec_pin_code_request(BD_ADDR_PTR p_bda);
void btm_sec_update_clock_offset(UINT16 handle, UINT16 clock_offset);
/**/
UINT32 *btm_read_trusted_mask(BD_ADDR bd_addr);

#ifdef __cplusplus
	}
#endif

#endif // BTE_BTM_INT_H
