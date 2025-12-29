#include "rfc_int.h"

/* Original source:
 * bluedroid <android.googlesource.com/platform/external/bluetooth/bluedroid>
 * stack/rfcomm/rfc_l2cap_if.c
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

#include "bt_target.h"
#include "bt_trace.h"
#include "bt_types.h"
#include "data_types.h"

#include "gki.h"
#include "l2cdefs.h"
#include "rfcdefs.h"
#include "l2c_api.h"
#include "port_int.h"

/*******************************************************************************
 * local function declarations
 */

static void RFCOMM_ConnectInd(BD_ADDR bd_addr, UINT16 lcid, UINT16 psm,
                              UINT8 id);
static void RFCOMM_ConnectCnf(UINT16 lcid, UINT16 result);
static void RFCOMM_ConfigInd(UINT16 lcid, tL2CAP_CFG_INFO *p_cfg);
static void RFCOMM_ConfigCnf(UINT16 lcid, tL2CAP_CFG_INFO *p_cfg);
static void RFCOMM_QoSViolationInd(BD_ADDR bd_addr);
static void RFCOMM_DisconnectInd(UINT16 lcid, BOOLEAN is_conf_needed);
static void RFCOMM_BufDataInd(UINT16 lcid, BT_HDR *p_buf);
static void RFCOMM_CongestionStatusInd(UINT16 lcid, BOOLEAN is_congested);

/*******************************************************************************
 * functions
 */

void rfcomm_l2cap_if_init(void)
{
	tL2CAP_APPL_INFO *p_l2c = &rfc_cb.rfc.reg_info;

	p_l2c->pL2CA_ConnectInd_Cb			= &RFCOMM_ConnectInd;
	p_l2c->pL2CA_ConnectCfm_Cb			= &RFCOMM_ConnectCnf;
	p_l2c->pL2CA_ConnectPnd_Cb			= NULL;
	p_l2c->pL2CA_ConfigInd_Cb			= &RFCOMM_ConfigInd;
	p_l2c->pL2CA_ConfigCfm_Cb			= &RFCOMM_ConfigCnf;
	p_l2c->pL2CA_DisconnectInd_Cb		= &RFCOMM_DisconnectInd;
	p_l2c->pL2CA_DisconnectCfm_Cb		= NULL;
	p_l2c->pL2CA_QoSViolationInd_Cb		= &RFCOMM_QoSViolationInd;
	p_l2c->pL2CA_DataInd_Cb				= &RFCOMM_BufDataInd;
	p_l2c->pL2CA_CongestionStatus_Cb	= &RFCOMM_CongestionStatusInd;

	L2CA_Register(BT_PSM_RFCOMM, p_l2c);
}

static void RFCOMM_ConnectInd(BD_ADDR bd_addr, UINT16 lcid, UINT16 psm,
                              UINT8 id)
{
	tRFC_MCB *p_mcb = rfc_alloc_multiplexer_channel(bd_addr, FALSE);

	rfc_save_lcid_mcb(p_mcb, lcid);

	if (!p_mcb)
	{
		L2CA_ConnectRsp(bd_addr, id, lcid, L2CAP_CONN_NO_RESOURCES, 0);
		return;
	}

	p_mcb->lcid = lcid;

	rfc_mx_sm_execute(p_mcb, RFC_MX_EVENT_CONN_IND, &id);
}

static void RFCOMM_ConnectCnf(UINT16 lcid, UINT16 result)
{
	tRFC_MCB *p_mcb = rfc_find_lcid_mcb(lcid);

	if (!p_mcb)
	{
		RFCOMM_TRACE(ERROR, "RFCOMM_ConnectCnf LCID:0x%x", lcid);
		return;
	}

	p_mcb->lcid = lcid;

	rfc_mx_sm_execute(p_mcb, RFC_MX_EVENT_CONN_CNF, &result);
}

static void RFCOMM_ConfigInd(UINT16 lcid, tL2CAP_CFG_INFO *p_cfg)
{
	tRFC_MCB *p_mcb = rfc_find_lcid_mcb(lcid);

	if (!p_mcb)
	{
		RFCOMM_TRACE(ERROR, "RFCOMM_ConfigInd LCID:0x%x", lcid);
		return;
	}

	rfc_mx_sm_execute(p_mcb, RFC_MX_EVENT_CONF_IND, p_cfg);
}

static void RFCOMM_ConfigCnf(UINT16 lcid, tL2CAP_CFG_INFO *p_cfg)
{
	tRFC_MCB *p_mcb = rfc_find_lcid_mcb(lcid);

	if (!p_mcb)
	{
		RFCOMM_TRACE(ERROR, "RFCOMM_ConfigCnf LCID:0x%x", lcid);
		return;
	}

	rfc_mx_sm_execute(p_mcb, RFC_MX_EVENT_CONF_CNF, p_cfg);
}

static void RFCOMM_QoSViolationInd(BD_ADDR bd_addr)
{
	/* ... */
}

