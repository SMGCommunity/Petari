#include "btm_api.h"

/* Original source:
 * bluedroid <android.googlesource.com/platform/external/bluetooth/bluedroid>
 * stack/btm/btm_sec.c
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

#define BTM_NO_AVAIL_SEC_SERVICES 0xffff

/*******************************************************************************
 * local function declarations
 */

static tBTM_STATUS btm_sec_execute_procedure(tBTM_SEC_DEV_REC* p_dev_rec);
static BOOLEAN btm_sec_start_get_name(tBTM_SEC_DEV_REC* p_dev_rec);
static BOOLEAN btm_sec_start_authentication(tBTM_SEC_DEV_REC* p_dev_rec);
static BOOLEAN btm_sec_start_encryption(tBTM_SEC_DEV_REC* p_dev_rec);
static UINT8 btm_sec_start_authorization(tBTM_SEC_DEV_REC* p_dev_rec);
static BOOLEAN btm_sec_are_all_trusted(UINT32* p_mask);
static tBTM_SEC_SERV_REC* btm_sec_find_first_serv(UINT16 psm);
static tBTM_SEC_SERV_REC* btm_sec_find_next_serv(tBTM_SEC_SERV_REC* p_cur);
static tBTM_SEC_SERV_REC* btm_sec_find_mx_serv(UINT8 is_originator, UINT16 psm, UINT32 mx_proto_id, UINT32 mx_chan_id);
static void btm_sec_collision_timeout(TIMER_LIST_ENT* p_tle);

static void btm_restore_mode(void);
static void btm_clr_client_scn(tBTM_SEC_DEV_REC* p_dev_rec);

/*******************************************************************************
 * functions
 */

BOOLEAN BTM_SecRegister(tBTM_APPL_INFO* p_cb_info) {
    btm_cb.api = *p_cb_info;

    BTM_TRACE(EVENT, "BTM_Sec: application registered");

    return TRUE;
}

BOOLEAN BTM_SecAddLinkKeyNotifyCallback(tBTM_LINK_KEY_CALLBACK* p_callback) {
    int i;

    for (i = 0; i < BTM_SEC_MAX_LINK_KEY_CALLBACKS; ++i) {
        if (btm_cb.p_link_key_callback[i])
            continue;

        btm_cb.p_link_key_callback[i] = p_callback;
        return TRUE;
    }

    return FALSE;
}

BOOLEAN BTM_SecDeleteLinkKeyNotifyCallback(tBTM_LINK_KEY_CALLBACK* p_callback) {
    int i;

    for (i = 0; i < BTM_SEC_MAX_LINK_KEY_CALLBACKS; ++i) {
        if (btm_cb.p_link_key_callback[i] != p_callback)
            continue;

        btm_cb.p_link_key_callback[i] = NULL;
        return TRUE;
    }

    return FALSE;
}

BOOLEAN BTM_SecAddRmtNameNotifyCallback(tBTM_RMT_NAME_CALLBACK* p_callback) {
    int i;

    for (i = 0; i < BTM_SEC_MAX_RMT_NAME_CALLBACKS; ++i) {
        if (btm_cb.p_rmt_name_callback[i])
            continue;

        btm_cb.p_rmt_name_callback[i] = p_callback;
        return TRUE;
    }

    return FALSE;
}

BOOLEAN BTM_SecDeleteRmtNameNotifyCallback(tBTM_RMT_NAME_CALLBACK* p_callback) {
    int i;

    for (i = 0; i < BTM_SEC_MAX_RMT_NAME_CALLBACKS; ++i) {
        if (btm_cb.p_rmt_name_callback[i] != p_callback)
            continue;

        btm_cb.p_rmt_name_callback[i] = NULL;
        return TRUE;
    }

    return FALSE;
}

void BTM_SecSetConnectFilterCallback(tBTM_FILTER_CB* p_callback) {
    btm_cb.p_conn_filter_cb = p_callback;
}

void BTM_SetSecurityMode(UINT8 security_mode) {
    switch (security_mode) {
    case BTM_SEC_MODE_NONE:
    case BTM_SEC_MODE_SERVICE:
    case BTM_SEC_MODE_LINK:
        break;

    default:
        BTM_TRACE(ERROR, "BTM_SetSecurityMode: mode:%d", security_mode);
        return;
    }

    if (btm_cb.security_mode == BTM_SEC_MODE_LINK && security_mode != BTM_SEC_MODE_LINK) {
        BTM_TRACE(DEBUG, "BTM_SetSecurityMode: Authen Enable -> FALSE");

        btsnd_hcic_write_auth_enable(FALSE);
        btsnd_hcic_write_encr_mode(HCI_ENCRYPT_MODE_DISABLED);
    }

    if (btm_cb.security_mode != BTM_SEC_MODE_LINK && security_mode == BTM_SEC_MODE_LINK) {
        BTM_TRACE(DEBUG, "BTM_SetSecurityMode: Authen Enable -> TRUE");

        btsnd_hcic_write_auth_enable(TRUE);
        btsnd_hcic_write_encr_mode(HCI_ENCRYPT_MODE_POINT_TO_POINT);
    }

    btm_cb.security_mode = security_mode;
}

void BTM_SetPinType(UINT8 pin_type, PIN_CODE pin_code, UINT8 pin_code_len) {
    BTM_TRACE(API, "BTM_SetPinType: pin type %d [variable-0, fixed-1], code %s, length %d", pin_type, pin_code, pin_code_len);

    if (btm_cb.devcb.state > BTM_DEV_STATE_WAIT_AFTER_RESET)
        btsnd_hcic_write_pin_type(pin_type);

    btm_cb.cfg.pin_type = pin_type;
    btm_cb.cfg.pin_code_len = pin_code_len;
    memcpy(btm_cb.cfg.pin_code, pin_code, pin_code_len);
}

void BTM_SetPairableMode(BOOLEAN allow_pairing, BOOLEAN connect_only_paired) {
    btm_cb.pairing_disabled = !allow_pairing;
    btm_cb.connect_only_paired = connect_only_paired;
}

BOOLEAN BTM_SetSecurityLevel(BOOLEAN is_originator, char* p_name, UINT8 service_id, UINT8 sec_level, UINT16 psm, UINT32 mx_proto_id,
                             UINT32 mx_chan_id) {
    tBTM_SEC_SERV_REC* p_srec;
    UINT16 index;
    UINT16 first_unused_record = BTM_NO_AVAIL_SEC_SERVICES;
    BOOLEAN record_allocated = FALSE;

    p_srec = btm_cb.sec_serv_rec;

    /* explicitly post-increment */
    for (index = 0; index < BTM_SEC_MAX_SERVICE_RECORDS; index++, p_srec++) {
        if (p_srec->security_flags & BTM_SEC_IN_USE) {
            if (p_srec->psm != psm)
                continue;

            if (p_srec->mx_proto_id != mx_proto_id)
                continue;

            if (service_id != p_srec->service_id)
                continue;

            if (strncmp(p_name, (char*)p_srec->orig_service_name, BTM_SEC_SERVICE_NAME_LEN) != 0 &&
                strncmp(p_name, (char*)p_srec->term_service_name, BTM_SEC_SERVICE_NAME_LEN) != 0) {
                continue;
            }

            record_allocated = TRUE;
            break;
        }

        if (!record_allocated) {
            record_allocated = TRUE;
            first_unused_record = index;
        }
    }

    if (record_allocated) {
        if (index >= BTM_SEC_MAX_SERVICE_RECORDS) {
            index = first_unused_record;
            p_srec = &btm_cb.sec_serv_rec[index];
        }

        p_srec->psm = psm;
        p_srec->service_id = service_id;
        p_srec->mx_proto_id = mx_proto_id;

        if (is_originator) {
            p_srec->orig_mx_chan_id = mx_chan_id;
            p_srec->security_flags &= ~0x38;

            BCM_STRNCPY_S((char*)p_srec->orig_service_name, sizeof p_srec->orig_service_name, p_name, BTM_SEC_SERVICE_NAME_LEN);
        } else {
            p_srec->term_mx_chan_id = mx_chan_id;
            p_srec->security_flags &= ~0x07;

            BCM_STRNCPY_S((char*)p_srec->term_service_name, sizeof p_srec->term_service_name, p_name, BTM_SEC_SERVICE_NAME_LEN);
        }

        p_srec->security_flags |= (UINT8)(sec_level | BTM_SEC_IN_USE);

        BTM_TRACE(API,
                  "BTM_SEC_REG[%d]: id %d, is_orig %d, psm 0x%04x, proto_id "
                  "%d, chan_id %d",
                  index, service_id, is_originator, psm, mx_proto_id, mx_chan_id);

        BTM_TRACE(API,
                  "               : sec: 0x%x, service name [%s] (up to %d "
                  "chars saved)",
                  p_srec->security_flags, p_name, BTM_SEC_SERVICE_NAME_LEN);
    } else {
        BTM_TRACE(WARNING, "BTM_SEC_REG: Out of Service Records (%d)", 24);
    }

    return record_allocated;
}

