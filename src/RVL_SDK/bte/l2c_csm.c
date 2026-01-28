#include "l2c_int.h"

/* Original source:
 * bluedroid <android.googlesource.com/platform/external/bluetooth/bluedroid>
 * stack/l2cap/l2c_csm.c
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

#include <stddef.h>

#include "bt_trace.h"
#include "bt_types.h"
#include "data_types.h"

#include "btm_api.h"
#include "btm_int.h"
#include "btu.h"
#include "gki.h"
#include "hcidefs.h"
#include "l2c_api.h"
#include "l2cdefs.h"

/*******************************************************************************
 * local function declarations
 */

static void l2c_csm_closed(tL2C_CCB *p_ccb, UINT16 event, void *p_data);
static void l2c_csm_orig_w4_sec_comp(tL2C_CCB *p_ccb, UINT16 event,
                                     void *p_data);
static void l2c_csm_term_w4_sec_comp(tL2C_CCB *p_ccb, UINT16 event,
                                     void *p_data);
static void l2c_csm_w4_l2cap_connect_rsp(tL2C_CCB *p_ccb, UINT16 event,
                                         void *p_data);
static void l2c_csm_w4_l2ca_connect_rsp(tL2C_CCB *p_ccb, UINT16 event,
                                        void *p_data);
static void l2c_csm_config(tL2C_CCB *p_ccb, UINT16 event, void *p_data);
static void l2c_csm_open(tL2C_CCB *p_ccb, UINT16 event, void *p_data);
static void l2c_csm_w4_l2cap_disconnect_rsp(tL2C_CCB *p_ccb, UINT16 event,
                                            void *p_data);
static void l2c_csm_w4_l2ca_disconnect_rsp(tL2C_CCB *p_ccb, UINT16 event,
                                           void *p_data);

/*******************************************************************************
 * functions
 */

void l2c_csm_execute(tL2C_CCB *p_ccb, UINT16 event, void *p_data)
{
	switch (p_ccb->chnl_state)
	{
	case CST_CLOSED:
		l2c_csm_closed(p_ccb, event, p_data);
		break;

	case CST_ORIG_W4_SEC_COMP:
		l2c_csm_orig_w4_sec_comp(p_ccb, event, p_data);
		break;

	case CST_TERM_W4_SEC_COMP:
		l2c_csm_term_w4_sec_comp(p_ccb, event, p_data);
		break;

	case CST_W4_L2CAP_CONNECT_RSP:
		l2c_csm_w4_l2cap_connect_rsp(p_ccb, event, p_data);
		break;

	case CST_W4_L2CA_CONNECT_RSP:
		l2c_csm_w4_l2ca_connect_rsp(p_ccb, event, p_data);
		break;

	case CST_CONFIG:
		l2c_csm_config(p_ccb, event, p_data);
		break;

	case CST_OPEN:
		l2c_csm_open(p_ccb, event, p_data);
		break;

	case CST_W4_L2CAP_DISCONNECT_RSP:
		l2c_csm_w4_l2cap_disconnect_rsp(p_ccb, event, p_data);
		break;

	case CST_W4_L2CA_DISCONNECT_RSP:
		l2c_csm_w4_l2ca_disconnect_rsp(p_ccb, event, p_data);
		break;

	default:
		break;
	}
}

