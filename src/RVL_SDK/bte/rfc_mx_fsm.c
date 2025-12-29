#include "bt_target.h"
#include "gki.h"
#include "rfc_int.h"

/* Original source:
 * bluedroid <android.googlesource.com/platform/external/bluetooth/bluedroid>
 * stack/rfcomm/rfc_mx_fsm.c
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

#include "bt_trace.h"
#include "bt_types.h"
#include "data_types.h"

#include "l2c_api.h"
#include "l2cdefs.h"
#include "port_int.h"
#include "rfcdefs.h"

/*******************************************************************************
 * macros
 */

#define L2CAP_SUCCESS 0

/*******************************************************************************
 * local function declarations
 */

static void rfc_mx_sm_state_idle(tRFC_MCB* p_mcb, UINT16 event, void* p_data);
static void rfc_mx_sm_state_wait_conn_cnf(tRFC_MCB* p_mcb, UINT16 event, void* p_data);
static void rfc_mx_sm_state_configure(tRFC_MCB* p_mcb, UINT16 event, void* p_data);
static void rfc_mx_sm_sabme_wait_ua(tRFC_MCB* p_mcb, UINT16 event, void* p_data);
static void rfc_mx_sm_state_wait_sabme(tRFC_MCB* p_mcb, UINT16 event, void* p_data);
static void rfc_mx_sm_state_connected(tRFC_MCB* p_mcb, UINT16 event, void* p_data);
static void rfc_mx_sm_state_disc_wait_ua(tRFC_MCB* p_mcb, UINT16 event, void* p_data);
static void rfc_mx_send_config_req(tRFC_MCB* p_mcb);
static void rfc_mx_conf_cnf(tRFC_MCB* p_mcb, tL2CAP_CFG_INFO* p_cfg);
static void rfc_mx_conf_ind(tRFC_MCB* p_mcb, tL2CAP_CFG_INFO* p_cfg);

/*******************************************************************************
 * functions
 */

void rfc_mx_sm_execute(tRFC_MCB* p_mcb, UINT16 event, void* p_data) {
    switch (p_mcb->state) {
    case RFC_MX_STATE_IDLE:
        rfc_mx_sm_state_idle(p_mcb, event, p_data);
        break;

    case RFC_MX_STATE_WAIT_CONN_CNF:
        rfc_mx_sm_state_wait_conn_cnf(p_mcb, event, p_data);
        break;

    case RFC_MX_STATE_CONFIGURE:
        rfc_mx_sm_state_configure(p_mcb, event, p_data);
        break;

    case RFC_MX_STATE_SABME_WAIT_UA:
        rfc_mx_sm_sabme_wait_ua(p_mcb, event, p_data);
        break;

    case RFC_MX_STATE_WAIT_SABME:
        rfc_mx_sm_state_wait_sabme(p_mcb, event, p_data);
        break;

    case RFC_MX_STATE_CONNECTED:
        rfc_mx_sm_state_connected(p_mcb, event, p_data);
        break;

    case RFC_MX_STATE_DISC_WAIT_UA:
        rfc_mx_sm_state_disc_wait_ua(p_mcb, event, p_data);
        break;
    }
}

