#include "hcimsgs.h"

/* Original source:
 * bluedroid <android.googlesource.com/platform/external/bluetooth/bluedroid>
 * stack/hcic/hcicmds.c
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

#include <macros.h>  // ARRAY_LENGTH

#include "bt_target.h"
#include "bt_types.h"
#include "data_types.h"

#include "btu.h"
#include "hcidefs.h"

/*******************************************************************************
 * functions
 */

BOOLEAN btsnd_hcic_inquiry(LAP const inq_lap, UINT8 duration, UINT8 response_cnt) {
    BT_HDR* p;
    UINT8* pp;

    if ((p = HCI_GET_CMD_BUF(HCIC_PARAM_SIZE_INQUIRY)) == NULL)
        return FALSE;

    pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_INQUIRY;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_INQUIRY);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_INQUIRY);

    LAP_TO_STREAM(pp, inq_lap);
    UINT8_TO_STREAM(pp, duration);
    UINT8_TO_STREAM(pp, response_cnt);

    btu_hcif_send_cmd(p);
    return TRUE;
}

BOOLEAN btsnd_hcic_inq_cancel(void) {
    BT_HDR* p;
    UINT8* pp;

    if ((p = HCI_GET_CMD_BUF(HCIC_PARAM_SIZE_INQ_CANCEL)) == NULL)
        return FALSE;

    pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_INQ_CANCEL;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_INQUIRY_CANCEL);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_INQ_CANCEL);

    btu_hcif_send_cmd(p);
    return TRUE;
}

BOOLEAN btsnd_hcic_per_inq_mode(UINT16 max_period, UINT16 min_period, LAP const inq_lap, UINT8 duration, UINT8 response_cnt) {
    BT_HDR* p;
    UINT8* pp;

    if ((p = HCI_GET_CMD_BUF(HCIC_PARAM_SIZE_PER_INQ_MODE)) == NULL)
        return FALSE;

    pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_PER_INQ_MODE;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_PERIODIC_INQUIRY_MODE);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_PER_INQ_MODE);

    UINT16_TO_STREAM(pp, max_period);
    UINT16_TO_STREAM(pp, min_period);
    LAP_TO_STREAM(pp, inq_lap);
    UINT8_TO_STREAM(pp, duration);
    UINT8_TO_STREAM(pp, response_cnt);

    btu_hcif_send_cmd(p);
    return TRUE;
}

BOOLEAN btsnd_hcic_exit_per_inq(void) {
    BT_HDR* p;
    UINT8* pp;

    if ((p = HCI_GET_CMD_BUF(HCIC_PARAM_SIZE_EXIT_PER_INQ)) == NULL)
        return FALSE;

    pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_EXIT_PER_INQ;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_EXIT_PERIODIC_INQUIRY_MODE);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_EXIT_PER_INQ);

    btu_hcif_send_cmd(p);
    return TRUE;
}

BOOLEAN btsnd_hcic_create_conn(BD_ADDR dest, UINT16 packet_types, UINT8 page_scan_rep_mode, UINT8 page_scan_mode, UINT16 clock_offset,
                               UINT8 allow_switch) {
    BT_HDR* p;
    UINT8* pp;

    if ((p = HCI_GET_CMD_BUF(HCIC_PARAM_SIZE_CREATE_CONN)) == NULL)
        return FALSE;

    pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_CREATE_CONN;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_CREATE_CONNECTION);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_CREATE_CONN);

    BDADDR_TO_STREAM(pp, dest);
    UINT16_TO_STREAM(pp, packet_types);
    UINT8_TO_STREAM(pp, page_scan_rep_mode);
    UINT8_TO_STREAM(pp, page_scan_mode);
    UINT16_TO_STREAM(pp, clock_offset);
    UINT8_TO_STREAM(pp, allow_switch);

    btu_hcif_send_cmd(p);
    return TRUE;
}

BOOLEAN btsnd_hcic_disconnect(UINT16 handle, UINT8 reason) {
    BT_HDR* p;
    UINT8* pp;

    if ((p = HCI_GET_CMD_BUF(HCIC_PARAM_SIZE_DISCONNECT)) == NULL)
        return FALSE;

    pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_DISCONNECT;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_DISCONNECT);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_DISCONNECT);

    UINT16_TO_STREAM(pp, handle);
    UINT8_TO_STREAM(pp, reason);

    btu_hcif_send_cmd(p);
    return TRUE;
}

BOOLEAN btsnd_hcic_add_SCO_conn(UINT16 handle, UINT16 packet_types) {
    BT_HDR* p;
    UINT8* pp;

    if ((p = HCI_GET_CMD_BUF(HCIC_PARAM_SIZE_ADD_SCO_CONN)) == NULL)
        return FALSE;

    pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_ADD_SCO_CONN;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_ADD_SCO_CONNECTION);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_ADD_SCO_CONN);

    UINT16_TO_STREAM(pp, handle);
    UINT16_TO_STREAM(pp, packet_types);

    btu_hcif_send_cmd(p);
    return TRUE;
}

BOOLEAN btsnd_hcic_create_conn_cancel(BD_ADDR dest) {
    BT_HDR* p;
    UINT8* pp;

    if ((p = HCI_GET_CMD_BUF(HCIC_PARAM_SIZE_CREATE_CONN_CANCEL)) == NULL)
        return FALSE;

    pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_CREATE_CONN_CANCEL;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_CREATE_CONNECTION_CANCEL);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_CREATE_CONN_CANCEL);

    BDADDR_TO_STREAM(pp, dest);

    btu_hcif_send_cmd(p);
    return TRUE;
}

void btsnd_hcic_accept_conn(void* buffer, BD_ADDR dest, UINT8 role) {
    BT_HDR* p = buffer;
    UINT8* pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_ACCEPT_CONN;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_ACCEPT_CONNECTION_REQUEST);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_ACCEPT_CONN);

    BDADDR_TO_STREAM(pp, dest);
    UINT8_TO_STREAM(pp, role);

    btu_hcif_send_cmd(p);
}

void btsnd_hcic_reject_conn(void* buffer, BD_ADDR dest, UINT8 reason) {
    BT_HDR* p = buffer;
    UINT8* pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_REJECT_CONN;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_REJECT_CONNECTION_REQUEST);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_REJECT_CONN);

    BDADDR_TO_STREAM(pp, dest);
    UINT8_TO_STREAM(pp, reason);

    btu_hcif_send_cmd(p);
}

BOOLEAN btsnd_hcic_link_key_req_reply(BD_ADDR bd_addr, LINK_KEY link_key) {
    BT_HDR* p;
    UINT8* pp;

    if ((p = HCI_GET_CMD_BUF(HCIC_PARAM_SIZE_LINK_KEY_REQ_REPLY)) == NULL)
        return FALSE;

    pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_LINK_KEY_REQ_REPLY;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_LINK_KEY_REQUEST_REPLY);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_LINK_KEY_REQ_REPLY);

    BDADDR_TO_STREAM(pp, bd_addr);
    ARRAY16_TO_STREAM(pp, link_key);

    btu_hcif_send_cmd(p);
    return TRUE;
}

BOOLEAN btsnd_hcic_link_key_neg_reply(BD_ADDR bd_addr) {
    BT_HDR* p;
    UINT8* pp;

    if ((p = HCI_GET_CMD_BUF(HCIC_PARAM_SIZE_LINK_KEY_NEG_REPLY)) == NULL)
        return FALSE;

    pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_LINK_KEY_NEG_REPLY;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_LINK_KEY_REQUEST_NEG_REPLY);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_LINK_KEY_NEG_REPLY);

    BDADDR_TO_STREAM(pp, bd_addr);

    btu_hcif_send_cmd(p);
    return TRUE;
}

