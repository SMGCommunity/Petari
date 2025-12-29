#include "hidd_int.h"

/*******************************************************************************
 * headers
 */

#include "revolution/types.h"
#include <cstring>

#include "bt_trace.h"
#include "bt_types.h"
#include "data_types.h"

#include "btm_api.h"  // BTM_SetQoS
#include "btm_int.h"
#include "gki.h"  // GKI_freebuf
#include "hid_conn.h"
#include "hidd_api.h"
#include "hiddefs.h"
#include "l2c_api.h"

/*******************************************************************************
 * local function declarations
 */

static void hidd_l2cif_connect_ind(BD_ADDR bd_addr, UINT16 l2cap_cid, UINT16 psm, UINT8 l2cap_id);
static void hidd_l2cif_connect_cfm(UINT16 l2cap_cid, UINT16 result);
static void hidd_l2cif_config_ind(UINT16 l2cap_cid, tL2CAP_CFG_INFO* p_cfg);
static void hidd_l2cif_config_cfm(UINT16 l2cap_cid, tL2CAP_CFG_INFO* p_cfg);
static void hidd_l2cif_disconnect_ind(UINT16 l2cap_cid, BOOLEAN ack_needed);
static void hidd_l2cif_disconnect_cfm(UINT16 l2cap_cid, UINT16 result);
static void hidd_l2cif_cong_ind(UINT16 l2cap_cid, BOOLEAN congested);
static void hidd_l2cif_data_ind(UINT16 l2cap_cid, BT_HDR* p_msg);

/*******************************************************************************
 * variables
 */

// .rodata
static tL2CAP_APPL_INFO const reg_info = {
    &hidd_l2cif_connect_ind,    &hidd_l2cif_connect_cfm,    NULL, &hidd_l2cif_config_ind, &hidd_l2cif_config_cfm,
    &hidd_l2cif_disconnect_ind, &hidd_l2cif_disconnect_cfm, NULL, &hidd_l2cif_data_ind,   &hidd_l2cif_cong_ind,
};

/*******************************************************************************
 * functions
 */

tHID_STATUS hidd_conn_reg(void) {
    if (!L2CA_Register(17, (tL2CAP_APPL_INFO*)&reg_info)) {
        HIDD_TRACE(ERROR, "HID Control Registration failed");
        return HID_ERR_L2CAP_FAILED;
    }

    if (!L2CA_Register(19, (tL2CAP_APPL_INFO*)&reg_info)) {
        L2CA_Deregister(17);

        HIDD_TRACE(ERROR, "HID Interrupt Registration failed");
        return HID_ERR_L2CAP_FAILED;
    }

    hd_cb.l2cap_ctrl_cfg.flush_to_present = TRUE;
    hd_cb.l2cap_ctrl_cfg.flush_to = 0xffff;

    hd_cb.l2cap_int_cfg.flush_to_present = TRUE;
    hd_cb.l2cap_int_cfg.flush_to = 0xffff;

    if (hd_cb.use_qos_flg == TRUE) {
        memcpy(&hd_cb.l2cap_ctrl_cfg.qos, &hd_cb.qos_info.ctrl_ch, sizeof hd_cb.l2cap_ctrl_cfg.qos);

        hd_cb.l2cap_ctrl_cfg.qos_present = TRUE;

        memcpy(&hd_cb.l2cap_int_cfg.qos, &hd_cb.qos_info.int_ch, sizeof hd_cb.l2cap_int_cfg.qos);

        hd_cb.l2cap_int_cfg.qos_present = TRUE;
    } else {
        hd_cb.l2cap_ctrl_cfg.qos_present = FALSE;
        hd_cb.l2cap_int_cfg.qos_present = FALSE;
    }

    hd_cb.conn.conn_state = HID_CONN_STATE_UNUSED;

    return HID_SUCCESS;
}

void hidd_conn_dereg(void) {
    L2CA_Deregister(17);
    L2CA_Deregister(19);
}

