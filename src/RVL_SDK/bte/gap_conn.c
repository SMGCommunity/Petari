#include "gap_int.h"

/* Original source:
 * bluedroid <android.googlesource.com/platform/external/bluetooth/bluedroid>
 * stack/gap/gap_conn.c
 */

/******************************************************************************
 *
 *  Copyright (C) 2009-2013 Broadcom Corporation
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
#include <stddef.h>  // NULL

#include "bt_target.h"
#include "bt_trace.h"
#include "bt_types.h"
#include "data_types.h"

#include "btm_api.h"
#include "btu.h"
#include "gap_api.h"
#include "gki.h"
#include "l2c_api.h"
#include "l2cdefs.h"
#include "profiles_api.h"

/*******************************************************************************
 * local function declarations
 */

static void gap_connect_ind(BD_ADDR bd_addr, UINT16 l2cap_cid, UINT16 psm, UINT8 l2cap_id);
static void gap_connect_cfm(UINT16 l2cap_cid, UINT16 result);
static void gap_config_ind(UINT16 l2cap_cid, tL2CAP_CFG_INFO* p_cfg);
static void gap_config_cfm(UINT16 l2cap_cid, tL2CAP_CFG_INFO* p_cfg);
static void gap_disconnect_ind(UINT16 l2cap_cid, BOOLEAN ack_needed);
static void gap_data_ind(UINT16 l2cap_cid, BT_HDR* p_msg);
static void gap_congestion_ind(UINT16 lcid, BOOLEAN is_congested);
static tGAP_CCB* gap_find_ccb_by_cid(UINT16 cid);
static tGAP_CCB* gap_find_ccb_by_handle(UINT16 handle);
static tGAP_CCB* gap_allocate_ccb(void);
static void gap_release_ccb(tGAP_CCB* p_ccb);

/*******************************************************************************
 * functions
 */

void gap_conn_init(void) {
    gap_cb.conn.reg_info.pL2CA_ConnectInd_Cb = &gap_connect_ind;
    gap_cb.conn.reg_info.pL2CA_ConnectCfm_Cb = &gap_connect_cfm;
    gap_cb.conn.reg_info.pL2CA_ConnectPnd_Cb = NULL;
    gap_cb.conn.reg_info.pL2CA_ConfigInd_Cb = &gap_config_ind;
    gap_cb.conn.reg_info.pL2CA_ConfigCfm_Cb = &gap_config_cfm;
    gap_cb.conn.reg_info.pL2CA_DisconnectInd_Cb = &gap_disconnect_ind;
    gap_cb.conn.reg_info.pL2CA_DisconnectCfm_Cb = NULL;
    gap_cb.conn.reg_info.pL2CA_QoSViolationInd_Cb = NULL;
    gap_cb.conn.reg_info.pL2CA_DataInd_Cb = &gap_data_ind;
    gap_cb.conn.reg_info.pL2CA_CongestionStatus_Cb = &gap_congestion_ind;
}

UINT16 GAP_ConnOpen(char* p_serv_name, UINT8 service_id, BOOLEAN is_server, BD_ADDR p_rem_bda, UINT16 psm, tL2CAP_CFG_INFO* p_cfg, UINT8 security,
                    tGAP_CONN_CALLBACK* p_cb) {
    tGAP_CCB* p_ccb;
    UINT16 cid;

    GAP_TRACE(EVENT, "GAP_CONN - Open Request");

    if (!BTM_SetSecurityLevel(!is_server, p_serv_name, service_id, security, psm, 0, 0)) {
        GAP_TRACE(ERROR, "GAP_CONN - Security Error");
        return GAP_INVALID_HANDLE;
    }

    if ((p_ccb = gap_allocate_ccb()) == NULL)
        return GAP_INVALID_HANDLE;

    if (p_rem_bda) {
        if (memcmp(p_rem_bda, BT_BD_ANY, BD_ADDR_LEN) != 0)
            p_ccb->rem_addr_specified = TRUE;

        memcpy(p_ccb->rem_dev_address, p_rem_bda, BD_ADDR_LEN);
    }

    if (!p_ccb->rem_addr_specified && !is_server) {
        gap_release_ccb(p_ccb);

        GAP_TRACE(ERROR, "GAP ERROR: Client must specify a remote BD ADDR to connect to!");

        return GAP_INVALID_HANDLE;
    }

    if (p_cfg)
        p_ccb->cfg = *p_cfg;

    p_ccb->psm = psm;
    p_ccb->p_callback = p_cb;
    p_ccb->security_flags = security;

    L2CA_Register(psm, &gap_cb.conn.reg_info);

    if (is_server) {
        p_ccb->con_state = 1;
        return p_ccb->gap_handle;
    } else {
        p_ccb->con_flags = 1;
        p_ccb->con_state = 2;

        if ((cid = L2CA_ConnectReq(psm, p_rem_bda)) != 0) {
            p_ccb->connection_id = cid;
            return p_ccb->gap_handle;
        } else {
            gap_release_ccb(p_ccb);
            return GAP_INVALID_HANDLE;
        }
    }
}

