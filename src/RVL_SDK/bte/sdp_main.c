#include "sdp_int.h"

/* Original source:
 * bluedroid <android.googlesource.com/platform/external/bluetooth/bluedroid>
 * stack/sdp/sdp_main.c
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

#include "btm_api.h"
#include "btu.h"
#include "gki.h"
#include "hcidefs.h"
#include "l2c_api.h"
#include "l2cdefs.h"
#include "sdp_api.h"

/*******************************************************************************
 * local function declarations
 */

static void sdp_connect_ind(BD_ADDR bd_addr, UINT16 l2cap_cid, UINT16 psm, UINT8 l2cap_id);
static void sdp_connect_cfm(UINT16 l2cap_cid, UINT16 result);
static void sdp_config_ind(UINT16 l2cap_cid, tL2CAP_CFG_INFO* p_cfg);
static void sdp_config_cfm(UINT16 l2cap_cid, tL2CAP_CFG_INFO* p_cfg);
static void sdp_disconnect_ind(UINT16 l2cap_cid, BOOLEAN ack_needed);
static void sdp_data_ind(UINT16 l2cap_cid, BT_HDR* p_msg);

static void sdp_disconnect_cfm(UINT16 l2cap_cid, UINT16 result);

/*******************************************************************************
 * variables
 */

// .bss
tSDP_CB sdp_cb;

/*******************************************************************************
 * functions
 */

void sdp_init(void) {
    memset(&sdp_cb, 0, sizeof sdp_cb);

    sdp_cb.l2cap_my_cfg.mtu_present = TRUE;
    sdp_cb.l2cap_my_cfg.mtu = SDP_MTU_SIZE;
    sdp_cb.l2cap_my_cfg.flush_to_present = TRUE;
    sdp_cb.l2cap_my_cfg.flush_to = SDP_FLUSH_TO;

    sdp_cb.max_attr_list_size = SDP_MTU_SIZE - 16;
    sdp_cb.max_recs_per_search = SDP_MAX_DISC_SERVER_RECS;

    if (!BTM_SetSecurityLevel(FALSE, SDP_SERVICE_NAME, BTM_SEC_SERVICE_SDP_SERVER, SDP_SECURITY_LEVEL, SDP_PSM, 0, 0)) {
        SDP_TRACE(ERROR, "Security Registration Server failed");
        return;
    }

    if (!BTM_SetSecurityLevel(TRUE, SDP_SERVICE_NAME, BTM_SEC_SERVICE_SDP_SERVER, SDP_SECURITY_LEVEL, SDP_PSM, 0, 0)) {
        SDP_TRACE(ERROR, "Security Registration for Client failed");
        return;
    }

    sdp_cb.trace_level = BT_TRACE_LEVEL_NONE;

    sdp_cb.reg_info.pL2CA_ConnectInd_Cb = &sdp_connect_ind;
    sdp_cb.reg_info.pL2CA_ConnectCfm_Cb = &sdp_connect_cfm;
    sdp_cb.reg_info.pL2CA_ConnectPnd_Cb = NULL;
    sdp_cb.reg_info.pL2CA_ConfigInd_Cb = &sdp_config_ind;
    sdp_cb.reg_info.pL2CA_ConfigCfm_Cb = &sdp_config_cfm;
    sdp_cb.reg_info.pL2CA_DisconnectInd_Cb = &sdp_disconnect_ind;
    sdp_cb.reg_info.pL2CA_DisconnectCfm_Cb = &sdp_disconnect_cfm;
    sdp_cb.reg_info.pL2CA_QoSViolationInd_Cb = NULL;
    sdp_cb.reg_info.pL2CA_DataInd_Cb = &sdp_data_ind;
    sdp_cb.reg_info.pL2CA_CongestionStatus_Cb = NULL;

    if (!L2CA_Register(SDP_PSM, &sdp_cb.reg_info))
        SDP_TRACE(ERROR, "SDP Registration failed");
}

UINT16 sdp_set_max_attr_list_size(UINT16 max_size) {
    if (max_size > sdp_cb.l2cap_my_cfg.mtu - 16)
        max_size = sdp_cb.l2cap_my_cfg.mtu - 16;

    sdp_cb.max_attr_list_size = max_size;

    return sdp_cb.max_attr_list_size;
}

static void sdp_connect_ind(BD_ADDR bd_addr, UINT16 l2cap_cid, UINT16 psm, UINT8 l2cap_id) {
    tCONN_CB* p_ccb;

    if ((p_ccb = sdpu_allocate_ccb()) == NULL)
        return;

    p_ccb->con_state = SDP_STATE_CFG_SETUP;

    memcpy(&p_ccb->device_address[0], bd_addr, BD_ADDR_LEN);
    p_ccb->connection_id = l2cap_cid;

    L2CA_ConnectRsp(bd_addr, l2cap_id, l2cap_cid, L2CAP_CONN_OK, L2CAP_CONN_OK);
    L2CA_ConfigReq(l2cap_cid, &sdp_cb.l2cap_my_cfg);

    SDP_TRACE(EVENT, "SDP - Rcvd L2CAP conn ind, sent config req, CID 0x%x", p_ccb->connection_id);
}

