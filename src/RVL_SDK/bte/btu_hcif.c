#include "btu.h"

/* Original source:
 * bluedroid <android.googlesource.com/platform/external/bluetooth/bluedroid>
 * stack/btu/btu_hcif.c
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

#include "bt_target.h"
#include "bt_trace.h"
#include "bt_types.h"
#include "data_types.h"
#include "revolution/types.h"
#include <cstring>

#include "btm_api.h"
#include "btm_int.h"
#include "gki.h"
#include "hcidefs.h"
#include "hcimsgs.h"
#include "hidd_int.h"
#include "l2c_int.h"

/*******************************************************************************
 * local function declarations
 */

static void btu_hcif_store_cmd(BT_HDR* p_buf);

static void btu_hcif_inquiry_comp_evt(UINT8* p, UINT16 evt_len);
static void btu_hcif_inquiry_result_evt(UINT8* p, UINT16 evt_len);
static void btu_hcif_inquiry_rssi_result_evt(UINT8* p, UINT16 evt_len);
static void btu_hcif_connection_comp_evt(UINT8* p, UINT16 evt_len);
static void btu_hcif_connection_request_evt(UINT8* p, UINT16 evt_len);
static void btu_hcif_disconnection_comp_evt(UINT8* p, UINT16 evt_len);
static void btu_hcif_authentication_comp_evt(UINT8* p, UINT16 evt_len);
static void btu_hcif_rmt_name_request_comp_evt(UINT8* p, UINT16 evt_len);
static void btu_hcif_encryption_change_evt(UINT8* p, UINT16 evt_len);
static void btu_hcif_change_conn_link_key_evt(UINT8* p, UINT16 evt_len);
static void btu_hcif_master_link_key_comp_evt(UINT8* p, UINT16 evt_len);
static void btu_hcif_read_rmt_features_comp_evt(UINT8* p, UINT16 evt_len);
static void btu_hcif_read_rmt_version_comp_evt(UINT8* p, UINT16 evt_len);
static void btu_hcif_qos_setup_comp_evt(UINT8* p, UINT16 evt_len);
static void btu_hcif_esco_connection_comp_evt(UINT8* p, UINT16 evt_len);
static void btu_hcif_esco_connection_chg_evt(UINT8* p, UINT16 evt_len);
static void btu_hcif_hdl_command_complete(UINT16 opcode, UINT8* p, UINT16 evt_len);
static void btu_hcif_command_complete_evt(UINT8* p, UINT16 evt_len);
static void btu_hcif_hdl_command_status(UINT16 opcode, UINT8 status, UINT8* p_cmd);
static void btu_hcif_command_status_evt(UINT8* p, UINT16 evt_len);

static void btu_hcif_hardware_error_evt(UINT8* p, UINT16 evt_len);
static void btu_hcif_flush_occured_evt(UINT8* p, UINT16 evt_len);
static void btu_hcif_role_change_evt(UINT8* p, UINT16 evt_len);
static void btu_hcif_num_compl_data_pkts_evt(UINT8* p, UINT16 evt_len);
static void btu_hcif_mode_change_evt(UINT8* p, UINT16 evt_len);
static void btu_hcif_return_link_keys_evt(UINT8* p, UINT16 evt_len);
static void btu_hcif_pin_code_request_evt(UINT8* p, UINT16 evt_len);
static void btu_hcif_link_key_request_evt(UINT8* p, UINT16 evt_len);
static void btu_hcif_link_key_notification_evt(UINT8* p, UINT16 evt_len);
static void btu_hcif_loopback_command_evt(UINT8* p, UINT16 evt_len);
static void btu_hcif_data_buf_overflow_evt(UINT8* p, UINT16 evt_len);
static void btu_hcif_max_slots_changed_evt(UINT8* p, UINT16 evt_len);
static void btu_hcif_read_clock_off_comp_evt(UINT8* p, UINT16 evt_len);
static void btu_hcif_conn_pkt_type_change_evt(UINT8* p, UINT16 evt_len);
static void btu_hcif_qos_violation_evt(UINT8* p, UINT16 evt_len);
static void btu_hcif_page_scan_mode_change_evt(UINT8* p, UINT16 evt_len);
static void btu_hcif_page_scan_rep_mode_chng_evt(UINT8* p, UINT16 evt_len);