static void l2c_csm_closed(tL2C_CCB *p_ccb, UINT16 event, void *p_data)
{
	tL2C_CONN_INFO *p_ci = p_data;
	tL2CA_DISCONNECT_IND_CB *disconnect_ind =
		p_ccb->p_rcb->api.pL2CA_DisconnectInd_Cb;
	tL2CA_CONNECT_CFM_CB *connect_cfm = p_ccb->p_rcb->api.pL2CA_ConnectCfm_Cb;
	UINT16 local_cid = p_ccb->local_cid;

	L2CAP_TRACE(EVENT, "L2CAP - st: CLOSED evt: %d", event);

	switch (event)
	{
	case L2CEVT_LP_DISCONNECT_IND:
		L2CAP_TRACE(
			API,
			"L2CAP - Calling Disconnect_Ind_Cb(), CID: 0x%04x  No Conf Needed",
			p_ccb->local_cid);
		l2cu_release_ccb(p_ccb);
		(*disconnect_ind)(local_cid, FALSE);
		break;

	case L2CEVT_LP_CONNECT_CFM:
		p_ccb->chnl_state = CST_ORIG_W4_SEC_COMP;
		btm_sec_l2cap_access_req(p_ccb->p_lcb->remote_bd_addr,
		                         p_ccb->p_rcb->psm, p_ccb->p_lcb->handle, TRUE,
		                         &l2c_link_sec_comp);
		break;

	case L2CEVT_LP_CONNECT_CFM_NEG:
		L2CAP_TRACE(API,
		            "L2CAP - Calling ConnectCfm_Cb(), CID: 0x%04x  Status: %d",
		            p_ccb->local_cid, p_ci->status);
		l2cu_release_ccb(p_ccb);
		(*connect_cfm)(local_cid, p_ci->status);

		break;

	case L2CEVT_L2CA_CONNECT_REQ:
		if (btm_sec_l2cap_access_req(p_ccb->p_lcb->remote_bd_addr,
		                             p_ccb->p_rcb->psm, p_ccb->p_lcb->handle,
		                             TRUE, &l2c_link_sec_comp)
		    == BTM_CMD_STARTED)
		{
			p_ccb->chnl_state = CST_ORIG_W4_SEC_COMP;
		}

		break;

	case L2CEVT_SEC_COMP:
		l2cu_send_peer_connect_req(p_ccb);

		p_ccb->chnl_state = CST_W4_L2CAP_CONNECT_RSP;

		btu_start_timer(&p_ccb->timer_entry, BTU_TTYPE_L2CAP_CHNL,
		                L2CAP_CHNL_CONNECT_TOUT);

		break;

	case L2CEVT_SEC_COMP_NEG:
		L2CAP_TRACE(API,
		            "L2CAP - Calling ConnectCfm_Cb(), CID: 0x%04x  Status: %d",
		            p_ccb->local_cid, L2CAP_CONN_TIMEOUT);
		l2cu_release_ccb(p_ccb);
		(*connect_cfm)(local_cid, L2CAP_CONN_SECURITY_BLOCK);
		break;

	case L2CEVT_L2CAP_CONNECT_REQ:
		p_ccb->chnl_state = CST_TERM_W4_SEC_COMP;
		btm_sec_l2cap_access_req(p_ccb->p_lcb->remote_bd_addr,
		                             p_ccb->p_rcb->psm, p_ccb->p_lcb->handle,
		                             FALSE, &l2c_link_sec_comp);
		break;

	case L2CEVT_TIMEOUT:
		L2CAP_TRACE(API,
		            "L2CAP - Calling ConnectCfm_Cb(), CID: 0x%04x  Status: %d",
		            p_ccb->local_cid, L2CAP_CONN_TIMEOUT);
		l2cu_release_ccb(p_ccb);
		(*connect_cfm)(local_cid, L2CAP_CONN_TIMEOUT);
		break;

	case L2CEVT_L2CAP_DATA:
	case L2CEVT_L2CA_DATA_WRITE:
		GKI_freebuf(p_data);
		break;

	case L2CEVT_L2CA_DISCONNECT_REQ:
		l2cu_release_ccb(p_ccb);
		break;
	}
}

static void l2c_csm_orig_w4_sec_comp(tL2C_CCB *p_ccb, UINT16 event,
                                     void *p_data)
{
	tL2CA_DISCONNECT_IND_CB *disconnect_ind =
		p_ccb->p_rcb->api.pL2CA_DisconnectInd_Cb;
	tL2CA_CONNECT_CFM_CB *connect_cfm = p_ccb->p_rcb->api.pL2CA_ConnectCfm_Cb;
	UINT16 local_cid = p_ccb->local_cid;

	L2CAP_TRACE(EVENT, "L2CAP - st: ORIG_W4_SEC_COMP evt: %d", event);

	switch (event)
	{
	case L2CEVT_LP_DISCONNECT_IND:
		L2CAP_TRACE(
			API,
			"L2CAP - Calling Disconnect_Ind_Cb(), CID: 0x%04x  No Conf Needed",
			p_ccb->local_cid);

		l2cu_release_ccb(p_ccb);
		(*disconnect_ind)(local_cid, FALSE);
		break;

	case L2CEVT_SEC_COMP:
		p_ccb->chnl_state = CST_W4_L2CAP_CONNECT_RSP;

		btu_start_timer(&p_ccb->timer_entry, BTU_TTYPE_L2CAP_CHNL,
		                L2CAP_CHNL_CONNECT_TOUT);
		l2cu_send_peer_connect_req(p_ccb);

		p_ccb->rtry_flag = FALSE;

		break;

	case L2CEVT_SEC_COMP_NEG:
		L2CAP_TRACE(API,
		            "L2CAP - Calling ConnectCfm_Cb(), CID: 0x%04x  Status: %d",
		            p_ccb->local_cid, HCI_ERR_AUTH_FAILURE);

		l2cu_release_ccb(p_ccb);
		(*connect_cfm)(local_cid, HCI_ERR_AUTH_FAILURE);
		break;

	case L2CEVT_L2CA_DATA_WRITE:
	case L2CEVT_L2CAP_DATA:
		GKI_freebuf(p_data);
		break;

	case L2CEVT_L2CA_DISCONNECT_REQ:
		btm_sec_abort_access_req(p_ccb->p_lcb->remote_bd_addr);

		l2cu_release_ccb(p_ccb);
		break;
	}
}

