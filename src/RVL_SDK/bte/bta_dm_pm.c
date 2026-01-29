#include "bta_dm_int.h"

/* Original source:
 * bluedroid <android.googlesource.com/platform/external/bluetooth/bluedroid>
 * bta/dm/bta_dm_pm.c
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

#include "bt_trace.h"
#include "bt_types.h"  // BD_ADDR
#include "data_types.h"

#include "bd.h"
#include "bta_api.h"
#include "bta_sys.h"
#include "btm_api.h"
#include "gki.h"  // GKI_getbuf
#include "hcidefs.h"

/*******************************************************************************
 * local function declarations
 */

static void bta_dm_pm_cback(tBTA_SYS_CONN_STATUS status, UINT8 id, UINT8 app_id, BD_ADDR peer_addr);
static void bta_dm_pm_set_mode(BD_ADDR peer_addr, BOOLEAN timed_out);
static BOOLEAN bta_dm_pm_park(BD_ADDR peer_addr);
static BOOLEAN bta_dm_pm_sniff(BD_ADDR peer_addr);
static void bta_dm_pm_active(BD_ADDR peer_addr);
static void bta_dm_pm_btm_cback(BD_ADDR bd_addr, tBTM_PM_STATUS status, UINT16 value, UINT8 hci_status);
static void bta_dm_pm_timer_cback(void* p_tle);

/*******************************************************************************
 * variables
 */

// .bss
tBTA_DM_CONNECTED_SRVCS bta_dm_conn_srvcs;

/*******************************************************************************
 * functions
 */

void bta_dm_init_pm(void) {
    memset(&bta_dm_conn_srvcs, 0, sizeof bta_dm_conn_srvcs);

    if (p_bta_dm_pm_cfg[0].app_id != 0) {
        bta_sys_pm_register(&bta_dm_pm_cback);

        BTM_PmRegister(BTM_PM_REG_SET | BTM_PM_REG_NOTIF, &bta_dm_cb.pm_id, &bta_dm_pm_btm_cback);
    }
}

void bta_dm_disable_pm(void) {
    BTM_PmRegister(BTM_PM_DEREG, &bta_dm_cb.pm_id, NULL);
}

