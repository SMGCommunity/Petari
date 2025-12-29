#include "btm_api.h"

/* Original source:
 * bluedroid <android.googlesource.com/platform/external/bluetooth/bluedroid>
 * stack/btm/btm_sco.c
 */

/******************************************************************************
 *
 *  Copyright (C) 2000-2012 Broadcom Corporation
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
#include "gki.h"
#include "hcidefs.h"
#include "hcimsgs.h"

/*******************************************************************************
 * types
 */

enum {
    SCO_ST_UNUSED,
    SCO_ST_LISTENING,
    SCO_ST_W4_CONN_RSP,
    SCO_ST_CONNECTING,
    SCO_ST_CONNECTED,
    SCO_ST_DISCONNECTING,
    SCO_ST_PEND_UNPARK,
};

/*******************************************************************************
 * local function declarations
 */

static void btm_esco_conn_rsp(UINT16 sco_inx, UINT8 hci_status, BD_ADDR bda, tBTM_ESCO_PARAMS* p_parms);
static tBTM_STATUS btm_send_connect_request(UINT16 acl_handle, tBTM_ESCO_PARAMS* p_setup);

/*******************************************************************************
 * variables
 */

// .rodata
static tBTM_ESCO_PARAMS const btm_esco_defaults = {BTM_64KBITS_RATE,
                                                   BTM_64KBITS_RATE,
                                                   0x000a,
                                                   0x0060,
                                                   BTM_SCO_PKT_TYPES_MASK_HV1 | BTM_SCO_PKT_TYPES_MASK_HV2 | BTM_SCO_PKT_TYPES_MASK_HV3 |
                                                       BTM_SCO_PKT_TYPES_MASK_EV3 | BTM_SCO_PKT_TYPES_MASK_EV4 | BTM_SCO_PKT_TYPES_MASK_EV5,
                                                   BTM_ESCO_RETRANS_POWER};

/*******************************************************************************
 * functions
 */

void btm_sco_init(void) {
    btm_cb.sco_cb.sco_disc_reason = BTM_INVALID_SCO_DISC_REASON;
    btm_cb.sco_cb.def_esco_parms = btm_esco_defaults;
    btm_cb.sco_cb.desired_sco_mode = BTM_DEFAULT_SCO_MODE;
}

static void btm_esco_conn_rsp(UINT16 sco_inx, UINT8 hci_status, BD_ADDR bda, tBTM_ESCO_PARAMS* p_parms) {
    tSCO_CONN* p_sco = NULL;
    tBTM_ESCO_PARAMS* p_setup;
    BT_HDR* p_buf;
    UINT16 temp_pkt_types;

    if ((p_buf = HCI_GET_CMD_BUF(HCIC_PARAM_SIZE_REJECT_ESCO)) == NULL) {
        BTM_TRACE(ERROR, "btm_esco_conn_rsp -> No Resources");
        return;
    }

    if (sco_inx < BTM_MAX_SCO_LINKS)
        p_sco = &btm_cb.sco_cb.sco_db[sco_inx];

    if (hci_status != HCI_SUCCESS) {
        if (p_sco) {
            p_sco->state = p_sco->state == SCO_ST_W4_CONN_RSP ? SCO_ST_LISTENING : SCO_ST_UNUSED;
        }

        if (!btm_cb.sco_cb.esco_supported)
            btsnd_hcic_reject_conn(p_buf, bda, hci_status);
        else
            btsnd_hcic_reject_esco_conn(p_buf, bda, hci_status);
    } else {
        btm_chg_all_acl_pkt_types(TRUE);

        p_sco->state = SCO_ST_CONNECTING;

        if (btm_cb.sco_cb.esco_supported && p_sco->esco.data.link_type == BTM_LINK_TYPE_ESCO) {
            p_setup = &p_sco->esco.setup;

            if (p_parms)
                *p_setup = *p_parms;

            p_setup->packet_types = btm_cb.sco_cb.def_esco_parms.packet_types;

            temp_pkt_types = BTM_SCO_SUPPORTED_PKTS_MASK & p_setup->packet_types & btm_cb.btm_sco_pkt_types_supported;

            if (!(temp_pkt_types & (HCI_ESCO_PKT_TYPES_MASK_EV3 | HCI_ESCO_PKT_TYPES_MASK_EV4 | HCI_ESCO_PKT_TYPES_MASK_EV5))) {
                temp_pkt_types |= HCI_ESCO_PKT_TYPES_MASK_EV3;
            }

            if (btm_cb.devcb.local_version.hci_version >= HCI_PROTO_VERSION_2_0) {
                temp_pkt_types |=
                    (p_setup->packet_types & BTM_SCO_EXCEPTION_PKTS_MASK) | (btm_cb.btm_sco_pkt_types_supported & BTM_SCO_EXCEPTION_PKTS_MASK);
            }

            btsnd_hcic_accept_esco_conn(p_buf, bda, p_setup->tx_bw, p_setup->rx_bw, p_setup->max_latency, p_setup->voice_contfmt,
                                        p_setup->retrans_effort, temp_pkt_types);

            p_setup->packet_types = temp_pkt_types;
        } else {
            btsnd_hcic_accept_conn(p_buf, bda, HCI_ROLE_MASTER);
        }
    }
}