void hidd_conn_disconnect(void) {
    tHID_CONN* p_hcon = &hd_cb.conn;
    tHID_DEV_PWR_MD act_pm = {0};

    HIDD_TRACE(EVENT, "HID - disconnect");

    hidd_pm_stop();
    hidd_pm_set_power_mode(&act_pm);

    if (p_hcon->ctrl_cid || p_hcon->intr_cid) {
        p_hcon->conn_state = HID_CONN_STATE_DISCONNECTING;

        if (p_hcon->intr_cid)
            L2CA_DisconnectReq(p_hcon->intr_cid);

        if (p_hcon->ctrl_cid)
            L2CA_DisconnectReq(p_hcon->ctrl_cid);
    } else {
        p_hcon->conn_state = HID_CONN_STATE_UNUSED;
    }
}

tHID_STATUS hidd_conn_initiate(void) {
    tHID_CONN* p_hcon = &hd_cb.conn;

    HIDD_TRACE(EVENT, "HID - Originate started");

    p_hcon->ctrl_cid = 0;
    p_hcon->intr_cid = 0;
    p_hcon->conn_flags = HID_CONN_FLAGS_IS_ORIG;

    if ((p_hcon->ctrl_cid = L2CA_ConnectReq(17, hd_cb.host_addr)) == 0) {
        HIDD_TRACE(WARNING, "HID - Originate failed");
        return HID_ERR_L2CAP_FAILED;
    } else {
        p_hcon->conn_state = HID_CONN_STATE_CONNECTING_CTRL;
        return HID_SUCCESS;
    }
}

void hidd_sec_check_complete_term(BD_ADDR bd_addr, void* p_ref_data, UINT8 res) {
    tHID_CONN* p_hcon = &hd_cb.conn;

    if (res == 0 && p_hcon->conn_state == HID_CONN_STATE_SECURITY) {
        p_hcon->conn_state = HID_CONN_STATE_CONNECTING_INTR;
        L2CA_ConnectRsp(bd_addr, p_hcon->ctrl_id, p_hcon->ctrl_cid, 0, 0);
        L2CA_ConfigReq(p_hcon->ctrl_cid, &hd_cb.l2cap_ctrl_cfg);
    } else if (res != 0) {
        hidd_conn_disconnect();
    }
}

static void hidd_l2cif_connect_ind(BD_ADDR bd_addr, UINT16 l2cap_cid, UINT16 psm, UINT8 l2cap_id) {
    tHID_CONN* p_hcon = &hd_cb.conn;
    BOOLEAN bAccept = TRUE;
    tL2CAP_CFG_INFO* p_l2cfg = NULL;

    HIDD_TRACE(EVENT, "HID - Rcvd L2CAP conn ind, PSM: 0x%04x  CID 0x%x", psm, l2cap_cid);

    if (hd_cb.virtual_cable && hd_cb.host_known && memcmp(hd_cb.host_addr, bd_addr, BD_ADDR_LEN) != 0) {
        bAccept = FALSE;
    } else if (psm == 19) {
        if (p_hcon->ctrl_cid == 0) {
            HIDD_TRACE(WARNING, "HID - Rcvd INTR L2CAP conn ind, but no CTL channel");
            bAccept = FALSE;
        }

        if (p_hcon->conn_state != 2) {
            HIDD_TRACE(WARNING, "HID - Rcvd INTR L2CAP conn ind, wrong state: %d", p_hcon->conn_state);
            bAccept = FALSE;
        }

        p_l2cfg = &hd_cb.l2cap_int_cfg;
    } else {
        if (p_hcon->conn_state != 0) {
            HIDD_TRACE(WARNING, "HID - Rcvd CTL L2CAP conn ind, wrong state: %d", p_hcon->conn_state);
            bAccept = FALSE;
        }

        p_l2cfg = &hd_cb.l2cap_ctrl_cfg;
    }

    if (!bAccept) {
        L2CA_ConnectRsp(bd_addr, l2cap_id, l2cap_cid, 4, 0);
        return;
    }

    if (psm == 17) {
        p_hcon->conn_flags = 0;
        p_hcon->ctrl_cid = l2cap_cid;
        p_hcon->ctrl_id = l2cap_id;
        p_hcon->conn_state = 6;

        btm_sec_mx_access_request(bd_addr, 17, FALSE, 7, hd_cb.sec_mask == 0 ? 2 : 1, &hidd_sec_check_complete_term, NULL);
        return;
    }

    p_hcon->conn_state = 3;
    p_hcon->intr_cid = l2cap_cid;

    L2CA_ConnectRsp(bd_addr, l2cap_id, l2cap_cid, 0, 0);
    L2CA_ConfigReq(l2cap_cid, p_l2cfg);

    memcpy(hd_cb.host_addr, bd_addr, BD_ADDR_LEN);

    HIDD_TRACE(EVENT, "HID - Rcvd L2CAP conn ind, sent config req, PSM: 0x%04x  CID 0x%x", psm, l2cap_cid);
}

