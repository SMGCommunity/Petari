#include "bta_hh_int.h"

/* Original source:
 * bluedroid <android.googlesource.com/platform/external/bluetooth/bluedroid>
 * bta/hh/bta_hh_act.c
 */

/******************************************************************************
 *
 *  Copyright (C) 2005-2012 Broadcom Corporation
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
#include "bt_types.h"  // BT_HDR
#include "data_types.h"

#include "bd.h"  // bdcpy
#include "bta_hh_api.h"
#include "bta_hh_co.h"
#include "bta_sys.h"
#include "gki.h"  // GKI_getbuf
#include "hiddefs.h"
#include "hidh_api.h"
#include "hidh_int.h"  // hh_cb
#include "l2c_int.h"
#include "sdp_api.h"  // SDP_SUCCESS
#include "utl.h"      // utl_freebuf

/*******************************************************************************
 * local function declarations
 */

static void bta_hh_sdp_cback(UINT16 result, UINT16 attr_mask, tHID_DEV_SDP_INFO* sdp_rec);

static void bta_hh_cback(UINT8 dev_handle, UINT8 event, UINT32 data, BT_HDR* pdata);
static tBTA_HH_STATUS bta_hh_get_trans_status(UINT32 result);
static char const* bta_hh_get_w4_event(UINT16 event);
static char const* bta_hh_hid_event_name(UINT16 event);

/*******************************************************************************
 * functions
 */

void bta_hh_api_enable(tBTA_HH_DATA* p_data) {
    tBTA_HH_STATUS status = BTA_HH_ERR;
    UINT8 xx;

    HID_HostInit();

    memset(&bta_hh_cb, 0, sizeof bta_hh_cb);

    HID_HostSetSecurityLevel("", p_data->api_enable.sec_mask);

    if (HID_HostRegister(&bta_hh_cback) == HID_SUCCESS) {
        bta_hh_cb.p_cback = p_data->api_enable.p_cback;

        status = BTA_HH_OK;

        for (xx = 0; xx < BTA_HH_MAX_KNOWN; ++xx) {
            bta_hh_cb.kdev[xx].state = BTA_HH_IDLE_ST;
            bta_hh_cb.kdev[xx].hid_handle = BTA_HH_INVALID_HANDLE;
            bta_hh_cb.kdev[xx].index = xx;

            bta_hh_cb.cb_index[xx] = BTA_HH_MAX_KNOWN;
        }
    }

    (*bta_hh_cb.p_cback)(BTA_HH_ENABLE_EVT, (tBTA_HH*)&status);
}

void bta_hh_api_disable(void) {
    UINT8 xx;

    if (!bta_hh_cb.p_cback)
        return;

    if (bta_hh_cb.cnt_num == 0) {
        bta_hh_disc_cmpl();
    } else {
        bta_hh_cb.w4_disable = TRUE;

        for (xx = 0; xx < BTA_HH_MAX_KNOWN; ++xx) {
            if (bta_hh_cb.kdev[xx].state == BTA_HH_CONN_ST) {
                bta_hh_sm_execute(&bta_hh_cb.kdev[xx], BTA_HH_API_CLOSE_EVT, NULL);
            }
        }
    }
}

void bta_hh_disc_cmpl(void) {
    UINT8 xx;
    tBTA_HH_STATUS status = BTA_HH_OK;

    if (HID_HostDeregister() != HID_SUCCESS)
        status = BTA_HH_ERR;

    for (xx = 0; xx < BTA_HH_MAX_KNOWN; ++xx)
        utl_freebuf((void**)&bta_hh_cb.kdev[xx].dscp_info.dsc_list);

    utl_freebuf((void**)&bta_hh_cb.p_disc_db);

    (*bta_hh_cb.p_cback)(BTA_HH_DISABLE_EVT, (tBTA_HH*)&status);

    memset(&bta_hh_cb, 0, sizeof bta_hh_cb);
}