static void rfc_mx_sm_state_idle(tRFC_MCB* p_mcb, UINT16 event, void* p_data) {
    RFCOMM_TRACE(EVENT, "rfc_mx_sm_state_idle - evt:%d", event);

    switch (event) {
    case RFC_MX_EVENT_START_REQ:
        p_mcb->peer_l2cap_mtu = L2CAP_DEFAULT_MTU - RFCOMM_MIN_OFFSET - 1;

        if ((p_mcb->lcid = L2CA_ConnectReq(BT_PSM_RFCOMM, p_mcb->bd_addr)) == 0) {
            PORT_StartCnf(p_mcb, RFCOMM_ERROR);
            return;
        }

        rfc_save_lcid_mcb(p_mcb, p_mcb->lcid);

        p_mcb->state = RFC_MX_STATE_WAIT_CONN_CNF;
        return;

    case RFC_MX_EVENT_START_RSP:
    case RFC_MX_EVENT_CONN_CNF:
    case RFC_MX_EVENT_CONF_IND:
    case RFC_MX_EVENT_CONF_CNF:
        RFCOMM_TRACE(ERROR, "Mx error state %d event %d", p_mcb->state, event);
        return;

    case RFC_MX_EVENT_CONN_IND:
        if (p_mcb->is_initiator) {
            L2CA_ConnectRsp(p_mcb->bd_addr, *(UINT8*)p_data, p_mcb->lcid, L2CAP_CONN_PENDING, 0);
            return;
        }

        rfc_timer_start(p_mcb, RFCOMM_CONN_TIMEOUT);
        L2CA_ConnectRsp(p_mcb->bd_addr, *(UINT8*)p_data, p_mcb->lcid, L2CAP_CONN_OK, 0);

        rfc_mx_send_config_req(p_mcb);

        p_mcb->state = RFC_MX_STATE_CONFIGURE;

        return;

    case RFC_EVENT_SABME:
        break;

    case RFC_EVENT_UA:
    case RFC_EVENT_DM:
        return;

    case RFC_EVENT_DISC:
        rfc_send_dm(p_mcb, RFCOMM_MX_DLCI, TRUE);
        return;

    case RFC_EVENT_UIH:
        rfc_send_dm(p_mcb, RFCOMM_MX_DLCI, FALSE);
        return;
    }

    RFCOMM_TRACE(EVENT, "RFCOMM MX ignored - evt:%d in state:%d", event, p_mcb->state);
}

static void rfc_mx_sm_state_wait_conn_cnf(tRFC_MCB* p_mcb, UINT16 event, void* p_data) {
    RFCOMM_TRACE(EVENT, "rfc_mx_sm_state_wait_conn_cnf - evt:%d", event);

    switch (event) {
    case RFC_MX_EVENT_START_REQ:
        RFCOMM_TRACE(ERROR, "Mx error state %d event %d", p_mcb->state, event);
        return;

    case RFC_MX_EVENT_CONN_CNF:
        if (*(UINT16*)p_data != L2CAP_SUCCESS) {
            p_mcb->state = RFC_MX_STATE_IDLE;

            PORT_StartCnf(p_mcb, *(UINT16*)p_data);
            return;
        }

        p_mcb->state = RFC_MX_STATE_CONFIGURE;
        rfc_mx_send_config_req(p_mcb);
        return;

    case RFC_MX_EVENT_DISC_IND:
        p_mcb->state = RFC_MX_STATE_IDLE;
        PORT_CloseInd(p_mcb);
        return;

    case RFC_EVENT_TIMEOUT:
        p_mcb->state = RFC_MX_STATE_IDLE;
        L2CA_DisconnectReq(p_mcb->lcid);

        PORT_CloseInd(p_mcb);

        return;
    }

    RFCOMM_TRACE(EVENT, "RFCOMM MX ignored - evt:%d in state:%d", event, p_mcb->state);
}

static void rfc_mx_sm_state_configure(tRFC_MCB* p_mcb, UINT16 event, void* p_data) {
    RFCOMM_TRACE(EVENT, "rfc_mx_sm_state_configure - evt:%d", event);

    switch (event) {
    case RFC_MX_EVENT_START_REQ:
    case RFC_MX_EVENT_CONN_CNF:
        RFCOMM_TRACE(ERROR, "Mx error state %d event %d", p_mcb->state, event);
        return;

    case RFC_MX_EVENT_CONF_IND:
        rfc_mx_conf_ind(p_mcb, (tL2CAP_CFG_INFO*)p_data);
        return;

    case RFC_MX_EVENT_CONF_CNF:
        rfc_mx_conf_cnf(p_mcb, (tL2CAP_CFG_INFO*)p_data);
        return;

    case RFC_MX_EVENT_DISC_IND:
        p_mcb->state = RFC_MX_STATE_IDLE;
        PORT_CloseInd(p_mcb);
        return;
    }

    RFCOMM_TRACE(EVENT, "RFCOMM MX ignored - evt:%d in state:%d", event, p_mcb->state);
}