UINT16 GAP_ConnClose(UINT16 gap_handle) {
    tGAP_CCB* p_ccb = gap_find_ccb_by_handle(gap_handle);

    GAP_TRACE(EVENT, "GAP_CONN - close  handle: 0x%x", gap_handle);

    if (p_ccb) {
        if (p_ccb->con_state != GAP_CCB_STATE_LISTENING)
            L2CA_DisconnectReq(p_ccb->connection_id);

        gap_release_ccb(p_ccb);

        return BT_PASS;
    }

    return GAP_ERR_BAD_HANDLE;
}

UINT16 GAP_ConnReadData(UINT16 gap_handle, UINT8* p_data, UINT16 max_len, UINT16* p_len) {
    tGAP_CCB* p_ccb = gap_find_ccb_by_handle(gap_handle);
    BT_HDR* p_buf;

    if (!p_ccb)
        return GAP_ERR_BAD_HANDLE;

    *p_len = 0;

    p_buf = GKI_getfirst(&p_ccb->rx_queue);
    if (!p_buf)
        return GAP_NO_DATA_AVAIL;

    GKI_disable();

    while (max_len && p_buf) {
        if (p_buf->len > max_len) {
            memcpy(p_data, (UINT8*)(p_buf + 1) + p_buf->offset, max_len);
            p_buf->offset += max_len;
            p_buf->len -= max_len;
            *p_len += max_len;
            break;
        } else {
            memcpy(p_data, (UINT8*)(p_buf + 1) + p_buf->offset, p_buf->len);
            *p_len += p_buf->len;
            max_len -= p_buf->len;

            if (max_len) {
                p_data += p_buf->len;
                p_buf = GKI_getnext(p_buf);
            }

            GKI_freebuf(GKI_dequeue(&p_ccb->rx_queue));
        }
    }

    GKI_enable();

    GAP_TRACE(EVENT, "GAP_ConnReadData returned:%d", *p_len);

    return BT_PASS;
}

UINT16 GAP_ConnBTRead(UINT16 gap_handle, BT_HDR** pp_buf) {
    tGAP_CCB* p_ccb = gap_find_ccb_by_handle(gap_handle);
    BT_HDR* p_buf;

    if (!p_ccb)
        return GAP_ERR_BAD_HANDLE;

    p_buf = GKI_dequeue(&p_ccb->rx_queue);
    if (p_buf) {
        *pp_buf = p_buf;
        return BT_PASS;
    } else {
        *pp_buf = NULL;
        return GAP_NO_DATA_AVAIL;
    }
}

UINT16 GAP_ConnBTWrite(UINT16 gap_handle, BT_HDR* p_buf) {
    tGAP_CCB* p_ccb = gap_find_ccb_by_handle(gap_handle);

    if (!p_ccb) {
        GKI_freebuf(p_buf);
        return GAP_ERR_BAD_HANDLE;
    }

    if (p_ccb->con_state != GAP_CCB_STATE_CONNECTED) {
        GKI_freebuf(p_buf);
        return GAP_ERR_BAD_STATE;
    }

    if (p_buf->offset < L2CAP_MIN_OFFSET) {
        GKI_freebuf(p_buf);
        return GAP_ERR_BUF_OFFSET;
    }

    if (L2CA_DataWrite(p_ccb->connection_id, p_buf))
        return BT_PASS;
    else
        return GAP_ERR_BAD_STATE;
}