static void bta_hh_sdp_cback(UINT16 result, UINT16 attr_mask, tHID_DEV_SDP_INFO* sdp_rec) {
    tBTA_HH_DEV_CB* p_cb = bta_hh_cb.p_cur;
    UINT8 hdl;
    tBTA_HH_STATUS status = BTA_HH_ERR_16;

    if (result == SDP_SUCCESS) {
        if (p_cb->sec_mask)
            attr_mask |= HID_SEC_REQUIRED;

        // NOTE: Do not touch
        APPL_TRACE(EVENT, "bta_hh_sdp_cback: p_cb: %d result 0x%02x, \
                            attr_mask 0x%02x",
                   p_cb, result, attr_mask);

        if (bta_hh_tod_spt(p_cb, sdp_rec->sub_class)) {
            if (p_cb->hid_handle == BTA_HH_INVALID_HANDLE) {
                if ((status = HID_HostAddDev(p_cb->addr, attr_mask, &hdl)) == HID_SUCCESS) {
                    bta_hh_cb.cb_index[hdl] = p_cb->index;

                    bta_hh_add_device_to_list(p_cb, hdl, attr_mask, &sdp_rec->dscp_info, sdp_rec->sub_class, p_cb->app_id);
                } else {
                    p_cb->app_id = 0;
                }
            }
        } else {
            status = BTA_HH_ERR_TOD_UNSPT;
        }
    }

    utl_freebuf((void**)&bta_hh_cb.p_disc_db);

    bta_hh_sm_execute(p_cb, BTA_HH_SDP_CMPL_EVT, (tBTA_HH_DATA*)&status);
}

void bta_hh_start_sdp(tBTA_HH_DEV_CB* p_cb, tBTA_HH_DATA* p_data) {
    tBTA_HH_STATUS status = BTA_HH_ERR_16;
    UINT8 hdl;

    p_cb->sec_mask = p_data->api_conn.sec_mask;
    p_cb->mode = p_data->api_conn.mode;

    if (p_cb->app_id != 0) {
        status = BTA_HH_OK;

        APPL_TRACE(DEBUG, "bta_hh_start_sdp:: skip SDP for known devices");

        if (p_cb->hid_handle == BTA_HH_INVALID_HANDLE) {
            if ((status = HID_HostAddDev(p_cb->addr, p_cb->attr_mask, &hdl)) == HID_SUCCESS) {
                bta_hh_add_device_to_list(p_cb, hdl, p_cb->attr_mask, NULL, p_cb->sub_class, p_cb->app_id);

                bta_hh_cb.cb_index[hdl] = p_cb->index;
            }
        }

        bta_hh_sm_execute(p_cb, BTA_HH_SDP_CMPL_EVT, (tBTA_HH_DATA*)&status);

        return;
    }

    if (!bta_hh_cb.p_disc_db) {
        bta_hh_cb.p_disc_db = GKI_getbuf(p_bta_hh_cfg->sdp_db_size);

        bta_hh_cb.p_cur = p_cb;

        if ((status = HID_HostGetSDPRecord(p_data->api_conn.bd_addr, bta_hh_cb.p_disc_db, p_bta_hh_cfg->sdp_db_size, &bta_hh_sdp_cback)) !=
            SDP_SUCCESS) {
            // NOTE: Do not touch
            APPL_TRACE(DEBUG, "bta_hh_start_sdp:  HID_HostGetSDPRecord failed: \
                Status 0x%2X",
                       status);

            status = BTA_HH_ERR_SDP;
            utl_freebuf((void**)&bta_hh_cb.p_disc_db);
        }
    }

    if (status != BTA_HH_OK)
        bta_hh_sm_execute(p_cb, BTA_HH_SDP_CMPL_EVT, (tBTA_HH_DATA*)&status);
}

