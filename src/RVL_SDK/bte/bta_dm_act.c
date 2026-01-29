#include "bta_dm_int.h"

/* Original source:
 * bluedroid <android.googlesource.com/platform/external/bluetooth/bluedroid>
 * bta/dm/bta_dm_act.c
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

#include <cstring>

#include <revolution/types.h>

#include <decomp.h>  // unk_t

#include "bt_target.h"
#include "bt_trace.h"  // APPL_TRACE
#include "bt_types.h"
#include "data_types.h"

#include "bd.h"
#include "bta_api.h"
#include "bta_dm_co.h"  // bta_dm_co_get_compress_memory
#include "bta_sys.h"
#include "bta_sys_int.h"  // bta_sys_cb
#include "btm_api.h"
#include "btm_int.h"
#include "btu.h"  // BT_BD_ANY
#include "gki.h"
#include "hcidefs.h"
#include "hcimsgs.h"  // btsnd_hcic_write_scan_enable
#include "l2c_api.h"
#include "sdp_api.h"
#include "sdpdefs.h"
#include "wbt_api.h"

/*******************************************************************************
 * local function declarations
 */

static void bta_dm_disable_timer_cback(TIMER_LIST_ENT* p_tle);

static void bta_dm_search_timer_cback(TIMER_LIST_ENT* p_tle);

static void bta_dm_find_services(BD_ADDR bd_addr);
static void bta_dm_discover_next_device(void);
static void bta_dm_sdp_callback(UINT16 sdp_status);
static void bta_dm_inq_results_cb(tBTM_INQ_RESULTS* p_inq);
static void bta_dm_inq_cmpl_cb(void* p_result);
static void bta_dm_service_search_remname_cback(BD_ADDR bd_addr, DEV_CLASS dc, tBTM_BD_NAME bd_name);
static void bta_dm_remname_cback(tBTM_REMOTE_DEV_NAME* p_remote_name);
static void bta_dm_disc_remname_cback(tBTM_REMOTE_DEV_NAME* p_remote_name);
static void bta_dm_start_remote_name_response_timer(void);
static void bta_dm_stop_remote_name_response_timer(void);
static void bta_dm_remote_name_request_cmd_failed(UINT8* bd_addr);

static tBTM_STATUS bta_dm_authorize_cback(BD_ADDR bd_addr, DEV_CLASS dev_class, BD_NAME bd_name, UINT8* service_name, UINT8 service_id,
                                          BOOLEAN is_originator);
static void bta_dm_pinname_cback(void* p_data);
static tBTM_STATUS bta_dm_pin_cback(BD_ADDR bd_addr, DEV_CLASS dev_class, BD_NAME bd_name);
static tBTM_STATUS bta_dm_link_key_request_cback(BD_ADDR bd_addr, LINK_KEY key);
static tBTM_STATUS bta_dm_new_link_key_cback(BD_ADDR bd_addr, DEV_CLASS dev_class, BD_NAME bd_name, LINK_KEY key, UINT8 key_type);
static tBTM_STATUS bta_dm_authentication_complete_cback(BD_ADDR bd_addr, DEV_CLASS dev_class, BD_NAME bd_name, int result);
static void bta_dm_local_addr_cback(BD_ADDR bd_addr);

static void bta_dm_signal_strength_timer_cback(TIMER_LIST_ENT* p_tle);
static void bta_dm_acl_change_cback(BD_ADDR p_bda, DEV_CLASS p_dc, BD_NAME p_bdn, BD_FEATURES features, BOOLEAN is_new);

static void bta_dm_disable_conn_down_timer_cback(TIMER_LIST_ENT* p_tle);
static void bta_dm_rssi_cback(tBTM_RSSI_RESULTS* p_result);
static void bta_dm_link_quality_cback(tBTM_LINK_QUALITY_RESULTS* p_result);

static BOOLEAN bta_dm_l2cap_server_compress_cback(BD_ADDR peer_addr, unk_t, unk_t, unk_t, unk_t, unk_t, unk_t, UINT8** p_mem_pool,
                                                  UINT32* mem_pool_size);
static void bta_dm_init_compress(void);
static void bta_dm_compress_cback(tBTA_SYS_CONN_STATUS status, UINT8 id, UINT8 app_id, BD_ADDR peer_addr);
static void bta_dm_rm_cback(tBTA_SYS_CONN_STATUS status, UINT8 id, UINT8 app_id, BD_ADDR peer_addr);
static void bta_dm_adjust_roles(BOOLEAN delay_role_switch);
static char* bta_dm_get_remname(void);

/*******************************************************************************
 * variables
 */

// .rodata
UINT16 const bta_service_id_to_uuid_lkup_tbl[BTA_MAX_SERVICE_ID] = {
    UUID_SERVCLASS_PNP_INFORMATION,
    UUID_SERVCLASS_SERIAL_PORT,
    UUID_SERVCLASS_DIALUP_NETWORKING,
    UUID_SERVCLASS_FAX,  // this slot is UUID_SERVCLASS_AUDIO_SOURCE in bluedroid
    UUID_SERVCLASS_LAN_ACCESS_USING_PPP,
    UUID_SERVCLASS_HEADSET,
    UUID_SERVCLASS_HF_HANDSFREE,
    UUID_SERVCLASS_OBEX_OBJECT_PUSH,
    UUID_SERVCLASS_OBEX_FILE_TRANSFER,
    UUID_SERVCLASS_CORDLESS_TELEPHONY,
    UUID_SERVCLASS_INTERCOM,
    UUID_SERVCLASS_IRMC_SYNC,
    UUID_SERVCLASS_DIRECT_PRINTING,
    UUID_SERVCLASS_IMAGING_RESPONDER,
    UUID_SERVCLASS_PANU,
    UUID_SERVCLASS_NAP,
    UUID_SERVCLASS_GN,
    UUID_SERVCLASS_SAP,
    UUID_SERVCLASS_AUDIO_SINK,
    UUID_SERVCLASS_AV_REMOTE_CONTROL,
    UUID_SERVCLASS_HUMAN_INTERFACE,
    UUID_SERVCLASS_VIDEO_SINK,
    UUID_SERVCLASS_PBAP_PSE,
};

UINT32 const bta_service_id_to_btm_srv_id_lkup_tbl[BTA_MAX_SERVICE_ID] = {
    0,
    BTM_SEC_SERVICE_SERIAL_PORT,
    BTM_SEC_SERVICE_DUN,
    BTM_SEC_SERVICE_FAX,  // this slot is BTM_SEC_SERVICE_AVDTP in bluedroid
    BTM_SEC_SERVICE_LAN_ACCESS,
    BTM_SEC_SERVICE_HEADSET_AG,
    BTM_SEC_SERVICE_AG_HANDSFREE,
    BTM_SEC_SERVICE_OBEX,
    BTM_SEC_SERVICE_OBEX_FTP,
    BTM_SEC_SERVICE_CORDLESS,
    BTM_SEC_SERVICE_INTERCOM,
    BTM_SEC_SERVICE_IRMC_SYNC,
    BTM_SEC_SERVICE_BPP_JOB,
    BTM_SEC_SERVICE_BIP,
    BTM_SEC_SERVICE_BNEP_PANU,
    BTM_SEC_SERVICE_BNEP_NAP,
    BTM_SEC_SERVICE_BNEP_GN,
    BTM_SEC_SERVICE_SAP,
    BTM_SEC_SERVICE_AVDTP,
    BTM_SEC_SERVICE_AVCTP,
    BTM_SEC_SERVICE_HID_SEC_CTRL,
    BTM_SEC_SERVICE_AVDTP,
    BTM_SEC_SERVICE_PBAP,
};

