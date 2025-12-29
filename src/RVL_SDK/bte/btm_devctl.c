#include "btm_api.h"

/* Original source:
 * bluedroid <android.googlesource.com/platform/external/bluetooth/bluedroid>
 * stack/btm/btm_devctl.c
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
#include "l2c_int.h"

/*******************************************************************************
 * macros
 */

#ifndef BTM_INIT_CLASS_OF_DEVICE
#define BTM_INIT_CLASS_OF_DEVICE "\x00\x1f\x00"
#endif

#ifndef BTM_DEV_RESET_TIMEOUT
#define BTM_DEV_RESET_TIMEOUT 4
#endif

#define BTM_DEV_REPLY_TIMEOUT 1
#define BTM_INFO_TIMEOUT 5

#define BTM_SCO_HOST_BUF_SIZE 30

#ifndef BTM_SCO_HOST_BUF_SIZE
#define BTM_SCO_HOST_BUF_SIZE 255
#endif

/*******************************************************************************
 * local function declarations
 */

static void btm_db_reset(void);

static void btm_dev_reset(void);

static void btm_continue_reset(void);
static void btm_after_reset_hold_complete(void);

/*******************************************************************************
 * functions
 */

void btm_dev_init(void) {
    memcpy(btm_cb.devcb.dev_class, (UINT8*)BTM_INIT_CLASS_OF_DEVICE, DEV_CLASS_LEN);

    // TODO: constant?
    btm_cb.devcb.page_timeout = 0x1400;

    btm_cb.devcb.reset_timer.param = (TIMER_PARAM_TYPE)TT_DEV_RESET;
    btm_cb.devcb.rln_timer.param = (TIMER_PARAM_TYPE)TT_DEV_RLN;
    btm_cb.devcb.rlinkp_timer.param = (TIMER_PARAM_TYPE)TT_DEV_RLNKP;

    btm_cb.btm_acl_pkt_types_supported = BTM_ACL_PKT_TYPES_MASK_DH1 | BTM_ACL_PKT_TYPES_MASK_DM1 | BTM_ACL_PKT_TYPES_MASK_DH3 |
                                         BTM_ACL_PKT_TYPES_MASK_DM3 | BTM_ACL_PKT_TYPES_MASK_DH5 | BTM_ACL_PKT_TYPES_MASK_DM5;

    btm_cb.btm_sco_pkt_types_supported = BTM_SCO_PKT_TYPES_MASK_HV1 | BTM_SCO_PKT_TYPES_MASK_HV2 | BTM_SCO_PKT_TYPES_MASK_HV3 |
                                         BTM_SCO_PKT_TYPES_MASK_EV3 | BTM_SCO_PKT_TYPES_MASK_EV4 | BTM_SCO_PKT_TYPES_MASK_EV5;

    btm_cb.first_disabled_channel = BTM_AFH_NO_CHANNEL;
    btm_cb.last_disabled_channel = BTM_AFH_NO_CHANNEL;

    btm_dev_reset();
}

static void btm_db_reset(void) {
    tBTM_CMPL_CB* p_cb;
    tBTM_STATUS status = BTM_DEV_RESET;

    btm_inq_db_reset();
    btm_discovery_db_reset();

    if (btm_cb.devcb.p_rln_cmpl_cb) {
        p_cb = btm_cb.devcb.p_rln_cmpl_cb;
        btm_cb.devcb.p_rln_cmpl_cb = NULL;

        if (p_cb)
            (*p_cb)(NULL);
    }

    if (btm_cb.devcb.p_rlinkp_cmpl_cb) {
        p_cb = btm_cb.devcb.p_rlinkp_cmpl_cb;
        btm_cb.devcb.p_rlinkp_cmpl_cb = NULL;

        if (p_cb)
            (*p_cb)(&status);
    }

    if (btm_cb.devcb.p_rssi_cmpl_cb) {
        p_cb = btm_cb.devcb.p_rssi_cmpl_cb;
        btm_cb.devcb.p_rssi_cmpl_cb = NULL;

        if (p_cb)
            (*p_cb)(&status);
    }
}

void btm_dev_absent(void) {
    btm_cb.devcb.state = BTM_DEV_STATE_WAIT_RESET_CMPLT;

    btm_db_reset();
    btm_inq_db_reset();

    btm_report_device_status(BTM_DEV_STATUS_DOWN);

    btu_stop_timer(&btm_cb.devcb.reset_timer);
}

void BTM_DeviceReset(tBTM_CMPL_CB* p_cb) {
    if (!btm_cb.devcb.p_reset_cmpl_cb || btm_cb.devcb.p_reset_cmpl_cb == p_cb) {
        btm_acl_device_down();

        btm_db_reset();

        btm_cb.devcb.p_reset_cmpl_cb = p_cb;

        btm_dev_reset();

        btm_report_device_status(BTM_DEV_STATUS_DOWN);
    }
}

