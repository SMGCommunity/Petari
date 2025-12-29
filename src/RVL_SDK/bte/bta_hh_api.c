#include "bta_hh_api.h"

/* Original source:
 * bluedroid <android.googlesource.com/platform/external/bluetooth/bluedroid>
 * bta/hh/bta_hh_api.c
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
#include "bt_types.h"
#include "data_types.h"

#include "bd.h"       // bdcpy
#include "bta_api.h"  // tBTA_SEC
#include "bta_hh_int.h"
#include "bta_sys.h"
#include "gki.h"
#include "hiddefs.h"

/*******************************************************************************
 * local function declarations
 */

static void bta_hh_snd_write_dev(UINT8 dev_handle, UINT8 t_type, UINT8 param, UINT16 data, UINT8 rpt_id, BT_HDR* p_data);

/*******************************************************************************
 * variables
 */

// .sdata2
static tBTA_SYS_REG const bta_hh_reg = {&bta_hh_hdl_event, &BTA_HhDisable};

/*******************************************************************************
 * functions
 */

void BTA_HhEnable(tBTA_SEC sec_mask, tBTA_HH_CBACK* p_cback) {
    tBTA_HH_API_ENABLE* p_buf;

    GKI_sched_lock();

    bta_sys_register(BTA_ID_HH, &bta_hh_reg);

    GKI_sched_unlock();

    if ((p_buf = GKI_getbuf(sizeof *p_buf)) != NULL) {
        memset(p_buf, 0, sizeof *p_buf);

        p_buf->hdr.event = BTA_HH_API_ENABLE_EVT;
        p_buf->p_cback = p_cback;
        p_buf->sec_mask = sec_mask;

        bta_sys_sendmsg(p_buf);
    }
}

void BTA_HhDisable(void) {
    BT_HDR* p_buf;

    if ((p_buf = GKI_getbuf(sizeof *p_buf)) != NULL) {
        // no memset? is this a bug?

        p_buf->event = BTA_HH_API_DISABLE_EVT;

        bta_sys_sendmsg(p_buf);
    }
}

void BTA_HhClose(UINT8 dev_handle) {
    BT_HDR* p_buf;

    if ((p_buf = GKI_getbuf(sizeof *p_buf)) != NULL) {
        memset(p_buf, 0, sizeof *p_buf);

        p_buf->event = BTA_HH_API_CLOSE_EVT;
        p_buf->layer_specific = dev_handle;

        bta_sys_sendmsg(p_buf);
    }
}

void BTA_HhOpen(BD_ADDR dev_bda, tBTA_HH_PROTO_MODE mode, tBTA_SEC sec_mask) {
    tBTA_HH_API_CONN* p_buf;

    if ((p_buf = GKI_getbuf(sizeof *p_buf)) != NULL) {
        memset(p_buf, 0, sizeof *p_buf);

        p_buf->hdr.event = BTA_HH_API_OPEN_EVT;
        p_buf->hdr.layer_specific = BTA_HH_INVALID_HANDLE;
        p_buf->sec_mask = sec_mask;
        p_buf->mode = mode;
        bdcpy(p_buf->bd_addr, dev_bda);

        bta_sys_sendmsg(p_buf);
    } else {
        APPL_TRACE(ERROR, "No resource to send HID host Connect request.");
    }
}

static void bta_hh_snd_write_dev(UINT8 dev_handle, UINT8 t_type, UINT8 param, UINT16 data, UINT8 rpt_id, BT_HDR* p_data) {
    tBTA_HH_CMD_DATA* p_buf;
    UINT16 len = sizeof *p_buf;  // ok

    if ((p_buf = GKI_getbuf(len)) != NULL) {
        memset(p_buf, 0, sizeof *p_buf);

        p_buf->hdr.event = BTA_HH_API_WRITE_DEV_EVT;
        p_buf->hdr.layer_specific = dev_handle;
        p_buf->t_type = t_type;
        p_buf->data = data;
        p_buf->param = param;
        p_buf->p_data = p_data;
        p_buf->rpt_id = rpt_id;

        bta_sys_sendmsg(p_buf);
    }
}

void BTA_HhSetReport(UINT8 dev_handle, tBTA_HH_RPT_TYPE r_type, BT_HDR* p_data) {
    bta_hh_snd_write_dev(dev_handle, HID_TRANS_SET_REPORT, r_type, 0, 0, p_data);
}

void BTA_HhGetReport(UINT8 dev_handle, tBTA_HH_RPT_TYPE r_type, UINT8 rpt_id, UINT16 buf_size) {
    UINT8 param = buf_size ? r_type | 0x08 : r_type;

    bta_hh_snd_write_dev(dev_handle, HID_TRANS_GET_REPORT, param, buf_size, rpt_id, NULL);
}