tBTM_APPL_INFO const bta_security = {&bta_dm_authorize_cback,
                                     &bta_dm_pin_cback,
                                     &bta_dm_new_link_key_cback,
                                     &bta_dm_link_key_request_cback,
                                     &bta_dm_authentication_complete_cback,
                                     NULL};

// .bss
static tBTA_DM_COMPRESS_SRVCS bta_dm_compress_srvcs[BTA_DM_NUM_COMPRESS_SRVS];

/*******************************************************************************
 * functions
 */

void bta_dm_enable(tBTA_DM_MSG* p_data) {
    DEV_CLASS dev_class;

    memset(&bta_dm_search_cb, 0, sizeof bta_dm_search_cb);
    memset(&bta_dm_conn_srvcs, 0, sizeof bta_dm_conn_srvcs);

    memcpy(dev_class, bta_dm_cfg.dev_class, DEV_CLASS_LEN);
    BTM_SetDeviceClass(dev_class);

    bta_dm_cb.p_sec_cback = p_data->enable.p_sec_cback;
    BTM_SecRegister((tBTM_APPL_INFO*)&bta_security);

    BTM_SetDefaultLinkSuperTout(bta_dm_cfg.link_timeout);
    BTM_WritePageTimeout(bta_dm_cfg.page_timeout);
    BTM_SetDefaultLinkPolicy(bta_dm_cfg.policy_settings);

    BTM_AclRegisterForChanges(&bta_dm_acl_change_cback);
    BTM_ReadLocalDeviceAddr((tBTM_CMPL_CB*)&bta_dm_local_addr_cback);

    bta_sys_rm_register(&bta_dm_rm_cback);
    bta_sys_compress_register(&bta_dm_compress_cback);

    bta_dm_init_pm();
    bta_dm_init_compress();

    WBT_ExtCreateRecord();
}

void bta_dm_disable(tBTA_DM_MSG* p_data) {
    // casting away constness
    L2CA_SetIdleTimeoutByBdAddr((BD_ADDR_PTR)BT_BD_ANY, 0);

    bta_sys_disable();

    BTM_SetDiscoverability(BTM_NON_DISCOVERABLE, 0, 0);
    BTM_SetConnectability(BTM_NON_CONNECTABLE, 0, 0);

    bta_dm_disable_pm();

    if (BTM_GetNumAclLinks() == 0) {
        bta_dm_cb.disable_timer.p_cback = (TIMER_CBACK*)&bta_dm_disable_conn_down_timer_cback;
        bta_sys_start_timer(&bta_dm_cb.disable_timer, 0, BTA_DISABLE_DELAY);
    } else {
        bta_dm_cb.disabling = TRUE;

        bta_dm_cb.disable_timer.p_cback = (TIMER_CBACK*)&bta_dm_disable_timer_cback;
        bta_sys_start_timer(&bta_dm_cb.disable_timer, 0, 5000);
    }
}

static void bta_dm_disable_timer_cback(TIMER_LIST_ENT* p_tle) {
    UINT8 i;

    APPL_TRACE(EVENT, " bta_dm_disable_timer_cback  ");

    if (BTM_GetNumAclLinks() != 0 && !bta_dm_cb.keep_acl_on_shut_down) {
        for (i = 0; i < bta_dm_cb.device_list.count; ++i)
            btm_remove_acl(bta_dm_cb.device_list.peer_device[i].peer_bdaddr);
    } else {
        bta_dm_cb.disabling = FALSE;

        (*bta_dm_cb.p_sec_cback)(BTA_DM_DISABLE_EVT, NULL);
    }
}

void bta_dm_set_dev_name(tBTA_DM_MSG* p_data) {
    BTM_SetLocalDeviceName(p_data->set_name.name);
}

void bta_dm_set_visibility(tBTA_DM_MSG* p_data) {
    BTM_SetDiscoverability((UINT8)p_data->set_visibility.disc_mode, 0, 0);
    BTM_SetConnectability((UINT8)p_data->set_visibility.conn_mode, 0, 0);
}

void bta_dm_bond(tBTA_DM_MSG* p_data) {
    UINT32 trusted_mask[BTM_SEC_SERVICE_ARRAY_SIZE];
    UINT32* current_trusted_mask;

    current_trusted_mask = btm_read_trusted_mask(p_data->bond.bd_addr);

    if (current_trusted_mask)
        memcpy(trusted_mask, current_trusted_mask, sizeof trusted_mask);
    else
        memset(trusted_mask, 0, sizeof trusted_mask);

    BTM_SecBond(p_data->bond.bd_addr, p_data->bond.pin_len, p_data->bond.p_pin, trusted_mask);
}

void bta_dm_pin_reply(tBTA_DM_MSG* p_data) {
    UINT32 trusted_mask[BTM_SEC_SERVICE_ARRAY_SIZE];
    UINT32* current_trusted_mask;

    current_trusted_mask = btm_read_trusted_mask(p_data->pin_reply.bd_addr);

    if (current_trusted_mask)
        memcpy(trusted_mask, current_trusted_mask, sizeof trusted_mask);
    else
        memset(trusted_mask, 0, sizeof trusted_mask);

    if (p_data->pin_reply.accept) {
        BTM_PINCodeReply(p_data->pin_reply.bd_addr, BTM_SUCCESS, p_data->pin_reply.pin_len, p_data->pin_reply.p_pin, trusted_mask);
    } else {
        BTM_PINCodeReply(p_data->pin_reply.bd_addr, BTM_NOT_AUTHORIZED, 0, NULL, trusted_mask);
    }
}

void bta_dm_auth_reply(tBTA_DM_MSG* p_data) {
    UINT32 trusted_mask[BTM_SEC_SERVICE_ARRAY_SIZE];
    UINT8 btm_mask_index = 0;
    UINT32* current_trusted_mask;

    current_trusted_mask = btm_read_trusted_mask(p_data->auth_reply.bd_addr);

    if (current_trusted_mask)
        memcpy(trusted_mask, current_trusted_mask, sizeof trusted_mask);
    else
        memset(trusted_mask, 0, sizeof trusted_mask);

    if (p_data->auth_reply.response != BTA_DM_NOT_AUTH) {
        if (p_data->auth_reply.response == BTA_DM_AUTH_PERM) {
            if (p_data->auth_reply.service <= BTA_MAX_SERVICE_ID) {
                btm_mask_index = bta_service_id_to_btm_srv_id_lkup_tbl[p_data->auth_reply.service] / BTM_SEC_ARRAY_BITS;

                trusted_mask[btm_mask_index] |=
                    1 << (bta_service_id_to_btm_srv_id_lkup_tbl[p_data->auth_reply.service] - btm_mask_index * BTM_SEC_ARRAY_BITS);
            }
        }

        BTM_DeviceAuthorized(p_data->auth_reply.bd_addr, BTM_SUCCESS, trusted_mask);
    } else {
        BTM_DeviceAuthorized(p_data->auth_reply.bd_addr, BTM_NOT_AUTHORIZED, trusted_mask);
    }
}

void bta_dm_search_start(tBTA_DM_MSG* p_data) {
    BTM_ClearInqDb(NULL);

    bta_dm_search_cb.p_search_cback = p_data->search.p_cback;
    bta_dm_search_cb.services = p_data->search.services;

    BTM_StartInquiry((tBTM_INQ_PARMS*)&p_data->search.inq_params, (tBTM_INQ_RESULTS_CB*)&bta_dm_inq_results_cb, &bta_dm_inq_cmpl_cb);
}