UINT8 BTM_SecClrService(UINT8 service_id) {
    tBTM_SEC_SERV_REC* p_srec = btm_cb.sec_serv_rec;
    UINT8 num_freed = 0;
    int i;

    /* explicitly post-increment */
    for (i = 0; i < BTM_SEC_MAX_SERVICE_RECORDS; i++, p_srec++) {
        if (!(p_srec->security_flags & BTM_SEC_IN_USE))
            continue;

        if (p_srec->psm == BT_PSM_SDP)
            continue;

        if (service_id && service_id != p_srec->service_id)
            continue;

        p_srec->security_flags = 0;

        ++num_freed;
    }

    return num_freed;
}

void BTM_PINCodeReply(BD_ADDR bd_addr, UINT8 res, UINT8 pin_len, UINT8* p_pin, UINT32* trusted_mask) {
    tBTM_SEC_DEV_REC* p_dev_rec;
    int trst;

    if (!btm_cb.sec_pin_code_req_tle.param)
        return;

    btu_stop_timer(&btm_cb.sec_pin_code_req_tle);

    btm_cb.sec_pin_code_req_tle.param = (TIMER_PARAM_TYPE)0;

    memset(btm_cb.sec_pin_code_req_bd_addr, 0xff, BD_ADDR_LEN);

    if (res) {
        btm_cb.acl_disc_reason = 14;

        btsnd_hcic_pin_code_neg_reply(bd_addr);
    } else {
        btm_cb.acl_disc_reason = 0;

        btsnd_hcic_pin_code_req_reply(bd_addr, pin_len, p_pin);

        if ((p_dev_rec = btm_find_dev(bd_addr)) != NULL)
            BTM_SEC_COPY_TRUSTED_DEVICE(trusted_mask, p_dev_rec->trusted_mask);
    }
}

void BTM_DeviceAuthorized(BD_ADDR bd_addr, UINT8 res, UINT32* trusted_mask) {
    tBTM_SEC_DEV_REC* p_dev_rec;

    if ((p_dev_rec = btm_find_dev(bd_addr)) == NULL) {
        BTM_TRACE(WARNING,
                  "Security Manager: Attempting Authorization of Unknown "
                  "Device Address [%02x%02x%02x%02x%02x%02x]",
                  bd_addr[0], bd_addr[1], bd_addr[2], bd_addr[3], bd_addr[4], bd_addr[5]);
        return;
    }

    BTM_TRACE(EVENT, "Security Manager: authorized status:%d State:%d", res, p_dev_rec ? p_dev_rec->sec_state : 0);

    if (res == BTM_SUCCESS) {
        p_dev_rec->sec_flags |= BTM_SEC_AUTHORIZED;

        BTM_SEC_COPY_TRUSTED_DEVICE(trusted_mask, p_dev_rec->trusted_mask);
    }

    if (p_dev_rec->sec_state != BTM_SEC_STATE_AUTHORIZING)
        return;

    p_dev_rec->sec_state = BTM_SEC_STATE_IDLE;

    if (res != BTM_SUCCESS) {
        if (p_dev_rec->p_callback) {
            btm_clr_client_scn(p_dev_rec);

            (*p_dev_rec->p_callback)(bd_addr, p_dev_rec->p_ref_data, res);
            p_dev_rec->p_callback = NULL;
        }

        return;
    }

    if ((res = btm_sec_execute_procedure(p_dev_rec)) != BTM_CMD_STARTED) {
        if (p_dev_rec->p_callback) {
            btm_clr_client_scn(p_dev_rec);

            (*p_dev_rec->p_callback)(bd_addr, p_dev_rec->p_ref_data, res);
            p_dev_rec->p_callback = NULL;
        }
    }
}

tBTM_STATUS BTM_SecBond(BD_ADDR bd_addr, UINT8 pin_len, UINT8* p_pin, UINT32* trusted_mask) {
    tBTM_SEC_DEV_REC* p_dev_rec;
    tBTM_INQ_INFO* p_inq_info;
    UINT8* p_features;
    BOOLEAN allow_switch;
    BOOLEAN result;

    BTM_TRACE(API, "BTM_SecBond BDA: %02x:%02x:%02x:%02x:%02x:%02x", bd_addr[0], bd_addr[1], bd_addr[2], bd_addr[3], bd_addr[4], bd_addr[5]);

    if (pin_len > 16) {
        BTM_TRACE(ERROR, "BTM_SecBond: Illegal Pin len:%d", pin_len);
        return 5;
    }

    p_dev_rec = btm_find_dev(bd_addr);
    if (!p_dev_rec) {
        p_dev_rec = btm_sec_alloc_dev(bd_addr);
        if (!p_dev_rec) {
            BTM_TRACE(ERROR, "BTM_SecBond: no device block");

            return 3;
        }
    }

    if (p_dev_rec->hci_handle != BTM_SEC_INVALID_HANDLE && p_dev_rec->sec_flags & BTM_SEC_AUTHENTICATED) {
        BTM_TRACE(WARNING, "BTM_SecBond -> Already Paired");

        return 0;
    }

    if (BTM_DeleteStoredLinkKey(bd_addr, NULL) != BTM_SUCCESS)
        return BTM_NO_RESOURCES;

    p_dev_rec->security_required = 0x50;
    p_dev_rec->is_originator = TRUE;
    BTM_SEC_COPY_TRUSTED_DEVICE(trusted_mask, p_dev_rec->trusted_mask);

    btm_cb.pin_code_len = pin_len;
    if (pin_len)
        memcpy(btm_cb.pin_code, p_pin, pin_len);

    p_dev_rec->sec_flags &= ~(BTM_SEC_LINK_KEY_KNOWN | BTM_SEC_AUTHENTICATED | BTM_SEC_ENCRYPTED);

    if ((p_dev_rec->dev_class[1] & BTM_COD_MAJOR_CLASS_MASK) == BTM_COD_MAJOR_PERIPHERAL && p_dev_rec->dev_class[2] & BTM_COD_MINOR_KEYBOARD &&
        btm_cb.cfg.pin_type != HCI_PIN_TYPE_FIXED) {
        btm_cb.pin_type_changed = TRUE;
        btsnd_hcic_write_pin_type(HCI_PIN_TYPE_FIXED);
    }

    btm_cb.disconnect_after_bond_reason = 0;

    if (p_dev_rec->hci_handle != BTM_SEC_INVALID_HANDLE) {
        p_dev_rec->sec_flags &= ~0x10;

        if (!btm_sec_start_authentication(p_dev_rec)) {
            return BTM_NO_RESOURCES;
        } else {
            btm_cb.bond_created_connection = FALSE;

            return BTM_CMD_STARTED;
        }
    }

    if (btm_cb.security_mode != 3) {
        BTM_TRACE(DEBUG, "BTM_SecBond: Authen Enable -> TRUE");

        if (!btsnd_hcic_write_auth_enable(TRUE)) {
            BTM_TRACE(ERROR, "BTM_SecBond: no buffer");
            return 3;
        }

        btm_cb.security_mode_changed = TRUE;
    }

    btm_cb.bond_created_connection = TRUE;

    p_features = BTM_ReadLocalFeatures();

    if (p_features[0] & 0x20)
        allow_switch = TRUE;
    else
        allow_switch = FALSE;

    p_dev_rec->sec_acl_connected = 0;

    if ((p_inq_info = BTM_InqDbRead(bd_addr)) != NULL) {
        result = btsnd_hcic_create_conn(bd_addr, 24, p_inq_info->results.page_scan_rep_mode, p_inq_info->results.page_scan_mode,
                                        p_inq_info->results.clock_offset | 0x8000, allow_switch);
    } else {
        result = btsnd_hcic_create_conn(bd_addr, 24, 1, 0, p_dev_rec->clock_offset, allow_switch);
    }

    if (result)
        return 1;

    BTM_TRACE(ERROR, "BTM_SecBond: no buffer");

    btm_restore_mode();

    return 3;
}