BOOLEAN btsnd_hcic_pin_code_req_reply(BD_ADDR bd_addr, UINT8 pin_code_len, PIN_CODE pin_code) {
    BT_HDR* p;
    UINT8* pp;
    int i;

    if ((p = HCI_GET_CMD_BUF(HCIC_PARAM_SIZE_PIN_CODE_REQ_REPLY)) == NULL)
        return FALSE;

    pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_PIN_CODE_REQ_REPLY;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_PIN_CODE_REQUEST_REPLY);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_PIN_CODE_REQ_REPLY);

    BDADDR_TO_STREAM(pp, bd_addr);
    UINT8_TO_STREAM(pp, pin_code_len);

    for (i = 0; i < pin_code_len; i++)
        *pp++ = *pin_code++;

    for (; i < PIN_CODE_LEN; i++)
        *pp++ = 0;

    btu_hcif_send_cmd(p);
    return TRUE;
}

BOOLEAN btsnd_hcic_pin_code_neg_reply(BD_ADDR bd_addr) {
    BT_HDR* p;
    UINT8* pp;

    if ((p = HCI_GET_CMD_BUF(HCIC_PARAM_SIZE_PIN_CODE_NEG_REPLY)) == NULL)
        return FALSE;

    pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_PIN_CODE_NEG_REPLY;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_PIN_CODE_REQUEST_NEG_REPLY);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_PIN_CODE_NEG_REPLY);

    BDADDR_TO_STREAM(pp, bd_addr);

    btu_hcif_send_cmd(p);
    return TRUE;
}

BOOLEAN btsnd_hcic_change_conn_type(UINT16 handle, UINT16 packet_types) {
    BT_HDR* p;
    UINT8* pp;

    if ((p = HCI_GET_CMD_BUF(HCIC_PARAM_SIZE_CHANGE_CONN_TYPE)) == NULL)
        return FALSE;

    pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_CHANGE_CONN_TYPE;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_CHANGE_CONN_PACKET_TYPE);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_CHANGE_CONN_TYPE);

    UINT16_TO_STREAM(pp, handle);
    UINT16_TO_STREAM(pp, packet_types);

    btu_hcif_send_cmd(p);
    return TRUE;
}

BOOLEAN btsnd_hcic_auth_request(UINT16 handle) {
    BT_HDR* p;
    UINT8* pp;

    if ((p = HCI_GET_CMD_BUF(HCIC_PARAM_SIZE_CMD_HANDLE)) == NULL)
        return FALSE;

    pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_CMD_HANDLE;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_AUTHENTICATION_REQUESTED);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_CMD_HANDLE);

    UINT16_TO_STREAM(pp, handle);

    btu_hcif_send_cmd(p);
    return TRUE;
}

BOOLEAN btsnd_hcic_set_conn_encrypt(UINT16 handle, BOOLEAN enable) {
    BT_HDR* p;
    UINT8* pp;

    if ((p = HCI_GET_CMD_BUF(HCIC_PARAM_SIZE_SET_CONN_ENCRYPT)) == NULL)
        return FALSE;

    pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_SET_CONN_ENCRYPT;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_SET_CONN_ENCRYPTION);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_SET_CONN_ENCRYPT);

    UINT16_TO_STREAM(pp, handle);
    UINT8_TO_STREAM(pp, enable);

    btu_hcif_send_cmd(p);
    return TRUE;
}

BOOLEAN btsnd_hcic_change_link_key(UINT16 handle) {
    BT_HDR* p;
    UINT8* pp;

    if ((p = HCI_GET_CMD_BUF(HCIC_PARAM_SIZE_CMD_HANDLE)) == NULL)
        return FALSE;

    pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_CMD_HANDLE;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_CHANGE_CONN_LINK_KEY);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_CMD_HANDLE);

    UINT16_TO_STREAM(pp, handle);

    btu_hcif_send_cmd(p);
    return TRUE;
}

BOOLEAN btsnd_hcic_master_link_key(BOOLEAN key_flag) {
    BT_HDR* p;
    UINT8* pp;

    if ((p = HCI_GET_CMD_BUF(HCIC_PARAM_SIZE_MASTER_LINK_KEY)) == NULL)
        return FALSE;

    pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_MASTER_LINK_KEY;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_MASTER_LINK_KEY);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_MASTER_LINK_KEY);

    UINT8_TO_STREAM(pp, key_flag);

    btu_hcif_send_cmd(p);
    return TRUE;
}

BOOLEAN btsnd_hcic_rmt_name_req(BD_ADDR bd_addr, UINT8 page_scan_rep_mode, UINT8 page_scan_mode, UINT16 clock_offset) {
    BT_HDR* p;
    UINT8* pp;

    if ((p = HCI_GET_CMD_BUF(HCIC_PARAM_SIZE_RMT_NAME_REQ)) == NULL)
        return FALSE;

    pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_RMT_NAME_REQ;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_RMT_NAME_REQUEST);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_RMT_NAME_REQ);

    BDADDR_TO_STREAM(pp, bd_addr);
    UINT8_TO_STREAM(pp, page_scan_rep_mode);
    UINT8_TO_STREAM(pp, page_scan_mode);
    UINT16_TO_STREAM(pp, clock_offset);

    btu_hcif_send_cmd(p);
    return TRUE;
}

BOOLEAN btsnd_hcic_rmt_name_req_cancel(BD_ADDR bd_addr) {
    BT_HDR* p;
    UINT8* pp;

    if ((p = HCI_GET_CMD_BUF(HCIC_PARAM_SIZE_RMT_NAME_REQ_CANCEL)) == NULL)
        return FALSE;

    pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_RMT_NAME_REQ_CANCEL;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_RMT_NAME_REQUEST_CANCEL);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_RMT_NAME_REQ_CANCEL);

    BDADDR_TO_STREAM(pp, bd_addr);

    btu_hcif_send_cmd(p);
    return TRUE;
}

BOOLEAN btsnd_hcic_rmt_features_req(UINT16 handle) {
    BT_HDR* p;
    UINT8* pp;

    if ((p = HCI_GET_CMD_BUF(HCIC_PARAM_SIZE_CMD_HANDLE)) == NULL)
        return FALSE;

    pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_CMD_HANDLE;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_READ_RMT_FEATURES);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_CMD_HANDLE);

    UINT16_TO_STREAM(pp, handle);

    btu_hcif_send_cmd(p);
    return TRUE;
}

void btsnd_hcic_rmt_ext_features(void* buffer, UINT16 handle, UINT8 page_num) {
    BT_HDR* p = buffer;
    UINT8* pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_RMT_EXT_FEATURES;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_READ_RMT_EXT_FEATURES);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_RMT_EXT_FEATURES);

    UINT16_TO_STREAM(pp, handle);
    UINT8_TO_STREAM(pp, page_num);

    btu_hcif_send_cmd(p);
}

BOOLEAN btsnd_hcic_rmt_ver_req(UINT16 handle) {
    BT_HDR* p;
    UINT8* pp;

    if ((p = HCI_GET_CMD_BUF(HCIC_PARAM_SIZE_CMD_HANDLE)) == NULL)
        return FALSE;

    pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_CMD_HANDLE;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_READ_RMT_VERSION_INFO);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_CMD_HANDLE);

    UINT16_TO_STREAM(pp, handle);

    btu_hcif_send_cmd(p);
    return TRUE;
}