static tBTM_STATUS btm_send_connect_request(UINT16 acl_handle, tBTM_ESCO_PARAMS* p_setup) {
    UINT16 temp_pkt_types;

    btm_chg_all_acl_pkt_types(TRUE);

    if (!btm_cb.sco_cb.esco_supported) {
        if (!btsnd_hcic_add_SCO_conn(acl_handle, BTM_ESCO_2_SCO(p_setup->packet_types))) {
            return BTM_NO_RESOURCES;
        }
    } else {
        temp_pkt_types = BTM_SCO_SUPPORTED_PKTS_MASK & p_setup->packet_types & btm_cb.btm_sco_pkt_types_supported;

        if (btm_cb.devcb.local_version.hci_version >= HCI_PROTO_VERSION_2_0) {
            temp_pkt_types |=
                (p_setup->packet_types & BTM_SCO_EXCEPTION_PKTS_MASK) | (btm_cb.btm_sco_pkt_types_supported & BTM_SCO_EXCEPTION_PKTS_MASK);
        }

        BTM_TRACE(API,
                  "      txbw 0x%x, rxbw 0x%x, lat 0x%x, voice 0x%x, "
                  "retrans 0x%02x, pkt 0x%04x",
                  p_setup->tx_bw, p_setup->rx_bw, p_setup->max_latency, p_setup->voice_contfmt, p_setup->retrans_effort, temp_pkt_types);

        if (!btsnd_hcic_setup_esco_conn(acl_handle, p_setup->tx_bw, p_setup->rx_bw, p_setup->max_latency, p_setup->voice_contfmt,
                                        p_setup->retrans_effort, temp_pkt_types)) {
            return BTM_NO_RESOURCES;
        } else {
            p_setup->packet_types = temp_pkt_types;
        }
    }

    return BTM_CMD_STARTED;
}

void btm_set_sco_ind_cback(tBTM_SCO_IND_CBACK* sco_ind_cb) {
    btm_cb.sco_cb.app_sco_ind_cb = sco_ind_cb;
}

void btm_accept_sco_link(UINT16 sco_inx, tBTM_ESCO_PARAMS* p_setup, tBTM_SCO_CB* p_conn_cb, tBTM_SCO_CB* p_disc_cb) {
    tSCO_CONN* p_sco;

    if (sco_inx >= BTM_MAX_SCO_LINKS)
        return;

    p_sco = &btm_cb.sco_cb.sco_db[sco_inx];
    p_sco->p_conn_cb = p_conn_cb;
    p_sco->p_disc_cb = p_disc_cb;

    BTM_TRACE(DEBUG, "TCS accept SCO: Packet Types 0x%04x", p_setup->packet_types);

    btm_esco_conn_rsp(sco_inx, HCI_SUCCESS, p_sco->esco.data.bd_addr, p_setup);
}

void btm_reject_sco_link(UINT16 sco_inx) {
    btm_esco_conn_rsp(sco_inx, HCI_ERR_HOST_REJECT_RESOURCES, btm_cb.sco_cb.sco_db[sco_inx].esco.data.bd_addr, NULL);
}