tBTM_STATUS BTM_SecUseMasterLinkKey(BOOLEAN use_master_key) {
    return btsnd_hcic_master_link_key(use_master_key) ? BTM_SUCCESS : BTM_NO_RESOURCES;
}

void BTM_SetMasterKeyCompCback(tBTM_MKEY_CALLBACK* mkey_cback) {
    btm_cb.mkey_cback = mkey_cback;
}

tBTM_STATUS BTM_SecGetDeviceLinkKey(BD_ADDR bd_addr, LINK_KEY link_key) {
    tBTM_SEC_DEV_REC* p_dev_rec;

    if (((p_dev_rec = btm_find_dev(bd_addr)) != NULL) && p_dev_rec->sec_flags & BTM_SEC_LINK_KEY_KNOWN) {
        memcpy(link_key, p_dev_rec->link_key, LINK_KEY_LEN);
        return BTM_SUCCESS;
    }

    return BTM_UNKNOWN_ADDR;
}

tBTM_STATUS BTM_SetEncryption(BD_ADDR bd_addr, tBTM_SEC_CBACK* p_callback, void* p_ref_data) {
    tBTM_SEC_DEV_REC* p_dev_rec;
    tBTM_STATUS rc;

    p_dev_rec = btm_find_dev(bd_addr);
    if (!p_dev_rec || (p_dev_rec->hci_handle == BTM_SEC_INVALID_HANDLE)) {
        BTM_TRACE(WARNING, "Security Manager: BTM_SetEncryption not connected");

        if (p_callback)
            (*p_callback)(bd_addr, p_ref_data, BTM_WRONG_MODE);

        return BTM_WRONG_MODE;
    }

    if ((p_dev_rec->sec_flags & (BTM_SEC_AUTHENTICATED | BTM_SEC_ENCRYPTED)) == (BTM_SEC_AUTHENTICATED | BTM_SEC_ENCRYPTED)) {
        BTM_TRACE(EVENT, "Security Manager: BTM_SetEncryption already encrypted");

        if (p_callback)
            (*p_callback)(bd_addr, p_ref_data, BTM_SUCCESS);

        return BTM_SUCCESS;
    }

    if (p_dev_rec->p_callback) {
        BTM_TRACE(WARNING, "Security Manager: BTM_SetEncryption busy");

        if (p_callback)
            (*p_callback)(bd_addr, p_ref_data, BTM_BUSY);

        return BTM_BUSY;
    }

    p_dev_rec->p_callback = p_callback;
    p_dev_rec->p_ref_data = p_ref_data;
    p_dev_rec->security_required |= BTM_SEC_IN_AUTHENTICATE | BTM_SEC_IN_ENCRYPT;
    p_dev_rec->is_originator = FALSE;

    BTM_TRACE(API,
              "Security Manager: BTM_SetEncryption Handle:%d State:%d "
              "Flags:0x%x Required:0x%x",
              p_dev_rec->hci_handle, p_dev_rec->sec_state, p_dev_rec->sec_flags, p_dev_rec->security_required);

    if ((rc = btm_sec_execute_procedure(p_dev_rec)) != BTM_CMD_STARTED && p_callback) {
        (*p_callback)(bd_addr, p_dev_rec->p_ref_data, rc);
        p_dev_rec->p_callback = NULL;
    }

    return rc;
}

tBTM_STATUS btm_sec_l2cap_access_req(BD_ADDR bd_addr, UINT16 psm, UINT16 handle, BOOLEAN is_originator, tBTM_SEC_CALLBACK* p_callback) {
    tBTM_SEC_DEV_REC* p_dev_rec;
    tBTM_SEC_SERV_REC* p_serv_rec;
    UINT8 security_required;
    tBTM_STATUS rc;

    p_dev_rec = btm_find_dev(bd_addr);
    if (!p_dev_rec) {
        p_dev_rec = btm_sec_alloc_dev(bd_addr);
        if (!p_dev_rec) {
            BTM_TRACE(WARNING, "Security Manager: l2cap_access_req PSM:%d no resources", psm);

            if (p_callback)
                (*p_callback)(bd_addr, NULL, 3);

            return 3;
        }
    }

    p_dev_rec->hci_handle = handle;

    p_serv_rec = btm_sec_find_first_serv(psm);
    if (!p_serv_rec) {
        BTM_TRACE(WARNING,
                  "Security Manager: l2cap_access_req PSM:%d no application "
                  "registerd",
                  psm);

        if (p_callback)
            (*p_callback)(bd_addr, NULL, BTM_MODE_UNSUPPORTED);

        return BTM_MODE_UNSUPPORTED;
    }

    if (!is_originator || (is_originator && !p_dev_rec->p_cur_service))
        p_dev_rec->p_cur_service = p_serv_rec;

    security_required = p_serv_rec->security_flags;

    p_serv_rec = btm_sec_find_next_serv(p_serv_rec);
    if (p_serv_rec) {
        BTM_TRACE(EVENT,
                  "Security Manager: l2cap_access_req PSM:%d postponed for "
                  "multiplexer",
                  psm);

        if (p_callback)
            (*p_callback)(bd_addr, NULL, 0);

        return 0;
    }

    p_dev_rec->p_callback = p_callback;
    p_dev_rec->p_ref_data = NULL;
    p_dev_rec->security_required = security_required;
    p_dev_rec->is_originator = is_originator;

    BTM_TRACE(EVENT,
              "Security Manager: l2cap_access_req PSM:%d Handle:%d State:%d "
              "Flags:0x%x Required:0x%x",
              psm, handle, p_dev_rec->sec_state, p_dev_rec->sec_flags, security_required);

    if (!is_originator && security_required & 1) {
        p_dev_rec->sec_flags &= ~1;

        BTM_TRACE(EVENT, "Security Manager: trusted:0x%04x%04x Flags:0x%x", p_dev_rec->trusted_mask[1], p_dev_rec->trusted_mask[0],
                  p_dev_rec->sec_flags);
    }

    if (((rc = btm_sec_execute_procedure(p_dev_rec)) != 1) && p_callback) {
        (*p_callback)(bd_addr, p_dev_rec->p_ref_data, rc);
        p_dev_rec->p_callback = NULL;
    }

    return rc;
}

