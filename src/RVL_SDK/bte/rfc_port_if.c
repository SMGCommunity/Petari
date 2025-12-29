#include "bt_target.h"
#include "bt_types.h"
#include "port_api.h"
#include "rfc_int.h"

/* Original source:
 * bluedroid <android.googlesource.com/platform/external/bluetooth/bluedroid>
 * stack/rfcomm/rfc_port_if.c
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

#include "rfcdefs.h"
#include <stddef.h>

#include "data_types.h"

#include "port_int.h"

/*******************************************************************************
 * variables
 */

// .bss
tRFC_CB rfc_cb;

/*******************************************************************************
 * functions
 */

void RFCOMM_StartReq(tRFC_MCB *p_mcb)
{
	rfc_mx_sm_execute(p_mcb, RFC_MX_EVENT_START_REQ, NULL);
}

void RFCOMM_StartRsp(tRFC_MCB *p_mcb, UINT16 result)
{
	rfc_mx_sm_execute(p_mcb, RFC_MX_EVENT_START_RSP, &result);
}

void RFCOMM_DlcEstablishReq(tRFC_MCB *p_mcb, UINT8 dlci, UINT16 mtu)
{
	tPORT *p_port = port_find_mcb_dlci_port(p_mcb, dlci);

	if (p_mcb->state != RFC_MX_STATE_CONNECTED)
	{
		PORT_DlcEstablishCnf(p_mcb, dlci, 0, RFCOMM_ERROR);
		return;
	}

	rfc_port_sm_execute(p_port, RFC_EVENT_OPEN, NULL);
}

void RFCOMM_DlcEstablishRsp(tRFC_MCB *p_mcb, UINT8 dlci, UINT16 mtu,
                            UINT16 result)
{
	tPORT *p_port = port_find_mcb_dlci_port(p_mcb, dlci);

	if (p_mcb->state != RFC_MX_STATE_CONNECTED && result == RFCOMM_SUCCESS)
	{
		PORT_DlcReleaseInd(p_mcb, dlci);
		return;
	}

	rfc_port_sm_execute(p_port, RFC_EVENT_ESTABLISH_RSP, &result);
}

void RFCOMM_ParNegReq(tRFC_MCB *p_mcb, UINT8 dlci, UINT16 mtu)
{
	tPORT *p_port = port_find_mcb_dlci_port(p_mcb, dlci);
	UINT8 flow;
	UINT8 cl;
	UINT8 k;

	if (p_mcb->state != RFC_MX_STATE_CONNECTED)
	{
		p_port->error = PORT_PAR_NEG_FAILED;
		return;
	}

	flow = p_mcb->flow == PORT_FC_UNDEFINED ? PORT_FC_DEFAULT : p_mcb->flow;

	if (flow == PORT_FC_CREDIT)
	{
		cl = RFCOMM_PN_CONV_LAYER_CBFC_I;
		k = p_port->credit_rx_max < RFCOMM_K_MAX ? p_port->credit_rx_max
		                                         : RFCOMM_K_MAX;
		p_port->credit_rx = k;
	}
	else
	{
		cl = RFCOMM_PN_CONV_LAYER_TYPE_1;
		k = 0;
	}

	p_port->rfc.expected_rsp |= RFC_RSP_PN;

	rfc_send_pn(p_mcb, dlci, TRUE, mtu, cl, k);

	rfc_port_timer_start(p_port, RFC_T2_TIMEOUT);
}

void RFCOMM_ParNegRsp(tRFC_MCB *p_mcb, UINT8 dlci, UINT16 mtu, UINT8 cl,
                      UINT8 k)
{
	if (p_mcb->state != RFC_MX_STATE_CONNECTED)
		return;

	rfc_send_pn(p_mcb, dlci, FALSE, mtu, cl, k);
}

void RFCOMM_PortNegReq(tRFC_MCB *p_mcb, UINT8 dlci, tPORT_STATE *p_pars)
{
	tPORT *p_port = port_find_mcb_dlci_port(p_mcb, dlci);

	if (p_mcb->state != RFC_MX_STATE_CONNECTED)
	{
		PORT_PortNegCnf(p_mcb, dlci, NULL, RFCOMM_ERROR);
		return;
	}

	if (!p_pars)
		p_port->rfc.expected_rsp |= RFC_RSP_RPN_REPLY;
	else
		p_port->rfc.expected_rsp |= RFC_RSP_RPN;

	rfc_send_rpn(p_mcb, dlci, TRUE, p_pars, RFCOMM_RPN_PM_MASK);
	rfc_port_timer_start(p_port, RFC_T2_TIMEOUT);
}

void RFCOMM_PortNegRsp(tRFC_MCB *p_mcb, UINT8 dlci, tPORT_STATE *p_pars,
                       UINT16 param_mask)
{
	if (p_mcb->state != RFC_MX_STATE_CONNECTED)
		return;

	rfc_send_rpn(p_mcb, dlci, FALSE, p_pars, param_mask);
}

void RFCOMM_ControlReq(tRFC_MCB *p_mcb, UINT8 dlci, tPORT_CTRL *p_pars)
{
	tPORT *p_port = port_find_mcb_dlci_port(p_mcb, dlci);

	if (p_port->state != PORT_STATE_OPENED
	    || p_port->rfc.state != RFC_STATE_OPENED)
	{
		return;
	}

	p_port->port_ctrl |= PORT_CTRL_REQ_SENT;

	p_port->rfc.expected_rsp |= RFC_RSP_MSC;

	rfc_send_msc(p_mcb, dlci, TRUE, p_pars);
	rfc_port_timer_start(p_port, RFC_T2_TIMEOUT);
}

void RFCOMM_FlowReq(tRFC_MCB *p_mcb, UINT8 dlci, UINT8 enable)
{
	tPORT *p_port = port_find_mcb_dlci_port(p_mcb, dlci);

	if (p_port->state != PORT_STATE_OPENED
	    || p_port->rfc.state != RFC_STATE_OPENED)
	{
		return;
	}

	p_port->local_ctrl.fc = !enable;

	p_port->rfc.expected_rsp |= RFC_RSP_MSC;

	rfc_send_msc(p_mcb, dlci, TRUE, &p_port->local_ctrl);
	rfc_port_timer_start(p_port, RFC_T2_TIMEOUT);
}

void RFCOMM_LineStatusReq(tRFC_MCB *p_mcb, UINT8 dlci, UINT8 status)
{
	tPORT *p_port = port_find_mcb_dlci_port(p_mcb, dlci);

	if (p_port->state != PORT_STATE_OPENED
	    || p_port->rfc.state != RFC_STATE_OPENED)
	{
		return;
	}

	p_port->rfc.expected_rsp |= RFC_RSP_RLS;

	rfc_send_rls(p_mcb, dlci, TRUE, status);
	rfc_port_timer_start(p_port, RFC_T2_TIMEOUT);
}

void RFCOMM_DlcReleaseReq(tRFC_MCB *p_mcb, UINT8 dlci)
{
	rfc_port_sm_execute(port_find_mcb_dlci_port(p_mcb, dlci), RFC_EVENT_CLOSE,
	                    NULL);
}

void RFCOMM_DataReq(tRFC_MCB *p_mcb, UINT8 dlci, BT_HDR *p_buf)
{
	rfc_port_sm_execute(port_find_mcb_dlci_port(p_mcb, dlci), RFC_EVENT_DATA,
	                    p_buf);
}