void bta_hh_sdp_cmpl(tBTA_HH_DEV_CB* p_cb, tBTA_HH_DATA* p_data) {
    tBTA_HH_CONN conn_dat;
    tBTA_HH_STATUS status;

    APPL_TRACE(DEBUG, "bta_hh_sdp_cmpl:  status 0x%2X", p_data->status);

    memset(&conn_dat, 0, sizeof conn_dat);
    conn_dat.handle = p_cb->hid_handle;
    bdcpy(conn_dat.bda, p_cb->addr);

    status = p_data->status;

    if (status == BTA_HH_OK) {
        if (!p_cb->incoming_conn) {
            HID_HostSetSecurityLevel("", p_cb->sec_mask);

            if ((status = HID_HostOpenDev(p_cb->hid_handle)) != HID_SUCCESS) {
                // NOTE: Do not touch
                APPL_TRACE(DEBUG, "bta_hh_sdp_cmpl:  HID_HostOpenDev failed: \
                    Status 0x%2X",
                           status);

                HID_HostRemoveDev(p_cb->hid_handle);
            }
        } else {
            bta_hh_sm_execute(p_cb, BTA_HH_OPEN_CMPL_EVT, NULL);
        }
    }

    if (status != BTA_HH_OK) {
        if (p_data->status == BTA_HH_ERR_TOD_UNSPT)
            conn_dat.status = 10;
        else
            conn_dat.status = BTA_HH_ERR_SDP;

        (*bta_hh_cb.p_cback)(BTA_HH_OPEN_EVT, (tBTA_HH*)&conn_dat);

        bta_hh_sm_execute(p_cb, BTA_HH_API_CLOSE_EVT, NULL);

        if (p_cb->app_id == 0 && !p_cb->incoming_conn)
            bta_hh_clean_up_kdev(p_cb);

        bta_hh_trace_dev_db();
    }
}

void bta_hh_api_disc_act(tBTA_HH_DEV_CB* p_cb, tBTA_HH_DATA* p_data) {
    tBTA_HH_CBDATA disc_dat;
    tHID_STATUS status;

    disc_dat.handle = p_data ? (UINT8)p_data->hdr.layer_specific : p_cb->hid_handle;
    disc_dat.status = BTA_HH_ERR;

    status = HID_HostCloseDev(disc_dat.handle);

    if (status)
        (*bta_hh_cb.p_cback)(BTA_HH_CLOSE_EVT, (tBTA_HH*)&disc_dat);
}

void bta_hh_open_cmpl_act(tBTA_HH_DEV_CB* p_cb, tBTA_HH_DATA* p_data) {
    tBTA_HH_CONN conn;
    UINT8 dev_handle = p_data ? (UINT8)p_data->hid_cback.hdr.layer_specific : p_cb->hid_handle;

    memset(&conn, 0, sizeof conn);
    conn.handle = dev_handle;
    bdcpy(conn.bda, p_cb->addr);

    ++bta_hh_cb.cnt_num;

    bta_hh_co_open(p_cb->hid_handle, p_cb->sub_class, p_cb->attr_mask, p_cb->app_id);

    bta_sys_conn_open(BTA_ID_HH, p_cb->app_id, p_cb->addr);

    if (p_cb->mode != BTA_HH_PROTO_RPT_MODE) {
        if ((conn.status = HID_HostWriteDev(dev_handle, HID_TRANS_SET_PROTOCOL, p_cb->mode, 0, 0, NULL)) != HID_SUCCESS) {
            conn.status = BTA_HH_ERR_PROTO;

            (*bta_hh_cb.p_cback)(BTA_HH_OPEN_EVT, (tBTA_HH*)&conn);
        } else {
            p_cb->w4_evt = BTA_HH_OPEN_EVT;
        }
    } else {
        p_cb->opened = TRUE;

        (*bta_hh_cb.p_cback)(BTA_HH_OPEN_EVT, (tBTA_HH*)&conn);
    }

    p_cb->incoming_conn = FALSE;
}