static void l2c_csm_term_w4_sec_comp(tL2C_CCB *p_ccb, UINT16 event,
                                     void *p_data)
{
	L2CAP_TRACE(EVENT, "L2CAP - st: TERM_W4_SEC_COMP evt: %d", event);

	switch (event)
	{
	case L2CEVT_LP_DISCONNECT_IND:
		btm_sec_abort_access_req(p_ccb->p_lcb->remote_bd_addr);

		l2cu_release_ccb(p_ccb);
		break;

	case L2CEVT_SEC_COMP:
		p_ccb->chnl_state = CST_W4_L2CA_CONNECT_RSP;

		btu_start_timer(&p_ccb->timer_entry, BTU_TTYPE_L2CAP_CHNL,
		                L2CAP_CHNL_CONNECT_TOUT);
		L2CAP_TRACE(API, "L2CAP - Calling Connect_Ind_Cb(), CID: 0x%04x",
		            p_ccb->local_cid);

		(*p_ccb->p_rcb->api.pL2CA_ConnectInd_Cb)(
			p_ccb->p_lcb->remote_bd_addr, p_ccb->local_cid, p_ccb->p_rcb->psm,
			p_ccb->remote_id);
		break;

	case L2CEVT_SEC_COMP_NEG:
		l2cu_send_peer_connect_rsp(p_ccb, L2CAP_CONN_SECURITY_BLOCK, 0);
		l2cu_release_ccb(p_ccb);
		break;

	case L2CEVT_L2CA_DATA_WRITE:
	case L2CEVT_L2CAP_DATA:
		GKI_freebuf(p_data);
		break;

	case L2CEVT_L2CA_DISCONNECT_REQ:
		l2cu_release_ccb(p_ccb);
		break;

	case L2CEVT_L2CAP_DISCONNECT_REQ:
		l2cu_send_peer_disc_rsp(p_ccb->p_lcb, p_ccb->remote_id,
		                        p_ccb->local_cid, p_ccb->remote_cid);

		btm_sec_abort_access_req(p_ccb->p_lcb->remote_bd_addr);

		l2cu_release_ccb(p_ccb);
		break;
	}
}