/*******************************************************************************
 * functions
 */

static void btu_hcif_store_cmd(BT_HDR* p_buf) {
    UINT16 opcode;
    BT_HDR* p_cmd;
    UINT8* p = (UINT8*)(p_buf + 1) + p_buf->offset;

    STREAM_TO_UINT16(p, &opcode);

    if (opcode == HCI_RESET || opcode == HCI_HOST_NUM_PACKETS_DONE)
        return;

    if ((p_cmd = HCI_GET_CMD_BUF(p_buf->len + p_buf->offset - HCIC_PREAMBLE_SIZE)) == NULL) {
        return;
    }

    memcpy(p_cmd, p_buf, sizeof *p_cmd);

    memcpy((UINT8*)(p_cmd + 1) + p_cmd->offset, (UINT8*)(p_buf + 1) + p_buf->offset, p_buf->len);

    GKI_enqueue(&btu_cb.cmd_cmpl_q, p_cmd);
    btu_start_timer(&btu_cb.cmd_cmpl_timer, BTU_TTYPE_BTU_CMD_CMPL, BTU_CMD_CMPL_TIMEOUT);
}

void btu_hcif_process_event(BT_HDR* p_msg) {
    UINT8* p = (UINT8*)(p_msg + 1) + p_msg->offset;
    UINT8 hci_evt_code;
    UINT8 hci_evt_len;
    UINT8* p_cc;

    STREAM_TO_UINT8(p, &hci_evt_code);
    STREAM_TO_UINT8(p, &hci_evt_len);

    if (btm_cb.devcb.state == 0) {
        p_cc = p;

        if (hci_evt_code != 14 && hci_evt_code != 15)
            return;
    }

    switch (hci_evt_code) {
    case HCI_INQUIRY_COMP_EVT:
        btu_hcif_inquiry_comp_evt(p, hci_evt_len);
        break;

    case HCI_INQUIRY_RESULT_EVT:
        btu_hcif_inquiry_result_evt(p, hci_evt_len);
        break;

    case HCI_INQUIRY_RSSI_RESULT_EVT:
        btu_hcif_inquiry_rssi_result_evt(p, hci_evt_len);
        break;

    case HCI_CONNECTION_COMP_EVT:
        btu_hcif_connection_comp_evt(p, hci_evt_len);
        break;

    case HCI_CONNECTION_REQUEST_EVT:
        btu_hcif_connection_request_evt(p, hci_evt_len);
        break;

    case HCI_DISCONNECTION_COMP_EVT:
        btu_hcif_disconnection_comp_evt(p, hci_evt_len);
        break;

    case HCI_AUTHENTICATION_COMP_EVT:
        btu_hcif_authentication_comp_evt(p, hci_evt_len);
        break;

    case HCI_RMT_NAME_REQUEST_COMP_EVT:
        btu_hcif_rmt_name_request_comp_evt(p, hci_evt_len);
        break;

    case HCI_ENCRYPTION_CHANGE_EVT:
        btu_hcif_encryption_change_evt(p, hci_evt_len);
        break;

    case HCI_CHANGE_CONN_LINK_KEY_EVT:
        btu_hcif_change_conn_link_key_evt(p, hci_evt_len);
        break;

    case HCI_MASTER_LINK_KEY_COMP_EVT:
        btu_hcif_master_link_key_comp_evt(p, hci_evt_len);
        break;

    case HCI_READ_RMT_FEATURES_COMP_EVT:
        btu_hcif_read_rmt_features_comp_evt(p, hci_evt_len);
        break;

    case HCI_READ_RMT_VERSION_COMP_EVT:
        btu_hcif_read_rmt_version_comp_evt(p, hci_evt_len);
        break;

    case HCI_QOS_SETUP_COMP_EVT:
        btu_hcif_qos_setup_comp_evt(p, hci_evt_len);
        break;

    case HCI_COMMAND_COMPLETE_EVT:
        btu_hcif_command_complete_evt(p, hci_evt_len);
        break;

    case HCI_COMMAND_STATUS_EVT:
        btu_hcif_command_status_evt(p, hci_evt_len);
        break;

    case HCI_HARDWARE_ERROR_EVT:
        btu_hcif_hardware_error_evt(p, hci_evt_len);
        break;

    case HCI_FLUSH_OCCURED_EVT:
        btu_hcif_flush_occured_evt(p, hci_evt_len);
        break;

    case HCI_ROLE_CHANGE_EVT:
        btu_hcif_role_change_evt(p, hci_evt_len);
        break;

    case HCI_NUM_COMPL_DATA_PKTS_EVT:
        btu_hcif_num_compl_data_pkts_evt(p, hci_evt_len);
        break;

    case HCI_MODE_CHANGE_EVT:
        btu_hcif_mode_change_evt(p, hci_evt_len);
        break;

    case HCI_RETURN_LINK_KEYS_EVT:
        btu_hcif_return_link_keys_evt(p, hci_evt_len);
        break;

    case HCI_PIN_CODE_REQUEST_EVT:
        btu_hcif_pin_code_request_evt(p, hci_evt_len);
        break;

    case HCI_LINK_KEY_REQUEST_EVT:
        btu_hcif_link_key_request_evt(p, hci_evt_len);
        break;

    case HCI_LINK_KEY_NOTIFICATION_EVT:
        btu_hcif_link_key_notification_evt(p, hci_evt_len);
        break;

    case HCI_LOOPBACK_COMMAND_EVT:
        btu_hcif_loopback_command_evt(p, hci_evt_len);
        break;

    case HCI_DATA_BUF_OVERFLOW_EVT:
        btu_hcif_data_buf_overflow_evt(p, hci_evt_len);
        break;

    case HCI_MAX_SLOTS_CHANGED_EVT:
        btu_hcif_max_slots_changed_evt(p, hci_evt_len);
        break;

    case HCI_READ_CLOCK_OFF_COMP_EVT:
        btu_hcif_read_clock_off_comp_evt(p, hci_evt_len);
        break;

    case HCI_CONN_PKT_TYPE_CHANGE_EVT:
        btu_hcif_conn_pkt_type_change_evt(p, hci_evt_len);
        break;

    case HCI_QOS_VIOLATION_EVT:
        btu_hcif_qos_violation_evt(p, hci_evt_len);
        break;

    case HCI_PAGE_SCAN_MODE_CHANGE_EVT:
        btu_hcif_page_scan_mode_change_evt(p, hci_evt_len);
        break;

    case HCI_PAGE_SCAN_REP_MODE_CHNG_EVT:
        btu_hcif_page_scan_rep_mode_chng_evt(p, hci_evt_len);
        break;

    case HCI_ESCO_CONNECTION_COMP_EVT:
        btu_hcif_esco_connection_comp_evt(p, hci_evt_len);
        break;

    case HCI_ESCO_CONNECTION_CHANGED_EVT:
        btu_hcif_esco_connection_chg_evt(p, hci_evt_len);
        break;

    case 0xef:
    case HCI_VENDOR_SPECIFIC_EVT:
        btm_vendor_specific_evt(p, hci_evt_len);
        break;
    }
}

