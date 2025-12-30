#include "rfc_int.h"

/* Original source:
 * bluedroid <android.googlesource.com/platform/external/bluetooth/bluedroid>
 * stack/rfcomm/rfc_ts_frames.c
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
#include "l2c_api.h"
#include "port_api.h"
#include "port_int.h"
#include "rfcdefs.h"

/*******************************************************************************
 * functions
 */

void rfc_send_sabme(tRFC_MCB *p_mcb, UINT8 dlci)
{
	BT_HDR *p_buf;
	UINT8 *p_data;
	UINT8 cr = RFCOMM_CR(p_mcb->is_initiator, TRUE);

	if ((p_buf = GKI_getpoolbuf(RFCOMM_CMD_POOL_ID)) == NULL)
		return;

	p_buf->offset = L2CAP_MIN_OFFSET;
	p_data = (UINT8 *)(p_buf + 1) + L2CAP_MIN_OFFSET;

	*p_data++ = RFCOMM_EA | cr | dlci << RFCOMM_SHIFT_DLCI;
	*p_data++ = RFCOMM_SABME | RFCOMM_PF;
	*p_data++ = RFCOMM_EA | 0;

	*p_data =
		RFCOMM_SABME_FCS((UINT8 *)(p_buf + 1) + L2CAP_MIN_OFFSET, cr, dlci);

	p_buf->len = 4;

	rfc_check_send_cmd(p_mcb, p_buf);
}

void rfc_send_ua(tRFC_MCB *p_mcb, UINT8 dlci)
{
	BT_HDR *p_buf;
	UINT8 *p_data;
	UINT8 cr = RFCOMM_CR(p_mcb->is_initiator, FALSE);

	if ((p_buf = GKI_getpoolbuf(RFCOMM_CMD_POOL_ID)) == NULL)
		return;

	p_buf->offset = L2CAP_MIN_OFFSET;
	p_data = (UINT8 *)(p_buf + 1) + L2CAP_MIN_OFFSET;

	*p_data++ = RFCOMM_EA | cr | dlci << RFCOMM_SHIFT_DLCI;
	*p_data++ = RFCOMM_UA | RFCOMM_PF;
	*p_data++ = RFCOMM_EA | 0;

	*p_data = RFCOMM_UA_FCS((UINT8 *)(p_buf + 1) + L2CAP_MIN_OFFSET, cr, dlci);

	p_buf->len = 4;

	rfc_check_send_cmd(p_mcb, p_buf);
}

void rfc_send_dm(tRFC_MCB *p_mcb, UINT8 dlci, BOOLEAN pf)
{
	BT_HDR *p_buf;
	UINT8 *p_data;
	UINT8 cr = RFCOMM_CR(p_mcb->is_initiator, FALSE);

	if ((p_buf = GKI_getpoolbuf(RFCOMM_CMD_POOL_ID)) == NULL)
		return;

	p_buf->offset = L2CAP_MIN_OFFSET;
	p_data = (UINT8 *)(p_buf + 1) + L2CAP_MIN_OFFSET;

	*p_data++ = RFCOMM_EA | cr | dlci << RFCOMM_SHIFT_DLCI;
	*p_data++ = RFCOMM_DM | (pf ? RFCOMM_PF : 0);
	*p_data++ = RFCOMM_EA | 0;

	*p_data = RFCOMM_DM_FCS((UINT8 *)(p_buf + 1) + L2CAP_MIN_OFFSET, cr, dlci);

	p_buf->len = 4;

	rfc_check_send_cmd(p_mcb, p_buf);
}