void BTM_SendHciReset(tBTM_CMPL_CB* p_cb) {
    if (!btm_cb.devcb.p_reset_cmpl_cb || btm_cb.devcb.p_reset_cmpl_cb == p_cb || btm_cb.devcb.p_reset_only_cmpl_cb == p_cb) {
        btm_acl_device_down();

        btm_db_reset();

        btm_cb.devcb.p_reset_only_cmpl_cb = p_cb;

        btm_dev_reset();

        btm_report_device_status(BTM_DEV_STATUS_DOWN);
    }
}

BOOLEAN BTM_IsDeviceUp(void) {
    return btm_cb.devcb.state == BTM_DEV_STATE_READY;
}

tBTM_STATUS BTM_SetAfhChannels(tBTM_AFH_CHANNEL first, tBTM_AFH_CHANNEL last) {
    BTM_TRACE(API, "BTM_SetAfhChannels first: %d (%d) last: %d (%d)", first, btm_cb.first_disabled_channel, last, btm_cb.last_disabled_channel);

    if (!HCI_LMP_AFH_CAP_MASTER_SUPPORTED(btm_cb.devcb.local_features) && !HCI_LMP_AFH_CLASS_SLAVE_SUPPORTED(btm_cb.devcb.local_features) &&
        !HCI_LMP_AFH_CLASS_MASTER_SUPPORTED(btm_cb.devcb.local_features)) {
        return BTM_MODE_UNSUPPORTED;
    }

    if (btm_cb.first_disabled_channel != first || btm_cb.last_disabled_channel != last) {
        if (btsnd_hcic_set_afh_channels(first, last)) {
            btm_cb.first_disabled_channel = first;
            btm_cb.last_disabled_channel = last;
        } else {
            return BTM_NO_RESOURCES;
        }
    }

    return BTM_SUCCESS;
}

tBTM_STATUS BTM_SetAfhChannelAssessment(BOOLEAN enable_or_disable) {
    if (!HCI_LMP_AFH_CAP_SLAVE_SUPPORTED(btm_cb.devcb.local_features))
        return BTM_MODE_UNSUPPORTED;

    if (!btsnd_hcic_write_afh_channel_assessment_mode(enable_or_disable))
        return BTM_NO_RESOURCES;

    return BTM_SUCCESS;
}

void BTM_ContinueReset(void) {
    /* ... */
}

static void btm_dev_reset(void) {
    btm_cb.devcb.state = BTM_DEV_STATE_WAIT_RESET_CMPLT;
    btm_cb.devcb.retry_count = 5;

    btu_start_timer(&btm_cb.devcb.reset_timer, BTU_TTYPE_BTM_DEV_CTL, BTM_DEV_RESET_TIMEOUT);

    btsnd_hcic_reset();
}

void btm_get_hci_buf_size(void) {
    void* p_buf;

    if (--btm_cb.devcb.retry_count == 0) {
        btm_dev_reset();
    } else {
        btu_start_timer(&btm_cb.devcb.reset_timer, BTU_TTYPE_BTM_DEV_CTL, BTM_DEV_REPLY_TIMEOUT);

        if ((p_buf = HCI_GET_CMD_BUF(HCIC_PARAM_SIZE_READ_CMD)) != NULL)
            btsnd_hcic_read_buffer_size(p_buf);
    }
}

void btm_get_local_version(void) {
    if (--btm_cb.devcb.retry_count == 0) {
        btm_dev_reset();
    } else {
        btu_start_timer(&btm_cb.devcb.reset_timer, BTU_TTYPE_BTM_DEV_CTL, BTM_DEV_REPLY_TIMEOUT);

        btsnd_hcic_read_local_ver();
        btsnd_hcic_read_bd_addr();

        btm_pm_reset();
    }
}

void btm_get_local_features(void) {
    if (--btm_cb.devcb.retry_count == 0) {
        btm_dev_reset();
    } else {
        btu_start_timer(&btm_cb.devcb.reset_timer, BTU_TTYPE_BTM_DEV_CTL, BTM_DEV_REPLY_TIMEOUT);

        btsnd_hcic_read_local_features();
    }
}

void btm_dev_timeout(TIMER_LIST_ENT* p_tle) {
    TIMER_PARAM_TYPE timer_type = p_tle->param;

    if (timer_type == (TIMER_PARAM_TYPE)TT_DEV_RESET) {
        switch (btm_cb.devcb.state) {
        case BTM_DEV_STATE_WAIT_RESET_CMPLT:
            btm_dev_reset();
            break;

        case BTM_DEV_STATE_WAIT_AFTER_RESET:
            btm_after_reset_hold_complete();
            break;

        case BTM_DEV_STATE_2:
            btm_get_hci_buf_size();
            break;

        case BTM_DEV_STATE_3:
            btm_get_local_version();
            break;
        }
    } else if (timer_type == (TIMER_PARAM_TYPE)TT_DEV_RLN) {
        tBTM_CMPL_CB* p_cb = btm_cb.devcb.p_rln_cmpl_cb;

        btm_cb.devcb.p_rln_cmpl_cb = NULL;

        if (p_cb)
            (*p_cb)(NULL);
    }
}