void btu_hcif_send_cmd(BT_HDR* p_buf) {
    if (p_buf && btu_cb.cmd_xmit_q.count) {
        GKI_enqueue(&btu_cb.cmd_xmit_q, p_buf);
        p_buf = NULL;
    }

    if (btu_cb.controller_cmd_window == 0 && btm_cb.devcb.state == 0)
        btu_cb.controller_cmd_window = btu_cb.cmd_xmit_q.count + 1;

    while (btu_cb.controller_cmd_window) {
        if (!p_buf)
            p_buf = GKI_dequeue(&btu_cb.cmd_xmit_q);

        if (!p_buf)
            break;

        btu_hcif_store_cmd(p_buf);

        --btu_cb.controller_cmd_window;

        bte_hcisu_send(p_buf, 0x2000);

        p_buf = NULL;
    }

    if (p_buf)
        GKI_enqueue(&btu_cb.cmd_xmit_q, p_buf);
}

void btu_hcif_send_host_rdy_for_data(void) {
    UINT16 num_pkts[MAX_L2CAP_LINKS + 4];
    UINT16 handles[MAX_L2CAP_LINKS + 4];
    UINT8 num_ents = l2c_link_pkts_rcvd(num_pkts, handles);
    BT_HDR* p_buf;

    if (num_ents && ((p_buf = GKI_getpoolbuf(2)) != NULL))
        btsnd_hcic_host_num_xmitted_pkts(p_buf, num_ents, handles, num_pkts);
}