void hidd_sec_check_complete_orig(BD_ADDR bd_addr, void* p_ref_data, UINT8 res) {
    UINT32 reason;
    tHID_CONN* p_hcon = &hd_cb.conn;

    if (!res && p_hcon->conn_state == 6) {
        HIDD_TRACE(EVENT, "HID Device - Originator security pass.");

        if ((p_hcon->intr_cid = L2CA_ConnectReq(19, hd_cb.host_addr)) == 0) {
            HIDD_TRACE(WARNING, "HID - INTR Originate failed");

            hidd_conn_disconnect();

            reason = 0x200;
            hd_cb.conn_tries = 16;

            hidd_mgmt_process_evt(3, &reason);
            return;
        }

        p_hcon->conn_state = 2;
    }

    if (res && p_hcon->conn_state == 6)
        hidd_conn_disconnect();
}

static void hidd_l2cif_connect_cfm(UINT16 l2cap_cid, UINT16 result) {
    UINT32 reason;
    tHID_CONN* p_hcon = &hd_cb.conn;
    tL2CAP_CFG_INFO* p_l2cfg;

    if ((p_hcon->ctrl_cid != l2cap_cid && p_hcon->intr_cid != l2cap_cid) || !(p_hcon->conn_flags & HID_CONN_FLAGS_IS_ORIG) ||
        (l2cap_cid == p_hcon->ctrl_cid && p_hcon->conn_state != HID_CONN_STATE_CONNECTING_CTRL) ||
        (l2cap_cid == p_hcon->intr_cid && p_hcon->conn_state != HID_CONN_STATE_CONNECTING_INTR)) {
        HIDD_TRACE(WARNING, "HID - Rcvd unexpected conn cnf, CID 0x%x ", l2cap_cid);
        return;
    }

    if (result != 0) {
        if (l2cap_cid == p_hcon->ctrl_cid)
            p_hcon->ctrl_cid = 0;
        else
            p_hcon->intr_cid = 0;

        reason = result | 0x100;

        hidd_conn_disconnect();

        if (result != 8 && result != 31 && result != 4)
            hd_cb.conn_tries = 16;

        hidd_mgmt_process_evt(3, &reason);
        return;
    }

    if (l2cap_cid == p_hcon->ctrl_cid) {
        p_hcon->conn_state = 6;

        btm_sec_mx_access_request(hd_cb.host_addr, 17, TRUE, 7, hd_cb.sec_mask == 0 ? 2 : 1, &hidd_sec_check_complete_orig, NULL);

        p_l2cfg = &hd_cb.l2cap_int_cfg;
    } else {
        p_hcon->conn_state = 3;
        p_l2cfg = &hd_cb.l2cap_ctrl_cfg;
    }

    L2CA_ConfigReq(l2cap_cid, p_l2cfg);

    HIDD_TRACE(EVENT, "HID - got CTRL conn cnf, sent cfg req, CID: 0x%x", l2cap_cid);
}

void hidd_l2c_connected(tHID_CONN* p_hcon) {
    p_hcon->conn_state = 4;

    if (hd_cb.use_qos_flg)
        BTM_SetQoS(hd_cb.host_addr, &hd_cb.qos_info.hci, NULL);

    hidd_mgmt_process_evt(0, &hd_cb);
    hidd_pm_init();
    hidd_pm_start();
}

