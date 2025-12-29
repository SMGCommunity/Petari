#include "l2c_int.h"

/* Original source:
 * bluedroid <android.googlesource.com/platform/external/bluetooth/bluedroid>
 * stack/l2cap/l2c_link.c
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

#include <cstring>
#include <stddef.h>

#include "bt_target.h"
#include "bt_trace.h"
#include "bt_types.h"
#include "data_types.h"

#include "btm_api.h"
#include "btm_int.h"
#include "btu.h"
#include "gki.h"
#include "hcidefs.h"
#include "hcimsgs.h"
#include "l2c_api.h"
#include "l2cdefs.h"

/*******************************************************************************
 * macros
 */

#define L2C_LINK_SEND_ACL_DATA(x) HCI_ACL_DATA_TO_LOWER(x)

/*******************************************************************************
 * functions
 */

void l2c_link_hci_conn_req(BD_ADDR bd_addr) {
    tL2C_LCB* p_lcb;
    BT_HDR* p_msg;

    p_msg = GKI_getpoolbuf(2);
    if (!p_msg)
        return;

    p_lcb = l2cu_find_lcb_by_bd_addr(bd_addr);
    if (!p_lcb) {
        p_lcb = l2cu_allocate_lcb(bd_addr);
        if (!p_lcb) {
            // TODO: #define HCI_ERR_HOST_REJECT_RESOURCES 13
            btsnd_hcic_reject_conn(p_msg, bd_addr, 0x14);
            L2CAP_TRACE(ERROR, "L2CAP failed to allocate LCB");
            return;
        }

        p_lcb->link_role = l2cu_get_conn_role(p_lcb);

        btsnd_hcic_accept_conn(p_msg, bd_addr, p_lcb->link_role);

        p_lcb->link_state = LST_CONNECTING;

        btu_start_timer(&p_lcb->timer_entry, BTU_TTYPE_L2CAP_LINK, L2CAP_LINK_CONNECT_TOUT);
        return;
    }

    if (p_lcb->link_state == LST_CONNECTING || p_lcb->link_state == LST_CONNECT_HOLDING) {
        btsnd_hcic_accept_conn(p_msg, bd_addr, p_lcb->link_role);

        p_lcb->link_state = LST_CONNECTING;
    } else if (p_lcb->link_state == LST_DISCONNECTING) {
        // TODO: #define HCI_ERR_HOST_REJECT_DEVICE 15
        btsnd_hcic_reject_conn(p_msg, bd_addr, 0x15);
    } else {
        L2CAP_TRACE(ERROR, "L2CAP got conn_req while connected");

        GKI_freebuf(p_msg);
    }
}

BOOLEAN l2c_link_hci_conn_comp(UINT8 status, UINT16 handle, BD_ADDR p_bda) {
    tL2C_CONN_INFO ci;
    tL2C_LCB* p_lcb;
    tL2C_CCB* p_ccb;
    tBTM_SEC_DEV_REC* p_dev_info = NULL;

    ci.status = status;
    memcpy(ci.bd_addr, p_bda, BD_ADDR_LEN);

    p_lcb = l2cu_find_lcb_by_bd_addr(ci.bd_addr);
    if (!p_lcb) {
        if (!btm_sec_is_bonding(handle))
            L2CAP_TRACE(WARNING, "L2CAP got conn_comp for unknown BD_ADDR");

        return FALSE;
    }

    if (p_lcb->link_state != LST_CONNECTING) {
        L2CAP_TRACE(ERROR, "L2CAP got conn_comp in bad state: %d  status: 0x%d", p_lcb->link_state, status);

        if (status != HCI_SUCCESS)
            l2c_link_hci_disc_comp(p_lcb->handle, status);

        return FALSE;
    }

    p_lcb->handle = handle;

    if (ci.status == HCI_SUCCESS) {
        p_lcb->link_state = LST_CONNECTED;

        if ((p_dev_info = btm_find_dev(p_bda)) != NULL) {
            btm_acl_created(ci.bd_addr, p_dev_info->dev_class, p_dev_info->sec_bd_name, handle, p_lcb->link_role);
        } else {
            btm_acl_created(ci.bd_addr, NULL, NULL, handle, p_lcb->link_role);
        }

        l2c_process_held_packets(FALSE);

        btu_stop_timer(&p_lcb->timer_entry);

        for (p_ccb = p_lcb->p_first_ccb; p_ccb; p_ccb = p_ccb->p_next_ccb)
            l2c_csm_execute(p_ccb, L2CEVT_LP_CONNECT_CFM, &ci);

        if (p_lcb->p_echo_rsp_cb) {
            l2cu_send_peer_echo_req(p_lcb, NULL, 0);
            btu_start_timer(&p_lcb->timer_entry, BTU_TTYPE_L2CAP_LINK, L2CAP_ECHO_RSP_TOUT);
        } else if (!p_lcb->p_first_ccb) {
            btu_start_timer(&p_lcb->timer_entry, BTU_TTYPE_L2CAP_LINK, L2CAP_LINK_STARTUP_TOUT);
        }
    } else if (ci.status == HCI_ERR_MAX_NUM_OF_CONNECTIONS && l2cu_lcb_disconnecting()) {
        p_lcb->link_state = LST_CONNECT_HOLDING;
        p_lcb->handle = HCI_INVALID_HANDLE;
    } else {
        for (p_ccb = p_lcb->p_first_ccb; p_ccb;) {
            tL2C_CCB* pn = p_ccb->p_next_ccb;

            l2c_csm_execute(p_ccb, L2CEVT_LP_CONNECT_CFM_NEG, &ci);

            p_ccb = pn;
        }

        l2cu_release_lcb(p_lcb);
    }

    return TRUE;
}

