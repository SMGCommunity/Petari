#include "rfc_int.h"

/* Original source:
 * bluedroid <android.googlesource.com/platform/external/bluetooth/bluedroid>
 * stack/rfcomm/rfc_port_fsm.c
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

#include "bt_trace.h"
#include "bt_types.h"
#include "data_types.h"
#include "revolution/types.h"
#include <cstring>

#include "btm_api.h"
#include "btm_int.h"
#include "gki.h"
#include "port_api.h"
#include "port_int.h"
#include "rfcdefs.h"

/*******************************************************************************
 * macros
 */

/*******************************************************************************
 * types
 */

/*******************************************************************************
 * local function declarations
 */

static void rfc_port_sm_state_closed(tPORT* p_port, UINT16 event, void* p_data);
static void rfc_port_sm_sabme_wait_ua(tPORT* p_port, UINT16 event, void* p_data);
static void rfc_port_sm_term_wait_sec_check(tPORT* p_port, UINT16 event, void* p_data);
static void rfc_port_sm_orig_wait_sec_check(tPORT* p_port, UINT16 event, void* p_data);
static void rfc_port_sm_opened(tPORT* p_port, UINT16 event, void* p_data);
static void rfc_port_sm_disc_wait_ua(tPORT* p_port, UINT16 event, void* p_data);
static void rfc_port_uplink_data(tPORT* p_port, BT_HDR* p_buf);

static void rfc_set_port_state(tPORT_STATE* port_pars, MX_FRAME* p_frame);

/*******************************************************************************
 * functions
 */

void rfc_port_sm_execute(tPORT* p_port, UINT16 event, void* p_data) {
    if (!p_port) {
        RFCOMM_TRACE(WARNING, "NULL port event %d", event);
        return;
    }

    switch (p_port->rfc.state) {
    case RFC_STATE_CLOSED:
        rfc_port_sm_state_closed(p_port, event, p_data);
        break;

    case RFC_STATE_SABME_WAIT_UA:
        rfc_port_sm_sabme_wait_ua(p_port, event, p_data);
        break;

    case RFC_STATE_ORIG_WAIT_SEC_CHECK:
        rfc_port_sm_orig_wait_sec_check(p_port, event, p_data);
        break;

    case RFC_STATE_TERM_WAIT_SEC_CHECK:
        rfc_port_sm_term_wait_sec_check(p_port, event, p_data);
        break;

    case RFC_STATE_OPENED:
        rfc_port_sm_opened(p_port, event, p_data);
        break;

    case RFC_STATE_DISC_WAIT_UA:
        rfc_port_sm_disc_wait_ua(p_port, event, p_data);
        break;
    }
}

static void rfc_port_sm_state_closed(tPORT* p_port, UINT16 event, void* p_data) {
    switch (event) {
    case RFC_EVENT_OPEN:
        p_port->rfc.state = RFC_STATE_ORIG_WAIT_SEC_CHECK;
        btm_sec_mx_access_request(p_port->rfc.p_mcb->bd_addr, BT_PSM_RFCOMM, TRUE, BTM_SEC_PROTO_RFCOMM, p_port->dlci / 2, &rfc_sec_check_complete,
                                  p_port);
        return;

    case RFC_EVENT_CLOSE:
        break;

    case RFC_EVENT_CLEAR:
        return;

    case RFC_EVENT_DATA:
        GKI_freebuf(p_data);
        break;

    case RFC_EVENT_SABME:
        p_port->rfc.state = RFC_STATE_TERM_WAIT_SEC_CHECK;
        btm_sec_mx_access_request(p_port->rfc.p_mcb->bd_addr, BT_PSM_RFCOMM, FALSE, BTM_SEC_PROTO_RFCOMM, p_port->dlci / 2, &rfc_sec_check_complete,
                                  p_port);
        return;

    case RFC_EVENT_UA:
        return;

    case RFC_EVENT_DM:
        rfc_port_closed(p_port);
        return;

    case RFC_EVENT_UIH:
        GKI_freebuf(p_data);
        rfc_send_dm(p_port->rfc.p_mcb, p_port->dlci, FALSE);
        return;

    case RFC_EVENT_DISC:
        rfc_send_dm(p_port->rfc.p_mcb, p_port->dlci, FALSE);
        return;

    case RFC_EVENT_TIMEOUT:
        Port_TimeOutCloseMux(p_port->rfc.p_mcb);
        RFCOMM_TRACE(ERROR, "Port error state %d event %d", p_port->rfc.state, event);
        return;
    }

    RFCOMM_TRACE(WARNING, "Port state closed Event ignored %d", event);
    return;
}