void btm_sec_mx_access_request(BD_ADDR bd_addr, UINT16 psm, BOOLEAN is_originator, UINT32 mx_proto_id, UINT32 mx_chan_id,
                               tBTM_SEC_CALLBACK* p_callback, void* p_ref_data) {
    tBTM_SEC_DEV_REC* p_dev_rec;
    tBTM_SEC_SERV_REC* p_serv_rec;
    tBTM_STATUS rc;

    p_dev_rec = btm_find_dev(bd_addr);
    if (!p_dev_rec) {
        p_dev_rec = btm_sec_alloc_dev(bd_addr);
        if (!p_dev_rec) {
            if (p_callback)
                (*p_callback)(bd_addr, p_ref_data, 3);

            return;
        }
    }

    p_serv_rec = btm_sec_find_mx_serv(is_originator, psm, mx_proto_id, mx_chan_id);

    if (!p_serv_rec) {
        if (p_callback)
            (*p_callback)(bd_addr, p_ref_data, BTM_MODE_UNSUPPORTED);

        BTM_TRACE(ERROR, "Security Manager: MX service not found PSM:%d Proto:%d SCN:%d", psm, mx_proto_id, mx_chan_id);
        return;
    }

    p_dev_rec->p_cur_service = p_serv_rec;
    p_dev_rec->security_required = p_serv_rec->security_flags;

    p_dev_rec->is_originator = is_originator;
    p_dev_rec->p_callback = p_callback;
    p_dev_rec->p_ref_data = p_ref_data;

    p_dev_rec->sec_flags &= ~(BTM_SEC_AUTHORIZED);

    if ((rc = btm_sec_execute_procedure(p_dev_rec)) != BTM_CMD_STARTED) {
        if (p_callback) {
            btm_clr_client_scn(p_dev_rec);
            (*p_callback)(bd_addr, p_ref_data, (UINT8)rc);
            p_dev_rec->p_callback = NULL;
        }
    }
}

void btm_sec_change_link_key(UINT16 handle) {
    tBTM_SEC_DEV_REC* p_dev_rec = btm_find_dev_by_handle(handle);

    if (!p_dev_rec)
        return;

    if (p_dev_rec->link_key_changed)
        return;

    if ((p_dev_rec->sec_flags & 0x06) != 0x06)
        return;

    if (!btsnd_hcic_change_link_key(handle))
        return;

    p_dev_rec->link_key_changed = TRUE;
}

void btm_sec_conn_req(BD_ADDR bda, DEV_CLASS dc) {
    void* p_buf;
    tBTM_SEC_DEV_REC* p_dev_rec;

    if (btm_cb.connect_only_paired) {
        p_dev_rec = btm_find_dev(bda);

        if (!p_dev_rec || !(p_dev_rec->sec_flags & 0x10)) {
            BTM_TRACE(EVENT, "Security Manager: connect request from not paired device");

            if ((p_buf = GKI_getpoolbuf(2)) != NULL)
                btsnd_hcic_reject_conn(p_buf, bda, HCI_ERR_HOST_REJECT_DEVICE);

            return;
        }
    }

    if (btm_cb.p_conn_filter_cb) {
        if (!(*btm_cb.p_conn_filter_cb)(bda, dc)) {
            if ((p_buf = GKI_getpoolbuf(2)) != NULL)
                btsnd_hcic_reject_conn(p_buf, bda, HCI_ERR_HOST_REJECT_DEVICE);

            return;
        }
    }

    memcpy(btm_cb.connecting_bda, bda, BD_ADDR_LEN);
    memcpy(btm_cb.connecting_dc, dc, DEV_CLASS_LEN);

    l2c_link_hci_conn_req(bda);
}

void btm_sec_init(UINT8 sec_mode) {
    btm_cb.security_mode = sec_mode;
    memset(btm_cb.sec_pin_code_req_bd_addr, 0xff, BD_ADDR_LEN);
}

void btm_sec_dev_reset(void) {
    if (btm_cb.security_mode != BTM_SEC_MODE_LINK)
        return;

    btsnd_hcic_write_auth_enable(TRUE);
    btsnd_hcic_write_encr_mode(HCI_ENCRYPT_MODE_POINT_TO_POINT);
}

void btm_sec_abort_access_req(BD_ADDR bd_addr) {
    tBTM_SEC_DEV_REC* p_dev_rec = btm_find_dev(bd_addr);

    if (!p_dev_rec)
        return;

    if (btm_cb.api.p_abort_callback) {
        (*btm_cb.api.p_abort_callback)(bd_addr, p_dev_rec->dev_class, p_dev_rec->sec_bd_name);
    }

    if (p_dev_rec->sec_state != BTM_SEC_STATE_AUTHORIZING && p_dev_rec->sec_state != BTM_SEC_STATE_AUTHENTICATING) {
        return;
    }

    p_dev_rec->sec_state = BTM_SEC_STATE_IDLE;

    btm_clr_client_scn(p_dev_rec);

    p_dev_rec->p_callback = NULL;
}

void btm_sec_rmt_name_request_complete(BD_ADDR_PTR p_bd_addr, BD_NAME_PTR p_bd_name, UINT8 status) {
    tBTM_SEC_DEV_REC* p_dev_rec;
    int i;

    if (p_bd_addr) {
        p_dev_rec = btm_find_dev(p_bd_addr);
    } else {
        p_dev_rec = btm_cb.sec_dev_rec;

        /* explicitly post-increment */
        for (i = 0; i < 16; i++, p_dev_rec++) {
            if (!(p_dev_rec->sec_flags & 0x80))
                continue;

            if (p_dev_rec->sec_state == 3)
                break;
        }

        if (i == 16)
            p_dev_rec = NULL;
    }

    BTM_TRACE(EVENT, "Security Manager: rmt_name_complete status:%d State:%d", status, p_dev_rec ? p_dev_rec->sec_state : 0);

    if (!p_dev_rec)
        return;

    if (status == 0) {
        memcpy(p_dev_rec->sec_bd_name, p_bd_name, 64);
        p_dev_rec->sec_flags |= 0x08;

        for (i = 0; i < BTM_SEC_MAX_RMT_NAME_CALLBACKS; ++i) {
            if (btm_cb.p_rmt_name_callback[i]) {
                (*btm_cb.p_rmt_name_callback[i])(p_bd_addr, p_dev_rec->dev_class, p_dev_rec->sec_bd_name);
            }
        }
    }

    if (p_dev_rec->link_key_not_sent) {
        if (p_dev_rec->security_required & 0x40 && !p_dev_rec->sec_acl_connected) {
            return;
        }

        p_dev_rec->link_key_not_sent = FALSE;

        if (btm_cb.api.p_link_key_callback) {
            (*btm_cb.api.p_link_key_callback)(p_dev_rec->bd_addr, p_dev_rec->dev_class, p_dev_rec->sec_bd_name, p_dev_rec->link_key,
                                              p_dev_rec->link_key_type);
        }

        for (i = 0; i < BTM_SEC_MAX_LINK_KEY_CALLBACKS; ++i) {
            if (btm_cb.p_link_key_callback[i]) {
                (*btm_cb.p_link_key_callback[i])(p_dev_rec->bd_addr, p_dev_rec->dev_class, p_dev_rec->sec_bd_name, p_dev_rec->link_key,
                                                 p_dev_rec->link_key_type);
            }
        }
    }

    if (p_dev_rec->security_required & 0x40 && p_dev_rec->sec_flags & 0x02) {
        p_dev_rec->security_required &= ~0x50;

        btsnd_hcic_disconnect(p_dev_rec->hci_handle, 19);
        return;
    }

    if (p_dev_rec->sec_state != 3)
        return;

    p_dev_rec->sec_state = 0;

    if (status != 0) {
        if (p_dev_rec->p_callback) {
            (*p_dev_rec->p_callback)(p_dev_rec->bd_addr, p_dev_rec->p_ref_data, 10);

            p_dev_rec->p_callback = NULL;
        }

        return;
    }

    status = btm_sec_execute_procedure(p_dev_rec);
    if (status != 1) {
        if (p_dev_rec->p_callback) {
            (*p_dev_rec->p_callback)(p_dev_rec->bd_addr, p_dev_rec->p_ref_data, status);

            p_dev_rec->p_callback = NULL;
        }
    }
}