void bta_dm_search_cancel(tBTA_DM_MSG* p_data) {
    tBTA_DM_MSG* p_msg;

    BTM_ClearInqDb(NULL);

    if (BTM_IsInquiryActive()) {
        BTM_CancelInquiry();
        bta_dm_search_cancel_notify(NULL);

        if ((p_msg = GKI_getbuf(sizeof *p_msg)) != NULL) {
            p_msg->hdr.event = BTA_DM_SEARCH_CMPL_EVT;
            bta_sys_sendmsg(p_msg);
        }
    } else if (!bta_dm_search_cb.services) {
        BTM_CancelRemoteDeviceName();
    }
}

void bta_dm_discover(tBTA_DM_MSG* p_data) {
    bta_dm_search_cb.p_search_cback = p_data->discover.p_cback;
    bta_dm_search_cb.services = p_data->discover.services;
    bta_dm_search_cb.services_to_search = bta_dm_search_cb.services;
    bta_dm_search_cb.service_index = 0;
    bta_dm_search_cb.services_found = 0;
    bta_dm_search_cb.peer_name[0] = '\0';

    BTM_SecAddRmtNameNotifyCallback(&bta_dm_service_search_remname_cback);

    bdcpy(bta_dm_search_cb.peer_bdaddr, p_data->discover.bd_addr);

    if (bta_dm_search_cb.services) {
        bta_dm_find_services(bta_dm_search_cb.peer_bdaddr);
    } else {
        bta_dm_start_remote_name_response_timer();

        if (BTM_ReadRemoteDeviceName(bta_dm_search_cb.peer_bdaddr, (tBTM_CMPL_CB*)&bta_dm_disc_remname_cback) != BTM_CMD_STARTED) {
            bta_dm_stop_remote_name_response_timer();

            bta_dm_remote_name_request_cmd_failed(bta_dm_search_cb.peer_bdaddr);
        }
    }
}

void bta_dm_inq_cmpl(tBTA_DM_MSG* p_data) {
    tBTA_DM_MSG* p_msg;
    tBTA_DM_SEARCH data;
    BOOLEAN search_complete = TRUE;
    tBTA_DM_DISC_RES disc_res;

    if ((bta_dm_search_cb.p_btm_inq_info = BTM_InqDbFirst()) != NULL) {
        search_complete = FALSE;

        if (bta_dm_search_cb.services) {
            if (BTM_IsAclConnectionUp(bta_dm_search_cb.p_btm_inq_info->results.remote_bd_addr)) {
                bta_dm_search_cb.wait_disc = FALSE;
            } else {
                bta_dm_search_cb.wait_disc = TRUE;
            }

            bta_dm_search_cb.sdp_results = FALSE;

            BTM_SecAddRmtNameNotifyCallback(&bta_dm_service_search_remname_cback);

            bta_dm_search_cb.service_index = 0;
            bta_dm_search_cb.services_found = 0;
            bta_dm_search_cb.services_to_search = bta_dm_search_cb.services;
            bta_dm_search_cb.peer_name[0] = '\0';

            bdcpy(bta_dm_search_cb.peer_bdaddr, bta_dm_search_cb.p_btm_inq_info->results.remote_bd_addr);

            bta_dm_find_services(bta_dm_search_cb.p_btm_inq_info->results.remote_bd_addr);
        } else {
            search_complete = TRUE;

            for (; bta_dm_search_cb.p_btm_inq_info; bta_dm_search_cb.p_btm_inq_info = BTM_InqDbNext(bta_dm_search_cb.p_btm_inq_info)) {
                if (bta_dm_search_cb.p_btm_inq_info->appl_knows_rem_name) {
                    bdcpy(disc_res.bd_addr, bta_dm_search_cb.p_btm_inq_info->results.remote_bd_addr);

                    disc_res.bd_name[0] = '\0';
                    disc_res.services = 0;

                    (*bta_dm_search_cb.p_search_cback)(BTA_DM_DISC_RES_EVT, (tBTA_DM_SEARCH*)&disc_res);
                } else {
                    bta_dm_start_remote_name_response_timer();

                    if (BTM_ReadRemoteDeviceName(bta_dm_search_cb.p_btm_inq_info->results.remote_bd_addr, (tBTM_CMPL_CB*)&bta_dm_remname_cback) !=
                        BTM_CMD_STARTED) {
                        bta_dm_stop_remote_name_response_timer();

                        bta_dm_remote_name_request_cmd_failed(bta_dm_search_cb.p_btm_inq_info->results.remote_bd_addr);
                    }

                    search_complete = FALSE;
                    break;
                }
            }
        }
    }

    if (search_complete) {
        bta_dm_search_cb.services = 0;

        if ((p_msg = GKI_getbuf(sizeof *p_msg)) != NULL) {
            p_msg->hdr.event = BTA_DM_SEARCH_CMPL_EVT;

            bta_sys_sendmsg(p_msg);
        }
    }

    data.inq_cmpl.num_resps = p_data->inq_cmpl.num;

    (*bta_dm_search_cb.p_search_cback)(BTA_DM_INQ_CMPL_EVT, &data);
}

void bta_dm_rmt_name(tBTA_DM_MSG* p_data) {
    tBTA_DM_MSG* p_msg;
    UINT8 search_complete = TRUE;
    tBTA_DM_DISC_RES disc_res;

    while ((bta_dm_search_cb.p_btm_inq_info = BTM_InqDbNext(bta_dm_search_cb.p_btm_inq_info)) != NULL) {
        if (bta_dm_search_cb.p_btm_inq_info->appl_knows_rem_name) {
            bdcpy(disc_res.bd_addr, bta_dm_search_cb.p_btm_inq_info->results.remote_bd_addr);

            disc_res.bd_name[0] = '\0';
            disc_res.services = 0;

            (*bta_dm_search_cb.p_search_cback)(BTA_DM_DISC_RES_EVT, (tBTA_DM_SEARCH*)&disc_res);
        } else {
            bta_dm_start_remote_name_response_timer();

            if (BTM_ReadRemoteDeviceName(bta_dm_search_cb.p_btm_inq_info->results.remote_bd_addr, (tBTM_CMPL_CB*)&bta_dm_remname_cback) !=
                BTM_CMD_STARTED) {
                bta_dm_stop_remote_name_response_timer();

                bta_dm_remote_name_request_cmd_failed(bta_dm_search_cb.p_btm_inq_info->results.remote_bd_addr);
            }

            search_complete = FALSE;
            break;
        }
    }

    if (search_complete) {
        if ((p_msg = GKI_getbuf(sizeof *p_msg)) != NULL) {
            p_msg->hdr.event = BTA_DM_SEARCH_CMPL_EVT;

            bta_sys_sendmsg(p_msg);
        }
    }

    (*bta_dm_search_cb.p_search_cback)(BTA_DM_DISC_RES_EVT, &p_data->disc_result.result);
}

void bta_dm_disc_rmt_name(tBTA_DM_MSG* p_data) {
    tBTA_DM_MSG* p_msg;

    if ((p_msg = GKI_getbuf(sizeof *p_msg)) != NULL) {
        p_msg->hdr.event = BTA_DM_SEARCH_CMPL_EVT;

        bta_sys_sendmsg(p_msg);
    }

    p_data->disc_result.result.disc_res.services = 0;

    (*bta_dm_search_cb.p_search_cback)(BTA_DM_DISC_RES_EVT, &p_data->disc_result.result);
}

