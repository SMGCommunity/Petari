#include "port_int.h"

/* Original source:
 * bluedroid <android.googlesource.com/platform/external/bluetooth/bluedroid>
 * stack/rfcomm/port_rfc.c
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

#include "revolution/types.h"
#include <cstring>

#include "bt_target.h"
#include "bt_trace.h"
#include "bt_types.h"
#include "data_types.h"

#include "gki.h"
#include "hcidefs.h"
#include "port_api.h"
#include "rfc_int.h"
#include "rfcdefs.h"
#include "sdpdefs.h"

/*******************************************************************************
 * functions
 */

int port_open_continue(tPORT* p_port) {
    tRFC_MCB* p_mcb;

    RFCOMM_TRACE(EVENT, "port_open_continue");

    if ((p_mcb = rfc_alloc_multiplexer_channel(p_port->bd_addr, TRUE)) == NULL) {
        RFCOMM_TRACE(WARNING, "port_open_continue no mx channel");
        port_release_port(p_port);
        return PORT_NO_RESOURCES;
    }

    p_port->rfc.p_mcb = p_mcb;

    p_mcb->port_inx[p_port->dlci] = p_port->inx;

    port_select_mtu(p_port);

    if (p_mcb->state == RFC_MX_STATE_CONNECTED)
        RFCOMM_ParNegReq(p_mcb, p_port->dlci, p_port->mtu);
    else
        RFCOMM_StartReq(p_mcb);

    return PORT_SUCCESS;
}

void port_start_control(tPORT* p_port) {
    tRFC_MCB* p_mcb = p_port->rfc.p_mcb;

    if (!p_mcb)
        return;

    RFCOMM_ControlReq(p_mcb, p_port->dlci, &p_port->local_ctrl);
}

void port_start_par_neg(tPORT* p_port) {
    tRFC_MCB* p_mcb = p_port->rfc.p_mcb;

    if (!p_mcb)
        return;

    RFCOMM_PortNegReq(p_mcb, p_port->dlci, &p_port->user_port_pars);
}

void port_start_close(tPORT* p_port) {
    tRFC_MCB* p_mcb = p_port->rfc.p_mcb;
    UINT8 old_signals;
    UINT32 events = 0;

    p_port->line_status |= LINE_STATUS_FAILED;
    old_signals = p_port->peer_ctrl.modem_signal;

    p_port->peer_ctrl.modem_signal &= ~(PORT_DTRDSR_ON | PORT_CTSRTS_ON | PORT_DCD_ON);

    events |= port_get_signal_changes(p_port, old_signals, p_port->peer_ctrl.modem_signal);

    if (p_port->ev_mask & PORT_EV_CONNECT_ERR)
        events |= PORT_EV_CONNECT_ERR;

    if (p_port->ev_mask & PORT_EV_ERR)
        events |= PORT_EV_ERR;

    if (p_port->p_callback && events)
        (*p_port->p_callback)(events, p_port->inx);

    if (!p_mcb || p_port->rfc.state == RFC_STATE_CLOSED) {
        if (p_port->p_mgmt_callback)
            (*p_port->p_mgmt_callback)(PORT_CLOSED, p_port->inx);

        port_release_port(p_port);
    } else {
        RFCOMM_DlcReleaseReq(p_mcb, p_port->dlci);
    }
}

void PORT_StartCnf(tRFC_MCB* p_mcb, UINT16 result) {
    tPORT* p_port;
    int i;
    BOOLEAN no_ports_up = TRUE;

    RFCOMM_TRACE(EVENT, "PORT_StartCnf result:%d", result);

    p_port = rfc_cb.port.port;

    /* explicitly post-increment */
    for (i = 0; i < MAX_RFC_PORTS; i++, p_port++) {
        if (p_port->rfc.p_mcb == p_mcb) {
            no_ports_up = FALSE;

            if (result == RFCOMM_SUCCESS)
                RFCOMM_ParNegReq(p_mcb, p_port->dlci, p_port->mtu);
            else {
                RFCOMM_TRACE(WARNING, "PORT_StartCnf failed result:%d", result);

                if (result == HCI_ERR_PAGE_TIMEOUT)
                    p_port->error = PORT_PAGE_TIMEOUT;
                else
                    p_port->error = PORT_START_FAILED;

                rfc_release_multiplexer_channel(p_mcb);
                p_port->rfc.p_mcb = NULL;

                if (p_port->p_callback && p_port->ev_mask & PORT_EV_CONNECT_ERR)
                    (*p_port->p_callback)(PORT_EV_CONNECT_ERR, p_port->inx);

                if (p_port->p_mgmt_callback)
                    (*p_port->p_mgmt_callback)(PORT_START_FAILED, p_port->inx);

                port_release_port(p_port);
            }
        }
    }

    if (no_ports_up)
        rfc_check_mcb_active(p_mcb);
}

