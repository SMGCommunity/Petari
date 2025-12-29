#include "l2c_api.h"

/* Original source:
 * bluedroid <android.googlesource.com/platform/external/bluetooth/bluedroid>
 * stack/l2cap/l2c_api.c
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

#include <decomp.h>

#include "bt_target.h"
#include "bt_trace.h"
#include "bt_types.h"
#include "data_types.h"

#include "btm_api.h"
#include "btu.h"
#include "gki.h"
#include "hcidefs.h"
#include "l2c_int.h"
#include "l2cdefs.h"

/*******************************************************************************
 * functions
 */

BOOLEAN L2CA_Register(UINT16 psm, tL2CAP_APPL_INFO* p_cb_info) {
    tL2C_RCB* p_rcb;

    L2CAP_TRACE(API, "L2CAP - L2CA_Register() called for PSM: 0x%04x", psm);

    if (!p_cb_info->pL2CA_ConfigCfm_Cb || !p_cb_info->pL2CA_ConfigInd_Cb || !p_cb_info->pL2CA_DataInd_Cb || !p_cb_info->pL2CA_DisconnectInd_Cb) {
        L2CAP_TRACE(ERROR, "L2CAP - no cb registering PSM: 0x%04x", psm);
        return FALSE;
    }

    if (L2C_INVALID_PSM(psm)) {
        L2CAP_TRACE(ERROR, "L2CAP - invalid PSM value, PSM: 0x%04x", psm);
        return FALSE;
    }

    if ((p_rcb = l2cu_find_rcb_by_psm(psm)) == NULL) {
        if ((p_rcb = l2cu_allocate_rcb(psm)) == NULL) {
            L2CAP_TRACE(WARNING, "L2CAP - no RCB available, PSM: 0x%04x", psm);
            return FALSE;
        }
    }

    p_rcb->api = *p_cb_info;

    return TRUE;
}

void L2CA_Deregister(UINT16 psm) {
    tL2C_RCB* p_rcb;

    L2CAP_TRACE(API, "L2CAP - L2CA_Deregister() called for PSM: 0x%04x", psm);

    if ((p_rcb = l2cu_find_rcb_by_psm(psm)) != NULL) {
        l2cu_release_rcb(p_rcb);
    } else {
        L2CAP_TRACE(WARNING, "L2CAP - PSM: 0x%04x not found for deregistration", psm);
    }
}

UINT16 L2CA_ConnectReq(UINT16 psm, BD_ADDR p_bd_addr) {
    tL2C_LCB* p_lcb;
    tL2C_CCB* p_ccb;
    tL2C_RCB* p_rcb;

    L2CAP_TRACE(API, "L2CA_ConnectReq()  PSM: 0x%04x", psm);

    L2CAP_TRACE(API, "L2CA_ConnectReq()  BDA: %02x-%02x-%02x-%02x-%02x-%02x", p_bd_addr[0], p_bd_addr[1], p_bd_addr[2], p_bd_addr[3], p_bd_addr[4],
                p_bd_addr[5]);

    if (!BTM_IsDeviceUp()) {
        L2CAP_TRACE(WARNING, "L2CAP connect req - BTU not ready");
        return 0;
    }

    if ((p_rcb = l2cu_find_rcb_by_psm(psm)) == NULL) {
        L2CAP_TRACE(WARNING, "L2CAP - no RCB for L2CA_conn_req, PSM: 0x%04x", psm);
        return 0;
    }

    if ((p_lcb = l2cu_find_lcb_by_bd_addr(p_bd_addr)) == NULL) {
        if (((p_lcb = l2cu_allocate_lcb(p_bd_addr)) == NULL)) {
            L2CAP_TRACE(WARNING, "L2CAP - no LCB for L2CA_conn_req");
            return 0;
        }

        if (!l2cu_create_conn(p_lcb))
            return 0;
    }

    if (p_lcb->link_state == LST_DISCONNECTING) {
        L2CAP_TRACE(API, "L2CAP API - L2CA_conn_req rejected - link disconnecting");
        return 0;
    }

    if ((p_ccb = l2cu_allocate_ccb(p_lcb)) == NULL) {
        L2CAP_TRACE(WARNING, "L2CAP - no CCB for L2CA_conn_req");
        return 0;
    }

    p_ccb->p_rcb = p_rcb;

    if (p_lcb->link_state == LST_CONNECTED)
        l2c_csm_execute(p_ccb, L2CEVT_L2CA_CONNECT_REQ, NULL);

    L2CAP_TRACE(API, "L2CAP - L2CA_conn_req() returned CID: 0x%04x", p_ccb->local_cid);

    return p_ccb->local_cid;
}