BOOLEAN btsnd_hcic_read_rmt_clk_offset(UINT16 handle) {
    BT_HDR* p;
    UINT8* pp;

    if ((p = HCI_GET_CMD_BUF(HCIC_PARAM_SIZE_CMD_HANDLE)) == NULL)
        return FALSE;

    pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_CMD_HANDLE;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_READ_RMT_CLOCK_OFFSET);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_CMD_HANDLE);

    UINT16_TO_STREAM(pp, handle);

    btu_hcif_send_cmd(p);
    return TRUE;
}

void btsnd_hcic_read_lmp_handle(void* buffer, UINT16 handle) {
    BT_HDR* p = buffer;
    UINT8* pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_CMD_HANDLE;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_READ_LMP_HANDLE);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_CMD_HANDLE);

    UINT16_TO_STREAM(pp, handle);

    btu_hcif_send_cmd(p);
}

BOOLEAN btsnd_hcic_setup_esco_conn(UINT16 handle, UINT32 tx_bw, UINT32 rx_bw, UINT16 max_latency, UINT16 voice, UINT8 retrans_effort,
                                   UINT16 packet_types) {
    BT_HDR* p;
    UINT8* pp;

    if ((p = HCI_GET_CMD_BUF(HCIC_PARAM_SIZE_SETUP_ESCO)) == NULL)
        return FALSE;

    pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_SETUP_ESCO;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_SETUP_ESCO_CONNECTION);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_SETUP_ESCO);

    UINT16_TO_STREAM(pp, handle);
    UINT32_TO_STREAM(pp, tx_bw);
    UINT32_TO_STREAM(pp, rx_bw);
    UINT16_TO_STREAM(pp, max_latency);
    UINT16_TO_STREAM(pp, voice);
    UINT8_TO_STREAM(pp, retrans_effort);
    UINT16_TO_STREAM(pp, packet_types);

    btu_hcif_send_cmd(p);
    return TRUE;
}

void btsnd_hcic_accept_esco_conn(void* buffer, BD_ADDR bd_addr, UINT32 tx_bw, UINT32 rx_bw, UINT16 max_latency, UINT16 content_fmt,
                                 UINT8 retrans_effort, UINT16 packet_types) {
    BT_HDR* p = buffer;
    UINT8* pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_ACCEPT_ESCO;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_ACCEPT_ESCO_CONNECTION);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_ACCEPT_ESCO);

    BDADDR_TO_STREAM(pp, bd_addr);
    UINT32_TO_STREAM(pp, tx_bw);
    UINT32_TO_STREAM(pp, rx_bw);
    UINT16_TO_STREAM(pp, max_latency);
    UINT16_TO_STREAM(pp, content_fmt);
    UINT8_TO_STREAM(pp, retrans_effort);
    UINT16_TO_STREAM(pp, packet_types);

    btu_hcif_send_cmd(p);
}

void btsnd_hcic_reject_esco_conn(void* buffer, BD_ADDR bd_addr, UINT8 reason) {
    BT_HDR* p = buffer;
    UINT8* pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_REJECT_ESCO;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_REJECT_ESCO_CONNECTION);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_REJECT_ESCO);

    BDADDR_TO_STREAM(pp, bd_addr);
    UINT8_TO_STREAM(pp, reason);

    btu_hcif_send_cmd(p);
}

BOOLEAN btsnd_hcic_hold_mode(void* buffer, UINT16 handle, UINT16 max_hold_period, UINT16 min_hold_period) {
    BT_HDR* p;
    UINT8* pp;

    // ???
    if (!buffer && (buffer = HCI_GET_CMD_BUF(HCIC_PARAM_SIZE_HOLD_MODE)) == NULL) {
        return FALSE;
    }

    p = buffer;
    pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_HOLD_MODE;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_HOLD_MODE);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_HOLD_MODE);

    UINT16_TO_STREAM(pp, handle);
    UINT16_TO_STREAM(pp, max_hold_period);
    UINT16_TO_STREAM(pp, min_hold_period);

    btu_hcif_send_cmd(p);
    return TRUE;
}

BOOLEAN btsnd_hcic_sniff_mode(void* buffer, UINT16 handle, UINT16 max_sniff_period, UINT16 min_sniff_period, UINT16 sniff_attempt,
                              UINT16 sniff_timeout) {
    BT_HDR* p;
    UINT8* pp;

    if (!buffer && (buffer = HCI_GET_CMD_BUF(HCIC_PARAM_SIZE_HOLD_MODE)) == NULL) {
        return FALSE;
    }

    p = buffer;
    pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_SNIFF_MODE;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_SNIFF_MODE);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_SNIFF_MODE);

    UINT16_TO_STREAM(pp, handle);
    UINT16_TO_STREAM(pp, max_sniff_period);
    UINT16_TO_STREAM(pp, min_sniff_period);
    UINT16_TO_STREAM(pp, sniff_attempt);
    UINT16_TO_STREAM(pp, sniff_timeout);

    btu_hcif_send_cmd(p);
    return TRUE;
}

BOOLEAN btsnd_hcic_exit_sniff_mode(void* buffer, UINT16 handle) {
    BT_HDR* p;
    UINT8* pp;

    if (!buffer && (buffer = HCI_GET_CMD_BUF(HCIC_PARAM_SIZE_HOLD_MODE)) == NULL) {
        return FALSE;
    }

    p = buffer;
    pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_CMD_HANDLE;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_EXIT_SNIFF_MODE);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_CMD_HANDLE);

    UINT16_TO_STREAM(pp, handle);

    btu_hcif_send_cmd(p);
    return TRUE;
}

BOOLEAN btsnd_hcic_park_mode(void* buffer, UINT16 handle, UINT16 beacon_max_interval, UINT16 beacon_min_interval) {
    BT_HDR* p;
    UINT8* pp;

    if (!buffer && (buffer = HCI_GET_CMD_BUF(HCIC_PARAM_SIZE_HOLD_MODE)) == NULL) {
        return FALSE;
    }

    p = buffer;
    pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_PARK_MODE;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_PARK_MODE);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_PARK_MODE);

    UINT16_TO_STREAM(pp, handle);
    UINT16_TO_STREAM(pp, beacon_max_interval);
    UINT16_TO_STREAM(pp, beacon_min_interval);

    btu_hcif_send_cmd(p);
    return TRUE;
}

BOOLEAN btsnd_hcic_exit_park_mode(void* buffer, UINT16 handle) {
    BT_HDR* p;
    UINT8* pp;

    if (!buffer && (buffer = HCI_GET_CMD_BUF(HCIC_PARAM_SIZE_HOLD_MODE)) == NULL) {
        return FALSE;
    }

    p = buffer;
    pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_CMD_HANDLE;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_EXIT_PARK_MODE);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_CMD_HANDLE);

    UINT16_TO_STREAM(pp, handle);

    btu_hcif_send_cmd(p);
    return TRUE;
}

BOOLEAN btsnd_hcic_qos_setup(UINT16 handle, UINT8 flags, UINT8 service_type, UINT32 token_rate, UINT32 peak, UINT32 latency, UINT32 delay_var) {
    BT_HDR* p;
    UINT8* pp;

    if ((p = HCI_GET_CMD_BUF(HCIC_PARAM_SIZE_QOS_SETUP)) == NULL)
        return FALSE;

    pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_QOS_SETUP;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_QOS_SETUP);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_QOS_SETUP);

    UINT16_TO_STREAM(pp, handle);
    UINT8_TO_STREAM(pp, flags);
    UINT8_TO_STREAM(pp, service_type);
    UINT32_TO_STREAM(pp, token_rate);
    UINT32_TO_STREAM(pp, peak);
    UINT32_TO_STREAM(pp, latency);
    UINT32_TO_STREAM(pp, delay_var);

    btu_hcif_send_cmd(p);
    return TRUE;
}