static void l2c_csm_w4_l2cap_connect_rsp(tL2C_CCB *p_ccb, UINT16 event,
                                         void *p_data)
{
	tL2C_CONN_INFO *p_ci = (tL2C_CONN_INFO *)p_data;
	tL2CA_DISCONNECT_IND_CB *disconnect_ind =
		p_ccb->p_rcb->api.pL2CA_DisconnectInd_Cb;
	tL2CA_CONNECT_CFM_CB *connect_cfm = p_ccb->p_rcb->api.pL2CA_ConnectCfm_Cb;
	UINT16 local_cid = p_ccb->local_cid;

	L2CAP_TRACE(EVENT, "L2CAP - st: W4_L2CAP_CON_RSP evt: %d", event);

	switch (event)
	{
	case L2CEVT_LP_DISCONNECT_IND:
		p_ccb->chnl_state = CST_CLOSED;
		L2CAP_TRACE(API,
		            "L2CAP - Calling Disconnect_Ind_Cb(), CID: 0x%04x "
		            " No Conf Needed",
		            p_ccb->local_cid);
		l2cu_release_ccb(p_ccb);
		(*disconnect_ind)(local_cid, FALSE);
		break;

	case L2CEVT_L2CAP_CONNECT_RSP:
		p_ccb->remote_cid = p_ci->remote_cid;
		p_ccb->chnl_state = CST_CONFIG;
		btu_start_timer(&p_ccb->timer_entry, BTU_TTYPE_L2CAP_CHNL,
		                L2CAP_CHNL_CFG_TOUT);
		L2CAP_TRACE(API,
		            "L2CAP - Calling Connect_Cfm_Cb(), CID: 0x%04x, Success",
		            p_ccb->local_cid);

		(*p_ccb->p_rcb->api.pL2CA_ConnectCfm_Cb)(p_ccb->local_cid,
		                                         L2CAP_CONN_OK);
		break;

	case L2CEVT_L2CAP_CONNECT_RSP_PND:
		btu_start_timer(&p_ccb->timer_entry, BTU_TTYPE_L2CAP_CHNL,
		                L2CAP_CHNL_CONNECT_TOUT_EXT);
		if (p_ccb->p_rcb->api.pL2CA_ConnectPnd_Cb)
		{
			L2CAP_TRACE(API, "L2CAP - Calling Connect_Pnd_Cb()");
			(*p_ccb->p_rcb->api.pL2CA_ConnectPnd_Cb)(0);
		}
		break;

	case L2CEVT_L2CAP_CONNECT_RSP_NEG:
		L2CAP_TRACE(
			API,
			"L2CAP - Calling Connect_Cfm_Cb(), CID: 0x%04x, Failure Code: %d",
			p_ccb->local_cid, p_ci->l2cap_result);
		l2cu_release_ccb(p_ccb);
		(*connect_cfm)(local_cid, p_ci->l2cap_result);
		break;

	case L2CEVT_TIMEOUT:
		L2CAP_TRACE(API,
		            "L2CAP - Calling Connect_Cfm_Cb(), CID: 0x%04x, Timeout",
		            p_ccb->local_cid);
		l2cu_release_ccb(p_ccb);
		(*connect_cfm)(local_cid, L2CAP_CONN_TIMEOUT);
		break;

	case L2CEVT_L2CA_DISCONNECT_REQ:
		l2cu_release_ccb(p_ccb);

		break;

	case L2CEVT_L2CA_DATA_WRITE:
	case L2CEVT_L2CAP_DATA:
		GKI_freebuf(p_data);
		break;
	}
}

static void l2c_csm_w4_l2ca_connect_rsp(tL2C_CCB *p_ccb, UINT16 event,
                                        void *p_data)
{
	tL2C_CONN_INFO *p_ci;
	tL2CA_DISCONNECT_IND_CB *disconnect_ind =
		p_ccb->p_rcb->api.pL2CA_DisconnectInd_Cb;
	UINT16 local_cid = p_ccb->local_cid;

	L2CAP_TRACE(EVENT, "L2CAP - st: W4_L2CA_CON_RSP evt: %d", event);

	switch (event)
	{
	case L2CEVT_LP_DISCONNECT_IND:
		L2CAP_TRACE(
			API,
			"L2CAP - Calling Disconnect_Ind_Cb(), CID: 0x%04x  No Conf Needed",
			p_ccb->local_cid);
		l2cu_release_ccb(p_ccb);
		(*disconnect_ind)(local_cid, FALSE);
		break;

	case L2CEVT_L2CA_CONNECT_RSP:
		p_ci = p_data;

		if (!p_ci || p_ci->l2cap_result == L2CAP_CONN_OK)
		{
			l2cu_send_peer_connect_rsp(p_ccb, L2CAP_CONN_OK, 0);
			p_ccb->chnl_state = CST_CONFIG;
			btu_start_timer(&p_ccb->timer_entry, BTU_TTYPE_L2CAP_CHNL,
			                L2CAP_CHNL_CFG_TOUT);
		}
		else
		{
			l2cu_send_peer_connect_rsp(p_ccb, p_ci->l2cap_result,
			                           p_ci->l2cap_status);
			btu_start_timer(&p_ccb->timer_entry, BTU_TTYPE_L2CAP_CHNL,
			                L2CAP_CHNL_CONNECT_TOUT_EXT);
		}
		break;

	case L2CEVT_L2CA_CONNECT_RSP_NEG:
		p_ci = p_data;
		l2cu_send_peer_connect_rsp(p_ccb, p_ci->l2cap_result,
		                           p_ci->l2cap_status);
		l2cu_release_ccb(p_ccb);
		break;

	case L2CEVT_TIMEOUT:
		l2cu_send_peer_connect_rsp(p_ccb, L2CAP_CONN_NO_PSM, 0);
		L2CAP_TRACE(
			API,
			"L2CAP - Calling Disconnect_Ind_Cb(), CID: 0x%04x  No Conf Needed",
			p_ccb->local_cid);
		l2cu_release_ccb(p_ccb);
		(*disconnect_ind)(local_cid, FALSE);
		break;

	case L2CEVT_L2CA_DATA_WRITE:
	case L2CEVT_L2CAP_DATA:
		GKI_freebuf(p_data);
		break;

	case L2CEVT_L2CA_DISCONNECT_REQ:
		l2cu_send_peer_disc_req(p_ccb);
		p_ccb->chnl_state = CST_W4_L2CAP_DISCONNECT_RSP;
		btu_start_timer(&p_ccb->timer_entry, BTU_TTYPE_L2CAP_CHNL,
		                L2CAP_CHNL_DISCONNECT_TOUT);
		break;
	}
}