BOOLEAN L2CA_ConnectRsp(BD_ADDR p_bd_addr, UINT8 id, UINT16 lcid, UINT16 result, UINT16 status) {
    tL2C_LCB* p_lcb;
    tL2C_CCB* p_ccb;

    L2CAP_TRACE(API, "L2CA_ConnectRsp()  CID: 0x%04x  Result: %d  Status: %d", lcid, result, status);

    L2CAP_TRACE(API, "L2CA_ConnectRsp()  BDA: %02x-%02x-%02x-%02x-%02x-%02x", p_bd_addr[0], p_bd_addr[1], p_bd_addr[2], p_bd_addr[3], p_bd_addr[4],
                p_bd_addr[5]);

    if ((p_lcb = l2cu_find_lcb_by_bd_addr(p_bd_addr)) == NULL) {
        L2CAP_TRACE(WARNING, "L2CAP - no LCB for L2CA_conn_rsp");
        return FALSE;
    }

    if ((p_ccb = l2cu_find_ccb_by_cid(p_lcb, lcid)) == NULL) {
        L2CAP_TRACE(WARNING, "L2CAP - no CCB for L2CA_conn_rsp");
        return FALSE;
    }

    if (p_ccb->remote_id != id) {
        L2CAP_TRACE(WARNING, "L2CAP - bad id in L2CA_conn_rsp. Exp: %d  Got: %d", p_ccb->remote_id, id);
        return FALSE;
    }

    if (result == L2CAP_CONN_OK) {
        l2c_csm_execute(p_ccb, L2CEVT_L2CA_CONNECT_RSP, NULL);
    } else {
        tL2C_CONN_INFO conn_info;

        conn_info.l2cap_result = result;
        conn_info.l2cap_status = status;

        if (result == L2CAP_CONN_PENDING)
            l2c_csm_execute(p_ccb, L2CEVT_L2CA_CONNECT_RSP, &conn_info);
        else
            l2c_csm_execute(p_ccb, L2CEVT_L2CA_CONNECT_RSP_NEG, &conn_info);
    }

    return TRUE;
}

BOOLEAN L2CA_ConfigReq(UINT16 cid, tL2CAP_CFG_INFO* p_cfg) {
    tL2C_CCB* p_ccb;

    L2CAP_TRACE(API, "L2CA_ConfigReq()  CID: 0x%04x", cid);

    if ((p_ccb = l2cu_find_ccb_by_cid(NULL, cid)) == NULL) {
        L2CAP_TRACE(WARNING, "L2CAP - no CCB for L2CA_cfg_req, CID: %d", cid);
        return FALSE;
    }

    p_cfg->fcr_present = FALSE;

    l2c_csm_execute(p_ccb, L2CEVT_L2CA_CONFIG_REQ, p_cfg);

    return TRUE;
}

BOOLEAN L2CA_ConfigRsp(UINT16 cid, tL2CAP_CFG_INFO* p_cfg) {
    tL2C_CCB* p_ccb;

    L2CAP_TRACE(API, "L2CA_ConfigRsp()  CID: 0x%04x  Result: %d", cid, p_cfg->result);

    if ((p_ccb = l2cu_find_ccb_by_cid(NULL, cid)) == NULL) {
        L2CAP_TRACE(WARNING, "L2CAP - no CCB for L2CA_cfg_rsp, CID: %d", cid);
        return FALSE;
    }

    if (p_cfg->result == L2CAP_CFG_OK)
        l2c_csm_execute(p_ccb, L2CEVT_L2CA_CONFIG_RSP, p_cfg);
    else
        l2c_csm_execute(p_ccb, L2CEVT_L2CA_CONFIG_RSP_NEG, p_cfg);

    return TRUE;
}