static void rfc_port_sm_sabme_wait_ua(tPORT* p_port, UINT16 event, void* p_data) {
    switch (event) {
    case RFC_EVENT_OPEN:
    case RFC_EVENT_ESTABLISH_RSP:
        RFCOMM_TRACE(ERROR, "Port error state %d event %d", p_port->rfc.state, event);
        return;

    case RFC_EVENT_CLOSE:
        rfc_port_timer_start(p_port, RFC_DISC_TIMEOUT);
        rfc_send_disc(p_port->rfc.p_mcb, p_port->dlci);
        p_port->rfc.expected_rsp = 0;
        p_port->rfc.state = RFC_STATE_DISC_WAIT_UA;
        return;

    case RFC_EVENT_CLEAR:
        rfc_port_closed(p_port);
        return;

    case RFC_EVENT_DATA:
        GKI_freebuf(p_data);
        break;

    case RFC_EVENT_UA:
        rfc_port_timer_stop(p_port);
        p_port->rfc.state = RFC_STATE_OPENED;
        PORT_DlcEstablishCnf(p_port->rfc.p_mcb, p_port->dlci, p_port->rfc.p_mcb->peer_l2cap_mtu, RFCOMM_SUCCESS);
        return;

    case RFC_EVENT_DM:
        PORT_DlcEstablishCnf(p_port->rfc.p_mcb, p_port->dlci, p_port->rfc.p_mcb->peer_l2cap_mtu, RFCOMM_ERROR);
        rfc_port_closed(p_port);
        return;

    case RFC_EVENT_DISC:
        rfc_send_ua(p_port->rfc.p_mcb, p_port->dlci);
        PORT_DlcEstablishCnf(p_port->rfc.p_mcb, p_port->dlci, p_port->rfc.p_mcb->peer_l2cap_mtu, RFCOMM_ERROR);
        rfc_port_closed(p_port);
        return;

    case RFC_EVENT_SABME:
        rfc_send_ua(p_port->rfc.p_mcb, p_port->dlci);
        return;

    case RFC_EVENT_UIH:
        GKI_freebuf(p_data);
        return;

    case RFC_EVENT_TIMEOUT:
        p_port->rfc.state = RFC_STATE_CLOSED;
        PORT_DlcEstablishCnf(p_port->rfc.p_mcb, p_port->dlci, p_port->rfc.p_mcb->peer_l2cap_mtu, RFCOMM_ERROR);
        return;
    }

    RFCOMM_TRACE(WARNING, "Port state sabme_wait_ua Event ignored %d", event);
}

