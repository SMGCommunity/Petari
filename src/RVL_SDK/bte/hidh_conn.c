#include "hidh_int.h"

/* Original source:
 * bluedroid <android.googlesource.com/platform/external/bluetooth/bluedroid>
 * stack/hid/hidh_conn.c
 */

/******************************************************************************
 *
 *  Copyright (C) 2002-2012 Broadcom Corporation
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
#include "btm_int.h"
#include "btu.h"
#include "gki.h"
#include "hcidefs.h"
#include "hid_conn.h"
#include "hiddefs.h"
#include "hidh_api.h"
#include "l2c_api.h"
#include "l2cdefs.h"

/*******************************************************************************
 * macros
 */

/*******************************************************************************
 * types
 */

/*******************************************************************************
 * local function declarations
 */

static void hidh_l2cif_connect_ind(BD_ADDR bd_addr, UINT16 l2cap_cid, UINT16 psm, UINT8 l2cap_id);
static void hidh_l2cif_connect_cfm(UINT16 l2cap_cid, UINT16 result);
static void hidh_l2cif_config_ind(UINT16 l2cap_cid, tL2CAP_CFG_INFO* p_cfg);
static void hidh_l2cif_config_cfm(UINT16 l2cap_cid, tL2CAP_CFG_INFO* p_cfg);
static void hidh_l2cif_disconnect_ind(UINT16 l2cap_cid, BOOLEAN ack_needed);
static void hidh_l2cif_disconnect_cfm(UINT16 l2cap_cid, UINT16 result);
static void hidh_l2cif_cong_ind(UINT16 l2cap_cid, BOOLEAN congested);
static void hidh_l2cif_data_ind(UINT16 l2cap_cid, BT_HDR* p_msg);

static UINT8 find_conn_by_cid(UINT16 cid);

// Only used if HID_HOST_MAX_CONN_RETRY > 0, but here it was 0.
static void hidh_conn_retry(UINT8 dhandle);

/*******************************************************************************
 * variables
 */

// .rodata
static tL2CAP_APPL_INFO const hst_reg_info = {
    &hidh_l2cif_connect_ind,    &hidh_l2cif_connect_cfm,    NULL, &hidh_l2cif_config_ind, &hidh_l2cif_config_cfm,
    &hidh_l2cif_disconnect_ind, &hidh_l2cif_disconnect_cfm, NULL, &hidh_l2cif_data_ind,   &hidh_l2cif_cong_ind,
};

/*******************************************************************************
 * functions
 */

tHID_STATUS hidh_conn_reg(void) {
    int xx;

    hh_cb.l2cap_cfg.mtu_present = TRUE;
    hh_cb.l2cap_cfg.mtu = HID_HOST_MTU;
    hh_cb.l2cap_cfg.flush_to_present = TRUE;
    hh_cb.l2cap_cfg.flush_to = HID_HOST_FLUSH_TO;

    if (!L2CA_Register(HID_PSM_CONTROL, (tL2CAP_APPL_INFO*)&hst_reg_info)) {
        HIDH_TRACE(ERROR, "HID Control Registration failed");
        return HID_ERR_L2CAP_FAILED;
    }

    if (!L2CA_Register(HID_PSM_INTERRUPT, (tL2CAP_APPL_INFO*)&hst_reg_info)) {
        L2CA_Deregister(HID_PSM_CONTROL);
        HIDH_TRACE(ERROR, "HID Interrupt Registration failed");
        return HID_ERR_L2CAP_FAILED;
    }

    for (xx = 0; xx < HID_HOST_MAX_DEVICES; xx++) {
        hh_cb.devices[xx].in_use = FALSE;
        hh_cb.devices[xx].conn.conn_state = HID_CONN_STATE_UNUSED;
    }

    return HID_SUCCESS;
}