void btsnd_hcic_role_discovery(void* buffer, UINT16 handle) {
    BT_HDR* p = buffer;
    UINT8* pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_CMD_HANDLE;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_ROLE_DISCOVERY);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_CMD_HANDLE);

    UINT16_TO_STREAM(pp, handle);

    btu_hcif_send_cmd(p);
}

BOOLEAN btsnd_hcic_switch_role(BD_ADDR bd_addr, UINT8 role) {
    BT_HDR* p;
    UINT8* pp;

    if ((p = HCI_GET_CMD_BUF(HCIC_PARAM_SIZE_SWITCH_ROLE)) == NULL)
        return FALSE;

    pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_SWITCH_ROLE;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_SWITCH_ROLE);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_SWITCH_ROLE);

    BDADDR_TO_STREAM(pp, bd_addr);
    UINT8_TO_STREAM(pp, role);

    btu_hcif_send_cmd(p);
    return TRUE;
}

BOOLEAN btsnd_hcic_read_policy_set(UINT16 handle) {
    BT_HDR* p;
    UINT8* pp;

    if ((p = HCI_GET_CMD_BUF(HCIC_PARAM_SIZE_CMD_HANDLE)) == NULL)
        return FALSE;

    pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_CMD_HANDLE;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_READ_POLICY_SETTINGS);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_CMD_HANDLE);

    UINT16_TO_STREAM(pp, handle);

    btu_hcif_send_cmd(p);
    return TRUE;
}

BOOLEAN btsnd_hcic_write_policy_set(UINT16 handle, UINT16 settings) {
    BT_HDR* p;
    UINT8* pp;

    if ((p = HCI_GET_CMD_BUF(HCIC_PARAM_SIZE_WRITE_POLICY_SET)) == NULL)
        return FALSE;

    pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_WRITE_POLICY_SET;
    p->offset = 0;
    UINT16_TO_STREAM(pp, HCI_WRITE_POLICY_SETTINGS);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_WRITE_POLICY_SET);

    UINT16_TO_STREAM(pp, handle);
    UINT16_TO_STREAM(pp, settings);

    btu_hcif_send_cmd(p);
    return TRUE;
}

void btsnd_hcic_read_def_policy_set(void* buffer) {
    BT_HDR* p = buffer;
    UINT8* pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_READ_DEF_POLICY_SET;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_READ_DEF_POLICY_SETTINGS);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_READ_DEF_POLICY_SET);

    btu_hcif_send_cmd(p);
}

void btsnd_hcic_write_def_policy_set(void* buffer, UINT16 settings) {
    BT_HDR* p = buffer;
    UINT8* pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_WRITE_DEF_POLICY_SET;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_WRITE_DEF_POLICY_SETTINGS);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_WRITE_DEF_POLICY_SET);

    UINT16_TO_STREAM(pp, settings);

    btu_hcif_send_cmd(p);
}

void btsnd_hcic_flow_specification(void* buffer, UINT16 handle, UINT8 flags, UINT8 flow_direct, UINT8 service_type, UINT32 token_rate,
                                   UINT32 token_bucket_size, UINT32 peak, UINT32 latency) {
    BT_HDR* p = buffer;
    UINT8* pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_FLOW_SPEC;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_FLOW_SPECIFICATION);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_FLOW_SPEC);

    UINT16_TO_STREAM(pp, handle);
    UINT8_TO_STREAM(pp, flags);
    UINT8_TO_STREAM(pp, flow_direct);
    UINT8_TO_STREAM(pp, service_type);
    UINT32_TO_STREAM(pp, token_rate);
    UINT32_TO_STREAM(pp, token_bucket_size);
    UINT32_TO_STREAM(pp, peak);
    UINT32_TO_STREAM(pp, latency);

    btu_hcif_send_cmd(p);
}

void btsnd_hcic_sniff_sub_rate(void* buffer, UINT16 handle, UINT16 max_lat, UINT16 min_remote_lat, UINT16 min_local_lat) {
    BT_HDR* p = buffer;
    UINT8* pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_SNIFF_SUB_RATE;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_SNIFF_SUB_RATE);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_SNIFF_SUB_RATE);

    UINT16_TO_STREAM(pp, handle);
    UINT16_TO_STREAM(pp, max_lat);
    UINT16_TO_STREAM(pp, min_remote_lat);
    UINT16_TO_STREAM(pp, min_local_lat);

    btu_hcif_send_cmd(p);
}

void btsnd_hcic_set_event_mask(void* buffer, BT_EVENT_MASK event_mask) {
    BT_HDR* p = buffer;
    UINT8* pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_SET_EVENT_MASK;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_SET_EVENT_MASK);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_SET_EVENT_MASK);

    ARRAY8_TO_STREAM(pp, event_mask);

    btu_hcif_send_cmd(p);
}

BOOLEAN btsnd_hcic_reset(void) {
    BT_HDR* p;
    UINT8* pp;

    if ((p = HCI_GET_CMD_BUF(HCIC_PARAM_SIZE_RESET)) == NULL)
        return FALSE;

    pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_RESET;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_RESET);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_RESET);

    btu_hcif_send_cmd(p);
    return TRUE;
}

void btsnd_hcic_set_event_filter(void* buffer, UINT8 filt_type, UINT8 filt_cond_type, UINT8* filt_cond, UINT8 filt_cond_len) {
    BT_HDR* p = buffer;
    UINT8* pp = (UINT8*)(p + 1);

    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_SET_EVENT_FILTER);

    if (filt_type) {
        p->len = (UINT16)(HCIC_PREAMBLE_SIZE + 2 + filt_cond_len);
        UINT8_TO_STREAM(pp, (UINT8)(2 + filt_cond_len));

        UINT8_TO_STREAM(pp, filt_type);
        UINT8_TO_STREAM(pp, filt_cond_type);

        if (filt_cond_type == HCI_FILTER_COND_DEVICE_CLASS) {
            DEVCLASS_TO_STREAM(pp, filt_cond);
            filt_cond += DEV_CLASS_LEN;
            DEVCLASS_TO_STREAM(pp, filt_cond);
            filt_cond += DEV_CLASS_LEN;

            filt_cond_len -= (2 * DEV_CLASS_LEN);
        } else if (filt_cond_type == HCI_FILTER_COND_BD_ADDR) {
            BDADDR_TO_STREAM(pp, filt_cond);
            filt_cond += BD_ADDR_LEN;

            filt_cond_len -= BD_ADDR_LEN;
        }

        if (filt_cond_len)
            ARRAY_TO_STREAM(pp, filt_cond, filt_cond_len);
    } else {
        p->len = (UINT16)(HCIC_PREAMBLE_SIZE + 1);
        UINT8_TO_STREAM(pp, 1);

        UINT8_TO_STREAM(pp, filt_type);
    }

    btu_hcif_send_cmd(p);
}

void btsnd_hcic_flush(void* buffer, UINT16 handle) {
    BT_HDR* p = buffer;
    UINT8* pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_CMD_HANDLE;
    p->offset = 0;
    UINT16_TO_STREAM(pp, HCI_FLUSH);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_CMD_HANDLE);

    UINT16_TO_STREAM(pp, handle);

    btu_hcif_send_cmd(p);
}

void btsnd_hcic_read_pin_type(void* buffer) {
    BT_HDR* p = buffer;
    UINT8* pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_READ_CMD;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_READ_PIN_TYPE);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_READ_CMD);

    btu_hcif_send_cmd(p);
}

BOOLEAN btsnd_hcic_write_pin_type(UINT8 type) {
    BT_HDR* p;
    UINT8* pp;

    if ((p = HCI_GET_CMD_BUF(HCIC_PARAM_SIZE_WRITE_PARAM1)) == NULL)
        return FALSE;

    pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_WRITE_PARAM1;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_WRITE_PIN_TYPE);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_WRITE_PARAM1);

    UINT8_TO_STREAM(pp, type);

    btu_hcif_send_cmd(p);
    return TRUE;
}

