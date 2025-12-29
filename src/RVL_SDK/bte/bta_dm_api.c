#include "bta_api.h"

/* Original source:
 * bluedroid <android.googlesource.com/platform/external/bluetooth/bluedroid>
 * bta/dm/bta_dm_api.c
 */

/******************************************************************************
 *
 *  Copyright (C) 2003-2012 Broadcom Corporation
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

#include "bt_target.h"  // BTA_DM_REMOTE_DEVICE_NAME_LENGTH
#include "bt_types.h"
#include "data_types.h"

#include "bd.h"  // bdcpy
#include "bta_dm_int.h"
#include "bta_sys.h"
#include "btm_api.h"
#include "gki.h"

/*******************************************************************************
 * variables
 */

// .rodata
static tBTA_SYS_REG const bta_dm_reg = {&bta_dm_sm_execute, NULL};

static tBTA_SYS_REG const bta_dm_search_reg = {&bta_dm_search_sm_execute, NULL};

/*******************************************************************************
 * functions
 */

tBTA_STATUS BTA_EnableBluetooth(tBTA_DM_SEC_CBACK* p_cback) {
    tBTA_DM_API_ENABLE* p_msg;

    memset(&bta_dm_cb, 0, sizeof bta_dm_cb);

    GKI_sched_lock();

    bta_sys_register(BTA_ID_DM, &bta_dm_reg);
    bta_sys_register(BTA_ID_DM_SEARCH, &bta_dm_search_reg);

    GKI_sched_unlock();

    if ((p_msg = GKI_getbuf(sizeof *p_msg)) != NULL) {
        p_msg->hdr.event = BTA_DM_API_ENABLE_EVT;
        p_msg->p_sec_cback = p_cback;

        bta_sys_sendmsg(p_msg);

        return BTA_SUCCESS;
    }

    return BTA_FAILURE;
}

void BTA_DisableBluetooth(void) {
    BT_HDR* p_msg;

    if ((p_msg = GKI_getbuf(sizeof *p_msg)) != NULL) {
        p_msg->event = BTA_DM_API_DISABLE_EVT;

        bta_sys_sendmsg(p_msg);
    }
}

BOOLEAN BTA_DmIsDeviceUp(void) {
    BOOLEAN status;

    GKI_sched_lock();

    status = BTM_IsDeviceUp();

    GKI_sched_unlock();

    return status;
}

void BTA_DmSetDeviceName(char* p_name) {
    tBTA_DM_API_SET_NAME* p_msg;

    if ((p_msg = GKI_getbuf(sizeof *p_msg)) != NULL) {
        p_msg->hdr.event = BTA_DM_API_SET_NAME_EVT;
        BCM_STRNCPY_S(p_msg->name, sizeof p_msg->name, p_name, BTA_DM_REMOTE_DEVICE_NAME_LENGTH);

        bta_sys_sendmsg(p_msg);
    }
}

void BTA_DmSetVisibility(tBTA_DM_DISC disc_mode, tBTA_DM_CONN conn_mode) {
    tBTA_DM_API_SET_VISIBILITY* p_msg;

    // ERRATUM: allocating much more space than necessary
    if ((p_msg = GKI_getbuf(sizeof(tBTA_DM_MSG))) != NULL) {
        p_msg->hdr.event = BTA_DM_API_SET_VISIBILITY_EVT;
        p_msg->disc_mode = disc_mode;
        p_msg->conn_mode = conn_mode;

        bta_sys_sendmsg(p_msg);
    }
}

void BTA_DmSearch(tBTA_DM_INQ* p_dm_inq, tBTA_SERVICE_MASK services, tBTA_DM_SEARCH_CBACK* p_cback) {
    tBTA_DM_API_SEARCH* p_msg;

    if ((p_msg = GKI_getbuf(sizeof *p_msg)) != NULL) {
        p_msg->hdr.event = BTA_DM_API_SEARCH_EVT;
        memcpy(&p_msg->inq_params, p_dm_inq, sizeof p_msg->inq_params);
        p_msg->services = services;
        p_msg->p_cback = p_cback;

        bta_sys_sendmsg(p_msg);
    }
}

void BTA_DmSearchCancel(void) {
    BT_HDR* p_msg;

    if ((p_msg = GKI_getbuf(sizeof *p_msg)) != NULL) {
        p_msg->event = BTA_DM_API_SEARCH_CANCEL_EVT;

        bta_sys_sendmsg(p_msg);
    }
}

void BTA_DmDiscover(BD_ADDR bd_addr, tBTA_SERVICE_MASK services, tBTA_DM_SEARCH_CBACK* p_cback) {
    tBTA_DM_API_DISCOVER* p_msg;

    if ((p_msg = GKI_getbuf(sizeof *p_msg)) != NULL) {
        p_msg->hdr.event = BTA_DM_API_DISCOVER_EVT;
        bdcpy(p_msg->bd_addr, bd_addr);
        p_msg->services = services;
        p_msg->p_cback = p_cback;

        bta_sys_sendmsg(p_msg);
    }
}