void btm_reset_complete(void) {
    BTM_TRACE(EVENT, "btm_reset_complete");

    if (btm_cb.devcb.p_reset_only_cmpl_cb) {
        tBTM_CMPL_CB* p_cb = btm_cb.devcb.p_reset_only_cmpl_cb;

        btm_cb.devcb.p_reset_only_cmpl_cb = NULL;

        (*p_cb)(NULL);
    } else if (btm_cb.devcb.state == BTM_DEV_STATE_WAIT_RESET_CMPLT) {
        l2cu_device_reset();

        btm_cb.btm_inq_vars.inq_scan_window = HCI_DEF_INQUIRYSCAN_WINDOW;
        btm_cb.btm_inq_vars.inq_scan_period = HCI_DEF_INQUIRYSCAN_INTERVAL;
        btm_cb.btm_inq_vars.inq_scan_type = HCI_DEF_SCAN_TYPE;

        btm_cb.btm_inq_vars.page_scan_window = HCI_DEF_PAGESCAN_WINDOW;
        btm_cb.btm_inq_vars.page_scan_period = HCI_DEF_PAGESCAN_INTERVAL;
        btm_cb.btm_inq_vars.page_scan_type = HCI_DEF_SCAN_TYPE;

        btm_after_reset_hold_complete();
    }
}

static void btm_continue_reset(void) {
    btm_cb.devcb.state = BTM_DEV_STATE_2;
    btm_cb.devcb.retry_count = 5;

    btm_get_hci_buf_size();

    BTM_SetDeviceClass(btm_cb.devcb.dev_class);
    BTM_SetLocalDeviceName(btm_cb.cfg.bd_name);
    BTM_SetPinType(btm_cb.cfg.pin_type, btm_cb.cfg.pin_code, btm_cb.cfg.pin_code_len);
}

static void btm_after_reset_hold_complete(void) {
    btm_continue_reset();
}

void btm_read_hci_buf_size_complete(UINT8* p, UINT16 evt_len) {
    UINT8 status;
    UINT8 lm_sco_buf_size;
    UINT16 lm_num_acl_bufs;
    UINT16 lm_num_sco_bufs;
    UINT16 acl_buf_size;

    STREAM_TO_UINT8(p, &status);

    if (status == HCI_SUCCESS) {
        STREAM_TO_UINT16(p, &btu_cb.hcit_acl_data_size);
        STREAM_TO_UINT8(p, &lm_sco_buf_size);
        STREAM_TO_UINT16(p, &lm_num_acl_bufs);
        STREAM_TO_UINT16(p, &lm_num_sco_bufs);

        btu_cb.hcit_acl_pkt_size = btu_cb.hcit_acl_data_size + HCI_DATA_PREAMBLE_SIZE;

        l2c_link_processs_num_bufs(lm_num_acl_bufs);

        acl_buf_size = L2CAP_MTU_SIZE;

        btsnd_hcic_set_host_buf_size(acl_buf_size, BTM_SCO_HOST_BUF_SIZE, L2CAP_HOST_FC_ACL_BUFS, 10);
    }

    btm_cb.devcb.state = BTM_DEV_STATE_3;
    btm_cb.devcb.retry_count = 5;

    btm_get_local_version();
}

void btm_read_local_version_complete(UINT8* p, UINT16 evt_len) {
    tBTM_VERSION_INFO* p_vi = &btm_cb.devcb.local_version;
    UINT8 status;

    STREAM_TO_UINT8(p, &status);

    if (status == HCI_SUCCESS) {
        STREAM_TO_UINT8(p, &p_vi->hci_version);
        STREAM_TO_UINT16(p, &p_vi->hci_revision);
        STREAM_TO_UINT8(p, &p_vi->lmp_version);
        STREAM_TO_UINT16(p, &p_vi->manufacturer);
        STREAM_TO_UINT16(p, &p_vi->lmp_subversion);
    }

    btm_cb.devcb.state = BTM_DEV_STATE_4;
    btm_cb.devcb.retry_count = 5;

    btm_get_local_features();
}

