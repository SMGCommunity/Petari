#ifndef BTE_SDP_INT_H
#define BTE_SDP_INT_H

/* Original source:
 * bluedroid <android.googlesource.com/platform/external/bluetooth/bluedroid>
 * stack/sdp/sdpint.h
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
#include "bt_types.h"
#include "data_types.h"

#include "gki.h"
#include "l2c_api.h"
#include "sdp_api.h"

/*******************************************************************************
 * macros
 */

#define SDP_CONTINUATION_LEN		2
#define SDP_MAX_CONTINUATION_LEN	16

#define SDP_INACT_TIMEOUT			30

#define MAX_UUIDS_PER_SEQ			16
#define MAX_ATTR_PER_SEQ			16

/*******************************************************************************
 * types
 */

#ifdef __cplusplus
	extern "C" {
#endif

enum
{
	SDP_PDU_ERROR_RESPONSE = 1,
	SDP_PDU_SERVICE_SEARCH_REQ,
	SDP_PDU_SERVICE_SEARCH_RSP,
	SDP_PDU_SERVICE_ATTR_REQ,
	SDP_PDU_SERVICE_ATTR_RSP,
	SDP_PDU_SERVICE_SEARCH_ATTR_REQ,
	SDP_PDU_SERVICE_SEARCH_ATTR_RSP,
};

typedef UINT8 tCONN_CB_STATE;
enum
{
	SDP_STATE_IDLE,
	SDP_STATE_CONN_SETUP,
	SDP_STATE_CFG_SETUP,
	SDP_STATE_CONNECTED,
};

typedef UINT8 tCONN_CB_FLAGS;
enum
{
	SDP_FLAGS_IS_ORIG			= 1 << 0,
	SDP_FLAGS_THEIR_CFG_DONE	= 1 << 1,
	SDP_FLAGS_OUR_CFG_DONE		= 1 << 2,
};

typedef UINT8 tCONN_CB_DISC_STATE;
enum
{
	SDP_DISC_WAIT_CONN,
	SDP_DISC_WAIT_HANDLES,
	SDP_DISC_WAIT_ATTR,
	SDP_DISC_WAIT_SEARCH_ATTR,
	SDP_DISC_WAIT_PASS_THRU,
	SDP_DISC_WAIT_CANCEL,
};

typedef struct
{
	UINT16	len;					// size 0x02, offset 0x00
	UINT8	value[MAX_UUID_SIZE];	// size 0x10, offset 0x02
} tUID_ENT; // size 0x12

typedef struct
{
	UINT16		num_uids;						// size 0x002, offset 0x000
	tUID_ENT	uuid_entry[MAX_UUIDS_PER_SEQ];	// size 0x120, offset 0x002
} tSDP_UUID_SEQ; // size 0x122

typedef struct
{
	UINT16	start;	// size 0x02, offset 0x00
	UINT16	end;	// size 0x02, offset 0x02
} tATT_ENT; // size 0x04

typedef struct
{
	UINT16		num_attr;						// size 0x02, offset 0x00
	tATT_ENT	attr_entry[MAX_ATTR_PER_SEQ];	// size 0x40, offset 0x02
} tSDP_ATTR_SEQ; // size 0x42

typedef struct
{
	UINT32	len;		// size 0x04, offset 0x00
	UINT8	*value_ptr;	// size 0x04, offset 0x04
	UINT16	id;			// size 0x02, offset 0x08
	UINT8	type;		// size 0x01, offset 0x0a
	/* 1 byte padding */
} tSDP_ATTRIBUTE; // size 0x0c

typedef struct
{
	UINT32			record_handle;					// size 0x004, offset 0x000
	UINT32			free_pad_ptr;					// size 0x004, offset 0x004
	UINT16			num_attributes;					// size 0x001, offset 0x008
	/* 2 bytes padding */
	tSDP_ATTRIBUTE	attribute[SDP_MAX_REC_ATTR];	// size 0x12c, offset 0x00c
	UINT8			attr_pad[SDP_MAX_PAD_LEN];		// size 0x15e, offset 0x138
	/* 2 bytes padding */
} tSDP_RECORD; // size 0x298

typedef struct
{
	UINT32		di_primary_handle;			// size 0x0004, offset 0x0000
	BOOLEAN		brcm_di_registered;			// size 0x0001, offset 0x0004
	/* 1 byte padding */
	UINT16		num_records;				// size 0x0002, offset 0x0006
	tSDP_RECORD	record[SDP_MAX_RECORDS];	// size 0x33e0, offset 0x0008
} tSDP_DB; // size 0x33e8

typedef struct
{
	tCONN_CB_STATE		con_state;							// size 0x001, offset 0x000
	tCONN_CB_FLAGS		con_flags;							// size 0x001, offset 0x001
	BD_ADDR				device_address;						// size 0x006, offset 0x002
	TIMER_LIST_ENT		timer_entry;						// size 0x018, offset 0x008
	UINT16				rem_mtu_size;						// size 0x002, offset 0x020
	UINT16				connection_id;						// size 0x002, offset 0x022
	UINT16				list_len;							// size 0x002, offset 0x024
	UINT8				rsp_list[1000];						// size 0x3e8, offset 0x026
	/* 2 bytes padding */
	tSDP_DISCOVERY_DB	*p_db;								// size 0x004, offset 0x410
	tSDP_DISC_CMPL_CB	*p_cb;								// size 0x004, offset 0x414
	UINT32				handles[SDP_MAX_DISC_SERVER_RECS];	// size 0x054, offset 0x418
	UINT16				num_handles;						// size 0x002, offset 0x46c
	UINT16				cur_handle;							// size 0x002, offset 0x46e
	UINT16				transaction_id;						// size 0x002, offset 0x470
	UINT16				disconnect_reason;					// size 0x002, offset 0x472
	tCONN_CB_DISC_STATE	disc_state;							// size 0x001, offset 0x474
	BOOLEAN				is_attr_search;						// size 0x001, offset 0x475
	UINT16				cont_offset;						// size 0x002, offset 0x476
} tCONN_CB; // size 0x1144

typedef struct
{
	tL2CAP_CFG_INFO		l2cap_my_cfg;				// size 0x003c, offset 0x0000
	tCONN_CB			ccb[SDP_MAX_CONNECTIONS];	// size 0x11e0, offset 0x003c
	tSDP_DB				server_db;					// size 0x33e8, offset 0x121c
	tL2CAP_APPL_INFO	reg_info;					// size 0x0028, offset 0x4604
	UINT16				max_attr_list_size;			// size 0x0002, offset 0x462c
	UINT16				max_recs_per_search;		// size 0x0002, offset 0x462e
	UINT8				trace_level;				// size 0x0001, offset 0x4630
	/* 3 bytes padding */
} tSDP_CB; // size 0x4634

/*******************************************************************************
 * external globals
 */

extern tSDP_CB sdp_cb;

/*******************************************************************************
 * functions
 */

tSDP_RECORD *sdp_db_service_search(tSDP_RECORD *p_rec, tSDP_UUID_SEQ *p_seq);
/**/
tSDP_RECORD *sdp_db_find_record(UINT32 handle);
tSDP_ATTRIBUTE *sdp_db_find_attr_in_rec(tSDP_RECORD *p_rec, UINT16 start_attr,
                                        UINT16 end_attr);

void sdp_disc_connected(tCONN_CB *p_ccb);
void sdp_disc_server_rsp(tCONN_CB *p_ccb, BT_HDR *p_msg);

void sdp_init(void);
UINT16 sdp_set_max_attr_list_size(UINT16 max_size);
/**/
tCONN_CB *sdp_conn_originate(UINT8 *p_bd_addr);
void sdp_disconnect(tCONN_CB *p_ccb, UINT16 reason);
/**/
void sdp_conn_timeout(tCONN_CB *p_ccb);

void sdp_server_handle_client_req(tCONN_CB *p_ccb, BT_HDR *p_msg);

tCONN_CB *sdpu_find_ccb_by_cid(UINT16 cid);
tCONN_CB *sdpu_allocate_ccb(void);
void sdpu_release_ccb(tCONN_CB *p_ccb);
UINT8 *sdpu_build_attrib_seq(UINT8 *p_out, UINT16 *p_attr, UINT16 num_attrs);
UINT8 *sdpu_build_attrib_entry(UINT8 *p_out, tSDP_ATTRIBUTE *p_attr);
void sdpu_build_n_send_error(tCONN_CB *p_ccb, UINT16 trans_num,
                             UINT16 error_code, char *p_error_text);
UINT8 *sdpu_extract_uid_seq(UINT8 *p, UINT16 param_len, tSDP_UUID_SEQ *p_seq);
UINT8 *sdpu_extract_attr_seq(UINT8 *p, UINT16 param_len, tSDP_ATTR_SEQ *p_seq);
UINT8 *sdpu_get_len_from_type(UINT8 *p, UINT8 type, UINT32 *p_len);
BOOLEAN sdpu_is_base_uuid(UINT8 *p_uuid);
BOOLEAN sdpu_compare_uuid_arrays(UINT8 *p_uuid1, UINT32 len1, UINT8 *p_uuid2,
                                 UINT16 len2);
BOOLEAN sdpu_compare_bt_uuids(tBT_UUID *p_uuid1, tBT_UUID *p_uuid2);
BOOLEAN sdpu_compare_uuid_with_attr(tBT_UUID *p_btuuid, tSDP_DISC_ATTR *p_attr);
void sdpu_sort_attr_list(UINT16 num_attr, tSDP_DISCOVERY_DB *p_db);

#ifdef __cplusplus
	}
#endif

#endif // BTE_SDP_INT_H