void PORT_StartInd(tRFC_MCB* p_mcb) {
    tPORT* p_port;
    int i;

    RFCOMM_TRACE(EVENT, "PORT_StartInd");

    p_port = rfc_cb.port.port;

    /* explicitly post-increment */
    for (i = 0; i < MAX_RFC_PORTS; i++, p_port++) {
        if (!p_port->rfc.p_mcb || p_port->rfc.p_mcb == p_mcb) {
            RFCOMM_StartRsp(p_mcb, RFCOMM_SUCCESS);
            return;
        }
    }

    RFCOMM_StartRsp(p_mcb, RFCOMM_ERROR);
}

void PORT_ParNegInd(tRFC_MCB* p_mcb, UINT8 dlci, UINT16 mtu, UINT8 cl, UINT8 k) {
    tPORT* p_port = port_find_mcb_dlci_port(p_mcb, dlci);
    UINT8 our_cl;
    UINT8 our_k;

    RFCOMM_TRACE(EVENT, "PORT_ParNegInd dlci:%d mtu:%d", dlci, mtu);

    if (!p_port) {
        p_port = port_find_dlci_port(dlci);
        if (!p_port) {
            rfc_send_dm(p_mcb, dlci, FALSE);
            rfc_check_mcb_active(p_mcb);

            RFCOMM_TRACE(EVENT, "PORT_ParNegInd: port not found");
            return;
        }

        p_mcb->port_inx[dlci] = p_port->inx;
    }

    memcpy(p_port->bd_addr, p_mcb->bd_addr, BD_ADDR_LEN);

    port_select_mtu(p_port);

    p_port->rfc.p_mcb = p_mcb;
    p_port->mtu = (p_port->mtu < mtu) ? p_port->mtu : mtu;
    p_port->peer_mtu = p_port->mtu;

    if (p_mcb->flow == PORT_FC_UNDEFINED) {
        if (cl == RFCOMM_PN_CONV_LAYER_TYPE_1)
            p_mcb->flow = PORT_FC_TS710;
        else
            p_mcb->flow = PORT_FC_CREDIT;
    }

    if (cl == RFCOMM_PN_CONV_LAYER_TYPE_1) {
        our_cl = RFCOMM_PN_CONV_LAYER_TYPE_1;
        our_k = 0;
    } else if (p_mcb->flow == PORT_FC_CREDIT) {
        port_get_credits(p_port, k);

        our_cl = RFCOMM_PN_CONV_LAYER_CBFC_R;
        our_k = p_port->credit_rx_max < RFCOMM_K_MAX ? p_port->credit_rx_max : RFCOMM_K_MAX;

        p_port->credit_rx = our_k;
    } else {
        our_cl = RFCOMM_PN_CONV_LAYER_TYPE_1;
        our_k = 0;
    }

    RFCOMM_ParNegRsp(p_mcb, dlci, p_port->mtu, our_cl, our_k);
}

void PORT_ParNegCnf(tRFC_MCB* p_mcb, UINT8 dlci, UINT16 mtu, UINT8 cl, UINT8 k) {
    tPORT* p_port = port_find_mcb_dlci_port(p_mcb, dlci);

    RFCOMM_TRACE(EVENT, "PORT_ParNegCnf dlci:%d mtu:%d cl: %d k: %d", dlci, mtu, cl, k);

    if (!p_port)
        return;

    if (p_mcb->flow == PORT_FC_UNDEFINED) {
        if (PORT_FC_DEFAULT == PORT_FC_TS710 && cl == RFCOMM_PN_CONV_LAYER_CBFC_R) {
            rfc_send_disc(p_mcb, p_port->dlci);
            rfc_port_closed(p_port);
            return;
        } else if (cl == RFCOMM_PN_CONV_LAYER_CBFC_R) {
            p_mcb->flow = PORT_FC_CREDIT;
        } else {
            p_mcb->flow = PORT_FC_TS710;
        }
    }

    p_port->mtu = p_port->mtu < mtu ? p_port->mtu : mtu;
    p_port->peer_mtu = p_port->mtu;

    if (p_mcb->flow == PORT_FC_CREDIT)
        port_get_credits(p_port, k);

    if (p_port->state == PORT_STATE_OPENING)
        RFCOMM_DlcEstablishReq(p_mcb, p_port->dlci, p_port->mtu);
}

