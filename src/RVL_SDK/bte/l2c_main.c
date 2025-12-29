#include "l2c_int.h"

/* Original source:
 * bluedroid <android.googlesource.com/platform/external/bluetooth/bluedroid>
 * stack/l2cap/l2c_main.c
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
#include "btu.h"
#include "gki.h"
#include "hcidefs.h"
#include "hcimsgs.h"
#include "l2c_api.h"
#include "l2cdefs.h"

/*******************************************************************************
 * local function declarations
 */

static void process_l2cap_cmd(tL2C_LCB* p_lcb, UINT8* p, UINT16 pkt_len);

/*******************************************************************************
 * variables
 */

// .bss
tL2C_CB l2cb;

/*******************************************************************************
 * functions
 */

void l2c_init(void) {
    INT16 xx;

    memset(&l2cb, 0, sizeof l2cb);

    for (xx = 0; xx < MAX_L2CAP_CHANNELS - 1; ++xx)
        l2cb.ccb_pool[xx].p_next_ccb = &l2cb.ccb_pool[xx + 1];

    l2cb.p_free_ccb_first = &l2cb.ccb_pool[0];
    l2cb.p_free_ccb_last = &l2cb.ccb_pool[MAX_L2CAP_CHANNELS - 1];
    l2cb.desire_role = HCI_ROLE_MASTER;
    l2cb.idle_timeout = L2CAP_LINK_INACTIVITY_TOUT;
    l2cb.l2cap_trace_level = BT_TRACE_LEVEL_NONE;
}

void l2c_rcv_acl_data(BT_HDR* p_msg) {
    UINT8* p = (UINT8*)(p_msg + 1) + p_msg->offset;
    UINT16 handle;
    UINT16 hci_len;
    UINT8 pkt_type;
    tL2C_LCB* p_lcb;
    tL2C_CCB* p_ccb = NULL;
    UINT16 l2cap_len;
    UINT16 rcv_cid;
    UINT16 bcst_psm;

    STREAM_TO_UINT16(p, &handle);
    pkt_type = HCID_GET_EVENT(handle);
    handle = HCID_GET_HANDLE(handle);

    if (pkt_type != L2CAP_PKT_START) {
        L2CAP_TRACE(WARNING, "L2CAP - expected pkt start, got: %d", pkt_type);
        GKI_freebuf(p_msg);
        return;
    }

    if ((p_lcb = l2cu_find_lcb_by_handle(handle)) == NULL) {
        UINT8 cmd_code;

        STREAM_TO_UINT16(p, &hci_len);
        STREAM_TO_UINT16(p, &l2cap_len);
        STREAM_TO_UINT16(p, &rcv_cid);
        STREAM_TO_UINT8(p, &cmd_code);

        if (p_msg->layer_specific == 0 && rcv_cid == L2CAP_SIGNALLING_CID && (cmd_code == L2CAP_CMD_INFO_REQ || cmd_code == L2CAP_CMD_CONN_REQ) &&
            !btm_sec_is_bonding(handle)) {
            L2CAP_TRACE(WARNING,
                        "L2CAP - holding ACL for unknown handle:%d ls:%d cid:%d "
                        "opcode:%d cur count:%d",
                        handle, p_msg->layer_specific, rcv_cid, cmd_code, l2cb.rcv_hold_q.count);
            p_msg->layer_specific = 2;
            GKI_enqueue(&l2cb.rcv_hold_q, p_msg);

            if (l2cb.rcv_hold_q.count == 1) {
                btu_start_timer(&l2cb.rcv_hold_tle, BTU_TTYPE_L2CAP_HOLD, BT_1SEC_TIMEOUT);
            }

            return;
        }

        GKI_freebuf(p_msg);
        return;
    }

    STREAM_TO_UINT16(p, &hci_len);
    p_msg->offset += 4;

    STREAM_TO_UINT16(p, &l2cap_len);
    STREAM_TO_UINT16(p, &rcv_cid);

    if (rcv_cid > 2)  // TODO: L2CAP_CONNECTIONLESS_CID?
    {
        if ((p_ccb = l2cu_find_ccb_by_cid(p_lcb, rcv_cid)) == NULL) {
            L2CAP_TRACE(WARNING, "L2CAP - unknown CID: %d", rcv_cid);
            GKI_freebuf(p_msg);
            return;
        }
    }

    if (hci_len >= L2CAP_PKT_OVERHEAD) {
        p_msg->len = hci_len - L2CAP_PKT_OVERHEAD;
        p_msg->offset += L2CAP_PKT_OVERHEAD;
    } else {
        L2CAP_TRACE(WARNING, "L2CAP - got incorrect hci header");
        GKI_freebuf(p_msg);
        return;
    }

    if (l2cap_len != p_msg->len) {
        L2CAP_TRACE(WARNING, "L2CAP - bad length in pkt. Exp: %d  Act: %d", l2cap_len, p_msg->len);

        GKI_freebuf(p_msg);
        return;
    }

    if (rcv_cid == L2CAP_SIGNALLING_CID) {
        process_l2cap_cmd(p_lcb, p, l2cap_len);
        GKI_freebuf(p_msg);
    } else if (rcv_cid == L2CAP_CONNECTIONLESS_CID) {
        STREAM_TO_UINT16(p, &bcst_psm);

        p_msg->offset += 2;
        p_msg->len -= 2;

        L2CAP_TRACE(DEBUG, "GOT CONNECTIONLESS DATA PSM:%d", bcst_psm);

        GKI_freebuf(p_msg);
    } else {
        if (!p_ccb)
            GKI_freebuf(p_msg);
        else
            l2c_csm_execute(p_ccb, 19, p_msg);
    }
}

