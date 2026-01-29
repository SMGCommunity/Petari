#include "hidh_api.h"

/* Original source:
 * bluedroid <android.googlesource.com/platform/external/bluetooth/bluedroid>
 * stack/hid/hidh_api.c
 */

/******************************************************************************
 *
 *  Copyright (C) 2002-2012 Broadcom Corporation
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

#include "btm_api.h"
#include "btu.h"
#include "gki.h"
#include "hid_conn.h"
#include "hiddefs.h"
#include "hidh_int.h"
#include "sdp_api.h"
#include "sdpdefs.h"

/*******************************************************************************
 * macros
 */

/*******************************************************************************
 * types
 */

/*******************************************************************************
 * local function declarations
 */

static void hidh_search_callback(UINT16 sdp_result);

/*******************************************************************************
 * variables
 */

// .bss
tHID_HOST_CTB hh_cb;

/*******************************************************************************
 * functions
 */

tHID_STATUS HID_HostGetSDPRecord(BD_ADDR addr, tSDP_DISCOVERY_DB* p_db, UINT32 db_len, tHID_HOST_SDP_CALLBACK* sdp_cback) {
    tSDP_UUID uuid_list;

    if (hh_cb.sdp_busy)
        return HID_ERR_SDP_BUSY;

    uuid_list.len = LEN_UUID_16;
    uuid_list.uu.uuid16 = UUID_SERVCLASS_HUMAN_INTERFACE;

    hh_cb.p_sdp_db = p_db;
    SDP_InitDiscoveryDb(p_db, db_len, 1, &uuid_list, 0, NULL);

    if (SDP_ServiceSearchRequest(addr, p_db, &hidh_search_callback)) {
        hh_cb.sdp_cback = sdp_cback;
        hh_cb.sdp_busy = TRUE;

        return HID_SUCCESS;
    } else {
        return HID_ERR_NO_RESOURCES;
    }
}

void hidh_get_str_attr(tSDP_DISC_REC* p_rec, UINT16 attr_id, UINT16 max_len, char* str) {
    tSDP_DISC_ATTR* p_attr;
    UINT16 name_len;

    if ((p_attr = SDP_FindAttributeInRec(p_rec, attr_id)) != NULL) {
        if ((name_len = SDP_DISC_ATTR_LEN(p_attr->attr_len_type)) < max_len) {
            memcpy(str, p_attr->attr_value.v.array, name_len);
            str[name_len] = '\0';
        } else {
            memcpy(str, p_attr->attr_value.v.array, max_len - 1);
            str[max_len] = '\0';
        }
    } else {
        str[0] = '\0';
    }
}