static void btu_hcif_inquiry_comp_evt(UINT8* p, UINT16 evt_len) {
    UINT8 status;
    STREAM_TO_UINT8(p, &status);

    btm_process_inq_complete(status);
}

static void btu_hcif_inquiry_result_evt(UINT8* p, UINT16 evt_len) {
    btm_process_inq_results(p, BTM_INQ_RESULT_STANDARD);
}

static void btu_hcif_inquiry_rssi_result_evt(UINT8* p, UINT16 evt_len) {
    btm_process_inq_results(p, BTM_INQ_RESULT_WITH_RSSI);
}

static void btu_hcif_connection_comp_evt(UINT8* p, UINT16 evt_len) {
    UINT8 status;
    UINT16 handle;
    BD_ADDR bda;
    UINT8 link_type;
    UINT8 enc_mode;
    tBTM_ESCO_DATA esco_data;

    STREAM_TO_UINT8(p, &status);
    STREAM_TO_UINT16(p, &handle);
    STREAM_TO_BDADDR(p, bda);
    STREAM_TO_UINT8(p, &link_type);
    STREAM_TO_UINT8(p, &enc_mode);

    handle = HCID_GET_HANDLE(handle);

    if (link_type == HCI_LINK_TYPE_ACL) {
        btm_sec_connected(bda, handle, status, enc_mode);

        l2c_link_hci_conn_comp(status, handle, bda);
    } else {
        memset(&esco_data, 0, sizeof esco_data);
        memcpy(esco_data.bd_addr, bda, BD_ADDR_LEN);

        btm_sco_connected(status, bda, handle, &esco_data);
    }
}

static void btu_hcif_connection_request_evt(UINT8* p, UINT16 evt_len) {
    BD_ADDR bda;
    DEV_CLASS dc;
    UINT8 link_type;

    STREAM_TO_BDADDR(p, bda);
    STREAM_TO_DEVCLASS(p, dc);
    STREAM_TO_UINT8(p, &link_type);

    if (link_type == HCI_LINK_TYPE_ACL)
        btm_sec_conn_req(bda, dc);
    else
        btm_sco_conn_req(bda, dc, link_type);
}

static void btu_hcif_disconnection_comp_evt(UINT8* p, UINT16 evt_len) {
    UINT8 status;
    UINT16 handle;
    UINT8 reason;

    STREAM_TO_UINT8(p, &status);
    STREAM_TO_UINT16(p, &handle);
    STREAM_TO_UINT8(p, &reason);

    handle = HCID_GET_HANDLE(handle);

    if (!l2c_link_hci_disc_comp(handle, reason))
        btm_sco_removed(handle, reason);

    btm_sec_disconnected(handle, reason);
}

static void btu_hcif_authentication_comp_evt(UINT8* p, UINT16 evt_len) {
    UINT8 status;
    UINT16 handle;

    STREAM_TO_UINT8(p, &status);
    STREAM_TO_UINT16(p, &handle);

    btm_sec_auth_complete(handle, status);
}

static void btu_hcif_rmt_name_request_comp_evt(UINT8* p, UINT16 evt_len) {
    UINT8 status;
    BD_ADDR bd_addr;

    STREAM_TO_UINT8(p, &status);
    STREAM_TO_BDADDR(p, bd_addr);

    evt_len -= BD_ADDR_LEN + 1;

    btm_process_remote_name(bd_addr, p, evt_len, status);

    btm_sec_rmt_name_request_complete(bd_addr, p, status);
}

static void btu_hcif_encryption_change_evt(UINT8* p, UINT16 evt_len) {
    UINT8 status;
    UINT16 handle;
    UINT8 encr_enable;

    STREAM_TO_UINT8(p, &status);
    STREAM_TO_UINT16(p, &handle);
    STREAM_TO_UINT8(p, &encr_enable);

    btm_acl_encrypt_change(handle, status, encr_enable);
    btm_sec_encrypt_change(handle, status, encr_enable);
}

static void btu_hcif_change_conn_link_key_evt(UINT8* p, UINT16 evt_len) {
    /* ... */
}