void l2c_link_sec_comp(BD_ADDR p_bda, void* p_ref_data, UINT8 status) {
    tL2C_CONN_INFO ci;
    tL2C_LCB* p_lcb;
    tL2C_CCB* p_ccb;
    tL2C_CCB* p_next_ccb;
    UINT8 event;

    ci.status = status;
    memcpy(ci.bd_addr, p_bda, BD_ADDR_LEN);

    p_lcb = l2cu_find_lcb_by_bd_addr(p_bda);
    if (!p_lcb) {
        L2CAP_TRACE(WARNING, "L2CAP got sec_comp for unknown BD_ADDR");
        return;
    }

    for (p_ccb = p_lcb->p_first_ccb; p_ccb; p_ccb = p_next_ccb) {
        p_next_ccb = p_ccb->p_next_ccb;

        event = status == BTM_SUCCESS ? L2CEVT_SEC_COMP : L2CEVT_SEC_COMP_NEG;

        l2c_csm_execute(p_ccb, event, &ci);
    }
}

BOOLEAN l2c_link_hci_disc_comp(UINT16 handle, UINT8 reason) {
    tL2C_LCB* p_lcb;
    tL2C_CCB* p_ccb;
    BOOLEAN status = TRUE;

    p_lcb = l2cu_find_lcb_by_handle(handle);

    if (!p_lcb) {
        status = FALSE;
    } else {
        if (btm_cb.acl_disc_reason != HCI_ERR_HOST_REJECT_SECURITY)
            btm_cb.acl_disc_reason = reason;

        for (p_ccb = p_lcb->p_first_ccb; p_ccb;) {
            tL2C_CCB* pn = p_ccb->p_next_ccb;

            l2c_csm_execute(p_ccb, L2CEVT_LP_DISCONNECT_IND, &reason);

            p_ccb = pn;
        }

        btm_sco_acl_removed(p_lcb->remote_bd_addr);
        l2cu_release_lcb(p_lcb);
    }

    if ((p_lcb = l2cu_find_lcb_by_state(LST_CONNECT_HOLDING)) != NULL)
        l2cu_create_conn(p_lcb);

    return status;
}

BOOLEAN l2c_link_hci_qos_violation(UINT16 handle) {
    tL2C_LCB* p_lcb;
    tL2C_CCB* p_ccb;

    p_lcb = l2cu_find_lcb_by_handle(handle);
    if (!p_lcb)
        return FALSE;

    for (p_ccb = p_lcb->p_first_ccb; p_ccb; p_ccb = p_ccb->p_next_ccb) {
        if (p_ccb->p_rcb->api.pL2CA_QoSViolationInd_Cb)
            l2c_csm_execute(p_ccb, L2CEVT_LP_QOS_VIOLATION_IND, NULL);
    }

    return TRUE;
}