static void bta_dm_pm_cback(tBTA_SYS_CONN_STATUS status, UINT8 id, UINT8 app_id, BD_ADDR peer_addr) {
    UINT8 i;
    UINT8 j;
    UINT8 k;
    UINT16 policy_setting;
    tBTM_STATUS btm_status;
    tBTM_VERSION_INFO vers;

    btm_status = BTM_ReadLocalVersion(&vers);

    if (btm_status == BTM_SUCCESS && vers.manufacturer == LMP_COMPID_BROADCOM && vers.hci_version < HCI_PROTO_VERSION_2_0) {
        if (status == BTA_SYS_SCO_OPEN) {
            policy_setting = bta_dm_cfg.policy_settings & (HCI_ENABLE_MASTER_SLAVE_SWITCH | HCI_ENABLE_HOLD_MODE | HCI_ENABLE_PARK_MODE);
            BTM_SetLinkPolicy(peer_addr, &policy_setting);
        } else if (status == BTA_SYS_SCO_CLOSE) {
            policy_setting = bta_dm_cfg.policy_settings;
            BTM_SetLinkPolicy(peer_addr, &policy_setting);
        }
    }

    for (i = 1; i <= p_bta_dm_pm_cfg[0].app_id; ++i) {
        if (p_bta_dm_pm_cfg[i].id == id && (p_bta_dm_pm_cfg[i].app_id == BTA_ALL_APP_ID || p_bta_dm_pm_cfg[i].app_id == app_id)) {
            break;
        }
    }

    if (i > p_bta_dm_pm_cfg[0].app_id)
        return;

    for (k = 0; k < BTA_DM_NUM_PM_TIMER; ++k) {
        if (bta_dm_cb.pm_timer[k].in_use && bdcmp(bta_dm_cb.pm_timer[k].peer_bdaddr, peer_addr) == 0) {
            bta_sys_stop_timer(&bta_dm_cb.pm_timer[k].timer);
            bta_dm_cb.pm_timer[k].in_use = FALSE;

            break;
        }
    }

    if (p_bta_dm_pm_spec[p_bta_dm_pm_cfg[i].spec_idx].actn_tbl[status][0].power_mode == BTA_DM_PM_NO_ACTION) {
        return;
    }

    for (j = 0; j < bta_dm_conn_srvcs.count; j++) {
        if (bta_dm_conn_srvcs.conn_srvc[j].id == id && bta_dm_conn_srvcs.conn_srvc[j].app_id == app_id &&
            bdcmp(bta_dm_conn_srvcs.conn_srvc[j].peer_bdaddr, peer_addr) == 0) {
            break;
        }
    }

    if (p_bta_dm_pm_spec[p_bta_dm_pm_cfg[i].spec_idx].actn_tbl[status][0].power_mode == BTA_DM_PM_NO_PREF) {
        if (j != bta_dm_conn_srvcs.count) {
            for (; j < bta_dm_conn_srvcs.count; ++j) {
                memcpy(&bta_dm_conn_srvcs.conn_srvc[j], &bta_dm_conn_srvcs.conn_srvc[j + 1], sizeof bta_dm_conn_srvcs.conn_srvc[j]);
            }

            --bta_dm_conn_srvcs.count;
        }
    } else if (j == bta_dm_conn_srvcs.count) {
        if (bta_dm_conn_srvcs.count == BTA_DM_NUM_CONN_SRVS) {
            APPL_TRACE(WARNING, "bta_dm_act no more connected service cbs");
            return;
        } else {
            bta_dm_conn_srvcs.conn_srvc[j].id = id;
            bta_dm_conn_srvcs.conn_srvc[j].app_id = app_id;
            bdcpy(bta_dm_conn_srvcs.conn_srvc[j].peer_bdaddr, peer_addr);

            ++bta_dm_conn_srvcs.count;
        }
    }

    for (i = 0; i < bta_dm_cb.device_list.count; ++i) {
        if (bdcmp(bta_dm_cb.device_list.peer_device[i].peer_bdaddr, peer_addr) == 0) {
            bta_dm_cb.device_list.peer_device[i].pm_mode_attempted = 0;
            bta_dm_cb.device_list.peer_device[i].pm_mode_failed = 0;

            break;
        }
    }

    bta_dm_conn_srvcs.conn_srvc[j].state = status;

    bta_dm_pm_set_mode(peer_addr, FALSE);
}