void rfc_send_disc(tRFC_MCB *p_mcb, UINT8 dlci)
{
	BT_HDR *p_buf;
	UINT8 *p_data;
	UINT8 cr = RFCOMM_CR(p_mcb->is_initiator, TRUE);

	if ((p_buf = GKI_getpoolbuf(RFCOMM_CMD_POOL_ID)) == NULL)
		return;

	p_buf->offset = L2CAP_MIN_OFFSET;
	p_data = (UINT8 *)(p_buf + 1) + L2CAP_MIN_OFFSET;

	*p_data++ = RFCOMM_EA | cr | dlci << RFCOMM_SHIFT_DLCI;
	*p_data++ = RFCOMM_DISC | RFCOMM_PF;
	*p_data++ = RFCOMM_EA | 0;

	*p_data =
		RFCOMM_DISC_FCS((UINT8 *)(p_buf + 1) + L2CAP_MIN_OFFSET, cr, dlci);

	p_buf->len = 4;

	rfc_check_send_cmd(p_mcb, p_buf);
}

void rfc_send_buf_uih(tRFC_MCB *p_mcb, UINT8 dlci, BT_HDR *p_buf)
{
	UINT8 *p_data;
	UINT8 cr = RFCOMM_CR(p_mcb->is_initiator, TRUE);
	UINT8 credits;

	p_buf->offset -= RFCOMM_CTRL_FRAME_LEN;

	if (p_buf->len > 127)
		--p_buf->offset;

	if (dlci)
		credits = p_buf->layer_specific;
	else
		credits = 0;

	if (credits)
		--p_buf->offset;

	p_data = (UINT8 *)(p_buf + 1) + p_buf->offset;

	*p_data++ = RFCOMM_EA | cr | dlci << RFCOMM_SHIFT_DLCI;
	*p_data++ = RFCOMM_UIH | (credits ? RFCOMM_PF : 0);

	if (p_buf->len <= 127)
	{
		*p_data++ = RFCOMM_EA | p_buf->len << 1;
		p_buf->len += 3;
	}
	else
	{
		*p_data++ = (p_buf->len & 0x7f) << 1;
		*p_data++ = p_buf->len >> RFCOMM_SHIFT_LENGTH2;
		p_buf->len += 4;
	}

	if (credits)
	{
		*p_data++ = credits;
		p_buf->len++;
	}

	p_data = (UINT8 *)(p_buf + 1) + p_buf->offset + p_buf->len++;

	*p_data = RFCOMM_UIH_FCS((UINT8 *)(p_buf + 1) + p_buf->offset, dlci);

	if (dlci == RFCOMM_MX_DLCI)
		rfc_check_send_cmd(p_mcb, p_buf);
	else
		L2CA_DataWrite(p_mcb->lcid, p_buf);
}

void rfc_send_pn(tRFC_MCB *p_mcb, UINT8 dlci, BOOLEAN is_command, UINT16 mtu,
                 UINT8 cl, UINT8 k)
{
	BT_HDR *p_buf;
	UINT8 *p_data;

	if ((p_buf = GKI_getpoolbuf(RFCOMM_CMD_POOL_ID)) == NULL)
		return;

	p_buf->offset = L2CAP_MIN_OFFSET + RFCOMM_CTRL_FRAME_LEN;
	p_data = (UINT8 *)(p_buf + 1) + p_buf->offset;

	*p_data++ = RFCOMM_EA | RFCOMM_I_CR(is_command) | RFCOMM_MX_PN;
	*p_data++ = RFCOMM_EA | RFCOMM_MX_PN_LEN << 1;

	*p_data++ = dlci;
	*p_data++ = RFCOMM_PN_FRAM_TYPE_UIH | cl;

	if (is_command)
		*p_data++ = RFCOMM_PN_PRIORITY_0;
	else
		*p_data++ = rfc_cb.rfc.rx_frame.u.pn.priority;

	*p_data++ = RFCOMM_T1_DSEC;
	*p_data++ = mtu & 0xff;
	*p_data++ = mtu >> 8;
	*p_data++ = RFCOMM_N2;
	*p_data = k;

	p_buf->len = RFCOMM_MX_PN_LEN + 2;

	rfc_send_buf_uih(p_mcb, RFCOMM_MX_DLCI, p_buf);
}