UINT16 GAP_ConnWriteData(UINT16 gap_handle, UINT8* p_data, UINT16 max_len, UINT16* p_len) {
    tGAP_CCB* p_ccb = gap_find_ccb_by_handle(gap_handle);
    BT_HDR* p_buf;

    *p_len = 0;

    if (!p_ccb)
        return GAP_ERR_BAD_HANDLE;

    if (p_ccb->con_state != GAP_CCB_STATE_CONNECTED)
        return GAP_ERR_BAD_STATE;

    while (max_len) {
        if ((p_buf = GKI_getpoolbuf(GAP_DATA_POOL_ID)) == NULL)
            return GAP_ERR_CONGESTED;

        p_buf->offset = L2CAP_MIN_OFFSET;
        p_buf->len = p_ccb->rem_mtu_size < max_len ? p_ccb->rem_mtu_size : max_len;
        p_buf->event = BT_EVT_TO_BTU_SP_DATA;

        memcpy((UINT8*)(p_buf + 1) + p_buf->offset, p_data, p_buf->len);

        *p_len += p_buf->len;
        max_len -= p_buf->len;
        p_data += p_buf->len;

        GAP_TRACE(EVENT, "GAP_WriteData %d bytes", p_buf->len);

        if (!L2CA_DataWrite(p_ccb->connection_id, p_buf))
            return GAP_ERR_BAD_STATE;
    }

    return BT_PASS;
}

UINT16 GAP_ConnReconfig(UINT16 gap_handle, tL2CAP_CFG_INFO* p_cfg) {
    tGAP_CCB* p_ccb = gap_find_ccb_by_handle(gap_handle);

    if (!p_ccb)
        return GAP_ERR_BAD_HANDLE;

    p_ccb->cfg = *p_cfg;

    if (p_ccb->con_state == GAP_CCB_STATE_CONNECTED)
        L2CA_ConfigReq(p_ccb->connection_id, p_cfg);

    return BT_PASS;
}

UINT16 GAP_ConnSetIdleTimeout(UINT16 gap_handle, UINT16 timeout) {
    tGAP_CCB* p_ccb = gap_find_ccb_by_handle(gap_handle);

    if (!p_ccb)
        return GAP_ERR_BAD_HANDLE;

    if (L2CA_SetIdleTimeout(p_ccb->connection_id, timeout, FALSE))
        return BT_PASS;
    else
        return GAP_ERR_BAD_HANDLE;
}

BD_ADDR_PTR GAP_ConnGetRemoteAddr(UINT16 gap_handle) {
    tGAP_CCB* p_ccb = gap_find_ccb_by_handle(gap_handle);

    if (p_ccb && p_ccb->con_state > GAP_CCB_STATE_LISTENING)
        return p_ccb->rem_dev_address;
    else
        return NULL;
}

UINT16 GAP_ConnGetRemMtuSize(UINT16 gap_handle) {
    tGAP_CCB* p_ccb = gap_find_ccb_by_handle(gap_handle);

    if (!p_ccb)
        return 0;

    return p_ccb->rem_mtu_size;
}

UINT16 GAP_ConnGetL2CAPCid(UINT16 gap_handle) {
    tGAP_CCB* p_ccb = gap_find_ccb_by_handle(gap_handle);

    if (!p_ccb)
        return 0;

    return p_ccb->connection_id;
}