static void process_l2cap_cmd(tL2C_LCB* p_lcb, UINT8* p, UINT16 pkt_len) {
    UINT8* p_pkt_end;
    UINT8* p_next_cmd;
    UINT8* p_cfg_end;
    UINT8* p_cfg_start;
    UINT8 cmd_code;
    UINT8 cfg_code;
    UINT8 cfg_len;
    UINT8 id;
    tL2C_CONN_INFO con_info;
    tL2CAP_CFG_INFO cfg_info;
    UINT16 rej_reason;
    UINT16 rej_mtu;
    UINT16 lcid;
    UINT16 rcid;
    UINT16 info_type;
    tL2C_CCB* p_ccb;
    tL2C_RCB* p_rcb;
    BOOLEAN cfg_rej;
    UINT16 cfg_rej_len;
    UINT16 cmd_len;

    p_next_cmd = p;
    p_pkt_end = p + pkt_len;

    while (TRUE) {
        if ((p = p_next_cmd) > p_pkt_end - 4)
            break;

        STREAM_TO_UINT8(p, &cmd_code);
        STREAM_TO_UINT8(p, &id);
        STREAM_TO_UINT16(p, &cmd_len);

        if ((p_next_cmd = p + cmd_len) > p_pkt_end) {
            L2CAP_TRACE(WARNING, "Command len bad  pkt_len: %d  cmd_len: %d  code: %d", pkt_len, cmd_len, cmd_code);
            break;
        }

        switch (cmd_code) {
        case L2CAP_CMD_REJECT:
            STREAM_TO_UINT16(p, &rej_reason);

            if (rej_reason == L2CAP_CMD_REJ_MTU_EXCEEDED) {
                STREAM_TO_UINT16(p, &rej_mtu);

                L2CAP_TRACE(WARNING, "L2CAP - MTU rej Handle: %d MTU: %d", p_lcb->handle, rej_mtu);
            }

            if (rej_reason == L2CAP_CMD_REJ_INVALID_CID) {
                STREAM_TO_UINT16(p, &rcid);
                STREAM_TO_UINT16(p, &lcid);

                L2CAP_TRACE(WARNING, "L2CAP - rej with CID invalid, LCID: %d RCID: %d", lcid, rcid);

                if (((p_ccb = l2cu_find_ccb_by_cid(p_lcb, lcid)) != NULL) && p_ccb->remote_cid == rcid) {
                    l2c_csm_execute(p_ccb, L2CEVT_LP_DISCONNECT_IND, NULL);
                }
            }

            break;

        case L2CAP_CMD_CONN_REQ:
            STREAM_TO_UINT16(p, &con_info.psm);
            STREAM_TO_UINT16(p, &rcid);

            if ((p_rcb = l2cu_find_rcb_by_psm(con_info.psm)) == NULL) {
                L2CAP_TRACE(WARNING, "L2CAP - rcvd conn req for unknown PSM: %d", con_info.psm);
                l2cu_reject_connection(p_lcb, rcid, id, L2CAP_CONN_NO_PSM);
                break;
            }

            if ((p_ccb = l2cu_allocate_ccb(p_lcb)) == NULL) {
                L2CAP_TRACE(ERROR, "L2CAP - unable to allocate CCB");
                l2cu_reject_connection(p_lcb, rcid, id, L2CAP_CONN_NO_RESOURCES);
                break;
            }

            p_ccb->remote_id = id;
            p_ccb->p_rcb = p_rcb;
            p_ccb->remote_cid = rcid;

            l2c_csm_execute(p_ccb, L2CEVT_L2CAP_CONNECT_REQ, &con_info);
            break;

        case L2CAP_CMD_CONN_RSP:
            STREAM_TO_UINT16(p, &con_info.remote_cid);
            STREAM_TO_UINT16(p, &lcid);
            STREAM_TO_UINT16(p, &con_info.l2cap_result);
            STREAM_TO_UINT16(p, &con_info.l2cap_status);

            if ((p_ccb = l2cu_find_ccb_by_cid(p_lcb, lcid)) == NULL) {
                L2CAP_TRACE(WARNING, "L2CAP - no CCB for conn rsp, LCID: %d RCID: %d", lcid, con_info.remote_cid);
                break;
            }
            if (p_ccb->local_id != id) {
                L2CAP_TRACE(WARNING, "L2CAP - con rsp - bad ID. Exp: %d Got: %d", p_ccb->local_id, id);
                break;
            }

            if (con_info.l2cap_result == L2CAP_CONN_OK)
                l2c_csm_execute(p_ccb, L2CEVT_L2CAP_CONNECT_RSP, &con_info);
            else if (con_info.l2cap_result == L2CAP_CONN_PENDING)
                l2c_csm_execute(p_ccb, L2CEVT_L2CAP_CONNECT_RSP_PND, &con_info);
            else
                l2c_csm_execute(p_ccb, L2CEVT_L2CAP_CONNECT_RSP_NEG, &con_info);

            break;

        case L2CAP_CMD_CONFIG_REQ:
            p_cfg_end = p + cmd_len;
            cfg_rej = FALSE;
            cfg_rej_len = 0;

            STREAM_TO_UINT16(p, &lcid);
            STREAM_TO_UINT16(p, &cfg_info.flags);

            p_cfg_start = p;

            cfg_info.flush_to_present = cfg_info.mtu_present = cfg_info.qos_present = cfg_info.fcr_present = FALSE;

            while (p < p_cfg_end) {
                STREAM_TO_UINT8(p, &cfg_code);
                STREAM_TO_UINT8(p, &cfg_len);

                switch (cfg_code & 0x7f) {
                case L2CAP_CFG_TYPE_MTU:
                    cfg_info.mtu_present = TRUE;
                    STREAM_TO_UINT16(p, &cfg_info.mtu);
                    break;

                case L2CAP_CFG_TYPE_FLUSH_TOUT:
                    cfg_info.flush_to_present = TRUE;
                    STREAM_TO_UINT16(p, &cfg_info.flush_to);
                    break;

                case L2CAP_CFG_TYPE_QOS:
                    cfg_info.qos_present = TRUE;
                    STREAM_TO_UINT8(p, &cfg_info.qos.qos_flags);
                    STREAM_TO_UINT8(p, &cfg_info.qos.service_type);
                    STREAM_TO_UINT32(p, &cfg_info.qos.token_rate);
                    STREAM_TO_UINT32(p, &cfg_info.qos.token_bucket_size);
                    STREAM_TO_UINT32(p, &cfg_info.qos.peak_bandwidth);
                    STREAM_TO_UINT32(p, &cfg_info.qos.latency);
                    STREAM_TO_UINT32(p, &cfg_info.qos.delay_variation);
                    break;

                case L2CAP_CFG_TYPE_FCR:
                    cfg_info.fcr_present = TRUE;
                    STREAM_TO_UINT8(p, &cfg_info.fcr.mode);
                    STREAM_TO_UINT8(p, &cfg_info.fcr.tx_win_sz);
                    STREAM_TO_UINT8(p, &cfg_info.fcr.max_transmit);
                    STREAM_TO_UINT16(p, &cfg_info.fcr.rtrans_tout);
                    STREAM_TO_UINT16(p, &cfg_info.fcr.mon_tout);
                    STREAM_TO_UINT16(p, &cfg_info.fcr.mps);
                    break;

                default:
                    if (cfg_len + L2CAP_CFG_OPTION_OVERHEAD <= cmd_len) {
                        p += cfg_len;

                        if (!(cfg_code & 0x80)) {
                            cfg_rej_len += cfg_len + L2CAP_CFG_OPTION_OVERHEAD;
                            cfg_rej = TRUE;
                        }
                    } else {
                        p = p_cfg_end;
                    }

                    break;
                }
            }

            if ((p_ccb = l2cu_find_ccb_by_cid(p_lcb, lcid)) != NULL) {
                p_ccb->remote_id = id;

                if (cfg_rej) {
                    l2cu_send_peer_config_rej(p_ccb, p_cfg_start, (UINT16)(cmd_len - L2CAP_CONFIG_REQ_LEN), cfg_rej_len);
                } else {
                    l2c_csm_execute(p_ccb, L2CEVT_L2CAP_CONFIG_REQ, &cfg_info);
                }
            } else {
                l2cu_send_peer_cmd_reject(p_lcb, L2CAP_CMD_REJ_INVALID_CID, id, 0, 0);
            }
            break;

        case L2CAP_CMD_CONFIG_RSP:
            p_cfg_end = p + cmd_len;
            STREAM_TO_UINT16(p, &lcid);
            STREAM_TO_UINT16(p, &cfg_info.flags);
            STREAM_TO_UINT16(p, &cfg_info.result);

            cfg_info.flush_to_present = cfg_info.mtu_present = cfg_info.qos_present = FALSE;
            cfg_info.fcr_present = FALSE;

            while (p < p_cfg_end) {
                STREAM_TO_UINT8(p, &cfg_code);
                STREAM_TO_UINT8(p, &cfg_len);

                switch (cfg_code & 0x7f) {
                case L2CAP_CFG_TYPE_MTU:
                    cfg_info.mtu_present = TRUE;
                    STREAM_TO_UINT16(p, &cfg_info.mtu);
                    break;

                case L2CAP_CFG_TYPE_FLUSH_TOUT:
                    cfg_info.flush_to_present = TRUE;
                    STREAM_TO_UINT16(p, &cfg_info.flush_to);
                    break;

                case L2CAP_CFG_TYPE_QOS:
                    cfg_info.qos_present = TRUE;
                    STREAM_TO_UINT8(p, &cfg_info.qos.qos_flags);
                    STREAM_TO_UINT8(p, &cfg_info.qos.service_type);
                    STREAM_TO_UINT32(p, &cfg_info.qos.token_rate);
                    STREAM_TO_UINT32(p, &cfg_info.qos.token_bucket_size);
                    STREAM_TO_UINT32(p, &cfg_info.qos.peak_bandwidth);
                    STREAM_TO_UINT32(p, &cfg_info.qos.latency);
                    STREAM_TO_UINT32(p, &cfg_info.qos.delay_variation);
                    break;

                case L2CAP_CFG_TYPE_FCR:
                    cfg_info.fcr_present = TRUE;
                    STREAM_TO_UINT8(p, &cfg_info.fcr.mode);
                    STREAM_TO_UINT8(p, &cfg_info.fcr.tx_win_sz);
                    STREAM_TO_UINT8(p, &cfg_info.fcr.max_transmit);
                    STREAM_TO_UINT16(p, &cfg_info.fcr.rtrans_tout);
                    STREAM_TO_UINT16(p, &cfg_info.fcr.mon_tout);
                    STREAM_TO_UINT16(p, &cfg_info.fcr.mps);
                    break;
                }
            }

            if ((p_ccb = l2cu_find_ccb_by_cid(p_lcb, lcid)) != NULL) {
                if (p_ccb->local_id != id) {
                    L2CAP_TRACE(WARNING, "L2CAP - cfg rsp - bad ID. Exp: %d Got: %d", p_ccb->local_id, id);
                    break;
                }

                if (cfg_info.result == L2CAP_CFG_OK) {
                    l2c_csm_execute(p_ccb, L2CEVT_L2CAP_CONFIG_RSP, &cfg_info);
                } else {
                    l2c_csm_execute(p_ccb, L2CEVT_L2CAP_CONFIG_RSP_NEG, &cfg_info);
                }
            } else {
                L2CAP_TRACE(WARNING, "L2CAP - rcvd cfg rsp for unknown CID: %d", lcid);
            }
            break;

        case L2CAP_CMD_DISC_REQ:
            STREAM_TO_UINT16(p, &lcid);
            STREAM_TO_UINT16(p, &rcid);

            if ((p_ccb = l2cu_find_ccb_by_cid(p_lcb, lcid)) != NULL) {
                if (p_ccb->remote_cid == rcid) {
                    p_ccb->remote_id = id;
                    l2c_csm_execute(p_ccb, L2CEVT_L2CAP_DISCONNECT_REQ, &con_info);
                }
            } else {
                l2cu_send_peer_disc_rsp(p_lcb, id, lcid, rcid);
            }

            break;

        case L2CAP_CMD_DISC_RSP:
            STREAM_TO_UINT16(p, &rcid);
            STREAM_TO_UINT16(p, &lcid);

            if ((p_ccb = l2cu_find_ccb_by_cid(p_lcb, lcid)) != NULL) {
                if ((p_ccb->remote_cid == rcid) && (p_ccb->local_id == id)) {
                    l2c_csm_execute(p_ccb, L2CEVT_L2CAP_DISCONNECT_RSP, &con_info);
                }
            }

            break;

        case L2CAP_CMD_ECHO_REQ:
            if (cmd_len < btu_cb.hcit_acl_pkt_size - 12)
                l2cu_send_peer_echo_rsp(p_lcb, id, p, cmd_len);
            else
                l2cu_send_peer_echo_rsp(p_lcb, id, NULL, 0);

            break;

        case L2CAP_CMD_ECHO_RSP:
            if (p_lcb->p_echo_rsp_cb) {
                tL2CA_ECHO_RSP_CB* p_cb = p_lcb->p_echo_rsp_cb;

                p_lcb->p_echo_rsp_cb = NULL;

                (*p_cb)(L2CAP_PING_RESULT_OK);
            }
            break;

        case L2CAP_CMD_INFO_REQ:
            STREAM_TO_UINT16(p, &info_type);
            l2cu_send_peer_info_rsp(p_lcb, id, info_type);
            break;

        case 11:
            break;

        default:
            L2CAP_TRACE(WARNING, "L2CAP - bad cmd code: %d", cmd_code);
            l2cu_send_peer_cmd_reject(p_lcb, L2CAP_CMD_REJ_NOT_UNDERSTOOD, id, 0, 0);
            return;
        }
    }
}