void btm_read_local_features_complete(UINT8* p, UINT16 evt_len) {
    tBTM_DEVCB* p_devcb = &btm_cb.devcb;
    tBTM_CMPL_CB* p_cb = p_devcb->p_reset_cmpl_cb;
    UINT8 status;
    UINT16 xx;
    UINT8 last;
    UINT8 first;

    p_devcb->p_reset_cmpl_cb = NULL;

    STREAM_TO_UINT8(p, &status);

    if (status != HCI_SUCCESS)
        return;

    p_devcb->state = BTM_DEV_STATE_READY;

    for (xx = 0; xx < HCI_NUM_FEATURE_BYTES; ++xx)
        STREAM_TO_UINT8(p, &p_devcb->local_features[xx]);

    btm_cb.btm_acl_pkt_types_supported = BTM_ACL_PKT_TYPES_MASK_DH1 | BTM_ACL_PKT_TYPES_MASK_DM1;

    if (HCI_3_SLOT_PACKETS_SUPPORTED(p_devcb->local_features)) {
        btm_cb.btm_acl_pkt_types_supported |= BTM_ACL_PKT_TYPES_MASK_DH3 | BTM_ACL_PKT_TYPES_MASK_DM3;
    }

    if (HCI_5_SLOT_PACKETS_SUPPORTED(p_devcb->local_features)) {
        btm_cb.btm_acl_pkt_types_supported |= BTM_ACL_PKT_TYPES_MASK_DH5 | BTM_ACL_PKT_TYPES_MASK_DM5;
    }

    if (btm_cb.devcb.local_version.hci_version >= HCI_PROTO_VERSION_2_0) {
        if (!HCI_EDR_ACL_2MPS_SUPPORTED(p_devcb->local_features)) {
            btm_cb.btm_acl_pkt_types_supported |= BTM_ACL_PKT_TYPES_MASK_NO_2_DH1 | BTM_ACL_PKT_TYPES_MASK_NO_2_DH3 | BTM_ACL_PKT_TYPES_MASK_NO_2_DH5;
        }

        if (!HCI_EDR_ACL_3MPS_SUPPORTED(p_devcb->local_features)) {
            btm_cb.btm_acl_pkt_types_supported |= BTM_ACL_PKT_TYPES_MASK_NO_3_DH1 | BTM_ACL_PKT_TYPES_MASK_NO_3_DH3 | BTM_ACL_PKT_TYPES_MASK_NO_3_DH5;
        }

        if (HCI_EDR_ACL_2MPS_SUPPORTED(p_devcb->local_features) || HCI_EDR_ACL_3MPS_SUPPORTED(p_devcb->local_features)) {
            if (!HCI_3_SLOT_EDR_ACL_SUPPORTED(p_devcb->local_features)) {
                btm_cb.btm_acl_pkt_types_supported |= BTM_ACL_PKT_TYPES_MASK_NO_2_DH3 | BTM_ACL_PKT_TYPES_MASK_NO_3_DH3;
            }

            if (!HCI_5_SLOT_EDR_ACL_SUPPORTED(p_devcb->local_features)) {
                btm_cb.btm_acl_pkt_types_supported |= BTM_ACL_PKT_TYPES_MASK_NO_2_DH5 | BTM_ACL_PKT_TYPES_MASK_NO_3_DH5;
            }
        }
    }

    BTM_TRACE(DEBUG, "Local supported ACL packet types: 0x%04x", btm_cb.btm_acl_pkt_types_supported);

    btm_cb.btm_sco_pkt_types_supported = 0;
    btm_cb.sco_cb.esco_supported = FALSE;

    if (HCI_SCO_LINK_SUPPORTED(p_devcb->local_features)) {
        btm_cb.btm_sco_pkt_types_supported = BTM_SCO_PKT_TYPES_MASK_HV1;

        if (HCI_HV2_PACKETS_SUPPORTED(p_devcb->local_features))
            btm_cb.btm_sco_pkt_types_supported |= BTM_SCO_PKT_TYPES_MASK_HV2;

        if (HCI_HV3_PACKETS_SUPPORTED(p_devcb->local_features))
            btm_cb.btm_sco_pkt_types_supported |= BTM_SCO_PKT_TYPES_MASK_HV3;
    }

    if (HCI_ESCO_EV3_SUPPORTED(p_devcb->local_features))
        btm_cb.btm_sco_pkt_types_supported |= BTM_SCO_PKT_TYPES_MASK_EV3;

    if (HCI_ESCO_EV4_SUPPORTED(p_devcb->local_features))
        btm_cb.btm_sco_pkt_types_supported |= BTM_SCO_PKT_TYPES_MASK_EV4;

    if (HCI_ESCO_EV5_SUPPORTED(p_devcb->local_features))
        btm_cb.btm_sco_pkt_types_supported |= BTM_SCO_PKT_TYPES_MASK_EV5;

    if (btm_cb.btm_sco_pkt_types_supported & BTM_ESCO_LINK_ONLY_MASK) {
        btm_cb.sco_cb.esco_supported = TRUE;

        if (HCI_EDR_ESCO_2MPS_SUPPORTED(p_devcb->local_features)) {
            if (!HCI_3_SLOT_EDR_ESCO_SUPPORTED(p_devcb->local_features)) {
                btm_cb.btm_sco_pkt_types_supported |= BTM_SCO_PKT_TYPES_MASK_NO_2_EV5;
            }
        } else {
            btm_cb.btm_sco_pkt_types_supported |= BTM_SCO_PKT_TYPES_MASK_NO_2_EV3 | BTM_SCO_PKT_TYPES_MASK_NO_2_EV5;
        }

        if (HCI_EDR_ESCO_3MPS_SUPPORTED(p_devcb->local_features)) {
            if (!HCI_3_SLOT_EDR_ESCO_SUPPORTED(p_devcb->local_features)) {
                btm_cb.btm_sco_pkt_types_supported |= BTM_SCO_PKT_TYPES_MASK_NO_3_EV5;
            }
        } else {
            btm_cb.btm_sco_pkt_types_supported |= BTM_SCO_PKT_TYPES_MASK_NO_3_EV3 | BTM_SCO_PKT_TYPES_MASK_NO_3_EV5;
        }
    }

    BTM_TRACE(DEBUG, "Local supported SCO packet types: 0x%04x", btm_cb.btm_sco_pkt_types_supported);

    if (HCI_SWITCH_SUPPORTED(p_devcb->local_features))
        btm_cb.btm_def_link_policy |= HCI_ENABLE_MASTER_SLAVE_SWITCH;
    else
        btm_cb.btm_def_link_policy &= ~HCI_ENABLE_MASTER_SLAVE_SWITCH;

    if (HCI_HOLD_MODE_SUPPORTED(p_devcb->local_features))
        btm_cb.btm_def_link_policy |= HCI_ENABLE_HOLD_MODE;
    else
        btm_cb.btm_def_link_policy &= ~HCI_ENABLE_HOLD_MODE;

    if (HCI_SNIFF_MODE_SUPPORTED(p_devcb->local_features))
        btm_cb.btm_def_link_policy |= HCI_ENABLE_SNIFF_MODE;
    else
        btm_cb.btm_def_link_policy &= ~HCI_ENABLE_SNIFF_MODE;

    if (HCI_PARK_MODE_SUPPORTED(p_devcb->local_features))
        btm_cb.btm_def_link_policy |= HCI_ENABLE_PARK_MODE;
    else
        btm_cb.btm_def_link_policy &= ~HCI_ENABLE_PARK_MODE;

    btm_sec_dev_reset();

    if (btm_cb.last_disabled_channel != BTM_AFH_NO_CHANNEL) {
        last = btm_cb.last_disabled_channel;
        first = btm_cb.first_disabled_channel;
        btm_cb.last_disabled_channel = BTM_AFH_NO_CHANNEL;
        btm_cb.first_disabled_channel = BTM_AFH_NO_CHANNEL;
        BTM_SetAfhChannels(first, last);
    }

    if (HCI_LMP_INQ_RSSI_SUPPORTED(p_devcb->local_features))
        BTM_SetInquiryMode(BTM_INQ_RESULT_WITH_RSSI);

    BTM_SetPageScanType(BTM_DEFAULT_SCAN_TYPE);
    BTM_SetInquiryScanType(BTM_DEFAULT_SCAN_TYPE);

    btm_report_device_status(BTM_DEV_STATUS_UP);

    if (p_cb)
        (*p_cb)(NULL);
}