void bta_hh_open_act(tBTA_HH_DEV_CB* p_cb, tBTA_HH_DATA* p_data) {
    tBTA_HH_API_CONN conn_data;

    UINT8 dev_handle = p_data ? (UINT8)p_data->hid_cback.hdr.layer_specific : p_cb->hid_handle;

    APPL_TRACE(EVENT, "bta_hh_open_act:  Device[%d] connected", dev_handle);

    if (p_cb->app_id != 0) {
        bta_hh_sm_execute(p_cb, BTA_HH_OPEN_CMPL_EVT, p_data);
    } else {
        p_cb->incoming_conn = TRUE;

        memset(&conn_data, 0, sizeof conn_data);
        bdcpy(conn_data.bd_addr, p_cb->addr);

        bta_hh_start_sdp(p_cb, (tBTA_HH_DATA*)&conn_data);
    }
}

void bta_hh_data_act(tBTA_HH_DEV_CB* p_cb, tBTA_HH_DATA* p_data) {
    BT_HDR* pdata = p_data->hid_cback.p_data;
    UINT8* p_rpt = (UINT8*)(pdata + 1) + pdata->offset;

    bta_hh_co_data(p_data->hid_cback.hdr.layer_specific, p_rpt, pdata->len, p_cb->mode, p_cb->sub_class, p_cb->app_id);

    utl_freebuf((void**)&pdata);
}

void bta_hh_handsk_act(tBTA_HH_DEV_CB* p_cb, tBTA_HH_DATA* p_data) {
    tBTA_HH_CBDATA cback_data;
    tBTA_HH_HSDATA hs_data;
    tBTA_HH_CONN conn;

    APPL_TRACE(DEBUG, "HANDSHAKE received for: event = %s data= %d", bta_hh_get_w4_event(p_cb->w4_evt), p_data->hid_cback.data);

    switch (p_cb->w4_evt) {
    case BTA_HH_GET_PROTO_EVT:
    case BTA_HH_GET_RPT_EVT:
    case BTA_HH_GET_IDLE_EVT:
        memset(&hs_data, 0, sizeof hs_data);
        hs_data.handle = p_cb->hid_handle;
        hs_data.status = bta_hh_get_trans_status(p_data->hid_cback.data);

        (*bta_hh_cb.p_cback)(p_cb->w4_evt, (tBTA_HH*)&hs_data);

        p_cb->w4_evt = 0;
        break;

    case BTA_HH_SET_RPT_EVT:
    case BTA_HH_SET_PROTO_EVT:
    case BTA_HH_SET_IDLE_EVT:
        cback_data.handle = p_cb->hid_handle;
        cback_data.status = bta_hh_get_trans_status(p_data->hid_cback.data);

        (*bta_hh_cb.p_cback)(p_cb->w4_evt, (tBTA_HH*)&cback_data);

        p_cb->w4_evt = 0;
        break;

    case BTA_HH_OPEN_EVT:
        conn.status = p_data->hid_cback.data ? BTA_HH_ERR_PROTO : BTA_HH_OK;
        conn.handle = p_cb->hid_handle;
        bdcpy(conn.bda, p_cb->addr);

        (*bta_hh_cb.p_cback)(p_cb->w4_evt, (tBTA_HH*)&conn);

        bta_hh_trace_dev_db();

        p_cb->w4_evt = 0;

        if (conn.status == BTA_HH_OK)
            p_cb->opened = TRUE;

        break;

    default:
        APPL_TRACE(DEBUG, "unknown transaction type");
        break;
    }

    bta_sys_idle(BTA_ID_HH, p_cb->app_id, p_cb->addr);
}