void PORT_DlcEstablishInd(tRFC_MCB* p_mcb, UINT8 dlci, UINT16 mtu) {
    tPORT* p_port = port_find_mcb_dlci_port(p_mcb, dlci);

    RFCOMM_TRACE(EVENT, "PORT_DlcEstablishInd dlci:%d mtu:%d", dlci, mtu);

    if (!p_port) {
        p_port = port_find_dlci_port(dlci);
        if (!p_port) {
            RFCOMM_DlcEstablishRsp(p_mcb, dlci, 0, RFCOMM_ERROR);
            return;
        }

        p_mcb->port_inx[dlci] = p_port->inx;
    }

    if (mtu && mtu < p_port->peer_mtu)
        p_port->peer_mtu = mtu;

    rfc_timer_stop(p_mcb);

    RFCOMM_DlcEstablishRsp(p_mcb, dlci, p_port->mtu, RFCOMM_SUCCESS);

    if (p_port->p_callback && p_port->ev_mask & PORT_EV_CONNECTED)
        (*p_port->p_callback)(PORT_EV_CONNECTED, p_port->inx);

    if (p_port->p_mgmt_callback)
        (*p_port->p_mgmt_callback)(PORT_SUCCESS, p_port->inx);

    p_port->state = PORT_STATE_OPENED;
}

void PORT_DlcEstablishCnf(tRFC_MCB* p_mcb, UINT8 dlci, UINT16 mtu, UINT16 result) {
    tPORT* p_port = port_find_mcb_dlci_port(p_mcb, dlci);

    RFCOMM_TRACE(EVENT, "PORT_DlcEstablishCnf dlci:%d mtu:%d result:%d", dlci, mtu, result);

    if (!p_port)
        return;

    if (result != RFCOMM_SUCCESS) {
        p_port->error = PORT_START_FAILED;
        port_rfc_closed(p_port, PORT_START_FAILED);
        return;
    }

    if (mtu && mtu < p_port->peer_mtu)
        p_port->peer_mtu = mtu;

    rfc_timer_stop(p_mcb);

    if (p_port->p_callback && p_port->ev_mask & PORT_EV_CONNECTED)
        (*p_port->p_callback)(PORT_EV_CONNECTED, p_port->inx);

    if (p_port->p_mgmt_callback)
        (*p_port->p_mgmt_callback)(PORT_SUCCESS, p_port->inx);

    p_port->state = PORT_STATE_OPENED;

    if (p_port->uuid == UUID_SERVCLASS_DIALUP_NETWORKING || p_port->uuid == UUID_SERVCLASS_FAX) {
        RFCOMM_PortNegReq(p_port->rfc.p_mcb, p_port->dlci, NULL);
    } else {
        RFCOMM_ControlReq(p_port->rfc.p_mcb, p_port->dlci, &p_port->local_ctrl);
    }
}

void PORT_PortNegInd(tRFC_MCB* p_mcb, UINT8 dlci, tPORT_STATE* p_pars, UINT16 param_mask) {
    tPORT* p_port = port_find_mcb_dlci_port(p_mcb, dlci);

    RFCOMM_TRACE(EVENT, "PORT_PortNegInd");

    if (!p_port) {
        p_port = port_find_dlci_port(dlci);
        if (!p_port) {
            p_port->peer_port_pars = *p_pars;
            RFCOMM_PortNegRsp(p_mcb, dlci, p_pars, 0);
            return;
        }

        p_mcb->port_inx[dlci] = p_port->inx;
    }

    p_port->peer_port_pars = *p_pars;
    RFCOMM_PortNegRsp(p_mcb, dlci, p_pars, param_mask);
}