void btsnd_hcic_new_unit_key(void* buffer) {
    BT_HDR* p = buffer;
    UINT8* pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_NEW_UNIT_KEY;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_CREATE_NEW_UNIT_KEY);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_NEW_UNIT_KEY);

    btu_hcif_send_cmd(p);
}

void btsnd_hcic_read_stored_key(void* buffer, BD_ADDR bd_addr, BOOLEAN read_all_flag) {
    BT_HDR* p = buffer;
    UINT8* pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_READ_STORED_KEY;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_READ_STORED_LINK_KEY);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_READ_STORED_KEY);

    BDADDR_TO_STREAM(pp, bd_addr);
    UINT8_TO_STREAM(pp, read_all_flag);

    btu_hcif_send_cmd(p);
}

void btsnd_hcic_write_stored_key(void* buffer, UINT8 num_keys, BD_ADDR* bd_addr, LINK_KEY* link_key) {
    BT_HDR* p = buffer;
    UINT8* pp = (UINT8*)(p + 1);
    int j;

    p->len = HCIC_PREAMBLE_SIZE + 1 + (num_keys * (BD_ADDR_LEN + LINK_KEY_LEN));
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_WRITE_STORED_LINK_KEY);
    UINT8_TO_STREAM(pp, p->len - HCIC_PREAMBLE_SIZE);

    if (num_keys > HCI_MAX_NUM_OF_LINK_KEYS_PER_CMD)
        num_keys = HCI_MAX_NUM_OF_LINK_KEYS_PER_CMD;

    UINT8_TO_STREAM(pp, num_keys);

    for (j = 0; j < num_keys; j++) {
        BDADDR_TO_STREAM(pp, bd_addr[j]);
        ARRAY16_TO_STREAM(pp, link_key[j]);
    }

    btu_hcif_send_cmd(p);
}

BOOLEAN btsnd_hcic_delete_stored_key(BD_ADDR bd_addr, BOOLEAN delete_all_flag) {
    BT_HDR* p;
    UINT8* pp;

    if ((p = HCI_GET_CMD_BUF(HCIC_PARAM_SIZE_DELETE_STORED_KEY)) == NULL)
        return FALSE;

    pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_DELETE_STORED_KEY;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_DELETE_STORED_LINK_KEY);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_DELETE_STORED_KEY);

    BDADDR_TO_STREAM(pp, bd_addr);
    UINT8_TO_STREAM(pp, delete_all_flag);

    btu_hcif_send_cmd(p);
    return TRUE;
}

void btsnd_hcic_change_name(void* buffer, BD_NAME name) {
    BT_HDR* p = buffer;
    UINT8* pp = (UINT8*)(p + 1);
    UINT16 len = strlen((char*)name) + 1;

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_CHANGE_NAME;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_CHANGE_LOCAL_NAME);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_CHANGE_NAME);

    ARRAY_TO_STREAM(pp, name, len);

    btu_hcif_send_cmd(p);
}

void btsnd_hcic_read_name(void* buffer) {
    BT_HDR* p = buffer;
    UINT8* pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_READ_CMD;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_READ_LOCAL_NAME);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_READ_CMD);

    btu_hcif_send_cmd(p);
}

void btsnd_hcic_read_conn_acc_tout(void* buffer) {
    BT_HDR* p = buffer;
    UINT8* pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_READ_CMD;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_READ_CONN_ACCEPT_TOUT);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_READ_CMD);

    btu_hcif_send_cmd(p);
}

void btsnd_hcic_write_conn_acc_tout(void* buffer, UINT16 timeout) {
    BT_HDR* p = buffer;
    UINT8* pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_WRITE_PARAM2;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_WRITE_CONN_ACCEPT_TOUT);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_WRITE_PARAM2);

    UINT16_TO_STREAM(pp, timeout);

    btu_hcif_send_cmd(p);
}

void btsnd_hcic_read_page_tout(void* buffer) {
    BT_HDR* p = buffer;
    UINT8* pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_READ_CMD;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_READ_PAGE_TOUT);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_READ_CMD);

    btu_hcif_send_cmd(p);
}

void btsnd_hcic_write_page_tout(void* buffer, UINT16 timeout) {
    BT_HDR* p = buffer;
    UINT8* pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_WRITE_PARAM2;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_WRITE_PAGE_TOUT);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_WRITE_PARAM2);

    UINT16_TO_STREAM(pp, timeout);

    btu_hcif_send_cmd(p);
}

void btsnd_hcic_read_scan_enable(void* buffer) {
    BT_HDR* p = buffer;
    UINT8* pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_READ_CMD;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_READ_SCAN_ENABLE);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_READ_CMD);

    btu_hcif_send_cmd(p);
}

void btsnd_hcic_write_scan_enable(void* buffer, UINT8 flag) {
    BT_HDR* p = buffer;
    UINT8* pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_WRITE_PARAM1;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_WRITE_SCAN_ENABLE);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_WRITE_PARAM1);

    UINT8_TO_STREAM(pp, flag);

    btu_hcif_send_cmd(p);
}

void btsnd_hcic_read_pagescan_cfg(void* buffer) {
    BT_HDR* p = buffer;
    UINT8* pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_READ_CMD;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_READ_PAGESCAN_CFG);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_READ_CMD);

    btu_hcif_send_cmd(p);
}

void btsnd_hcic_write_pagescan_cfg(void* buffer, UINT16 interval, UINT16 window) {
    BT_HDR* p = buffer;
    UINT8* pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_WRITE_PAGESCAN_CFG;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_WRITE_PAGESCAN_CFG);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_WRITE_PAGESCAN_CFG);

    UINT16_TO_STREAM(pp, interval);
    UINT16_TO_STREAM(pp, window);

    btu_hcif_send_cmd(p);
}

void btsnd_hcic_read_inqscan_cfg(void* buffer) {
    BT_HDR* p = buffer;
    UINT8* pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_READ_CMD;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_READ_INQUIRYSCAN_CFG);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_READ_CMD);

    btu_hcif_send_cmd(p);
}

void btsnd_hcic_write_inqscan_cfg(void* buffer, UINT16 interval, UINT16 window) {
    BT_HDR* p = buffer;
    UINT8* pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_WRITE_INQSCAN_CFG;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_WRITE_INQUIRYSCAN_CFG);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_WRITE_INQSCAN_CFG);

    UINT16_TO_STREAM(pp, interval);
    UINT16_TO_STREAM(pp, window);

    btu_hcif_send_cmd(p);
}

void btsnd_hcic_read_auth_enable(void* buffer) {
    BT_HDR* p = buffer;
    UINT8* pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_READ_CMD;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_READ_AUTHENTICATION_ENABLE);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_READ_CMD);

    btu_hcif_send_cmd(p);
}

BOOLEAN btsnd_hcic_write_auth_enable(UINT8 flag) {
    BT_HDR* p;
    UINT8* pp;

    if ((p = HCI_GET_CMD_BUF(HCIC_PARAM_SIZE_WRITE_PARAM1)) == NULL)
        return FALSE;

    pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_WRITE_PARAM1;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_WRITE_AUTHENTICATION_ENABLE);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_WRITE_PARAM1);

    UINT8_TO_STREAM(pp, flag);

    btu_hcif_send_cmd(p);
    return TRUE;
}

void btsnd_hcic_read_encr_mode(void* buffer) {
    BT_HDR* p = buffer;
    UINT8* pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_READ_CMD;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_READ_ENCRYPTION_MODE);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_READ_CMD);

    btu_hcif_send_cmd(p);
}