static void gap_connect_ind(BD_ADDR bd_addr, UINT16 l2cap_cid, UINT16 psm, UINT8 l2cap_id) {
    UINT16 xx;
    tGAP_CCB* p_ccb;

    /* explicitly post-increment */
    for (xx = 0, p_ccb = gap_cb.conn.ccb_pool; xx < GAP_MAX_CONNECTIONS; xx++, p_ccb++) {
        if (p_ccb->con_state == GAP_CCB_STATE_LISTENING && p_ccb->psm == psm &&
            (!p_ccb->rem_addr_specified || memcmp(bd_addr, p_ccb->rem_dev_address, BD_ADDR_LEN) == 0)) {
            break;
        }
    }

    if (xx == GAP_MAX_CONNECTIONS) {
        GAP_TRACE(WARNING, "*******");
        GAP_TRACE(WARNING, "WARNING: GAP Conn Indication for Unexpected Bd "
                           "Addr...Disconnecting");
        GAP_TRACE(WARNING, "*******");

        L2CA_DisconnectReq(l2cap_cid);
        return;
    }

    p_ccb->con_state = GAP_CCB_STATE_CFG_SETUP;

    memcpy(p_ccb->rem_dev_address, bd_addr, BD_ADDR_LEN);
    p_ccb->connection_id = l2cap_cid;

    L2CA_ConnectRsp(bd_addr, l2cap_id, l2cap_cid, L2CAP_CONN_OK, L2CAP_CONN_OK);

    GAP_TRACE(EVENT, "GAP_CONN - Rcvd L2CAP conn ind, CID: 0x%x", p_ccb->connection_id);

    L2CA_ConfigReq(l2cap_cid, &p_ccb->cfg);
}

static void gap_connect_cfm(UINT16 l2cap_cid, UINT16 result) {
    tGAP_CCB* p_ccb = gap_find_ccb_by_cid(l2cap_cid);

    if (!p_ccb)
        return;

    if (result == L2CAP_CONN_OK && p_ccb->con_state == GAP_CCB_STATE_CONN_SETUP) {
        p_ccb->con_state = GAP_CCB_STATE_CFG_SETUP;

        L2CA_ConfigReq(l2cap_cid, &p_ccb->cfg);
    } else {
        if (p_ccb->p_callback)
            (*p_ccb->p_callback)(p_ccb->gap_handle, GAP_EVT_CONN_CLOSED);

        gap_release_ccb(p_ccb);
    }
}

static void gap_config_ind(UINT16 l2cap_cid, tL2CAP_CFG_INFO* p_cfg) {
    tGAP_CCB* p_ccb = gap_find_ccb_by_cid(l2cap_cid);

    if (!p_ccb)
        return;

    if (!p_cfg->mtu_present || p_cfg->mtu > L2CAP_MTU_SIZE)
        p_ccb->rem_mtu_size = L2CAP_MTU_SIZE;
    else
        p_ccb->rem_mtu_size = p_cfg->mtu;

    p_cfg->flush_to_present = FALSE;
    p_cfg->mtu_present = FALSE;
    p_cfg->result = L2CAP_CFG_OK;

    L2CA_ConfigRsp(l2cap_cid, p_cfg);

    p_ccb->con_flags |= GAP_CCB_FLAGS_THEIR_CFG_DONE;

    if (p_ccb->con_flags & GAP_CCB_FLAGS_OUR_CFG_DONE) {
        p_ccb->con_state = GAP_CCB_STATE_CONNECTED;

        (*p_ccb->p_callback)(p_ccb->gap_handle, GAP_EVT_CONN_OPENED);
    }
}

static void gap_config_cfm(UINT16 l2cap_cid, tL2CAP_CFG_INFO* p_cfg) {
    tGAP_CCB* p_ccb = gap_find_ccb_by_cid(l2cap_cid);

    if (!p_ccb)
        return;

    if (p_cfg->result == L2CAP_CFG_OK) {
        p_ccb->con_flags |= GAP_CCB_FLAGS_OUR_CFG_DONE;

        if (p_ccb->con_flags & GAP_CCB_FLAGS_THEIR_CFG_DONE) {
            p_ccb->con_state = GAP_CCB_STATE_CONNECTED;

            (*p_ccb->p_callback)(p_ccb->gap_handle, GAP_EVT_CONN_OPENED);
        }
    } else {
        (*p_ccb->p_callback)(p_ccb->gap_handle, GAP_EVT_CONN_CLOSED);
        gap_release_ccb(p_ccb);
    }
}