tBTM_STATUS BTM_CreateSco(BD_ADDR remote_bda, BOOLEAN is_orig, UINT16 pkt_types, UINT16* p_sco_inx, tBTM_SCO_CB* p_conn_cb, tBTM_SCO_CB* p_disc_cb) {
    tBTM_ESCO_PARAMS* p_setup;
    tSCO_CONN* p = btm_cb.sco_cb.sco_db;
    UINT16 xx;
    UINT16 acl_handle = 0;
    UINT16 temp_pkt_types;
    tBTM_PM_MODE md;
    tBTM_PM_PWR_MD pm;

    *p_sco_inx = BTM_INVALID_SCO_INDEX;

    if (is_orig) {
        if (!remote_bda || ((acl_handle = BTM_GetHCIConnHandle(remote_bda)) == 0xffff)) {
            return BTM_UNKNOWN_ADDR;
        }
    }

    if (remote_bda) {
        /* explicitly post-increment */
        for (xx = 0; xx < BTM_MAX_SCO_LINKS; xx++, p++) {
            if ((p->state == SCO_ST_CONNECTING || p->state == SCO_ST_LISTENING || p->state == SCO_ST_PEND_UNPARK) &&
                memcmp(p->esco.data.bd_addr, remote_bda, BD_ADDR_LEN) == 0) {
                return BTM_BUSY;
            }
        }
    } else {
        /* explicitly post-increment */
        for (xx = 0; xx < BTM_MAX_SCO_LINKS; xx++, p++) {
            if (p->state == SCO_ST_LISTENING && !p->rem_bd_known)
                return BTM_BUSY;
        }
    }

    /* explicitly post-increment */
    for (xx = 0, p = btm_cb.sco_cb.sco_db; xx < BTM_MAX_SCO_LINKS; xx++, p++) {
        if (p->state == SCO_ST_UNUSED) {
            if (remote_bda) {
                if (is_orig) {
                    if (BTM_ReadPowerMode(remote_bda, &md) == BTM_SUCCESS) {
                        if (md == BTM_PM_MD_PARK || md == BTM_PM_MD_SNIFF) {
                            pm.mode = BTM_PM_MD_ACTIVE;
                            BTM_SetPowerMode(BTM_PM_SET_ONLY_ID, remote_bda, &pm);

                            p->state = SCO_ST_PEND_UNPARK;
                        }
                    }
                }

                memcpy(p->esco.data.bd_addr, remote_bda, BD_ADDR_LEN);
                p->rem_bd_known = TRUE;
            } else {
                p->rem_bd_known = FALSE;
            }

            if (pkt_types == BTM_IGNORE_SCO_PKT_TYPE)
                pkt_types = btm_cb.sco_cb.def_esco_parms.packet_types;

            p_setup = &p->esco.setup;
            *p_setup = btm_cb.sco_cb.def_esco_parms;
            p_setup->packet_types = btm_cb.sco_cb.desired_sco_mode == BTM_LINK_TYPE_SCO ? pkt_types & BTM_SCO_LINK_ONLY_MASK : pkt_types;

            temp_pkt_types = BTM_SCO_SUPPORTED_PKTS_MASK & p_setup->packet_types & btm_cb.btm_sco_pkt_types_supported;

            if (btm_cb.devcb.local_version.hci_version >= HCI_PROTO_VERSION_2_0) {
                if (btm_cb.sco_cb.desired_sco_mode == HCI_LINK_TYPE_ESCO) {
                    temp_pkt_types |=
                        (p_setup->packet_types & BTM_SCO_EXCEPTION_PKTS_MASK) | (btm_cb.btm_sco_pkt_types_supported & BTM_SCO_EXCEPTION_PKTS_MASK);
                } else {
                    temp_pkt_types |= BTM_SCO_EXCEPTION_PKTS_MASK;
                }
            }

            p_setup->packet_types = temp_pkt_types;
            p->p_conn_cb = p_conn_cb;
            p->p_disc_cb = p_disc_cb;
            p->hci_handle = BTM_INVALID_HCI_HANDLE;

            if (p->state != SCO_ST_PEND_UNPARK) {
                if (is_orig) {
                    BTM_TRACE(API,
                              "BTM_CreateSco -> (e)SCO Link for ACL "
                              "handle 0x%04x, Desired Type %d",
                              acl_handle, btm_cb.sco_cb.desired_sco_mode);

                    if (btm_send_connect_request(acl_handle, p_setup) != BTM_CMD_STARTED) {
                        return BTM_NO_RESOURCES;
                    }

                    p->state = SCO_ST_CONNECTING;
                } else {
                    p->state = SCO_ST_LISTENING;
                }
            }

            *p_sco_inx = xx;

            return BTM_CMD_STARTED;
        }
    }

    return BTM_NO_RESOURCES;
}