static void btu_hcif_master_link_key_comp_evt(UINT8* p, UINT16 evt_len) {
    UINT8 status;
    UINT16 handle;
    UINT8 key_flg;

    STREAM_TO_UINT8(p, &status);
    STREAM_TO_UINT16(p, &handle);
    STREAM_TO_UINT8(p, &key_flg);

    btm_sec_mkey_comp_event(handle, status, key_flg);
}

static void btu_hcif_read_rmt_features_comp_evt(UINT8* p, UINT16 evt_len) {
    btm_read_remote_features_complete(p);
}

static void btu_hcif_read_rmt_version_comp_evt(UINT8* p, UINT16 evt_len) {
    btm_read_remote_version_complete(p);
}

static void btu_hcif_qos_setup_comp_evt(UINT8* p, UINT16 evt_len) {
    UINT8 status;
    UINT16 handle;
    FLOW_SPEC flow;

    STREAM_TO_UINT8(p, &status);
    STREAM_TO_UINT16(p, &handle);
    STREAM_TO_UINT8(p, &flow.qos_flags);
    STREAM_TO_UINT8(p, &flow.service_type);
    STREAM_TO_UINT32(p, &flow.token_rate);
    STREAM_TO_UINT32(p, &flow.peak_bandwidth);
    STREAM_TO_UINT32(p, &flow.latency);
    STREAM_TO_UINT32(p, &flow.delay_variation);

    btm_qos_setup_complete(status, handle, &flow);
}

static void btu_hcif_esco_connection_comp_evt(UINT8* p, UINT16 evt_len) {
    tBTM_ESCO_DATA data;
    UINT16 handle;
    BD_ADDR bda;
    UINT8 status;

    STREAM_TO_UINT8(p, &status);
    STREAM_TO_UINT16(p, &handle);
    STREAM_TO_BDADDR(p, bda);
    STREAM_TO_UINT8(p, &data.link_type);
    STREAM_TO_UINT8(p, &data.tx_interval);
    STREAM_TO_UINT8(p, &data.retrans_window);
    STREAM_TO_UINT16(p, &data.rx_pkt_len);
    STREAM_TO_UINT16(p, &data.tx_pkt_len);
    STREAM_TO_UINT8(p, &data.air_mode);

    memcpy(data.bd_addr, bda, BD_ADDR_LEN);
    btm_sco_connected(status, bda, handle, &data);
}

static void btu_hcif_esco_connection_chg_evt(UINT8* p, UINT16 evt_len) {
    UINT16 handle;
    UINT16 tx_pkt_len;
    UINT16 rx_pkt_len;
    UINT8 status;
    UINT8 tx_interval;
    UINT8 retrans_window;

    STREAM_TO_UINT8(p, &status);
    STREAM_TO_UINT16(p, &handle);
    STREAM_TO_UINT8(p, &tx_interval);
    STREAM_TO_UINT8(p, &retrans_window);
    STREAM_TO_UINT16(p, &rx_pkt_len);
    STREAM_TO_UINT16(p, &tx_pkt_len);

    btm_esco_proc_conn_chg(status, handle, tx_interval, retrans_window, rx_pkt_len, tx_pkt_len);
}

static void btu_hcif_hdl_command_complete(UINT16 opcode, UINT8* p, UINT16 evt_len) {
    switch (opcode) {
    case HCI_RESET:
        btm_reset_complete();
        break;

    case HCI_INQUIRY_CANCEL:
        btm_process_inq_complete(HCI_SUCCESS);
        break;

    case HCI_SET_EVENT_FILTER:
        btm_event_filter_complete(p);
        break;

    case HCI_READ_STORED_LINK_KEY:
        btm_read_stored_link_key_complete(p);
        break;

    case HCI_WRITE_STORED_LINK_KEY:
        btm_write_stored_link_key_complete(p);
        break;

    case HCI_DELETE_STORED_LINK_KEY:
        btm_delete_stored_link_key_complete(p);
        break;

    case HCI_READ_LOCAL_VERSION_INFO:
        btm_read_local_version_complete(p, evt_len);
        break;

    case HCI_READ_POLICY_SETTINGS:
        btm_read_link_policy_complete(p);
        break;

    case HCI_READ_BUFFER_SIZE:
        btm_read_hci_buf_size_complete(p, evt_len);
        break;

    case HCI_READ_LOCAL_FEATURES:
        btm_read_local_features_complete(p, evt_len);
        break;

    case HCI_READ_LOCAL_NAME:
        btm_read_local_name_complete(p, evt_len);
        break;

    case HCI_READ_BD_ADDR:
        btm_read_local_addr_complete(p, evt_len);
        break;

    case HCI_GET_LINK_QUALITY:
        btm_read_link_quality_complete(p);
        break;

    case HCI_READ_RSSI:
        btm_read_rssi_complete(p);
        break;

    default:
        if ((opcode & HCI_GROUP_VENDOR_SPECIFIC) == HCI_GROUP_VENDOR_SPECIFIC)
            btm_vsc_complete(p, opcode, evt_len);

        break;
    }
}