void l2c_link_timeout(tL2C_LCB* p_lcb) {
    tL2C_CCB* p_ccb;
    UINT16 timeout;
    tBTM_STATUS rc;

    if (p_lcb->link_state == LST_CONNECTING_WAIT_SWITCH || p_lcb->link_state == LST_CONNECTING || p_lcb->link_state == LST_CONNECT_HOLDING ||
        p_lcb->link_state == LST_DISCONNECTING) {
        for (p_ccb = p_lcb->p_first_ccb; p_ccb;) {
            tL2C_CCB* pn = p_ccb->p_next_ccb;

            l2c_csm_execute(p_ccb, L2CEVT_LP_DISCONNECT_IND, NULL);

            p_ccb = pn;
        }

        l2cu_release_lcb(p_lcb);
    }

    if (p_lcb->link_state == LST_CONNECTED) {
        if (p_lcb->p_echo_rsp_cb) {
            tL2CA_ECHO_RSP_CB* p_cb = p_lcb->p_echo_rsp_cb;

            p_lcb->p_echo_rsp_cb = NULL;

            (*p_cb)(L2CAP_PING_RESULT_NO_RESP);

            L2CAP_TRACE(WARNING, "L2CAP - ping timeout");

            for (p_ccb = p_lcb->p_first_ccb; p_ccb;) {
                tL2C_CCB* pn = p_ccb->p_next_ccb;

                l2c_csm_execute(p_ccb, L2CEVT_LP_DISCONNECT_IND, NULL);

                p_ccb = pn;
            }
        }

        if (!p_lcb->p_first_ccb) {
            rc = btm_sec_disconnect(p_lcb->handle, HCI_ERR_PEER_USER);

            if (rc == BTM_CMD_STARTED) {
                p_lcb->link_state = LST_DISCONNECTING;
                timeout = L2CAP_LINK_DISCONNECT_TOUT;
            } else if (rc == BTM_SUCCESS) {
                p_lcb->link_state = LST_DISCONNECTING;
                timeout = 0xffff;
            } else {
                timeout = BT_1SEC_TIMEOUT;
            }

            if (timeout != 0xffff) {
                btu_start_timer(&p_lcb->timer_entry, BTU_TTYPE_L2CAP_LINK, timeout);
            }
        } else {
            l2c_link_check_send_pkts(p_lcb, NULL, NULL);
        }
    }
}

BOOLEAN l2c_link_send_to_lower(tL2C_LCB* p_lcb, BT_HDR* p_buf) {
    UINT16 num_segs;

    if (p_buf->len <= btu_cb.hcit_acl_pkt_size) {
        --p_lcb->link_xmit_window;
        ++p_lcb->sent_not_acked;
        --l2cb.controller_xmit_window;

        L2C_LINK_SEND_ACL_DATA(p_buf);
    } else {
        num_segs = (p_buf->len - HCI_DATA_PREAMBLE_SIZE + btu_cb.hcit_acl_data_size - 1) / btu_cb.hcit_acl_data_size;

        if (num_segs > l2cb.controller_xmit_window) {
            num_segs = l2cb.controller_xmit_window;
            p_buf->layer_specific = num_segs;
            p_lcb->partial_segment_being_sent = TRUE;
        }

        if (num_segs > p_lcb->link_xmit_window) {
            num_segs = p_lcb->link_xmit_window;
            p_buf->layer_specific = num_segs;
            p_lcb->partial_segment_being_sent = TRUE;
        }

        l2cb.controller_xmit_window -= num_segs;
        p_lcb->link_xmit_window -= num_segs;
        p_lcb->sent_not_acked += num_segs;

        L2C_LINK_SEND_ACL_DATA(p_buf);
    }

    return TRUE;
}