void btm_sec_auth_complete(UINT16 handle, UINT8 status) {
    tBTM_SEC_DEV_REC* p_dev_rec = btm_find_dev_by_handle(handle);

    BTM_TRACE(EVENT, "Security Manager: auth_complete status:%d State:%d", status, p_dev_rec ? p_dev_rec->sec_state : 0);

    btm_restore_mode();

    if (btm_cb.pin_type_changed) {
        btm_cb.pin_type_changed = FALSE;
        btsnd_hcic_write_pin_type(btm_cb.cfg.pin_type);
    }

    if (p_dev_rec && p_dev_rec->security_required & 0x40 && !btm_cb.bond_created_connection) {
        p_dev_rec->security_required &= ~0x50;

        if (btm_cb.disconnect_after_bond_reason != 0)
            btm_sec_disconnect(handle, btm_cb.disconnect_after_bond_reason);
    }

    if (!p_dev_rec)
        return;

    if (p_dev_rec->sec_state != 1) {
        if (btm_cb.api.p_auth_complete_callback && status != 0) {
            (*btm_cb.api.p_auth_complete_callback)(p_dev_rec->bd_addr, p_dev_rec->dev_class, p_dev_rec->sec_bd_name, status);
        }

        return;
    }

    if (status == 12 && (p_dev_rec->sec_flags & 0x06) == 6)
        status = 0;

    if (btm_cb.api.p_auth_complete_callback) {
        (*btm_cb.api.p_auth_complete_callback)(p_dev_rec->bd_addr, p_dev_rec->dev_class, p_dev_rec->sec_bd_name, status);
    }

    p_dev_rec->sec_state = 0;

    if (p_dev_rec->security_required & 0x40) {
        p_dev_rec->security_required &= ~0x50;

        btsnd_hcic_disconnect(p_dev_rec->hci_handle, 19);
        return;
    }

    if (status != 0) {
        if (p_dev_rec->p_callback) {
            btm_clr_client_scn(p_dev_rec);

            (*p_dev_rec->p_callback)(p_dev_rec->bd_addr, p_dev_rec->p_ref_data, 10);

            p_dev_rec->p_callback = NULL;
        }

        btsnd_hcic_disconnect(p_dev_rec->hci_handle, 5);
        return;
    }

    p_dev_rec->sec_flags |= 0x02;

    status = btm_sec_execute_procedure(p_dev_rec);
    if (status != 1 && p_dev_rec->p_callback) {
        btm_clr_client_scn(p_dev_rec);

        (*p_dev_rec->p_callback)(p_dev_rec->bd_addr, p_dev_rec->p_ref_data, status);

        p_dev_rec->p_callback = NULL;
    }
}

void btm_sec_mkey_comp_event(UINT16 handle, UINT8 status, UINT8 key_flg) {
    tBTM_SEC_DEV_REC* p_dev_rec = btm_find_dev_by_handle(handle);
    UINT8 bd_addr[BD_ADDR_LEN] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

    BTM_TRACE(EVENT, "Security Manager: mkey comp status:%d State:%d", status, (p_dev_rec) ? p_dev_rec->sec_state : 0);

    if (btm_cb.mkey_cback) {
        if (!p_dev_rec)
            (btm_cb.mkey_cback)(bd_addr, status, key_flg);
        else
            (btm_cb.mkey_cback)(p_dev_rec->bd_addr, status, key_flg);
    }
}

void btm_sec_encrypt_change(UINT16 handle, UINT8 status, UINT8 encr_enable) {
    tBTM_SEC_DEV_REC* p_dev_rec = btm_find_dev_by_handle(handle);

    BTM_TRACE(EVENT, "Security Manager: encrypt_change status:%d State:%d", status, p_dev_rec ? p_dev_rec->sec_state : 0);

    if (!p_dev_rec)
        return;

    if (status == HCI_ERR_LMP_ERR_TRANS_COLLISION) {
        btm_cb.p_collided_dev_rec = p_dev_rec;
        btm_cb.sec_collision_tle.param = (TIMER_PARAM_TYPE)&btm_sec_collision_timeout;
        btu_start_timer(&btm_cb.sec_collision_tle, 22, 1);

        return;
    }

    if (status != 0) {
        if (p_dev_rec->p_callback) {
            (*p_dev_rec->p_callback)(p_dev_rec->bd_addr, p_dev_rec->p_ref_data, 10);
            p_dev_rec->p_callback = NULL;
        }

        return;
    }

    p_dev_rec->sec_flags |= BTM_SEC_AUTHENTICATED | BTM_SEC_ENCRYPTED;

    if (p_dev_rec->sec_state != 2)
        return;

    p_dev_rec->sec_state = BTM_SEC_STATE_IDLE;

    status = btm_sec_execute_procedure(p_dev_rec);

    if (status != BTM_CMD_STARTED && p_dev_rec->p_callback) {
        (*p_dev_rec->p_callback)(p_dev_rec->bd_addr, p_dev_rec->p_ref_data, status);
        p_dev_rec->p_callback = NULL;
    }
}

BOOLEAN btm_sec_is_bonding(UINT16 handle) {
    tBTM_SEC_DEV_REC* p_dev_rec = btm_find_dev_by_handle(handle);

    if (p_dev_rec && p_dev_rec->security_required & 0x40)
        return TRUE;

    return FALSE;
}

void btm_sec_connected(BD_ADDR bda, UINT16 handle, UINT8 status, UINT8 enc_mode) {
    tBTM_SEC_DEV_REC* p_dev_rec = btm_find_dev(bda);
    UINT8 res;
    int i;

    BTM_TRACE(EVENT, "Security Manager: btm_sec_connected handle:%d status:%d enc_mode:%d", handle, status, enc_mode);

    if (!p_dev_rec) {
        if (status == 0) {
            p_dev_rec = btm_sec_alloc_dev(bda);
            if (!p_dev_rec)
                return;
        } else {
            return;
        }
    } else {
        p_dev_rec->timestamp = btm_cb.dev_rec_count++;
        p_dev_rec->sec_acl_connected = 1;
    }

    if (p_dev_rec->security_required & 0x40) {
        btm_restore_mode();

        if (btm_cb.pin_type_changed) {
            btm_cb.pin_type_changed = FALSE;
            btsnd_hcic_write_pin_type(btm_cb.cfg.pin_type);
        }
    }

    if (status != 0) {
        if (0 || status == 5 || status == 6 || status == 14 || status == 24 || status == 0x26 || status == 0x29 || status == 0x25 || status == 0x17 ||
            status == 0x22 || p_dev_rec->security_required & 0x40) {
            p_dev_rec->security_required &= ~0x50;
            p_dev_rec->sec_flags &= ~0x10;

            if (btm_cb.sec_pin_code_req_tle.param) {
                btu_stop_timer(&btm_cb.sec_pin_code_req_tle);
                btm_cb.sec_pin_code_req_tle.param = (TIMER_PARAM_TYPE)0;
                status = 14;
            }

            if (btm_cb.acl_disc_reason == 14)
                status = 14;

            if (btm_cb.api.p_auth_complete_callback)
                (*btm_cb.api.p_auth_complete_callback)(p_dev_rec->bd_addr, p_dev_rec->dev_class, p_dev_rec->sec_bd_name, status);
        } else {
            if (p_dev_rec->p_callback) {
                btm_clr_client_scn(p_dev_rec);

                if (status == 8 || status == 31 || status == 4) {
                    (*p_dev_rec->p_callback)(p_dev_rec->bd_addr, p_dev_rec->p_ref_data, 8);
                } else {
                    (*p_dev_rec->p_callback)(p_dev_rec->bd_addr, p_dev_rec->p_ref_data, 10);
                }
            }
        }

        return;
    }

    if (p_dev_rec->security_required & 0x40 && p_dev_rec->link_key_not_sent) {
        p_dev_rec->link_key_not_sent = FALSE;

        if (btm_cb.api.p_link_key_callback) {
            (*btm_cb.api.p_link_key_callback)(p_dev_rec->bd_addr, p_dev_rec->dev_class, p_dev_rec->sec_bd_name, p_dev_rec->link_key,
                                              p_dev_rec->link_key_type);
        }

        for (i = 0; i < BTM_SEC_MAX_LINK_KEY_CALLBACKS; ++i) {
            if (btm_cb.p_link_key_callback[i]) {
                (*btm_cb.p_link_key_callback[i])(p_dev_rec->bd_addr, p_dev_rec->dev_class, p_dev_rec->sec_bd_name, p_dev_rec->link_key,
                                                 p_dev_rec->link_key_type);
            }
        }

        if (btm_cb.api.p_auth_complete_callback) {
            (*btm_cb.api.p_auth_complete_callback)(p_dev_rec->bd_addr, p_dev_rec->dev_class, p_dev_rec->sec_bd_name, 0);
        }

        p_dev_rec->security_required &= ~0x50;

        btsnd_hcic_disconnect(handle, 19);

        return;
    }

    p_dev_rec->hci_handle = handle;
    p_dev_rec->sec_flags &= ~0x07;

    if (enc_mode)
        p_dev_rec->sec_flags |= 0x06;

    if (btm_cb.security_mode == 3)
        p_dev_rec->sec_flags |= 0x02;

    p_dev_rec->link_key_changed = FALSE;

    if (p_dev_rec->sec_flags & 0x08 && !p_dev_rec->is_originator)
        return;

    if ((res = btm_sec_execute_procedure(p_dev_rec)) != 1) {
        if (p_dev_rec->p_callback) {
            btm_clr_client_scn(p_dev_rec);

            (*p_dev_rec->p_callback)(p_dev_rec->bd_addr, p_dev_rec->p_ref_data, res);
            p_dev_rec->p_callback = NULL;
        }
    }
}