// NOTE for future reference: mode is the mode of tHID_DEV_PWR_MD
void btm_sco_chk_pend_unpark(tHCI_STATUS hci_status, tHCI_HANDLE hci_handle, UINT8 mode) {
    UINT16 xx;
    tHCI_HANDLE acl_handle;
    tSCO_CONN* p = btm_cb.sco_cb.sco_db;

    /* explicitly post-increment */
    for (xx = 0; xx < BTM_MAX_SCO_LINKS; xx++, p++) {
        if (p->state == SCO_ST_PEND_UNPARK && mode == 0 && hci_status == HCI_SUCCESS &&
            ((acl_handle = BTM_GetHCIConnHandle(p->esco.data.bd_addr)) == hci_handle)) {
            BTM_TRACE(API,
                      "btm_sco_chk_pend_unpark -> (e)SCO Link for ACL handle "
                      "0x%04x, Desired Type %d",
                      acl_handle, btm_cb.sco_cb.desired_sco_mode);

            if (btm_send_connect_request(acl_handle, &p->esco.setup) == BTM_CMD_STARTED) {
                p->state = SCO_ST_CONNECTING;
            }
        }
    }
}

void btm_sco_conn_req(BD_ADDR bda, DEV_CLASS dev_class, UINT8 link_type) {
    tSCO_CB* p_sco = &btm_cb.sco_cb;
    tSCO_CONN* p = p_sco->sco_db;
    UINT16 xx;
    tBTM_ESCO_CONN_REQ_EVT_DATA evt_data;

    /* explicitly post-increment */
    for (xx = 0; xx < BTM_MAX_SCO_LINKS; xx++, p++) {
        if (((p->state == SCO_ST_LISTENING && p->rem_bd_known) || p->state == SCO_ST_CONNECTING) &&
            memcmp(p->esco.data.bd_addr, bda, BD_ADDR_LEN) == 0) {
            p->rem_bd_known = TRUE;
            p->esco.data.link_type = link_type;
            memcpy(p->esco.data.bd_addr, bda, BD_ADDR_LEN);

            if (!p->esco.p_esco_cback) {
                btm_esco_conn_rsp(xx, HCI_SUCCESS, bda, NULL);
            } else {
                memcpy(evt_data.bd_addr, bda, BD_ADDR_LEN);
                memcpy(evt_data.dev_class, dev_class, DEV_CLASS_LEN);
                evt_data.link_type = link_type;
                evt_data.sco_inx = xx;

                p->state = SCO_ST_W4_CONN_RSP;
                (*p->esco.p_esco_cback)(BTM_ESCO_CONN_REQ_EVT, (tBTM_ESCO_EVT_DATA*)&evt_data);
            }

            return;
        }
    }

    if (btm_cb.sco_cb.app_sco_ind_cb) {
        /* explicitly post-increment */
        for (xx = 0, p = btm_cb.sco_cb.sco_db; xx < BTM_MAX_SCO_LINKS; xx++, p++) {
            if (p->state == SCO_ST_UNUSED) {
                p->is_orig = FALSE;
                p->state = SCO_ST_LISTENING;

                p->esco.data.link_type = link_type;
                memcpy(p->esco.data.bd_addr, bda, BD_ADDR_LEN);
                p->rem_bd_known = TRUE;
                break;
            }
        }

        if (xx < BTM_MAX_SCO_LINKS) {
            btm_cb.sco_cb.app_sco_ind_cb(xx);
            return;
        }
    }

    BTM_TRACE(WARNING, "btm_sco_conn_req: No one wants this SCO connection; rejecting it");

    btm_esco_conn_rsp(BTM_MAX_SCO_LINKS, HCI_ERR_HOST_REJECT_RESOURCES, bda, NULL);
}