tHID_STATUS hidh_conn_disconnect(UINT8 dhandle) {
    tHID_CONN* p_hcon = &hh_cb.devices[dhandle].conn;

    HIDH_TRACE(EVENT, "HID - disconnect");

    if (p_hcon->ctrl_cid || p_hcon->intr_cid) {
        p_hcon->conn_state = HID_CONN_STATE_DISCONNECTING;

        if (p_hcon->intr_cid)
            L2CA_DisconnectReq(p_hcon->intr_cid);

        if (p_hcon->ctrl_cid)
            L2CA_DisconnectReq(p_hcon->ctrl_cid);
    } else {
        p_hcon->conn_state = HID_CONN_STATE_UNUSED;
    }

    return HID_SUCCESS;
}

void hidh_sec_check_complete_term(BD_ADDR bd_addr, void* p_ref_data, UINT8 res) {
    tHID_HOST_DEV_CTB* p_dev = p_ref_data;
    UINT8 dhandle = p_dev - hh_cb.devices;

    if (res == BTM_SUCCESS && p_dev->conn.conn_state == HID_CONN_STATE_SECURITY) {
        p_dev->conn.disc_reason = HID_SUCCESS;
        p_dev->conn.conn_state = HID_CONN_STATE_CONNECTING_INTR;

        L2CA_ConnectRsp(p_dev->addr, p_dev->conn.ctrl_id, p_dev->conn.ctrl_cid, L2CAP_CONN_OK, L2CAP_CONN_OK);

        L2CA_ConfigReq(p_dev->conn.ctrl_cid, &hh_cb.l2cap_cfg);
    } else if (res != BTM_SUCCESS) {
        p_dev->conn.disc_reason = HID_ERR_AUTH_FAILED;

        hidh_conn_disconnect(dhandle);
    }
}

static void hidh_l2cif_connect_ind(BD_ADDR bd_addr, UINT16 l2cap_cid, UINT16 psm, UINT8 l2cap_id) {
    tHID_CONN* p_hcon;
    BOOLEAN bAccept = TRUE;
    int i;
    tHID_HOST_DEV_CTB* p_dev;

    HIDH_TRACE(EVENT, "HID - Rcvd L2CAP conn ind, PSM: 0x%04x  CID 0x%x", psm, l2cap_cid);

    for (i = 0; i < HID_HOST_MAX_DEVICES; i++) {
        if (hh_cb.devices[i].in_use && memcmp(bd_addr, hh_cb.devices[i].addr, BD_ADDR_LEN) == 0) {
            break;
        }
    }

    if (i == HID_HOST_MAX_DEVICES) {
        L2CA_ConnectRsp(bd_addr, l2cap_id, l2cap_cid, L2CAP_CONN_SECURITY_BLOCK, 0);
        return;
    }

    p_hcon = &hh_cb.devices[i].conn;
    p_dev = &hh_cb.devices[i];

    if (psm == HID_PSM_INTERRUPT) {
        if (p_hcon->ctrl_cid == 0) {
            HIDH_TRACE(WARNING, "HID - Rcvd INTR L2CAP conn ind, but no CTL channel");
            bAccept = FALSE;
        }
        if (p_hcon->conn_state != HID_CONN_STATE_CONNECTING_INTR) {
            HIDH_TRACE(WARNING, "HID - Rcvd INTR L2CAP conn ind, wrong state: %d", p_hcon->conn_state);
            bAccept = FALSE;
        }
    } else {
        if (p_hcon->conn_state != HID_CONN_STATE_UNUSED) {
            HIDH_TRACE(WARNING, "HID - Rcvd CTL L2CAP conn ind, wrong state: %d", p_hcon->conn_state);
            bAccept = FALSE;
        }
    }

    if (!bAccept) {
        L2CA_ConnectRsp(bd_addr, l2cap_id, l2cap_cid, L2CAP_CONN_NO_RESOURCES, L2CAP_CONN_OK);
        return;
    }

    if (psm == HID_PSM_CONTROL) {
        p_hcon->conn_flags = 0;
        p_hcon->ctrl_cid = l2cap_cid;
        p_hcon->ctrl_id = l2cap_id;
        p_hcon->disc_reason = HID_L2CAP_CONN_FAIL;

        p_hcon->conn_state = HID_CONN_STATE_SECURITY;

        btm_sec_mx_access_request(p_dev->addr, HID_PSM_CONTROL, FALSE, BTM_SEC_PROTO_HID,
                                  p_dev->attr_mask & HID_SEC_REQUIRED ? HID_SEC_CHN : HID_NOSEC_CHN, &hidh_sec_check_complete_term, p_dev);

        return;
    }

    p_hcon->conn_state = HID_CONN_STATE_CONFIG;
    p_hcon->intr_cid = l2cap_cid;

    L2CA_ConnectRsp(bd_addr, l2cap_id, l2cap_cid, L2CAP_CONN_OK, L2CAP_CONN_OK);

    L2CA_ConfigReq(l2cap_cid, &hh_cb.l2cap_cfg);

    HIDH_TRACE(EVENT, "HID - Rcvd L2CAP conn ind, sent config req, PSM: 0x%04x  CID 0x%x", psm, l2cap_cid);
}