static void rfc_port_sm_term_wait_sec_check(tPORT* p_port, UINT16 event, void* p_data) {
    switch (event) {
    case RFC_EVENT_SEC_COMPLETE:
        if (*(UINT8*)p_data != BTM_SUCCESS) {
            if (p_port->rfc.p_mcb) {
                rfc_send_dm(p_port->rfc.p_mcb, p_port->dlci, TRUE);
                port_rfc_closed(p_port, PORT_SEC_FAILED);
            }
        } else {
            PORT_DlcEstablishInd(p_port->rfc.p_mcb, p_port->dlci, p_port->rfc.p_mcb->peer_l2cap_mtu);
        }

        return;

    case RFC_EVENT_OPEN:
    case RFC_EVENT_CLOSE:
        RFCOMM_TRACE(ERROR, "Port error state %d event %d", p_port->rfc.state, event);
        return;

    case RFC_EVENT_CLEAR:
        btm_sec_abort_access_req(p_port->rfc.p_mcb->bd_addr);
        rfc_port_closed(p_port);
        return;

    case RFC_EVENT_DATA:
        RFCOMM_TRACE(ERROR, "Port error state Term Wait Sec event Data");
        GKI_freebuf(p_data);
        return;

    case RFC_EVENT_SABME:
        /* muff1n - I was going to remove this comment as I usually do, since
         * it's not useful at this stage (I plan to put them back in a cleanup
         * pass!), but the use of "dares" here was quite funny so I've left
         * this one in. I have no idea as to whether it is warranted or not, but
         * it made me laugh so I'm keeping it here.
         */
        /* Ignore SABME retransmission if client dares to do so */
        return;

    case RFC_EVENT_DISC:
        btm_sec_abort_access_req(p_port->rfc.p_mcb->bd_addr);
        p_port->rfc.state = RFC_STATE_CLOSED;
        rfc_send_ua(p_port->rfc.p_mcb, p_port->dlci);

        PORT_DlcReleaseInd(p_port->rfc.p_mcb, p_port->dlci);
        return;

    case RFC_EVENT_UIH:
        GKI_freebuf(p_data);
        return;

    case RFC_EVENT_ESTABLISH_RSP:
        if (*(UINT8*)p_data != RFCOMM_SUCCESS) {
            if (p_port->rfc.p_mcb)
                rfc_send_dm(p_port->rfc.p_mcb, p_port->dlci, TRUE);
        } else {
            rfc_send_ua(p_port->rfc.p_mcb, p_port->dlci);
            p_port->rfc.state = RFC_STATE_OPENED;
        }
        return;
    }

    RFCOMM_TRACE(WARNING, "Port state term_wait_sec_check Event ignored %d", event);
}

static void rfc_port_sm_orig_wait_sec_check(tPORT* p_port, UINT16 event, void* p_data) {
    switch (event) {
    case RFC_EVENT_SEC_COMPLETE:
        if (*(UINT8*)p_data != BTM_SUCCESS) {
            PORT_DlcEstablishCnf(p_port->rfc.p_mcb, p_port->dlci, 0, RFCOMM_SECURITY_ERR);
            rfc_port_closed(p_port);
            return;
        }

        rfc_send_sabme(p_port->rfc.p_mcb, p_port->dlci);
        rfc_port_timer_start(p_port, RFC_PORT_T1_TIMEOUT);
        p_port->rfc.state = RFC_STATE_SABME_WAIT_UA;
        return;

    case RFC_EVENT_OPEN:
    case RFC_EVENT_SABME:
        RFCOMM_TRACE(ERROR, "Port error state %d event %d", p_port->rfc.state, event);
        return;

    case RFC_EVENT_CLOSE:
        btm_sec_abort_access_req(p_port->rfc.p_mcb->bd_addr);
        rfc_port_closed(p_port);
        return;

    case RFC_EVENT_DATA:
        RFCOMM_TRACE(ERROR, "Port error state Orig Wait Sec event Data");
        GKI_freebuf(p_data);
        return;

    case RFC_EVENT_UIH:
        GKI_freebuf(p_data);
        return;
    }

    RFCOMM_TRACE(WARNING, "Port state orig_wait_sec_check Event ignored %d", event);
}