void btm_sco_connected(tHCI_STATUS hci_status, BD_ADDR bda, tHCI_HANDLE hci_handle, tBTM_ESCO_DATA* p_esco_data) {
    tSCO_CONN* p = btm_cb.sco_cb.sco_db;
    UINT16 xx;
    BOOLEAN spt = FALSE;
    tBTM_CHG_ESCO_PARAMS parms;

    btm_cb.sco_cb.sco_disc_reason = hci_status;

    /* explicitly post-increment */
    for (xx = 0; xx < BTM_MAX_SCO_LINKS; xx++, p++) {
        if ((p->state == SCO_ST_CONNECTING || p->state == SCO_ST_LISTENING || p->state == SCO_ST_W4_CONN_RSP) && p->rem_bd_known &&
            (!bda || memcmp(p->esco.data.bd_addr, bda, BD_ADDR_LEN) == 0)) {
            if (hci_status != HCI_SUCCESS) {
                if (p->state == SCO_ST_CONNECTING) {
                    if (hci_status != 35) {
                        p->state = SCO_ST_UNUSED;

                        (*p->p_disc_cb)(xx);
                    }
                } else {
                    p->state = SCO_ST_LISTENING;
                }

                return;
            }

            if (p->state == SCO_ST_LISTENING)
                spt = TRUE;

            p->state = SCO_ST_CONNECTED;
            p->hci_handle = hci_handle;

            if (!btm_cb.sco_cb.esco_supported) {
                p->esco.data.link_type = BTM_LINK_TYPE_SCO;

                if (spt) {
                    parms.packet_types = p->esco.setup.packet_types;
                    parms.max_latency = p->esco.setup.max_latency;
                    parms.retrans_effort = p->esco.setup.retrans_effort;

                    BTM_ChangeEScoLinkParms(xx, &parms);
                }
            } else {
                if (p_esco_data)
                    p->esco.data = *p_esco_data;
            }

            (*p->p_conn_cb)(xx);

            return;
        }
    }
}

tBTM_STATUS BTM_RemoveSco(UINT16 sco_inx) {
    tSCO_CONN* p = &btm_cb.sco_cb.sco_db[sco_inx];
    UINT16 tempstate;

    if (sco_inx >= BTM_MAX_SCO_LINKS || p->state == SCO_ST_UNUSED)
        return BTM_UNKNOWN_ADDR;

    if (p->hci_handle == BTM_INVALID_HCI_HANDLE || p->state == SCO_ST_PEND_UNPARK) {
        p->hci_handle = BTM_INVALID_HCI_HANDLE;
        p->state = SCO_ST_UNUSED;
        p->esco.p_esco_cback = NULL;
        return BTM_SUCCESS;
    }

    tempstate = p->state;
    p->state = SCO_ST_DISCONNECTING;

    if (!btsnd_hcic_disconnect(p->hci_handle, HCI_ERR_PEER_USER)) {
        p->state = tempstate;
        return BTM_NO_RESOURCES;
    }

    return BTM_CMD_STARTED;
}

void btm_remove_sco_links(BD_ADDR bda) {
    tSCO_CONN* p = btm_cb.sco_cb.sco_db;
    UINT16 xx;

    /* explicitly post-increment */
    for (xx = 0; xx < BTM_MAX_SCO_LINKS; xx++, p++) {
        if (!p->rem_bd_known)
            continue;

        if (memcmp(p->esco.data.bd_addr, bda, BD_ADDR_LEN) != 0)
            continue;

        BTM_RemoveSco(xx);
    }
}

void btm_sco_removed(UINT16 hci_handle, UINT8 reason) {
    tSCO_CONN* p = btm_cb.sco_cb.sco_db;
    UINT16 xx;

    btm_cb.sco_cb.sco_disc_reason = reason;

    if (btm_num_sco_links_active() <= 1)
        btm_chg_all_acl_pkt_types(FALSE);

    p = btm_cb.sco_cb.sco_db;

    /* explicitly post-increment */
    for (xx = 0; xx < BTM_MAX_SCO_LINKS; xx++, p++) {
        if (p->state == SCO_ST_UNUSED || p->state == SCO_ST_LISTENING)
            continue;

        if (p->hci_handle != hci_handle)
            continue;

        p->state = SCO_ST_UNUSED;
        p->hci_handle = BTM_INVALID_HCI_HANDLE;
        p->rem_bd_known = FALSE;
        p->esco.p_esco_cback = NULL;
        (*p->p_disc_cb)(xx);

        return;
    }
}