void hidh_proc_repage_timeout(TIMER_LIST_ENT* p_tle) {
    hidh_conn_initiate((UINT32)p_tle->param);

    ++hh_cb.devices[(UINT32)p_tle->param].conn_tries;

    (*hh_cb.callback)((UINT32)p_tle->param, HID_HDEV_EVT_RETRYING, hh_cb.devices[(UINT32)p_tle->param].conn_tries, NULL);
}

void hidh_sec_check_complete_orig(BD_ADDR bd_addr, void* p_ref_data, UINT8 res) {
    tHID_HOST_DEV_CTB* p_dev = p_ref_data;
    UINT8 dhandle = p_dev - hh_cb.devices;
    UINT32 reason;

    if (res == BTM_SUCCESS && p_dev->conn.conn_state == HID_CONN_STATE_SECURITY) {
        HIDH_TRACE(EVENT, "HID - Originator security pass.");
        p_dev->conn.disc_reason = HID_SUCCESS;

        if ((p_dev->conn.intr_cid = L2CA_ConnectReq(HID_PSM_INTERRUPT, hh_cb.devices[dhandle].addr)) == 0) {
            HIDH_TRACE(WARNING, "HID - INTR Originate failed");
            reason = HID_L2CAP_REQ_FAIL;

            hidh_conn_disconnect(dhandle);
            (*hh_cb.callback)(dhandle, HID_HDEV_EVT_CLOSE, reason, NULL);
            return;
        } else {
            p_dev->conn.conn_state = HID_CONN_STATE_CONNECTING_INTR;
        }
    }

    if (res != BTM_SUCCESS && p_dev->conn.conn_state == HID_CONN_STATE_SECURITY) {
        p_dev->conn.disc_reason = HID_ERR_AUTH_FAILED;
        hidh_conn_disconnect(dhandle);
    }
}