void l2c_link_check_send_pkts(tL2C_LCB* p_lcb, tL2C_CCB* p_ccb, BT_HDR* p_buf) {
    tBTM_PM_MODE mode;
    tBTM_PM_PWR_MD pm;

    if (p_buf) {
        if (p_lcb->link_xmit_data_q.count >= p_lcb->link_cong_start && !p_lcb->cong_sent) {
            for (p_ccb = p_lcb->p_first_ccb; p_ccb; p_ccb = p_ccb->p_next_ccb) {
                L2CAP_TRACE(WARNING, "L2CAP - Congested(), CID: 0x%04x, Congested", p_ccb->local_cid);

                if (p_ccb->p_rcb->api.pL2CA_CongestionStatus_Cb) {
                    L2CAP_TRACE(API,
                                "L2CAP - Calling CongestionStatus_Cb(), CID: "
                                "0x%04x, Congested",
                                p_ccb->local_cid);

                    (*p_ccb->p_rcb->api.pL2CA_CongestionStatus_Cb)(p_ccb->local_cid, TRUE);
                }
            }

            p_lcb->cong_sent = TRUE;
        }

        if (p_lcb->link_xmit_data_q.count <= p_lcb->link_cong_discard) {
            p_buf->layer_specific = 0;
            GKI_enqueue(&p_lcb->link_xmit_data_q, p_buf);
        } else {
            GKI_freebuf(p_buf);
            L2CAP_TRACE(WARNING, "!!! L2CAP - buffer dropped");
        }
    }

    if (p_lcb->partial_segment_being_sent)
        return;

    if (BTM_ReadPowerMode(p_lcb->remote_bd_addr, &mode) == BTM_SUCCESS && mode == BTM_ACL_MODE_PARK && p_lcb->link_xmit_data_q.count != 0) {
        pm.mode = BTM_ACL_MODE_NORMAL;
        BTM_SetPowerMode(0x80, p_lcb->remote_bd_addr, &pm);
        btu_start_timer(&p_lcb->timer_entry, BTU_TTYPE_L2CAP_LINK, 1);
        return;
    }

    while (l2cb.controller_xmit_window && p_lcb->link_xmit_window) {
        if (!p_lcb->link_xmit_data_q.p_first)
            break;

        if (((BT_HDR*)p_lcb->link_xmit_data_q.p_first)->layer_specific)
            break;

        if (p_lcb->partial_segment_being_sent)
            return;

        p_buf = GKI_dequeue(&p_lcb->link_xmit_data_q);
        if (!p_buf)
            break;

        if (!l2c_link_send_to_lower(p_lcb, p_buf))
            break;
    }

    if (p_lcb->partial_segment_being_sent)
        return;

    if (p_lcb->link_xmit_data_q.count && p_lcb->link_xmit_window) {
        btu_start_timer(&p_lcb->timer_entry, BTU_TTYPE_L2CAP_LINK, L2CAP_LINK_FLOW_CONTROL_TOUT);
    }

    if (!p_lcb->cong_sent)
        return;

    if (p_lcb->link_xmit_data_q.count > p_lcb->link_cong_end)
        return;

    p_lcb->cong_sent = FALSE;

    for (p_ccb = p_lcb->p_first_ccb; p_ccb; p_ccb = p_ccb->p_next_ccb) {
        if (p_ccb->p_rcb->api.pL2CA_CongestionStatus_Cb) {
            L2CAP_TRACE(API,
                        "L2CAP - Calling CongestionStatus_Cb(), CID: 0x%04x, "
                        "Uncongested",
                        p_ccb->local_cid);

            (*p_ccb->p_rcb->api.pL2CA_CongestionStatus_Cb)(p_ccb->local_cid, FALSE);

            if (p_lcb->cong_sent)
                break;
        }
    }
}

void l2c_link_adjust_allocation(void) {
    UINT16 qq;
    UINT16 yy;
    UINT16 cc;
    tL2C_LCB* p_lcb;
    int num_hi_pri_links = 0;

    if (l2cb.num_links_active == 0)
        return;

    /* explicitly post-increment */
    for (yy = 0, p_lcb = l2cb.lcb_pool; yy < MAX_L2CAP_LINKS; yy++, p_lcb++) {
        if (p_lcb->in_use && p_lcb->acl_priority == L2CAP_PRIORITY_HIGH)
            ++num_hi_pri_links;
    }

    qq = l2cb.num_lm_acl_bufs / l2cb.num_links_active + 1;

    /* explicitly post-increment */
    for (yy = 0, p_lcb = l2cb.lcb_pool; yy < MAX_L2CAP_LINKS; yy++, p_lcb++) {
        if (!p_lcb->in_use)
            continue;

        if (num_hi_pri_links == 0 || p_lcb->acl_priority == L2CAP_PRIORITY_HIGH) {
            p_lcb->link_xmit_quota = qq;
            p_lcb->link_xmit_window = qq;
        } else {
            p_lcb->link_xmit_quota = 3;
            p_lcb->link_xmit_window = 3;
        }

        cc = p_lcb->link_xmit_quota * 120 / 100;
        p_lcb->link_cong_start = cc;

        p_lcb->link_cong_end = cc > 3 ? cc - 3 : 0;
        p_lcb->link_cong_discard = cc + 2;

        L2CAP_TRACE(DEBUG,
                    "LCB %d Priority:%d XmitWindow:%d Congestion Start:%d "
                    "End:%d Discard:%d",
                    yy, p_lcb->acl_priority, p_lcb->link_xmit_window, p_lcb->link_cong_start, p_lcb->link_cong_end, p_lcb->link_cong_discard);
    }
}