BOOLEAN btsnd_hcic_write_encr_mode(UINT8 mode) {
    BT_HDR* p;
    UINT8* pp;

    if ((p = HCI_GET_CMD_BUF(HCIC_PARAM_SIZE_WRITE_PARAM1)) == NULL)
        return FALSE;

    pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_WRITE_PARAM1;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_WRITE_ENCRYPTION_MODE);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_WRITE_PARAM1);

    UINT8_TO_STREAM(pp, mode);

    btu_hcif_send_cmd(p);
    return TRUE;
}

void btsnd_hcic_read_dev_class(void* buffer) {
    BT_HDR* p = buffer;
    UINT8* pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_READ_CMD;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_READ_CLASS_OF_DEVICE);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_READ_CMD);

    btu_hcif_send_cmd(p);
}

void btsnd_hcic_write_dev_class(void* buffer, DEV_CLASS dev_class) {
    BT_HDR* p = buffer;
    UINT8* pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_WRITE_PARAM3;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_WRITE_CLASS_OF_DEVICE);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_WRITE_PARAM3);

    DEVCLASS_TO_STREAM(pp, dev_class);

    btu_hcif_send_cmd(p);
}

void btsnd_hcic_read_voice_settings(void* buffer) {
    BT_HDR* p = buffer;
    UINT8* pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_READ_CMD;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_READ_VOICE_SETTINGS);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_READ_CMD);

    btu_hcif_send_cmd(p);
}

void btsnd_hcic_write_voice_settings(void* buffer, UINT16 flags) {
    BT_HDR* p = buffer;
    UINT8* pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_WRITE_PARAM2;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_WRITE_VOICE_SETTINGS);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_WRITE_PARAM2);

    UINT16_TO_STREAM(pp, flags);

    btu_hcif_send_cmd(p);
}

void btsnd_hcic_read_auto_flush_tout(void* buffer, UINT16 handle) {
    BT_HDR* p = buffer;
    UINT8* pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_CMD_HANDLE;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_READ_AUTO_FLUSH_TOUT);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_CMD_HANDLE);

    UINT16_TO_STREAM(pp, handle);

    btu_hcif_send_cmd(p);
}

void btsnd_hcic_write_auto_flush_tout(void* buffer, UINT16 handle, UINT16 tout) {
    BT_HDR* p = buffer;
    UINT8* pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_WRITE_AUTO_FLUSH_TOUT;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_WRITE_AUTO_FLUSH_TOUT);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_WRITE_AUTO_FLUSH_TOUT);

    UINT16_TO_STREAM(pp, handle);
    UINT16_TO_STREAM(pp, tout);

    btu_hcif_send_cmd(p);
}

void btsnd_hcic_read_num_bcast_xmit(void* buffer) {
    BT_HDR* p = buffer;
    UINT8* pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_READ_CMD;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_READ_NUM_BCAST_REXMITS);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_READ_CMD);

    btu_hcif_send_cmd(p);
}

void btsnd_hcic_write_num_bcast_xmit(void* buffer, UINT8 num) {
    BT_HDR* p = buffer;
    UINT8* pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_WRITE_PARAM1;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_WRITE_NUM_BCAST_REXMITS);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_WRITE_PARAM1);

    UINT8_TO_STREAM(pp, num);

    btu_hcif_send_cmd(p);
}

void btsnd_hcic_read_hold_mode_act(void* buffer) {
    BT_HDR* p = buffer;
    UINT8* pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_READ_CMD;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_READ_HOLD_MODE_ACTIVITY);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_READ_CMD);

    btu_hcif_send_cmd(p);
}

void btsnd_hcic_write_hold_mode_act(void* buffer, UINT8 flags) {
    BT_HDR* p = buffer;
    UINT8* pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_WRITE_PARAM1;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_WRITE_HOLD_MODE_ACTIVITY);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_WRITE_PARAM1);

    UINT8_TO_STREAM(pp, flags);

    btu_hcif_send_cmd(p);
}

void btsnd_hcic_read_tx_power(void* buffer, UINT16 handle, UINT8 type) {
    BT_HDR* p = buffer;
    UINT8* pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_READ_TX_POWER;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_READ_TRANSMIT_POWER_LEVEL);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_READ_TX_POWER);

    UINT16_TO_STREAM(pp, handle);
    UINT8_TO_STREAM(pp, type);

    btu_hcif_send_cmd(p);
}

void btsnd_hcic_read_sco_flow_enable(void* buffer) {
    BT_HDR* p = buffer;
    UINT8* pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_READ_CMD;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_READ_SCO_FLOW_CTRL_ENABLE);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_READ_CMD);

    btu_hcif_send_cmd(p);
}

void btsnd_hcic_write_sco_flow_enable(void* buffer, UINT8 flag) {
    BT_HDR* p = buffer;
    UINT8* pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_WRITE_PARAM1;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_WRITE_SCO_FLOW_CTRL_ENABLE);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_WRITE_PARAM1);

    UINT8_TO_STREAM(pp, flag);

    btu_hcif_send_cmd(p);
}

BOOLEAN btsnd_hcic_set_host_flow_ctrl(UINT8 value) {
    BT_HDR* p;
    UINT8* pp;

    if ((p = HCI_GET_CMD_BUF(HCIC_PARAM_SIZE_WRITE_PARAM1)) == NULL)
        return FALSE;

    pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_WRITE_PARAM1;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_SET_HC_TO_HOST_FLOW_CTRL);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_WRITE_PARAM1);

    UINT8_TO_STREAM(pp, value);

    btu_hcif_send_cmd(p);
    return TRUE;
}

BOOLEAN btsnd_hcic_set_host_buf_size(UINT16 acl_len, UINT8 sco_len, UINT16 acl_num, UINT16 sco_num) {
    BT_HDR* p;
    UINT8* pp;

    if ((p = HCI_GET_CMD_BUF(HCIC_PARAM_SIZE_SET_HOST_BUF_SIZE)) == NULL)
        return FALSE;

    pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_SET_HOST_BUF_SIZE;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_HOST_BUFFER_SIZE);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_SET_HOST_BUF_SIZE);

    UINT16_TO_STREAM(pp, acl_len);
    UINT8_TO_STREAM(pp, sco_len);
    UINT16_TO_STREAM(pp, acl_num);
    UINT16_TO_STREAM(pp, sco_num);

    btu_hcif_send_cmd(p);
    return TRUE;
}

void btsnd_hcic_host_num_xmitted_pkts(void* buffer, UINT8 num_handles, UINT16* handle, UINT16* num_pkts) {
    BT_HDR* p = buffer;
    UINT8* pp = (UINT8*)(p + 1);
    int j;

    p->len = HCIC_PREAMBLE_SIZE + 1 + (num_handles * 4);
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_HOST_NUM_PACKETS_DONE);
    UINT8_TO_STREAM(pp, p->len - HCIC_PREAMBLE_SIZE);

    UINT8_TO_STREAM(pp, num_handles);

    for (j = 0; j < num_handles; j++) {
        UINT16_TO_STREAM(pp, handle[j]);
        UINT16_TO_STREAM(pp, num_pkts[j]);
    }

    btu_hcif_send_cmd(p);
}

void btsnd_hcic_read_link_super_tout(void* buffer, UINT16 handle) {
    BT_HDR* p = buffer;
    UINT8* pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_CMD_HANDLE;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_READ_LINK_SUPER_TOUT);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_CMD_HANDLE);

    UINT16_TO_STREAM(pp, handle);

    btu_hcif_send_cmd(p);
}