void rfc_send_fcon(tRFC_MCB *p_mcb, BOOLEAN is_command)
{
	BT_HDR *p_buf;
	UINT8 *p_data;

	if ((p_buf = GKI_getpoolbuf(RFCOMM_CMD_POOL_ID)) == NULL)
		return;

	p_buf->offset = L2CAP_MIN_OFFSET + RFCOMM_CTRL_FRAME_LEN;
	p_data = (UINT8 *)(p_buf + 1) + p_buf->offset;

	*p_data++ = RFCOMM_EA | RFCOMM_I_CR(is_command) | RFCOMM_MX_FCON;
	*p_data++ = RFCOMM_EA | RFCOMM_MX_FCON_LEN << 1;

	p_buf->len = RFCOMM_MX_FCON_LEN + 2;

	rfc_send_buf_uih(p_mcb, RFCOMM_MX_DLCI, p_buf);
}

void rfc_send_fcoff(tRFC_MCB *p_mcb, BOOLEAN is_command)
{
	BT_HDR *p_buf;
	UINT8 *p_data;

	if ((p_buf = GKI_getpoolbuf(RFCOMM_CMD_POOL_ID)) == NULL)
		return;

	p_buf->offset = L2CAP_MIN_OFFSET + RFCOMM_CTRL_FRAME_LEN;
	p_data = (UINT8 *)(p_buf + 1) + p_buf->offset;

	*p_data++ = RFCOMM_EA | RFCOMM_I_CR(is_command) | RFCOMM_MX_FCOFF;
	*p_data++ = RFCOMM_EA | RFCOMM_MX_FCOFF_LEN << 1;

	p_buf->len = RFCOMM_MX_FCOFF_LEN + 2;

	rfc_send_buf_uih(p_mcb, RFCOMM_MX_DLCI, p_buf);
}

void rfc_send_msc(tRFC_MCB *p_mcb, UINT8 dlci, BOOLEAN is_command,
                  tPORT_CTRL *p_pars)
{
	BT_HDR *p_buf;
	UINT8 *p_data;
	UINT8 signals = p_pars->modem_signal;
	UINT8 break_duration = p_pars->break_signal;
	UINT8 len;

	if ((p_buf = GKI_getpoolbuf(RFCOMM_CMD_POOL_ID)) == NULL)
		return;

	p_buf->offset = L2CAP_MIN_OFFSET + RFCOMM_CTRL_FRAME_LEN;
	p_data = (UINT8 *)(p_buf + 1) + p_buf->offset;

	if (break_duration)
		len = RFCOMM_MX_MSC_LEN_WITH_BREAK;
	else
		len = RFCOMM_MX_MSC_LEN_NO_BREAK;

	*p_data++ = RFCOMM_EA | RFCOMM_I_CR(is_command) | RFCOMM_MX_MSC;
	*p_data++ = RFCOMM_EA | len << 1;

	*p_data++ = RFCOMM_EA | RFCOMM_CR_MASK | (dlci << RFCOMM_SHIFT_DLCI);
	*p_data++ = RFCOMM_EA | (p_pars->fc ? RFCOMM_MSC_FC : 0)
	          | (signals & MODEM_SIGNAL_DTRDSR ? RFCOMM_MSC_RTC : 0)
	          | (signals & MODEM_SIGNAL_RTSCTS ? RFCOMM_MSC_RTR : 0)
	          | (signals & MODEM_SIGNAL_RI ? RFCOMM_MSC_IC : 0)
	          | (signals & MODEM_SIGNAL_DCD ? RFCOMM_MSC_DV : 0);

	if (break_duration)
	{
		*p_data++ = RFCOMM_EA | RFCOMM_MSC_BREAK_PRESENT_MASK
		          | break_duration << RFCOMM_MSC_SHIFT_BREAK;
	}

	p_buf->len = len + 2;

	rfc_send_buf_uih(p_mcb, RFCOMM_MX_DLCI, p_buf);
}