UINT8 btm_get_voice_coding_support(void) {
    // No names for these constants in bluedroid and I don't care

    UINT8 code = 0;

    if (HCI_LMP_CVSD_SUPPORTED(btm_cb.devcb.local_features))
        code |= 1 << 0;

    if (HCI_LMP_A_LAW_SUPPORTED(btm_cb.devcb.local_features))
        code |= 1 << 1;

    if (HCI_LMP_U_LAW_SUPPORTED(btm_cb.devcb.local_features))
        code |= 1 << 2;

    return code;
}

tBTM_STATUS BTM_SetLocalDeviceName(char* p_name) {
    void* p_buf;
    UINT8* p;

    if (!p_name)
        return BTM_ILLEGAL_VALUE;

    if (btm_cb.devcb.state == BTM_DEV_STATE_WAIT_RESET_CMPLT || btm_cb.devcb.state == BTM_DEV_STATE_WAIT_AFTER_RESET) {
        return BTM_DEV_RESET;
    }

    if ((p_buf = HCI_GET_CMD_BUF(HCIC_PARAM_SIZE_CHANGE_NAME)) != NULL) {
        p = (UINT8*)btm_cb.cfg.bd_name;
        if (p != (UINT8*)p_name) {
            memset(btm_cb.cfg.bd_name, 0, BTM_MAX_LOC_BD_NAME_LEN + 1);
            BCM_STRNCPY_S(btm_cb.cfg.bd_name, BTM_MAX_LOC_BD_NAME_LEN + 1, p_name, BTM_MAX_LOC_BD_NAME_LEN);
        }

        btsnd_hcic_change_name(p_buf, p);
        return BTM_CMD_STARTED;
    } else {
        return BTM_NO_RESOURCES;
    }
}

tBTM_STATUS BTM_ReadLocalDeviceName(tBTM_CMPL_CB* p_cb) {
    void* p_buf;

    if (btm_cb.devcb.p_rln_cmpl_cb)
        return BTM_BUSY;

    // NOTE: HCIC_PARAM_SIZE_READ_NAME is not defined
    if ((p_buf = HCI_GET_CMD_BUF(HCIC_PARAM_SIZE_READ_NAME)) != NULL) {
        btu_start_timer(&btm_cb.devcb.rln_timer, BTU_TTYPE_BTM_DEV_CTL, BTM_INFO_TIMEOUT);

        btm_cb.devcb.p_rln_cmpl_cb = p_cb;

        btsnd_hcic_read_name(p_buf);
        return BTM_CMD_STARTED;
    }

    return BTM_NO_RESOURCES;
}