static void bta_dm_pm_set_mode(BD_ADDR peer_addr, BOOLEAN timed_out) {
    tBTA_DM_PM_ACTION pm_action = BTA_DM_PM_NO_ACTION;
    UINT16 timeout = 0;
    UINT8 i;
    UINT8 j;
    tBTA_DM_PM_ACTION failed_pm = 0;
    tBTA_DM_PEER_DEVICE* p_peer_device = NULL;
    tBTA_DM_PM_ACTION allowed_modes = 0;
    tBTA_DM_PM_ACTION pref_modes = 0;

    if (!bta_dm_cb.device_list.count)
        return;

    for (i = 0; i < bta_dm_cb.device_list.count; ++i) {
        if (bdcmp(bta_dm_cb.device_list.peer_device[i].peer_bdaddr, peer_addr) == 0) {
            failed_pm = bta_dm_cb.device_list.peer_device[i].pm_mode_failed;
            p_peer_device = &bta_dm_cb.device_list.peer_device[i];
            break;
        }
    }

    if (!p_peer_device)
        return;

    for (i = 0; i < bta_dm_conn_srvcs.count; ++i) {
        if (bdcmp(bta_dm_conn_srvcs.conn_srvc[i].peer_bdaddr, peer_addr) == 0) {
            for (j = 1; j <= p_bta_dm_pm_cfg[0].app_id; ++j) {
                if (p_bta_dm_pm_cfg[j].id == bta_dm_conn_srvcs.conn_srvc[i].id &&
                    (p_bta_dm_pm_cfg[j].app_id == BTA_ALL_APP_ID || p_bta_dm_pm_cfg[j].app_id == bta_dm_conn_srvcs.conn_srvc[i].app_id)) {
                    break;
                }
            }

            allowed_modes |= p_bta_dm_pm_spec[p_bta_dm_pm_cfg[j].spec_idx].allow_mask;

            if (!(p_bta_dm_pm_spec[p_bta_dm_pm_cfg[j].spec_idx].actn_tbl[bta_dm_conn_srvcs.conn_srvc[i].state][0].power_mode & failed_pm)) {
                pref_modes |= p_bta_dm_pm_spec[p_bta_dm_pm_cfg[j].spec_idx].actn_tbl[bta_dm_conn_srvcs.conn_srvc[i].state][0].power_mode;

                if (p_bta_dm_pm_spec[p_bta_dm_pm_cfg[j].spec_idx].actn_tbl[bta_dm_conn_srvcs.conn_srvc[i].state][0].power_mode > pm_action) {
                    pm_action = p_bta_dm_pm_spec[p_bta_dm_pm_cfg[j].spec_idx].actn_tbl[bta_dm_conn_srvcs.conn_srvc[i].state][0].power_mode;
                    timeout = p_bta_dm_pm_spec[p_bta_dm_pm_cfg[j].spec_idx].actn_tbl[bta_dm_conn_srvcs.conn_srvc[i].state][0].timeout;
                }
            } else if (!(p_bta_dm_pm_spec[p_bta_dm_pm_cfg[j].spec_idx].actn_tbl[bta_dm_conn_srvcs.conn_srvc[i].state][1].power_mode & failed_pm)) {
                pref_modes |= p_bta_dm_pm_spec[p_bta_dm_pm_cfg[j].spec_idx].actn_tbl[bta_dm_conn_srvcs.conn_srvc[i].state][1].power_mode;

                if (p_bta_dm_pm_spec[p_bta_dm_pm_cfg[j].spec_idx].actn_tbl[bta_dm_conn_srvcs.conn_srvc[i].state][1].power_mode > pm_action) {
                    pm_action = p_bta_dm_pm_spec[p_bta_dm_pm_cfg[j].spec_idx].actn_tbl[bta_dm_conn_srvcs.conn_srvc[i].state][1].power_mode;
                    timeout = p_bta_dm_pm_spec[p_bta_dm_pm_cfg[j].spec_idx].actn_tbl[bta_dm_conn_srvcs.conn_srvc[i].state][1].timeout;
                }
            }
        }
    }

    if ((BTA_DM_PM_PARK | BTA_DM_PM_SNIFF) & pm_action && !(allowed_modes & pm_action)) {
        pm_action = (BTA_DM_PM_PARK | BTA_DM_PM_SNIFF) & allowed_modes & pref_modes;

        if (!pm_action)
            timeout = 0;
    }

    if (!timed_out && timeout) {
        for (i = 0; i < BTA_DM_NUM_PM_TIMER; ++i) {
            if (!bta_dm_cb.pm_timer[i].in_use) {
                bta_dm_cb.pm_timer[i].in_use = TRUE;
                bdcpy(bta_dm_cb.pm_timer[i].peer_bdaddr, peer_addr);
                bta_dm_cb.pm_timer[i].timer.p_cback = &bta_dm_pm_timer_cback;

                bta_sys_start_timer(&bta_dm_cb.pm_timer[i].timer, 0, timeout);

                return;
            }
        }

        if (i == BTA_DM_NUM_PM_TIMER) {
            APPL_TRACE(WARNING, "bta_dm_act no more pm timers");

            return;
        }
    }

    if (pm_action == BTA_DM_PM_NO_ACTION) {
        /* ... */
    } else if (pm_action == BTA_DM_PM_PARK) {
        p_peer_device->pm_mode_attempted = BTA_DM_PM_PARK;
        bta_dm_pm_park(peer_addr);
    } else if (pm_action == BTA_DM_PM_SNIFF) {
        p_peer_device->pm_mode_attempted = BTA_DM_PM_SNIFF;
        bta_dm_pm_sniff(peer_addr);
    } else if (pm_action == BTA_DM_PM_ACTIVE) {
        bta_dm_pm_active(peer_addr);
    }
}

static BOOLEAN bta_dm_pm_park(BD_ADDR peer_addr) {
    tBTM_PM_MODE mode;

    BTM_ReadPowerMode(peer_addr, &mode);

    if (mode != BTM_PM_MD_PARK)
        BTM_SetPowerMode(bta_dm_cb.pm_id, peer_addr, &p_bta_dm_pm_md[1]);

    return TRUE;
}

