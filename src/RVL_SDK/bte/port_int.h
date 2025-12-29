#ifndef BTE_RFCOMM_PORT_INT_H
#define BTE_RFCOMM_PORT_INT_H

/* Original source:
 * bluedroid <android.googlesource.com/platform/external/bluetooth/bluedroid>
 * stack/rfcomm/port_int.h
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
#include "port_api.h"
#include "rfcdefs.h"

/*******************************************************************************
 * types
 */

#ifdef __cplusplus
	extern "C" {
#endif

typedef UINT8 tPORT_FC_MECHANISM;
enum
{
	PORT_FC_UNDEFINED,

	PORT_FC_TS710,
	PORT_FC_CREDIT,
};

typedef UINT8 tPORT_CTRL_MODEM_SIGNAL;
enum
{
	MODEM_SIGNAL_DTRDSR	= 1 << 0,
	MODEM_SIGNAL_RTSCTS	= 1 << 1,
	MODEM_SIGNAL_RI		= 1 << 2,
	MODEM_SIGNAL_DCD	= 1 << 3,
};

typedef UINT8 tPORT_CTRL_BREAK_SIGNAL;
enum
{
	RFCOMM_CTRL_BREAK_ASAP,
	RFCOMM_CTRL_BREAK_IN_SEQ,
};

typedef UINT8 tRFC_PORT_STATE;
enum
{
	RFC_PORT_STATE_IDLE,
	RFC_PORT_STATE_WAIT_START,
	RFC_PORT_STATE_OPENING,
	RFC_PORT_STATE_OPENED,
	RFC_PORT_STATE_CLOSING,
};

typedef UINT8 tRFC_PORT_RSP_FLAGS;
enum
{
	RFC_RSP_PN			= 1 << 0,
	RFC_RSP_RPN_REPLY	= 1 << 1,
	RFC_RSP_RPN			= 1 << 2,
	RFC_RSP_MSC			= 1 << 3,
	RFC_RSP_RLS			= 1 << 4,
};

typedef UINT8 tPORT_INFO_STATE;
enum
{
	PORT_STATE_CLOSED,
	PORT_STATE_OPENING,
	PORT_STATE_OPENED,
	PORT_STATE_CLOSING,
};

typedef UINT8 tPORT_INFO_CTRL;
enum
{
	PORT_CTRL_REQ_SENT			= 1 << 0,
	PORT_CTRL_REQ_CONFIRMED		= 1 << 1,
	PORT_CTRL_IND_RECEIVED		= 1 << 2,
	PORT_CTRL_IND_RESPONDED		= 1 << 3,
};

typedef struct
{
	TIMER_LIST_ENT		tle;							// size 0x18, offset 0x00
	BUFFER_Q			cmd_q;							// size 0x0c, offset 0x18
	UINT8				port_inx[RFCOMM_MAX_DLCI + 1];	// size 0x3e, offset 0x24
	BD_ADDR				bd_addr;						// size 0x06, offset 0x62
	UINT16				lcid;							// size 0x02, offset 0x68
	UINT16				peer_l2cap_mtu;					// size 0x02, offset 0x6a
	UINT8				state;							// size 0x01, offset 0x6c
	BOOLEAN				is_initiator;					// size 0x01, offset 0x6d
	BOOLEAN				local_cfg_sent;					// size 0x01, offset 0x6e
	BOOLEAN				peer_cfg_rcvd;					// size 0x01, offset 0x6f
	BOOLEAN				restart_required;				// size 0x01, offset 0x70
	BOOLEAN				peer_ready;						// size 0x01, offset 0x71
	tPORT_FC_MECHANISM	flow;							// size 0x01, offset 0x72
	BOOLEAN				l2cap_congested;				// size 0x01, offset 0x73
	BOOLEAN				is_disc_initiator;				// size 0x01, offset 0x74
	/* 3 bytes padding */
} tRFC_MCB; // size 0x78

typedef struct t_rfc_port
{
	tRFC_PORT_STATE		state;			// size 0x01, offset 0x00
	tRFC_PORT_RSP_FLAGS	expected_rsp;	// size 0x01, offset 0x01
	/* 2 bytes padding */
	tRFC_MCB			*p_mcb;			// size 0x04, offset 0x04
	TIMER_LIST_ENT		tle;			// size 0x18, offset 0x08
} tRFC_PORT; // size 0x20

typedef struct
{
	BUFFER_Q		queue;			// size 0x0c, offset 0x00
	BOOLEAN			peer_fc;		// size 0x01, offset 0x0c
	BOOLEAN			user_fc;		// size 0x01, offset 0x0d
	/* 2 bytes padding */
	UINT32			queue_size;		// size 0x04, offset 0x10
	tPORT_CALLBACK	*p_callback;	// size 0x04, offset 0x14
} tPORT_DATA; // size 0x18

typedef struct
{
	tPORT_CTRL_MODEM_SIGNAL	modem_signal;		// size 0x01, offset 0x00
	UINT8					break_signal;		// size 0x01, offset 0x01
	UINT8					discard_buffers;	// size 0x01, offset 0x02 // TODO: fold into boolean?
	tPORT_CTRL_BREAK_SIGNAL	break_signal_seq;	// size 0x01, offset 0x03
	BOOLEAN					fc;					// size 0x01, offset 0x04
} tPORT_CTRL; // size 0x05

typedef struct t_port_info
{
	UINT8					inx;					// size 0x01, offset 0x00
	BOOLEAN					in_use;					// size 0x01, offset 0x01
	tPORT_INFO_STATE		state;					// size 0x01, offset 0x02
	UINT8					scn;					// size 0x01, offset 0x03
	UINT16					uuid;					// size 0x02, offset 0x04
	BD_ADDR					bd_addr;				// size 0x06, offset 0x06
	BOOLEAN					is_server;				// size 0x01, offset 0x0c
	UINT8					dlci;					// size 0x01, offset 0x0d
	UINT8					error;					// size 0x01, offset 0x0e
	UINT8					line_status;			// size 0x01, offset 0x0f
	UINT8					default_signal_state;	// size 0x01, offset 0x10
	/* 1 byte padding */
	UINT16					mtu;					// size 0x02, offset 0x12
	UINT16					peer_mtu;				// size 0x02, offset 0x14
	/* 2 bytes padding */
	tPORT_DATA				tx;						// size 0x18, offset 0x18
	tPORT_DATA				rx;						// size 0x18, offset 0x30
	tPORT_STATE				user_port_pars;			// size 0x09, offset 0x48
	tPORT_STATE				peer_port_pars;			// size 0x09, offset 0x51
	tPORT_CTRL				local_ctrl;				// size 0x05, offset 0x5a
	tPORT_CTRL				peer_ctrl;				// size 0x05, offset 0x5f
	tPORT_INFO_CTRL			port_ctrl;				// size 0x01, offset 0x64
	BOOLEAN					rx_flag_ev_pending;		// size 0x01, offset 0x65
	/* 2 bytes padding */
	tRFC_PORT				rfc;					// size 0x20, offset 0x68
	UINT32					ev_mask;				// size 0x04, offset 0x88
	tPORT_CALLBACK			*p_callback;			// size 0x04, offset 0x8c
	tPORT_CALLBACK			*p_mgmt_callback;		// size 0x04, offset 0x90
	tPORT_DATA_CALLBACK		*p_data_callback;		// size 0x04, offset 0x94
	UINT16					credit_tx;				// size 0x02, offset 0x98
	UINT16					credit_rx;				// size 0x02, offset 0x9a
	UINT16					credit_rx_max;			// size 0x02, offset 0x9c
	UINT16					credit_rx_low;			// size 0x02, offset 0x9e
	UINT16					rx_buf_critical;		// size 0x02, offset 0xa0
	/* 2 bytes padding */
} tPORT; // size 0xa4

typedef struct
{
	tPORT		port[MAX_RFC_PORTS];			// size 0x334, offset 0x000
	tRFC_MCB	rfc_mcb[MAX_BD_CONNECTIONS];	// size 0x078, offset 0x334
} tPORT_CB; // size 0x3ac

/*******************************************************************************
 * functions
 */

int port_open_continue(tPORT *p_port);
void port_start_control(tPORT *p_port);
void port_start_par_neg(tPORT *p_port);
void port_start_close(tPORT *p_port);
void PORT_StartCnf(tRFC_MCB *p_mcb, UINT16 result);
void PORT_StartInd(tRFC_MCB *p_mcb);
void PORT_ParNegInd(tRFC_MCB *p_mcb, UINT8 dlci, UINT16 mtu, UINT8 cl, UINT8 k);
void PORT_ParNegCnf(tRFC_MCB *p_mcb, UINT8 dlci, UINT16 mtu, UINT8 cl, UINT8 k);
void PORT_DlcEstablishInd(tRFC_MCB *p_mcb, UINT8 dlci, UINT16 mtu);
void PORT_DlcEstablishCnf(tRFC_MCB *p_mcb, UINT8 dlci, UINT16 mtu,
                          UINT16 result);
void PORT_PortNegInd(tRFC_MCB *p_mcb, UINT8 dlci, tPORT_STATE *p_pars,
                     UINT16 param_mask);
void PORT_PortNegCnf(tRFC_MCB *p_mcb, UINT8 dlci, tPORT_STATE *p_pars,
                     UINT16 result);
void PORT_ControlInd(tRFC_MCB *p_mcb, UINT8 dlci, tPORT_CTRL *p_pars);
void PORT_ControlCnf(tRFC_MCB *p_mcb, UINT8 dlci, tPORT_CTRL *p_pars);
void PORT_LineStatusInd(tRFC_MCB *p_mcb, UINT8 dlci, UINT8 line_status);
void PORT_DlcReleaseInd(tRFC_MCB *p_mcb, UINT8 dlci);
void PORT_CloseInd(tRFC_MCB *p_mcb);
void Port_TimeOutCloseMux(tRFC_MCB *p_mcb);
void PORT_DataInd(tRFC_MCB *p_mcb, UINT8 dlci, BT_HDR *p_buf);
void PORT_FlowInd(tRFC_MCB *p_mcb, UINT8 dlci, BOOLEAN enable_data);
UINT32 port_rfc_send_tx_data(tPORT *p_port);
void port_rfc_closed(tPORT *p_port, UINT8 res);
void port_get_credits(tPORT *p_port, UINT8 k);

tPORT *port_allocate_port(UINT8 dlci, BD_ADDR bd_addr);
void port_set_defaults(tPORT *p_port);
void port_select_mtu(tPORT *p_port);
void port_release_port(tPORT *p_port);
tRFC_MCB *port_find_mcb(BD_ADDR bd_addr);
tPORT *port_find_mcb_dlci_port(tRFC_MCB *p_mcb, UINT8 dlci);
tPORT *port_find_dlci_port(UINT8 dlci);
tPORT *port_find_port(UINT8 dlci, BD_ADDR bd_addr);
UINT32 port_flow_control_user(tPORT *p_port);
UINT32 port_get_signal_changes(tPORT *p_port, UINT8 old_signals, UINT8 signal);
void port_flow_control_peer(tPORT *p_port, BOOLEAN enable, UINT16 count);

#ifdef __cplusplus
	}
#endif

#endif // BTE_RFCOMM_PORT_INT_H