static void rfc_mx_sm_sabme_wait_ua(tRFC_MCB* p_mcb, UINT16 event, void* p_data) {
    RFCOMM_TRACE(EVENT, "rfc_mx_sm_sabme_wait_ua - evt:%d", event);

    switch (event) {
    case RFC_MX_EVENT_START_REQ:
    case RFC_MX_EVENT_CONN_CNF:
        RFCOMM_TRACE(ERROR, "Mx error state %d event %d", p_mcb->state, event);
        return;

    case RFC_MX_EVENT_CONF_IND:
        rfc_mx_conf_ind(p_mcb, p_data);
        return;

    case RFC_MX_EVENT_CONF_CNF:
        rfc_mx_conf_cnf(p_mcb, p_data);
        return;

    case RFC_MX_EVENT_DISC_IND:
        p_mcb->state = RFC_MX_STATE_IDLE;
        PORT_CloseInd(p_mcb);
        return;

    case RFC_EVENT_UA:
        rfc_timer_stop(p_mcb);

        p_mcb->state = RFC_MX_STATE_CONNECTED;
        p_mcb->peer_ready = TRUE;

        PORT_StartCnf(p_mcb, RFCOMM_SUCCESS);
        return;

    case RFC_EVENT_DM:
        rfc_timer_stop(p_mcb);
        /* fallthrough */

    case RFC_EVENT_TIMEOUT:
        p_mcb->state = RFC_MX_STATE_IDLE;
        L2CA_DisconnectReq(p_mcb->lcid);

        PORT_StartCnf(p_mcb, RFCOMM_ERROR);
        return;
    }

    RFCOMM_TRACE(EVENT, "RFCOMM MX ignored - evt:%d in state:%d", event, p_mcb->state);
}

static void rfc_mx_sm_state_wait_sabme(tRFC_MCB* p_mcb, UINT16 event, void* p_data) {
    RFCOMM_TRACE(EVENT, "rfc_mx_sm_state_wait_sabme - evt:%d", event);

    switch (event) {
    case RFC_MX_EVENT_DISC_IND:
        p_mcb->state = RFC_MX_STATE_IDLE;
        PORT_CloseInd(p_mcb);
        return;

    case RFC_EVENT_SABME:
        PORT_StartInd(p_mcb);
        return;

    case RFC_MX_EVENT_START_RSP:
        if (*(UINT16*)p_data != RFCOMM_SUCCESS) {
            rfc_send_dm(p_mcb, RFCOMM_MX_DLCI, TRUE);
        } else {
            rfc_send_ua(p_mcb, RFCOMM_MX_DLCI);

            p_mcb->state = RFC_MX_STATE_CONNECTED;
            p_mcb->peer_ready = TRUE;
        }

        return;
    }

    RFCOMM_TRACE(EVENT, "RFCOMM MX ignored - evt:%d in state:%d", event, p_mcb->state);
}

static void rfc_mx_sm_state_connected(tRFC_MCB* p_mcb, UINT16 event, void* p_data) {
    RFCOMM_TRACE(EVENT, "rfc_mx_sm_state_connected - evt:%d", event);

    switch (event) {
    case RFC_EVENT_TIMEOUT:
    case RFC_MX_EVENT_CLOSE_REQ:
        rfc_timer_start(p_mcb, RFC_DISC_TIMEOUT);
        p_mcb->state = RFC_MX_STATE_DISC_WAIT_UA;
        rfc_send_disc(p_mcb, RFCOMM_MX_DLCI);
        return;

    case RFC_MX_EVENT_DISC_IND:
        p_mcb->state = RFC_MX_STATE_IDLE;
        PORT_CloseInd(p_mcb);
        return;

    case RFC_EVENT_DISC:
        rfc_send_ua(p_mcb, RFCOMM_MX_DLCI);

        if (p_mcb->is_initiator)
            L2CA_DisconnectReq(p_mcb->lcid);

        PORT_CloseInd(p_mcb);
        return;
    }

    RFCOMM_TRACE(EVENT, "RFCOMM MX ignored - evt:%d in state:%d", event, p_mcb->state);
}