static void rfc_port_sm_opened(tPORT* p_port, UINT16 event, void* p_data) {
    switch (event) {
    case RFC_EVENT_OPEN:
        RFCOMM_TRACE(ERROR, "Port error state %d event %d", p_port->rfc.state, event);
        return;

    case RFC_EVENT_CLOSE:
        rfc_port_timer_start(p_port, RFC_DISC_TIMEOUT);
        rfc_send_disc(p_port->rfc.p_mcb, p_port->dlci);
        p_port->rfc.expected_rsp = 0;
        p_port->rfc.state = RFC_STATE_DISC_WAIT_UA;
        return;

    case RFC_EVENT_CLEAR:
        rfc_port_closed(p_port);
        return;

    case RFC_EVENT_DATA:
        if (p_port->rfc.p_mcb->flow == PORT_FC_CREDIT && ((BT_HDR*)p_data)->len < p_port->peer_mtu && !p_port->rx.user_fc &&
            p_port->credit_rx_max > p_port->credit_rx) {
            ((BT_HDR*)p_data)->layer_specific = (UINT8)(p_port->credit_rx_max - p_port->credit_rx);
            p_port->credit_rx = p_port->credit_rx_max;
        } else {
            ((BT_HDR*)p_data)->layer_specific = 0;
        }

        rfc_send_buf_uih(p_port->rfc.p_mcb, p_port->dlci, p_data);
        rfc_dec_credit(p_port);
        return;

    case RFC_EVENT_UA:
        return;

    case RFC_EVENT_SABME:
        rfc_send_ua(p_port->rfc.p_mcb, p_port->dlci);
        return;

    case RFC_EVENT_DM:
        PORT_DlcReleaseInd(p_port->rfc.p_mcb, p_port->dlci);
        rfc_port_closed(p_port);
        return;

    case RFC_EVENT_DISC:
        p_port->rfc.state = RFC_STATE_CLOSED;
        rfc_send_ua(p_port->rfc.p_mcb, p_port->dlci);

        PORT_DlcReleaseInd(p_port->rfc.p_mcb, p_port->dlci);

        return;

    case RFC_EVENT_UIH:
        rfc_port_uplink_data(p_port, p_data);
        return;

    case RFC_EVENT_TIMEOUT:
        Port_TimeOutCloseMux(p_port->rfc.p_mcb);
        RFCOMM_TRACE(ERROR, "Port error state %d event %d", p_port->rfc.state, event);
        return;
    }

    RFCOMM_TRACE(WARNING, "Port state opened Event ignored %d", event);
}

static void rfc_port_sm_disc_wait_ua(tPORT* p_port, UINT16 event, void* p_data) {
    switch (event) {
    case RFC_EVENT_OPEN:
    case RFC_EVENT_ESTABLISH_RSP:
        RFCOMM_TRACE(ERROR, "Port error state %d event %d", p_port->rfc.state, event);
        return;

    case RFC_EVENT_CLEAR:
        rfc_port_closed(p_port);
        return;

    case RFC_EVENT_DATA:
        GKI_freebuf(p_data);
        return;

    case RFC_EVENT_UA:
        p_port->rfc.p_mcb->is_disc_initiator = TRUE;
        /* fallthrough */

    case RFC_EVENT_DM:
        rfc_port_closed(p_port);
        return;

    case RFC_EVENT_SABME:
        rfc_send_dm(p_port->rfc.p_mcb, p_port->dlci, TRUE);
        return;

    case RFC_EVENT_DISC:
        rfc_send_dm(p_port->rfc.p_mcb, p_port->dlci, TRUE);
        return;

    case RFC_EVENT_UIH:
        GKI_freebuf(p_data);
        rfc_send_dm(p_port->rfc.p_mcb, p_port->dlci, FALSE);
        return;

    case RFC_EVENT_TIMEOUT:
        rfc_port_closed(p_port);
        return;
    }

    RFCOMM_TRACE(WARNING, "Port state disc_wait_ua Event ignored %d", event);
}

static void rfc_port_uplink_data(tPORT* p_port, BT_HDR* p_buf) {
    PORT_DataInd(p_port->rfc.p_mcb, p_port->dlci, p_buf);
}