void bta_dm_sdp_result(tBTA_DM_MSG* p_data) {
    tSDP_DISC_REC* p_sdp_rec = NULL;
    tBTA_DM_MSG* p_msg;
    BOOLEAN service_found = FALSE;
    UINT16 service;

    if (p_data->sdp_event.sdp_result == SDP_SUCCESS || p_data->sdp_event.sdp_result == SDP_NO_RECS_MATCH ||
        p_data->sdp_event.sdp_result == SDP_DB_FULL) {
        service = bta_service_id_to_uuid_lkup_tbl[bta_dm_search_cb.service_index - 1];

        if (p_data->sdp_event.sdp_result == SDP_DB_FULL ||
            ((p_sdp_rec = SDP_FindServiceInDb(bta_dm_search_cb.p_sdp_db, service, p_sdp_rec)) != NULL)) {
            if (service == UUID_SERVCLASS_PNP_INFORMATION) {
                if (p_sdp_rec) {
                    if (SDP_FindAttributeInRec(p_sdp_rec, ATTR_ID_EXT_BRCM_VERSION)) {
                        service_found = TRUE;
                    }
                }
            } else {
                service_found = TRUE;
            }

            if (service_found) {
                bta_dm_search_cb.services_found |= BTA_SERVICE_ID_TO_SERVICE_MASK(bta_dm_search_cb.service_index - 1);
            }
        }

        GKI_freebuf(bta_dm_search_cb.p_sdp_db);
        bta_dm_search_cb.p_sdp_db = NULL;

        if (bta_dm_search_cb.services_to_search) {
            bta_dm_find_services(bta_dm_search_cb.peer_bdaddr);
        } else {
            BTM_SecDeleteRmtNameNotifyCallback(&bta_dm_service_search_remname_cback);

            if ((p_msg = GKI_getbuf(sizeof *p_msg)) != NULL) {
                p_msg->hdr.event = BTA_DM_DISCOVERY_RESULT_EVT;
                p_msg->disc_result.result.disc_res.result = BTA_SUCCESS;

                p_msg->disc_result.result.disc_res.services = bta_dm_search_cb.services_found;

                bdcpy(p_msg->disc_result.result.disc_res.bd_addr, bta_dm_search_cb.peer_bdaddr);
                BCM_STRNCPY_S((char*)p_msg->disc_result.result.disc_res.bd_name, sizeof(BD_NAME), bta_dm_get_remname(),
                              BTA_DM_REMOTE_DEVICE_NAME_LENGTH);

                bta_sys_sendmsg(p_msg);
            }
        }
    } else {
        if (p_data->sdp_event.sdp_result == SDP_CONN_FAILED || p_data->sdp_event.sdp_result == SDP_CONN_REJECTED ||
            p_data->sdp_event.sdp_result == SDP_SECURITY_ERR) {
            bta_dm_search_cb.wait_disc = FALSE;
        }

        GKI_freebuf(bta_dm_search_cb.p_sdp_db);
        bta_dm_search_cb.p_sdp_db = NULL;

        BTM_SecDeleteRmtNameNotifyCallback(&bta_dm_service_search_remname_cback);

        if ((p_msg = GKI_getbuf(sizeof *p_msg)) != NULL) {
            p_msg->hdr.event = BTA_DM_DISCOVERY_RESULT_EVT;
            p_msg->disc_result.result.disc_res.result = BTA_FAILURE;
            p_msg->disc_result.result.disc_res.services = bta_dm_search_cb.services_found;

            bdcpy(p_msg->disc_result.result.disc_res.bd_addr, bta_dm_search_cb.peer_bdaddr);
            BCM_STRNCPY_S((char*)p_msg->disc_result.result.disc_res.bd_name, sizeof(BD_NAME), bta_dm_get_remname(), BTA_DM_REMOTE_DEVICE_NAME_LENGTH);

            bta_sys_sendmsg(p_msg);
        }
    }
}

void bta_dm_search_cmpl(tBTA_DM_MSG* p_data) {
    (*bta_dm_search_cb.p_search_cback)(BTA_DM_DISC_CMPL_EVT, NULL);
}

void bta_dm_disc_result(tBTA_DM_MSG* p_data) {
    tBTA_DM_MSG* p_msg;

    (*bta_dm_search_cb.p_search_cback)(BTA_DM_DISC_RES_EVT, &p_data->disc_result.result);

    if ((p_msg = GKI_getbuf(sizeof *p_msg)) != NULL) {
        p_msg->hdr.event = BTA_DM_SEARCH_CMPL_EVT;

        bta_sys_sendmsg(p_msg);
    }
}

void bta_dm_search_result(tBTA_DM_MSG* p_data) {
    if (p_data->disc_result.result.disc_res.services) {
        (*bta_dm_search_cb.p_search_cback)(BTA_DM_DISC_RES_EVT, &p_data->disc_result.result);
    }

    if (!bta_dm_search_cb.wait_disc) {
        bta_dm_search_cb.wait_disc = FALSE;

        bta_dm_discover_next_device();
    } else {
        bta_dm_search_cb.sdp_results = TRUE;
        bta_dm_search_cb.search_timer.p_cback = (TIMER_CBACK*)&bta_dm_search_timer_cback;

        bta_sys_start_timer(&bta_dm_search_cb.search_timer, 0, 1000 * (L2CAP_LINK_INACTIVITY_TOUT + 1));
    }
}

static void bta_dm_search_timer_cback(TIMER_LIST_ENT* p_tle) {
    APPL_TRACE(EVENT, " bta_dm_search_timer_cback  ");

    bta_dm_search_cb.wait_disc = FALSE;
    bta_dm_discover_next_device();
}

void bta_dm_free_sdp_db(tBTA_DM_MSG* p_data) {
    if (bta_dm_search_cb.p_sdp_db) {
        GKI_freebuf(bta_dm_search_cb.p_sdp_db);
        bta_dm_search_cb.p_sdp_db = NULL;
    }
}

void bta_dm_queue_search(tBTA_DM_MSG* p_data) {
    bta_dm_search_cb.p_search_queue = GKI_getbuf(sizeof bta_dm_search_cb.p_search_queue->search);

    memcpy(&bta_dm_search_cb.p_search_queue->search, &p_data->search, sizeof bta_dm_search_cb.p_search_queue->search);
}

void bta_dm_queue_disc(tBTA_DM_MSG* p_data) {
    bta_dm_search_cb.p_search_queue = GKI_getbuf(sizeof bta_dm_search_cb.p_search_queue->discover);

    memcpy(bta_dm_search_cb.p_search_queue, p_data, sizeof bta_dm_search_cb.p_search_queue->discover);
}

void bta_dm_search_clear_queue(tBTA_DM_MSG* p_data) {
    if (bta_dm_search_cb.p_search_queue) {
        GKI_freebuf(bta_dm_search_cb.p_search_queue);
        bta_dm_search_cb.p_search_queue = NULL;
    }
}

void bta_dm_search_cancel_cmpl(tBTA_DM_MSG* p_data) {
    if (bta_dm_search_cb.p_search_queue) {
        bta_sys_sendmsg(bta_dm_search_cb.p_search_queue);
        bta_dm_search_cb.p_search_queue = NULL;
    }
}

void bta_dm_search_cancel_transac_cmpl(tBTA_DM_MSG* p_data) {
    if (bta_dm_search_cb.p_sdp_db) {
        GKI_freebuf(bta_dm_search_cb.p_sdp_db);
        bta_dm_search_cb.p_sdp_db = NULL;
    }

    bta_dm_search_cancel_notify(NULL);
}

void bta_dm_search_cancel_notify(tBTA_DM_MSG* p_data) {
    (*bta_dm_search_cb.p_search_cback)(BTA_DM_SEARCH_CANCEL_CMPL_EVT, NULL);
}