static void l2c_csm_config(tL2C_CCB *p_ccb, UINT16 event, void *p_data)
{
	tL2CAP_CFG_INFO *p_cfg = p_data;
	tL2CA_DISCONNECT_IND_CB *disconnect_ind =
		p_ccb->p_rcb->api.pL2CA_DisconnectInd_Cb;
	UINT16 local_cid = p_ccb->local_cid;

	L2CAP_TRACE(EVENT, "L2CAP - st: CONFIG evt: %d", event);

	switch (event)
	{
	case L2CEVT_LP_DISCONNECT_IND:
		L2CAP_TRACE(
			API,
			"L2CAP - Calling Disconnect_Ind_Cb(), CID: 0x%04x  No Conf Needed",
			p_ccb->local_cid);
		l2cu_release_ccb(p_ccb);
		(*disconnect_ind)(local_cid, FALSE);
		break;

	case L2CEVT_L2CAP_CONFIG_REQ:
		if (l2cu_process_peer_cfg_req(p_ccb, p_cfg) != 0)
		{
			L2CAP_TRACE(API, "L2CAP - Calling Config_Req_Cb(), CID: 0x%04x",
			            p_ccb->local_cid);
			(*p_ccb->p_rcb->api.pL2CA_ConfigInd_Cb)(p_ccb->local_cid, p_cfg);
		}
		else
		{
			l2cu_send_peer_config_rsp(p_ccb, p_cfg);
		}
		break;

	case L2CEVT_L2CAP_CONFIG_RSP:
		l2cu_process_peer_cfg_rsp(p_ccb, p_cfg);

		p_ccb->config_done |= OB_CFG_DONE;

		if (p_ccb->config_done & IB_CFG_DONE)
		{
			p_ccb->chnl_state = CST_OPEN;

			btu_stop_timer(&p_ccb->timer_entry);
			forward_peer_data(p_ccb, 0);
		}

		L2CAP_TRACE(API, "L2CAP - Calling Config_Rsp_Cb(), CID: 0x%04x",
		            p_ccb->local_cid);
		(*p_ccb->p_rcb->api.pL2CA_ConfigCfm_Cb)(p_ccb->local_cid, p_cfg);
		break;

	case L2CEVT_L2CAP_CONFIG_RSP_NEG:
		btu_stop_timer(&p_ccb->timer_entry);

		L2CAP_TRACE(
			API,
			"L2CAP - Calling Config_Rsp_Cb(), CID: 0x%04x, Failure: %d",
			p_ccb->local_cid, p_cfg->result);
		(*p_ccb->p_rcb->api.pL2CA_ConfigCfm_Cb)(p_ccb->local_cid, p_cfg);

		break;

	case L2CEVT_L2CAP_DISCONNECT_REQ:
		btu_start_timer(&p_ccb->timer_entry, BTU_TTYPE_L2CAP_CHNL,
		                L2CAP_CHNL_DISCONNECT_TOUT);
		p_ccb->chnl_state = CST_W4_L2CA_DISCONNECT_RSP;
		L2CAP_TRACE(
			API,
			"L2CAP - Calling Disconnect_Ind_Cb(), CID: 0x%04x  Conf Needed",
			p_ccb->local_cid);
		(*p_ccb->p_rcb->api.pL2CA_DisconnectInd_Cb)(p_ccb->local_cid, TRUE);
		break;

	case L2CEVT_L2CA_CONFIG_REQ:
		l2cu_process_our_cfg_req(p_ccb, p_cfg);
		l2cu_send_peer_config_req(p_ccb, p_cfg);
		btu_start_timer(&p_ccb->timer_entry, BTU_TTYPE_L2CAP_CHNL,
		                L2CAP_CHNL_CFG_TOUT);
		break;

	case L2CEVT_L2CA_CONFIG_RSP:
		l2cu_process_our_cfg_rsp(p_ccb, p_cfg);

		p_ccb->config_done |= IB_CFG_DONE;

		if (p_ccb->config_done & OB_CFG_DONE)
		{
			p_ccb->chnl_state = CST_OPEN;
			btu_stop_timer(&p_ccb->timer_entry);
			forward_peer_data(p_ccb, 0);
		}

		l2cu_send_peer_config_rsp(p_ccb, p_cfg);

		break;

	case L2CEVT_L2CA_CONFIG_RSP_NEG:
		l2cu_send_peer_config_rsp(p_ccb, p_cfg);
		btu_start_timer(&p_ccb->timer_entry, BTU_TTYPE_L2CAP_CHNL,
		                L2CAP_CHNL_CFG_TOUT);
		break;

	case L2CEVT_L2CA_DISCONNECT_REQ:
		l2cu_send_peer_disc_req(p_ccb);
		p_ccb->chnl_state = CST_W4_L2CAP_DISCONNECT_RSP;
		btu_start_timer(&p_ccb->timer_entry, BTU_TTYPE_L2CAP_CHNL,
		                L2CAP_CHNL_DISCONNECT_TOUT);
		break;

	case L2CEVT_L2CAP_DATA:
		L2CAP_TRACE(API, "L2CAP - Calling DataInd_Cb(), CID: 0x%04x",
		            p_ccb->local_cid);

		(*p_ccb->p_rcb->api.pL2CA_DataInd_Cb)(p_ccb->local_cid, p_data);
		break;

	case L2CEVT_L2CA_DATA_WRITE:
		if (p_ccb->config_done & OB_CFG_DONE)
			GKI_enqueue(&p_ccb->xmit_hold_q, p_data);
		else
			GKI_freebuf(p_data);

		break;

	case L2CEVT_TIMEOUT:
		l2cu_send_peer_disc_req(p_ccb);
		L2CAP_TRACE(
			API,
			"L2CAP - Calling Disconnect_Ind_Cb(), CID: 0x%04x  No Conf Needed",
			p_ccb->local_cid);
		l2cu_release_ccb(p_ccb);
		(*disconnect_ind)(local_cid, FALSE);
		break;
	}
}