void btm_read_local_name_complete(UINT8* p, UINT16 evt_len) {
    tBTM_CMPL_CB* p_cb = btm_cb.devcb.p_rln_cmpl_cb;
    UINT8 status;

    btu_stop_timer(&btm_cb.devcb.rln_timer);

    btm_cb.devcb.p_rln_cmpl_cb = NULL;

    if (p_cb) {
        STREAM_TO_UINT8(p, &status);

        if (status == HCI_SUCCESS)
            (*p_cb)(p);
        else
            (*p_cb)(NULL);
    }
}

tBTM_STATUS BTM_ReadLocalDeviceAddr(tBTM_CMPL_CB* p_cb) {
    if (p_cb)
        (*p_cb)(btm_cb.devcb.local_addr);

    return BTM_SUCCESS;
}

void btm_read_local_addr_complete(UINT8* p, UINT16 evt_len) {
    UINT8 status;

    STREAM_TO_UINT8(p, &status);

    if (status == HCI_SUCCESS)
        STREAM_TO_BDADDR(p, btm_cb.devcb.local_addr);
}

tBTM_STATUS BTM_ReadLocalVersion(tBTM_VERSION_INFO* p_vers) {
    if (btm_cb.devcb.state < BTM_DEV_STATE_4)
        return BTM_DEV_RESET;

    *p_vers = btm_cb.devcb.local_version;

    return BTM_SUCCESS;
}

tBTM_STATUS BTM_SetDeviceClass(DEV_CLASS dev_class) {
    void* p_buf;

    memcpy(btm_cb.devcb.dev_class, dev_class, DEV_CLASS_LEN);

    if (btm_cb.devcb.state == BTM_DEV_STATE_WAIT_RESET_CMPLT || btm_cb.devcb.state == BTM_DEV_STATE_WAIT_AFTER_RESET) {
        return BTM_DEV_RESET;
    }

    if ((p_buf = HCI_GET_CMD_BUF(HCIC_PARAM_SIZE_WRITE_PARAM3)) != NULL) {
        btsnd_hcic_write_dev_class(p_buf, dev_class);
        return BTM_SUCCESS;
    }

    return BTM_NO_RESOURCES;
}

DEV_CLASS_PTR BTM_ReadDeviceClass(void) {
    return btm_cb.devcb.dev_class;
}

UINT8* BTM_ReadLocalFeatures(void) {
    return btm_cb.devcb.local_features;
}

tBTM_DEV_STATUS_CB* BTM_RegisterForDeviceStatusNotif(tBTM_DEV_STATUS_CB* p_cb) {
    tBTM_DEV_STATUS_CB* p_prev = btm_cb.devcb.p_dev_status_cb;

    btm_cb.devcb.p_dev_status_cb = p_cb;

    return p_prev;
}

tBTM_STATUS BTM_VendorSpecificCommand(UINT16 opcode, UINT8 param_len, UINT8* p_param_buf, tBTM_CMPL_CB* p_cb) {
    void* p_buf;

    BTM_TRACE(EVENT, "BTM: BTM_VendorSpecificCommand: Opcode: 0x%04X, ParamLen: %i.", opcode, param_len);

    if (btm_cb.devcb.vsc_busy) {
        BTM_TRACE(EVENT, "BTM: Unable to send vendor specific command "
                         "(controller is busy).");

        return BTM_BUSY;
    }

    if ((p_buf = GKI_getbuf(sizeof(BT_HDR) + param_len + HCIC_PREAMBLE_SIZE)) != NULL) {
        btm_cb.devcb.p_vsc_cmpl_cb = p_cb;

        if (p_cb)
            btm_cb.devcb.vsc_busy = TRUE;

        btsnd_hcic_vendor_spec_cmd(p_buf, opcode, param_len, p_param_buf);

        if (p_cb)
            return BTM_CMD_STARTED;
        else
            return BTM_SUCCESS;
    } else {
        return BTM_NO_RESOURCES;
    }
}

void btm_vsc_complete(UINT8* p, UINT16 opcode, UINT16 evt_len) {
    tBTM_CMPL_CB* p_cb = btm_cb.devcb.p_vsc_cmpl_cb;
    tBTM_VSC_CMPL vcs_cplt_params;
    UINT8* p_dest = vcs_cplt_params.param_buf;

    btm_cb.devcb.vsc_busy = FALSE;
    btm_cb.devcb.p_vsc_cmpl_cb = NULL;

    if (p_cb) {
        vcs_cplt_params.opcode = opcode;
        vcs_cplt_params.param_len = evt_len;
        memcpy(p_dest, p, evt_len);

        (*p_cb)(&vcs_cplt_params);
    }
}