void rfc_process_pn(tRFC_MCB* p_mcb, BOOLEAN is_command, MX_FRAME* p_frame) {
    tPORT* p_port;
    UINT8 dlci = p_frame->dlci;

    if (is_command) {
        if (p_mcb->state != RFC_MX_STATE_DISC_WAIT_UA) {
            PORT_ParNegInd(p_mcb, dlci, p_frame->u.pn.mtu, p_frame->u.pn.conv_layer, p_frame->u.pn.k);
        } else {
            rfc_send_dm(p_mcb, dlci, FALSE);
            RFCOMM_TRACE(WARNING, "***** MX PN while disconnecting *****");
        }

        return;
    }

    p_port = port_find_mcb_dlci_port(p_mcb, dlci);
    if (!p_port || !(p_port->rfc.expected_rsp & RFC_RSP_PN))
        return;

    p_port->rfc.expected_rsp &= ~RFC_RSP_PN;

    rfc_port_timer_stop(p_port);

    PORT_ParNegCnf(p_mcb, dlci, p_frame->u.pn.mtu, p_frame->u.pn.conv_layer, p_frame->u.pn.k);
}

void rfc_process_rpn(tRFC_MCB* p_mcb, BOOLEAN is_command, BOOLEAN is_request, MX_FRAME* p_frame) {
    tPORT_STATE port_pars;
    tPORT* p_port;

    if ((p_port = port_find_mcb_dlci_port(p_mcb, p_frame->dlci)) == NULL) {
        if (is_command) {
            memset(&port_pars, 0, sizeof(tPORT_STATE));
            rfc_set_port_state(&port_pars, p_frame);

            PORT_PortNegInd(p_mcb, p_frame->dlci, &port_pars, p_frame->u.rpn.param_mask);
        }
        return;
    }

    if (is_command && is_request) {
        port_pars = p_port->peer_port_pars;
        rfc_send_rpn(p_mcb, p_frame->dlci, FALSE, &p_port->peer_port_pars, 0);
        return;
    }

    port_pars = p_port->peer_port_pars;

    rfc_set_port_state(&port_pars, p_frame);

    if (is_command) {
        PORT_PortNegInd(p_mcb, p_frame->dlci, &port_pars, p_frame->u.rpn.param_mask);
        return;
    }

    p_port = port_find_mcb_dlci_port(p_mcb, p_frame->dlci);
    if (!p_port || !(p_port->rfc.expected_rsp & (RFC_RSP_RPN | RFC_RSP_RPN_REPLY)))
        return;

    rfc_port_timer_stop(p_port);

    if (p_port->rfc.expected_rsp & RFC_RSP_RPN_REPLY) {
        p_port->rfc.expected_rsp &= ~RFC_RSP_RPN_REPLY;

        p_port->peer_port_pars = port_pars;

        if (port_pars.fc_type == (RFCOMM_FC_RTR_ON_INPUT | RFCOMM_FC_RTR_ON_OUTPUT) ||
            port_pars.fc_type == (RFCOMM_FC_RTC_ON_INPUT | RFCOMM_FC_RTC_ON_OUTPUT)) {
            p_frame->u.rpn.param_mask = RFCOMM_RPN_PM_MASK;
        } else {
            p_port->peer_port_pars.fc_type = (RFCOMM_FC_RTR_ON_INPUT | RFCOMM_FC_RTR_ON_OUTPUT);

            p_port->rfc.expected_rsp |= RFC_RSP_RPN;
            rfc_send_rpn(p_mcb, p_frame->dlci, TRUE, &p_port->peer_port_pars, RFCOMM_RPN_PM_RTR_ON_INPUT | RFCOMM_RPN_PM_RTR_ON_OUTPUT);
            rfc_port_timer_start(p_port, RFC_T2_TIMEOUT);
            return;
        }
    } else
        p_port->rfc.expected_rsp &= ~RFC_RSP_RPN;

    if ((p_frame->u.rpn.param_mask & (RFCOMM_RPN_PM_RTR_ON_INPUT | RFCOMM_RPN_PM_RTR_ON_OUTPUT)) ==
            (RFCOMM_RPN_PM_RTR_ON_INPUT | RFCOMM_RPN_PM_RTR_ON_OUTPUT) ||
        (p_frame->u.rpn.param_mask & (RFCOMM_RPN_PM_RTC_ON_INPUT | RFCOMM_RPN_PM_RTC_ON_OUTPUT)) ==
            (RFCOMM_RPN_PM_RTC_ON_INPUT | RFCOMM_RPN_PM_RTC_ON_OUTPUT)) {
        PORT_PortNegCnf(p_mcb, p_port->dlci, &port_pars, RFCOMM_SUCCESS);
        return;
    }

    if (p_port->peer_port_pars.fc_type == (RFCOMM_FC_RTR_ON_INPUT | RFCOMM_FC_RTR_ON_OUTPUT)) {
        p_port->peer_port_pars.fc_type = (RFCOMM_FC_RTC_ON_INPUT | RFCOMM_FC_RTC_ON_OUTPUT);

        p_port->rfc.expected_rsp |= RFC_RSP_RPN;

        rfc_send_rpn(p_mcb, p_frame->dlci, TRUE, &p_port->peer_port_pars, RFCOMM_RPN_PM_RTC_ON_INPUT | RFCOMM_RPN_PM_RTC_ON_OUTPUT);
        rfc_port_timer_start(p_port, RFC_T2_TIMEOUT);
        return;
    }

    if (p_port->peer_port_pars.fc_type == (RFCOMM_FC_RTC_ON_INPUT | RFCOMM_FC_RTC_ON_OUTPUT)) {
        p_port->peer_port_pars.fc_type = RFCOMM_FC_OFF;
        PORT_PortNegCnf(p_mcb, p_port->dlci, &port_pars, RFCOMM_SUCCESS);
    }
}