static void hidh_l2cif_connect_cfm(UINT16 l2cap_cid, UINT16 result) {
    UINT8 dhandle;
    tHID_CONN* p_hcon = NULL;
    UINT32 reason;
    tHID_HOST_DEV_CTB* p_dev = NULL;

    if ((dhandle = find_conn_by_cid(l2cap_cid)) != HID_HOST_MAX_DEVICES) {
        p_dev = &hh_cb.devices[dhandle];
        p_hcon = &hh_cb.devices[dhandle].conn;
    }

    if (!p_hcon || !(p_hcon->conn_flags & HID_CONN_FLAGS_IS_ORIG) ||
        (l2cap_cid == p_hcon->ctrl_cid && p_hcon->conn_state != HID_CONN_STATE_CONNECTING_CTRL) ||
        (l2cap_cid == p_hcon->intr_cid && p_hcon->conn_state != HID_CONN_STATE_CONNECTING_INTR)) {
        HIDH_TRACE(WARNING, "HID - Rcvd unexpected conn cnf, CID 0x%x ", l2cap_cid);
        return;
    }

    if (result != L2CAP_CONN_OK) {
        if (l2cap_cid == p_hcon->ctrl_cid)
            p_hcon->ctrl_cid = 0;
        else
            p_hcon->intr_cid = 0;

        hidh_conn_disconnect(dhandle);

        reason = HID_L2CAP_CONN_FAIL | result;
        (*hh_cb.callback)(dhandle, HID_HDEV_EVT_CLOSE, reason, NULL);
        return;
    }

    if (l2cap_cid == p_hcon->ctrl_cid) {
        p_hcon->conn_state = HID_CONN_STATE_SECURITY;
        p_hcon->disc_reason = HID_L2CAP_CONN_FAIL;

        btm_sec_mx_access_request(p_dev->addr, HID_PSM_CONTROL, TRUE, BTM_SEC_PROTO_HID,
                                  p_dev->attr_mask & HID_SEC_REQUIRED ? HID_SEC_CHN : HID_NOSEC_CHN, &hidh_sec_check_complete_orig, p_dev);
    } else {
        p_hcon->conn_state = HID_CONN_STATE_CONFIG;
    }

    L2CA_ConfigReq(l2cap_cid, &hh_cb.l2cap_cfg);

    HIDH_TRACE(EVENT, "HID - got CTRL conn cnf, sent cfg req, CID: 0x%x", l2cap_cid);
}

static void hidh_l2cif_config_ind(UINT16 l2cap_cid, tL2CAP_CFG_INFO* p_cfg) {
    UINT8 dhandle;
    tHID_CONN* p_hcon = NULL;
    tHID_HOST_DEV_CTB* p_dev;

    if ((dhandle = find_conn_by_cid(l2cap_cid)) != HID_HOST_MAX_DEVICES) {
        p_dev = &hh_cb.devices[dhandle];
        p_hcon = &hh_cb.devices[dhandle].conn;
    }

    if (!p_hcon) {
        HIDH_TRACE(WARNING, "HID - Rcvd L2CAP cfg ind, unknown CID: 0x%x", l2cap_cid);
        return;
    }

    HIDH_TRACE(EVENT, "HID - Rcvd cfg ind, sent cfg cfm, CID: 0x%x", l2cap_cid);

    if (!p_cfg->mtu_present || p_cfg->mtu > HID_HOST_MTU)
        p_hcon->rem_mtu_size = HID_HOST_MTU;
    else
        p_hcon->rem_mtu_size = p_cfg->mtu;

    p_cfg->flush_to_present = FALSE;
    p_cfg->mtu_present = FALSE;
    p_cfg->result = L2CAP_CFG_OK;

    L2CA_ConfigRsp(l2cap_cid, p_cfg);

    if (l2cap_cid == p_hcon->ctrl_cid)
        p_hcon->conn_flags |= HID_CONN_FLAGS_THEIR_CTRL_CFG_DONE;
    else
        p_hcon->conn_flags |= HID_CONN_FLAGS_THEIR_INTR_CFG_DONE;

    if ((p_hcon->conn_flags & HID_CONN_FLAGS_ALL_CONFIGURED) == HID_CONN_FLAGS_ALL_CONFIGURED && p_hcon->conn_state == HID_CONN_STATE_CONFIG) {
        p_hcon->conn_state = HID_CONN_STATE_CONNECTED;

        hh_cb.devices[dhandle].state = HID_DEV_CONNECTED;
        (*hh_cb.callback)(dhandle, HID_HDEV_EVT_OPEN, 0, NULL);
    }
}