static void bta_dm_find_services(BD_ADDR bd_addr) {
    tSDP_UUID uuid;
    UINT16 attr_list[] = {
        ATTR_ID_SERVICE_CLASS_ID_LIST,
        ATTR_ID_EXT_BRCM_VERSION,
    };
    UINT16 num_attrs = 1;
    tBTA_DM_MSG* p_msg;

    while (bta_dm_search_cb.service_index < BTA_MAX_SERVICE_ID) {
        if (bta_dm_search_cb.services_to_search & BTA_SERVICE_ID_TO_SERVICE_MASK(bta_dm_search_cb.service_index)) {
            if ((bta_dm_search_cb.p_sdp_db = GKI_getbuf(BTA_DM_SDP_DB_SIZE)) != NULL) {
                bta_dm_search_cb.services_to_search &= ~BTA_SERVICE_ID_TO_SERVICE_MASK(bta_dm_search_cb.service_index);

                uuid.len = LEN_UUID_16;
                uuid.uu.uuid16 = bta_service_id_to_uuid_lkup_tbl[bta_dm_search_cb.service_index];

                if (uuid.uu.uuid16 == UUID_SERVCLASS_PNP_INFORMATION)
                    num_attrs = 2;

                SDP_InitDiscoveryDb(bta_dm_search_cb.p_sdp_db, BTA_DM_SDP_DB_SIZE, 1, &uuid, num_attrs, attr_list);

                if (!SDP_ServiceSearchAttributeRequest(bd_addr, bta_dm_search_cb.p_sdp_db, &bta_dm_sdp_callback)) {
                    GKI_freebuf(bta_dm_search_cb.p_sdp_db);
                    bta_dm_search_cb.p_sdp_db = NULL;
                    bta_dm_search_cb.service_index = BTA_MAX_SERVICE_ID;
                } else {
                    ++bta_dm_search_cb.service_index;
                    return;
                }
            }
        }

        ++bta_dm_search_cb.service_index;
    }

    if (bta_dm_search_cb.service_index >= BTA_MAX_SERVICE_ID) {
        if ((p_msg = GKI_getbuf(sizeof *p_msg)) != NULL) {
            p_msg->hdr.event = BTA_DM_DISCOVERY_RESULT_EVT;
            p_msg->disc_result.result.disc_res.services = bta_dm_search_cb.services_found;

            bdcpy(p_msg->disc_result.result.disc_res.bd_addr, bta_dm_search_cb.peer_bdaddr);
            BCM_STRNCPY_S((char*)p_msg->disc_result.result.disc_res.bd_name, sizeof(BD_NAME), bta_dm_get_remname(), BTA_DM_REMOTE_DEVICE_NAME_LENGTH);

            bta_sys_sendmsg(p_msg);
        }
    }
}

static void bta_dm_discover_next_device(void) {
    tBTA_DM_MSG* p_msg;

    if ((bta_dm_search_cb.p_btm_inq_info = BTM_InqDbNext(bta_dm_search_cb.p_btm_inq_info)) != NULL) {
        if (BTM_IsAclConnectionUp(bta_dm_search_cb.p_btm_inq_info->results.remote_bd_addr)) {
            bta_dm_search_cb.wait_disc = FALSE;
        } else {
            bta_dm_search_cb.wait_disc = TRUE;
        }

        bta_dm_search_cb.sdp_results = FALSE;

        BTM_SecAddRmtNameNotifyCallback(&bta_dm_service_search_remname_cback);

        bta_dm_search_cb.service_index = 0;
        bta_dm_search_cb.services_found = 0;
        bta_dm_search_cb.peer_name[0] = '\0';
        bta_dm_search_cb.services_to_search = bta_dm_search_cb.services;

        bdcpy(bta_dm_search_cb.peer_bdaddr, bta_dm_search_cb.p_btm_inq_info->results.remote_bd_addr);

        bta_dm_find_services(bta_dm_search_cb.p_btm_inq_info->results.remote_bd_addr);
    } else {
        bta_dm_search_cb.services = 0;

        if ((p_msg = GKI_getbuf(sizeof *p_msg)) != NULL) {
            p_msg->hdr.event = BTA_DM_SEARCH_CMPL_EVT;

            bta_sys_sendmsg(p_msg);
        }
    }
}

static void bta_dm_sdp_callback(UINT16 sdp_status) {
    tBTA_DM_SDP_RESULT* p_msg;

    if ((p_msg = GKI_getbuf(sizeof *p_msg)) != NULL) {
        p_msg->hdr.event = BTA_DM_SDP_RESULT_EVT;
        p_msg->sdp_result = sdp_status;
        bta_sys_sendmsg(p_msg);
    }
}

static void bta_dm_inq_results_cb(tBTM_INQ_RESULTS* p_inq) {
    tBTA_DM_SEARCH result;
    tBTM_INQ_INFO* p_inq_info;

    bdcpy(result.inq_res.bd_addr, p_inq->remote_bd_addr);
    memcpy(result.inq_res.dev_class, p_inq->dev_class, DEV_CLASS_LEN);
    result.inq_res.rssi = p_inq->rssi;

    if ((p_inq_info = BTM_InqDbRead(p_inq->remote_bd_addr)) != NULL)
        result.inq_res.remt_name_not_required = FALSE;

    (*bta_dm_search_cb.p_search_cback)(BTA_DM_INQ_RES_EVT, &result);

    if (p_inq_info)
        p_inq_info->appl_knows_rem_name = result.inq_res.remt_name_not_required;
}

static void bta_dm_inq_cmpl_cb(void* p_result) {
    tBTA_DM_MSG* p_msg;

    if ((p_msg = GKI_getbuf(sizeof *p_msg)) != NULL) {
        p_msg->inq_cmpl.hdr.event = BTA_DM_INQUIRY_CMPL_EVT;
        p_msg->inq_cmpl.num = ((tBTM_INQUIRY_CMPL*)p_result)->num_resp;
        bta_sys_sendmsg(p_msg);
    }
}

static void bta_dm_service_search_remname_cback(BD_ADDR bd_addr, DEV_CLASS dc, tBTM_BD_NAME bd_name) {
    BCM_STRNCPY_S((char*)bta_dm_search_cb.peer_name, sizeof(BD_NAME), (char*)bd_name, BTA_DM_REMOTE_DEVICE_NAME_LENGTH - 1);
    BTM_SecDeleteRmtNameNotifyCallback(&bta_dm_service_search_remname_cback);
}

static void bta_dm_remname_cback(tBTM_REMOTE_DEV_NAME* p_remote_name) {
    tBTA_DM_DISC_RESULT* p_msg;

    bta_dm_stop_remote_name_response_timer();

    if ((p_msg = GKI_getbuf(sizeof *p_msg)) != NULL) {
        bdcpy(p_msg->result.disc_res.bd_addr, bta_dm_search_cb.p_btm_inq_info->results.remote_bd_addr);
        BCM_STRNCPY_S((char*)p_msg->result.disc_res.bd_name, sizeof(BD_NAME), (char*)p_remote_name->remote_bd_name, BTA_DM_REMOTE_DEVICE_NAME_LENGTH);

        p_msg->hdr.event = BTA_DM_REMT_NAME_EVT;
        bta_sys_sendmsg(p_msg);
    }
}

static void bta_dm_disc_remname_cback(tBTM_REMOTE_DEV_NAME* p_remote_name) {
    tBTA_DM_DISC_RESULT* p_msg;

    bta_dm_stop_remote_name_response_timer();

    if ((p_msg = GKI_getbuf(sizeof *p_msg)) != NULL) {
        bdcpy(p_msg->result.disc_res.bd_addr, bta_dm_search_cb.peer_bdaddr);
        BCM_STRNCPY_S((char*)p_msg->result.disc_res.bd_name, sizeof(BD_NAME), (char*)p_remote_name->remote_bd_name, BTA_DM_REMOTE_DEVICE_NAME_LENGTH);

        p_msg->hdr.event = BTA_DM_REMT_NAME_EVT;
        bta_sys_sendmsg(p_msg);
    }
}