static void sdp_connect_cfm(UINT16 l2cap_cid, UINT16 result) {
    tCONN_CB* p_ccb;

    if ((p_ccb = sdpu_find_ccb_by_cid(l2cap_cid)) == NULL) {
        SDP_TRACE(WARNING, "SDP - Rcvd conn cnf for unknown CID 0x%x", l2cap_cid);
        return;
    }

    if (result == L2CAP_CONN_OK && p_ccb->con_state == SDP_STATE_CONN_SETUP) {
        p_ccb->con_state = 2;
        L2CA_ConfigReq(l2cap_cid, &sdp_cb.l2cap_my_cfg);

        SDP_TRACE(EVENT, "SDP - got conn cnf, sent cfg req, CID: 0x%x", p_ccb->connection_id);
    } else {
        SDP_TRACE(WARNING, "SDP - Rcvd conn cnf with error: 0x%x  CID 0x%x", result, p_ccb->connection_id);

        if (p_ccb->p_cb) {
            if (result == HCI_ERR_HOST_REJECT_SECURITY || result == HCI_ERR_AUTH_FAILURE || result == HCI_ERR_PAIRING_NOT_ALLOWED ||
                result == HCI_ERR_PAIRING_WITH_UNIT_KEY_NOT_SUPPORTED || result == HCI_ERR_KEY_MISSING) {
                (*p_ccb->p_cb)(SDP_SECURITY_ERR);
            } else if (result == HCI_ERR_HOST_REJECT_DEVICE) {
                (*p_ccb->p_cb)(SDP_CONN_REJECTED);
            } else {
                (*p_ccb->p_cb)(SDP_CONN_FAILED);
            }
        }

        sdpu_release_ccb(p_ccb);
    }
}

static void sdp_config_ind(UINT16 l2cap_cid, tL2CAP_CFG_INFO* p_cfg) {
    tCONN_CB* p_ccb;

    if ((p_ccb = sdpu_find_ccb_by_cid(l2cap_cid)) == NULL) {
        SDP_TRACE(WARNING, "SDP - Rcvd L2CAP cfg ind, unknown CID: 0x%x", l2cap_cid);
        return;
    }

    if (!p_cfg->mtu_present) {
        p_ccb->rem_mtu_size = L2CAP_DEFAULT_MTU > SDP_MTU_SIZE ? SDP_MTU_SIZE : L2CAP_DEFAULT_MTU;
    } else {
        if (p_cfg->mtu > SDP_MTU_SIZE)
            p_ccb->rem_mtu_size = SDP_MTU_SIZE;
        else
            p_ccb->rem_mtu_size = p_cfg->mtu;
    }

    p_cfg->flush_to_present = FALSE;
    p_cfg->mtu_present = FALSE;
    p_cfg->result = L2CAP_CFG_OK;

    L2CA_ConfigRsp(l2cap_cid, p_cfg);

    SDP_TRACE(EVENT, "SDP - Rcvd cfg ind, sent cfg cfm, CID: 0x%x", l2cap_cid);

    p_ccb->con_flags |= SDP_FLAGS_THEIR_CFG_DONE;

    if (p_ccb->con_flags & SDP_FLAGS_OUR_CFG_DONE) {
        p_ccb->con_state = SDP_STATE_CONNECTED;

        if (p_ccb->con_flags & SDP_FLAGS_IS_ORIG) {
            sdp_disc_connected(p_ccb);
        } else {
            btu_start_timer(&p_ccb->timer_entry, BTU_TTYPE_SDP, SDP_INACT_TIMEOUT);
        }
    }
}

static void sdp_config_cfm(UINT16 l2cap_cid, tL2CAP_CFG_INFO* p_cfg) {
    tCONN_CB* p_ccb;

    SDP_TRACE(EVENT, "SDP - Rcvd cfg cfm, CID: 0x%x  Result: %d", l2cap_cid, p_cfg->result);

    if ((p_ccb = sdpu_find_ccb_by_cid(l2cap_cid)) == NULL) {
        SDP_TRACE(WARNING, "SDP - Rcvd L2CAP cfg ind, unknown CID: 0x%x", l2cap_cid);
        return;
    }

    if (p_cfg->result == L2CAP_CFG_OK) {
        p_ccb->con_flags |= SDP_FLAGS_OUR_CFG_DONE;

        if (p_ccb->con_flags & SDP_FLAGS_THEIR_CFG_DONE) {
            p_ccb->con_state = SDP_STATE_CONNECTED;

            if (p_ccb->con_flags & SDP_FLAGS_IS_ORIG) {
                sdp_disc_connected(p_ccb);
            } else {
                btu_start_timer(&p_ccb->timer_entry, BTU_TTYPE_SDP, SDP_INACT_TIMEOUT);
            }
        }
    } else {
        sdp_disconnect(p_ccb, SDP_CFG_FAILED);
    }
}