static void btu_hcif_command_complete_evt(UINT8* p, UINT16 evt_len) {
    UINT16 cc_opcode;
    BT_HDR* p_cmd;
    UINT8* pp;
    UINT16 queued_opcode;

    STREAM_TO_UINT8(p, &btu_cb.controller_cmd_window);
    STREAM_TO_UINT16(p, &cc_opcode);

    evt_len -= 3;

    if (cc_opcode != HCI_RESET && cc_opcode != HCI_HOST_NUM_PACKETS_DONE && cc_opcode != HCI_COMMAND_NONE) {
        p_cmd = GKI_dequeue(&btu_cb.cmd_cmpl_q);

        if (p_cmd) {
            pp = (UINT8*)(p_cmd + 1);
            STREAM_TO_UINT16(pp, &queued_opcode);

            if (cc_opcode == 0xfc4c && queued_opcode != cc_opcode) {
                GKI_enqueue_head(&btu_cb.cmd_cmpl_q, p_cmd);
                return;
            } else {
                GKI_freebuf(p_cmd);
            }
        }

        if (!GKI_queue_is_empty(&btu_cb.cmd_cmpl_q))
            btu_start_timer(&btu_cb.cmd_cmpl_timer, 60, 8);
        else
            btu_stop_timer(&btu_cb.cmd_cmpl_timer);
    }

    btu_hcif_hdl_command_complete(cc_opcode, p, evt_len);
    btu_hcif_send_cmd(NULL);
}

static void btu_hcif_hdl_command_status(UINT16 opcode, UINT8 status, UINT8* p_cmd) {
    BD_ADDR bd_addr;
    UINT16 handle;
    tBTM_ESCO_DATA esco_data;

    switch (opcode) {
    case HCI_EXIT_SNIFF_MODE:
    case HCI_EXIT_PARK_MODE:
    case HCI_HOLD_MODE:
    case HCI_SNIFF_MODE:
    case HCI_PARK_MODE:
        btm_pm_proc_cmd_status(status);
        return;
    }

    if (status == HCI_SUCCESS)
        return;

    switch (opcode) {
    case HCI_INQUIRY:
        btm_process_inq_complete(status);
        break;

    case HCI_RMT_NAME_REQUEST:
        btm_process_remote_name(NULL, NULL, 0, status);

        btm_sec_rmt_name_request_complete(NULL, NULL, status);
        break;

    case HCI_QOS_SETUP_COMP_EVT:
        btm_qos_setup_complete(status, 0, NULL);
        break;

    case HCI_SWITCH_ROLE:
        btm_acl_role_changed(status, NULL, BTM_ROLE_UNDEFINED);
        l2c_link_role_change_failed();
        break;

    case HCI_CREATE_CONNECTION:
        if (p_cmd) {
            ++p_cmd;
            STREAM_TO_BDADDR(p_cmd, bd_addr);

            btm_sec_connected(bd_addr, HCI_INVALID_HANDLE, status, 0);
            l2c_link_hci_conn_comp(status, HCI_INVALID_HANDLE, bd_addr);
        }

        break;

    case HCI_SETUP_ESCO_CONNECTION:
        if (p_cmd) {
            ++p_cmd;
            STREAM_TO_UINT16(p_cmd, &handle);

            if (btm_is_sco_active(handle))
                btm_esco_proc_conn_chg(status, handle, 0, 0, 0, 0);
            else
                btm_sco_connected(status, NULL, handle, &esco_data);
        }

        break;

    default:
        if ((opcode & HCI_GROUP_VENDOR_SPECIFIC) == HCI_GROUP_VENDOR_SPECIFIC)
            btm_vsc_complete(p_cmd, opcode, 1);
    }
}