static void l2c_csm_open(tL2C_CCB *p_ccb, UINT16 event, void *p_data)
{
	tL2CA_DISCONNECT_IND_CB *disconnect_ind =
		p_ccb->p_rcb->api.pL2CA_DisconnectInd_Cb;
	UINT16 local_cid = p_ccb->local_cid;
	tL2CAP_CFG_INFO *p_cfg;
	tL2C_CHNL_STATE tempstate;
	UINT8 tempcfgdone;

	L2CAP_TRACE(EVENT, "L2CAP - st: OPEN evt: %d", event);

	switch (event)
	{
	case L2CEVT_LP_DISCONNECT_IND:
		L2CAP_TRACE(
			API,
			"L2CAP - Calling Disconnect_Ind_Cb(), CID: 0x%04x  No Conf Needed",
			p_ccb->local_cid);

		l2cu_release_ccb(p_ccb);
		(*disconnect_ind)(local_cid, FALSE);
		break;

	case L2CEVT_LP_QOS_VIOLATION_IND:
		if (p_ccb->p_rcb->api.pL2CA_QoSViolationInd_Cb)
		{
			(*p_ccb->p_rcb->api.pL2CA_QoSViolationInd_Cb)(
				p_ccb->p_lcb->remote_bd_addr);
		}

		break;

	case L2CEVT_L2CAP_CONFIG_REQ:
		p_cfg = p_data;
		tempstate = p_ccb->chnl_state;
		tempcfgdone = p_ccb->config_done;
		p_ccb->chnl_state = CST_CONFIG;
		p_ccb->config_done = 0;

		btu_start_timer(&p_ccb->timer_entry, BTU_TTYPE_L2CAP_CHNL,
		                L2CAP_CHNL_CFG_TOUT);

		if (l2cu_process_peer_cfg_req(p_ccb, p_cfg))
		{
			(*p_ccb->p_rcb->api.pL2CA_ConfigInd_Cb)(p_ccb->local_cid, p_cfg);
		}
		else
		{
			btu_stop_timer(&p_ccb->timer_entry);
			p_ccb->chnl_state = tempstate;
			p_ccb->config_done = tempcfgdone;
			l2cu_send_peer_config_rsp(p_ccb, p_cfg);
		}

		break;

	case L2CEVT_L2CAP_DISCONNECT_REQ:
		p_ccb->chnl_state = CST_W4_L2CA_DISCONNECT_RSP;
		btu_start_timer(&p_ccb->timer_entry, BTU_TTYPE_L2CAP_CHNL,
		                L2CAP_CHNL_DISCONNECT_TOUT);

		L2CAP_TRACE(
			API,
			"L2CAP - Calling Disconnect_Ind_Cb(), CID: 0x%04x  Conf Needed",
			p_ccb->local_cid);
		(*p_ccb->p_rcb->api.pL2CA_DisconnectInd_Cb)(p_ccb->local_cid, TRUE);
		break;

	case L2CEVT_L2CAP_DATA:
		(*p_ccb->p_rcb->api.pL2CA_DataInd_Cb)(p_ccb->local_cid,
		                                      p_data);
		break;

	case L2CEVT_L2CA_DISCONNECT_REQ:
		l2cu_send_peer_disc_req(p_ccb);
		p_ccb->chnl_state = CST_W4_L2CAP_DISCONNECT_RSP;
		btu_start_timer(&p_ccb->timer_entry, BTU_TTYPE_L2CAP_CHNL,
		                L2CAP_CHNL_DISCONNECT_TOUT);
		break;

	case L2CEVT_L2CA_DATA_WRITE:
		((BT_HDR *)p_data)->event = p_ccb->local_cid;
		forward_peer_data(p_ccb, p_data);
		break;

	case L2CEVT_L2CA_CONFIG_REQ:
		l2cu_process_our_cfg_req(p_ccb, p_data);
		l2cu_send_peer_config_req(p_ccb, p_data);

		p_ccb->chnl_state = CST_CONFIG;
		p_ccb->config_done = 0;

		btu_start_timer(&p_ccb->timer_entry, BTU_TTYPE_L2CAP_CHNL,
		                L2CAP_CHNL_CFG_TOUT);
		break;
	}
}