void bta_hh_ctrl_dat_act(tBTA_HH_DEV_CB* p_cb, tBTA_HH_DATA* p_data) {
    BT_HDR* pdata = p_data->hid_cback.p_data;
    UINT8* data = (UINT8*)(pdata + 1) + pdata->offset;
    tBTA_HH_HSDATA hs_data;

    APPL_TRACE(DEBUG, "Ctrl DATA received w4: event[%s]", bta_hh_get_w4_event(p_cb->w4_evt));

    hs_data.status = BTA_HH_OK;
    hs_data.handle = p_cb->hid_handle;

    switch (p_cb->w4_evt) {
    case BTA_HH_GET_IDLE_EVT:
        hs_data.rsp_data.idle_rate = *data;
        break;

    case BTA_HH_GET_RPT_EVT:
        memcpy(&hs_data.rsp_data.rpt_data, pdata, sizeof hs_data.rsp_data.rpt_data);

        break;

    case BTA_HH_GET_PROTO_EVT:
        hs_data.rsp_data.proto_mode = (UINT8)*data;  // ?
        break;

    default:
    case BTA_HH_SET_PROTO_EVT:
    case BTA_HH_SET_RPT_EVT:
    case BTA_HH_SET_IDLE_EVT:
        APPL_TRACE(DEBUG, "invalid  transaction type for DATA payload: 4_evt[%s]", bta_hh_get_w4_event(p_cb->w4_evt));

        break;
    }

    bta_sys_busy(BTA_ID_HH, p_cb->app_id, p_cb->addr);
    bta_sys_idle(BTA_ID_HH, p_cb->app_id, p_cb->addr);

    (*bta_hh_cb.p_cback)(p_cb->w4_evt, (tBTA_HH*)&hs_data);

    p_cb->w4_evt = 0;
    utl_freebuf((void**)&pdata);
}

void bta_hh_close_act(tBTA_HH_DEV_CB* p_cb, tBTA_HH_DATA* p_data) {
    tBTA_HH_CONN conn_dat;
    tBTA_HH_CBDATA disc_dat = {BTA_HH_OK, 0};
    UINT32 reason = p_data->hid_cback.data;
    UINT16 event = p_cb->vp ? BTA_HH_VC_UNPLUG_EVT : BTA_HH_CLOSE_EVT;

    disc_dat.handle = p_cb->hid_handle;
    disc_dat.status = p_data->hid_cback.data;

    if (reason & (HID_L2CAP_CONN_FAIL | HID_L2CAP_REQ_FAIL | HID_L2CAP_CFG_FAIL) || reason == HID_ERR_AUTH_FAILED || reason == HID_ERR_L2CAP_FAILED ||
        !p_cb->opened) {
        conn_dat.handle = p_cb->hid_handle;
        conn_dat.status = reason == HID_ERR_AUTH_FAILED ? BTA_HH_ERR_AUTH_FAILED : BTA_HH_ERR;
        bdcpy(conn_dat.bda, p_cb->addr);
        HID_HostCloseDev(p_cb->hid_handle);

        (*bta_hh_cb.p_cback)(BTA_HH_OPEN_EVT, (tBTA_HH*)&conn_dat);

        bta_hh_trace_dev_db();
        return;
    }

    bta_hh_co_close(p_cb->hid_handle, p_cb->app_id);
    bta_sys_conn_close(BTA_ID_HH, p_cb->app_id, p_cb->addr);

    --bta_hh_cb.cnt_num;

    p_cb->opened = FALSE;

    if (disc_dat.status)
        disc_dat.status = BTA_HH_ERR;

    (*bta_hh_cb.p_cback)(event, (tBTA_HH*)&disc_dat);

    if (p_cb->vp) {
        HID_HostRemoveDev(p_cb->hid_handle);
        bta_hh_clean_up_kdev(p_cb);
    }

    bta_hh_trace_dev_db();

    p_cb->vp = FALSE;
    p_cb->w4_evt = 0;

    if (bta_hh_cb.cnt_num == 0 && bta_hh_cb.w4_disable)
        bta_hh_disc_cmpl();
}

void bta_hh_get_dscp_act(tBTA_HH_DEV_CB* p_cb, tBTA_HH_DATA* p_data) {
    (*bta_hh_cb.p_cback)(BTA_HH_GET_DSCP_EVT, (tBTA_HH*)&p_cb->dscp_info);
}