static void hidh_search_callback(UINT16 sdp_result) {
    tSDP_DISCOVERY_DB* p_db = hh_cb.p_sdp_db;
    tSDP_DISC_REC* p_rec;
    tSDP_DISC_ATTR* p_attr;
    tSDP_DISC_ATTR* p_subattr1;
    tSDP_DISC_ATTR* p_subattr2;
    tSDP_DISC_ATTR* p_repdesc;
    tBT_UUID hid_uuid;
    tHID_DEV_SDP_INFO* p_nvi = &hh_cb.sdp_rec;
    UINT16 attr_mask = 0;

    hid_uuid.len = LEN_UUID_16;
    hid_uuid.uu.uuid16 = UUID_SERVCLASS_HUMAN_INTERFACE;

    hh_cb.sdp_busy = FALSE;

    if (sdp_result != SDP_SUCCESS) {
        (*hh_cb.sdp_cback)(sdp_result, 0, NULL);
        return;
    }

    if ((p_rec = SDP_FindServiceUUIDInDb(p_db, &hid_uuid, NULL)) == NULL) {
        (*hh_cb.sdp_cback)(HID_SDP_NO_SERV_UUID, 0, NULL);
        return;
    }

    memset(&hh_cb.sdp_rec, 0, sizeof hh_cb.sdp_rec);

    if (((p_attr = SDP_FindAttributeInRec(p_rec, ATTR_ID_HID_DESCRIPTOR_LIST)) == NULL) ||
        SDP_DISC_ATTR_TYPE(p_attr->attr_len_type) != DATA_ELE_SEQ_DESC_TYPE || ((p_subattr1 = p_attr->attr_value.v.p_sub_attr) == NULL) ||
        SDP_DISC_ATTR_TYPE(p_subattr1->attr_len_type) != DATA_ELE_SEQ_DESC_TYPE || ((p_subattr2 = p_subattr1->attr_value.v.p_sub_attr) == NULL) ||
        ((p_repdesc = p_subattr2->p_next_attr) == NULL) || SDP_DISC_ATTR_TYPE(p_repdesc->attr_len_type) != TEXT_STR_DESC_TYPE) {
        (*hh_cb.sdp_cback)(HID_SDP_MANDATORY_MISSING, 0, NULL);
        return;
    }

    if ((p_nvi->dscp_info.dl_len = SDP_DISC_ATTR_LEN(p_repdesc->attr_len_type)) != 0) {
        p_nvi->dscp_info.dsc_list = (UINT8*)&p_repdesc->attr_value;
    }

    if (((p_attr = SDP_FindAttributeInRec(p_rec, ATTR_ID_HID_VIRTUAL_CABLE)) != NULL) && p_attr->attr_value.v.u8) {
        attr_mask |= HID_VIRTUAL_CABLE;
    }

    if (((p_attr = SDP_FindAttributeInRec(p_rec, ATTR_ID_HID_RECONNECT_INITIATE)) != NULL) && p_attr->attr_value.v.u8) {
        attr_mask |= HID_RECONN_INIT;
    }

    if (((p_attr = SDP_FindAttributeInRec(p_rec, ATTR_ID_HID_NORMALLY_CONNECTABLE)) != NULL) && p_attr->attr_value.v.u8) {
        attr_mask |= HID_NORMALLY_CONNECTABLE;
    }

    if (((p_attr = SDP_FindAttributeInRec(p_rec, ATTR_ID_HID_SDP_DISABLE)) != NULL) && p_attr->attr_value.v.u8) {
        attr_mask |= HID_SDP_DISABLE;
    }

    if (((p_attr = SDP_FindAttributeInRec(p_rec, ATTR_ID_HID_BATTERY_POWER)) != NULL) && p_attr->attr_value.v.u8) {
        attr_mask |= HID_BATTERY_POWER;
    }

    if (((p_attr = SDP_FindAttributeInRec(p_rec, ATTR_ID_HID_REMOTE_WAKE)) != NULL) && p_attr->attr_value.v.u8) {
        attr_mask |= HID_REMOTE_WAKE;
    }

    hidh_get_str_attr(p_rec, ATTR_ID_SERVICE_NAME, HID_MAX_SVC_NAME_LEN, p_nvi->svc_name);
    hidh_get_str_attr(p_rec, ATTR_ID_SERVICE_DESCRIPTION, HID_MAX_SVC_DESCR_LEN, p_nvi->svc_descr);
    hidh_get_str_attr(p_rec, ATTR_ID_PROVIDER_NAME, HID_MAX_PROV_NAME_LEN, p_nvi->prov_name);

    if ((p_attr = SDP_FindAttributeInRec(p_rec, ATTR_ID_HID_DEVICE_RELNUM)) != NULL) {
        p_nvi->rel_num = p_attr->attr_value.v.u16;
    }

    if ((p_attr = SDP_FindAttributeInRec(p_rec, ATTR_ID_HID_COUNTRY_CODE)) != NULL) {
        p_nvi->ctry_code = p_attr->attr_value.v.u8;
    }

    if ((p_attr = SDP_FindAttributeInRec(p_rec, ATTR_ID_HID_DEVICE_SUBCLASS)) != NULL) {
        p_nvi->sub_class = p_attr->attr_value.v.u8;
    }

    if ((p_attr = SDP_FindAttributeInRec(p_rec, ATTR_ID_HID_PARSER_VERSION)) != NULL) {
        p_nvi->hpars_ver = p_attr->attr_value.v.u16;
    }

    if ((p_attr = SDP_FindAttributeInRec(p_rec, ATTR_ID_HID_LINK_SUPERVISION_TO)) != NULL) {
        attr_mask |= HID_SUP_TOUT_AVLBL;
        p_nvi->sup_timeout = p_attr->attr_value.v.u16;
    }

    hh_cb.sdp_rec.p_sdp_layer_rec = p_rec;
    (*hh_cb.sdp_cback)(SDP_SUCCESS, attr_mask, &hh_cb.sdp_rec);
}