void btm_sco_acl_removed(BD_ADDR bda) {
    tSCO_CONN* p = btm_cb.sco_cb.sco_db;
    UINT16 xx;

    /* explicitly post-increment */
    for (xx = 0; xx < BTM_MAX_SCO_LINKS; xx++, p++) {
        if (p->state == SCO_ST_UNUSED)
            continue;

        if (bda && (memcmp(p->esco.data.bd_addr, bda, BD_ADDR_LEN) != 0 || !p->rem_bd_known)) {
            continue;
        }

        p->state = SCO_ST_UNUSED;
        p->esco.p_esco_cback = NULL;
        (*p->p_disc_cb)(xx);
    }
}

tBTM_STATUS BTM_SetScoPacketTypes(UINT16 sco_inx, UINT16 pkt_types) {
    tBTM_CHG_ESCO_PARAMS parms;
    tSCO_CONN* p;

    if (sco_inx >= BTM_MAX_SCO_LINKS)
        return BTM_UNKNOWN_ADDR;

    p = &btm_cb.sco_cb.sco_db[sco_inx];
    parms.packet_types = pkt_types;

    parms.max_latency = p->esco.setup.max_latency;
    parms.retrans_effort = p->esco.setup.retrans_effort;

    return BTM_ChangeEScoLinkParms(sco_inx, &parms);
}

UINT16 BTM_ReadScoPacketTypes(UINT16 sco_inx) {
    tSCO_CONN* p = &btm_cb.sco_cb.sco_db[sco_inx];

    if (sco_inx < BTM_MAX_SCO_LINKS && p->state == SCO_ST_CONNECTED)
        return p->esco.setup.packet_types;
    else
        return 0;
}

UINT16 BTM_ReadScoDiscReason(void) {
    UINT16 res = btm_cb.sco_cb.sco_disc_reason;

    btm_cb.sco_cb.sco_disc_reason = BTM_INVALID_SCO_DISC_REASON;

    return res;
}

UINT16 BTM_ReadDeviceScoPacketTypes(void) {
    return btm_cb.btm_sco_pkt_types_supported;
}

void btm_route_sco_data(BT_HDR* p_msg) {
    GKI_freebuf(p_msg);
}

UINT16 BTM_ReadScoHandle(UINT16 sco_inx) {
    tSCO_CONN* p = &btm_cb.sco_cb.sco_db[sco_inx];

    if (sco_inx < BTM_MAX_SCO_LINKS && p->state == SCO_ST_CONNECTED)
        return p->hci_handle;
    else
        return BTM_INVALID_HCI_HANDLE;
}

BD_ADDR_PTR BTM_ReadScoBdAddr(UINT16 sco_inx) {
    tSCO_CONN* p = &btm_cb.sco_cb.sco_db[sco_inx];

    if (sco_inx < BTM_MAX_SCO_LINKS && p->rem_bd_known)
        return p->esco.data.bd_addr;
    else
        return NULL;
}

tBTM_STATUS BTM_SetEScoMode(tBTM_SCO_TYPE sco_mode, tBTM_ESCO_PARAMS* p_parms) {
    tSCO_CB* p_esco = &btm_cb.sco_cb;
    tBTM_ESCO_PARAMS* p_def = &p_esco->def_esco_parms;

    if (p_esco->esco_supported) {
        if (p_parms) {
            if (sco_mode == BTM_LINK_TYPE_ESCO) {
                *p_def = *p_parms;
            } else {
                p_def->packet_types = p_parms->packet_types;

                if (btm_cb.devcb.local_version.hci_version >= HCI_PROTO_VERSION_2_0) {
                    p_def->packet_types |= BTM_SCO_EXCEPTION_PKTS_MASK;
                }
            }
        }

        p_esco->desired_sco_mode = sco_mode;
        BTM_TRACE(API, "BTM_SetEScoMode -> mode %d", sco_mode);
    } else {
        p_esco->desired_sco_mode = BTM_LINK_TYPE_SCO;
        p_def->packet_types &= BTM_SCO_LINK_ONLY_MASK;
        p_def->retrans_effort = 0;
        BTM_TRACE(API, "BTM_SetEScoMode -> mode SCO (eSCO not supported)");
    }

    BTM_TRACE(DEBUG,
              "    txbw 0x%08x, rxbw 0x%08x, max_lat 0x%04x, voice "
              "0x%04x, pkt 0x%04x, rtx effort 0x%02x",
              p_def->tx_bw, p_def->rx_bw, p_def->max_latency, p_def->voice_contfmt, p_def->packet_types, p_def->retrans_effort);

    return BTM_SUCCESS;
}