void bta_hh_maint_dev_act(tBTA_HH_DEV_CB* p_cb, tBTA_HH_DATA* p_data) {
    tBTA_HH_MAINT_DEV* p_dev_info = &p_data->api_maintdev;
    tBTA_HH_DEV_INFO dev_info;
    UINT8 dev_handle;

    dev_info.status = BTA_HH_ERR;
    dev_info.handle = BTA_HH_INVALID_HANDLE;

    switch (p_dev_info->sub_event) {
    case BTA_HH_ADD_DEV_EVT:
        bdcpy(dev_info.bda, p_dev_info->bda);

        if (p_cb->hid_handle == BTA_HH_INVALID_HANDLE) {
            if (HID_HostAddDev(p_dev_info->bda, p_dev_info->attr_mask, &dev_handle) == HID_SUCCESS) {
                dev_info.handle = dev_handle;
                dev_info.status = BTA_HH_OK;

                bta_hh_add_device_to_list(p_cb, dev_handle, p_dev_info->attr_mask, NULL, p_dev_info->sub_class, p_dev_info->app_id);

                bta_hh_cb.cb_index[dev_handle] = p_cb->index;
            }
        } else {
            dev_info.handle = p_cb->hid_handle;
            dev_info.status = BTA_HH_OK;
        }

        bta_hh_trace_dev_db();

        break;

    case BTA_HH_RMV_DEV_EVT:
        dev_info.handle = p_dev_info->hdr.layer_specific;

        bdcpy(dev_info.bda, p_cb->addr);
        if (p_cb->state != BTA_HH_CONN_ST) {
            if (HID_HostRemoveDev(dev_info.handle) == HID_SUCCESS) {
                dev_info.status = BTA_HH_OK;

                bta_hh_clean_up_kdev(p_cb);
            }
        }

        break;

    default:
        APPL_TRACE(DEBUG, "invalid command");
        break;
    }

    (*bta_hh_cb.p_cback)(p_dev_info->sub_event, (tBTA_HH*)&dev_info);
}

void bta_hh_get_acl_q_info(void) {
    tBTA_HH_ACL_QUEUE_INFO get_queue_info;
    tBTA_HH_DEV_CB* p_cb;
    UINT8 num_links = 0;
    UINT8 i;
    tL2C_CCB* p_l2c_ccb;
    tL2C_LCB* p_l2c_lcb;

    memset(&get_queue_info, 0, sizeof get_queue_info);

    for (i = 0; i < BTA_HH_MAX_KNOWN; ++i) {
        p_cb = &bta_hh_cb.kdev[i];

        if (p_cb->in_use && p_cb->state == BTA_HH_CONN_ST) {
            get_queue_info.queue_info[num_links].handle = p_cb->hid_handle;

            if ((p_l2c_ccb = l2cu_find_ccb_by_cid(NULL, hh_cb.devices[p_cb->hid_handle].conn.intr_cid)) != NULL) {
                if (((p_l2c_lcb = p_l2c_ccb->p_lcb) != NULL) && p_l2c_lcb->in_use) {
                    get_queue_info.queue_info[num_links].num_queued = p_l2c_lcb->link_xmit_data_q.count;
                    get_queue_info.queue_info[num_links].num_not_acked = p_l2c_lcb->sent_not_acked;
                }
            }

            ++num_links;
        }
    }

    get_queue_info.num_avail_buffs = l2cb.controller_xmit_window;
    get_queue_info.num_buffs = l2cb.num_lm_acl_bufs;
    get_queue_info.num_links = num_links;

    (*bta_hh_cb.p_cback)(BTA_HH_GET_ACL_Q_EVT, (tBTA_HH*)&get_queue_info);
}