BOOLEAN btsnd_hcic_write_link_super_tout(UINT16 handle, UINT16 timeout) {
    BT_HDR* p;
    UINT8* pp;

    if ((p = HCI_GET_CMD_BUF(HCIC_PARAM_SIZE_WRITE_LINK_SUPER_TOUT)) == NULL)
        return FALSE;

    pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_WRITE_LINK_SUPER_TOUT;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_WRITE_LINK_SUPER_TOUT);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_WRITE_LINK_SUPER_TOUT);

    UINT16_TO_STREAM(pp, handle);
    UINT16_TO_STREAM(pp, timeout);

    btu_hcif_send_cmd(p);
    return TRUE;
}

void btsnd_hcic_read_max_iac(void* buffer) {
    BT_HDR* p = buffer;
    UINT8* pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_READ_CMD;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_READ_NUM_SUPPORTED_IAC);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_READ_CMD);

    btu_hcif_send_cmd(p);
}

void btsnd_hcic_read_cur_iac_lap(void* buffer) {
    BT_HDR* p = buffer;
    UINT8* pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_READ_CMD;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_READ_CURRENT_IAC_LAP);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_READ_CMD);

    btu_hcif_send_cmd(p);
}

void btsnd_hcic_write_cur_iac_lap(void* buffer, UINT8 num_cur_iac, LAP* const iac_lap) {
    BT_HDR* p = buffer;
    UINT8* pp = (UINT8*)(p + 1);
    int i;

    p->len = HCIC_PREAMBLE_SIZE + 1 + (LAP_LEN * num_cur_iac);
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_WRITE_CURRENT_IAC_LAP);
    UINT8_TO_STREAM(pp, p->len - HCIC_PREAMBLE_SIZE);

    UINT8_TO_STREAM(pp, num_cur_iac);

    for (i = 0; i < num_cur_iac; i++)
        LAP_TO_STREAM(pp, iac_lap[i]);

    btu_hcif_send_cmd(p);
}

void btsnd_hcic_read_page_scan_per(void* buffer) {
    BT_HDR* p = buffer;
    UINT8* pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_READ_CMD;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_READ_PAGESCAN_PERIOD_MODE);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_READ_CMD);

    btu_hcif_send_cmd(p);
}

void btsnd_hcic_write_page_scan_per(void* buffer, UINT8 mode) {
    BT_HDR* p = buffer;
    UINT8* pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_WRITE_PARAM1;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_WRITE_PAGESCAN_PERIOD_MODE);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_WRITE_PARAM1);

    UINT8_TO_STREAM(pp, mode);

    btu_hcif_send_cmd(p);
}

void btsnd_hcic_read_page_scan_mode(void* buffer) {
    BT_HDR* p = buffer;
    UINT8* pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_READ_CMD;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_READ_PAGESCAN_MODE);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_READ_CMD);

    btu_hcif_send_cmd(p);
}

void btsnd_hcic_write_page_scan_mode(void* buffer, UINT8 mode) {
    BT_HDR* p = buffer;
    UINT8* pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_WRITE_PARAM1;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_WRITE_PAGESCAN_MODE);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_WRITE_PARAM1);

    UINT8_TO_STREAM(pp, mode);

    btu_hcif_send_cmd(p);
}

void btsnd_hcic_read_ext_inquiry_response(void* buffer) {
    BT_HDR* p = buffer;
    UINT8* pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_READ_CMD;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_READ_EXT_INQ_RESPONSE);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_READ_CMD);

    btu_hcif_send_cmd(p);
}

void btsnd_hcic_write_ext_inquiry_response(void* buffer, UINT8 fec_req, UINT8* p_data) {
    BT_HDR* p = buffer;
    UINT8* pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_EXT_INQ_RESP;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_WRITE_EXT_INQ_RESPONSE);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_EXT_INQ_RESP);

    UINT8_TO_STREAM(pp, fec_req);
    ARRAY_TO_STREAM(pp, p_data, HCIC_PARAM_SIZE_EXT_INQ_RESP - 1);

    btu_hcif_send_cmd(p);
}

BOOLEAN btsnd_hcic_read_local_ver(void) {
    BT_HDR* p;
    UINT8* pp;

    if ((p = HCI_GET_CMD_BUF(HCIC_PARAM_SIZE_READ_CMD)) == NULL)
        return FALSE;

    pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_READ_CMD;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_READ_LOCAL_VERSION_INFO);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_READ_CMD);

    btu_hcif_send_cmd(p);
    return TRUE;
}

void btsnd_hcic_read_local_supported_cmds(void* buffer) {
    BT_HDR* p = buffer;
    UINT8* pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_READ_CMD;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_READ_LOCAL_SUPPORTED_CMDS);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_READ_CMD);

    btu_hcif_send_cmd(p);
}

BOOLEAN btsnd_hcic_read_local_features(void) {
    BT_HDR* p;
    UINT8* pp;

    if ((p = HCI_GET_CMD_BUF(HCIC_PARAM_SIZE_READ_CMD)) == NULL)
        return FALSE;

    pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_READ_CMD;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_READ_LOCAL_FEATURES);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_READ_CMD);

    btu_hcif_send_cmd(p);
    return TRUE;
}

void btsnd_hcic_read_local_ext_features(void* buffer, UINT8 page_num) {
    BT_HDR* p = buffer;
    UINT8* pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_LOCAL_EXT_FEATURES;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_READ_LOCAL_EXT_FEATURES);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_LOCAL_EXT_FEATURES);

    UINT8_TO_STREAM(pp, page_num);

    btu_hcif_send_cmd(p);
}

void btsnd_hcic_read_buffer_size(void* buffer) {
    BT_HDR* p = buffer;
    UINT8* pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_READ_CMD;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_READ_BUFFER_SIZE);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_READ_CMD);

    btu_hcif_send_cmd(p);
}

void btsnd_hcic_read_country_code(void* buffer) {
    BT_HDR* p = buffer;
    UINT8* pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_READ_CMD;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_READ_COUNTRY_CODE);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_READ_CMD);

    btu_hcif_send_cmd(p);
}

BOOLEAN btsnd_hcic_read_bd_addr(void) {
    BT_HDR* p;
    UINT8* pp;

    if ((p = HCI_GET_CMD_BUF(HCIC_PARAM_SIZE_READ_CMD)) == NULL)
        return FALSE;

    pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_READ_CMD;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_READ_BD_ADDR);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_READ_CMD);

    btu_hcif_send_cmd(p);
    return TRUE;
}

void btsnd_hcic_read_fail_conn_count(void* buffer, UINT16 handle) {
    BT_HDR* p = buffer;
    UINT8* pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_CMD_HANDLE;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_READ_FAILED_CONN_COUNT);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_CMD_HANDLE);

    UINT16_TO_STREAM(pp, handle);

    btu_hcif_send_cmd(p);
}

void btsnd_hcic_reset_fail_conn_count(void* buffer, UINT16 handle) {
    BT_HDR* p = buffer;
    UINT8* pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_CMD_HANDLE;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_RESET_FAILED_CONN_COUNT);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_CMD_HANDLE);

    UINT16_TO_STREAM(pp, handle);

    btu_hcif_send_cmd(p);
}

BOOLEAN btsnd_hcic_get_link_quality(UINT16 handle) {
    BT_HDR* p;
    UINT8* pp;

    if ((p = HCI_GET_CMD_BUF(HCIC_PARAM_SIZE_CMD_HANDLE)) == NULL)
        return FALSE;

    pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_CMD_HANDLE;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_GET_LINK_QUALITY);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_CMD_HANDLE);

    UINT16_TO_STREAM(pp, handle);

    btu_hcif_send_cmd(p);
    return TRUE;
}