void PORT_PortNegCnf(tRFC_MCB* p_mcb, UINT8 dlci, tPORT_STATE* p_pars, UINT16 result) {
    tPORT* p_port = port_find_mcb_dlci_port(p_mcb, dlci);

    RFCOMM_TRACE(EVENT, "PORT_PortNegCnf");

    if (!p_port) {
        RFCOMM_TRACE(WARNING, "PORT_PortNegCnf no port");
        return;
    }

    if (result != RFCOMM_SUCCESS) {
        p_port->error = PORT_PORT_NEG_FAILED;

        RFCOMM_DlcReleaseReq(p_mcb, p_port->dlci);

        port_rfc_closed(p_port, PORT_PORT_NEG_FAILED);
        return;
    }

    if (!(p_port->port_ctrl & PORT_CTRL_REQ_SENT))
        RFCOMM_ControlReq(p_port->rfc.p_mcb, p_port->dlci, &p_port->local_ctrl);
    else
        RFCOMM_TRACE(WARNING, "PORT_PortNegCnf Control Already sent");
}

void PORT_ControlInd(tRFC_MCB* p_mcb, UINT8 dlci, tPORT_CTRL* p_pars) {
    tPORT* p_port = port_find_mcb_dlci_port(p_mcb, dlci);
    UINT32 event;
    UINT8 old_signals;

    RFCOMM_TRACE(EVENT, "PORT_ControlInd");

    if (!p_port)
        return;

    old_signals = p_port->peer_ctrl.modem_signal;

    event = port_get_signal_changes(p_port, old_signals, p_pars->modem_signal);

    p_port->peer_ctrl = *p_pars;

    if (!(p_port->port_ctrl & PORT_CTRL_REQ_SENT)) {
        RFCOMM_ControlReq(p_port->rfc.p_mcb, p_port->dlci, &p_port->local_ctrl);
    } else {
        if (!(p_port->port_ctrl & PORT_CTRL_IND_RECEIVED))
            event |= PORT_EV_CONNECTED & p_port->ev_mask;

        if (p_port->port_ctrl & PORT_CTRL_REQ_CONFIRMED)
            event |= port_rfc_send_tx_data(p_port);
    }

    p_port->port_ctrl |= PORT_CTRL_IND_RECEIVED | PORT_CTRL_IND_RESPONDED;

    if (p_pars->break_signal)
        event |= PORT_EV_BREAK & p_port->ev_mask;

    if (event && p_port->p_callback)
        (*p_port->p_callback)(event, p_port->inx);

    RFCOMM_TRACE(EVENT, "PORT_ControlInd DTR_DSR : %d, RTS_CTS : %d, RI : %d, DCD : %d", p_port->peer_ctrl.modem_signal & MODEM_SIGNAL_DTRDSR ? 1 : 0,
                 p_port->peer_ctrl.modem_signal & MODEM_SIGNAL_RTSCTS ? 1 : 0, p_port->peer_ctrl.modem_signal & MODEM_SIGNAL_RI ? 1 : 0,
                 p_port->peer_ctrl.modem_signal & MODEM_SIGNAL_DCD ? 1 : 0);
}

void PORT_ControlCnf(tRFC_MCB* p_mcb, UINT8 dlci, tPORT_CTRL* p_pars) {
    tPORT* p_port = port_find_mcb_dlci_port(p_mcb, dlci);
    UINT32 event = 0;

    RFCOMM_TRACE(EVENT, "PORT_ControlCnf");

    if (!p_port)
        return;

    if (!(p_port->port_ctrl & PORT_CTRL_REQ_CONFIRMED)) {
        p_port->port_ctrl |= PORT_CTRL_REQ_CONFIRMED;

        if (p_port->port_ctrl & PORT_CTRL_IND_RECEIVED)
            event = p_port->ev_mask & PORT_EV_CONNECTED;
    }

    if (p_port->port_ctrl & PORT_CTRL_IND_RECEIVED)
        event |= port_rfc_send_tx_data(p_port);

    if (event && p_port->p_callback)
        (*p_port->p_callback)(event, p_port->inx);
}