BOOLEAN L2CA_DisconnectReq(UINT16 cid) {
    tL2C_CCB* p_ccb;

    L2CAP_TRACE(API, "L2CA_DisconnectReq()  CID: 0x%04x", cid);

    if ((p_ccb = l2cu_find_ccb_by_cid(NULL, cid)) == NULL) {
        L2CAP_TRACE(WARNING, "L2CAP - no CCB for L2CA_disc_req, CID: %d", cid);
        return FALSE;
    }

    l2c_csm_execute(p_ccb, L2CEVT_L2CA_DISCONNECT_REQ, NULL);

    return TRUE;
}

BOOLEAN L2CA_DisconnectRsp(UINT16 cid) {
    tL2C_CCB* p_ccb;

    L2CAP_TRACE(API, "L2CA_DisconnectRsp()  CID: 0x%04x", cid);

    if ((p_ccb = l2cu_find_ccb_by_cid(NULL, cid)) == NULL) {
        L2CAP_TRACE(WARNING, "L2CAP - no CCB for L2CA_disc_rsp, CID: %d", cid);
        return FALSE;
    }

    l2c_csm_execute(p_ccb, L2CEVT_L2CA_DISCONNECT_RSP, NULL);

    return TRUE;
}

UINT8 L2CA_DataWrite(UINT16 cid, BT_HDR* p_data) {
    tL2C_CCB* p_ccb;

    L2CAP_TRACE(API, "L2CA_DataWrite()  CID: 0x%04x  Len: %d", cid, p_data->len);

    if ((p_ccb = l2cu_find_ccb_by_cid(NULL, cid)) == NULL) {
        L2CAP_TRACE(WARNING, "L2CAP - no CCB for L2CA_DataWrite, CID: %d", cid);
        GKI_freebuf(p_data);
        return L2CAP_DW_FAILED;
    }

    if (p_data->len <= p_ccb->out_mtu) {
        l2c_csm_execute(p_ccb, L2CEVT_L2CA_DATA_WRITE, p_data);

        if (p_ccb->p_lcb->cong_sent)
            return L2CAP_DW_CONGESTED;
    } else {
        L2CAP_TRACE(WARNING, "L2CAP - cannot send message bigger than "
                             "peer's mtu size");

        GKI_freebuf(p_data);
        return L2CAP_DW_FAILED;
    }

    return L2CAP_DW_SUCCESS;
}

BOOLEAN L2CA_Ping(BD_ADDR p_bd_addr, tL2CA_ECHO_RSP_CB* p_callback) {
    tL2C_LCB* p_lcb;

    L2CAP_TRACE(API, "L2CA_Ping()  BDA: %02x-%02x-%02x-%02x-%02x-%02x", p_bd_addr[0], p_bd_addr[1], p_bd_addr[2], p_bd_addr[3], p_bd_addr[4],
                p_bd_addr[5]);

    if (!BTM_IsDeviceUp())
        return FALSE;

    if ((p_lcb = l2cu_find_lcb_by_bd_addr(p_bd_addr)) == NULL) {
        if ((p_lcb = l2cu_allocate_lcb(p_bd_addr)) == NULL) {
            L2CAP_TRACE(WARNING, "L2CAP - no LCB for L2CA_ping");
            return FALSE;
        }

        if (!l2cu_create_conn(p_lcb))
            return FALSE;

        p_lcb->p_echo_rsp_cb = p_callback;
        return TRUE;
    }

    if (p_lcb->p_echo_rsp_cb) {
        L2CAP_TRACE(WARNING, "L2CAP - rejected second L2CA_ping");
        return FALSE;
    }

    if (p_lcb->link_state == LST_DISCONNECTING) {
        L2CAP_TRACE(WARNING, "L2CAP - L2CA_ping rejected - link disconnecting");
        return FALSE;
    }

    p_lcb->p_echo_rsp_cb = p_callback;

    if (p_lcb->link_state == LST_CONNECTED) {
        l2cu_send_peer_echo_req(p_lcb, NULL, 0);
        btu_start_timer(&p_lcb->timer_entry, BTU_TTYPE_L2CAP_LINK, L2CAP_ECHO_RSP_TOUT);
    }

    return TRUE;
}