static void RFCOMM_DisconnectInd(UINT16 lcid, BOOLEAN is_conf_needed)
{
	tRFC_MCB *p_mcb = rfc_find_lcid_mcb(lcid);

	if (is_conf_needed)
		L2CA_DisconnectRsp(lcid);

	if (!p_mcb)
	{
		RFCOMM_TRACE(WARNING, "RFCOMM_DisconnectInd LCID:0x%x", lcid);
		return;
	}

	rfc_mx_sm_execute(p_mcb, RFC_MX_EVENT_DISC_IND, NULL);
}

static void RFCOMM_BufDataInd(UINT16 lcid, BT_HDR *p_buf)
{
	tRFC_MCB *p_mcb = rfc_find_lcid_mcb(lcid);
	tPORT *p_port;
	UINT8 event;

	if (!p_mcb)
	{
		RFCOMM_TRACE(WARNING, "RFCOMM_BufDataInd LCID:0x%x", lcid);
		GKI_freebuf(p_buf);
		return;
	}

	event = rfc_parse_data(p_mcb, &rfc_cb.rfc.rx_frame, p_buf);

	if (event == RFC_EVENT_BAD_FRAME)
	{
		GKI_freebuf(p_buf);
		return;
	}

	if (rfc_cb.rfc.rx_frame.dlci == RFCOMM_MX_DLCI)
	{
		if (event == RFC_EVENT_UIH)
		{
			rfc_process_mx_message(p_mcb, p_buf);
			return;
		}

		rfc_mx_sm_execute(p_mcb, event, NULL);

		GKI_freebuf(p_buf);
		return;
	}

	if (((p_port = port_find_mcb_dlci_port(p_mcb, rfc_cb.rfc.rx_frame.dlci))
	     == NULL)
	    || !p_port->rfc.p_mcb)
	{
		if (event != RFC_EVENT_SABME)
		{
			if ((p_mcb->is_initiator && !rfc_cb.rfc.rx_frame.cr)
			    || (!p_mcb->is_initiator && rfc_cb.rfc.rx_frame.cr))
			{
				rfc_send_dm(p_mcb, rfc_cb.rfc.rx_frame.dlci,
				            rfc_cb.rfc.rx_frame.pf);
			}

			GKI_freebuf(p_buf);
			return;
		}

		if ((p_port = port_find_dlci_port(rfc_cb.rfc.rx_frame.dlci)) == NULL)
		{
			rfc_send_dm(p_mcb, rfc_cb.rfc.rx_frame.dlci, TRUE);
			GKI_freebuf(p_buf);
			return;
		}

		p_mcb->port_inx[rfc_cb.rfc.rx_frame.dlci] = p_port->inx;
		p_port->rfc.p_mcb = p_mcb;
	}

	if (event == RFC_EVENT_UIH)
	{
		if (p_buf->len > 0)
			rfc_port_sm_execute(p_port, event, p_buf);
		else
			GKI_freebuf(p_buf);

		if (rfc_cb.rfc.rx_frame.credit != 0)
			rfc_inc_credit(p_port, rfc_cb.rfc.rx_frame.credit);

		return;
	}

	rfc_port_sm_execute(p_port, event, NULL);
	GKI_freebuf(p_buf);
}

static void RFCOMM_CongestionStatusInd(UINT16 lcid, BOOLEAN is_congested)
{
	tRFC_MCB *p_mcb = rfc_find_lcid_mcb(lcid);

	if (!p_mcb)
	{
		RFCOMM_TRACE(ERROR, "RFCOMM_CongestionStatusInd dropped LCID:0x%x",
		             lcid);
		return;
	}
	else
	{
		RFCOMM_TRACE(EVENT, "RFCOMM_CongestionStatusInd LCID:0x%x", lcid);
	}

	rfc_process_l2cap_congestion(p_mcb, is_congested);
}

tRFC_MCB *rfc_find_lcid_mcb(UINT16 lcid)
{
	tRFC_MCB *p_mcb;

	if (lcid - L2CAP_BASE_APPL_CID >= MAX_L2CAP_CHANNELS)
	{
		RFCOMM_TRACE(ERROR, "rfc_find_lcid_mcb LCID:0x%x", lcid);
		return NULL;
	}

	if ((p_mcb = rfc_cb.rfc.p_rfc_lcid_mcb[lcid - L2CAP_BASE_APPL_CID]) != NULL)
	{
		if (p_mcb->lcid != lcid)
		{
			RFCOMM_TRACE(WARNING,
			             "rfc_find_lcid_mcb LCID reused LCID:0x%x current:0x%x",
			             lcid, p_mcb->lcid);
			return NULL;
		}
	}

	return p_mcb;
}

void rfc_save_lcid_mcb(tRFC_MCB *p_mcb, UINT16 lcid)
{
	rfc_cb.rfc.p_rfc_lcid_mcb[lcid - L2CAP_BASE_APPL_CID] = p_mcb;
}
