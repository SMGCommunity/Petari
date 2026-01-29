#ifndef BTE_L2CAP_INT_H
#define BTE_L2CAP_INT_H

/* Original source:
 * bluedroid <android.googlesource.com/platform/external/bluetooth/bluedroid>
 * stack/l2cap/l2c_int.h
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

/*******************************************************************************
 * macros
 */

#define L2CAP_MIN_MTU					48

#define L2CAP_ECHO_RSP_TOUT				30
#define L2CAP_CHNL_CONNECT_TOUT			60
#define L2CAP_CHNL_CFG_TOUT				30
#define L2CAP_CHNL_CONNECT_TOUT_EXT		120
#define L2CAP_CHNL_DISCONNECT_TOUT		30
#define L2CAP_LINK_CONNECT_TOUT			60
#define L2CAP_DELAY_CHECK_SM4			2
#define L2CAP_LINK_DISCONNECT_TOUT		30
#define L2CAP_LINK_FLOW_CONTROL_TOUT	2
#define L2CAP_LINK_CONNECT_TOUT_EXT		120
#define L2CAP_LINK_ROLE_SWITCH_TOUT		10

#define L2CAP_SEND_CMD_OFFSET			0

/*******************************************************************************
 * types
 */

#ifdef __cplusplus
	extern "C" {
#endif

typedef enum
{
	CST_CLOSED,
	CST_ORIG_W4_SEC_COMP,
	CST_TERM_W4_SEC_COMP,
	CST_W4_L2CAP_CONNECT_RSP,
	CST_W4_L2CA_CONNECT_RSP,
	CST_CONFIG,
	CST_OPEN,
	CST_W4_L2CAP_DISCONNECT_RSP,
	CST_W4_L2CA_DISCONNECT_RSP,
} tL2C_CHNL_STATE;

typedef enum
{
	LST_DISCONNECTED,
	LST_CONNECT_HOLDING,
	LST_CONNECTING_WAIT_SWITCH,
	LST_CONNECTING,
	LST_CONNECTED,
	LST_DISCONNECTING,
} tL2C_LINK_STATE;

enum
{
	L2CEVT_LP_CONNECT_CFM			= 0,
	L2CEVT_LP_CONNECT_CFM_NEG		= 1,
	L2CEVT_LP_DISCONNECT_IND		= 3,
	L2CEVT_LP_QOS_VIOLATION_IND		= 6,
	L2CEVT_SEC_COMP					= 7,
	L2CEVT_SEC_COMP_NEG				= 8,
	L2CEVT_L2CAP_CONNECT_REQ		= 10,
	L2CEVT_L2CAP_CONNECT_RSP		= 11,
	L2CEVT_L2CAP_CONNECT_RSP_PND	= 12,
	L2CEVT_L2CAP_CONNECT_RSP_NEG	= 13,
	L2CEVT_L2CAP_CONFIG_REQ			= 14,
	L2CEVT_L2CAP_CONFIG_RSP			= 15,
	L2CEVT_L2CAP_CONFIG_RSP_NEG		= 16,
	L2CEVT_L2CAP_DISCONNECT_REQ		= 17,
	L2CEVT_L2CAP_DISCONNECT_RSP		= 18,

	L2CEVT_L2CAP_DATA				= 19,
	L2CEVT_L2CA_CONNECT_REQ			= 20,
	L2CEVT_L2CA_CONNECT_RSP			= 21,
	L2CEVT_L2CA_CONNECT_RSP_NEG		= 22,
	L2CEVT_L2CA_CONFIG_REQ			= 23,
	L2CEVT_L2CA_CONFIG_RSP			= 24,
	L2CEVT_L2CA_CONFIG_RSP_NEG		= 25,
	L2CEVT_L2CA_DISCONNECT_REQ		= 26,
	L2CEVT_L2CA_DISCONNECT_RSP		= 27,
	L2CEVT_L2CA_DATA_WRITE			= 29,

	L2CEVT_TIMEOUT					= 30,
	L2CEVT_SEC_RE_SEND_CMD			= 31,
	L2CEVT_ACK_TIMEOUT				= 32,

#define L2CEVT_SEC_COMP		(L2CEVT_SEC_COMP + 0)
#define L2CEVT_SEC_COMP_NEG	(L2CEVT_SEC_COMP_NEG + 0)
};

enum
{
	L2CAP_ADJ_ID	= 0x03,
};

enum
{
	L2CAP_PEER_CFG_UNACCEPTABLE,
	L2CAP_PEER_CFG_OK,
	L2CAP_PEER_CFG_DISCONNECT,
};

typedef UINT8 tL2C_CCB_CFG_FLAGS;
enum
{
	IB_CFG_DONE		= 1 << 0,
	OB_CFG_DONE		= 1 << 1,
	RECONFIG_FLAG	= 1 << 2,

	CFG_DONE_MASK	= IB_CFG_DONE | OB_CFG_DONE
};

typedef struct
{
	BD_ADDR	bd_addr;		// size 0x06, offset 0x00
	UINT8	status;			// size 0x01, offset 0x06
	/* 1 byte padding */
	UINT16	psm;			// size 0x02, offset 0x08
	UINT16	l2cap_result;	// size 0x02, offset 0x0a
	UINT16	l2cap_status;	// size 0x02, offset 0x0c
	UINT16	remote_cid;		// size 0x02, offset 0x0e
} tL2C_CONN_INFO; // size 0x10

typedef struct
{
	BOOLEAN				in_use;	// size 0x01, offset 0x00
	/* 1 byte padding */
	UINT16				psm;	// size 0x02, offset 0x02
	tL2CAP_APPL_INFO	api;	// size 0x28, offset 0x04
} tL2C_RCB; // size 0x2c

typedef struct t_l2c_ccb
{
	BOOLEAN				in_use;			// size 0x01, offset 0x00
	/* 3 bytes padding */
	tL2C_CHNL_STATE		chnl_state;		// size 0x04, offset 0x04
	struct t_l2c_ccb	*p_next_ccb;	// size 0x04, offset 0x08
	struct t_l2c_ccb	*p_prev_ccb;	// size 0x04, offset 0x0c
	struct t_l2c_linkcb	*p_lcb;			// size 0x04, offset 0x10
	UINT16				local_cid;		// size 0x02, offset 0x14
	UINT16				remote_cid;		// size 0x02, offset 0x16
	TIMER_LIST_ENT		timer_entry;	// size 0x18, offset 0x18
	tL2C_RCB			*p_rcb;			// size 0x04, offset 0x30
	tL2C_CCB_CFG_FLAGS	config_done;	// size 0x01, offset 0x34
	UINT8				local_id;		// size 0x01, offset 0x35
	UINT8				remote_id;		// size 0x01, offset 0x36
	BOOLEAN				rtry_flag;		// size 0x01, offset 0x37
	UINT16				in_mtu;			// size 0x02, offset 0x38
	UINT16				out_mtu;		// size 0x02, offset 0x3a
	UINT16				our_flush_to;	// size 0x02, offset 0x3c
	/* 2 bytes padding */
	FLOW_SPEC			in_flow;		// size 0x18, offset 0x40
	FLOW_SPEC			out_flow;		// size 0x18, offset 0x58
	BUFFER_Q			xmit_hold_q;	// size 0x0c, offset 0x70
} tL2C_CCB; // size 0x7c

typedef struct t_l2c_linkcb
{
	BOOLEAN				in_use;						// size 0x01, offset 0x00
	/* 3 bytes padding */
	tL2C_LINK_STATE		link_state;					// size 0x04, offset 0x04
	tL2C_CCB			*p_first_ccb;				// size 0x04, offset 0x08
	tL2C_CCB			*p_last_ccb;				// size 0x04, offset 0x0c
	TIMER_LIST_ENT		timer_entry;				// size 0x18, offset 0x10
	UINT16				handle;						// size 0x02, offset 0x28
	BD_ADDR				remote_bd_addr;				// size 0x06, offset 0x2a
	UINT8				link_role;					// size 0x01, offset 0x30
	UINT8				id;							// size 0x01, offset 0x31
	UINT16				link_flush_tout;			// size 0x02, offset 0x32
	UINT16				link_xmit_quota;			// size 0x02, offset 0x34
	UINT16				link_xmit_window;			// size 0x02, offset 0x36
	UINT16				sent_not_acked;				// size 0x02, offset 0x38
	UINT16				link_cong_start;			// size 0x02, offset 0x3a
	UINT16				link_cong_end;				// size 0x02, offset 0x3c
	UINT16				link_cong_discard;			// size 0x02, offset 0x3e
	BOOLEAN				partial_segment_being_sent;	// size 0x01, offset 0x40
	BOOLEAN				cong_sent;					// size 0x01, offset 0x41
	/* 2 bytes padding */
	BUFFER_Q			link_xmit_data_q;			// size 0x0c, offset 0x44
	BT_HDR				*p_hcit_rcv_acl;			// size 0x04, offset 0x50
	tL2CA_ECHO_RSP_CB	*p_echo_rsp_cb;				// size 0x04, offset 0x54
	UINT16				idle_timeout;				// size 0x02, offset 0x58
	UINT8				acl_priority;				// size 0x01, offset 0x5a
	/* 1 byte padding */
} tL2C_LCB; // size 0x5c

typedef struct
{
	UINT8			l2cap_trace_level;				// size 0x001, offset 0x000
	UINT8			desire_role;					// size 0x001, offset 0x001
	UINT16			num_lm_acl_bufs;				// size 0x002, offset 0x002
	UINT16			controller_xmit_window;			// size 0x002, offset 0x004
	/* 2 bytes padding */
	tL2C_LCB		lcb_pool[MAX_L2CAP_LINKS];		// size 0x170, offset 0x008
	tL2C_CCB		ccb_pool[MAX_L2CAP_CHANNELS];	// size 0x4d8, offset 0x178
	tL2C_RCB		rcb_pool[MAX_L2CAP_CLIENTS];	// size 0x160, offset 0x650
	tL2C_CCB		*p_free_ccb_first;				// size 0x004, offset 0x7b0
	tL2C_CCB		*p_free_ccb_last;				// size 0x004, offset 0x7b4
	UINT16			num_links_active;				// size 0x002, offset 0x7b8
	UINT16			idle_timeout;					// size 0x002, offset 0x7ba
	tL2C_LCB		*p_cur_hcit_lcb;				// size 0x004, offset 0x7bc
	BUFFER_Q		rcv_hold_q;						// size 0x00c, offset 0x7c0
	TIMER_LIST_ENT	rcv_hold_tle;					// size 0x018, offset 0x7cc
	UINT8			compress_pool_id;				// size 0x001, offset 0x7e4
	/* 3 bytes padding */
} tL2C_CB; // size 0x7e8

/*******************************************************************************
 * external globals
 */

extern tL2C_CB l2cb;

/*******************************************************************************
 * functions
 */

void l2c_csm_execute(tL2C_CCB *p_ccb, UINT16 event, void *p_data);
void forward_peer_data(tL2C_CCB *p_ccb, BT_HDR *p_buf);

void l2c_link_hci_conn_req(BD_ADDR bd_addr);
BOOLEAN l2c_link_hci_conn_comp(UINT8 status, UINT16 handle, BD_ADDR p_bda);
void l2c_link_sec_comp(BD_ADDR p_bda, void *p_ref_data, UINT8 status);
BOOLEAN l2c_link_hci_disc_comp(UINT16 handle, UINT8 reason);
BOOLEAN l2c_link_hci_qos_violation(UINT16 handle);
void l2c_link_timeout(tL2C_LCB *p_lcb);
BOOLEAN l2c_link_send_to_lower(tL2C_LCB *p_lcb, BT_HDR *p_buf);
void l2c_link_check_send_pkts(tL2C_LCB *p_lcb,
                              tL2C_CCB *p_ccb,
                              BT_HDR *p_buf);
void l2c_link_adjust_allocation(void);
void l2c_link_process_num_completed_pkts(UINT8 *p);
/* NOTE the misspelling of process */
void l2c_link_processs_num_bufs(UINT16 num_lm_acl_bufs);
BT_HDR *l2cap_link_chk_pkt_start(BT_HDR *p_cur_buf);
BOOLEAN l2cap_link_chk_pkt_end(void);
BT_HDR *l2c_link_get_decompressed_pkt(BT_HDR *p_cur);
UINT8 l2c_link_pkts_rcvd(UINT16 *num_pkts, UINT16 *handles);
void l2c_link_role_changed(BD_ADDR bd_addr, UINT8 new_role);
void l2c_link_role_change_failed(void);
void l2c_link_segments_xmitted(BT_HDR *p_msg);
void l2c_pin_code_request(BD_ADDR bd_addr);

void l2c_init(void);
void l2c_rcv_acl_data(BT_HDR *p_msg);
void l2c_process_compress_setup_timeout(TIMER_LIST_ENT *p_tle);
void l2c_process_timeout(TIMER_LIST_ENT *p_tle);
void l2c_process_held_packets(BOOLEAN timed_out);

tL2C_LCB *l2cu_allocate_lcb(BD_ADDR p_bd_addr);
void l2cu_release_lcb(tL2C_LCB *p_lcb);
tL2C_LCB *l2cu_find_lcb_by_bd_addr(BD_ADDR p_bd_addr);
tL2C_LCB *l2cu_find_lcb_by_handle(UINT16 handle);
UINT8 l2cu_get_conn_role (tL2C_LCB *p_this_lcb);
BT_HDR *l2cu_build_header(UINT16 hci_handle, UINT16 len, UINT8 cmd, UINT8 id);
void l2cu_send_peer_cmd_reject(tL2C_LCB *p_lcb, UINT16 reason, UINT8 rem_id,
                               UINT16 p1, UINT16 p2);
void l2cu_send_peer_connect_req(tL2C_CCB *p_ccb);
void l2cu_send_peer_connect_rsp(tL2C_CCB *p_ccb, UINT16 result, UINT16 status);
void l2cu_reject_connection(tL2C_LCB *p_lcb, UINT16 remote_cid, UINT8 rem_id,
                            UINT16 result);
void l2cu_send_peer_config_req(tL2C_CCB *p_ccb, tL2CAP_CFG_INFO *p_cfg);
void l2cu_send_peer_config_rsp(tL2C_CCB *p_ccb, tL2CAP_CFG_INFO *p_cfg);
void l2cu_send_peer_config_rej(tL2C_CCB *p_ccb, UINT8 *p_data, UINT16 data_len,
                               UINT16 rej_len);
void l2cu_send_peer_disc_req(tL2C_CCB *p_ccb);
void l2cu_send_peer_disc_rsp(tL2C_LCB *p_lcb, UINT8 remote_id, UINT16 local_cid,
                             UINT16 remote_cid);
void l2cu_send_peer_echo_req(tL2C_LCB *p_lcb, UINT8 *p_data, UINT16 data_len);
void l2cu_send_peer_echo_rsp(tL2C_LCB *p_lcb, UINT8 id, UINT8 *p_data,
                             UINT16 data_len);
void l2cu_send_peer_info_req(tL2C_LCB *p_lcb, UINT16 info_type);
void l2cu_send_peer_info_rsp(tL2C_LCB *p_lcb, UINT8 remote_id, UINT16 info_type);
tL2C_CCB *l2cu_allocate_ccb(tL2C_LCB *p_lcb);
void l2cu_release_ccb(tL2C_CCB *p_ccb);
tL2C_CCB *l2cu_find_ccb_by_cid(tL2C_LCB *p_lcb, UINT16 local_cid);
tL2C_CCB *l2cu_find_ccb_by_remote_cid(tL2C_LCB *p_lcb, UINT16 remote_cid);
tL2C_RCB *l2cu_allocate_rcb(UINT16 psm);
void l2cu_release_rcb(tL2C_RCB *p_rcb);
tL2C_RCB *l2cu_find_rcb_by_psm(UINT16 psm);
BOOLEAN l2cu_process_peer_cfg_req(tL2C_CCB *p_ccb, tL2CAP_CFG_INFO *p_cfg);
void l2cu_process_peer_cfg_rsp(tL2C_CCB *p_ccb, tL2CAP_CFG_INFO *p_cfg);
void l2cu_process_our_cfg_req(tL2C_CCB *p_ccb, tL2CAP_CFG_INFO *p_cfg);
void l2cu_process_our_cfg_rsp(tL2C_CCB *p_ccb, tL2CAP_CFG_INFO *p_cfg);
void l2cu_device_reset(void);
BOOLEAN l2cu_create_conn(tL2C_LCB *p_lcb);
BOOLEAN l2cu_create_conn_after_switch(tL2C_LCB *p_lcb);
tL2C_LCB *l2cu_find_lcb_by_state(tL2C_LINK_STATE state);
BOOLEAN l2cu_lcb_disconnecting(void);
void l2cu_send_feature_req(tL2C_CCB *p_ccb);
BOOLEAN l2cu_check_feature_req(tL2C_LCB *p_lcb, UINT8 id, UINT8 *p_data,
                               UINT16 data_len);
void l2cu_check_feature_rsp(tL2C_LCB *p_lcb, UINT8 id, UINT8 *p_data,
                            UINT16 data_len);
void l2cu_stop_compression(/* void? */);
BOOLEAN l2cu_lcb_has_feature_enable(/* tL2C_LCB *p_lcb? */);
BOOLEAN l2cu_ccb_has_compression_enable(/* tL2C_CCB *p_ccb? */);
UINT32 L2CA_GetCompressionFixMemoryPoolSize(/* void? */);

#ifdef __cplusplus
	}
#endif

#endif // BTE_L2CAP_INT_H
