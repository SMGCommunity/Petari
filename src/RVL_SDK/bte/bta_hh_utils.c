#include "bta_hh_int.h"

/* Original source:
 * bluedroid <android.googlesource.com/platform/external/bluetooth/bluedroid>
 * bt/hh/bta_hh_utils.c
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
#include "bt_types.h"  // BD_ADDR
#include "data_types.h"

#include "bd.h"
#include "bta_hh_api.h"
#include "gki.h"      // GKI_getbuf
#include "hiddefs.h"  // tHID_DEV_DSCP_INFO
#include "utl.h"      // utl_freebuf

/*******************************************************************************
 * macros
 */

#define BTA_HH_KB_CTRL_MASK 0x11
#define BTA_HH_KB_SHIFT_MASK 0x22
#define BTA_HH_KB_ALT_MASK 0x44

#define BTA_HH_KB_CAPS_LOCK 0x39
#define BTA_HH_KB_NUM_LOCK 0x53

/*******************************************************************************
 * variables
 */

// TODO: Are these USB HID key codes?

// .rodata
static UINT8 const vir_key_tbl_1[(0x38 + 1) - 0x27] = {
    0x30, 0x0d, 0x1b, 0x08, 0x09, 0x20, 0xbd, 0xbb, 0xdb, 0xdd, 0xdc, 0xdc, 0xba, 0xde, 0xc0, 0xbc, 0xbe, 0xbf,
};

static UINT8 const vir_key_tbl_2[(0x63 + 1) - 0x49] = {
    0x2d, 0x24, 0x21, 0x2e, 0x23, 0x22, 0x27, 0x25, 0x28, 0x26, 0xff, 0xbf, 0x6a, 0x6d,
    0x6b, 0x0d, 0x23, 0x28, 0x22, 0x25, 0x00, 0x27, 0x24, 0x26, 0x21, 0x2d, 0x2e,
};

/*******************************************************************************
 * functions
 */

UINT8 bta_hh_find_cb(BD_ADDR bda) {
    UINT8 xx;

    for (xx = 0; xx < BTA_HH_MAX_KNOWN; ++xx) {
        if (bdcmp(bda, bta_hh_cb.kdev[xx].addr) == 0 && bdcmp(bda, bd_addr_null) != 0) {
            APPL_TRACE(DEBUG, "found kdev_cb[%d] hid_handle = %d ", xx, bta_hh_cb.kdev[xx].hid_handle);

            return xx;
        } else {
            APPL_TRACE(DEBUG, "in_use ? [%d] kdev[%d].hid_handle = %d state = [%d]", bta_hh_cb.kdev[xx].in_use, xx, bta_hh_cb.kdev[xx].hid_handle,
                       bta_hh_cb.kdev[xx].state);
        }
    }

    for (xx = 0; xx < BTA_HH_MAX_KNOWN; ++xx) {
        if (!bta_hh_cb.kdev[xx].in_use) {
            bdcpy(bta_hh_cb.kdev[xx].addr, bda);
            break;
        }
    }

    APPL_TRACE(DEBUG, "bta_hh_find_cb:: index = %d while max = %d", xx, BTA_HH_MAX_KNOWN);

    return xx;
}

void bta_hh_clean_up_kdev(tBTA_HH_DEV_CB* p_cb) {
    UINT8 index;

    if (p_cb->hid_handle != BTA_HH_INVALID_HANDLE)
        bta_hh_cb.cb_index[p_cb->hid_handle] = BTA_HH_MAX_KNOWN;

    index = p_cb->index;

    utl_freebuf((void**)&p_cb->dscp_info.dsc_list);

    memset(p_cb, 0, sizeof *p_cb);

    p_cb->index = index;
    p_cb->state = BTA_HH_IDLE_ST;
    p_cb->hid_handle = BTA_HH_INVALID_HANDLE;
}

