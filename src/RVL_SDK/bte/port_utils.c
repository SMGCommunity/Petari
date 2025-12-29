#include "port_int.h"

/* Original source:
 * bluedroid <android.googlesource.com/platform/external/bluetooth/bluedroid>
 * stack/rfcomm/port_utils.c
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

#include "btm_int.h"
#include "gki.h"
#include "l2cdefs.h"
#include "port_api.h"
#include "rfc_int.h"
#include "rfcdefs.h"

/*******************************************************************************
 * macros
 */

/*******************************************************************************
 * variables
 */

// .rodata
static tPORT_STATE const default_port_pars = {PORT_BAUD_RATE_9600, PORT_8_BITS, PORT_ONESTOPBIT, PORT_PARITY_NO, PORT_ODD_PARITY,
                                              PORT_FC_OFF,         0,           PORT_XON_DC1,    PORT_XOFF_DC3};

/*******************************************************************************
 * functions
 */

tPORT* port_allocate_port(UINT8 dlci, BD_ADDR bd_addr) {
    tPORT* p_port = rfc_cb.port.port;
    UINT8 xx;
    UINT8 yy;

    /* explicitly post-increment */
    for (xx = 0, yy = rfc_cb.rfc.last_port + 1; xx < MAX_RFC_PORTS; xx++, yy++) {
        if (yy >= MAX_RFC_PORTS)
            yy = 0;

        p_port = &rfc_cb.port.port[yy];
        if (!p_port->in_use) {
            memset(p_port, 0, sizeof *p_port);

            p_port->in_use = TRUE;
            p_port->inx = yy + 1;

            p_port->dlci = dlci;
            memcpy(p_port->bd_addr, bd_addr, BD_ADDR_LEN);

            port_set_defaults(p_port);

            rfc_cb.rfc.last_port = yy;
            return p_port;
        }
    }

    return NULL;
}

void port_set_defaults(tPORT* p_port) {
    p_port->ev_mask = 0;
    p_port->p_callback = NULL;
    p_port->port_ctrl = 0;
    p_port->error = 0;
    p_port->line_status = 0;
    p_port->rx_flag_ev_pending = FALSE;
    p_port->peer_mtu = RFCOMM_DEFAULT_MTU;

    p_port->user_port_pars = default_port_pars;
    p_port->peer_port_pars = default_port_pars;

    p_port->credit_tx = 0;
    p_port->credit_rx = 0;

    memset(&p_port->local_ctrl, 0, sizeof p_port->local_ctrl);
    memset(&p_port->peer_ctrl, 0, sizeof p_port->peer_ctrl);
    memset(&p_port->rx, 0, sizeof p_port->rx);
    memset(&p_port->tx, 0, sizeof p_port->tx);
}

void port_select_mtu(tPORT* p_port) {
    UINT16 packet_size;

    if (p_port->mtu == 0) {
        packet_size = btm_get_max_packet_size(p_port->bd_addr);
        if (packet_size == 0) {
            RFCOMM_TRACE(WARNING, "port_select_mtu bad packet size");
            p_port->mtu = RFCOMM_DEFAULT_MTU;
        } else if (L2CAP_MTU_SIZE + L2CAP_PKT_OVERHEAD >= packet_size) {
            p_port->mtu = (L2CAP_MTU_SIZE + L2CAP_PKT_OVERHEAD) / packet_size * packet_size - RFCOMM_DATA_OVERHEAD - L2CAP_PKT_OVERHEAD;

            RFCOMM_TRACE(DEBUG, "port_select_mtu selected %d based on connection speed", p_port->mtu);
        } else {
            p_port->mtu = L2CAP_MTU_SIZE - RFCOMM_DATA_OVERHEAD;

            RFCOMM_TRACE(DEBUG, "port_select_mtu selected %d based on l2cap PDU size", p_port->mtu);
        }
    } else {
        RFCOMM_TRACE(DEBUG, "port_select_mtu application selected %d", p_port->mtu);
    }

    p_port->credit_rx_max = PORT_RX_HIGH_WM / p_port->mtu;
    p_port->credit_rx_low = PORT_RX_LOW_WM / p_port->mtu;
    p_port->rx_buf_critical = PORT_RX_CRITICAL_WM / p_port->mtu;

    RFCOMM_TRACE(DEBUG,
                 "port_select_mtu credit_rx_max %d, credit_rx_low %d, "
                 "rx_buf_critical %d",
                 p_port->credit_rx_max, p_port->credit_rx_low, p_port->rx_buf_critical);
}

void port_release_port(tPORT* p_port) {
    BT_HDR* p_buf;

    while ((p_buf = GKI_dequeue(&p_port->rx.queue)) != NULL)
        GKI_freebuf(p_buf);

    p_port->rx.queue_size = 0;

    while ((p_buf = GKI_dequeue(&p_port->tx.queue)) != NULL)
        GKI_freebuf(p_buf);

    p_port->tx.queue_size = 0;

    p_port->state = PORT_STATE_CLOSED;

    if (p_port->rfc.state == RFC_STATE_CLOSED) {
        RFCOMM_TRACE(DEBUG, "rfc_port_closed DONE");

        if (p_port->rfc.p_mcb) {
            p_port->rfc.p_mcb->port_inx[p_port->dlci] = 0;

            rfc_check_mcb_active(p_port->rfc.p_mcb);
        }

        rfc_port_timer_stop(p_port);

        memset(p_port, 0, sizeof(tPORT));
    }
}