void bta_hh_write_dev_act(tBTA_HH_DEV_CB* p_cb, tBTA_HH_DATA* p_data) {
    tBTA_HH_CBDATA cbdata = {BTA_HH_OK, 0};
    UINT16 event = p_data->api_sndcmd.t_type - BTA_HH_FST_BTE_TRANS_EVT + BTA_HH_FST_TRANS_CB_EVT;
    tHID_STATUS hid_status;

    cbdata.handle = p_cb->hid_handle;

    if ((hid_status = HID_HostWriteDev(p_cb->hid_handle, p_data->api_sndcmd.t_type, p_data->api_sndcmd.param, p_data->api_sndcmd.data,
                                       p_data->api_sndcmd.rpt_id, p_data->api_sndcmd.p_data)) != HID_SUCCESS) {
        APPL_TRACE(ERROR, "HID_HostWriteDev Error %d", hid_status);
        cbdata.status = BTA_HH_ERR;

        if (p_data->api_sndcmd.t_type != HID_TRANS_CONTROL && p_data->api_sndcmd.t_type != HID_TRANS_DATA) {
            (*bta_hh_cb.p_cback)(event, (tBTA_HH*)&cbdata);
        } else if (p_data->api_sndcmd.param == BTA_HH_CTRL_VIRTUAL_CABLE_UNPLUG) {
            (*bta_hh_cb.p_cback)(BTA_HH_VC_UNPLUG_EVT, (tBTA_HH*)&cbdata);
        }
    } else {
        switch (p_data->api_sndcmd.t_type) {
        case HID_TRANS_SET_PROTOCOL:
        case HID_TRANS_GET_REPORT:
        case HID_TRANS_SET_REPORT:
        case HID_TRANS_GET_PROTOCOL:
        case HID_TRANS_GET_IDLE:
        case HID_TRANS_SET_IDLE:
            p_cb->w4_evt = event;
            break;

        case HID_TRANS_DATA:
        case HID_TRANS_CONTROL:
            if (p_data->api_sndcmd.param == BTA_HH_CTRL_VIRTUAL_CABLE_UNPLUG)
                p_cb->vp = TRUE;

            break;

        default:
        case HID_TRANS_DATAC:
            APPL_TRACE(DEBUG, "bta_hh_write_dev_act:: cmd type = %d", p_data->api_sndcmd.t_type);

            break;
        }

        if (p_data->api_sndcmd.t_type != HID_TRANS_CONTROL) {
            bta_sys_busy(BTA_ID_HH, p_cb->app_id, p_cb->addr);
            bta_sys_idle(BTA_ID_HH, p_cb->app_id, p_cb->addr);
        } else if (p_data->api_sndcmd.param == BTA_HH_CTRL_SUSPEND) {
            bta_sys_sco_close(BTA_ID_HH, p_cb->app_id, p_cb->addr);
        } else if (p_data->api_sndcmd.param == BTA_HH_CTRL_EXIT_SUSPEND) {
            bta_sys_busy(BTA_ID_HH, p_cb->app_id, p_cb->addr);
        }
    }
}

static void bta_hh_cback(UINT8 dev_handle, UINT8 event, UINT32 data, BT_HDR* pdata) {
    tBTA_HH_CBACK_DATA* p_buf = NULL;
    UINT16 sm_event = BTA_HH_INVALID_EVT;
    UINT8 xx = 0;

    APPL_TRACE(DEBUG, "bta_hh_cback::HID_event [%s]", bta_hh_hid_event_name(event));

    switch (event) {
    case HID_HDEV_EVT_OPEN:
        sm_event = BTA_HH_INT_OPEN_EVT;
        break;

    case HID_HDEV_EVT_CLOSE:
        sm_event = BTA_HH_INT_CLOSE_EVT;
        break;

    case HID_HDEV_EVT_INTR_DATA:
        sm_event = BTA_HH_INT_DATA_EVT;
        break;

    case HID_HDEV_EVT_HANDSHAKE:
        sm_event = BTA_HH_INT_HANDSK_EVT;
        break;

    case HID_HDEV_EVT_CTRL_DATA:
        sm_event = BTA_HH_INT_CTRL_DATA;
        break;

    case HID_HDEV_EVT_RETRYING:
        break;

    case HID_HDEV_EVT_INTR_DATC:
    case HID_HDEV_EVT_CTRL_DATC:
        utl_freebuf((void**)&pdata);
        break;

    case HID_HDEV_EVT_VC_UNPLUG:
        for (xx = 0; xx < BTA_HH_MAX_KNOWN; ++xx) {
            if (bta_hh_cb.kdev[xx].hid_handle == dev_handle) {
                bta_hh_cb.kdev[xx].vp = TRUE;
                break;
            }
        }

        break;
    }

    if (sm_event != BTA_HH_INVALID_EVT && (p_buf = GKI_getbuf(sizeof *p_buf + sizeof *p_buf->p_data)) != NULL) {
        p_buf->hdr.event = sm_event;
        p_buf->hdr.layer_specific = dev_handle;
        p_buf->data = data;
        p_buf->p_data = pdata;

        bta_sys_sendmsg(p_buf);
    }
}