void BTA_DmBond(BD_ADDR bd_addr, UINT8 pin_len, UINT8* p_pin) {
    tBTA_DM_API_BOND* p_msg;

    if ((p_msg = GKI_getbuf(sizeof *p_msg)) != NULL) {
        p_msg->hdr.event = BTA_DM_API_BOND_EVT;
        bdcpy(p_msg->bd_addr, bd_addr);
        p_msg->pin_len = pin_len;
        memcpy(p_msg->p_pin, p_pin, pin_len);

        bta_sys_sendmsg(p_msg);
    }
}

void BTA_DmPinReply(BD_ADDR bd_addr, BOOLEAN accept, UINT8 pin_len, UINT8* p_pin) {
    tBTA_DM_API_PIN_REPLY* p_msg;

    if ((p_msg = GKI_getbuf(sizeof *p_msg)) != NULL) {
        p_msg->hdr.event = BTA_DM_API_PIN_REPLY_EVT;
        bdcpy(p_msg->bd_addr, bd_addr);
        p_msg->accept = accept;

        if (accept) {
            p_msg->pin_len = pin_len;
            memcpy(p_msg->p_pin, p_pin, pin_len);
        }

        bta_sys_sendmsg(p_msg);
    }
}

tBTA_STATUS BTA_DmAddDevice(BD_ADDR bd_addr, LINK_KEY link_key, UINT32 trusted_mask, BOOLEAN is_trusted) {
    BOOLEAN status;
    UINT32 trusted_services_mask[BTM_SEC_SERVICE_ARRAY_SIZE];
    UINT8 index = 0;
    UINT8 btm_mask_index = 0;

    memset(trusted_services_mask, 0, sizeof trusted_services_mask);

    if (is_trusted) {
        for (; trusted_mask != 0 && index < BTA_MAX_SERVICE_ID; ++index) {
            if (trusted_mask & (1 << index)) {
                btm_mask_index = bta_service_id_to_btm_srv_id_lkup_tbl[index] / BTM_SEC_ARRAY_BITS;

                trusted_services_mask[btm_mask_index] |= 1 << (bta_service_id_to_btm_srv_id_lkup_tbl[index] - btm_mask_index * BTM_SEC_ARRAY_BITS);

                trusted_mask &= ~(1 << index);
            }
        }
    }

    GKI_sched_lock();

    status = BTM_SecAddDevice(bd_addr, NULL, NULL, NULL, trusted_services_mask, link_key);

    GKI_sched_unlock();

    if (status)
        return BTA_SUCCESS;
    else
        return BTA_FAILURE;
}

tBTA_STATUS BTA_DmRemoveDevice(BD_ADDR bd_addr) {
    BOOLEAN status;

    GKI_sched_lock();

    status = BTM_SecDeleteDevice(bd_addr);

    GKI_sched_unlock();

    if (status)
        return BTA_SUCCESS;
    else
        return BTA_FAILURE;
}

void BTA_DmAuthorizeReply(BD_ADDR bd_addr, tBTA_SERVICE_ID service, tBTA_AUTH_RESP response) {
    tBTA_DM_API_AUTH_REPLY* p_msg;

    if ((p_msg = GKI_getbuf(sizeof *p_msg)) != NULL) {
        p_msg->hdr.event = BTA_DM_API_AUTH_REPLY_EVT;
        bdcpy(p_msg->bd_addr, bd_addr);
        p_msg->service = service;
        p_msg->response = response;

        bta_sys_sendmsg(p_msg);
    }
}

void BTA_DmSignalStrength(tBTA_SIG_STRENGTH_MASK mask, UINT16 period, BOOLEAN start) {
    tBTA_API_DM_SIG_STRENGTH* p_msg;

    if ((p_msg = GKI_getbuf(sizeof *p_msg)) != NULL) {
        p_msg->hdr.event = BTA_DM_API_SIG_STRENGTH_EVT;
        p_msg->mask = mask;
        p_msg->period = period;
        p_msg->start = start;

        bta_sys_sendmsg(p_msg);
    }
}

void BTA_KeepACLLinksOnShutDown(BOOLEAN keep) {
    BT_HDR* p_msg;

    if ((p_msg = GKI_getbuf(sizeof *p_msg)) != NULL) {
        p_msg->event = BTA_DM_API_KEEP_ACL_LINKS;
        p_msg->layer_specific = keep;

        bta_sys_sendmsg(p_msg);
    }
}

void BTA_DmSendHciReset(void) {
    BT_HDR* p_msg;

    if ((p_msg = GKI_getbuf(sizeof *p_msg)) != NULL) {
        p_msg->event = BTA_DM_API_RESET_HCI;

        bta_sys_sendmsg(p_msg);
    }
}