tBTM_STATUS btm_sec_disconnect(UINT16 handle, UINT8 reason) {
    tBTM_SEC_DEV_REC* p_dev_rec = btm_find_dev_by_handle(handle);
    UINT8 old_state;

    if (!p_dev_rec)
        return 5;

    if (p_dev_rec->security_required & 0x40) {
        btm_cb.disconnect_after_bond_reason = reason;
        return 0;
    }

    if (p_dev_rec->sec_state == 5)
        return 1;

    old_state = p_dev_rec->sec_state;
    p_dev_rec->sec_state = 5;

    if (btsnd_hcic_disconnect(handle, reason))
        return 1;

    p_dev_rec->sec_state = old_state;
    return 3;
}

void btm_sec_disconnected(UINT16 handle, UINT8 reason) {
    tBTM_SEC_DEV_REC* p_dev_rec = btm_find_dev_by_handle(handle);

    if (!p_dev_rec)
        return;

    if (p_dev_rec->security_required & 0x40 && btm_cb.api.p_auth_complete_callback) {
        if (reason == 23) {
            (*btm_cb.api.p_auth_complete_callback)(p_dev_rec->bd_addr, p_dev_rec->dev_class, p_dev_rec->sec_bd_name, 23);
        } else {
            (*btm_cb.api.p_auth_complete_callback)(p_dev_rec->bd_addr, p_dev_rec->dev_class, p_dev_rec->sec_bd_name, 5);
        }
    }

    if (btm_cb.sec_pin_code_req_tle.param && memcmp(btm_cb.sec_pin_code_req_bd_addr, p_dev_rec->bd_addr, 6) == 0) {
        btu_stop_timer(&btm_cb.sec_pin_code_req_tle);
        btm_cb.sec_pin_code_req_tle.param = (TIMER_PARAM_TYPE)0;
        memset(btm_cb.sec_pin_code_req_bd_addr, 0xff, BD_ADDR_LEN);

        if (btm_cb.api.p_auth_complete_callback) {
            (*btm_cb.api.p_auth_complete_callback)(p_dev_rec->bd_addr, p_dev_rec->dev_class, p_dev_rec->sec_bd_name, 14);
        }
    }

    p_dev_rec->hci_handle = 0xffff;
    p_dev_rec->sec_state = 0;
    p_dev_rec->sec_flags &= ~0x07;
    p_dev_rec->security_required &= ~0x7f;
    p_dev_rec->p_callback = NULL;
}

void btm_sec_link_key_notification(BD_ADDR_PTR p_bda, UINT8* p_link_key, UINT8 key_type) {
    tBTM_SEC_DEV_REC* p_dev_rec = btm_find_dev(p_bda);
    int i;

    if (!p_dev_rec) {
        p_dev_rec = btm_sec_alloc_dev(p_bda);
        if (!p_dev_rec)
            return;
    }

    BTM_TRACE(EVENT, "btm_sec_link_key_notification()  BDA: %02x:%02x:%02x:%02x:%02x:%02x", p_bda[0], p_bda[1], p_bda[2], p_bda[3], p_bda[4],
              p_bda[5]);
    BTM_TRACE(EVENT, "                                TYPE: %d", key_type);

    if (btm_cb.devcb.local_version.hci_version < 1)
        p_dev_rec->link_key_type = 0xff;
    else
        p_dev_rec->link_key_type = key_type;

    p_dev_rec->sec_flags |= 0x10;
    memcpy(p_dev_rec->link_key, p_link_key, LINK_KEY_LEN);

    if (!(p_dev_rec->sec_flags & 0x08) || (p_dev_rec->security_required & 0x40 && !p_dev_rec->sec_acl_connected)) {
        p_dev_rec->link_key_not_sent = 1;
        return;
    }

    if (btm_cb.api.p_link_key_callback) {
        (*btm_cb.api.p_link_key_callback)(p_bda, p_dev_rec->dev_class, p_dev_rec->sec_bd_name, p_link_key, p_dev_rec->link_key_type);
    }

    for (i = 0; i < BTM_SEC_MAX_LINK_KEY_CALLBACKS; ++i) {
        if (btm_cb.p_link_key_callback[i]) {
            (*btm_cb.p_link_key_callback[i])(p_bda, p_dev_rec->dev_class, p_dev_rec->sec_bd_name, p_link_key, p_dev_rec->link_key_type);
        }
    }
}

void btm_sec_link_key_request(BD_ADDR_PTR p_bda) {
    tBTM_SEC_DEV_REC* p_dev_rec = btm_find_dev(p_bda);
    LINK_KEY link_key;

    BTM_TRACE(EVENT, "btm_sec_link_key_request()  BDA: %02x:%02x:%02x:%02x:%02x:%02x", p_bda[0], p_bda[1], p_bda[2], p_bda[3], p_bda[4], p_bda[5]);

    if (p_dev_rec && p_dev_rec->sec_flags & 0x10) {
        btsnd_hcic_link_key_req_reply(p_bda, p_dev_rec->link_key);
        return;
    }

    if (!p_dev_rec)
        p_dev_rec = btm_sec_alloc_dev(p_bda);

    if (!(p_dev_rec->security_required & 0x40) && btm_cb.api.p_link_key_req_callback) {
        if (p_dev_rec) {
            if (!(*btm_cb.api.p_link_key_req_callback)(p_bda, p_dev_rec->link_key)) {
                btsnd_hcic_link_key_req_reply(p_bda, p_dev_rec->link_key);
                return;
            }
        } else if (!(*btm_cb.api.p_link_key_req_callback)(p_bda, link_key)) {
            btsnd_hcic_link_key_req_reply(p_bda, link_key);
            return;
        }
    }

    btsnd_hcic_link_key_neg_reply(p_bda);
}