static void btu_hcif_command_status_evt(UINT8* p, UINT16 evt_len) {
    UINT8 status;
    UINT16 opcode;
    BT_HDR* p_cmd = NULL;
    UINT16 cmd_opcode;
    UINT8* p_data = NULL;

    STREAM_TO_UINT8(p, &status);
    STREAM_TO_UINT8(p, &btu_cb.controller_cmd_window);
    STREAM_TO_UINT16(p, &opcode);

    if (opcode != HCI_RESET && opcode != HCI_HOST_NUM_PACKETS_DONE && opcode != HCI_COMMAND_NONE) {
        if ((p_cmd = GKI_dequeue(&btu_cb.cmd_cmpl_q)) != NULL) {
            p_data = (UINT8*)(p_cmd + 1) + p_cmd->offset;
            STREAM_TO_UINT16(p_data, &cmd_opcode);

            if (cmd_opcode != opcode) {
                p_data = NULL;
                BT_TRACE(HCI, WARNING, "Event mismatch opcode=%X cmd opcode=%X", opcode, cmd_opcode);
            }
        }

        if (BTU_CMD_CMPL_TIMEOUT > 0) {
            if (!GKI_queue_is_empty(&btu_cb.cmd_cmpl_q)) {
                btu_start_timer(&btu_cb.cmd_cmpl_timer, BTU_TTYPE_BTU_CMD_CMPL, BTU_CMD_CMPL_TIMEOUT);
            } else {
                btu_stop_timer(&btu_cb.cmd_cmpl_timer);
            }
        }
    }

    btu_hcif_hdl_command_status(opcode, status, p_data);

    if (p_cmd)
        GKI_freebuf(p_cmd);

    btu_hcif_send_cmd(NULL);
}

void btu_hcif_cmd_timeout(void) {
    BT_HDR* p_cmd;
    UINT8* p;
    UINT16 opcode;
    UINT8 event[1];

    btu_cb.controller_cmd_window = 1;

    if ((p_cmd = GKI_dequeue(&btu_cb.cmd_cmpl_q)) == NULL) {
        BT_TRACE(HCI, WARNING, "Cmd timeout; no cmd in queue");
        return;
    }

    if (BTU_CMD_CMPL_TIMEOUT > 0) {
        if (!GKI_queue_is_empty(&btu_cb.cmd_cmpl_q)) {
            btu_start_timer(&btu_cb.cmd_cmpl_timer, BTU_TTYPE_BTU_CMD_CMPL, BTU_CMD_CMPL_TIMEOUT);
        }
    }

    p = (UINT8*)(p_cmd + 1) + p_cmd->offset;

    STREAM_TO_UINT16(p, &opcode);

    BT_TRACE(HCI, WARNING, "BTU HCI command timeout - cmd opcode = 0x%02x", opcode);

    switch (opcode) {
    case HCI_HOLD_MODE:
    case HCI_SNIFF_MODE:
    case HCI_EXIT_SNIFF_MODE:
    case HCI_PARK_MODE:
    case HCI_EXIT_PARK_MODE:
    case HCI_INQUIRY:
    case HCI_RMT_NAME_REQUEST:
    case HCI_QOS_SETUP_COMP_EVT:
    case HCI_CREATE_CONNECTION:
        btu_hcif_hdl_command_status(opcode, HCI_ERR_UNSPECIFIED, p);
        break;

    default:
        event[0] = HCI_ERR_UNSPECIFIED;
        btu_hcif_hdl_command_complete(opcode, event, 1);
        break;
    }

    GKI_freebuf(p_cmd);

    btm_report_device_status(BTM_DEV_STATUS_CMD_TOUT);
    btu_hcif_send_cmd(NULL);
}

static void btu_hcif_hardware_error_evt(UINT8* p, UINT16 evt_len) {
    BT_TRACE(HCI, ERROR, "Ctlr H/w error event");

    if (BTM_IsDeviceUp())
        BTM_DeviceReset(NULL);
}

static void btu_hcif_flush_occured_evt(UINT8* p, UINT16 evt_len) {
    /* ... */
}