tBTM_STATUS BTM_RegisterForVSEvents(tBTM_VS_EVT_CB* p_cb) {
    if (btm_cb.devcb.p_vend_spec_cb && p_cb)
        return BTM_BUSY;

    btm_cb.devcb.p_vend_spec_cb = p_cb;

    return BTM_SUCCESS;
}

void btm_vendor_specific_evt(UINT8* p, UINT8 evt_len) {
    tBTM_DEVCB* p_devcb = &btm_cb.devcb;

    if (p_devcb->local_version.manufacturer != LMP_COMPID_ZEEVO)  // ???
    {
        BTM_TRACE(EVENT, "BTM Event: Received a vendor specific event from controller");
    }

    p_devcb->p_vsc_cmpl_cb = NULL;
    p_devcb->vsc_busy = FALSE;

    if (p_devcb->p_vend_spec_cb)
        (*p_devcb->p_vend_spec_cb)(evt_len, p);
}

tBTM_STATUS BTM_WritePageTimeout(UINT16 timeout) {
    void* p_buf;

    BTM_TRACE(EVENT, "BTM: BTM_WritePageTimeout: Timeout: %d.", timeout);

    btm_cb.devcb.page_timeout = timeout * 625 / 1000;

    if ((p_buf = HCI_GET_CMD_BUF(HCIC_PARAM_SIZE_WRITE_PARAM2)) != NULL) {
        btsnd_hcic_write_page_tout(p_buf, timeout);
        return BTM_SUCCESS;
    } else {
        return BTM_NO_RESOURCES;
    }
}

tBTM_STATUS BTM_WriteVoiceSettings(UINT16 settings) {
    void* p_buf;

    BTM_TRACE(EVENT, "BTM: BTM_WriteVoiceSettings: Settings: 0x%04x.", settings);

    if ((p_buf = HCI_GET_CMD_BUF(HCIC_PARAM_SIZE_WRITE_PARAM2)) != NULL) {
        btsnd_hcic_write_voice_settings(p_buf, settings & 0x03ff);
        return BTM_SUCCESS;
    } else {
        return BTM_NO_RESOURCES;
    }
}

tBTM_STATUS BTM_EnableTestMode(void) {
    void* p_buf;
    UINT8 cond;

    BTM_TRACE(EVENT, "BTM: BTM_EnableTestMode");

    if ((p_buf = HCI_GET_CMD_BUF(2 + sizeof cond)) != NULL) {
        cond = HCI_DO_AUTO_ACCEPT_CONNECT;
        btsnd_hcic_set_event_filter(p_buf, HCI_FILTER_CONNECTION_SETUP, HCI_FILTER_COND_NEW_DEVICE, &cond, sizeof cond);
    } else {
        return BTM_NO_RESOURCES;
    }

    if (BTM_SetConnectability(BTM_CONNECTABLE, BTM_DEFAULT_CONN_WINDOW, BTM_DEFAULT_CONN_INTERVAL) != BTM_SUCCESS) {
        return BTM_NO_RESOURCES;
    }

    if (BTM_SetDiscoverability(BTM_GENERAL_DISCOVERABLE, BTM_DEFAULT_DISC_WINDOW, BTM_DEFAULT_DISC_INTERVAL) != BTM_SUCCESS) {
        return BTM_NO_RESOURCES;
    }

    if ((p_buf = HCI_GET_CMD_BUF(HCIC_PARAM_SIZE_READ_CMD)) != NULL) {
        btsnd_hcic_enable_test_mode(p_buf);
        return BTM_SUCCESS;
    } else {
        return BTM_NO_RESOURCES;
    }
}

UINT8 btm_get_hci_version(void) {
    return btm_cb.devcb.local_version.hci_version;
}

tBTM_STATUS BTM_ReadStoredLinkKey(BD_ADDR bd_addr, tBTM_CMPL_CB* p_cb) {
    void* p_buf;
    BD_ADDR local_bd_addr;
    BOOLEAN read_all_flag = FALSE;

    if (btm_cb.devcb.p_stored_link_key_cmpl_cb)
        return BTM_BUSY;

    if (!bd_addr) {
        read_all_flag = TRUE;

        bd_addr = local_bd_addr;
    }

    BTM_TRACE(EVENT, "BTM: BTM_ReadStoredLinkKey: Read_All: %s", read_all_flag ? "TRUE" : "FALSE");

    if ((p_buf = HCI_GET_CMD_BUF(HCIC_PARAM_SIZE_READ_STORED_KEY)) != NULL) {
        btm_cb.devcb.p_stored_link_key_cmpl_cb = p_cb;

        btsnd_hcic_read_stored_key(p_buf, bd_addr, read_all_flag);
        return BTM_SUCCESS;
    } else {
        return BTM_NO_RESOURCES;
    }
}