void rfc_send_rls(tRFC_MCB *p_mcb, UINT8 dlci, BOOLEAN is_command, UINT8 status)
{
	BT_HDR *p_buf;
	UINT8 *p_data;

	if ((p_buf = GKI_getpoolbuf(RFCOMM_CMD_POOL_ID)) == NULL)
		return;

	p_buf->offset = L2CAP_MIN_OFFSET + RFCOMM_CTRL_FRAME_LEN;
	p_data = (UINT8 *)(p_buf + 1) + p_buf->offset;

	*p_data++ = RFCOMM_EA | RFCOMM_I_CR(is_command) | RFCOMM_MX_RLS;
	*p_data++ = RFCOMM_EA | RFCOMM_MX_RLS_LEN << 1;

	*p_data++ = RFCOMM_EA | RFCOMM_CR_MASK | dlci << RFCOMM_SHIFT_DLCI;
	*p_data++ = RFCOMM_RLS_ERROR | status;

	p_buf->len = RFCOMM_MX_RLS_LEN + 2;

	rfc_send_buf_uih(p_mcb, RFCOMM_MX_DLCI, p_buf);
}

void rfc_send_nsc(tRFC_MCB *p_mcb)
{
	BT_HDR *p_buf;
	UINT8 *p_data;

	if ((p_buf = GKI_getpoolbuf(RFCOMM_CMD_POOL_ID)) == NULL)
		return;

	p_buf->offset = L2CAP_MIN_OFFSET + RFCOMM_CTRL_FRAME_LEN;
	p_data = (UINT8 *)(p_buf + 1) + p_buf->offset;

	*p_data++ = RFCOMM_EA | RFCOMM_I_CR(FALSE) | RFCOMM_MX_NSC;
	*p_data++ = RFCOMM_EA | RFCOMM_MX_NSC_LEN << 1;

	*p_data++ = rfc_cb.rfc.rx_frame.ea
	          | rfc_cb.rfc.rx_frame.cr << RFCOMM_SHIFT_CR
	          | rfc_cb.rfc.rx_frame.type;

	p_buf->len = RFCOMM_MX_NSC_LEN + 2;

	rfc_send_buf_uih(p_mcb, RFCOMM_MX_DLCI, p_buf);
}

void rfc_send_rpn(tRFC_MCB *p_mcb, UINT8 dlci, BOOLEAN is_command,
                  tPORT_STATE *p_pars, UINT16 mask)
{
	BT_HDR *p_buf;
	UINT8 *p_data;

	if ((p_buf = GKI_getpoolbuf(RFCOMM_CMD_POOL_ID)) == NULL)
		return;

	p_buf->offset = L2CAP_MIN_OFFSET + RFCOMM_CTRL_FRAME_LEN;
	p_data = (UINT8 *)(p_buf + 1) + p_buf->offset;

	*p_data++ = RFCOMM_EA | RFCOMM_I_CR(is_command) | RFCOMM_MX_RPN;

	if (!p_pars)
	{
		*p_data++ = RFCOMM_EA | RFCOMM_MX_RPN_REQ_LEN << 1;
		*p_data++ = RFCOMM_EA | RFCOMM_CR_MASK | dlci << RFCOMM_SHIFT_DLCI;

		p_buf->len = RFCOMM_MX_RPN_REQ_LEN + 2;
	}
	else
	{
		*p_data++ = RFCOMM_EA | RFCOMM_MX_RPN_LEN << 1;
		*p_data++ = RFCOMM_EA | RFCOMM_CR_MASK | dlci << RFCOMM_SHIFT_DLCI;
		*p_data++ = p_pars->baud_rate;
		*p_data++ = p_pars->byte_size << RFCOMM_RPN_BITS_SHIFT
		          | p_pars->stop_bits << RFCOMM_RPN_STOP_BITS_SHIFT
		          | p_pars->parity << RFCOMM_RPN_PARITY_SHIFT
		          | p_pars->parity_type << RFCOMM_RPN_PARITY_TYPE_SHIFT;
		*p_data++ = p_pars->fc_type;
		*p_data++ = p_pars->xon_char;
		*p_data++ = p_pars->xoff_char;
		*p_data++ = mask & 0xff;
		*p_data++ = mask >> 8;

		p_buf->len = RFCOMM_MX_RPN_LEN + 2;
	}

	rfc_send_buf_uih(p_mcb, RFCOMM_MX_DLCI, p_buf);
}