void PORT_LineStatusInd(tRFC_MCB* p_mcb, UINT8 dlci, UINT8 line_status) {
    tPORT* p_port = port_find_mcb_dlci_port(p_mcb, dlci);
    UINT32 event = 0;

    RFCOMM_TRACE(EVENT, "PORT_LineStatusInd");

    if (!p_port)
        return;

    p_port->line_status |= line_status;

    if (line_status & PORT_ERR_OVERRUN)
        event |= PORT_EV_OVERRUN;

    if (line_status & PORT_ERR_BREAK)
        event |= PORT_EV_BREAK;

    if (line_status & ~(PORT_ERR_OVERRUN | PORT_ERR_BREAK))
        event |= PORT_EV_ERR;

    if (p_port->p_callback && p_port->ev_mask & event)
        (*p_port->p_callback)(p_port->ev_mask& event, p_port->inx);
}

void PORT_DlcReleaseInd(tRFC_MCB* p_mcb, UINT8 dlci) {
    tPORT* p_port = port_find_mcb_dlci_port(p_mcb, dlci);

    RFCOMM_TRACE(EVENT, "PORT_DlcReleaseInd");

    if (!p_port)
        return;

    port_rfc_closed(p_port, PORT_CLOSED);
}

void PORT_CloseInd(tRFC_MCB* p_mcb) {
    tPORT* p_port;
    int i;

    RFCOMM_TRACE(EVENT, "PORT_CloseInd");

    p_port = rfc_cb.port.port;

    /* explicitly post-increment */
    for (i = 0; i < MAX_RFC_PORTS; i++, p_port++) {
        if (p_port->rfc.p_mcb == p_mcb)
            port_rfc_closed(p_port, PORT_PEER_CONNECTION_FAILED);
    }

    rfc_release_multiplexer_channel(p_mcb);
}

void Port_TimeOutCloseMux(tRFC_MCB* p_mcb) {
    tPORT* p_port;
    int i;

    RFCOMM_TRACE(EVENT, "Port_TimeOutCloseMux");

    p_port = rfc_cb.port.port;

    /* explicitly post-increment */
    for (i = 0; i < MAX_RFC_PORTS; i++, p_port++) {
        if (p_port->rfc.p_mcb == p_mcb)
            port_rfc_closed(p_port, PORT_PEER_TIMEOUT);
    }
}

void PORT_DataInd(tRFC_MCB* p_mcb, UINT8 dlci, BT_HDR* p_buf) {
    tPORT* p_port = port_find_mcb_dlci_port(p_mcb, dlci);
    UINT8 rx_char1;
    UINT32 events = 0;
    UINT8* p;
    int i;

    RFCOMM_TRACE(EVENT, "PORT_DataInd with data length %d", p_buf->len);

    if (!p_port) {
        GKI_freebuf(p_buf);
        return;
    }

    if (p_port->p_data_callback) {
        port_flow_control_peer(p_port, TRUE, 1);

        (*p_port->p_data_callback)(p_port->inx, (UINT8*)(p_buf + 1) + p_buf->offset, p_buf->len);

        GKI_freebuf(p_buf);
        return;
    }

    if (p_port->rx.queue_size + p_buf->len > PORT_RX_CRITICAL_WM || p_port->rx.queue.count + 1 > p_port->rx_buf_critical) {
        RFCOMM_TRACE(EVENT, "PORT_DataInd. Buffer over run. Dropping the buffer");
        GKI_freebuf(p_buf);

        RFCOMM_LineStatusReq(p_mcb, dlci, LINE_STATUS_OVERRUN);
        return;
    }

    if (((rx_char1 = p_port->user_port_pars.rx_char1) != 0) && p_port->ev_mask & PORT_EV_RXFLAG) {
        for (i = 0, p = (UINT8*)(p_buf + 1) + p_buf->offset; i < p_buf->len; ++i) {
            if (*p++ == rx_char1) {
                events |= PORT_EV_RXFLAG;
                break;
            }
        }
    }

    GKI_enqueue(&p_port->rx.queue, p_buf);
    p_port->rx.queue_size += p_buf->len;

    port_flow_control_peer(p_port, FALSE, 0);

    if (p_port->rx.user_fc) {
        if (events & PORT_EV_RXFLAG)
            p_port->rx_flag_ev_pending = TRUE;

        return;
    }

    events |= PORT_EV_RXCHAR;

    events &= p_port->ev_mask;

    if (p_port->p_callback && events)
        (*p_port->p_callback)(events, p_port->inx);
}