void l2c_link_process_num_completed_pkts(UINT8* p) {
    UINT8 num_handles;
    UINT8 xx;
    UINT16 handle;
    UINT16 num_sent;
    tL2C_LCB* p_lcb;

    STREAM_TO_UINT8(p, &num_handles);

    for (xx = 0; xx < num_handles; xx++) {
        STREAM_TO_UINT16(p, &handle);
        STREAM_TO_UINT16(p, &num_sent);

        p_lcb = l2cu_find_lcb_by_handle(handle);

        if (p_lcb) {
            p_lcb->link_xmit_window += num_sent;
            if (p_lcb->link_xmit_window > p_lcb->link_xmit_quota)
                p_lcb->link_xmit_window = p_lcb->link_xmit_quota;

            if (p_lcb->sent_not_acked > num_sent)
                p_lcb->sent_not_acked -= num_sent;
            else
                p_lcb->sent_not_acked = 0;

            l2cb.controller_xmit_window += num_sent;

            l2c_link_check_send_pkts(p_lcb, NULL, NULL);
        }
    }
}

/* NOTE the misspelling of process */
void l2c_link_processs_num_bufs(UINT16 num_lm_acl_bufs) {
    l2cb.num_lm_acl_bufs = num_lm_acl_bufs;
    l2cb.controller_xmit_window = num_lm_acl_bufs;
}

BT_HDR* l2cap_link_chk_pkt_start(BT_HDR* p_cur_buf) {
    UINT8* p = (UINT8*)(p_cur_buf + 1) + p_cur_buf->offset;
    UINT16 handle;
    UINT16 hci_len;
    UINT16 pkt_type;
    tL2C_LCB* p_lcb;
    BT_HDR* result;

    p_cur_buf->layer_specific = 0;

    STREAM_TO_UINT16(p, &handle);
    STREAM_TO_UINT16(p, &hci_len);

    pkt_type = HCID_GET_EVENT(handle);
    handle = HCID_GET_HANDLE(handle);

    l2cb.p_cur_hcit_lcb = NULL;

    p_lcb = l2cu_find_lcb_by_handle(handle);
    if (!p_lcb)
        return p_cur_buf;

    if (pkt_type == L2CAP_PKT_START) {
        if (p_lcb->p_hcit_rcv_acl) {
            L2CAP_TRACE(WARNING, "L2CAP - dropping incomplete pkt");
            GKI_freebuf(p_lcb->p_hcit_rcv_acl);
        }

        p_lcb->p_hcit_rcv_acl = p_cur_buf;
        l2cb.p_cur_hcit_lcb = p_lcb;
        result = p_cur_buf;
    } else {
        if (p_lcb->p_hcit_rcv_acl) {
            UINT8* p_f = (UINT8*)(p_lcb->p_hcit_rcv_acl + 1) + p_lcb->p_hcit_rcv_acl->offset + 2;
            UINT16 total_len;

            STREAM_TO_UINT16(p_f, &total_len);

            if (total_len + hci_len <= L2CAP_MTU_SIZE + HCI_DATA_PREAMBLE_SIZE) {
                result = p_lcb->p_hcit_rcv_acl;
                l2cb.p_cur_hcit_lcb = p_lcb;

                if (p_cur_buf->len > HCI_DATA_PREAMBLE_SIZE) {
                    UINT8* p = (UINT8*)(p_cur_buf + 1) + p_cur_buf->offset + HCI_DATA_PREAMBLE_SIZE;
                    UINT8* p1 = (UINT8*)(result + 1) + result->offset + result->len;

                    memcpy(p1, p, p_cur_buf->len - HCI_DATA_PREAMBLE_SIZE);
                    result->len += p_cur_buf->len - HCI_DATA_PREAMBLE_SIZE;
                }

                GKI_freebuf(p_cur_buf);

                total_len += hci_len;
                p_f -= 2;
                UINT16_TO_STREAM(p_f, total_len);
            } else {
                L2CAP_TRACE(WARNING, "L2CAP - dropping too long pkt");

                GKI_freebuf(p_lcb->p_hcit_rcv_acl);
                p_lcb->p_hcit_rcv_acl = NULL;
                result = NULL;
            }
        } else {
            result = NULL;
        }
    }

    if (!result)
        GKI_freebuf(p_cur_buf);

    return result;
}