static void hidd_l2cif_config_ind(UINT16 l2cap_cid, tL2CAP_CFG_INFO* p_cfg) {
    tHID_CONN* p_hcon = &hd_cb.conn;

    if (p_hcon->ctrl_cid != l2cap_cid && p_hcon->intr_cid != l2cap_cid) {
        HIDD_TRACE(WARNING, "HID - Rcvd L2CAP cfg ind, unknown CID: 0x%x", l2cap_cid);
        return;
    }

    HIDD_TRACE(EVENT, "HID - Rcvd cfg ind, sent cfg cfm, CID: 0x%x", l2cap_cid);

    if (!p_cfg->mtu_present || p_cfg->mtu > 0x40)
        p_hcon->rem_mtu_size = 0x40;
    else
        p_hcon->rem_mtu_size = p_cfg->mtu;

    p_cfg->flush_to_present = 0;
    p_cfg->mtu_present = 0;
    p_cfg->result = 0;

    L2CA_ConfigRsp(l2cap_cid, p_cfg);

    if (l2cap_cid == p_hcon->ctrl_cid)
        p_hcon->conn_flags |= HID_CONN_FLAGS_THEIR_CTRL_CFG_DONE;
    else
        p_hcon->conn_flags |= HID_CONN_FLAGS_THEIR_INTR_CFG_DONE;

    if ((p_hcon->conn_flags & HID_CONN_FLAGS_ALL_CONFIGURED) == HID_CONN_FLAGS_ALL_CONFIGURED && p_hcon->conn_state == 3) {
        hidd_l2c_connected(p_hcon);
    }
}

static void hidd_l2cif_config_cfm(UINT16 l2cap_cid, tL2CAP_CFG_INFO* p_cfg) {
    UINT32 reason;
    tHID_CONN* p_hcon = &hd_cb.conn;

    HIDD_TRACE(EVENT, "HID - Rcvd cfg cfm, CID: 0x%x  Result: %d", l2cap_cid, p_cfg->result);

    if (p_hcon->ctrl_cid != l2cap_cid && p_hcon->intr_cid != l2cap_cid) {
        HIDD_TRACE(WARNING, "HID - Rcvd L2CAP cfg ind, unknown CID: 0x%x", l2cap_cid);
        return;
    }

    if (p_cfg->result != 0) {
        reason = 0x400 | p_cfg->result;

        hidd_conn_disconnect();

        hd_cb.conn_tries = 16;

        hidd_mgmt_process_evt(3, &reason);
        return;
    }

    if (l2cap_cid == p_hcon->ctrl_cid)
        p_hcon->conn_flags |= HID_CONN_FLAGS_OUR_CTRL_CFG_DONE;
    else
        p_hcon->conn_flags |= HID_CONN_FLAGS_OUR_INTR_CFG_DONE;

    if ((p_hcon->conn_flags & HID_CONN_FLAGS_ALL_CONFIGURED) == HID_CONN_FLAGS_ALL_CONFIGURED && p_hcon->conn_state == 3) {
        hidd_l2c_connected(p_hcon);
    }
}

static void hidd_l2cif_disconnect_ind(UINT16 l2cap_cid, BOOLEAN ack_needed) {
    tHID_CONN* p_hcon = &hd_cb.conn;
    UINT16 disc_res = 0;
    UINT8 evt = 1;

    if (p_hcon->ctrl_cid != l2cap_cid && p_hcon->intr_cid != l2cap_cid) {
        HIDD_TRACE(WARNING, "HID - Rcvd L2CAP disc, unknown CID: 0x%x", l2cap_cid);
        return;
    }

    if (ack_needed)
        L2CA_DisconnectRsp(l2cap_cid);

    HIDD_TRACE(EVENT, "HID - Rcvd L2CAP disc, CID: 0x%x", l2cap_cid);

    p_hcon->conn_state = 5;

    if (l2cap_cid == p_hcon->ctrl_cid)
        p_hcon->ctrl_cid = 0;
    else
        p_hcon->intr_cid = 0;

    if (p_hcon->ctrl_cid)
        return;

    if (p_hcon->intr_cid)
        return;

    if (!ack_needed)
        disc_res = btm_get_acl_disc_reason_code();

    HIDD_TRACE(EVENT, "disc_res: 0x%x", disc_res);

    if (disc_res == 8 || disc_res == 31)
        evt = 2;

    p_hcon->conn_state = 0;

    hidd_mgmt_process_evt(evt, &disc_res);
    hidd_pm_stop();
}

