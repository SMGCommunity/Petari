#include "bt_target.h"
#include "bt_trace.h"
#include "bt_types.h"
#include "btu.h"
#include "data_types.h"
#include "gki.h"
#include "l2c_api.h"
#include "port_api.h"
#include "port_int.h"
#include "rfc_int.h"
#include "rfcdefs.h"

#include "revolution/types.h"
#include <cstring>

/* Original source:
 * bluedroid <android.googlesource.com/platform/external/bluetooth/bluedroid>
 * stack/rfcomm/rfc_utils.c
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
 * variables
 */

// .rodata
static UINT8 const rfc_crctable[256] = {
    0x00, 0x91, 0xe3, 0x72, 0x07, 0x96, 0xe4, 0x75, 0x0e, 0x9f, 0xed, 0x7c, 0x09, 0x98, 0xea, 0x7b, 0x1c, 0x8d, 0xff, 0x6e, 0x1b, 0x8a, 0xf8, 0x69,
    0x12, 0x83, 0xf1, 0x60, 0x15, 0x84, 0xf6, 0x67, 0x38, 0xa9, 0xdb, 0x4a, 0x3f, 0xae, 0xdc, 0x4d, 0x36, 0xa7, 0xd5, 0x44, 0x31, 0xa0, 0xd2, 0x43,
    0x24, 0xb5, 0xc7, 0x56, 0x23, 0xb2, 0xc0, 0x51, 0x2a, 0xbb, 0xc9, 0x58, 0x2d, 0xbc, 0xce, 0x5f, 0x70, 0xe1, 0x93, 0x02, 0x77, 0xe6, 0x94, 0x05,
    0x7e, 0xef, 0x9d, 0x0c, 0x79, 0xe8, 0x9a, 0x0b, 0x6c, 0xfd, 0x8f, 0x1e, 0x6b, 0xfa, 0x88, 0x19, 0x62, 0xf3, 0x81, 0x10, 0x65, 0xf4, 0x86, 0x17,
    0x48, 0xd9, 0xab, 0x3a, 0x4f, 0xde, 0xac, 0x3d, 0x46, 0xd7, 0xa5, 0x34, 0x41, 0xd0, 0xa2, 0x33, 0x54, 0xc5, 0xb7, 0x26, 0x53, 0xc2, 0xb0, 0x21,
    0x5a, 0xcb, 0xb9, 0x28, 0x5d, 0xcc, 0xbe, 0x2f, 0xe0, 0x71, 0x03, 0x92, 0xe7, 0x76, 0x04, 0x95, 0xee, 0x7f, 0x0d, 0x9c, 0xe9, 0x78, 0x0a, 0x9b,
    0xfc, 0x6d, 0x1f, 0x8e, 0xfb, 0x6a, 0x18, 0x89, 0xf2, 0x63, 0x11, 0x80, 0xf5, 0x64, 0x16, 0x87, 0xd8, 0x49, 0x3b, 0xaa, 0xdf, 0x4e, 0x3c, 0xad,
    0xd6, 0x47, 0x35, 0xa4, 0xd1, 0x40, 0x32, 0xa3, 0xc4, 0x55, 0x27, 0xb6, 0xc3, 0x52, 0x20, 0xb1, 0xca, 0x5b, 0x29, 0xb8, 0xcd, 0x5c, 0x2e, 0xbf,
    0x90, 0x01, 0x73, 0xe2, 0x97, 0x06, 0x74, 0xe5, 0x9e, 0x0f, 0x7d, 0xec, 0x99, 0x08, 0x7a, 0xeb, 0x8c, 0x1d, 0x6f, 0xfe, 0x8b, 0x1a, 0x68, 0xf9,
    0x82, 0x13, 0x61, 0xf0, 0x85, 0x14, 0x66, 0xf7, 0xa8, 0x39, 0x4b, 0xda, 0xaf, 0x3e, 0x4c, 0xdd, 0xa6, 0x37, 0x45, 0xd4, 0xa1, 0x30, 0x42, 0xd3,
    0xb4, 0x25, 0x57, 0xc6, 0xb3, 0x22, 0x50, 0xc1, 0xba, 0x2b, 0x59, 0xc8, 0xbd, 0x2c, 0x5e, 0xcf,
};