static void hidh_l2cif_config_cfm(UINT16 l2cap_cid, tL2CAP_CFG_INFO* p_cfg) {
    UINT8 dhandle;
    tHID_CONN* p_hcon = NULL;
    UINT32 reason;

    HIDH_TRACE(EVENT, "HID - Rcvd cfg cfm, CID: 0x%x  Result: %d", l2cap_cid, p_cfg->result);

    if ((dhandle = find_conn_by_cid(l2cap_cid)) != HID_HOST_MAX_DEVICES)
        p_hcon = &hh_cb.devices[dhandle].conn;

    if (p_hcon == NULL) {
        HIDH_TRACE(WARNING, "HID - Rcvd L2CAP cfg ind, unknown CID: 0x%x", l2cap_cid);
        return;
    }

    if (p_cfg->result != L2CAP_CFG_OK) {
        hidh_conn_disconnect(dhandle);

        reason = HID_L2CAP_CFG_FAIL | p_cfg->result;
        (*hh_cb.callback)(dhandle, HID_HDEV_EVT_CLOSE, reason, NULL);
        return;
    }

    if (l2cap_cid == p_hcon->ctrl_cid)
        p_hcon->conn_flags |= HID_CONN_FLAGS_OUR_CTRL_CFG_DONE;
    else
        p_hcon->conn_flags |= HID_CONN_FLAGS_OUR_INTR_CFG_DONE;

    if ((p_hcon->conn_flags & HID_CONN_FLAGS_ALL_CONFIGURED) == HID_CONN_FLAGS_ALL_CONFIGURED && p_hcon->conn_state == HID_CONN_STATE_CONFIG) {
        p_hcon->conn_state = HID_CONN_STATE_CONNECTED;

        hh_cb.devices[dhandle].state = HID_DEV_CONNECTED;
        (*hh_cb.callback)(dhandle, HID_HDEV_EVT_OPEN, 0, NULL);
    }
}

static void hidh_l2cif_disconnect_ind(UINT16 l2cap_cid, BOOLEAN ack_needed) {
    UINT8 dhandle;
    tHID_CONN* p_hcon = NULL;
    UINT16 disc_res = HCI_SUCCESS;
    UINT16 hid_close_evt_reason;

    if ((dhandle = find_conn_by_cid(l2cap_cid)) != HID_HOST_MAX_DEVICES)
        p_hcon = &hh_cb.devices[dhandle].conn;

    if (p_hcon == NULL) {
        HIDH_TRACE(WARNING, "HID - Rcvd L2CAP disc, unknown CID: 0x%x", l2cap_cid);
        return;
    }

    if (ack_needed)
        L2CA_DisconnectRsp(l2cap_cid);

    HIDH_TRACE(EVENT, "HID - Rcvd L2CAP disc, CID: 0x%x", l2cap_cid);

    p_hcon->conn_state = HID_CONN_STATE_DISCONNECTING;

    if (l2cap_cid == p_hcon->ctrl_cid)
        p_hcon->ctrl_cid = 0;
    else
        p_hcon->intr_cid = 0;

    if (!p_hcon->ctrl_cid && !p_hcon->intr_cid) {
        hh_cb.devices[dhandle].state = HID_DEV_NO_CONN;
        p_hcon->conn_state = HID_CONN_STATE_UNUSED;

        if (!ack_needed)
            disc_res = btm_get_acl_disc_reason_code();

        hid_close_evt_reason = p_hcon->disc_reason;

        if (disc_res == HCI_ERR_AUTH_FAILURE || disc_res == HCI_ERR_KEY_MISSING || disc_res == HCI_ERR_HOST_REJECT_SECURITY ||
            disc_res == HCI_ERR_PAIRING_NOT_ALLOWED || disc_res == HCI_ERR_UNIT_KEY_USED || disc_res == HCI_ERR_PAIRING_WITH_UNIT_KEY_NOT_SUPPORTED ||
            disc_res == HCI_ERR_ENCRY_MODE_NOT_ACCEPTABLE || disc_res == HCI_ERR_REPEATED_ATTEMPTS) {
            hid_close_evt_reason = HID_ERR_AUTH_FAILED;
        }

        (*hh_cb.callback)(dhandle, HID_HDEV_EVT_CLOSE, hid_close_evt_reason, NULL);
    }
}