tBTM_STATUS BTM_RegForEScoEvts(UINT16 sco_inx, tBTM_ESCO_CBACK* p_esco_cback) {
    if (!btm_cb.sco_cb.esco_supported) {
        btm_cb.sco_cb.sco_db[sco_inx].esco.p_esco_cback = NULL;
        return BTM_MODE_UNSUPPORTED;
    }

    if (sco_inx < BTM_MAX_SCO_LINKS && btm_cb.sco_cb.sco_db[sco_inx].state != SCO_ST_UNUSED) {
        btm_cb.sco_cb.sco_db[sco_inx].esco.p_esco_cback = p_esco_cback;
        return BTM_SUCCESS;
    }

    return BTM_ILLEGAL_VALUE;
}

tBTM_STATUS BTM_ReadEScoLinkParms(UINT16 sco_inx, tBTM_ESCO_DATA* p_parms) {
    BTM_TRACE(API, "BTM_ReadEScoLinkParms -> sco_inx 0x%04x", sco_inx);

    if (sco_inx < BTM_MAX_SCO_LINKS && btm_cb.sco_cb.sco_db[sco_inx].state >= SCO_ST_CONNECTED) {
        *p_parms = btm_cb.sco_cb.sco_db[sco_inx].esco.data;
        return BTM_SUCCESS;
    }

    memset(p_parms, 0, sizeof *p_parms);
    return BTM_WRONG_MODE;
}

tBTM_STATUS BTM_ChangeEScoLinkParms(UINT16 sco_inx, tBTM_CHG_ESCO_PARAMS* p_parms) {
    tBTM_ESCO_PARAMS* p_setup;
    tSCO_CONN* p_sco;
    UINT16 temp_pkt_types;

    if (sco_inx >= BTM_MAX_SCO_LINKS || btm_cb.sco_cb.sco_db[sco_inx].state != SCO_ST_CONNECTED) {
        return BTM_WRONG_MODE;
    }

    p_sco = &btm_cb.sco_cb.sco_db[sco_inx];
    p_setup = &p_sco->esco.setup;

    if (p_sco->esco.data.link_type == BTM_LINK_TYPE_SCO || !btm_cb.sco_cb.esco_supported) {
        p_setup->packet_types = p_parms->packet_types & (btm_cb.btm_sco_pkt_types_supported & BTM_SCO_LINK_ONLY_MASK);

        BTM_TRACE(API, "BTM_ChangeEScoLinkParms -> SCO Link for handle 0x%04x, pkt 0x%04x", p_sco->hci_handle, p_setup->packet_types);

        if (!btsnd_hcic_change_conn_type(p_sco->hci_handle, BTM_ESCO_2_SCO(p_setup->packet_types))) {
            return BTM_NO_RESOURCES;
        }
    } else {
        temp_pkt_types = BTM_SCO_SUPPORTED_PKTS_MASK & p_parms->packet_types & btm_cb.btm_sco_pkt_types_supported;

        if (btm_cb.devcb.local_version.hci_version >= HCI_PROTO_VERSION_2_0) {
            temp_pkt_types |=
                ((p_parms->packet_types & BTM_SCO_EXCEPTION_PKTS_MASK) | (btm_cb.btm_sco_pkt_types_supported & BTM_SCO_EXCEPTION_PKTS_MASK));
        }

        BTM_TRACE(API, "BTM_ChangeEScoLinkParms -> eSCO Link for handle 0x%04x", p_sco->hci_handle);
        BTM_TRACE(API,
                  "      txbw 0x%x, rxbw 0x%x, lat 0x%x, voice 0x%x, retrans "
                  "0x%02x, pkt 0x%04x",
                  p_setup->tx_bw, p_setup->rx_bw, p_parms->max_latency, p_setup->voice_contfmt, p_parms->retrans_effort, temp_pkt_types);

        if (!btsnd_hcic_setup_esco_conn(p_sco->hci_handle, p_setup->tx_bw, p_setup->rx_bw, p_parms->max_latency, p_setup->voice_contfmt,
                                        p_parms->retrans_effort, temp_pkt_types)) {
            return BTM_NO_RESOURCES;
        } else {
            p_parms->packet_types = temp_pkt_types;
        }
    }

    return BTM_CMD_STARTED;
}