/*******************************************************************************
 * functions
 */

UINT8 rfc_calc_fcs(UINT16 len, UINT8* p) {
    UINT8 fcs = 0xff;

    while (len--)
        fcs = rfc_crctable[fcs ^ *p++];

    return 0xff - fcs;
}

BOOLEAN rfc_check_fcs(UINT16 len, UINT8* p, UINT8 received_fcs) {
    UINT8 fcs = 0xff;

    while (len--)
        fcs = rfc_crctable[fcs ^ *p++];

    fcs = rfc_crctable[fcs ^ received_fcs];

    return fcs == 0xcf;
}

tRFC_MCB* rfc_alloc_multiplexer_channel(BD_ADDR bd_addr, BOOLEAN is_initiator) {
    int i;
    int j;
    tRFC_MCB* p_mcb = NULL;

    for (i = 0; i < MAX_BD_CONNECTIONS; ++i) {
        if (rfc_cb.port.rfc_mcb[i].state != RFC_MX_STATE_IDLE && memcmp(rfc_cb.port.rfc_mcb[i].bd_addr, bd_addr, BD_ADDR_LEN) == 0) {
            rfc_timer_stop(&rfc_cb.port.rfc_mcb[i]);

            return &rfc_cb.port.rfc_mcb[i];
        }
    }

    /* explicitly post-increment */
    for (i = 0, j = rfc_cb.rfc.last_mux + 1; i < MAX_BD_CONNECTIONS; i++, j++) {
        if (j >= MAX_BD_CONNECTIONS)
            j = 0;

        p_mcb = &rfc_cb.port.rfc_mcb[j];

        if (rfc_cb.port.rfc_mcb[j].state == RFC_MX_STATE_IDLE) {
            memset(p_mcb, 0, sizeof *p_mcb);
            memcpy(p_mcb->bd_addr, bd_addr, BD_ADDR_LEN);

            GKI_init_q(&p_mcb->cmd_q);

            p_mcb->is_initiator = is_initiator;

            rfc_timer_start(p_mcb, RFC_MCB_INIT_INACT_TIMER);

            rfc_cb.rfc.last_mux = j;
            return p_mcb;
        }
    }

    return NULL;
}

void rfc_release_multiplexer_channel(tRFC_MCB* p_mcb) {
    void* p_buf;

    rfc_timer_stop(p_mcb);

    while ((p_buf = GKI_dequeue(&p_mcb->cmd_q)) != NULL)
        GKI_freebuf(p_buf);

    memset(p_mcb, 0, sizeof *p_mcb);
    p_mcb->state = RFC_MX_STATE_IDLE;
}

void rfc_timer_start(tRFC_MCB* p_mcb, UINT16 timeout) {
    TIMER_LIST_ENT* p_tle = &p_mcb->tle;

    RFCOMM_TRACE(EVENT, "rfc_timer_start - timeout:%d", timeout);

    p_tle->param = (TIMER_PARAM_TYPE)p_mcb;

    btu_start_timer(p_tle, BTU_TTYPE_RFCOMM_MFC, timeout);
}

void rfc_timer_stop(tRFC_MCB* p_mcb) {
    RFCOMM_TRACE(EVENT, "rfc_timer_stop");

    btu_stop_timer(&p_mcb->tle);
}

void rfc_port_timer_start(tPORT* p_port, UINT16 timeout) {
    TIMER_LIST_ENT* p_tle = &p_port->rfc.tle;

    RFCOMM_TRACE(EVENT, "rfc_port_timer_start - timeout:%d", timeout);

    p_tle->param = (TIMER_PARAM_TYPE)p_port;

    btu_start_timer(p_tle, BTU_TTYPE_RFCOMM_PORT, timeout);
}