static void hidh_l2cif_disconnect_cfm(UINT16 l2cap_cid, UINT16 result) {
    UINT8 dhandle;
    tHID_CONN* p_hcon = NULL;

    if ((dhandle = find_conn_by_cid(l2cap_cid)) != HID_HOST_MAX_DEVICES)
        p_hcon = &hh_cb.devices[dhandle].conn;

    if (!p_hcon) {
        HIDH_TRACE(WARNING, "HID - Rcvd L2CAP disc cfm, unknown CID: 0x%x", l2cap_cid);
        return;
    }

    HIDH_TRACE(EVENT, "HID - Rcvd L2CAP disc cfm, CID: 0x%x", l2cap_cid);

    if (l2cap_cid == p_hcon->ctrl_cid)
        p_hcon->ctrl_cid = 0;
    else
        p_hcon->intr_cid = 0;

    if (!p_hcon->ctrl_cid && !p_hcon->intr_cid) {
        hh_cb.devices[dhandle].state = HID_DEV_NO_CONN;
        p_hcon->conn_state = HID_CONN_STATE_UNUSED;
        (*hh_cb.callback)(dhandle, HID_HDEV_EVT_CLOSE, p_hcon->disc_reason, NULL);
    }
}

static void hidh_l2cif_cong_ind(UINT16 l2cap_cid, BOOLEAN congested) {
    UINT8 dhandle;
    tHID_CONN* p_hcon = NULL;

    if ((dhandle = find_conn_by_cid(l2cap_cid)) != HID_HOST_MAX_DEVICES)
        p_hcon = &hh_cb.devices[dhandle].conn;

    if (!p_hcon) {
        HIDH_TRACE(WARNING, "HID - Rcvd L2CAP congestion status, unknown CID: 0x%x", l2cap_cid);
        return;
    }

    HIDH_TRACE(EVENT, "HID - Rcvd L2CAP congestion status, CID: 0x%x  Cong: %d", l2cap_cid, congested);

    if (congested)
        p_hcon->conn_flags |= HID_CONN_FLAGS_CONGESTED;
    else
        p_hcon->conn_flags &= ~HID_CONN_FLAGS_CONGESTED;
}

static void hidh_l2cif_data_ind(UINT16 l2cap_cid, BT_HDR* p_msg) {
    UINT8* p_data = (UINT8*)(p_msg + 1) + p_msg->offset;
    UINT8 ttype;
    UINT8 param;
    UINT8 rep_type;
    UINT8 evt;
    UINT8 dhandle;
    tHID_CONN* p_hcon = NULL;

    if ((dhandle = find_conn_by_cid(l2cap_cid)) != HID_HOST_MAX_DEVICES)
        p_hcon = &hh_cb.devices[dhandle].conn;

    if (!p_hcon) {
        HIDH_TRACE(WARNING, "HID - Rcvd L2CAP data, unknown CID: 0x%x", l2cap_cid);
        GKI_freebuf(p_msg);
        return;
    }

    ttype = HID_GET_TRANS_FROM_HDR(*p_data);
    param = HID_GET_PARAM_FROM_HDR(*p_data);
    rep_type = param & HID_PAR_REP_TYPE_MASK;
    ++p_data;

    --p_msg->len;
    ++p_msg->offset;

    switch (ttype) {
    case HID_TRANS_HANDSHAKE:
        (*hh_cb.callback)(dhandle, HID_HDEV_EVT_HANDSHAKE, param, NULL);
        GKI_freebuf(p_msg);
        break;

    case HID_TRANS_CONTROL:
        switch (param) {
        case HID_PAR_CONTROL_VIRTUAL_CABLE_UNPLUG:
            hidh_conn_disconnect(dhandle);
            (*hh_cb.callback)(dhandle, HID_HDEV_EVT_VC_UNPLUG, 0, NULL);
            break;

        default:
            break;
        }

        GKI_freebuf(p_msg);
        break;

    case HID_TRANS_DATA:
        evt = hh_cb.devices[dhandle].conn.intr_cid == l2cap_cid ? HID_HDEV_EVT_INTR_DATA : HID_HDEV_EVT_CTRL_DATA;
        (*hh_cb.callback)(dhandle, evt, rep_type, p_msg);
        break;

    case HID_TRANS_DATAC:
        evt = hh_cb.devices[dhandle].conn.intr_cid == l2cap_cid ? HID_HDEV_EVT_INTR_DATC : HID_HDEV_EVT_CTRL_DATC;
        (*hh_cb.callback)(dhandle, evt, rep_type, p_msg);
        break;

    default:
        GKI_freebuf(p_msg);
        break;
    }
}