BOOLEAN l2cap_link_chk_pkt_end(void) {
    UINT8* p;
    BT_HDR* p_buf;
    UINT16 l2cap_len;
    tL2C_LCB* p_lcb;

    if (((p_lcb = l2cb.p_cur_hcit_lcb) == NULL) || ((p_buf = p_lcb->p_hcit_rcv_acl) == NULL)) {
        return TRUE;
    }

    p = (UINT8*)(p_buf + 1) + p_buf->offset + HCI_DATA_PREAMBLE_SIZE;

    STREAM_TO_UINT16(p, &l2cap_len);

    if (l2cap_len > p_buf->len - (HCI_DATA_PREAMBLE_SIZE + L2CAP_PKT_OVERHEAD)) {
        return FALSE;
    } else {
        p_lcb->p_hcit_rcv_acl = NULL;
        return TRUE;
    }
}

BT_HDR* l2c_link_get_decompressed_pkt(BT_HDR* p_cur) {
    return p_cur;
}

UINT8 l2c_link_pkts_rcvd(UINT16* num_pkts, UINT16* handles) {
    UINT8 num_found = 0;

    return num_found;
}

void l2c_link_role_changed(BD_ADDR bd_addr, UINT8 new_role) {
    tL2C_LCB* p_lcb;
    int xx;

    p_lcb = l2cu_find_lcb_by_bd_addr(bd_addr);
    if (p_lcb)
        p_lcb->link_role = new_role;

    /* explicitly post-increment */
    for (xx = 0, p_lcb = l2cb.lcb_pool; xx < MAX_L2CAP_LINKS; xx++, p_lcb++) {
        if (p_lcb->in_use && p_lcb->link_state == LST_CONNECTING_WAIT_SWITCH)
            l2cu_create_conn_after_switch(p_lcb);
    }
}

void l2c_link_role_change_failed(void) {
    tL2C_LCB* p_lcb;
    int xx;

    /* explicitly post-increment */
    for (xx = 0, p_lcb = l2cb.lcb_pool; xx < MAX_L2CAP_LINKS; xx++, p_lcb++) {
        if (p_lcb->in_use && p_lcb->link_state == LST_CONNECTING_WAIT_SWITCH)
            l2cu_create_conn_after_switch(p_lcb);
    }
}

void l2c_link_segments_xmitted(BT_HDR* p_msg) {
    UINT8* p = (UINT8*)(p_msg + 1) + p_msg->offset;
    UINT16 handle;
    tL2C_LCB* p_lcb;

    STREAM_TO_UINT16(p, &handle);
    handle = HCID_GET_HANDLE(handle);

    if ((p_lcb = l2cu_find_lcb_by_handle(handle)) == NULL) {
        L2CAP_TRACE(WARNING, "L2CAP - rcvd segment complete, unknown handle: %d", handle);
        GKI_freebuf(p_msg);
        return;
    }

    if (p_lcb->link_state == LST_CONNECTED) {
        p_msg->layer_specific = 0;
        GKI_enqueue_head(&p_lcb->link_xmit_data_q, p_msg);

        p_lcb->partial_segment_being_sent = FALSE;

        l2c_link_check_send_pkts(p_lcb, NULL, NULL);
    } else {
        GKI_freebuf(p_msg);
    }
}

void l2c_pin_code_request(BD_ADDR bd_addr) {
    tL2C_LCB* p_lcb = l2cu_find_lcb_by_bd_addr(bd_addr);

    if (p_lcb && !p_lcb->p_first_ccb) {
        btu_start_timer(&p_lcb->timer_entry, BTU_TTYPE_L2CAP_LINK, L2CAP_LINK_CONNECT_TOUT_EXT);
    }
}