void HID_HostInit(void) {
    memset(&hh_cb, 0, sizeof hh_cb);

    hh_cb.trace_level = BT_TRACE_LEVEL_NONE;
}

tHID_STATUS HID_HostRegister(tHID_HOST_DEV_CALLBACK* dev_cback) {
    tHID_STATUS st;

    if (hh_cb.reg_flag)
        return HID_ERR_ALREADY_REGISTERED;

    if (!dev_cback)
        return HID_ERR_INVALID_PARAM;

    if ((st = hidh_conn_reg()) != HID_SUCCESS)
        return st;

    hh_cb.callback = dev_cback;
    hh_cb.reg_flag = TRUE;

    return HID_SUCCESS;
}

tHID_STATUS HID_HostDeregister(void) {
    UINT8 i;

    if (!hh_cb.reg_flag)
        return HID_ERR_NOT_REGISTERED;

    for (i = 0; i < HID_HOST_MAX_DEVICES; i++)
        HID_HostRemoveDev(i);

    hidh_conn_dereg();
    hh_cb.reg_flag = FALSE;

    return HID_SUCCESS;
}

tHID_STATUS HID_HostAddDev(BD_ADDR addr, UINT16 attr_mask, UINT8* handle) {
    int i;

    if (!hh_cb.reg_flag)
        return HID_ERR_NOT_REGISTERED;

    for (i = 0; i < HID_HOST_MAX_DEVICES; i++) {
        if (memcmp(addr, hh_cb.devices[i].addr, BD_ADDR_LEN) == 0)
            break;
    }

    if (i == HID_HOST_MAX_DEVICES) {
        for (i = 0; i < HID_HOST_MAX_DEVICES; i++) {
            if (!hh_cb.devices[i].in_use)
                break;
        }
    }

    if (i == HID_HOST_MAX_DEVICES)
        return HID_ERR_NO_RESOURCES;

    if (!hh_cb.devices[i].in_use) {
        hh_cb.devices[i].in_use = TRUE;
        memcpy(hh_cb.devices[i].addr, addr, BD_ADDR_LEN);
        hh_cb.devices[i].state = HID_DEV_NO_CONN;
        hh_cb.devices[i].conn_tries = 0;
    }

    hh_cb.devices[i].attr_mask = attr_mask;

    *handle = i;

    return HID_SUCCESS;
}

tHID_STATUS HID_HostRemoveDev(UINT8 dev_handle) {
    if (!hh_cb.reg_flag)
        return HID_ERR_NOT_REGISTERED;

    if (dev_handle > HID_HOST_MAX_DEVICES || !hh_cb.devices[dev_handle].in_use)
        return HID_ERR_INVALID_PARAM;

    HID_HostCloseDev(dev_handle);
    hh_cb.devices[dev_handle].in_use = FALSE;
    hh_cb.devices[dev_handle].conn.conn_state = HID_CONN_STATE_UNUSED;
    hh_cb.devices[dev_handle].conn.ctrl_cid = hh_cb.devices[dev_handle].conn.intr_cid = 0;

    return HID_SUCCESS;
}

tHID_STATUS HID_HostOpenDev(UINT8 dev_handle) {
    if (!hh_cb.reg_flag)
        return HID_ERR_NOT_REGISTERED;

    if (dev_handle > HID_HOST_MAX_DEVICES || !hh_cb.devices[dev_handle].in_use)
        return HID_ERR_INVALID_PARAM;

    if (hh_cb.devices[dev_handle].state != HID_DEV_NO_CONN)
        return HID_ERR_ALREADY_CONN;

    hh_cb.devices[dev_handle].conn_tries = 1;
    return hidh_conn_initiate(dev_handle);
}