static void hidd_l2cif_disconnect_cfm(UINT16 l2cap_cid, UINT16 result) {
    UINT16 disc_res = 0;
    tHID_CONN* p_hcon = &hd_cb.conn;

    if (p_hcon->ctrl_cid != l2cap_cid && p_hcon->intr_cid != l2cap_cid) {
        HIDD_TRACE(WARNING, "HID - Rcvd L2CAP disc cfm, unknown CID: 0x%x", l2cap_cid);
        return;
    }

    HIDD_TRACE(EVENT, "HID - Rcvd L2CAP disc cfm, CID: 0x%x", l2cap_cid);

    if (l2cap_cid == p_hcon->ctrl_cid)
        p_hcon->ctrl_cid = 0;
    else
        p_hcon->intr_cid = 0;

    if (p_hcon->ctrl_cid)
        return;

    if (p_hcon->intr_cid)
        return;

    p_hcon->conn_state = 0;

    hidd_mgmt_process_evt(1, &disc_res);
    hidd_pm_stop();
}

static void hidd_l2cif_cong_ind(UINT16 l2cap_cid, BOOLEAN congested) {
    tHID_CONN* p_hcon = &hd_cb.conn;

    if (p_hcon->ctrl_cid != l2cap_cid && p_hcon->intr_cid != l2cap_cid) {
        HIDD_TRACE(WARNING, "HID - Rcvd L2CAP congestion status, unknown CID: 0x%x", l2cap_cid);
        return;
    }

    HIDD_TRACE(EVENT, "HID - Rcvd L2CAP congestion status, CID: 0x%x  Cong: %d", l2cap_cid, congested);

    if (congested)
        p_hcon->conn_flags |= HID_CONN_FLAGS_CONGESTED;
    else
        p_hcon->conn_flags &= ~HID_CONN_FLAGS_CONGESTED;

    (*hd_cb.callback)(14, p_hcon->ctrl_cid == l2cap_cid, (tHID_DEV_REG_INFO_CBACK_DATA*)&congested);
}

static void hidd_l2cif_data_ind(UINT16 l2cap_cid, BT_HDR* p_msg) {
    tHID_CONN* p_hcon = &hd_cb.conn;
    UINT8* p_data = (UINT8*)(p_msg + 1) + p_msg->offset;
    UINT8 ttype;
    UINT8 param;
    UINT8 rep_type;
    UINT8 idle_rate;
    tHID_DEV_REP_DATA get_rep;
    BOOLEAN suspend = FALSE;

    if (p_hcon->ctrl_cid != l2cap_cid && p_hcon->intr_cid != l2cap_cid) {
        HIDD_TRACE(WARNING, "HID - Rcvd L2CAP data, unknown CID: 0x%x", l2cap_cid);
        GKI_freebuf(p_msg);
        return;
    }

    ttype = HID_GET_TRANS_FROM_HDR(*p_data);
    param = HID_GET_PARAM_FROM_HDR(*p_data);
    rep_type = param & HID_PAR_REP_TYPE_MASK;
    ++p_data;

    switch (ttype) {
    case 1:
        switch (param) {
        case 0:
        case 1:
        case 2:
        case 4:
            break;

        case 3:
            suspend = TRUE;
            hidd_pm_suspend_evt();
            break;

        case 5:
            hidd_mgmt_process_evt(5, NULL);
            hd_cb.unplug_on = TRUE;
            break;

        default:
            GKI_freebuf(p_msg);
            HID_DevHandShake(4);
            return;
        }

        (*hd_cb.callback)(5, param, NULL);
        break;

    case 4:
        if (param & 8) {
            STREAM_TO_UINT8(p_data, &get_rep.rep_id);
            STREAM_TO_UINT16(p_data, &hd_cb.get_rep_buf_sz);
        } else {
            hd_cb.get_rep_buf_sz = 0;
        }

        get_rep.rep_type = param & 3;
        (*hd_cb.callback)(6, param, (tHID_DEV_REG_INFO_CBACK_DATA*)&get_rep);
        break;

    case 5:
        (*hd_cb.callback)(7, rep_type, (tHID_DEV_REG_INFO_CBACK_DATA*)&p_msg);
        break;

    case 10:
        (*hd_cb.callback)(12, rep_type, (tHID_DEV_REG_INFO_CBACK_DATA*)&p_msg);
        break;

    case 11:
        (*hd_cb.callback)(13, rep_type, (tHID_DEV_REG_INFO_CBACK_DATA*)&p_msg);
        break;

    case 6:
        (*hd_cb.callback)(8, 0, NULL);
        break;

    case 7:
        (*hd_cb.callback)(9, param & 1, NULL);
        break;

    case 8:
        (*hd_cb.callback)(10, 0, NULL);
        break;

    case 9:
        STREAM_TO_UINT8(p_data, &idle_rate);
        (*hd_cb.callback)(11, idle_rate, NULL);
        break;

    default:
        GKI_freebuf(p_msg);
        HID_DevHandShake(3);
        return;
    }

    if (!suspend)
        hidd_pm_start();

    if (ttype != 5 && ttype != 10 && ttype != 11)
        GKI_freebuf(p_msg);
}