void PORT_FlowInd(tRFC_MCB* p_mcb, UINT8 dlci, BOOLEAN enable_data) {
    tPORT* p_port = NULL;
    UINT32 events = 0;
    int i;

    RFCOMM_TRACE(EVENT, "PORT_FlowInd fc:%d", enable_data);

    if (dlci == 0) {
        p_mcb->peer_ready = enable_data;
    } else {
        if ((p_port = port_find_mcb_dlci_port(p_mcb, dlci)) == NULL)
            return;

        p_port->tx.peer_fc = !enable_data;
    }

    for (i = 0; i < MAX_RFC_PORTS; ++i) {
        if (dlci == 0) {
            p_port = &rfc_cb.port.port[i];

            if (!p_port->in_use || p_port->rfc.p_mcb != p_mcb || p_port->rfc.state != RFC_STATE_OPENED) {
                continue;
            }
        }

        events = 0;
        events |= port_flow_control_user(p_port);
        events |= port_rfc_send_tx_data(p_port);
        events &= p_port->ev_mask;

        if (p_port->p_callback && events)
            (*p_port->p_callback)(events, p_port->inx);

        if (dlci != 0)
            break;
    }
}

UINT32 port_rfc_send_tx_data(tPORT* p_port) {
    UINT32 events = 0;
    BT_HDR* p_buf;

    if (p_port->tx.queue_size > 0) {
        while (!p_port->tx.peer_fc && p_port->rfc.p_mcb && p_port->rfc.p_mcb->peer_ready) {
            if ((p_buf = GKI_dequeue(&p_port->tx.queue)) != NULL) {
                RFCOMM_TRACE(EVENT, "Sending RFCOMM_DataReq");

                p_port->tx.queue_size -= p_buf->len;

                RFCOMM_DataReq(p_port->rfc.p_mcb, p_port->dlci, p_buf);

                events |= PORT_EV_TXCHAR;

                if (p_port->tx.queue_size == 0) {
                    events |= PORT_EV_TXEMPTY;
                    break;
                }
            } else {
                events |= PORT_EV_TXEMPTY;
                break;
            }
        }

        events |= port_flow_control_user(p_port);
    }

    return events & p_port->ev_mask;
}

void port_rfc_closed(tPORT* p_port, UINT8 res) {
    UINT8 old_signals;
    UINT32 events = 0;
    tRFC_MCB* p_mcb = p_port->rfc.p_mcb;

    if (p_port->state == PORT_STATE_OPENING && p_port->is_server) {
        RFCOMM_TRACE(EVENT, "port_rfc_closed in OPENING state ignored");

        rfc_port_timer_stop(p_port);
        p_port->rfc.state = RFC_STATE_CLOSED;

        if (p_mcb) {
            p_mcb->port_inx[p_port->dlci] = 0;

            rfc_check_mcb_active(p_mcb);
            p_port->rfc.p_mcb = NULL;
        }

        p_port->dlci &= 0xfe;

        return;
    }

    if (p_port->state != PORT_STATE_CLOSING && p_port->state != PORT_STATE_CLOSED) {
        p_port->line_status |= LINE_STATUS_FAILED;

        old_signals = p_port->peer_ctrl.modem_signal;

        p_port->peer_ctrl.modem_signal &= ~(PORT_DTRDSR_ON | PORT_CTSRTS_ON | PORT_DCD_ON);

        events |= port_get_signal_changes(p_port, old_signals, p_port->peer_ctrl.modem_signal);

        if (p_port->ev_mask & PORT_EV_CONNECT_ERR)
            events |= PORT_EV_CONNECT_ERR;
    }

    RFCOMM_TRACE(EVENT, "port_rfc_closed state:%d sending events:%x", p_port->state, events);

    if (p_port->p_callback && events)
        (*p_port->p_callback)(events, p_port->inx);

    if (p_port->p_mgmt_callback)
        (*p_port->p_mgmt_callback)(res, p_port->inx);

    p_port->rfc.state = RFC_STATE_CLOSED;

    port_release_port(p_port);
}

void port_get_credits(tPORT* p_port, UINT8 k) {
    p_port->credit_tx = k;

    if (p_port->credit_tx == 0)
        p_port->tx.peer_fc = TRUE;
}