static void l2c_csm_w4_l2cap_disconnect_rsp(tL2C_CCB *p_ccb, UINT16 event,
                                            void *p_data)
{
	tL2CA_DISCONNECT_CFM_CB *disconnect_cfm =
		p_ccb->p_rcb->api.pL2CA_DisconnectCfm_Cb;
	tL2CA_DISCONNECT_IND_CB *disconnect_ind =
		p_ccb->p_rcb->api.pL2CA_DisconnectInd_Cb;
	UINT16 local_cid = p_ccb->local_cid;

	L2CAP_TRACE(EVENT, "L2CAP - st: W4_L2CAP_DISC_RSP evt: %d", event);

	switch (event)
	{
	case L2CEVT_LP_DISCONNECT_IND:
		L2CAP_TRACE(
			API,
			"L2CAP - Calling Disconnect_Ind_Cb(), CID: 0x%04x  No Conf Needed",
			p_ccb->local_cid);
		l2cu_release_ccb(p_ccb);
		(*disconnect_ind)(local_cid, FALSE);
		break;

	case L2CEVT_L2CAP_DISCONNECT_RSP:
		l2cu_release_ccb(p_ccb);

		if (disconnect_cfm)
		{
			L2CAP_TRACE(API, "L2CAP - Calling DisconnectCfm_Cb(), CID: 0x%04x",
			            local_cid);
			(*disconnect_cfm)(local_cid, L2CAP_DISC_OK);
		}

		break;

	case L2CEVT_L2CAP_DISCONNECT_REQ:
		l2cu_send_peer_disc_rsp(p_ccb->p_lcb, p_ccb->remote_id,
		                        p_ccb->local_cid, p_ccb->remote_cid);
		l2cu_release_ccb(p_ccb);

		if (disconnect_cfm)
		{
			L2CAP_TRACE(API, "L2CAP - Calling DisconnectCfm_Cb(), CID: 0x%04x",
			            local_cid);
			(*disconnect_cfm)(local_cid, L2CAP_DISC_OK);
		}

		break;

	case L2CEVT_TIMEOUT:
		l2cu_release_ccb(p_ccb);

		if (disconnect_cfm)
		{
			L2CAP_TRACE(API, "L2CAP - Calling DisconnectCfm_Cb(), CID: 0x%04x",
			            local_cid);
			(*disconnect_cfm)(local_cid, L2CAP_DISC_TIMEOUT);
		}

		break;

	case L2CEVT_L2CAP_DATA:
	case L2CEVT_L2CA_DATA_WRITE:
		GKI_freebuf(p_data);
		break;
	}
}