tHID_STATUS hidd_conn_snd_data(tSND_DATA_PARAMS* p_data) {
    tHID_CONN* p_hcon = &hd_cb.conn;
    BT_HDR* p_buf;
    UINT8* p_out;
    UINT16 bytes_copied;
    UINT8 seg_req = 0;
    UINT16 data_size;
    UINT16 cid;
    UINT8 pool_id = p_data->ctrl_ch != 0 ? 2 : 2;
    UINT16 rem_mtu;
    tSND_DATA_PARAMS datc;

    hidd_pm_start();

    if (p_hcon->conn_flags & 32)
        return 8;

    if (!p_data)
        return 5;

    if (p_data->trans_type == 10 && p_data->ctrl_ch && hd_cb.get_rep_buf_sz) {
        rem_mtu = hd_cb.get_rep_buf_sz;
        hd_cb.get_rep_buf_sz = 0;
    } else {
        rem_mtu = p_hcon->rem_mtu_size;
    }

    do {
        if (!p_data->buf) {
            p_buf = GKI_getpoolbuf(pool_id);
            if (!p_buf)
                return 3;

            p_buf->offset = 9;
            seg_req = 0;
            data_size = 0;
            bytes_copied = 0;
        } else if (p_data->buf->len > rem_mtu - 1) {
            p_buf = GKI_getpoolbuf(pool_id);
            if (!p_buf)
                return 3;

            p_buf->offset = 9;
            seg_req = 1;
            data_size = p_data->buf->len;
            bytes_copied = rem_mtu - 1;
        } else {
            p_buf = p_data->buf;

            p_buf->offset -= 1;
            seg_req = 0;
            data_size = p_data->buf->len;
            bytes_copied = p_data->buf->len;
        }

        p_out = (UINT8*)(p_buf + 1) + p_buf->offset;
        *p_out++ = (p_data->trans_type << 4) | (p_data->param & 0xf);

        if (seg_req) {
            memcpy(p_out, (UINT8*)(p_data->buf + 1) + p_data->buf->offset, bytes_copied);

            p_data->buf->offset += bytes_copied;
            p_data->buf->len -= bytes_copied;
        }

        p_buf->len = bytes_copied + 1;
        data_size -= bytes_copied;

        cid = p_data->ctrl_ch != 0 ? p_hcon->ctrl_cid : p_hcon->intr_cid;

        if (p_hcon->conn_flags & 32 || !L2CA_DataWrite(cid, p_buf))
            return 8;

        if (data_size)
            p_data->trans_type = 11;
    } while (data_size);

    if (bytes_copied == rem_mtu - 1) {
        datc.buf = NULL;
        datc.ctrl_ch = p_data->ctrl_ch;
        datc.param = p_data->param;
        datc.trans_type = 11;

        hidd_conn_snd_data(&datc);
    }

    return 0;
}