void btm_sec_pin_code_request_timeout(void) {
    tBTM_CB* p_cb = &btm_cb;

    BTM_TRACE(EVENT, "btm_sec_pin_code_request_timeout()");

    p_cb->sec_pin_code_req_tle.param = (TIMER_PARAM_TYPE)0;
    memset(btm_cb.sec_pin_code_req_bd_addr, 0xff, BD_ADDR_LEN);

    btsnd_hcic_pin_code_neg_reply(p_cb->connecting_bda);
}

void btm_sec_pin_code_request(BD_ADDR_PTR p_bda) {
    tBTM_SEC_DEV_REC* p_dev_rec;
    tBTM_CB* p_cb = &btm_cb;
    UINT8 send_neg_reply = 0;
    int i = 0;

    BTM_TRACE(EVENT, "btm_sec_pin_code_request()  BDA: %02x:%02x:%02x:%02x:%02x:%02x", p_bda[0], p_bda[1], p_bda[2], p_bda[3], p_bda[4], p_bda[5]);

    if (!p_cb->pairing_disabled && p_cb->cfg.pin_type == 1) {
        p_dev_rec = btm_find_dev(p_bda);

        if (!p_dev_rec) {
            /* explicitly post-increment */
            for (p_dev_rec = btm_cb.sec_dev_rec; i < BTM_SEC_MAX_DEVICE_RECORDS; i++, p_dev_rec++) {
                if (!(p_dev_rec->sec_flags & 0x80))
                    break;
            }
        }

        if (i >= 16) {
            btsnd_hcic_pin_code_neg_reply(p_bda);
        } else {
            btsnd_hcic_pin_code_req_reply(p_bda, p_cb->cfg.pin_code_len, p_cb->cfg.pin_code);
        }

        return;
    }

    p_dev_rec = btm_find_dev(p_bda);
    if (!p_dev_rec)
        p_dev_rec = btm_sec_alloc_dev(p_bda);

    memcpy(btm_cb.sec_pin_code_req_bd_addr, p_bda, 6);

    if (p_dev_rec) {
        if (memcmp(p_bda, p_cb->connecting_bda, 6) == 0)
            memcpy(p_dev_rec->dev_class, p_cb->connecting_dc, 3);

        if (p_dev_rec->security_required & 0x40) {
            BTM_TRACE(EVENT, "btm_sec_pin_code_request bonding sending reply");

            btsnd_hcic_pin_code_req_reply(p_bda, p_cb->pin_code_len, p_cb->pin_code);

            if (btm_cb.security_mode_changed) {
                btm_cb.security_mode_changed = FALSE;
                BTM_TRACE(DEBUG, "btm_sec_pin_code_request: Authen Enable -> %d", btm_cb.security_mode == 3);
                btsnd_hcic_write_auth_enable(btm_cb.security_mode == 3);
            }
        } else {
            if (p_cb->pairing_disabled || !p_cb->api.p_pin_callback ||
                (!p_dev_rec->is_originator && (p_dev_rec->dev_class[1] & 31) == 5 && p_dev_rec->dev_class[2] & 0x40)) {
                send_neg_reply = 1;
            } else {
                memcpy(p_cb->connecting_bda, p_bda, 6);
                memcpy(p_cb->connecting_dc, p_dev_rec->dev_class, 3);

                p_cb->sec_pin_code_req_tle.param = (TIMER_PARAM_TYPE)&btm_sec_pin_code_request_timeout;

                btu_start_timer(&p_cb->sec_pin_code_req_tle, 22, 60);
                (*p_cb->api.p_pin_callback)(p_bda, p_dev_rec->dev_class, p_dev_rec->sec_bd_name);
            }
        }
    } else {
        send_neg_reply = 1;
    }

    if (send_neg_reply) {
        BTM_TRACE(WARNING,
                  "btm_sec_pin_code_request(): Pairing disabled:%d; PIN "
                  "callback:%x, Dev Rec:%x!",
                  p_cb->pairing_disabled, p_cb->api.p_pin_callback, p_dev_rec);

        btsnd_hcic_pin_code_neg_reply(p_bda);
    }
}

void btm_sec_update_clock_offset(UINT16 handle, UINT16 clock_offset) {
    tBTM_SEC_DEV_REC* p_dev_rec;
    tBTM_INQ_INFO* p_inq_info;

    if ((p_dev_rec = btm_find_dev_by_handle(handle)) == NULL)
        return;

    p_dev_rec->clock_offset = clock_offset | BTM_CLOCK_OFFSET_VALID;

    if ((p_inq_info = BTM_InqDbRead(p_dev_rec->bd_addr)) == NULL)
        return;

    p_inq_info->results.clock_offset = clock_offset | BTM_CLOCK_OFFSET_VALID;
}

static tBTM_STATUS btm_sec_execute_procedure(tBTM_SEC_DEV_REC* p_dev_rec) {
    tL2C_LCB* p_lcb;

    BTM_TRACE(EVENT, "btm_sec_execute_procedure: Required:0x%x Flags:0x%x State:%d", p_dev_rec->security_required, p_dev_rec->sec_flags,
              p_dev_rec->sec_state);

    if (p_dev_rec->sec_state)
        return 1;

    if (p_dev_rec->hci_handle == 0xffff) {
        if ((p_lcb = l2cu_allocate_lcb(p_dev_rec->bd_addr)) == NULL) {
            L2CAP_TRACE(WARNING, "L2CAP - no LCB for L2CA_conn_req");
            return 3;
        }

        if (!l2cu_create_conn(p_lcb))
            return 3;

        p_dev_rec->is_originator = TRUE;
        return 1;
    }

    if (!(p_dev_rec->sec_flags & BTM_SEC_NAME_KNOWN) && p_dev_rec->hci_handle != BTM_SEC_INVALID_HANDLE) {
        BTM_TRACE(EVENT, "Security Manager: Start get name");

        if (btm_sec_start_get_name(p_dev_rec))
            return BTM_CMD_STARTED;

        p_dev_rec->sec_flags |= 8;
    }

    if (!(p_dev_rec->sec_flags & BTM_SEC_AUTHENTICATED) &&
        ((p_dev_rec->is_originator && p_dev_rec->security_required & BTM_SEC_OUT_AUTHENTICATE) ||
         (!p_dev_rec->is_originator && p_dev_rec->security_required & BTM_SEC_IN_AUTHENTICATE)) &&
        p_dev_rec->hci_handle != BTM_SEC_INVALID_HANDLE) {
        BTM_TRACE(EVENT, "Security Manager: Start authentication");

        if (!btm_sec_start_authentication(p_dev_rec))
            return BTM_NO_RESOURCES;

        return BTM_CMD_STARTED;
    }

    if (!(p_dev_rec->sec_flags & BTM_SEC_ENCRYPTED) &&
        ((p_dev_rec->is_originator && p_dev_rec->security_required & BTM_SEC_OUT_ENCRYPT) ||
         (!p_dev_rec->is_originator && p_dev_rec->security_required & BTM_SEC_IN_ENCRYPT)) &&
        p_dev_rec->hci_handle != BTM_SEC_INVALID_HANDLE) {
        BTM_TRACE(EVENT, "Security Manager: Start encryption");

        if (!btm_sec_start_encryption(p_dev_rec))
            return BTM_NO_RESOURCES;

        return BTM_CMD_STARTED;
    }
    if (!(p_dev_rec->sec_flags & BTM_SEC_AUTHORIZED) && ((p_dev_rec->is_originator && (p_dev_rec->security_required & BTM_SEC_OUT_AUTHORIZE)) ||
                                                         (!p_dev_rec->is_originator && (p_dev_rec->security_required & BTM_SEC_IN_AUTHORIZE)))) {
        if (!btm_sec_are_all_trusted(p_dev_rec->trusted_mask) && p_dev_rec->p_cur_service->service_id < BTM_SEC_MAX_SERVICES &&
            !BTM_SEC_IS_SERVICE_TRUSTED(p_dev_rec->trusted_mask, p_dev_rec->p_cur_service->service_id)) {
            BTM_TRACE(EVENT, "Security Manager: Start authorization");
            return btm_sec_start_authorization(p_dev_rec);
        }
    }

    p_dev_rec->security_required &= ~63;

    BTM_TRACE(EVENT, "Security Manager: trusted:0x%04x%04x", p_dev_rec->trusted_mask[1], p_dev_rec->trusted_mask[0]);
    BTM_TRACE(EVENT, "Security Manager: access granted");

    return BTM_SUCCESS;
}

