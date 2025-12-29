#ifndef BTE_RFCOMM_INT_H
#define BTE_RFCOMM_INT_H

/* Original source:
 * bluedroid <android.googlesource.com/platform/external/bluetooth/bluedroid>
 * stack/rfcomm/rfc_int.h
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

#include "l2c_api.h"
#include "port_api.h"
#include "port_int.h"

/*******************************************************************************
 * macros
 */

#define RFCOMM_MIN_MTU						23
#define RFCOMM_MAX_MTU						32767

#define RFC_T1_TIMEOUT						20
#define RFC_PORT_T1_TIMEOUT					60
#define RFC_T2_TIMEOUT						60
#define RFC_DISC_TIMEOUT					3
#define RFCOMM_CONN_TIMEOUT					120

#define RFC_MCB_INIT_INACT_TIMER			60
#define RFC_MCB_RELEASE_INACT_TIMER			2

#define RFCOMM_SABME_FCS(p_data, cr, dlci)	rfc_calc_fcs(3, p_data)
#define RFCOMM_UA_FCS(p_data, cr, dlci)		rfc_calc_fcs(3, p_data)
#define RFCOMM_DM_FCS(p_data, cr, dlci)		rfc_calc_fcs(3, p_data)
#define RFCOMM_DISC_FCS(p_data, cr, dlci)	rfc_calc_fcs(3, p_data)
#define RFCOMM_UIH_FCS(p_data, dlci)		rfc_calc_fcs(2, p_data)

/*******************************************************************************
 * types
 */