#if 0

UINT8 hidh_conn_snd_data(UINT8 dhandle /* r1+0x8 */, UINT8 trans_type /* r25 */,
                         UINT8 param /* r22 */, UINT16 data /* r24 */,
                         UINT8 report_id /* r23 */, BT_HDR *buf /* r31 */)
{
	struct hid_conn *p_hcon; // r30
	BT_HDR *p_buf;           // r29
	UINT8 *p_out;            // r27
	UINT16 bytes_copied;     // r28
	UINT8 seg_req;           // r1+0xC
	UINT16 data_size;        // r26
	UINT16 cid;              // r1+0xE
	UINT8 pool_id;           // r1+0xB
	UINT8 use_data;          // r1+0xA
	UINT8 blank_datc;        // r1+0x9

	// References
	// -> struct host_ctb hh_cb;
}

#endif
tHID_STATUS hidh_conn_snd_data(UINT8 dhandle, UINT8 trans_type, UINT8 param, UINT16 data, UINT8 report_id, BT_HDR* buf) {
    tHID_CONN* p_hcon = &hh_cb.devices[dhandle].conn;
    BT_HDR* p_buf;
    UINT8* p_out;
    UINT16 bytes_copied;
    BOOLEAN seg_req = FALSE;
    UINT16 data_size;
    UINT16 cid;
    UINT8 pool_id;
    UINT8 use_data = 0;
    BOOLEAN blank_datc = FALSE;

    if (p_hcon->conn_flags & HID_CONN_FLAGS_CONGESTED) {
        if (buf)
            GKI_freebuf(buf);

        return HID_ERR_CONGESTED;
    }

    switch (trans_type) {
    case HID_TRANS_CONTROL:
    case HID_TRANS_GET_REPORT:
    case HID_TRANS_SET_REPORT:
    case HID_TRANS_GET_PROTOCOL:
    case HID_TRANS_SET_PROTOCOL:
    case HID_TRANS_GET_IDLE:
    case HID_TRANS_SET_IDLE:
        cid = p_hcon->ctrl_cid;
        pool_id = HID_CONTROL_POOL_ID;
        break;

    case HID_TRANS_DATA:
        cid = p_hcon->intr_cid;
        pool_id = HID_INTERRUPT_POOL_ID;
        break;

    default:
        return HID_ERR_INVALID_PARAM;
    }

    if (trans_type == HID_TRANS_SET_IDLE)
        use_data = 1;
    else if (trans_type == HID_TRANS_GET_REPORT && (param & 0x08) != 0)
        use_data = 2;

    do {
        if (buf == NULL || blank_datc) {
            if ((p_buf = GKI_getpoolbuf(pool_id)) == NULL)
                return HID_ERR_NO_RESOURCES;

            p_buf->offset = L2CAP_MIN_OFFSET;
            seg_req = FALSE;
            data_size = 0;
            bytes_copied = 0;
            blank_datc = FALSE;
        } else if (buf->len > p_hcon->rem_mtu_size - 1) {
            if ((p_buf = GKI_getpoolbuf(pool_id)) == NULL)
                return HID_ERR_NO_RESOURCES;

            p_buf->offset = L2CAP_MIN_OFFSET;
            seg_req = TRUE;
            data_size = buf->len;
            bytes_copied = p_hcon->rem_mtu_size - 1;
        } else {
            p_buf = buf;
            p_buf->offset -= 1;
            seg_req = FALSE;
            data_size = buf->len;
            bytes_copied = buf->len;
        }

        p_out = (UINT8*)(p_buf + 1) + p_buf->offset;
        *p_out++ = HID_BUILD_HDR(trans_type, param);

        if (trans_type == HID_TRANS_GET_REPORT && report_id != 0) {
            *p_out = report_id;
            data_size = bytes_copied = 1;
        }

        if (seg_req) {
            memcpy(p_out, (UINT8*)(buf + 1) + buf->offset, bytes_copied);
            buf->offset += bytes_copied;
            buf->len -= bytes_copied;
        } else if (use_data == 1) {
            p_out[bytes_copied] = data & 0xff;
        } else if (use_data == 2) {
            p_out[bytes_copied] = data & 0xff;
            1 [p_out + bytes_copied] = (data >> 8) & 0xff;
        }

        p_buf->len = bytes_copied + 1 + use_data;
        data_size -= bytes_copied;

        if (p_hcon->conn_flags & HID_CONN_FLAGS_CONGESTED || !L2CA_DataWrite(cid, p_buf)) {
            return HID_ERR_CONGESTED;
        }

        if (data_size) {
            trans_type = HID_TRANS_DATAC;
        } else if (bytes_copied == p_hcon->rem_mtu_size - 1) {
            trans_type = HID_TRANS_DATAC;
            blank_datc = TRUE;
        }
    } while (data_size || blank_datc);

    return HID_SUCCESS;
}