static BOOLEAN btm_sec_start_get_name(tBTM_SEC_DEV_REC* p_dev_rec) {
    UINT8 tempstate = p_dev_rec->sec_state;

    p_dev_rec->sec_state = BTM_SEC_STATE_GETTING_NAME;

    if (btm_initiate_rem_name(p_dev_rec->bd_addr, NULL, BTM_RMT_NAME_SEC, 30, NULL) != BTM_CMD_STARTED) {
        p_dev_rec->sec_state = tempstate;
        return FALSE;
    }

    return TRUE;
}

static BOOLEAN btm_sec_start_authentication(tBTM_SEC_DEV_REC* p_dev_rec) {
    p_dev_rec->sec_state = BTM_SEC_STATE_AUTHENTICATING;

    return btsnd_hcic_auth_request(p_dev_rec->hci_handle);
}

static BOOLEAN btm_sec_start_encryption(tBTM_SEC_DEV_REC* p_dev_rec) {
    if (!btsnd_hcic_set_conn_encrypt(p_dev_rec->hci_handle, TRUE))
        return FALSE;

    p_dev_rec->sec_state = BTM_SEC_STATE_ENCRYPTING;
    return TRUE;
}

static UINT8 btm_sec_start_authorization(tBTM_SEC_DEV_REC* p_dev_rec) {
    UINT8 result;
    UINT8* p_service_name = NULL;
    UINT8 service_id;

    if (p_dev_rec->sec_flags & BTM_SEC_NAME_KNOWN || p_dev_rec->hci_handle == BTM_SEC_INVALID_HANDLE) {
        if (!btm_cb.api.p_authorize_callback)
            return BTM_MODE_UNSUPPORTED;

        if (p_dev_rec->p_cur_service) {
            if (p_dev_rec->is_originator)
                p_service_name = p_dev_rec->p_cur_service->orig_service_name;
            else
                p_service_name = p_dev_rec->p_cur_service->term_service_name;

            service_id = p_dev_rec->p_cur_service->service_id;
        } else {
            service_id = 0;
        }

        p_dev_rec->sec_state = BTM_SEC_STATE_AUTHORIZING;
        result = (*btm_cb.api.p_authorize_callback)(p_dev_rec->bd_addr, p_dev_rec->dev_class, p_dev_rec->sec_bd_name, p_service_name, service_id,
                                                    p_dev_rec->is_originator);

        if (result == BTM_SUCCESS) {
            p_dev_rec->sec_flags |= BTM_SEC_AUTHORIZED;
            p_dev_rec->sec_state = BTM_SEC_STATE_IDLE;
        }

        return result;
    }

    btm_sec_start_get_name(p_dev_rec);
    return BTM_CMD_STARTED;
}

static BOOLEAN btm_sec_are_all_trusted(UINT32* p_mask) {
    int trusted_inx;
    for (trusted_inx = 0; trusted_inx < BTM_SEC_SERVICE_ARRAY_SIZE; ++trusted_inx) {
        if (p_mask[trusted_inx] != BTM_SEC_TRUST_ALL)
            return FALSE;
    }

    return TRUE;
}

static tBTM_SEC_SERV_REC* btm_sec_find_first_serv(UINT16 psm) {
    tBTM_SEC_SERV_REC* p_serv_rec = btm_cb.sec_serv_rec;
    int i;

    /* explicitly post-increment */
    for (i = 0; i < BTM_SEC_MAX_SERVICE_RECORDS; i++, p_serv_rec++) {
        if (p_serv_rec->security_flags & BTM_SEC_IN_USE && p_serv_rec->psm == psm) {
            return p_serv_rec;
        }
    }

    return NULL;
}

static tBTM_SEC_SERV_REC* btm_sec_find_next_serv(tBTM_SEC_SERV_REC* p_cur) {
    tBTM_SEC_SERV_REC* p_serv_rec = btm_cb.sec_serv_rec;
    BOOLEAN after_current = FALSE;
    int i;

    /* explicitly post-increment */
    for (i = 0; i < BTM_SEC_MAX_SERVICE_RECORDS; i++, p_serv_rec++) {
        if (p_serv_rec->security_flags & BTM_SEC_IN_USE && p_serv_rec->psm == p_cur->psm) {
            if (!after_current) {
                if (p_cur == p_serv_rec)
                    after_current = TRUE;
            } else {
                return p_serv_rec;
            }
        }
    }

    return NULL;
}

static tBTM_SEC_SERV_REC* btm_sec_find_mx_serv(UINT8 is_originator, UINT16 psm, UINT32 mx_proto_id, UINT32 mx_chan_id) {
    tBTM_SEC_SERV_REC* p_serv_rec = btm_cb.sec_serv_rec;
    int i;

    /* explicitly post-increment */
    for (i = 0; i < BTM_SEC_MAX_SERVICE_RECORDS; i++, p_serv_rec++) {
        if (p_serv_rec->security_flags & BTM_SEC_IN_USE && p_serv_rec->psm == psm && p_serv_rec->mx_proto_id == mx_proto_id &&
            ((is_originator && p_serv_rec->orig_mx_chan_id == mx_chan_id) || (!is_originator && p_serv_rec->term_mx_chan_id == mx_chan_id))) {
            return p_serv_rec;
        }
    }

    return NULL;
}

static void btm_sec_collision_timeout(TIMER_LIST_ENT* p_tle) {
    tBTM_STATUS status;

    BTM_TRACE(EVENT, "btm_sec_collision_timeout()");
    btm_cb.sec_collision_tle.param = 0;

    status = btm_sec_execute_procedure(btm_cb.p_collided_dev_rec);

    if (status == BTM_CMD_STARTED)
        return;

    if (btm_cb.p_collided_dev_rec->p_callback) {
        btm_clr_client_scn(btm_cb.p_collided_dev_rec);

        (*btm_cb.p_collided_dev_rec->p_callback)(btm_cb.p_collided_dev_rec->bd_addr, btm_cb.p_collided_dev_rec->p_ref_data, status);
        btm_cb.p_collided_dev_rec->p_callback = NULL;
    }
}

UINT32* btm_read_trusted_mask(BD_ADDR bd_addr) {
    tBTM_SEC_DEV_REC* p_dev_rec = btm_find_dev(bd_addr);

    if (p_dev_rec)
        return p_dev_rec->trusted_mask;
    else
        return NULL;
}

static void btm_restore_mode(void) {
    if (!btm_cb.security_mode_changed)
        return;

    btm_cb.security_mode_changed = FALSE;

    BTM_TRACE(DEBUG, "btm_restore_mode: Authen Enable -> %d", btm_cb.security_mode == BTM_SEC_MODE_LINK);

    btsnd_hcic_write_auth_enable(btm_cb.security_mode == BTM_SEC_MODE_LINK);
}

static void btm_clr_client_scn(tBTM_SEC_DEV_REC* p_dev_rec) {
    if (!p_dev_rec->is_originator)
        return;

    if (!p_dev_rec->p_cur_service)
        return;

    if (p_dev_rec->p_cur_service->mx_proto_id != 3)
        return;

    p_dev_rec->p_cur_service->orig_mx_chan_id = 0;
}