static tBTA_HH_STATUS bta_hh_get_trans_status(UINT32 result) {
    switch (result) {
    case HID_PAR_HANDSHAKE_RSP_SUCCESS:
        return BTA_HH_OK;

    case HID_PAR_HANDSHAKE_RSP_NOT_READY:
    case HID_PAR_HANDSHAKE_RSP_ERR_INVALID_REP_ID:
    case HID_PAR_HANDSHAKE_RSP_ERR_UNSUPPORTED_REQ:
    case HID_PAR_HANDSHAKE_RSP_ERR_INVALID_PARAM:
        return result;

    default:
    case HID_PAR_HANDSHAKE_RSP_ERR_UNKNOWN:
    case HID_PAR_HANDSHAKE_RSP_ERR_FATAL:
        return BTA_HH_HS_ERROR;
    }
}

static char const* bta_hh_get_w4_event(UINT16 event) {
    switch (event) {
    case BTA_HH_GET_RPT_EVT:
        return "BTA_HH_GET_RPT_EVT";

    case BTA_HH_SET_RPT_EVT:
        return "BTA_HH_SET_RPT_EVT";

    case BTA_HH_GET_PROTO_EVT:
        return "BTA_HH_GET_PROTO_EVT";

    case BTA_HH_SET_PROTO_EVT:
        return "BTA_HH_SET_PROTO_EVT";

    case BTA_HH_GET_IDLE_EVT:
        return "BTA_HH_GET_IDLE_EVT";

    case BTA_HH_SET_IDLE_EVT:
        return "BTA_HH_SET_IDLE_EVT";

    case BTA_HH_OPEN_EVT:
        return "BTA_HH_OPEN_EVT";

    default:
        return "Unknown event";
    }
}

static char const* bta_hh_hid_event_name(UINT16 event) {
    switch (event) {
    case HID_HDEV_EVT_OPEN:
        return "HID_HDEV_EVT_OPEN";

    case HID_HDEV_EVT_CLOSE:
        return "HID_HDEV_EVT_CLOSE";

    case HID_HDEV_EVT_RETRYING:
        return "HID_HDEV_EVT_RETRYING";

    case HID_HDEV_EVT_INTR_DATA:
        return "HID_HDEV_EVT_INTR_DATA";

    case HID_HDEV_EVT_INTR_DATC:
        return "HID_HDEV_EVT_INTR_DATC";

    case HID_HDEV_EVT_CTRL_DATA:
        return "HID_HDEV_EVT_CTRL_DATA";

    case HID_HDEV_EVT_CTRL_DATC:
        return "HID_HDEV_EVT_CTRL_DATC";

    case HID_HDEV_EVT_HANDSHAKE:
        return "HID_HDEV_EVT_HANDSHAKE";

    case HID_HDEV_EVT_VC_UNPLUG:
        return "HID_HDEV_EVT_VC_UNPLUG";

    default:
        return "Unknown HID event";
    }
}