#ifdef __cplusplus
	extern "C" {
#endif

enum
{
	RFCOMM_SUCCESS		= 0,
	RFCOMM_ERROR		= 1,
	RFCOMM_SECURITY_ERR	= 112,
};

enum
{
	LINE_STATUS_OVERRUN	= 1 << 1,
	LINE_STATUS_FAILED	= 1 << 4,
};

enum
{
	RFC_MX_STATE_IDLE,
	RFC_MX_STATE_WAIT_CONN_CNF,
	RFC_MX_STATE_CONFIGURE,
	RFC_MX_STATE_SABME_WAIT_UA,
	RFC_MX_STATE_WAIT_SABME,
	RFC_MX_STATE_CONNECTED,
	RFC_MX_STATE_DISC_WAIT_UA,
};

enum
{
	RFC_STATE_CLOSED,
	RFC_STATE_SABME_WAIT_UA,
	RFC_STATE_ORIG_WAIT_SEC_CHECK,
	RFC_STATE_TERM_WAIT_SEC_CHECK,
	RFC_STATE_OPENED,
	RFC_STATE_DISC_WAIT_UA,
};

enum
{
	RFC_EVENT_SABME			=  0,
	RFC_EVENT_UA			=  1,
	RFC_EVENT_DM			=  2,
	RFC_EVENT_DISC			=  3,
	RFC_EVENT_UIH			=  4,
	RFC_EVENT_TIMEOUT		=  5,

	RFC_EVENT_BAD_FRAME		= 50,
};

enum
{
	RFC_MX_EVENT_START_REQ			=  6,
	RFC_MX_EVENT_START_RSP			=  7,
	RFC_MX_EVENT_CLOSE_REQ			=  8,
	RFC_MX_EVENT_CONN_CNF			=  9,
	RFC_MX_EVENT_CONN_IND			= 10,
	RFC_MX_EVENT_CONF_CNF			= 11,
	RFC_MX_EVENT_CONF_IND			= 12,
	RFC_MX_EVENT_QOS_VIOLATION_IND	= 13,
	RFC_MX_EVENT_DISC_IND			= 14,
};

enum
{
	RFC_EVENT_OPEN			=  9,

	RFC_EVENT_ESTABLISH_RSP	= 11,
	RFC_EVENT_CLOSE			= 12,
	RFC_EVENT_CLEAR			= 13,
	RFC_EVENT_DATA			= 14,
	RFC_EVENT_SEC_COMPLETE	= 15,
};

typedef struct
{
	UINT8	dlci;	// size 0x01, offset 0x00
	UINT8	type;	// size 0x01, offset 0x01
	UINT8	cr;		// size 0x01, offset 0x02
	UINT8	ea;		// size 0x01, offset 0x03
	UINT8	pf;		// size 0x01, offset 0x04
	UINT8	credit;	// size 0x01, offset 0x05
	/* 2 bytes padding */
	union
	{
		struct
		{
			UINT8	dlci;			// size 0x01, offset 0x00
			UINT8	frame_type;		// size 0x01, offset 0x01
			UINT8	conv_layer;		// size 0x01, offset 0x02
			UINT8	priority;		// size 0x01, offset 0x03
			UINT8	t1;				// size 0x01, offset 0x04
			/* 1 byte padding */
			UINT16	mtu;			// size 0x01, offset 0x06
			UINT8	n2;				// size 0x01, offset 0x08
			UINT8	k;				// size 0x01, offset 0x09
		} pn; // size 0x0a
		struct
		{
			UINT8	*p_data;		// size 0x04, offset 0x00
			UINT16	data_len;		// size 0x02, offset 0x04
			/* 2 bytes padding */
		} test; // size 0x08
		struct
		{
			UINT8	dlci;			// size 0x01, offset 0x00
			UINT8	signals;		// size 0x01, offset 0x01
			UINT8	break_present;	// size 0x01, offset 0x02
			UINT8	break_duration;	// size 0x01, offset 0x03
		} msc; // size 0x04
		struct
		{
			UINT8	ea;				// size 0x01, offset 0x00
			UINT8	cr;				// size 0x01, offset 0x01
			UINT8	type;			// size 0x01, offset 0x02
		} nsc; // size 0x03
		struct
		{
			UINT8	dlci;			// size 0x01, offset 0x00
			UINT8	is_request;		// size 0x01, offset 0x01
			UINT8	baud_rate;		// size 0x01, offset 0x02
			UINT8	byte_size;		// size 0x01, offset 0x03
			UINT8	stop_bits;		// size 0x01, offset 0x04
			UINT8	parity;			// size 0x01, offset 0x05
			UINT8	parity_type;	// size 0x01, offset 0x06
			UINT8	fc_type;		// size 0x01, offset 0x07
			UINT8	xon_char;		// size 0x01, offset 0x08
			UINT8	xoff_char;		// size 0x01, offset 0x09
			UINT16	param_mask;		// size 0x02, offset 0x0a
		} rpn; // size 0x0c
		struct
		{
			UINT8	dlci;			// size 0x01, offset 0x00
			UINT8	line_status;	// size 0x01, offset 0x01
		} rls; // size 0x02
	} u; // size 0x0c, offset 0x08
} MX_FRAME; // size 0x14

typedef struct
{
	MX_FRAME			rx_frame;								// size 0x14, offset 0x00
	tL2CAP_APPL_INFO	reg_info;								// size 0x28, offset 0x14
	tRFC_MCB			*p_rfc_lcid_mcb[MAX_L2CAP_CHANNELS];	// size 0x28, offset 0x3c
	BOOLEAN				peer_rx_disabled;						// size 0x01, offset 0x64
	UINT8				last_mux;								// size 0x01, offset 0x65
	UINT8				last_port;								// size 0x01, offset 0x66
	/* 1 byte padding */
} tRFCOMM_CB; // size 0x68

typedef struct
{
	tRFCOMM_CB	rfc;			// size 0x068, offset 0x000
	tPORT_CB	port;			// size 0x3ac, offset 0x068
	UINT8		trace_level;	// size 0x001, offset 0x414
	/* 3 bytes padding */
} tRFC_CB; // size 0x418

/*******************************************************************************
 * external globals
 */

extern tRFC_CB rfc_cb;

/*******************************************************************************
 * functions
 */

UINT8 rfc_calc_fcs(UINT16 len, UINT8 *p);

// ---

void rfcomm_l2cap_if_init(void);
tRFC_MCB *rfc_find_lcid_mcb(UINT16 lcid);
void rfc_save_lcid_mcb(tRFC_MCB *p_mcb, UINT16 lcid);

void rfc_mx_sm_execute(tRFC_MCB *p_mcb, UINT16 event, void *p_data);

void rfc_port_sm_execute(tPORT *p_port, UINT16 event, void *p_data);
/**/
void rfc_process_pn(tRFC_MCB *p_mcb, BOOLEAN is_command, MX_FRAME *p_frame);
void rfc_process_rpn(tRFC_MCB *p_mcb, BOOLEAN is_command, BOOLEAN is_request,
                     MX_FRAME *p_frame);
void rfc_process_msc(tRFC_MCB *p_mcb, BOOLEAN is_command, MX_FRAME *p_frame);
void rfc_process_rls(tRFC_MCB *p_mcb, BOOLEAN is_command, MX_FRAME *p_frame);
void rfc_process_nsc (tRFC_MCB *p_mcb, MX_FRAME *p_frame);
void rfc_process_test_rsp(tRFC_MCB *p_mcb, BT_HDR *p_buf);
void rfc_process_fcon(tRFC_MCB *p_mcb, BOOLEAN is_command);
void rfc_process_fcoff(tRFC_MCB *p_mcb, BOOLEAN is_command);
void rfc_process_l2cap_congestion(tRFC_MCB *p_mcb, BOOLEAN is_congested);

void RFCOMM_StartReq(tRFC_MCB *p_mcb);
void RFCOMM_StartRsp(tRFC_MCB *p_mcb, UINT16 result);
void RFCOMM_DlcEstablishReq(tRFC_MCB *p_mcb, UINT8 dlci, UINT16 mtu);
void RFCOMM_DlcEstablishRsp(tRFC_MCB *p_mcb, UINT8 dlci, UINT16 mtu,
                            UINT16 result);
void RFCOMM_ParNegReq(tRFC_MCB *p_mcb, UINT8 dlci, UINT16 mtu);
void RFCOMM_ParNegRsp(tRFC_MCB *p_mcb, UINT8 dlci, UINT16 mtu, UINT8 cl,
                      UINT8 k);
void RFCOMM_PortNegReq(tRFC_MCB *p_mcb, UINT8 dlci, tPORT_STATE *p_pars);
void RFCOMM_PortNegRsp(tRFC_MCB *p_mcb, UINT8 dlci, tPORT_STATE *p_pars,
                       UINT16 param_mask);
void RFCOMM_ControlReq(tRFC_MCB *p_mcb, UINT8 dlci, tPORT_CTRL *p_pars);
void RFCOMM_FlowReq(tRFC_MCB *p_mcb, UINT8 dlci, UINT8 enable);
void RFCOMM_LineStatusReq(tRFC_MCB *p_mcb, UINT8 dlci, UINT8 status);
void RFCOMM_DlcReleaseReq(tRFC_MCB *p_mcb, UINT8 dlci);
void RFCOMM_DataReq(tRFC_MCB *p_mcb, UINT8 dlci, BT_HDR *p_buf);

void rfc_send_sabme(tRFC_MCB *p_mcb, UINT8 dlci);
void rfc_send_ua(tRFC_MCB *p_mcb, UINT8 dlci);
void rfc_send_dm(tRFC_MCB *p_mcb, UINT8 dlci, BOOLEAN pf);
void rfc_send_disc(tRFC_MCB *p_mcb, UINT8 dlci);
void rfc_send_buf_uih(tRFC_MCB *p_mcb, UINT8 dlci, BT_HDR *p_buf);
void rfc_send_pn(tRFC_MCB *p_mcb, UINT8 dlci, BOOLEAN is_command, UINT16 mtu,
                 UINT8 cl, UINT8 k);
void rfc_send_fcon(tRFC_MCB *p_mcb, BOOLEAN is_command);
void rfc_send_fcoff(tRFC_MCB *p_mcb, BOOLEAN is_command);
void rfc_send_msc(tRFC_MCB *p_mcb, UINT8 dlci, BOOLEAN is_command,
                  tPORT_CTRL *p_pars);
void rfc_send_rls(tRFC_MCB *p_mcb, UINT8 dlci, BOOLEAN is_command, UINT8 status);
void rfc_send_nsc(tRFC_MCB *p_mcb);
void rfc_send_rpn(tRFC_MCB *p_mcb, UINT8 dlci, BOOLEAN is_command,
                  tPORT_STATE *p_pars, UINT16 mask);
void rfc_send_test(tRFC_MCB *p_mcb, BOOLEAN is_command, BT_HDR *p_buf);
void rfc_send_credit(tRFC_MCB *p_mcb, UINT8 dlci, UINT8 credit);
UINT8 rfc_parse_data(tRFC_MCB *p_mcb, MX_FRAME *p_frame, BT_HDR *p_buf);
void rfc_process_mx_message(tRFC_MCB *p_mcb, BT_HDR *p_buf);

UINT8 rfc_calc_fcs(UINT16 len, UINT8 *p);
BOOLEAN rfc_check_fcs(UINT16 len, UINT8 *p, UINT8 received_fcs);
tRFC_MCB *rfc_alloc_multiplexer_channel(BD_ADDR bd_addr, BOOLEAN is_initiator);
void rfc_release_multiplexer_channel(tRFC_MCB *p_mcb);
void rfc_timer_start(tRFC_MCB *p_mcb, UINT16 timeout);
void rfc_timer_stop(tRFC_MCB *p_mcb);
void rfc_port_timer_start(tPORT *p_port, UINT16 timeout);
void rfc_port_timer_stop(tPORT *p_port);
void rfc_check_mcb_active(tRFC_MCB *p_mcb);
void rfcomm_process_timeout(TIMER_LIST_ENT *p_tle);
void rfc_sec_check_complete(BD_ADDR bd_addr, void *p_ref_data, UINT8 res);
void rfc_port_closed(tPORT *p_port);
void rfc_inc_credit(tPORT *p_port, UINT8 credit);
void rfc_dec_credit(tPORT *p_port);
void rfc_check_send_cmd(tRFC_MCB *p_mcb, BT_HDR *p_buf);

#ifdef __cplusplus
	}
#endif

#endif // BTE_RFCOMM_INT_H