tHID_STATUS hidh_conn_initiate(UINT8 dhandle) {
    tHID_HOST_DEV_CTB* p_dev = &hh_cb.devices[dhandle];

    if (p_dev->conn.conn_state != HID_CONN_STATE_UNUSED)
        return HID_ERR_CONN_IN_PROCESS;

    p_dev->conn.ctrl_cid = 0;
    p_dev->conn.intr_cid = 0;
    p_dev->conn.disc_reason = HID_L2CAP_CONN_FAIL;
    p_dev->conn.conn_flags = HID_CONN_FLAGS_IS_ORIG;

    if ((p_dev->conn.ctrl_cid = L2CA_ConnectReq(HID_PSM_CONTROL, p_dev->addr)) == 0) {
        HIDH_TRACE(WARNING, "HID - Originate failed");

        /* ERRATUM: pointer subtraction already implicitly divides by size of
         * pointed to type; doing it again manually is incorrect
         */
        dhandle = (p_dev - hh_cb.devices) / sizeof *p_dev;  // NOLINT

        (*hh_cb.callback)(dhandle, HID_HDEV_EVT_CLOSE, HID_ERR_L2CAP_FAILED, NULL);
    } else {
        p_dev->conn.conn_state = HID_CONN_STATE_CONNECTING_CTRL;
    }

    return HID_SUCCESS;
}

static UINT8 find_conn_by_cid(UINT16 cid) {
    UINT8 xx;

    for (xx = 0; xx < HID_HOST_MAX_DEVICES; xx++) {
        if (hh_cb.devices[xx].in_use && hh_cb.devices[xx].conn.conn_state != HID_CONN_STATE_UNUSED &&
            (hh_cb.devices[xx].conn.ctrl_cid == cid || hh_cb.devices[xx].conn.intr_cid == cid)) {
            break;
        }
    }

    return xx;
}

void hidh_conn_dereg(void) {
    L2CA_Deregister(HID_PSM_CONTROL);
    L2CA_Deregister(HID_PSM_INTERRUPT);
}

static void hidh_conn_retry(UINT8 dhandle) {
    tHID_HOST_DEV_CTB* p_dev = &hh_cb.devices[dhandle];

    p_dev->conn.conn_state = HID_CONN_STATE_UNUSED;
    p_dev->conn.timer_entry.param = (TIMER_PARAM_TYPE)dhandle;
    btu_start_timer(&p_dev->conn.timer_entry, BTU_TTYPE_HID_HOST_REPAGE_TO, HID_HOST_REPAGE_WIN);
}