static void bta_dm_start_remote_name_response_timer(void) {
    if (btm_cb.devcb.page_timeout < 30000) {
        bta_sys_start_timer(&bta_dm_search_cb.rnr_timer, BTA_DM_REMT_NAME_RESP_EVT, btm_cb.devcb.page_timeout + 100);
    }
}

static void bta_dm_stop_remote_name_response_timer(void) {
    bta_sys_stop_timer(&bta_dm_search_cb.rnr_timer);
}

static void bta_dm_remote_name_request_cmd_failed(UINT8* bd_addr) {
    tBTA_DM_DISC_RESULT* p_msg;

    if ((p_msg = GKI_getbuf(sizeof *p_msg)) != NULL) {
        bdcpy(p_msg->result.disc_res.bd_addr, bd_addr);
        p_msg->result.disc_res.bd_name[0] = '\0';

        p_msg->hdr.event = BTA_DM_REMT_NAME_EVT;
        bta_sys_sendmsg(p_msg);
    }
}

void bta_dm_cancel_rmt_name(tBTA_DM_MSG* p_data) {
    BTM_CancelRemoteDeviceName();
}

static tBTM_STATUS bta_dm_authorize_cback(BD_ADDR bd_addr, DEV_CLASS dev_class, BD_NAME bd_name, UINT8* service_name, UINT8 service_id,
                                          BOOLEAN is_originator) {
    tBTA_DM_SEC sec_event;
    UINT8 index = 1;

    bdcpy(sec_event.authorize.bd_addr, bd_addr);

    BCM_STRNCPY_S((char*)sec_event.authorize.bd_name, sizeof(BD_NAME), (char*)bd_name, BTA_DM_REMOTE_DEVICE_NAME_LENGTH);

    while (index <= BTA_MAX_SERVICE_ID) {
        if (bta_service_id_to_btm_srv_id_lkup_tbl[index] == service_id) {
            sec_event.authorize.service = index;
            break;
        }

        ++index;
    }

    if (bta_dm_cb.p_sec_cback && index <= BTA_MAX_SERVICE_ID) {
        (*bta_dm_cb.p_sec_cback)(BTA_DM_AUTHORIZE_EVT, &sec_event);
        return BTM_CMD_STARTED;
    } else {
        return BTM_NOT_AUTHORIZED;
    }
}

static void bta_dm_pinname_cback(void* p_data) {
    tBTM_REMOTE_DEV_NAME* p_result = p_data;
    tBTA_DM_SEC sec_event;
    UINT32 bytes_to_copy;

    bdcpy(sec_event.pin_req.bd_addr, bta_dm_cb.pin_bd_addr);
    BTA_COPY_DEVICE_CLASS(sec_event.pin_req.dev_class, bta_dm_cb.pin_dev_class);

    if (p_result && p_result->status == BTM_SUCCESS) {
        bytes_to_copy = p_result->length < BTA_DM_REMOTE_DEVICE_NAME_LENGTH ? p_result->length : BTA_DM_REMOTE_DEVICE_NAME_LENGTH;
        memcpy(sec_event.pin_req.bd_name, p_result->remote_bd_name, bytes_to_copy);
        sec_event.pin_req.bd_name[BTA_DM_REMOTE_DEVICE_NAME_LENGTH] = '\0';
    } else {
        sec_event.pin_req.bd_name[0] = '\0';
    }

    (*bta_dm_cb.p_sec_cback)(BTA_DM_PIN_REQ_EVT, &sec_event);
}

static tBTM_STATUS bta_dm_pin_cback(BD_ADDR bd_addr, DEV_CLASS dev_class, BD_NAME bd_name) {
    tBTA_DM_SEC sec_event;

    if (!bta_dm_cb.p_sec_cback)
        return BTM_NOT_AUTHORIZED;

    if (bd_name[0] == '\0') {
        bdcpy(bta_dm_cb.pin_bd_addr, bd_addr);
        BTA_COPY_DEVICE_CLASS(bta_dm_cb.pin_dev_class, dev_class);

        if (BTM_ReadRemoteDeviceName(bd_addr, bta_dm_pinname_cback) == BTM_CMD_STARTED) {
            return BTM_CMD_STARTED;
        }

        APPL_TRACE(WARNING, " bta_dm_pin_cback() -> Failed to start Remote Name Request  ");
    }

    bdcpy(sec_event.pin_req.bd_addr, bd_addr);
    BTA_COPY_DEVICE_CLASS(sec_event.pin_req.dev_class, dev_class);
    BCM_STRNCPY_S((char*)sec_event.pin_req.bd_name, sizeof(BD_NAME), (char*)bd_name, BTA_DM_REMOTE_DEVICE_NAME_LENGTH);
    sec_event.pin_req.bd_name[BTA_DM_REMOTE_DEVICE_NAME_LENGTH] = '\0';

    (*bta_dm_cb.p_sec_cback)(BTA_DM_PIN_REQ_EVT, &sec_event);

    return BTM_CMD_STARTED;
}

static tBTM_STATUS bta_dm_link_key_request_cback(BD_ADDR bd_addr, LINK_KEY key) {
    return BTM_NOT_AUTHORIZED;
}

static tBTM_STATUS bta_dm_new_link_key_cback(BD_ADDR bd_addr, DEV_CLASS dev_class, BD_NAME bd_name, LINK_KEY key, UINT8 key_type) {
    tBTA_DM_SEC sec_event;

    bdcpy(sec_event.auth_cmpl.bd_addr, bd_addr);
    memcpy(sec_event.auth_cmpl.bd_name, bd_name, BTA_DM_REMOTE_DEVICE_NAME_LENGTH - 1);
    sec_event.auth_cmpl.bd_name[BTA_DM_REMOTE_DEVICE_NAME_LENGTH - 1] = '\0';
    memcpy(sec_event.auth_cmpl.key, key, LINK_KEY_LEN);
    sec_event.auth_cmpl.key_present = TRUE;
    sec_event.auth_cmpl.success = TRUE;

    if (bta_dm_cb.p_sec_cback)
        (*bta_dm_cb.p_sec_cback)(BTA_DM_AUTH_CMPL_EVT, &sec_event);

    return BTM_CMD_STARTED;
}

static tBTM_STATUS bta_dm_authentication_complete_cback(BD_ADDR bd_addr, DEV_CLASS dev_class, BD_NAME bd_name, int result) {
    tBTA_DM_SEC sec_event;

    if (result != BTM_SUCCESS) {
        bdcpy(sec_event.auth_cmpl.bd_addr, bd_addr);

        memcpy(sec_event.auth_cmpl.bd_name, bd_name, BTA_DM_REMOTE_DEVICE_NAME_LENGTH - 1);
        sec_event.auth_cmpl.bd_name[BTA_DM_REMOTE_DEVICE_NAME_LENGTH - 1] = '\0';
        sec_event.auth_cmpl.success = FALSE;
        sec_event.auth_cmpl.key_present = FALSE;

        if (bta_dm_cb.p_sec_cback)
            (*bta_dm_cb.p_sec_cback)(BTA_DM_AUTH_CMPL_EVT, &sec_event);
    }

    return BTM_SUCCESS;
}

static void bta_dm_local_addr_cback(BD_ADDR bd_addr) {
    if (bta_dm_cb.p_sec_cback)
        (*bta_dm_cb.p_sec_cback)(BTA_DM_INQ_RES_EVT, (tBTA_DM_SEC*)bd_addr);
}

void bta_dm_signal_strength(tBTA_DM_MSG* p_data) {
    if (p_data->sig_strength.start) {
        bta_dm_cb.signal_strength_mask = p_data->sig_strength.mask;
        bta_dm_cb.signal_strength_period = p_data->sig_strength.period;
        bta_dm_signal_strength_timer_cback(NULL);
    } else {
        bta_sys_stop_timer(&bta_dm_cb.signal_strength_timer);
    }
}