void rfc_send_test(tRFC_MCB *p_mcb, BOOLEAN is_command, BT_HDR *p_buf)
{
	UINT8 *p_data;
	UINT16 xx;
	UINT8 *p_src;
	UINT8 *p_dest;

	if (p_buf->offset < L2CAP_MIN_OFFSET + RFCOMM_MIN_OFFSET + 2)
	{
		p_src = (UINT8 *)(p_buf + 1) + p_buf->offset + p_buf->len - 1;
		p_dest = (UINT8 *)(p_buf + 1) + L2CAP_MIN_OFFSET + RFCOMM_MIN_OFFSET + 2
		       + p_buf->len - 1;

		for (xx = 0; xx < p_buf->len; ++xx)
			*p_dest-- = *p_src--;

		p_buf->offset = L2CAP_MIN_OFFSET + RFCOMM_MIN_OFFSET + 2;
	}

	p_buf->offset -= 2;
	p_data = (UINT8 *)(p_buf + 1) + p_buf->offset;

	*p_data++ = RFCOMM_EA | RFCOMM_I_CR(is_command) | RFCOMM_MX_TEST;
	*p_data++ = RFCOMM_EA | p_buf->len << 1;

	p_buf->len += 2;

	rfc_send_buf_uih(p_mcb, RFCOMM_MX_DLCI, p_buf);
}

void rfc_send_credit(tRFC_MCB *p_mcb, UINT8 dlci, UINT8 credit)
{
	BT_HDR *p_buf;
	UINT8 *p_data;
	UINT8 cr = RFCOMM_CR(p_mcb->is_initiator, TRUE);

	if ((p_buf = GKI_getpoolbuf(RFCOMM_CMD_POOL_ID)) == NULL)
		return;

	p_buf->offset = L2CAP_MIN_OFFSET;
	p_data = (UINT8 *)(p_buf + 1) + p_buf->offset;

	*p_data++ = RFCOMM_EA | cr | dlci << RFCOMM_SHIFT_DLCI;
	*p_data++ = RFCOMM_UIH | RFCOMM_PF;
	*p_data++ = RFCOMM_EA | 0;
	*p_data++ = credit;
	*p_data = RFCOMM_UIH_FCS((UINT8 *)(p_buf + 1) + p_buf->offset, dlci);

	p_buf->len = 5;

	rfc_check_send_cmd(p_mcb, p_buf);
}