static BOOLEAN bta_dm_pm_sniff(BD_ADDR peer_addr) {
    tBTM_PM_MODE mode;

    BTM_ReadPowerMode(peer_addr, &mode);

    if (mode != BTM_PM_MD_SNIFF)
        BTM_SetPowerMode(bta_dm_cb.pm_id, peer_addr, &p_bta_dm_pm_md[0]);

    return TRUE;
}

static void bta_dm_pm_active(BD_ADDR peer_addr) {
    tBTM_PM_PWR_MD pm;
    pm.mode = BTM_PM_MD_ACTIVE;

    BTM_SetPowerMode(bta_dm_cb.pm_id, peer_addr, &pm);
}

static void bta_dm_pm_btm_cback(BD_ADDR bd_addr, tBTM_PM_STATUS status, UINT16 value, UINT8 hci_status) {
    tBTA_DM_PM_BTM_STATUS* p_buf;

    if ((p_buf = GKI_getbuf(sizeof *p_buf)) != NULL) {
        p_buf->hdr.event = BTA_DM_PM_BTM_STATUS_EVT;
        p_buf->status = status;
        p_buf->value = value;
        p_buf->hci_status = hci_status;
        bdcpy(p_buf->bd_addr, bd_addr);

        bta_sys_sendmsg(p_buf);
    }
}

static void bta_dm_pm_timer_cback(void* p_tle) {
    tBTA_DM_PM_TIMER* p_buf;
    UINT8 i;

    for (i = 0; i < BTA_DM_NUM_PM_TIMER; ++i) {
        if (bta_dm_cb.pm_timer[i].in_use && &bta_dm_cb.pm_timer[i].timer == p_tle) {
            bta_dm_cb.pm_timer[i].in_use = FALSE;
            break;
        }
    }

    if (i != BTA_DM_NUM_PM_TIMER) {
        if ((p_buf = GKI_getbuf(sizeof *p_buf)) != NULL) {
            p_buf->hdr.event = BTA_DM_PM_TIMER_EVT;
            bdcpy(p_buf->bd_addr, bta_dm_cb.pm_timer[i].peer_bdaddr);

            bta_sys_sendmsg(p_buf);
        }
    }
}

void bta_dm_pm_btm_status(tBTA_DM_MSG* p_data) {
    UINT8 i;

    for (i = 0; i < BTA_DM_NUM_PM_TIMER; ++i) {
        if (bta_dm_cb.pm_timer[i].in_use && bdcmp(bta_dm_cb.pm_timer[i].peer_bdaddr, p_data->pm_status.bd_addr) == 0) {
            bta_sys_stop_timer(&bta_dm_cb.pm_timer[i].timer);
            bta_dm_cb.pm_timer[i].in_use = FALSE;

            break;
        }
    }

    switch (p_data->pm_status.status) {
    case BTM_PM_STS_ACTIVE:
        if (p_data->pm_status.hci_status != 0) {
            for (i = 0; i < bta_dm_cb.device_list.count; ++i) {
                if (bdcmp(bta_dm_cb.device_list.peer_device[i].peer_bdaddr, p_data->pm_status.bd_addr) == 0) {
                    if (!(bta_dm_cb.device_list.peer_device[i].pm_mode_attempted & (BTA_DM_PM_PARK | BTA_DM_PM_SNIFF))) {
                        return;
                    }

                    bta_dm_cb.device_list.peer_device[i].pm_mode_failed |=
                        bta_dm_cb.device_list.peer_device[i].pm_mode_attempted & (BTA_DM_PM_PARK | BTA_DM_PM_SNIFF);

                    bta_dm_pm_set_mode(p_data->pm_status.bd_addr, FALSE);

                    return;
                }
            }

            return;
        }

        bta_dm_pm_set_mode(p_data->pm_status.bd_addr, FALSE);
        break;

        /* NOTE: bluedroid includes other switch cases here guarded by
         * preprocessor conditional blocks that are not active here, hence the
         * single-case switch block.
         */
    }
}

void bta_dm_pm_timer(tBTA_DM_MSG* p_data) {
    bta_dm_pm_set_mode(p_data->pm_status.bd_addr, TRUE);
}