static void bta_dm_signal_strength_timer_cback(TIMER_LIST_ENT* p_tle) {
    UINT8 i;

    if (bta_dm_cb.signal_strength_mask & BTA_SIG_STRENGTH_RSSI_MASK) {
        for (i = 0; i < bta_dm_cb.device_list.count; ++i) {
            BTM_ReadRSSI(bta_dm_cb.device_list.peer_device[i].peer_bdaddr, (tBTM_CMPL_CB*)bta_dm_rssi_cback);
        }
    }

    if (bta_dm_cb.signal_strength_mask & BTA_SIG_STRENGTH_LINK_QUALITY_MASK) {
        for (i = 0; i < bta_dm_cb.device_list.count; ++i) {
            BTM_ReadLinkQuality(bta_dm_cb.device_list.peer_device[i].peer_bdaddr, (tBTM_CMPL_CB*)bta_dm_link_quality_cback);
        }
    }

    if (bta_dm_cb.signal_strength_period) {
        bta_dm_cb.signal_strength_timer.p_cback = (TIMER_CBACK*)&bta_dm_signal_strength_timer_cback;

        bta_sys_start_timer(&bta_dm_cb.signal_strength_timer, 0, 1000 * bta_dm_cb.signal_strength_period);
    }
}

static void bta_dm_acl_change_cback(BD_ADDR p_bda, DEV_CLASS p_dc, BD_NAME p_bdn, BD_FEATURES features, BOOLEAN is_new) {
    tBTA_DM_ACL_CHANGE* p_msg;

    if ((p_msg = GKI_getbuf(sizeof *p_msg)) != NULL) {
        bdcpy(p_msg->bd_addr, p_bda);
        p_msg->is_new = is_new;

        p_msg->hdr.event = BTA_DM_ACL_CHANGE_EVT;
        bta_sys_sendmsg(p_msg);
    }
}

void bta_dm_acl_change(tBTA_DM_MSG* p_data) {
    UINT8 i;
    tBTA_DM_SEC conn;
    BOOLEAN is_new = p_data->acl_change.is_new;
    BD_ADDR_PTR p_bda = p_data->acl_change.bd_addr;

    if (is_new) {
        for (i = 0; i < bta_dm_cb.device_list.count; ++i) {
            if (bdcmp(bta_dm_cb.device_list.peer_device[i].peer_bdaddr, p_bda) == 0) {
                break;
            }
        }

        if (i == bta_dm_cb.device_list.count) {
            bdcpy(bta_dm_cb.device_list.peer_device[bta_dm_cb.device_list.count].peer_bdaddr, p_bda);

            ++bta_dm_cb.device_list.count;
        }

        bta_dm_cb.device_list.peer_device[i].conn_state = BTA_DM_CONNECTED;
        bta_dm_cb.device_list.peer_device[i].pref_role = BTA_ANY_ROLE;
        bdcpy(conn.link_up.bd_addr, p_bda);

        (*bta_dm_cb.p_sec_cback)(BTA_DM_LINK_UP_EVT, &conn);
    } else {
        for (i = 0; i < bta_dm_cb.device_list.count; i++) {
            if (bdcmp(bta_dm_cb.device_list.peer_device[i].peer_bdaddr, p_bda) == 0) {
                for (; i < bta_dm_cb.device_list.count; i++) {
                    memcpy(&bta_dm_cb.device_list.peer_device[i], &bta_dm_cb.device_list.peer_device[i + 1],
                           sizeof bta_dm_cb.device_list.peer_device[i]);
                }

                break;
            }
        }

        --bta_dm_cb.device_list.count;

        if (bta_dm_search_cb.wait_disc && bdcmp(bta_dm_search_cb.peer_bdaddr, p_bda) == 0) {
            bta_dm_search_cb.wait_disc = FALSE;

            if (bta_dm_search_cb.sdp_results) {
                APPL_TRACE(EVENT, " timer stopped  ");
                bta_sys_stop_timer(&bta_dm_search_cb.search_timer);
                bta_dm_discover_next_device();
            }
        }

        if (bta_dm_cb.disabling) {
            if (!BTM_GetNumAclLinks()) {
                bta_dm_cb.disabling = FALSE;

                bta_sys_stop_timer(&bta_dm_cb.disable_timer);
                bta_dm_cb.disable_timer.p_cback = (TIMER_CBACK*)&bta_dm_disable_conn_down_timer_cback;
                bta_sys_start_timer(&bta_dm_cb.disable_timer, 0, 1000);
            }
        }

        bdcpy(conn.link_down.bd_addr, p_bda);
        conn.link_down.status = btm_get_acl_disc_reason_code();

        (*bta_dm_cb.p_sec_cback)(BTA_DM_LINK_DOWN_EVT, &conn);
    }
}

static void bta_dm_disable_conn_down_timer_cback(TIMER_LIST_ENT* p_tle) {
    (*bta_dm_cb.p_sec_cback)(BTA_DM_DISABLE_EVT, NULL);
}

static void bta_dm_rssi_cback(tBTM_RSSI_RESULTS* p_result) {
    tBTA_DM_SIG_STRENGTH result;

    if (p_result->status == BTM_SUCCESS) {
        bdcpy(result.bd_addr, p_result->rem_bda);
        result.mask = BTA_SIG_STRENGTH_RSSI_MASK;
        result.rssi_value = p_result->rssi;

        (*bta_dm_cb.p_sec_cback)(BTA_DM_SIG_STRENGTH_EVT, (tBTA_DM_SEC*)&result);
    }
}

static void bta_dm_link_quality_cback(tBTM_LINK_QUALITY_RESULTS* p_result) {
    tBTA_DM_SIG_STRENGTH result;

    if (p_result->status == BTM_SUCCESS) {
        bdcpy(result.bd_addr, p_result->rem_bda);
        result.mask = BTA_SIG_STRENGTH_LINK_QUALITY_MASK;
        result.link_quality_value = p_result->link_quality;

        (*bta_dm_cb.p_sec_cback)(BTA_DM_SIG_STRENGTH_EVT, (tBTA_DM_SEC*)&result);
    }
}

BOOLEAN bta_sys_check_compress(unk_t param_1, UINT8 app_id, BD_ADDR peer_addr) {
    BOOLEAN status = FALSE;
    int i;

    for (i = 0; i < BTA_DM_NUM_COMPRESS_SRVS; ++i) {
        if (bta_dm_compress_srvcs[i].state == BTA_COMPRESS_STATE_1 && bdcmp(bta_dm_compress_srvcs[i].peer_bdaddr, peer_addr) == 0 &&
            bta_dm_compress_srvcs[i].app_id == app_id) {
            APPL_TRACE(EVENT, "bta_sys_check_compress OK, BTA ID %d", bta_dm_compress_srvcs[i].id);

            status = TRUE;
            break;
        }
    }

    return status;
}

static BOOLEAN bta_dm_l2cap_server_compress_cback(BD_ADDR peer_addr, unk_t param_2, unk_t param_3, unk_t param_4, unk_t param_5, unk_t param_6,
                                                  unk_t param_7, UINT8** p_mem_pool, UINT32* mem_pool_size) {
    BOOLEAN status = FALSE;
    int i;

    for (i = 0; i < BTA_DM_NUM_COMPRESS_SRVS; ++i) {
        if (bta_dm_compress_srvcs[i].state == BTA_COMPRESS_STATE_1 && bdcmp(bta_dm_compress_srvcs[i].peer_bdaddr, peer_addr) == 0) {
            APPL_TRACE(EVENT, "bta_dm_l2cap_server_compress_cback, BTA ID %d", bta_dm_compress_srvcs[i].id);

            status = bta_dm_co_get_compress_memory(bta_dm_compress_srvcs[i].id, p_mem_pool, mem_pool_size);
            break;
        }
    }

    return status;
}