UINT8 rfc_parse_data(tRFC_MCB *p_mcb, MX_FRAME *p_frame, BT_HDR *p_buf)
{
	UINT8 ead;
	UINT8 eal;
	UINT8 fcs;
	UINT8 *p_data = (UINT8 *)(p_buf + 1) + p_buf->offset;
	UINT8 *p_start = p_data;
	UINT16 len;

	if (p_buf->len < RFCOMM_CTRL_FRAME_LEN)
	{
		RFCOMM_TRACE(ERROR, "Bad Length1: %d", p_buf->len);
		return RFC_EVENT_BAD_FRAME;
	}

	RFCOMM_PARSE_CTRL_FIELD(ead, p_frame->cr, p_frame->dlci, p_data);
	RFCOMM_PARSE_TYPE_FIELD(p_frame->type, p_frame->pf, p_data);
	RFCOMM_PARSE_LEN_FIELD(eal, len, p_data);

	p_buf->len -= 3 + !ead + !eal + 1;
	p_buf->offset += 3 + !ead + !eal;

	if (p_mcb->flow == PORT_FC_CREDIT && p_frame->type == RFCOMM_UIH
	    && p_frame->dlci != RFCOMM_MX_DLCI && p_frame->pf == 1)
	{
		p_frame->credit = *p_data++;
		--p_buf->len;
		++p_buf->offset;
	}
	else
	{
		p_frame->credit = 0;
	}

	if (p_buf->len != len)
	{
		RFCOMM_TRACE(ERROR, "Bad Length2 %d %d", p_buf->len, len);
		return RFC_EVENT_BAD_FRAME;
	}

	fcs = p_data[len];

	switch (p_frame->type)
	{
	case RFCOMM_SABME:
		if (RFCOMM_FRAME_IS_RSP(p_mcb->is_initiator, p_frame->cr)
		    || !p_frame->pf || len || !RFCOMM_VALID_DLCI(p_frame->dlci)
		    || !rfc_check_fcs(RFCOMM_CTRL_FRAME_LEN, p_start, fcs))
		{
			RFCOMM_TRACE(ERROR, "Bad SABME");
			return RFC_EVENT_BAD_FRAME;
		}
		else
		{
			return RFC_EVENT_SABME;
		}

	case RFCOMM_UA:
		if (RFCOMM_FRAME_IS_CMD(p_mcb->is_initiator, p_frame->cr)
		    || !p_frame->pf || len || !RFCOMM_VALID_DLCI(p_frame->dlci)
		    || !rfc_check_fcs(RFCOMM_CTRL_FRAME_LEN, p_start, fcs))
		{
			RFCOMM_TRACE(ERROR, "Bad UA");
			return RFC_EVENT_BAD_FRAME;
		}
		else
		{
			return RFC_EVENT_UA;
		}

	case RFCOMM_DM:
		if (RFCOMM_FRAME_IS_CMD(p_mcb->is_initiator, p_frame->cr) || len
		    || !RFCOMM_VALID_DLCI(p_frame->dlci)
		    || !rfc_check_fcs(RFCOMM_CTRL_FRAME_LEN, p_start, fcs))
		{
			RFCOMM_TRACE(ERROR, "Bad DM");
			return RFC_EVENT_BAD_FRAME;
		}
		else
		{
			return RFC_EVENT_DM;
		}

	case RFCOMM_DISC:
		if (RFCOMM_FRAME_IS_RSP(p_mcb->is_initiator, p_frame->cr)
		    || !p_frame->pf || len || !RFCOMM_VALID_DLCI(p_frame->dlci)
		    || !rfc_check_fcs(RFCOMM_CTRL_FRAME_LEN, p_start, fcs))
		{
			RFCOMM_TRACE(ERROR, "Bad DISC");
			return RFC_EVENT_BAD_FRAME;
		}
		else
		{
			return RFC_EVENT_DISC;
		}

	case RFCOMM_UIH:
		if (!RFCOMM_VALID_DLCI(p_frame->dlci))
		{
			RFCOMM_TRACE(ERROR, "Bad UIH - invalid DLCI");
			return RFC_EVENT_BAD_FRAME;
		}
		else if (!rfc_check_fcs(2, p_start, fcs))
		{
			RFCOMM_TRACE(ERROR, "Bad UIH - FCS");
			return RFC_EVENT_BAD_FRAME;
		}
		else if (RFCOMM_FRAME_IS_RSP(p_mcb->is_initiator, p_frame->cr))
		{
			RFCOMM_TRACE(ERROR, "Bad UIH - response");
			return RFC_EVENT_UIH;
		}
		else
		{
			return RFC_EVENT_UIH;
		}
	}

	return RFC_EVENT_BAD_FRAME;
}