void BTA_HhSetProtoMode(UINT8 dev_handle, tBTA_HH_PROTO_MODE p_type) {
    bta_hh_snd_write_dev(dev_handle, HID_TRANS_SET_PROTOCOL, (UINT8)p_type, 0, 0, NULL);
}

void BTA_HhGetProtoMode(UINT8 dev_handle) {
    bta_hh_snd_write_dev(dev_handle, HID_TRANS_GET_PROTOCOL, 0, 0, 0, NULL);
}

void BTA_HhSetIdle(UINT8 dev_handle, UINT16 idle_rate) {
    bta_hh_snd_write_dev(dev_handle, HID_TRANS_SET_IDLE, 0, idle_rate, 0, NULL);
}

void BTA_HhGetIdle(UINT8 dev_handle) {
    bta_hh_snd_write_dev(dev_handle, HID_TRANS_GET_IDLE, 0, 0, 0, NULL);
}

void BTA_HhSendCtrl(UINT8 dev_handle, tBTA_HH_TRANS_CTRL_TYPE c_type) {
    bta_hh_snd_write_dev(dev_handle, HID_TRANS_CONTROL, (UINT8)c_type, 0, 0, NULL);
}

void BTA_HhSendData(UINT8 dev_handle, BT_HDR* p_data) {
    bta_hh_snd_write_dev(dev_handle, HID_TRANS_DATA, BTA_HH_RPTT_OUTPUT, 0, 0, p_data);
}

void BTA_HhGetDscpInfo(UINT8 dev_handle) {
    BT_HDR* p_buf;

    if ((p_buf = GKI_getbuf(sizeof *p_buf)) != NULL) {
        memset(p_buf, 0, sizeof *p_buf);

        p_buf->event = BTA_HH_API_GET_DSCP_EVT;
        p_buf->layer_specific = dev_handle;

        bta_sys_sendmsg(p_buf);
    }
}

void BTA_HhAddDev(BD_ADDR bda, tBTA_HH_ATTR_MASK attr_mask, UINT8 sub_class, UINT8 app_id, tHID_DEV_DSCP_INFO dscp_info) {
    tBTA_HH_MAINT_DEV* p_buf;

    if ((p_buf = GKI_getbuf(sizeof *p_buf)) != NULL) {
        memset(p_buf, 0, sizeof *p_buf);

        p_buf->hdr.event = BTA_HH_API_MAINT_DEV_EVT;
        p_buf->sub_event = BTA_HH_ADD_DEV_EVT;
        p_buf->hdr.layer_specific = BTA_HH_INVALID_HANDLE;
        p_buf->attr_mask = (UINT16)attr_mask;
        p_buf->sub_class = sub_class;
        p_buf->app_id = app_id;
        memcpy(&p_buf->dscp_info, &dscp_info, sizeof p_buf->dscp_info);
        bdcpy(p_buf->bda, bda);

        bta_sys_sendmsg(p_buf);
    }
}

void BTA_HhRemoveDev(UINT8 dev_handle) {
    tBTA_HH_MAINT_DEV* p_buf;

    if ((p_buf = GKI_getbuf(sizeof *p_buf)) != NULL) {
        memset(p_buf, 0, sizeof *p_buf);

        p_buf->hdr.event = BTA_HH_API_MAINT_DEV_EVT;
        p_buf->sub_event = BTA_HH_RMV_DEV_EVT;
        p_buf->hdr.layer_specific = dev_handle;

        bta_sys_sendmsg(p_buf);
    }
}

void BTA_HhGetAclQueueInfo(void) {
    BT_HDR* p_buf;

    if ((p_buf = GKI_getbuf(sizeof *p_buf)) != NULL) {
        memset(p_buf, 0, sizeof *p_buf);

        p_buf->event = BTA_HH_API_GET_ACL_Q_EVT;

        bta_sys_sendmsg(p_buf);
    }
}

void BTA_HhParseBootRpt(tBTA_HH_BOOT_RPT* p_data, UINT8* p_report, UINT16 report_len) {
    p_data->dev_type = BTA_HH_DEVT_UNKNOWN;

    if (p_report) {
        switch (p_report[0]) {
        case BTA_HH_KEYBD_RPT_ID:
            p_data->dev_type = p_report[0];
            bta_hh_parse_keybd_rpt(p_data, p_report + 1, report_len - 1);
            break;

        case BTA_HH_MOUSE_RPT_ID:
            p_data->dev_type = p_report[0];
            bta_hh_parse_mice_rpt(p_data, p_report + 1, report_len - 1);
            break;

        default:
            APPL_TRACE(DEBUG, "Unknown boot report: %d", p_report[0]);
            break;
        }
    }
}