static void btu_hcif_role_change_evt(UINT8* p, UINT16 evt_len) {
    UINT8 status;
    BD_ADDR bda;
    UINT8 role;

    STREAM_TO_UINT8(p, &status);
    STREAM_TO_BDADDR(p, bda);
    STREAM_TO_UINT8(p, &role);

    if (status == 0)
        l2c_link_role_changed(bda, role);

    btm_acl_role_changed(status, bda, role);
}

static void btu_hcif_num_compl_data_pkts_evt(UINT8* p, UINT16 evt_len) {
    l2c_link_process_num_completed_pkts(p);
}

static void btu_hcif_mode_change_evt(UINT8* p, UINT16 evt_len) {
    UINT8 status;
    UINT16 handle;
    UINT8 current_mode;
    UINT16 interval;

    STREAM_TO_UINT8(p, &status);
    STREAM_TO_UINT16(p, &handle);
    STREAM_TO_UINT8(p, &current_mode);
    STREAM_TO_UINT16(p, &interval);

    btm_sco_chk_pend_unpark(status, handle, current_mode);
    btm_pm_proc_mode_change(status, handle, current_mode, interval);

    hidd_pm_proc_mode_change(status, current_mode, interval);
}

static void btu_hcif_return_link_keys_evt(UINT8* p, UINT16 evt_len) {
    UINT8 num_keys;
    tBTM_RETURN_LINK_KEYS_EVT* result;

    num_keys = *p;
    if (!num_keys)
        return;

    result = (tBTM_RETURN_LINK_KEYS_EVT*)(--p);
    result->event = BTM_CB_EVT_RETURN_LINK_KEYS;

    btm_return_link_keys_evt(result);
}

static void btu_hcif_pin_code_request_evt(UINT8* p, UINT16 evt_len) {
    BD_ADDR bda;

    STREAM_TO_BDADDR(p, bda);

    l2c_pin_code_request(bda);
    btm_sec_pin_code_request(bda);
}

static void btu_hcif_link_key_request_evt(UINT8* p, UINT16 evt_len) {
    BD_ADDR bda;

    STREAM_TO_BDADDR(p, bda);

    btm_sec_link_key_request(bda);
}

static void btu_hcif_link_key_notification_evt(UINT8* p, UINT16 evt_len) {
    BD_ADDR bda;
    LINK_KEY key;
    UINT8 key_type;

    STREAM_TO_BDADDR(p, bda);
    STREAM_TO_ARRAY16(p, key);
    STREAM_TO_UINT8(p, &key_type);

    btm_sec_link_key_notification(bda, key, key_type);
}

static void btu_hcif_loopback_command_evt(UINT8* p, UINT16 evt_len) {
    /* ... */
}

static void btu_hcif_data_buf_overflow_evt(UINT8* p, UINT16 evt_len) {
    /* ... */
}

static void btu_hcif_max_slots_changed_evt(UINT8* p, UINT16 evt_len) {
    /* ... */
}

static void btu_hcif_read_clock_off_comp_evt(UINT8* p, UINT16 evt_len) {
    UINT8 status;
    UINT16 handle;
    UINT16 clock_offset;

    STREAM_TO_UINT8(p, &status);

    if (status != HCI_SUCCESS)
        return;

    STREAM_TO_UINT16(p, &handle);
    STREAM_TO_UINT16(p, &clock_offset);

    handle = HCID_GET_HANDLE(handle);

    btm_process_clk_off_comp_evt(handle, clock_offset);
    btm_sec_update_clock_offset(handle, clock_offset);
}

static void btu_hcif_conn_pkt_type_change_evt(UINT8* p, UINT16 evt_len) {
    /* ... */
}

static void btu_hcif_qos_violation_evt(UINT8* p, UINT16 evt_len) {
    UINT16 handle;

    STREAM_TO_UINT16(p, &handle);

    handle = HCID_GET_HANDLE(handle);

    l2c_link_hci_qos_violation(handle);
}

static void btu_hcif_page_scan_mode_change_evt(UINT8* p, UINT16 evt_len) {
    /* ... */
}

static void btu_hcif_page_scan_rep_mode_chng_evt(UINT8* p, UINT16 evt_len) {
    /* ... */
}