static void sdp_disconnect_ind(UINT16 l2cap_cid, BOOLEAN ack_needed) {
    tCONN_CB* p_ccb;

    if ((p_ccb = sdpu_find_ccb_by_cid(l2cap_cid)) == NULL) {
        SDP_TRACE(WARNING, "SDP - Rcvd L2CAP disc, unknown CID: 0x%x", l2cap_cid);
        return;
    }

    if (ack_needed)
        L2CA_DisconnectRsp(l2cap_cid);

    SDP_TRACE(EVENT, "SDP - Rcvd L2CAP disc, CID: 0x%x", l2cap_cid);

    if (p_ccb->p_cb) {
        (*p_ccb->p_cb)(p_ccb->con_state == SDP_STATE_CONNECTED ? SDP_SUCCESS : SDP_CONN_FAILED);
    }

    sdpu_release_ccb(p_ccb);
}

static void sdp_data_ind(UINT16 l2cap_cid, BT_HDR* p_msg) {
    tCONN_CB* p_ccb;

    if ((p_ccb = sdpu_find_ccb_by_cid(l2cap_cid)) != NULL) {
        if (p_ccb->con_state == SDP_STATE_CONNECTED) {
            if (p_ccb->con_flags & SDP_FLAGS_IS_ORIG)
                sdp_disc_server_rsp(p_ccb, p_msg);
            else
                sdp_server_handle_client_req(p_ccb, p_msg);
        } else {
            SDP_TRACE(WARNING, "SDP - Ignored L2CAP data while in state: %d, CID: 0x%x", p_ccb->con_state, l2cap_cid);
        }
    } else {
        SDP_TRACE(WARNING, "SDP - Rcvd L2CAP data, unknown CID: 0x%x", l2cap_cid);
    }

    GKI_freebuf(p_msg);
}

tCONN_CB* sdp_conn_originate(UINT8* p_bd_addr) {
    tCONN_CB* p_ccb;
    UINT16 cid;

    if ((p_ccb = sdpu_allocate_ccb()) == NULL) {
        SDP_TRACE(WARNING, "SDP - no spare CCB for orig");
        return (NULL);
    }

    SDP_TRACE(EVENT, "SDP - Originate started");

    p_ccb->con_flags |= SDP_FLAGS_IS_ORIG;

    memcpy(p_ccb->device_address, p_bd_addr, BD_ADDR_LEN);

    p_ccb->con_state = SDP_STATE_CONN_SETUP;

    cid = L2CA_ConnectReq(SDP_PSM, p_bd_addr);

    if (cid != 0) {
        p_ccb->connection_id = cid;

        return (p_ccb);
    } else {
        SDP_TRACE(WARNING, "SDP - Originate failed");
        sdpu_release_ccb(p_ccb);
        return NULL;
    }
}

void sdp_disconnect(tCONN_CB* p_ccb, UINT16 reason) {
    SDP_TRACE(EVENT, "SDP - disconnect  CID: 0x%x", p_ccb->connection_id);

    if (p_ccb->con_state != SDP_STATE_CONN_SETUP) {
        L2CA_DisconnectReq(p_ccb->connection_id);
        p_ccb->disconnect_reason = reason;
    } else {
        if (p_ccb->p_cb)
            (*p_ccb->p_cb)(reason);

        sdpu_release_ccb(p_ccb);
    }
}

static void sdp_disconnect_cfm(UINT16 l2cap_cid, UINT16 result) {
    tCONN_CB* p_ccb;

    if ((p_ccb = sdpu_find_ccb_by_cid(l2cap_cid)) == NULL) {
        SDP_TRACE(WARNING, "SDP - Rcvd L2CAP disc cfm, unknown CID: 0x%x", l2cap_cid);
        return;
    }

    SDP_TRACE(EVENT, "SDP - Rcvd L2CAP disc cfm, CID: 0x%x", l2cap_cid);

    if (p_ccb->p_cb)
        (*p_ccb->p_cb)(p_ccb->disconnect_reason);

    sdpu_release_ccb(p_ccb);
}

void sdp_conn_timeout(tCONN_CB* p_ccb) {
    SDP_TRACE(EVENT, "SDP - CCB timeout in state: %d  CID: 0x%x", p_ccb->con_state, p_ccb->connection_id);

    L2CA_DisconnectReq(p_ccb->connection_id);

    if (p_ccb->p_cb)
        (*p_ccb->p_cb)(SDP_CONN_FAILED);

    sdpu_release_ccb(p_ccb);
}