static void rfc_mx_sm_state_disc_wait_ua(tRFC_MCB* p_mcb, UINT16 event, void* p_data) {
    RFCOMM_TRACE(EVENT, "rfc_mx_sm_state_disc_wait_ua - evt:%d", event);

    switch (event) {
    case RFC_EVENT_UA:
    case RFC_EVENT_DM:
    case RFC_EVENT_TIMEOUT:
        L2CA_DisconnectReq(p_mcb->lcid);

        if (p_mcb->restart_required) {
            if ((p_mcb->lcid = L2CA_ConnectReq(BT_PSM_RFCOMM, p_mcb->bd_addr)) == 0) {
                PORT_StartCnf(p_mcb, RFCOMM_ERROR);
                return;
            }

            rfc_save_lcid_mcb(p_mcb, p_mcb->lcid);

            p_mcb->restart_required = FALSE;
            p_mcb->local_cfg_sent = FALSE;
            p_mcb->peer_cfg_rcvd = FALSE;

            p_mcb->state = RFC_MX_STATE_WAIT_CONN_CNF;
            return;
        }

        rfc_release_multiplexer_channel(p_mcb);
        return;

    case RFC_EVENT_DISC:
        rfc_send_ua(p_mcb, RFCOMM_MX_DLCI);
        return;

    case RFC_EVENT_UIH:
        GKI_freebuf(p_data);
        rfc_send_dm(p_mcb, RFCOMM_MX_DLCI, FALSE);
        return;

    case RFC_MX_EVENT_START_REQ:
        p_mcb->restart_required = TRUE;
        return;

    case RFC_MX_EVENT_DISC_IND:
        p_mcb->state = RFC_MX_STATE_IDLE;
        PORT_CloseInd(p_mcb);
        return;

    case RFC_MX_EVENT_CLOSE_REQ:
        return;

    case RFC_MX_EVENT_QOS_VIOLATION_IND:
        break;
    }

    RFCOMM_TRACE(EVENT, "RFCOMM MX ignored - evt:%d in state:%d", event, p_mcb->state);
}

static void rfc_mx_send_config_req(tRFC_MCB* p_mcb) {
    tL2CAP_CFG_INFO cfg;

    RFCOMM_TRACE(EVENT, "rfc_mx_send_config_req");

    memset(&cfg, 0, sizeof cfg);

    cfg.mtu_present = TRUE;
    cfg.mtu = L2CAP_MTU_SIZE;
    cfg.flush_to_present = FALSE;
    cfg.qos_present = FALSE;

    L2CA_ConfigReq(p_mcb->lcid, &cfg);
}

static void rfc_mx_conf_cnf(tRFC_MCB* p_mcb, tL2CAP_CFG_INFO* p_cfg) {
    RFCOMM_TRACE(EVENT, "rfc_mx_conf_cnf p_cfg:%08x res:%d ", p_cfg, p_cfg ? p_cfg->result : 0);

    if (p_cfg->result != L2CAP_CFG_OK) {
        if (p_mcb->is_initiator) {
            PORT_StartCnf(p_mcb, p_cfg->result);
            L2CA_DisconnectReq(p_mcb->lcid);
        }

        rfc_release_multiplexer_channel(p_mcb);
        return;
    }

    p_mcb->local_cfg_sent = TRUE;

    if (p_mcb->state == RFC_MX_STATE_CONFIGURE && p_mcb->peer_cfg_rcvd) {
        if (p_mcb->is_initiator) {
            p_mcb->state = RFC_MX_STATE_SABME_WAIT_UA;

            rfc_send_sabme(p_mcb, RFCOMM_MX_DLCI);
        } else {
            p_mcb->state = RFC_MX_STATE_WAIT_SABME;
        }
    }
}

static void rfc_mx_conf_ind(tRFC_MCB* p_mcb, tL2CAP_CFG_INFO* p_cfg) {
    RFCOMM_TRACE(EVENT, "rfc_mx_conf_ind p_cfg:%0x", p_cfg);

    if (p_cfg->mtu_present)
        p_mcb->peer_l2cap_mtu = p_cfg->mtu - RFCOMM_MIN_OFFSET - 1;
    else
        p_mcb->peer_l2cap_mtu = L2CAP_DEFAULT_MTU - RFCOMM_MIN_OFFSET - 1;

    p_cfg->mtu_present = FALSE;
    p_cfg->flush_to_present = FALSE;
    p_cfg->qos_present = FALSE;

    p_cfg->result = L2CAP_CFG_OK;

    L2CA_ConfigRsp(p_mcb->lcid, p_cfg);

    p_mcb->peer_cfg_rcvd = TRUE;

    if (p_mcb->state == RFC_MX_STATE_CONFIGURE && p_mcb->local_cfg_sent) {
        if (p_mcb->is_initiator) {
            p_mcb->state = RFC_MX_STATE_SABME_WAIT_UA;
            rfc_send_sabme(p_mcb, RFCOMM_MX_DLCI);
        } else {
            p_mcb->state = RFC_MX_STATE_WAIT_SABME;
        }
    }
}