void rfc_process_msc(tRFC_MCB* p_mcb, BOOLEAN is_command, MX_FRAME* p_frame) {
    tPORT_CTRL pars;
    tPORT* p_port;
    UINT8 modem_signals = p_frame->u.msc.signals;
    BOOLEAN new_peer_fc = FALSE;

    p_port = port_find_mcb_dlci_port(p_mcb, p_frame->dlci);
    if (!p_port)
        return;

    pars.modem_signal = 0;

    if (modem_signals & RFCOMM_MSC_RTC)
        pars.modem_signal |= MODEM_SIGNAL_DTRDSR;

    if (modem_signals & RFCOMM_MSC_RTR)
        pars.modem_signal |= MODEM_SIGNAL_RTSCTS;

    if (modem_signals & RFCOMM_MSC_IC)
        pars.modem_signal |= MODEM_SIGNAL_RI;

    if (modem_signals & RFCOMM_MSC_DV)
        pars.modem_signal |= MODEM_SIGNAL_DCD;

    pars.fc = (modem_signals & RFCOMM_MSC_FC) == RFCOMM_MSC_FC;

    pars.break_signal = p_frame->u.msc.break_present ? p_frame->u.msc.break_duration : 0;
    pars.discard_buffers = 0;
    pars.break_signal_seq = RFCOMM_CTRL_BREAK_IN_SEQ;

    if (is_command) {
        rfc_send_msc(p_mcb, p_frame->dlci, FALSE, &pars);

        if (p_port->rfc.p_mcb->flow != PORT_FC_CREDIT) {
            p_port->peer_ctrl.fc = new_peer_fc = pars.fc;

            if (new_peer_fc != p_port->tx.peer_fc)
                PORT_FlowInd(p_mcb, p_frame->dlci, !new_peer_fc);
        }

        PORT_ControlInd(p_mcb, p_frame->dlci, &pars);

        return;
    }

    if (!(p_port->rfc.expected_rsp & RFC_RSP_MSC))
        return;

    p_port->rfc.expected_rsp &= ~RFC_RSP_MSC;

    rfc_port_timer_stop(p_port);

    PORT_ControlCnf(p_port->rfc.p_mcb, p_port->dlci, &pars);
}