static void l2c_csm_w4_l2ca_disconnect_rsp(tL2C_CCB *p_ccb, UINT16 event,
                                           void *p_data)
{
	tL2CA_DISCONNECT_IND_CB *disconnect_ind =
		p_ccb->p_rcb->api.pL2CA_DisconnectInd_Cb;
	UINT16 local_cid = p_ccb->local_cid;

	L2CAP_TRACE(EVENT, "L2CAP - st: W4_L2CA_DISC_RSP evt: %d", event);

	switch (event)
	{
	case L2CEVT_LP_DISCONNECT_IND:
		L2CAP_TRACE(
			API,
			"L2CAP - Calling Disconnect_Ind_Cb(), CID: 0x%04x  No Conf Needed",
			p_ccb->local_cid);
		l2cu_release_ccb(p_ccb);
		(*disconnect_ind)(local_cid, FALSE);
		break;

	case L2CEVT_TIMEOUT:
		l2cu_send_peer_disc_rsp(p_ccb->p_lcb, p_ccb->remote_id,
		                        p_ccb->local_cid, p_ccb->remote_cid);
		L2CAP_TRACE(
			API,
			"L2CAP - Calling Disconnect_Ind_Cb(), CID: 0x%04x  No Conf Needed",
			p_ccb->local_cid);
		l2cu_release_ccb(p_ccb);
		(*disconnect_ind)(local_cid, FALSE);
		break;

	case L2CEVT_L2CA_DISCONNECT_RSP:
		l2cu_send_peer_disc_rsp(p_ccb->p_lcb, p_ccb->remote_id,
		                        p_ccb->local_cid, p_ccb->remote_cid);
		l2cu_release_ccb(p_ccb);
		break;

	case L2CEVT_L2CAP_DATA:
	case L2CEVT_L2CA_DATA_WRITE:
		GKI_freebuf(p_data);
		break;
	}
}

void forward_peer_data(tL2C_CCB *p_ccb, BT_HDR *p_buf)
{
	UINT8 *p;

	if (p_buf && p_ccb->xmit_hold_q.count)
	{
		GKI_enqueue(&p_ccb->xmit_hold_q, p_buf);
		p_buf = GKI_dequeue(&p_ccb->xmit_hold_q);
	}
	else if (!p_buf)
	{
		p_buf = GKI_dequeue(&p_ccb->xmit_hold_q);
	}

	while (p_buf)
	{
		if (p_buf->offset < 9)
		{
			L2CAP_TRACE(ERROR, "L2CAP - cannot send buffer, offset: %d",
			            p_buf->offset);

			GKI_freebuf(p_buf);

			p_buf = GKI_dequeue(&p_ccb->xmit_hold_q);
		}
		else
		{
			p_buf->offset -= 8;
			p_buf->len += 4;

			p = (UINT8 *)(p_buf + 1) + p_buf->offset;

			UINT16_TO_STREAM(p, 0x2000 | p_ccb->p_lcb->handle);

			if (p_buf->len > btu_cb.hcit_acl_data_size)
				UINT16_TO_STREAM(p, btu_cb.hcit_acl_data_size);
			else
				UINT16_TO_STREAM(p, p_buf->len);

			UINT16_TO_STREAM(p, p_buf->len - 4);
			UINT16_TO_STREAM(p, p_ccb->remote_cid);

			p_buf->len += 4;
			l2c_link_check_send_pkts(p_ccb->p_lcb, NULL, p_buf);

			p_buf = GKI_dequeue(&p_ccb->xmit_hold_q);
		}

	}
}