BOOLEAN L2CA_SetIdleTimeout(UINT16 cid, UINT16 timeout, BOOLEAN is_global) {
    tL2C_CCB* p_ccb;
    tL2C_LCB* p_lcb;

    if (is_global) {
        l2cb.idle_timeout = timeout;
    } else {
        if ((p_ccb = l2cu_find_ccb_by_cid(NULL, cid)) == NULL) {
            L2CAP_TRACE(WARNING, "L2CAP - no CCB for L2CA_SetIdleTimeout, CID: %d", cid);
            return FALSE;
        }

        p_lcb = p_ccb->p_lcb;

        if (p_lcb && p_lcb->in_use && p_lcb->link_state == LST_CONNECTED)
            p_lcb->idle_timeout = timeout;
        else
            return FALSE;
    }

    return TRUE;
}

BOOLEAN L2CA_SetIdleTimeoutByBdAddr(BD_ADDR bd_addr, UINT16 timeout) {
    tL2C_LCB* p_lcb;

    if (memcmp(BT_BD_ANY, bd_addr, BD_ADDR_LEN) != 0) {
        p_lcb = l2cu_find_lcb_by_bd_addr(bd_addr);

        if (p_lcb && p_lcb->in_use && p_lcb->link_state == LST_CONNECTED)
            p_lcb->idle_timeout = timeout;
        else
            return FALSE;
    } else {
        int xx;
        tL2C_LCB* p_lcb = &l2cb.lcb_pool[0];

        for (xx = 0; xx < MAX_L2CAP_LINKS; xx++, p_lcb++) {
            if (p_lcb->in_use && p_lcb->link_state == LST_CONNECTED)
                p_lcb->idle_timeout = timeout;
        }
    }

    return TRUE;
}

UINT8 L2CA_SetTraceLevel(UINT8 new_level) {
    if (new_level != 0xff)
        l2cb.l2cap_trace_level = new_level;

    return l2cb.l2cap_trace_level;
}

UINT8 L2CA_SetDesireRole(UINT8 new_role) {
    if (new_role == HCI_ROLE_MASTER || new_role == HCI_ROLE_SLAVE)
        l2cb.desire_role = new_role;

    return l2cb.desire_role;
}

BOOLEAN L2CA_SetAclPriority(BD_ADDR bd_addr, UINT8 priority) {
    tBTM_VERSION_INFO lversion;
    tL2C_LCB* p_lcb;
    UINT8* pp;
    UINT8 command[HCI_BRCM_ACL_PRIORITY_PARAM_SIZE];
    UINT8 vs_param;
    UINT8 status;

    status = BTM_ReadLocalVersion(&lversion);
    if (status == 0 && lversion.manufacturer == LMP_COMPID_BROADCOM) {
        L2CAP_TRACE(API, "L2CA_SetAclPriority()  bdaddr: %02x%02x%02x%02x%02x%02x", bd_addr[0], bd_addr[1], bd_addr[2], bd_addr[3], bd_addr[4],
                    bd_addr[5]);

        if ((p_lcb = l2cu_find_lcb_by_bd_addr(bd_addr)) == NULL) {
            L2CAP_TRACE(WARNING, "L2CAP - no LCB for L2CA_SetAclPriority");
            return FALSE;
        }

        vs_param = priority == L2CAP_PRIORITY_HIGH ? HCI_BRCM_ACL_PRIORITY_HIGH : HCI_BRCM_ACL_PRIORITY_LOW;

        pp = command;
        UINT16_TO_STREAM(pp, p_lcb->handle);
        UINT8_TO_STREAM(pp, vs_param);

        BTM_VendorSpecificCommand(HCI_BRCM_SET_ACL_PRIORITY, HCI_BRCM_ACL_PRIORITY_PARAM_SIZE, command, NULL);

        p_lcb->acl_priority = priority;
        l2c_link_adjust_allocation();
    }

    return TRUE;
}