static void bta_dm_init_compress(void) {
    memset(bta_dm_compress_srvcs, 0, sizeof bta_dm_compress_srvcs);
    bta_sys_compress_register(&bta_dm_compress_cback);
    L2CA_RegisterCompression(&bta_dm_l2cap_server_compress_cback, 0);
}

static void bta_dm_compress_cback(tBTA_SYS_CONN_STATUS status, UINT8 id, UINT8 app_id, BD_ADDR peer_addr) {
    // Yes, they are defined like this.
    UINT8 j;
    UINT8 k;
    UINT8 i;
    BOOLEAN disallow = FALSE;

    if (status == BTA_SYS_CONN_OPEN) {
        for (j = 1; j <= p_bta_dm_compress_cfg->app_id; ++j) {
            if ((app_id == p_bta_dm_compress_cfg[j].app_id || BTA_ALL_APP_ID == p_bta_dm_compress_cfg[j].app_id) &&
                id == p_bta_dm_compress_cfg[j].id && p_bta_dm_compress_cfg[j].mask == 1) {
                disallow = FALSE;

                for (k = 1; k <= p_bta_dm_compress_cfg->app_id; ++k) {
                    if (p_bta_dm_compress_cfg[k].mask == 2) {
                        for (i = 0; i < bta_dm_conn_srvcs.count; ++i) {
                            if (bta_dm_conn_srvcs.conn_srvc[i].id == p_bta_dm_compress_cfg[k].id) {
                                disallow = TRUE;
                            }
                        }
                    }
                }

                if (disallow)
                    return;

                bta_dm_compress_srvcs[j - 1].app_id = app_id;
                bta_dm_compress_srvcs[j - 1].id = id;
                bdcpy(bta_dm_compress_srvcs[j - 1].peer_bdaddr, peer_addr);
                bta_dm_compress_srvcs[j - 1].state = BTA_COMPRESS_STATE_1;

                APPL_TRACE(EVENT, "bta_dm_compress_cback open app_id %d, BTA id %d, state %d", bta_dm_compress_srvcs[j - 1].app_id,
                           bta_dm_compress_srvcs[j - 1].id, bta_dm_compress_srvcs[j - 1].state);

                break;
            }
        }
    } else if (status == BTA_SYS_CONN_CLOSE) {
        for (j = 1; j <= p_bta_dm_compress_cfg->app_id; ++j) {
            if ((app_id == p_bta_dm_compress_cfg[j].app_id || BTA_ALL_APP_ID == p_bta_dm_compress_cfg[j].app_id) &&
                id == p_bta_dm_compress_cfg[j].id) {
                bta_dm_compress_srvcs[j - 1].state = BTA_COMPRESS_STATE_0;

                APPL_TRACE(EVENT,
                           "bta_dm_compress_cback close app_id %d, BTA id %d, "
                           "state %d",
                           bta_dm_compress_srvcs[j - 1].app_id, bta_dm_compress_srvcs[j - 1].id, bta_dm_compress_srvcs[j - 1].state);

                break;
            }
        }
    }
}

static void bta_dm_rm_cback(tBTA_SYS_CONN_STATUS status, UINT8 id, UINT8 app_id, BD_ADDR peer_addr) {
    UINT8 i;
    UINT8 j;
    tBTA_PREF_ROLES role;

    if (status == BTA_SYS_CONN_OPEN) {
        for (i = 0; i < bta_dm_cb.device_list.count; ++i) {
            if (bdcmp(bta_dm_cb.device_list.peer_device[i].peer_bdaddr, peer_addr) == 0) {
                bta_dm_cb.device_list.peer_device[i].conn_state = BTA_DM_CONNECTED;

                for (j = 1; j <= p_bta_dm_rm_cfg->app_id; ++j) {
                    if ((app_id == p_bta_dm_rm_cfg[j].app_id || BTA_ALL_APP_ID == p_bta_dm_rm_cfg[j].app_id) && id == p_bta_dm_rm_cfg[j].id) {
                        role = p_bta_dm_rm_cfg[j].cfg;

                        if (role > bta_dm_cb.device_list.peer_device[i].pref_role) {
                            bta_dm_cb.device_list.peer_device[i].pref_role = role;
                        }

                        break;
                    }
                }

                break;
            }
        }
    }
}

static void bta_dm_adjust_roles(BOOLEAN delay_role_switch) {
    UINT8 i;
    BOOLEAN set_master_role = FALSE;

    if (bta_dm_cb.device_list.count) {
        if (p_bta_dm_rm_cfg->cfg == BTA_DM_NO_SCATTERNET) {
            L2CA_SetDesireRole(HCI_ROLE_MASTER);
            set_master_role = TRUE;
        }

        for (i = 0; i < bta_dm_cb.device_list.count; ++i) {
            if (bta_dm_cb.device_list.peer_device[i].conn_state == BTA_DM_CONNECTED) {
                if (!set_master_role && bta_dm_cb.device_list.peer_device[i].pref_role != BTA_ANY_ROLE &&
                    p_bta_dm_rm_cfg->cfg == BTA_DM_PARTIAL_SCATTERNET) {
                    L2CA_SetDesireRole(HCI_ROLE_MASTER);
                    set_master_role = TRUE;
                }

                if (bta_dm_cb.device_list.peer_device[i].pref_role == BTA_MASTER_ROLE_ONLY || bta_dm_cb.device_list.count > 1) {
                    BTM_SwitchRole(bta_dm_cb.device_list.peer_device[i].peer_bdaddr, HCI_ROLE_MASTER, NULL);
                }
            }
        }

        if (!set_master_role)
            L2CA_SetDesireRole(L2CAP_DESIRED_LINK_ROLE);
    } else {
        L2CA_SetDesireRole(L2CAP_DESIRED_LINK_ROLE);
    }
}

static char* bta_dm_get_remname(void) {
    char* p_name = bta_dm_search_cb.peer_name;
    char* p_temp;

    if (*p_name == '\0') {
        if ((p_temp = BTM_SecReadDevName(bta_dm_search_cb.peer_bdaddr)) != NULL)
            p_name = p_temp;
    }

    return p_name;
}

void bta_dm_keep_acl(tBTA_DM_MSG* p_data) {
    bta_dm_cb.keep_acl_on_shut_down = p_data->hdr.layer_specific;

    if (bta_dm_cb.keep_acl_on_shut_down) {
        L2CA_SetIdleTimeoutByBdAddr((BD_ADDR_PTR)BT_BD_ANY, 0xffff);
        L2CA_SetIdleTimeout(0, 0xffff, TRUE);
    } else {
        L2CA_SetIdleTimeoutByBdAddr((BD_ADDR_PTR)BT_BD_ANY, 2);
        L2CA_SetIdleTimeout(0, 2, TRUE);
    }
}

void bta_dm_immediate_disable(void) {
    (*bta_dm_cb.p_sec_cback)(BTA_DM_DISABLE_EVT, NULL);
}

void bta_dm_reset_complete(void* p1) {
    /* ... */
}

void bta_dm_send_hci_reset(tBTA_DM_MSG* p_data) {
    void* p_buf;

    bta_sys_cb.events_disabled = TRUE;

    if ((p_buf = HCI_GET_CMD_BUF(sizeof *p_buf)) != NULL)
        btsnd_hcic_write_scan_enable(p_buf, HCI_NO_SCAN_ENABLED);

    BTM_SendHciReset(&bta_dm_reset_complete);
}