void bta_hh_add_device_to_list(tBTA_HH_DEV_CB* p_cb, UINT8 handle, UINT16 attr_mask, tHID_DEV_DSCP_INFO* p_dscp_info, UINT8 sub_class, UINT8 app_id) {
    APPL_TRACE(DEBUG, "subclass = 0x%2x", sub_class);

    p_cb->hid_handle = handle;
    p_cb->in_use = TRUE;
    p_cb->attr_mask = attr_mask;

    p_cb->sub_class = sub_class;
    p_cb->app_id = app_id;

    if (p_dscp_info) {
        utl_freebuf((void**)&p_cb->dscp_info.dsc_list);

        if (p_dscp_info->dl_len && (p_cb->dscp_info.dsc_list = GKI_getbuf(p_dscp_info->dl_len)) != NULL) {
            p_cb->dscp_info.dl_len = p_dscp_info->dl_len;
            memcpy(p_cb->dscp_info.dsc_list, p_dscp_info->dsc_list, p_dscp_info->dl_len);
        }
    }
}

BOOLEAN bta_hh_tod_spt(tBTA_HH_DEV_CB* p_cb, UINT8 sub_class) {
    UINT8 xx;
    UINT8 cod = sub_class >> 2;

    for (xx = 0; xx < p_bta_hh_cfg->max_devt_spt; ++xx) {
        if (cod == (UINT8)p_bta_hh_cfg->p_devt_list[xx].tod) {
            p_cb->app_id = p_bta_hh_cfg->p_devt_list[xx].app_id;

            return TRUE;
        }
    }

    return FALSE;
}

void bta_hh_parse_keybd_rpt(tBTA_HH_BOOT_RPT* p_kb_data, UINT8* p_report, UINT16 report_len) {
    tBTA_HH_KB_CB* p_kb = &bta_hh_cb.kb_cb;
    tBTA_HH_KEYBD_RPT* p_data = &p_kb_data->data_rpt.keybd_rpt;
    UINT8 this_char;
    UINT8 ctl_shift;
    UINT16 xx;
    UINT16 yy;
    UINT8 this_report[BTA_HH_MAX_RPT_CHARS];

    APPL_TRACE(DEBUG, "bta_hh_parse_keybd_rpt:  (report=%p, report_len=%d) called", p_report, report_len);

    if (report_len < 2)
        return;

    ctl_shift = *p_report++;
    --report_len;

    if (report_len > BTA_HH_MAX_RPT_CHARS)
        report_len = BTA_HH_MAX_RPT_CHARS;

    memset(this_report, 0, BTA_HH_MAX_RPT_CHARS);
    memcpy(this_report, p_report, report_len);

    if (ctl_shift & BTA_HH_KB_SHIFT_MASK)
        p_kb->upper = TRUE;
    else if (p_kb->upper)
        p_kb->upper = FALSE;

    if (ctl_shift & BTA_HH_KB_CTRL_MASK)
        p_kb->ctrl = TRUE;
    else if (p_kb->ctrl)
        p_kb->ctrl = FALSE;

    if (ctl_shift & BTA_HH_KB_ALT_MASK) {
        APPL_TRACE(ERROR, "Alt key pressed");

        p_kb->alt = TRUE;
    } else if (p_kb->alt) {
        APPL_TRACE(ERROR, "Alt key not pressed");

        p_kb->alt = FALSE;
    }

    p_data->upper = p_kb->upper;
    p_data->ctrl = p_kb->ctrl;
    p_data->alt = p_kb->alt;

    for (xx = 0; xx < report_len; ++xx) {
        for (yy = 0; yy < BTA_HH_MAX_RPT_CHARS; ++yy) {
            if (this_report[xx] == p_kb->last_report[yy])
                this_report[xx] = 0;
        }
    }

    for (xx = 0; xx < report_len; ++xx) {
        APPL_TRACE(DEBUG, "this_char = %02x", this_report[xx]);

        this_char = this_report[xx];

        /* TODO: The constants here are USB HID key codes. Clean this up with
         * named constants.
         */

        if (this_char == 0x00)
            continue;

        if (this_char == BTA_HH_KB_CAPS_LOCK) {
            p_kb->caps_lock = p_kb->caps_lock ? FALSE : TRUE;
            continue;
        }

        /* Explicitly not compound addition operators for these next three */
        if (0x04 <= this_char && this_char <= 0x1d) {
            this_char = this_char + 0x3d;
        } else if (0x1e <= this_char && this_char <= 0x26) {
            this_char = this_char + 0x13;
        } else if (0x3a <= this_char && this_char <= 0x45) {
            this_char = this_char + 0x36;
        } else if (0x27 <= this_char && this_char <= 0x38) {
            this_char = vir_key_tbl_1[this_char - 0x27];
        } else if (this_char == 0x5c) {
            this_char = 0xbc;
        } else if (0x49 <= this_char && this_char <= 0x63) {
            this_char = vir_key_tbl_2[this_char - 0x49];

            if (this_char == 0xff) {
                p_kb->num_lock = p_kb->num_lock ? FALSE : TRUE;
                continue;
            }

            if (p_kb->num_lock) {
                if (this_char == 0x62)
                    this_char = 0x30;
                else if (this_char == 0x63)
                    this_char = 0xbe;
                else
                    this_char = this_char - 0x28;
            }
        } else {
            // NOTE: Do not touch
            APPL_TRACE(ERROR, "BTA_HhParseKeybdRpt:  Cannot interpret scan code \
                0x%02x",
                       this_char);

            continue;
        }

        p_data->caps_lock = p_kb->caps_lock;
        p_data->this_char = this_char;
    }

    memset(p_kb->last_report, 0, BTA_HH_MAX_RPT_CHARS);
    memcpy(p_kb->last_report, p_report, report_len);
}