void L2CA_SetCompression(UINT16 local_cid, UINT8 pe_type, UINT8 mem_level, UINT8 wbits, UINT8 direction, UINT8 param_6, UINT8 enable) {
    tL2C_LCB* p_lcb;
    tL2C_CCB* p_ccb;

    L2CAP_TRACE(API,
                "L2CA_SetCompression() local cid %d, direction %d, "
                "pe_type %d, mem_level %d, wbits %d, enable %d",
                local_cid, direction, pe_type, mem_level, wbits, enable);

    if ((p_ccb = l2cu_find_ccb_by_cid(NULL, local_cid)) == NULL)
        return;

    p_lcb = p_ccb->p_lcb;

    if (p_lcb && p_lcb->in_use && p_lcb->link_state == LST_CONNECTED) {
#if 0
		/* ... */
#endif
    } else {
        return;
    }
}

void L2CA_RegisterCompression(tL2CA_COMPRESS_CB* param_1, unk_t param_2) {
#if 0
	/* ... */
#endif
}

UINT8 L2CA_Flush(UINT16 cid) {
    tL2C_CCB* p_ccb;
    tL2C_LCB* p_lcb;
    BT_HDR* p_buf;
    int flushed = 0;
    int xx;
    int num_bufs;

    if ((p_ccb = l2cu_find_ccb_by_cid(NULL, cid)) == NULL) {
        L2CAP_TRACE(WARNING, "L2CAP - no CCB for L2CA_Flush, CID: %d", cid);
        return FALSE;
    }

    p_lcb = p_ccb->p_lcb;

    while ((p_buf = GKI_dequeue(&p_ccb->xmit_hold_q)) != NULL) {
        ++flushed;
        GKI_freebuf(p_buf);
    }

    num_bufs = p_lcb->link_xmit_data_q.count;

    for (xx = 0; xx < num_bufs; ++xx) {
        p_buf = GKI_dequeue(&p_lcb->link_xmit_data_q);

        if (p_buf->layer_specific != 0 || p_buf->event != cid) {
            GKI_enqueue(&p_lcb->link_xmit_data_q, p_buf);
        } else {
            GKI_freebuf(p_buf);
            ++flushed;
        }
    }

    L2CAP_TRACE(WARNING, "L2CA_Flush()  CID: 0x%04x flushed %d buffers", cid, flushed);

    return TRUE;
}

UINT16 L2CA_GetNumQueuedBufs(UINT16 lcid) {
    tL2C_CCB* p_ccb = l2cu_find_ccb_by_cid(NULL, lcid);
    tL2C_LCB* p_lcb;
    BT_HDR* p_buf;
    UINT16 num_bufs = 0;

    if (!p_ccb || ((p_lcb = p_ccb->p_lcb) == NULL)) {
        L2CAP_TRACE(API, "L2CA_GetNumQueuedBufs()  CID: 0x%04x  abmormally returning 0", lcid);
        return 0;
    }

    for (p_buf = p_lcb->link_xmit_data_q.p_first; p_buf; p_buf = GKI_getnext(p_buf)) {
        if (p_buf->event == lcid)
            ++num_bufs;
    }

    L2CAP_TRACE(API, "L2CA_GetNumQueuedBufs()  CID: 0x%04x  returning %d", lcid, num_bufs);
    return num_bufs;
}