void rfc_port_timer_stop(tPORT* p_port) {
    RFCOMM_TRACE(EVENT, "rfc_port_timer_stop");

    btu_stop_timer(&p_port->rfc.tle);
}

void rfc_check_mcb_active(tRFC_MCB* p_mcb) {
    UINT16 i;

    for (i = 0; i < RFCOMM_MAX_DLCI; ++i) {
        if (p_mcb->port_inx[i]) {
            p_mcb->is_disc_initiator = FALSE;
            return;
        }
    }

    if (p_mcb->is_disc_initiator) {
        p_mcb->is_disc_initiator = FALSE;
        rfc_mx_sm_execute(p_mcb, RFC_MX_EVENT_CLOSE_REQ, NULL);
    } else {
        rfc_timer_start(p_mcb, RFC_MCB_RELEASE_INACT_TIMER);
    }
}

void rfcomm_process_timeout(TIMER_LIST_ENT* p_tle) {
    switch (p_tle->event) {
    case BTU_TTYPE_RFCOMM_MFC:
        rfc_mx_sm_execute((tRFC_MCB*)p_tle->param, RFC_EVENT_TIMEOUT, NULL);
        break;

    case BTU_TTYPE_RFCOMM_PORT:
        rfc_port_sm_execute((tPORT*)p_tle->param, RFC_EVENT_TIMEOUT, NULL);
        break;

    default:
        break;
    }
}

void rfc_sec_check_complete(BD_ADDR bd_addr, void* p_ref_data, UINT8 res) {
    tPORT* p_port = p_ref_data;

    if (!p_port->in_use || (p_port->rfc.state != RFC_STATE_ORIG_WAIT_SEC_CHECK && p_port->rfc.state != RFC_STATE_TERM_WAIT_SEC_CHECK)) {
        return;
    }

    rfc_port_sm_execute(p_ref_data, RFC_EVENT_SEC_COMPLETE, &res);
}

void rfc_port_closed(tPORT* p_port) {
    tRFC_MCB* p_mcb = p_port->rfc.p_mcb;

    RFCOMM_TRACE(DEBUG, "rfc_port_closed");

    rfc_port_timer_stop(p_port);

    p_port->rfc.state = RFC_STATE_CLOSED;

    if (p_mcb) {
        p_mcb->port_inx[p_port->dlci] = 0;
        p_port->dlci = 0;

        rfc_check_mcb_active(p_mcb);
    }

    port_rfc_closed(p_port, PORT_CLOSED);
}

void rfc_inc_credit(tPORT* p_port, UINT8 credit) {
    if (p_port->rfc.p_mcb->flow == PORT_FC_CREDIT) {
        p_port->credit_tx += credit;

        RFCOMM_TRACE(EVENT, "rfc_inc_credit:%d", p_port->credit_tx);

        if (p_port->tx.peer_fc == TRUE)
            PORT_FlowInd(p_port->rfc.p_mcb, p_port->dlci, TRUE);
    }
}

void rfc_dec_credit(tPORT* p_port) {
    if (p_port->rfc.p_mcb->flow == PORT_FC_CREDIT) {
        if (p_port->credit_tx > 0)
            --p_port->credit_tx;

        if (p_port->credit_tx == 0)
            p_port->tx.peer_fc = TRUE;
    }
}

void rfc_check_send_cmd(tRFC_MCB* p_mcb, BT_HDR* p_buf) {
    BT_HDR* p;

    if (p_buf)
        GKI_enqueue(&p_mcb->cmd_q, p_buf);

    while (!p_mcb->l2cap_congested) {
        if ((p = GKI_dequeue(&p_mcb->cmd_q)) == NULL)
            break;

        L2CA_DataWrite(p_mcb->lcid, p);
    }
}