tBTM_STATUS BTM_WriteStoredLinkKey(UINT8 num_keys, BD_ADDR* bd_addr, LINK_KEY* link_key, tBTM_CMPL_CB* p_cb) {
    void* p_buf;

    if (btm_cb.devcb.p_stored_link_key_cmpl_cb)
        return BTM_BUSY;

    BTM_TRACE(EVENT, "BTM: BTM_WriteStoredLinkKey: num_keys: %d", num_keys);

    if (num_keys > HCI_MAX_NUM_OF_LINK_KEYS_PER_CMD)
        num_keys = HCI_MAX_NUM_OF_LINK_KEYS_PER_CMD;

    if ((p_buf = HCI_GET_CMD_BUF(1 + num_keys * (BD_ADDR_LEN + LINK_KEY_LEN))) != NULL) {
        btm_cb.devcb.p_stored_link_key_cmpl_cb = p_cb;
        btsnd_hcic_write_stored_key(p_buf, num_keys, bd_addr, link_key);

        return BTM_SUCCESS;
    } else {
        return BTM_NO_RESOURCES;
    }
}

tBTM_STATUS BTM_DeleteStoredLinkKey(BD_ADDR bd_addr, tBTM_CMPL_CB* p_cb) {
    BD_ADDR local_bd_addr;
    BOOLEAN delete_all_flag = FALSE;

    if (btm_cb.devcb.p_stored_link_key_cmpl_cb)
        return BTM_BUSY;

    if (!bd_addr) {
        delete_all_flag = TRUE;

        bd_addr = local_bd_addr;
    }

    BTM_TRACE(EVENT, "BTM: BTM_DeleteStoredLinkKey: delete_all_flag: %s", delete_all_flag ? "TRUE" : "FALSE");

    btm_cb.devcb.p_stored_link_key_cmpl_cb = p_cb;

    if (!btsnd_hcic_delete_stored_key(bd_addr, delete_all_flag))
        return BTM_NO_RESOURCES;
    else
        return BTM_SUCCESS;
}

void btm_read_stored_link_key_complete(UINT8* p) {
    tBTM_CMPL_CB* p_cb = btm_cb.devcb.p_stored_link_key_cmpl_cb;
    tBTM_READ_STORED_LINK_KEY_COMPLETE result;

    btm_cb.devcb.p_stored_link_key_cmpl_cb = NULL;

    if (p_cb) {
        result.event = BTM_CB_EVT_READ_STORED_LINK_KEYS;

        STREAM_TO_UINT8(p, &result.status);
        STREAM_TO_UINT16(p, &result.max_keys);
        STREAM_TO_UINT16(p, &result.read_keys);

        (*p_cb)(&result);
    }
}

void btm_write_stored_link_key_complete(UINT8* p) {
    tBTM_CMPL_CB* p_cb = btm_cb.devcb.p_stored_link_key_cmpl_cb;
    tBTM_WRITE_STORED_LINK_KEY_COMPLETE result;

    btm_cb.devcb.p_stored_link_key_cmpl_cb = NULL;

    if (p_cb) {
        result.event = BTM_CB_EVT_WRITE_STORED_LINK_KEYS;

        STREAM_TO_UINT8(p, &result.status);
        STREAM_TO_UINT8(p, &result.num_keys);

        (*p_cb)(&result);
    }
}

void btm_delete_stored_link_key_complete(UINT8* p) {
    tBTM_CMPL_CB* p_cb = btm_cb.devcb.p_stored_link_key_cmpl_cb;
    tBTM_DELETE_STORED_LINK_KEY_COMPLETE result;

    btm_cb.devcb.p_stored_link_key_cmpl_cb = NULL;

    if (p_cb) {
        result.event = BTM_CB_EVT_DELETE_STORED_LINK_KEYS;

        STREAM_TO_UINT8(p, &result.status);
        STREAM_TO_UINT16(p, &result.num_keys);

        (*p_cb)(&result);
    }
}

void btm_return_link_keys_evt(tBTM_RETURN_LINK_KEYS_EVT* result) {
    tBTM_CMPL_CB* p_cb = btm_cb.devcb.p_stored_link_key_cmpl_cb;
    UINT8 i;
    UINT8* p;
    UINT8* p1;
    UINT8 bd_addr[BD_ADDR_LEN];
    UINT8 link_key[LINK_KEY_LEN];

    if (p_cb) {
        p = (UINT8*)(result + 1);

        for (i = 0; i < result->num_keys; ++i) {
            p1 = p;

            REVERSE_STREAM_TO_ARRAY(p1, bd_addr, BD_ADDR_LEN);
            REVERSE_STREAM_TO_ARRAY(p1, link_key, LINK_KEY_LEN);

            ARRAY_TO_STREAM(p, bd_addr, BD_ADDR_LEN);
            ARRAY_TO_STREAM(p, link_key, LINK_KEY_LEN);
        }

        (*p_cb)(result);
    }
}

void btm_report_device_status(tBTM_DEV_STATUS status) {
    tBTM_DEV_STATUS_CB* p_cb = btm_cb.devcb.p_dev_status_cb;

    if (p_cb)
        (*p_cb)(status);
}