void BTM_EScoConnRsp(UINT16 sco_inx, UINT8 hci_status, tBTM_ESCO_PARAMS* p_parms) {
    if (sco_inx >= BTM_MAX_SCO_LINKS)
        return;

    if (btm_cb.sco_cb.sco_db[sco_inx].state != SCO_ST_W4_CONN_RSP)
        return;

    btm_esco_conn_rsp(sco_inx, hci_status, btm_cb.sco_cb.sco_db[sco_inx].esco.data.bd_addr, p_parms);
}

tBTM_SCO_TYPE btm_read_def_esco_mode(tBTM_ESCO_PARAMS* p_parms) {
    *p_parms = btm_cb.sco_cb.def_esco_parms;

    return btm_cb.sco_cb.desired_sco_mode;
}

void btm_esco_proc_conn_chg(UINT8 status, UINT16 handle, UINT8 tx_interval, UINT8 retrans_window, UINT16 rx_pkt_len, UINT16 tx_pkt_len) {
    tSCO_CONN* p = btm_cb.sco_cb.sco_db;
    tBTM_CHG_ESCO_EVT_DATA data;
    UINT16 xx;

    BTM_TRACE(EVENT, "btm_esco_proc_conn_chg -> handle 0x%04x, status 0x%02x", handle, status);

    /* explicitly post-increment */
    for (xx = 0; xx < BTM_MAX_SCO_LINKS; xx++, p++) {
        if (p->state != SCO_ST_CONNECTED)
            continue;

        if (handle != p->hci_handle)
            continue;

        if (p->esco.p_esco_cback) {
            memcpy(data.bd_addr, p->esco.data.bd_addr, BD_ADDR_LEN);
            data.hci_status = status;
            data.sco_inx = xx;
            data.rx_pkt_len = p->esco.data.rx_pkt_len = rx_pkt_len;
            data.tx_pkt_len = p->esco.data.tx_pkt_len = tx_pkt_len;
            data.tx_interval = p->esco.data.tx_interval = tx_interval;
            data.retrans_window = p->esco.data.retrans_window = retrans_window;

            (*p->esco.p_esco_cback)(BTM_ESCO_CHG_EVT, (tBTM_ESCO_EVT_DATA*)&data);
        }

        return;
    }
}

BOOLEAN btm_is_sco_active(UINT16 handle) {
    UINT16 xx;
    tSCO_CONN* p = btm_cb.sco_cb.sco_db;

    /* explicitly post-increment */
    for (xx = 0; xx < BTM_MAX_SCO_LINKS; xx++, p++)
        if (handle == p->hci_handle && p->state == SCO_ST_CONNECTED)
            return TRUE;

    return FALSE;
}

UINT8 btm_num_sco_links_active(void) {
    tSCO_CONN* p = btm_cb.sco_cb.sco_db;
    UINT16 xx;
    UINT8 num_scos = 0;

    /* explicitly post-increment */
    for (xx = 0; xx < BTM_MAX_SCO_LINKS; xx++, p++) {
        switch (p->state) {
        case SCO_ST_W4_CONN_RSP:
        case SCO_ST_CONNECTING:
        case SCO_ST_CONNECTED:
        case SCO_ST_DISCONNECTING:
        case SCO_ST_PEND_UNPARK:
            ++num_scos;
            break;
        }
    }

    return num_scos;
}

BOOLEAN btm_is_any_sco_active(void) {
    UINT16 xx;
    tSCO_CONN* p = btm_cb.sco_cb.sco_db;

    /* explicitly post-increment */
    for (xx = 0; xx < BTM_MAX_SCO_LINKS; xx++, p++)
        if (p->state == SCO_ST_CONNECTED)
            return TRUE;

    return FALSE;
}

BOOLEAN btm_is_sco_active_by_bdaddr(BD_ADDR remote_bda) {
    UINT8 xx;
    tSCO_CONN* p = btm_cb.sco_cb.sco_db;

    /* explicitly post-increment */
    for (xx = 0; xx < BTM_MAX_SCO_LINKS; xx++, p++) {
        if (memcmp(p->esco.data.bd_addr, remote_bda, BD_ADDR_LEN) != 0)
            continue;

        if (p->state != SCO_ST_CONNECTED)
            continue;

        return TRUE;
    }

    return FALSE;
}