void rfc_process_rls(tRFC_MCB* p_mcb, BOOLEAN is_command, MX_FRAME* p_frame) {
    tPORT* p_port;

    if (is_command) {
        PORT_LineStatusInd(p_mcb, p_frame->dlci, p_frame->u.rls.line_status);
        rfc_send_rls(p_mcb, p_frame->dlci, FALSE, p_frame->u.rls.line_status);
    } else {
        p_port = port_find_mcb_dlci_port(p_mcb, p_frame->dlci);

        if (!p_port || !(p_port->rfc.expected_rsp & RFC_RSP_RLS))
            return;

        p_port->rfc.expected_rsp &= ~RFC_RSP_RLS;

        rfc_port_timer_stop(p_port);
    }
}

void rfc_process_nsc(tRFC_MCB* p_mcb, MX_FRAME* p_frame) {
    /* ... */
}

void rfc_process_test_rsp(tRFC_MCB* p_mcb, BT_HDR* p_buf) {
    GKI_freebuf(p_buf);
}

void rfc_process_fcon(tRFC_MCB* p_mcb, BOOLEAN is_command) {
    if (is_command) {
        rfc_cb.rfc.peer_rx_disabled = FALSE;

        rfc_send_fcon(p_mcb, FALSE);

        if (!p_mcb->l2cap_congested)
            PORT_FlowInd(p_mcb, 0, TRUE);
    }
}

void rfc_process_fcoff(tRFC_MCB* p_mcb, BOOLEAN is_command) {
    if (is_command) {
        rfc_cb.rfc.peer_rx_disabled = TRUE;

        if (!p_mcb->l2cap_congested)
            PORT_FlowInd(p_mcb, 0, FALSE);

        rfc_send_fcoff(p_mcb, FALSE);
    }
}

void rfc_process_l2cap_congestion(tRFC_MCB* p_mcb, BOOLEAN is_congested) {
    p_mcb->l2cap_congested = is_congested;

    if (!is_congested)
        rfc_check_send_cmd(p_mcb, NULL);

    if (!rfc_cb.rfc.peer_rx_disabled) {
        if (!is_congested)
            PORT_FlowInd(p_mcb, 0, TRUE);
        else
            PORT_FlowInd(p_mcb, 0, FALSE);
    }
}

static void rfc_set_port_state(tPORT_STATE* port_pars, MX_FRAME* p_frame) {
    if (p_frame->u.rpn.param_mask & RFCOMM_RPN_PM_BIT_RATE)
        port_pars->baud_rate = p_frame->u.rpn.baud_rate;

    if (p_frame->u.rpn.param_mask & RFCOMM_RPN_PM_DATA_BITS)
        port_pars->byte_size = p_frame->u.rpn.byte_size;

    if (p_frame->u.rpn.param_mask & RFCOMM_RPN_PM_STOP_BITS)
        port_pars->stop_bits = p_frame->u.rpn.stop_bits;

    if (p_frame->u.rpn.param_mask & RFCOMM_RPN_PM_PARITY)
        port_pars->parity = p_frame->u.rpn.parity;

    if (p_frame->u.rpn.param_mask & RFCOMM_RPN_PM_PARITY_TYPE)
        port_pars->parity_type = p_frame->u.rpn.parity_type;

    if (p_frame->u.rpn.param_mask & (RFCOMM_RPN_PM_XONXOFF_ON_INPUT | RFCOMM_RPN_PM_XONXOFF_ON_OUTPUT | RFCOMM_RPN_PM_RTR_ON_INPUT |
                                     RFCOMM_RPN_PM_RTR_ON_OUTPUT | RFCOMM_RPN_PM_RTC_ON_INPUT | RFCOMM_RPN_PM_RTC_ON_OUTPUT)) {
        port_pars->fc_type = p_frame->u.rpn.fc_type;
    }

    if (p_frame->u.rpn.param_mask & RFCOMM_RPN_PM_XON_CHAR)
        port_pars->xon_char = p_frame->u.rpn.xon_char;

    if (p_frame->u.rpn.param_mask & RFCOMM_RPN_PM_XOFF_CHAR)
        port_pars->xoff_char = p_frame->u.rpn.xoff_char;
}