BOOLEAN btsnd_hcic_read_rssi(UINT16 handle) {
    BT_HDR* p;
    UINT8* pp;

    if ((p = HCI_GET_CMD_BUF(HCIC_PARAM_SIZE_CMD_HANDLE)) == NULL)
        return FALSE;

    pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_CMD_HANDLE;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_READ_RSSI);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_CMD_HANDLE);

    UINT16_TO_STREAM(pp, handle);

    btu_hcif_send_cmd(p);
    return TRUE;
}

void btsnd_hcic_read_loopback_mode(void* buffer) {
    BT_HDR* p = buffer;
    UINT8* pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_READ_CMD;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_READ_LOOPBACK_MODE);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_READ_CMD);

    btu_hcif_send_cmd(p);
}

void btsnd_hcic_write_loopback_mode(void* buffer, UINT8 mode) {
    BT_HDR* p = buffer;
    UINT8* pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_WRITE_PARAM1;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_WRITE_LOOPBACK_MODE);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_WRITE_PARAM1);

    UINT8_TO_STREAM(pp, mode);

    btu_hcif_send_cmd(p);
}

void btsnd_hcic_enable_test_mode(void* buffer) {
    BT_HDR* p = buffer;
    UINT8* pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_READ_CMD;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_ENABLE_DEV_UNDER_TEST_MODE);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_READ_CMD);

    btu_hcif_send_cmd(p);
}

BOOLEAN btsnd_hcic_write_afh_channel_assessment_mode(UINT8 mode) {
    BT_HDR* p;
    UINT8* pp;

    if ((p = HCI_GET_CMD_BUF(HCIC_PARAM_SIZE_WRITE_PARAM1)) == NULL)
        return FALSE;

    pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_WRITE_PARAM1;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_WRITE_AFH_ASSESSMENT_MODE);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_WRITE_PARAM1);

    UINT8_TO_STREAM(pp, mode);

    btu_hcif_send_cmd(p);
    return TRUE;
}

void btsnd_hcic_read_afh_channel_assessment_mode(void* buffer) {
    BT_HDR* p = buffer;
    UINT8* pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_READ_CMD;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_READ_AFH_ASSESSMENT_MODE);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_READ_CMD);

    btu_hcif_send_cmd(p);
}

BOOLEAN btsnd_hcic_set_afh_channels(UINT8 first, UINT8 last) {
    BT_HDR* p;
    UINT8* pp;
    UINT8 channels[10] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F};
    int i;

    if ((p = HCI_GET_CMD_BUF(HCIC_PARAM_SIZE_SET_AFH_CHANNELS)) == NULL)
        return FALSE;

    pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_SET_AFH_CHANNELS;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_SET_AFH_CHANNELS);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_SET_AFH_CHANNELS);

    // Why 79, specifically
    if (first <= last && last <= 78) {
        for (i = first; i <= last; i++) {
            int byte_offset = i / 8;
            int bit_offset = i % 8;
            channels[byte_offset] &= ~(1 << bit_offset);
        }
    }

    for (i = 0; i < (int)ARRAY_LENGTH(channels); i++)
        *pp++ = channels[i];

    btu_hcif_send_cmd(p);
    return TRUE;
}

void btsnd_hcic_set_afh_host_channel_class(void* buffer, UINT8* p_afhchannelmap) {
    BT_HDR* p = buffer;
    UINT8* pp = (UINT8*)(p + 1);
    int i;

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_SET_AFH_CHANNELS;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_SET_AFH_CHANNELS);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_SET_AFH_CHANNELS);

    for (i = 0; i < 10; i++)
        *pp++ = p_afhchannelmap[9 - i];

    btu_hcif_send_cmd(p);
}

void btsnd_hcic_read_afh_channel_map(void* buffer, UINT16 handle) {
    BT_HDR* p = buffer;
    UINT8* pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_CMD_HANDLE;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_READ_AFH_CH_MAP);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_CMD_HANDLE);

    UINT16_TO_STREAM(pp, handle);

    btu_hcif_send_cmd(p);
}

void btsnd_hcic_read_clock(void* buffer, UINT16 handle, UINT8 which_clock) {
    BT_HDR* p = buffer;
    UINT8* pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_READ_CLOCK;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_READ_CLOCK);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_READ_CLOCK);

    UINT16_TO_STREAM(pp, handle);
    UINT8_TO_STREAM(pp, which_clock);

    btu_hcif_send_cmd(p);
}

void btsnd_hcic_read_inqscan_type(void* buffer) {
    BT_HDR* p = buffer;
    UINT8* pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_READ_CMD;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_READ_INQSCAN_TYPE);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_READ_CMD);

    btu_hcif_send_cmd(p);
}

void btsnd_hcic_write_inqscan_type(void* buffer, UINT8 type) {
    BT_HDR* p = buffer;
    UINT8* pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_WRITE_PARAM1;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_WRITE_INQSCAN_TYPE);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_WRITE_PARAM1);

    UINT8_TO_STREAM(pp, type);

    btu_hcif_send_cmd(p);
}

void btsnd_hcic_read_inquiry_mode(void* buffer) {
    BT_HDR* p = buffer;
    UINT8* pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_READ_CMD;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_READ_INQUIRY_MODE);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_READ_CMD);

    btu_hcif_send_cmd(p);
}

void btsnd_hcic_write_inquiry_mode(void* buffer, UINT8 mode) {
    BT_HDR* p = buffer;
    UINT8* pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_WRITE_PARAM1;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_WRITE_INQUIRY_MODE);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_WRITE_PARAM1);

    UINT8_TO_STREAM(pp, mode);

    btu_hcif_send_cmd(p);
}

void btsnd_hcic_read_pagescan_type(void* buffer) {
    BT_HDR* p = buffer;
    UINT8* pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_READ_CMD;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_READ_PAGESCAN_TYPE);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_READ_CMD);

    btu_hcif_send_cmd(p);
}

void btsnd_hcic_write_pagescan_type(void* buffer, UINT8 type) {
    BT_HDR* p = buffer;
    UINT8* pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_WRITE_PARAM1;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_WRITE_PAGESCAN_TYPE);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_WRITE_PARAM1);

    UINT8_TO_STREAM(pp, type);

    btu_hcif_send_cmd(p);
}

void btsnd_hcic_vendor_spec_cmd(void* buffer, UINT16 opcode, UINT8 len, UINT8* p_data) {
    BT_HDR* p = buffer;
    UINT8* pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + len;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_GROUP_VENDOR_SPECIFIC | opcode);
    UINT8_TO_STREAM(pp, len);
    ARRAY_TO_STREAM(pp, p_data, len);

    btu_hcif_send_cmd(p);
}

void btsnd_hcic_data(BT_HDR* p_buf, UINT16 len, UINT16 handle, UINT8 boundary, UINT8 broadcast) {
    UINT8* p;

    p_buf->offset -= 4;
    p_buf->len += 4;

    p = (UINT8*)(p_buf + 1) + p_buf->offset;

    UINT16_TO_STREAM(p, handle | ((boundary & 3) << 12) | ((broadcast & 3) << 14));
    UINT16_TO_STREAM(p, len);

    HCI_ACL_DATA_TO_LOWER(p_buf);
}

void btsnd_hcic_nop(void* buffer) {
    BT_HDR* p = buffer;
    UINT8* pp = (UINT8*)(p + 1);

    p->len = HCIC_PREAMBLE_SIZE + HCIC_PARAM_SIZE_READ_CMD;
    p->offset = 0;

    UINT16_TO_STREAM(pp, HCI_COMMAND_NONE);
    UINT8_TO_STREAM(pp, HCIC_PARAM_SIZE_READ_CMD);

    btu_hcif_send_cmd(p);
}