void bta_hh_parse_mice_rpt(tBTA_HH_BOOT_RPT* p_mice_data, UINT8* p_report, UINT16 report_len) {
    tBTA_HH_MICE_RPT* p_data = &p_mice_data->data_rpt.mice_rpt;
    UINT8 xx;

    // NOTE: Do not touch
    APPL_TRACE(DEBUG, "bta_hh_parse_mice_rpt:  bta_keybd_rpt_rcvd(report=%p, \
                report_len=%d) called",
               p_report, report_len);

    if (report_len < 3)
        return;

    if (report_len > BTA_HH_MAX_RPT_CHARS)
        report_len = BTA_HH_MAX_RPT_CHARS;

    for (xx = 0; xx < report_len; ++xx)
        APPL_TRACE(DEBUG, "this_char = %02x", p_report[xx]);

    p_data->mouse_button = p_report[0] & 0x07;
    p_data->delta_x = p_report[1];
    p_data->delta_y = p_report[2];

    APPL_TRACE(DEBUG, "mice button: 0x%2x", p_data->mouse_button);
    APPL_TRACE(DEBUG, "mice move: x = %d y = %d", p_data->delta_x, p_data->delta_y);
}

void bta_hh_trace_dev_db(void) {
    UINT8 xx;

    APPL_TRACE(DEBUG, "bta_hh_trace_dev_db:: Device DB list********************");

    for (xx = 0; xx < BTA_HH_MAX_KNOWN; ++xx) {
        APPL_TRACE(DEBUG, "kdev[%d] in_use[%d]  handle[%d] ", xx, bta_hh_cb.kdev[xx].in_use, bta_hh_cb.kdev[xx].hid_handle);

        APPL_TRACE(DEBUG, "\t\t\t attr_mask[%04x] state [%d] sub_class[%02x] index = %d", bta_hh_cb.kdev[xx].attr_mask, bta_hh_cb.kdev[xx].state,
                   bta_hh_cb.kdev[xx].sub_class, bta_hh_cb.kdev[xx].index);
    }

    APPL_TRACE(DEBUG, "*********************************************************");
}