tRFC_MCB* port_find_mcb(BD_ADDR bd_addr) {
    int i;

    for (i = 0; i < MAX_BD_CONNECTIONS; ++i) {
        if (rfc_cb.port.rfc_mcb[i].state != RFC_MX_STATE_IDLE && memcmp(rfc_cb.port.rfc_mcb[i].bd_addr, bd_addr, BD_ADDR_LEN) == 0) {
            return &rfc_cb.port.rfc_mcb[i];
        }
    }

    return NULL;
}

tPORT* port_find_mcb_dlci_port(tRFC_MCB* p_mcb, UINT8 dlci) {
    UINT8 inx;

    if (!p_mcb)
        return NULL;

    if (dlci > RFCOMM_MAX_DLCI)
        return NULL;

    inx = p_mcb->port_inx[dlci];
    if (inx == 0)
        return NULL;

    return &rfc_cb.port.port[inx - 1];
}

tPORT* port_find_dlci_port(UINT8 dlci) {
    UINT16 i;
    tPORT* p_port;

    for (i = 0; i < MAX_RFC_PORTS; ++i) {
        p_port = &rfc_cb.port.port[i];

        if (!p_port->in_use)
            continue;

        if (p_port->rfc.p_mcb)
            continue;

        if (p_port->dlci == dlci)
            return p_port;

        if (dlci & 0x01 && p_port->dlci == dlci - 1) {
            ++p_port->dlci;
            return p_port;
        }
    }

    return NULL;
}

tPORT* port_find_port(UINT8 dlci, BD_ADDR bd_addr) {
    UINT16 i;
    tPORT* p_port;

    for (i = 0; i < MAX_RFC_PORTS; i++) {
        p_port = &rfc_cb.port.port[i];

        if (p_port->in_use && p_port->dlci == dlci && memcmp(p_port->bd_addr, bd_addr, BD_ADDR_LEN) == 0) {
            return p_port;
        }
    }

    return NULL;
}

UINT32 port_flow_control_user(tPORT* p_port) {
    UINT32 event = 0;

    BOOLEAN fc = p_port->tx.peer_fc || !p_port->rfc.p_mcb || !p_port->rfc.p_mcb->peer_ready || p_port->tx.queue_size > PORT_TX_HIGH_WM ||
                 p_port->tx.queue.count > PORT_TX_BUF_HIGH_WM;

    if (p_port->tx.user_fc == fc)
        return 0;

    p_port->tx.user_fc = fc;

    if (fc)
        event = PORT_EV_FC;
    else
        event = PORT_EV_FC | PORT_EV_FCS;

    return event;
}

UINT32 port_get_signal_changes(tPORT* p_port, UINT8 old_signals, UINT8 signal) {
    UINT8 changed_signals = signal ^ old_signals;
    UINT32 events = 0;

    if (changed_signals & PORT_DTRDSR_ON) {
        events |= PORT_EV_DSR;

        if (signal & PORT_DTRDSR_ON)
            events |= PORT_EV_DSRS;
    }

    if (changed_signals & PORT_CTSRTS_ON) {
        events |= PORT_EV_CTS;

        if (signal & PORT_CTSRTS_ON)
            events |= PORT_EV_CTSS;
    }

    if (changed_signals & PORT_RING_ON)
        events |= PORT_EV_RING;

    if (changed_signals & PORT_DCD_ON) {
        events |= PORT_EV_RLSD;

        if (signal & PORT_DCD_ON)
            events |= PORT_EV_RLSDS;
    }

    return p_port->ev_mask & events;
}

void port_flow_control_peer(tPORT* p_port, BOOLEAN enable, UINT16 count) {
    if (!p_port->rfc.p_mcb)
        return;

    if (p_port->rfc.p_mcb->flow == PORT_FC_CREDIT) {
        if (enable) {
            if (count > p_port->credit_rx)
                p_port->credit_rx = 0;
            else
                p_port->credit_rx -= count;

            if (p_port->credit_rx <= p_port->credit_rx_low && !p_port->rx.user_fc && p_port->credit_rx_max > p_port->credit_rx) {
                rfc_send_credit(p_port->rfc.p_mcb, p_port->dlci, p_port->credit_rx_max - p_port->credit_rx);

                p_port->credit_rx = p_port->credit_rx_max;

                p_port->rx.peer_fc = FALSE;
            }
        } else {
            if (p_port->p_data_callback)
                p_port->rx.peer_fc = TRUE;
            else if (p_port->rx.queue.count >= p_port->credit_rx_max)
                p_port->rx.peer_fc = TRUE;
        }
    } else if (enable) {
        if (p_port->rx.peer_fc && p_port->rx.queue_size < PORT_RX_LOW_WM && p_port->rx.queue.count < PORT_RX_BUF_LOW_WM) {
            p_port->rx.peer_fc = FALSE;

            if (!p_port->rx.user_fc)
                RFCOMM_FlowReq(p_port->rfc.p_mcb, p_port->dlci, TRUE);
        }
    } else if (p_port->p_data_callback) {
        p_port->rx.peer_fc = TRUE;
        RFCOMM_FlowReq(p_port->rfc.p_mcb, p_port->dlci, FALSE);
    } else if ((p_port->rx.queue_size > PORT_RX_HIGH_WM || p_port->rx.queue.count > PORT_RX_BUF_HIGH_WM) && !p_port->rx.peer_fc) {
        RFCOMM_TRACE(EVENT, "PORT_DataInd Data reached HW. Sending FC set.");

        p_port->rx.peer_fc = TRUE;
        RFCOMM_FlowReq(p_port->rfc.p_mcb, p_port->dlci, FALSE);
    }
}