void rfc_process_mx_message(tRFC_MCB *p_mcb, BT_HDR *p_buf)
{
	UINT8 *p_data = (UINT8 *)(p_buf + 1) + p_buf->offset;
	MX_FRAME *p_rx_frame = &rfc_cb.rfc.rx_frame;
	UINT16 length = p_buf->len;
	UINT8 ea;
	UINT8 cr;
	UINT8 mx_len;
	BOOLEAN is_command;

	p_rx_frame->ea = *p_data & RFCOMM_EA;
	p_rx_frame->cr = (*p_data & RFCOMM_CR_MASK) >> RFCOMM_SHIFT_CR;
	p_rx_frame->type = *p_data++ & ~(RFCOMM_CR_MASK | RFCOMM_EA_MASK);

	if (!p_rx_frame->ea || !length)
	{
		RFCOMM_TRACE(ERROR, "Illegal MX Frame ea:%d len:%d", p_rx_frame->ea,
		             length);
		GKI_freebuf(p_buf);
		return;
	}

	--length;

	is_command = p_rx_frame->cr;

	ea = *p_data & RFCOMM_EA;

	mx_len = *p_data++ >> RFCOMM_SHIFT_LENGTH1;
	--length;

	if (!ea)
	{
		mx_len += *p_data++ << RFCOMM_SHIFT_LENGTH2;
		--length;
	}

	if (mx_len != length)
	{
		RFCOMM_TRACE(ERROR, "Bad MX frame");
		GKI_freebuf(p_buf);
		return;
	}

	switch (p_rx_frame->type)
	{
	case RFCOMM_MX_PN:
		if (length != RFCOMM_MX_PN_LEN)
			break;

		p_rx_frame->dlci = *p_data++ & RFCOMM_PN_DLCI_MASK;
		p_rx_frame->u.pn.frame_type = *p_data & RFCOMM_PN_FRAME_TYPE_MASK;
		p_rx_frame->u.pn.conv_layer = *p_data++ & RFCOMM_PN_CONV_LAYER_MASK;
		p_rx_frame->u.pn.priority = *p_data++ & RFCOMM_PN_PRIORITY_MASK;
		p_rx_frame->u.pn.t1 = *p_data++;
		p_rx_frame->u.pn.mtu = p_data[0] + (p_data[1] << 8);
		p_data += 2;

		p_rx_frame->u.pn.n2 = *p_data++;
		p_rx_frame->u.pn.k = *p_data++ & RFCOMM_PN_K_MASK;

		if (!p_rx_frame->dlci || !RFCOMM_VALID_DLCI(p_rx_frame->dlci)
		    || p_rx_frame->u.pn.mtu < RFCOMM_MIN_MTU
		    || p_rx_frame->u.pn.mtu > RFCOMM_MAX_MTU)
		{
			RFCOMM_TRACE(ERROR, "Bad PN frame");
			break;
		}

		GKI_freebuf(p_buf);

		rfc_process_pn(p_mcb, is_command, p_rx_frame);
		return;

	case RFCOMM_MX_TEST:
		if (!length)
			break;

		p_rx_frame->u.test.p_data = p_data;
		p_rx_frame->u.test.data_len = length;

		p_buf->offset += 2;
		p_buf->len -= 2;

		if (is_command)
			rfc_send_test(p_mcb, FALSE, p_buf);
		else
			rfc_process_test_rsp(p_mcb, p_buf);

		return;

	case RFCOMM_MX_FCON:
		if (length != RFCOMM_MX_FCON_LEN)
			break;

		GKI_freebuf(p_buf);

		rfc_process_fcon(p_mcb, is_command);
		return;

	case RFCOMM_MX_FCOFF:
		if (length != RFCOMM_MX_FCOFF_LEN)
			break;

		GKI_freebuf(p_buf);

		rfc_process_fcoff(p_mcb, is_command);
		return;

	case RFCOMM_MX_MSC:

		ea = *p_data & RFCOMM_EA;
		cr = (*p_data & RFCOMM_CR_MASK) >> RFCOMM_SHIFT_CR;
		p_rx_frame->dlci = *p_data++ >> RFCOMM_SHIFT_DLCI;

		if (!ea || !cr || !p_rx_frame->dlci
		    || !RFCOMM_VALID_DLCI(p_rx_frame->dlci))
		{
			RFCOMM_TRACE(ERROR, "Bad MSC frame");
			break;
		}

		p_rx_frame->u.msc.signals = *p_data++;

		if (mx_len == RFCOMM_MX_MSC_LEN_WITH_BREAK)
		{
			p_rx_frame->u.msc.break_present =
				*p_data & RFCOMM_MSC_BREAK_PRESENT_MASK;
			p_rx_frame->u.msc.break_duration =
				(*p_data & RFCOMM_MSC_BREAK_MASK) >> RFCOMM_MSC_SHIFT_BREAK;
		}
		else
		{
			p_rx_frame->u.msc.break_present = FALSE;
			p_rx_frame->u.msc.break_duration = 0;
		}

		GKI_freebuf(p_buf);

		rfc_process_msc(p_mcb, is_command, p_rx_frame);
		return;

	case RFCOMM_MX_NSC:
		if (length != RFCOMM_MX_NSC_LEN || !is_command)
			break;

		p_rx_frame->u.nsc.ea = *p_data & RFCOMM_EA;
		p_rx_frame->u.nsc.cr = (*p_data & RFCOMM_CR_MASK) >> RFCOMM_SHIFT_CR;
		p_rx_frame->u.nsc.type = *p_data++ >> RFCOMM_SHIFT_DLCI;

		GKI_freebuf(p_buf);

		rfc_process_nsc(p_mcb, p_rx_frame);
		return;

	case RFCOMM_MX_RPN:
		if (length != RFCOMM_MX_RPN_REQ_LEN && length != RFCOMM_MX_RPN_LEN)
			break;

		ea = *p_data & RFCOMM_EA;
		cr = (*p_data & RFCOMM_CR_MASK) >> RFCOMM_SHIFT_CR;
		p_rx_frame->dlci = *p_data++ >> RFCOMM_SHIFT_DLCI;

		if (!ea || !cr || !p_rx_frame->dlci
		    || !RFCOMM_VALID_DLCI(p_rx_frame->dlci))
		{
			RFCOMM_TRACE(ERROR, "Bad RPN frame");
			break;
		}

		p_rx_frame->u.rpn.is_request = length == RFCOMM_MX_RPN_REQ_LEN;

		if (!p_rx_frame->u.rpn.is_request)
		{
			p_rx_frame->u.rpn.baud_rate = *p_data++;
			p_rx_frame->u.rpn.byte_size =
				*p_data >> RFCOMM_RPN_BITS_SHIFT & RFCOMM_RPN_BITS_MASK;
			p_rx_frame->u.rpn.stop_bits =
				*p_data >> RFCOMM_RPN_STOP_BITS_SHIFT
				& RFCOMM_RPN_STOP_BITS_MASK;
			p_rx_frame->u.rpn.parity =
				*p_data >> RFCOMM_RPN_PARITY_SHIFT & RFCOMM_RPN_PARITY_MASK;
			p_rx_frame->u.rpn.parity_type =
				*p_data++ >> RFCOMM_RPN_PARITY_TYPE_SHIFT
				& RFCOMM_RPN_PARITY_TYPE_MASK;

			p_rx_frame->u.rpn.fc_type = *p_data++ & RFCOMM_FC_MASK;
			p_rx_frame->u.rpn.xon_char = *p_data++;
			p_rx_frame->u.rpn.xoff_char = *p_data++;
			p_rx_frame->u.rpn.param_mask =
				p_data[0] + (p_data[1] << 8) & RFCOMM_RPN_PM_MASK;
		}

		GKI_freebuf(p_buf);

		rfc_process_rpn(p_mcb, is_command, p_rx_frame->u.rpn.is_request,
		                p_rx_frame);
		return;

	case RFCOMM_MX_RLS:
		if (length != RFCOMM_MX_RLS_LEN)
			break;

		ea = *p_data & RFCOMM_EA;
		cr = (*p_data & RFCOMM_CR_MASK) >> RFCOMM_SHIFT_CR;

		p_rx_frame->dlci = *p_data++ >> RFCOMM_SHIFT_DLCI;
		p_rx_frame->u.rls.line_status = *p_data & ~0x01;

		if (!ea || !cr || !p_rx_frame->dlci
		    || !RFCOMM_VALID_DLCI(p_rx_frame->dlci))
		{
			RFCOMM_TRACE(ERROR, "Bad RPN frame");
			break;
		}

		GKI_freebuf(p_buf);

		rfc_process_rls(p_mcb, is_command, p_rx_frame);
		return;
	}

	GKI_freebuf(p_buf);

	if (is_command)
		rfc_send_nsc(p_mcb);
}