tHID_STATUS HID_HostWriteDev(UINT8 dev_handle, UINT8 t_type, UINT8 param, UINT16 data, UINT8 report_id, BT_HDR* pbuf) {
    tHID_STATUS status = HID_SUCCESS;

    if (!hh_cb.reg_flag) {
        HIDH_TRACE(ERROR, "HID_ERR_NOT_REGISTERED");
        status = HID_ERR_NOT_REGISTERED;
    }

    if (dev_handle > HID_HOST_MAX_DEVICES || !hh_cb.devices[dev_handle].in_use) {
        HIDH_TRACE(ERROR, "HID_ERR_INVALID_PARAM");
        status = HID_ERR_INVALID_PARAM;
    }

    if (hh_cb.devices[dev_handle].state != HID_DEV_CONNECTED) {
        HIDH_TRACE(ERROR, "HID_ERR_NO_CONNECTION dev_handle %d", dev_handle);
        status = HID_ERR_NO_CONNECTION;
    }

    if (status != HID_SUCCESS) {
        if (pbuf)
            GKI_freebuf(pbuf);
    } else {
        status = hidh_conn_snd_data(dev_handle, t_type, param, data, report_id, pbuf);
    }

    return status;
}

tHID_STATUS HID_HostCloseDev(UINT8 dev_handle) {
    if (!hh_cb.reg_flag)
        return HID_ERR_NOT_REGISTERED;

    if (dev_handle > HID_HOST_MAX_DEVICES || !hh_cb.devices[dev_handle].in_use)
        return HID_ERR_INVALID_PARAM;

    hh_cb.devices[dev_handle].conn_tries = HID_HOST_MAX_CONN_RETRY + 1;
    btu_stop_timer(&hh_cb.devices[dev_handle].conn.timer_entry);

    if (hh_cb.devices[dev_handle].state != HID_DEV_CONNECTED)
        return HID_ERR_NO_CONNECTION;

    hh_cb.devices[dev_handle].conn_tries = HID_HOST_MAX_CONN_RETRY + 1;
    return hidh_conn_disconnect(dev_handle);
}

tHID_STATUS HID_HostSetSecurityLevel(char* serv_name, UINT8 sec_lvl) {
    if (!BTM_SetSecurityLevel(FALSE, serv_name, BTM_SEC_SERVICE_HID_SEC_CTRL, sec_lvl, HID_PSM_CONTROL, BTM_SEC_PROTO_HID, HID_SEC_CHN)) {
        HIDH_TRACE(ERROR, "Security Registration 1 failed");
        return HID_ERR_NO_RESOURCES;
    }

    if (!BTM_SetSecurityLevel(TRUE, serv_name, BTM_SEC_SERVICE_HID_SEC_CTRL, sec_lvl, HID_PSM_CONTROL, BTM_SEC_PROTO_HID, HID_SEC_CHN)) {
        HIDH_TRACE(ERROR, "Security Registration 2 failed");
        return HID_ERR_NO_RESOURCES;
    }

    if (!BTM_SetSecurityLevel(FALSE, serv_name, BTM_SEC_SERVICE_HID_NOSEC_CTRL, BTM_SEC_NONE, HID_PSM_CONTROL, BTM_SEC_PROTO_HID, HID_NOSEC_CHN)) {
        HIDH_TRACE(ERROR, "Security Registration 3 failed");
        return HID_ERR_NO_RESOURCES;
    }

    if (!BTM_SetSecurityLevel(TRUE, serv_name, BTM_SEC_SERVICE_HID_NOSEC_CTRL, BTM_SEC_NONE, HID_PSM_CONTROL, BTM_SEC_PROTO_HID, HID_NOSEC_CHN)) {
        HIDH_TRACE(ERROR, "Security Registration 4 failed");
        return HID_ERR_NO_RESOURCES;
    }

    if (!BTM_SetSecurityLevel(TRUE, serv_name, BTM_SEC_SERVICE_HID_INTR, BTM_SEC_NONE, HID_PSM_INTERRUPT, BTM_SEC_PROTO_HID, 0)) {
        HIDH_TRACE(ERROR, "Security Registration 5 failed");
        return HID_ERR_NO_RESOURCES;
    }

    if (!BTM_SetSecurityLevel(FALSE, serv_name, BTM_SEC_SERVICE_HID_INTR, BTM_SEC_NONE, HID_PSM_INTERRUPT, BTM_SEC_PROTO_HID, 0)) {
        HIDH_TRACE(ERROR, "Security Registration 6 failed");
        return HID_ERR_NO_RESOURCES;
    }

    return HID_SUCCESS;
}