static void gap_disconnect_ind(UINT16 l2cap_cid, BOOLEAN ack_needed) {
    tGAP_CCB* p_ccb;

    GAP_TRACE(EVENT, "GAP_CONN - Rcvd L2CAP disc, CID: 0x%x", l2cap_cid);

    if ((p_ccb = gap_find_ccb_by_cid(l2cap_cid)) == NULL)
        return;

    if (ack_needed)
        L2CA_DisconnectRsp(l2cap_cid);

    (*p_ccb->p_callback)(p_ccb->gap_handle, GAP_EVT_CONN_CLOSED);
    gap_release_ccb(p_ccb);
}

static void gap_data_ind(UINT16 l2cap_cid, BT_HDR* p_msg) {
    tGAP_CCB* p_ccb = gap_find_ccb_by_cid(l2cap_cid);

    if (!p_ccb) {
        GKI_freebuf(p_msg);
        return;
    }

    if (p_ccb->con_state == GAP_CCB_STATE_CONNECTED) {
        GKI_enqueue(&p_ccb->rx_queue, p_msg);

        (*p_ccb->p_callback)(p_ccb->gap_handle, GAP_EVT_CONN_DATA_AVAIL);
    } else {
        GKI_freebuf(p_msg);
    }
}

static void gap_congestion_ind(UINT16 lcid, BOOLEAN is_congested) {
    tGAP_CCB* p_ccb;
    UINT16 event;

    GAP_TRACE(EVENT, "GAP_CONN - Rcvd L2CAP Is Congested (%d), CID: 0x%x", is_congested, lcid);

    if ((p_ccb = gap_find_ccb_by_cid(lcid)) == NULL)
        return;

    event = is_congested ? GAP_EVT_CONN_CONGESTED : GAP_EVT_CONN_UNCONGESTED;
    (*p_ccb->p_callback)(p_ccb->gap_handle, event);
}

static tGAP_CCB* gap_find_ccb_by_cid(UINT16 cid) {
    UINT16 xx;
    tGAP_CCB* p_ccb;

    /* explicitly post-increment */
    for (xx = 0, p_ccb = gap_cb.conn.ccb_pool; xx < GAP_MAX_CONNECTIONS; xx++, p_ccb++) {
        if (p_ccb->con_state != GAP_CCB_STATE_IDLE && p_ccb->connection_id == cid) {
            return p_ccb;
        }
    }

    return NULL;
}

static tGAP_CCB* gap_find_ccb_by_handle(UINT16 handle) {
    tGAP_CCB* p_ccb;

    if (handle < GAP_MAX_CONNECTIONS) {
        p_ccb = &gap_cb.conn.ccb_pool[handle];

        if (p_ccb->con_state != GAP_CCB_STATE_IDLE)
            return p_ccb;
    }

    return NULL;
}

static tGAP_CCB* gap_allocate_ccb(void) {
    UINT16 xx;
    tGAP_CCB* p_ccb;

    /* explicitly post-increment */
    for (xx = 0, p_ccb = gap_cb.conn.ccb_pool; xx < GAP_MAX_CONNECTIONS; xx++, p_ccb++) {
        if (p_ccb->con_state == GAP_CCB_STATE_IDLE) {
            memset(p_ccb, 0, sizeof *p_ccb);

            p_ccb->gap_handle = xx;
            p_ccb->rem_mtu_size = L2CAP_MTU_SIZE;

            return p_ccb;
        }
    }

    return NULL;
}

static void gap_release_ccb(tGAP_CCB* p_ccb) {
    UINT16 xx;
    UINT16 psm = p_ccb->psm;

    while (p_ccb->rx_queue.p_first)
        GKI_freebuf(GKI_dequeue(&p_ccb->rx_queue));

    p_ccb->con_state = GAP_CCB_STATE_IDLE;

    /* explicitly post-increment */
    for (xx = 0, p_ccb = gap_cb.conn.ccb_pool; xx < GAP_MAX_CONNECTIONS; xx++, p_ccb++) {
        if (p_ccb->con_state != GAP_CCB_STATE_IDLE && p_ccb->psm == psm)
            return;
    }

    L2CA_Deregister(psm);
}