void l2c_process_compress_setup_timeout(TIMER_LIST_ENT* p_tle) {
#if 0
	/* ... */
#endif
}

void l2c_process_timeout(TIMER_LIST_ENT* p_tle) {
    switch (p_tle->event) {
    case BTU_TTYPE_L2CAP_LINK:
        l2c_link_timeout((tL2C_LCB*)p_tle->param);
        break;

    case BTU_TTYPE_L2CAP_CHNL:
        l2c_csm_execute(((tL2C_CCB*)p_tle->param), L2CEVT_TIMEOUT, NULL);
        break;

    case BTU_TTYPE_L2CAP_HOLD:
        l2c_process_held_packets(TRUE);
        break;

    case BTU_TTYPE_73:
        l2c_process_compress_setup_timeout(p_tle);
        break;
    }
}

void l2c_process_held_packets(BOOLEAN timed_out) {
    BT_HDR* p_buf;
    BT_HDR* p_buf1;
    BUFFER_Q* p_rcv_hold_q = &l2cb.rcv_hold_q;

    if (!p_rcv_hold_q->count)
        return;

    if (!timed_out) {
        btu_stop_timer(&l2cb.rcv_hold_tle);
        L2CAP_TRACE(WARNING, "L2CAP HOLD CONTINUE");
    } else {
        L2CAP_TRACE(WARNING, "L2CAP HOLD TIMEOUT");
    }

    for (p_buf = GKI_getfirst(p_rcv_hold_q); p_buf; p_buf = p_buf1) {
        p_buf1 = GKI_getnext(p_buf);

        if (!timed_out || !p_buf->layer_specific || !--p_buf->layer_specific) {
            GKI_remove_from_queue(p_rcv_hold_q, p_buf);
            p_buf->layer_specific = 0xffff;
            l2c_rcv_acl_data(p_buf);
        }
    }

    if (p_rcv_hold_q->count) {
        btu_start_timer(&l2cb.rcv_hold_tle, BTU_TTYPE_L2CAP_HOLD, BT_1SEC_TIMEOUT);
    }
}
