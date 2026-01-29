#include "btm_api.h"

/* Original source:
 * bluedroid <android.googlesource.com/platform/external/bluetooth/bluedroid>
 * stack/btm/btm_inq.c
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

#include <cstring>
#include <mem.h>
#include <stddef.h>

#include <macros.h>  // BOOLIFY_TERNARY

#include "bt_target.h"
#include "bt_trace.h"
#include "bt_types.h"
#include "data_types.h"
#include "gki_target.h"  // GKI_MAX_BUF_SIZE

#include "btm_int.h"
#include "btu.h"
#include "gki.h"
#include "hcidefs.h"
#include "hcimsgs.h"

/*******************************************************************************
 * local function declarations
 */

static void btm_clr_inq_result_flt(void);
static BOOLEAN btm_inq_find_bdaddr(BD_ADDR p_bda);
static tINQ_DB_ENT* btm_inq_db_find(BD_ADDR p_bda);
static tINQ_DB_ENT* btm_inq_db_new(BD_ADDR p_bda);
static tBTM_STATUS btm_set_inq_event_filter(UINT8 filter_cond_type, tBTM_INQ_FILT_COND* p_filt_cond);

static void btm_initiate_inquiry(tBTM_INQUIRY_VAR_ST* p_inq);

/*******************************************************************************
 * variables
 */

// .sdata2
static LAP const general_inq_lap = {0x9e, 0x8b, 0x33};
static LAP const limited_inq_lap = {0x9e, 0x8b, 0x00};

/*******************************************************************************
 * functions
 */

tBTM_STATUS BTM_SetDiscoverability(tBTM_DISC_MODE inq_mode, UINT16 window, UINT16 interval) {
    UINT8 scan_mode = 0;
    UINT16 service_class;
    UINT8* p_cod;
    UINT8 major;
    UINT8 minor;
    DEV_CLASS cod;
    LAP temp_lap[2];
    void* p_buf;
    BOOLEAN is_limited;
    BOOLEAN cod_limited;

    if (inq_mode != BTM_NON_DISCOVERABLE && inq_mode != BTM_LIMITED_DISCOVERABLE && inq_mode != BTM_GENERAL_DISCOVERABLE) {
        return BTM_ILLEGAL_VALUE;
    }

    if (btm_cb.devcb.state < BTM_DEV_STATE_3)
        return BTM_DEV_RESET;

    if (!window)
        window = BTM_DEFAULT_DISC_WINDOW;

    if (!interval)
        interval = BTM_DEFAULT_DISC_INTERVAL;

    BTM_TRACE(API,
              "BTM_SetDiscoverability: mode %d [NonDisc-0, Lim-1, Gen-2], "
              "window 0x%04x, interval 0x%04x",
              inq_mode, window, interval);

    if (inq_mode != BTM_NON_DISCOVERABLE) {
        if (window < HCI_MIN_INQUIRYSCAN_WINDOW || window > HCI_MAX_INQUIRYSCAN_WINDOW || interval < HCI_MIN_INQUIRYSCAN_INTERVAL ||
            interval > HCI_MAX_INQUIRYSCAN_INTERVAL || window > interval) {
            return BTM_ILLEGAL_VALUE;
        }
    }

    if (inq_mode != BTM_NON_DISCOVERABLE) {
        if ((p_buf = HCI_GET_CMD_BUF(1 + LAP_LEN * 2)) != NULL) {
            if (inq_mode & BTM_LIMITED_DISCOVERABLE) {
                memcpy(temp_lap[0], limited_inq_lap, LAP_LEN);
                memcpy(temp_lap[1], general_inq_lap, LAP_LEN);

                btsnd_hcic_write_cur_iac_lap(p_buf, 2, temp_lap);
            } else {
                btsnd_hcic_write_cur_iac_lap(p_buf, 1, (LAP*)&general_inq_lap);
            }

            scan_mode |= HCI_INQUIRY_SCAN_ENABLED;
        } else {
            return BTM_NO_RESOURCES;
        }
    }

    if (window != btm_cb.btm_inq_vars.inq_scan_window || interval != btm_cb.btm_inq_vars.inq_scan_period) {
        if ((p_buf = HCI_GET_CMD_BUF(HCIC_PARAM_SIZE_WRITE_INQSCAN_CFG)) != NULL) {
            btm_cb.btm_inq_vars.inq_scan_window = window;
            btm_cb.btm_inq_vars.inq_scan_period = interval;
            btsnd_hcic_write_inqscan_cfg(p_buf, interval, window);
        } else {
            return BTM_NO_RESOURCES;
        }
    }

    if ((p_buf = HCI_GET_CMD_BUF(HCIC_PARAM_SIZE_WRITE_PARAM1)) != NULL) {
        if (btm_cb.btm_inq_vars.connectable_mode != BTM_NON_CONNECTABLE)
            scan_mode |= HCI_PAGE_SCAN_ENABLED;

        btm_cb.btm_inq_vars.discoverable_mode = inq_mode;
        btsnd_hcic_write_scan_enable(p_buf, scan_mode);
    } else {
        return BTM_NO_RESOURCES;
    }

    p_cod = BTM_ReadDeviceClass();
    BTM_GET_COD_SERVICE_CLASS(p_cod, &service_class);

    is_limited = BOOLIFY_TERNARY(inq_mode & BTM_LIMITED_DISCOVERABLE);
    cod_limited = BOOLIFY_TERNARY(service_class & BTM_COD_SERVICE_LMTD_DISCOVER);

    if (is_limited ^ cod_limited) {
        BTM_GET_COD_MINOR_CLASS(p_cod, &minor);
        BTM_GET_COD_MAJOR_CLASS(p_cod, &major);

        if (is_limited)
            service_class |= BTM_COD_SERVICE_LMTD_DISCOVER;
        else
            service_class &= ~BTM_COD_SERVICE_LMTD_DISCOVER;

        MAKE_COD_FROM_FIELDS(cod, minor, major, service_class);

        BTM_SetDeviceClass(cod);
    }

    return BTM_SUCCESS;
}

tBTM_STATUS BTM_SetInquiryScanType(tBTM_INQ_SCAN_TYPE scan_type) {
    BT_HDR* p_buf;

    if (scan_type != BTM_SCAN_TYPE_STANDARD && scan_type != BTM_SCAN_TYPE_INTERLACED) {
        return BTM_ILLEGAL_VALUE;
    }

    if (!HCI_LMP_INTERLACED_INQ_SCAN_SUPPORTED(btm_cb.devcb.local_features))
        return BTM_MODE_UNSUPPORTED;

    if (scan_type != btm_cb.btm_inq_vars.inq_scan_type) {
        if (BTM_IsDeviceUp()) {
            if ((p_buf = HCI_GET_CMD_BUF(HCIC_PARAM_SIZE_WRITE_PARAM1)) != NULL) {
                btsnd_hcic_write_inqscan_type(p_buf, scan_type);
                btm_cb.btm_inq_vars.inq_scan_type = scan_type;
            } else {
                return BTM_NO_RESOURCES;
            }
        } else {
            return BTM_WRONG_MODE;
        }
    }

    return BTM_SUCCESS;
}

tBTM_STATUS BTM_SetPageScanType(UINT16 scan_type) {
    BT_HDR* p_buf;

    if (scan_type != BTM_SCAN_TYPE_STANDARD && scan_type != BTM_SCAN_TYPE_INTERLACED) {
        return BTM_ILLEGAL_VALUE;
    }

    if (!HCI_LMP_INTERLACED_PAGE_SCAN_SUPPORTED(btm_cb.devcb.local_features))
        return BTM_MODE_UNSUPPORTED;

    if (scan_type != btm_cb.btm_inq_vars.page_scan_type) {
        if (BTM_IsDeviceUp()) {
            if ((p_buf = HCI_GET_CMD_BUF(HCIC_PARAM_SIZE_WRITE_PARAM1)) != NULL) {
                btsnd_hcic_write_pagescan_type(p_buf, scan_type);
                btm_cb.btm_inq_vars.page_scan_type = scan_type;
            } else {
                return BTM_NO_RESOURCES;
            }
        } else {
            return BTM_WRONG_MODE;
        }
    }

    return BTM_SUCCESS;
}

tBTM_STATUS BTM_SetInquiryMode(tBTM_INQ_RESULT_TYPE mode) {
    BT_HDR* p_buf;

    if (mode != BTM_INQ_RESULT_STANDARD && mode != BTM_INQ_RESULT_WITH_RSSI)
        return BTM_ILLEGAL_VALUE;

    if (!HCI_LMP_INQ_RSSI_SUPPORTED(btm_cb.devcb.local_features))
        return BTM_MODE_UNSUPPORTED;

    if (!BTM_IsDeviceUp())
        return BTM_WRONG_MODE;

    if ((p_buf = HCI_GET_CMD_BUF(HCIC_PARAM_SIZE_WRITE_PARAM1)) != NULL)
        btsnd_hcic_write_inquiry_mode(p_buf, mode);
    else
        return BTM_NO_RESOURCES;

    return BTM_SUCCESS;
}

tBTM_DISC_MODE BTM_ReadDiscoverability(UINT16* p_window, UINT16* p_interval) {
    if (p_window)
        *p_window = btm_cb.btm_inq_vars.inq_scan_window;

    if (p_interval)
        *p_interval = btm_cb.btm_inq_vars.inq_scan_period;

    return btm_cb.btm_inq_vars.discoverable_mode;
}

tBTM_STATUS BTM_SetPeriodicInquiryMode(tBTM_INQ_PARMS* p_inqparms, UINT16 max_delay, UINT16 min_delay, tBTM_INQ_RESULTS_CB* p_results_cb) {
    tBTM_STATUS status;
    tBTM_INQUIRY_VAR_ST* p_inq = &btm_cb.btm_inq_vars;

    BTM_TRACE(API,
              "BTM_SetPeriodicInquiryMode: mode: %d, dur: %d, rsps: %d, flt: "
              "%d, min: %d, max: %d",
              p_inqparms->mode, p_inqparms->duration, p_inqparms->max_resps, p_inqparms->filter_cond_type, min_delay, max_delay);

    if (!BTM_IsDeviceUp())
        return BTM_WRONG_MODE;

    if (p_inq->inq_active || p_inq->inqfilt_active)
        return BTM_BUSY;

    if (p_inqparms->mode != BTM_GENERAL_INQUIRY && p_inqparms->mode != BTM_LIMITED_INQUIRY) {
        return BTM_ILLEGAL_VALUE;
    }

    if (p_inqparms->duration < BTM_MIN_INQUIRY_LENGTH || p_inqparms->duration > BTM_MAX_INQUIRY_LENGTH || min_delay <= p_inqparms->duration ||
        min_delay < BTM_PER_INQ_MIN_MIN_PERIOD || min_delay > BTM_PER_INQ_MAX_MIN_PERIOD || max_delay <= min_delay ||
        max_delay < BTM_PER_INQ_MIN_MAX_PERIOD || max_delay > BTM_PER_INQ_MAX_MAX_PERIOD) {
        return BTM_ILLEGAL_VALUE;
    }

    p_inq->inqparms = *p_inqparms;
    p_inq->per_min_delay = min_delay;
    p_inq->per_max_delay = max_delay;
    p_inq->inq_cmpl_info.num_resp = 0;
    p_inq->p_inq_results_cb = p_results_cb;

    p_inq->inq_active = p_inqparms->mode == BTM_LIMITED_INQUIRY ? BTM_LIMITED_INQUIRY_ACTIVE | BTM_PERIODIC_INQUIRY_ACTIVE :
                                                                  BTM_GENERAL_INQUIRY_ACTIVE | BTM_PERIODIC_INQUIRY_ACTIVE;

    if (p_inqparms->filter_cond_type != BTM_CLR_INQUIRY_FILTER) {
        p_inq->state = BTM_INQ_CLR_FILT_STATE;
        p_inqparms->filter_cond_type = BTM_CLR_INQUIRY_FILTER;
    } else {
        p_inq->state = BTM_INQ_SET_FILT_STATE;
    }

    if ((status = btm_set_inq_event_filter(p_inqparms->filter_cond_type, &p_inqparms->filter_cond)) != BTM_CMD_STARTED) {
        p_inq->p_inq_results_cb = NULL;
        p_inq->state = BTM_INQ_INACTIVE_STATE;
    }

    return status;
}

tBTM_STATUS BTM_CancelPeriodicInquiry(void) {
    tBTM_INQUIRY_VAR_ST* p_inq = &btm_cb.btm_inq_vars;
    tBTM_STATUS status = BTM_SUCCESS;

    BTM_TRACE(API, "BTM_CancelPeriodicInquiry called");

    if (!BTM_IsDeviceUp())
        return BTM_WRONG_MODE;

    if (btm_cb.btm_inq_vars.inq_active & BTM_PERIODIC_INQUIRY_ACTIVE) {
        btm_cb.btm_inq_vars.inq_active = BTM_INQUIRY_INACTIVE;
        btm_cb.btm_inq_vars.p_inq_results_cb = NULL;

        if (!btsnd_hcic_exit_per_inq())
            status = BTM_NO_RESOURCES;

        if (p_inq->inqfilt_active)
            ++p_inq->pending_filt_complete_event;

        p_inq->inqfilt_active = FALSE;
        ++p_inq->inq_counter;
    }

    return status;
}

tBTM_STATUS BTM_SetConnectability(tBTM_CONN_MODE page_mode, UINT16 window, UINT16 interval) {
    void* p_buf;
    tHCI_SCAN_MODE scan_mode = 0;
    tBTM_INQUIRY_VAR_ST* p_inq = &btm_cb.btm_inq_vars;

    if (page_mode != BTM_NON_CONNECTABLE && page_mode != BTM_CONNECTABLE)
        return BTM_ILLEGAL_VALUE;

    if (btm_cb.devcb.state < BTM_DEV_STATE_3)
        return BTM_DEV_RESET;

    if (window == 0)
        window = BTM_DEFAULT_CONN_WINDOW;

    if (interval == 0)
        interval = BTM_DEFAULT_CONN_INTERVAL;

    BTM_TRACE(API,
              "BTM_SetConnectability: mode %d [NonConn-0, Conn-1], window "
              "0x%04x, interval 0x%04x",
              page_mode, window, interval);

    if (page_mode == BTM_CONNECTABLE) {
        if (window < HCI_MIN_PAGESCAN_WINDOW || window > HCI_MAX_PAGESCAN_WINDOW || interval < HCI_MIN_PAGESCAN_INTERVAL ||
            interval > HCI_MAX_PAGESCAN_INTERVAL || window > interval) {
            return BTM_ILLEGAL_VALUE;
        }

        scan_mode |= HCI_PAGE_SCAN_ENABLED;
    }

    if (window != p_inq->page_scan_window || interval != p_inq->page_scan_period) {
        if ((p_buf = HCI_GET_CMD_BUF(HCIC_PARAM_SIZE_WRITE_PAGESCAN_CFG)) != NULL) {
            p_inq->page_scan_window = window;
            p_inq->page_scan_period = interval;
            btsnd_hcic_write_pagescan_cfg(p_buf, interval, window);
        } else {
            return BTM_NO_RESOURCES;
        }
    }

    if ((p_buf = HCI_GET_CMD_BUF(HCIC_PARAM_SIZE_WRITE_PARAM1)) != NULL) {
        if (p_inq->discoverable_mode != BTM_NON_DISCOVERABLE)
            scan_mode |= HCI_INQUIRY_SCAN_ENABLED;

        p_inq->connectable_mode = page_mode;
        btsnd_hcic_write_scan_enable(p_buf, scan_mode);
    } else {
        return BTM_NO_RESOURCES;
    }

    return BTM_SUCCESS;
}

UINT16 BTM_ReadConnectability(UINT16* p_window, UINT16* p_interval) {
    if (p_window)
        *p_window = btm_cb.btm_inq_vars.page_scan_window;

    if (p_interval)
        *p_interval = btm_cb.btm_inq_vars.page_scan_period;

    return btm_cb.btm_inq_vars.connectable_mode;
}

UINT16 BTM_IsInquiryActive(void) {
    return btm_cb.btm_inq_vars.inq_active;
}

tBTM_STATUS BTM_CancelInquiry(void) {
    tBTM_STATUS status = BTM_SUCCESS;
    tBTM_INQUIRY_VAR_ST* p_inq = &btm_cb.btm_inq_vars;

    BTM_TRACE(API, "BTM_CancelInquiry called");

    if (!BTM_IsDeviceUp())
        return BTM_WRONG_MODE;

    if (p_inq->inq_active && !(p_inq->inq_active & BTM_PERIODIC_INQUIRY_ACTIVE)) {
        p_inq->inq_active = BTM_INQUIRY_INACTIVE;
        p_inq->state = BTM_INQ_INACTIVE_STATE;
        p_inq->p_inq_results_cb = NULL;
        p_inq->p_inq_cmpl_cb = NULL;

        if (p_inq->inqfilt_active) {
            p_inq->inqfilt_active = FALSE;
            ++p_inq->pending_filt_complete_event;
        } else {
            if (!btsnd_hcic_inq_cancel())
                status = BTM_NO_RESOURCES;
        }

        ++p_inq->inq_counter;
        btm_clr_inq_result_flt();
    }

    return status;
}

tBTM_STATUS BTM_StartInquiry(tBTM_INQ_PARMS* p_inqparms, tBTM_INQ_RESULTS_CB* p_results_cb, tBTM_CMPL_CB* p_cmpl_cb) {
    tBTM_STATUS status;
    tBTM_INQUIRY_VAR_ST* p_inq = &btm_cb.btm_inq_vars;

    BTM_TRACE(API, "BTM_StartInquiry: mode: %d, dur: %d, rsps: %d, flt: %d", p_inqparms->mode, p_inqparms->duration, p_inqparms->max_resps,
              p_inqparms->filter_cond_type);

    if (p_inq->inq_active != BTM_INQUIRY_INACTIVE || p_inq->inqfilt_active)
        return BTM_BUSY;

    if (p_inqparms->mode != BTM_GENERAL_INQUIRY && p_inqparms->mode != BTM_LIMITED_INQUIRY) {
        return BTM_ILLEGAL_VALUE;
    }

    if (!BTM_IsDeviceUp())
        return BTM_WRONG_MODE;

    p_inq->inqparms = *p_inqparms;

    p_inq->state = BTM_INQ_ACTIVE_STATE;
    p_inq->p_inq_cmpl_cb = p_cmpl_cb;
    p_inq->p_inq_results_cb = p_results_cb;
    p_inq->inq_cmpl_info.num_resp = 0;
    p_inq->inq_active = p_inqparms->mode == BTM_LIMITED_INQUIRY ? BTM_LIMITED_INQUIRY_ACTIVE : BTM_GENERAL_INQUIRY_ACTIVE;

    switch (p_inqparms->filter_cond_type) {
    case BTM_CLR_INQUIRY_FILTER:
        p_inq->state = BTM_INQ_SET_FILT_STATE;
        break;

    case BTM_FILTER_COND_DEVICE_CLASS:
    case BTM_FILTER_COND_BD_ADDR:
        p_inq->state = BTM_INQ_CLR_FILT_STATE;
        p_inqparms->filter_cond_type = BTM_CLR_INQUIRY_FILTER;
        break;

    default:
        return BTM_ILLEGAL_VALUE;
    }

    if ((status = btm_set_inq_event_filter(p_inqparms->filter_cond_type, &p_inqparms->filter_cond)) != BTM_CMD_STARTED) {
        p_inq->state = BTM_INQ_INACTIVE_STATE;
    }

    return status;
}

tBTM_STATUS BTM_ReadRemoteDeviceName(BD_ADDR remote_bda, tBTM_CMPL_CB* p_cb) {
    tBTM_INQ_INFO* p_cur = NULL;
    tINQ_DB_ENT* p_i;

    BTM_TRACE(API, "BTM_ReadRemoteDeviceName: bd addr [%02x%02x%02x%02x%02x%02x]", remote_bda[0], remote_bda[1], remote_bda[2], remote_bda[3],
              remote_bda[4], remote_bda[5]);

    if ((p_i = btm_inq_db_find(remote_bda)) != NULL)
        p_cur = &p_i->inq_info;

    return btm_initiate_rem_name(remote_bda, p_cur, BTM_RMT_NAME_EXT, BTM_EXT_RMT_NAME_TIMEOUT, p_cb);
}

tBTM_STATUS BTM_CancelRemoteDeviceName(void) {
    tBTM_INQUIRY_VAR_ST* p_inq = &btm_cb.btm_inq_vars;

    BTM_TRACE(API, "BTM_CancelRemoteDeviceName()");

    if (p_inq->remname_active & BTM_RMT_NAME_EXT) {
        if (btsnd_hcic_rmt_name_req_cancel(p_inq->remname_bda))
            return BTM_CMD_STARTED;
        else
            return BTM_NO_RESOURCES;
    } else {
        return BTM_WRONG_MODE;
    }
}

tBTM_INQ_INFO* BTM_InqFirstResult(void) {
    UINT16 xx;
    tINQ_DB_ENT* p_ent = btm_cb.btm_inq_vars.inq_db;
    UINT32 cur_inq_count = btm_cb.btm_inq_vars.inq_counter - 1;

    /* explicitly post-increment */
    for (xx = 0; xx < BTM_INQ_DB_SIZE; xx++, p_ent++) {
        if (p_ent->in_use && p_ent->inq_count == cur_inq_count)
            return &p_ent->inq_info;
    }

    return NULL;
}

tBTM_INQ_INFO* BTM_InqNextResult(tBTM_INQ_INFO* p_cur) {
    tINQ_DB_ENT* p_ent;
    UINT16 inx;
    UINT32 cur_inq_count = btm_cb.btm_inq_vars.inq_counter - 1;

    if (p_cur) {
        // containerof
        p_ent = (tINQ_DB_ENT*)((UINT8*)p_cur - offsetof(tINQ_DB_ENT, inq_info));
        inx = p_ent - btm_cb.btm_inq_vars.inq_db + 1;

        /* explicitly post-increment */
        for (p_ent = &btm_cb.btm_inq_vars.inq_db[inx]; inx < BTM_INQ_DB_SIZE; inx++, p_ent++) {
            if (p_ent->in_use && p_ent->inq_count == cur_inq_count)
                return &p_ent->inq_info;
        }

        return NULL;
    } else {
        return BTM_InqDbFirst();
    }
}

tBTM_INQ_INFO* BTM_InqDbRead(BD_ADDR p_bda) {
    UINT16 xx;
    tINQ_DB_ENT* p_ent = btm_cb.btm_inq_vars.inq_db;

    BTM_TRACE(API, "BTM_InqDbRead: bd addr [%02x%02x%02x%02x%02x%02x]", p_bda[0], p_bda[1], p_bda[2], p_bda[3], p_bda[4], p_bda[5]);

    /* explicitly post-increment */
    for (xx = 0; xx < BTM_INQ_DB_SIZE; xx++, p_ent++) {
        if (p_ent->in_use && memcmp(p_ent->inq_info.results.remote_bd_addr, p_bda, BD_ADDR_LEN) == 0) {
            return &p_ent->inq_info;
        }
    }

    return NULL;
}

tBTM_INQ_INFO* BTM_InqDbFirst(void) {
    UINT16 xx;
    tINQ_DB_ENT* p_ent = btm_cb.btm_inq_vars.inq_db;

    /* explicitly post-increment */
    for (xx = 0; xx < BTM_INQ_DB_SIZE; xx++, p_ent++) {
        if (p_ent->in_use)
            return &p_ent->inq_info;
    }

    return NULL;
}

tBTM_INQ_INFO* BTM_InqDbNext(tBTM_INQ_INFO* p_cur) {
    tINQ_DB_ENT* p_ent;
    UINT16 inx;

    if (p_cur) {
        // containerof
        p_ent = (tINQ_DB_ENT*)((UINT8*)p_cur - offsetof(tINQ_DB_ENT, inq_info));
        inx = p_ent - btm_cb.btm_inq_vars.inq_db + 1;

        /* explicitly post-increment */
        for (p_ent = &btm_cb.btm_inq_vars.inq_db[inx]; inx < BTM_INQ_DB_SIZE; inx++, p_ent++) {
            if (p_ent->in_use)
                return &p_ent->inq_info;
        }

        return NULL;
    } else {
        return BTM_InqDbFirst();
    }
}

tBTM_STATUS BTM_ClearInqDb(BD_ADDR p_bda) {
    tBTM_INQUIRY_VAR_ST* p_inq = &btm_cb.btm_inq_vars;

    if (p_inq->inq_active != BTM_INQUIRY_INACTIVE || p_inq->remname_active || p_inq->inqfilt_active) {
        return BTM_BUSY;
    }

    btm_clr_inq_db(p_bda);

    return BTM_SUCCESS;
}

UINT8 BTM_ReadNumInqDbEntries(void) {
    UINT8 num_entries;
    UINT8 num_results;
    tINQ_DB_ENT* p_ent = btm_cb.btm_inq_vars.inq_db;

    /* explicitly post-increment */
    for (num_entries = 0, num_results = 0; num_entries < BTM_INQ_DB_SIZE; num_entries++, p_ent++) {
        if (p_ent->in_use)
            ++num_results;
    }

    return num_results;
}

tBTM_STATUS BTM_InquiryRegisterForChanges(tBTM_INQ_DB_CHANGE_CB* p_cb) {
    if (!p_cb)
        btm_cb.btm_inq_vars.p_inq_change_cb = NULL;
    else if (btm_cb.btm_inq_vars.p_inq_change_cb)
        return BTM_BUSY;
    else
        btm_cb.btm_inq_vars.p_inq_change_cb = p_cb;

    return BTM_SUCCESS;
}

void BTM_SetInquiryFilterCallback(tBTM_FILTER_CB* p_callback) {
    btm_cb.p_inq_filter_cb = p_callback;
}

void btm_inq_db_reset(void) {
    tBTM_REMOTE_DEV_NAME rem_name;
    tBTM_INQUIRY_VAR_ST* p_inq = &btm_cb.btm_inq_vars;
    UINT8 num_responses;
    tBTM_INQ_ACTIVE_STATUS temp_inq_active;
    tBTM_STATUS status;

    btu_stop_timer(&p_inq->inq_timer_ent);

    if (p_inq->inq_active != BTM_INQUIRY_INACTIVE) {
        temp_inq_active = p_inq->inq_active;
        p_inq->inq_active = BTM_INQUIRY_INACTIVE;

        if (temp_inq_active == BTM_LIMITED_INQUIRY_ACTIVE || temp_inq_active == BTM_GENERAL_INQUIRY_ACTIVE) {
            if (p_inq->p_inq_cmpl_cb) {
                num_responses = 0;
                (*p_inq->p_inq_cmpl_cb)(&num_responses);
            }
        }
    }

    if (p_inq->remname_active & BTM_RMT_NAME_EXT) {
        btu_stop_timer(&p_inq->rmt_name_timer_ent);
        p_inq->remname_active = BTM_RMT_NAME_INACTIVE;
        memset(p_inq->remname_bda, 0, BD_ADDR_LEN);

        if (p_inq->p_remname_cmpl_cb) {
            rem_name.status = BTM_DEV_RESET;

            (*p_inq->p_remname_cmpl_cb)(&rem_name);
            p_inq->p_remname_cmpl_cb = NULL;
        }
    }

    if (p_inq->inqfilt_active) {
        p_inq->inqfilt_active = FALSE;

        if (p_inq->p_inqfilter_cmpl_cb) {
            status = BTM_DEV_RESET;
            (*p_inq->p_inqfilter_cmpl_cb)(&status);
        }
    }

    p_inq->state = BTM_INQ_INACTIVE_STATE;
    p_inq->pending_filt_complete_event = 0;
    p_inq->p_inq_results_cb = NULL;
    btm_clr_inq_db(NULL);
    btm_clr_inq_result_flt();

    p_inq->discoverable_mode = BTM_NON_DISCOVERABLE;
    p_inq->connectable_mode = BTM_NON_CONNECTABLE;
    p_inq->page_scan_type = BTM_SCAN_TYPE_STANDARD;
    p_inq->inq_scan_type = BTM_SCAN_TYPE_STANDARD;
}

void btm_inq_db_init(void) {
    /* ... */
}

void btm_clr_inq_db(BD_ADDR p_bda) {
    tBTM_INQUIRY_VAR_ST* p_inq = &btm_cb.btm_inq_vars;
    tINQ_DB_ENT* p_ent = p_inq->inq_db;
    UINT16 xx;

    /* explicitly post-increment */
    for (xx = 0; xx < BTM_INQ_DB_SIZE; xx++, p_ent++) {
        if (p_ent->in_use) {
            if (!p_bda || memcmp(p_ent->inq_info.results.remote_bd_addr, p_bda, BD_ADDR_LEN) == 0) {
                p_ent->in_use = FALSE;

                if (btm_cb.btm_inq_vars.p_inq_change_cb) {
                    (*btm_cb.btm_inq_vars.p_inq_change_cb)(&p_ent->inq_info, FALSE);
                }
            }
        }
    }
}

static void btm_clr_inq_result_flt(void) {
    tBTM_INQUIRY_VAR_ST* p_inq = &btm_cb.btm_inq_vars;

    if (p_inq->p_bd_db) {
        GKI_freebuf(p_inq->p_bd_db);
        p_inq->p_bd_db = NULL;
    }

    p_inq->num_bd_entries = 0;
    p_inq->max_bd_entries = 0;
}

static BOOLEAN btm_inq_find_bdaddr(BD_ADDR p_bda) {
    tBTM_INQUIRY_VAR_ST* p_inq = &btm_cb.btm_inq_vars;
    tINQ_BDADDR* p_db = p_inq->p_bd_db;
    UINT16 xx;

    if (p_inq->inq_active & BTM_PERIODIC_INQUIRY_ACTIVE || !p_db)
        return FALSE;

    /* explicitly post-increment */
    for (xx = 0; xx < p_inq->num_bd_entries; xx++, p_db++) {
        if (memcmp(p_db->bd_addr, p_bda, BD_ADDR_LEN) == 0 && p_db->inq_count == p_inq->inq_counter) {
            return TRUE;
        }
    }

    if (xx < p_inq->max_bd_entries) {
        p_db->inq_count = p_inq->inq_counter;
        memcpy(p_db->bd_addr, p_bda, BD_ADDR_LEN);

        ++p_inq->num_bd_entries;
    }

    return FALSE;
}

static tINQ_DB_ENT* btm_inq_db_find(BD_ADDR p_bda) {
    UINT16 xx;
    tINQ_DB_ENT* p_ent = btm_cb.btm_inq_vars.inq_db;

    /* explicitly post-increment */
    for (xx = 0; xx < BTM_INQ_DB_SIZE; xx++, p_ent++) {
        if (p_ent->in_use && memcmp(p_ent->inq_info.results.remote_bd_addr, p_bda, BD_ADDR_LEN) == 0) {
            return p_ent;
        }
    }

    return NULL;
}

static tINQ_DB_ENT* btm_inq_db_new(BD_ADDR p_bda) {
    UINT16 xx;
    tINQ_DB_ENT* p_ent = btm_cb.btm_inq_vars.inq_db;
    tINQ_DB_ENT* p_old = btm_cb.btm_inq_vars.inq_db;
    UINT32 ot = 0xffffffff;

    /* explicitly post-increment */
    for (xx = 0; xx < BTM_INQ_DB_SIZE; xx++, p_ent++) {
        if (!p_ent->in_use) {
            memset(p_ent, 0, sizeof *p_ent);
            memcpy(p_ent->inq_info.results.remote_bd_addr, p_bda, BD_ADDR_LEN);
            p_ent->in_use = TRUE;

            return p_ent;
        }

        if (p_ent->time_of_resp < ot) {
            p_old = p_ent;
            ot = p_ent->time_of_resp;
        }
    }

    if (btm_cb.btm_inq_vars.p_inq_change_cb)
        (*btm_cb.btm_inq_vars.p_inq_change_cb)(&p_old->inq_info, FALSE);

    memset(p_old, 0, sizeof *p_old);
    memcpy(p_old->inq_info.results.remote_bd_addr, p_bda, BD_ADDR_LEN);
    p_old->in_use = TRUE;

    return p_old;
}

static tBTM_STATUS btm_set_inq_event_filter(tBTM_INQ_FILT_COND_TYPE filter_cond_type, tBTM_INQ_FILT_COND* p_filt_cond) {
    void* p_buf;
    UINT8 condition_length = DEV_CLASS_LEN * 2;
    UINT8 condition_buf[DEV_CLASS_LEN * 2];
    UINT8* p_cond = condition_buf;

    p_buf = HCI_GET_CMD_BUF(2 + condition_length);
    if (p_buf) {
        switch (filter_cond_type) {
        case BTM_FILTER_COND_DEVICE_CLASS:
            memcpy(condition_buf, p_filt_cond->cod_cond.dev_class, DEV_CLASS_LEN);
            memcpy(&condition_buf[DEV_CLASS_LEN], p_filt_cond->cod_cond.dev_class_mask, DEV_CLASS_LEN);
            break;

        case BTM_FILTER_COND_BD_ADDR:
            p_cond = p_filt_cond->bdaddr_cond;
            break;

        case BTM_CLR_INQUIRY_FILTER:
            condition_length = 0;
            break;

        default:
            return BTM_ILLEGAL_VALUE;
        }

        btm_cb.btm_inq_vars.inqfilt_active = TRUE;
        btsnd_hcic_set_event_filter(p_buf, HCI_FILTER_INQUIRY_RESULT, filter_cond_type, p_cond, condition_length);

        return BTM_CMD_STARTED;
    } else {
        return BTM_NO_RESOURCES;
    }
}

void btm_event_filter_complete(UINT8* p) {
    tHCI_STATUS hci_status;
    tBTM_STATUS status;
    tBTM_INQUIRY_VAR_ST* p_inq = &btm_cb.btm_inq_vars;
    tBTM_CMPL_CB* p_cb = p_inq->p_inqfilter_cmpl_cb;

    if (p_inq->pending_filt_complete_event != 0) {
        --p_inq->pending_filt_complete_event;
        return;
    }

    if (p_inq->inqfilt_active == TRUE) {
        STREAM_TO_UINT8(p, &hci_status);
        if (hci_status != HCI_SUCCESS) {
            BTM_TRACE(WARNING, "BTM Warning: Set Event Filter Failed (HCI returned 0x%x)", hci_status);

            status = BTM_ERR_PROCESSING;
        } else {
            status = BTM_SUCCESS;
        }

        if (p_inq->state == BTM_INQ_INACTIVE_STATE) {
            p_inq->inqfilt_active = FALSE;

            if (p_cb)
                (*p_cb)(&status);
        } else {
            if (status != BTM_SUCCESS) {
                btm_process_inq_complete(BTM_ERR_PROCESSING);

                p_inq->inqfilt_active = FALSE;
                p_inq->inq_active = BTM_INQUIRY_INACTIVE;
                p_inq->state = BTM_INQ_INACTIVE_STATE;

                return;
            }

            if (p_inq->state == BTM_INQ_CLR_FILT_STATE) {
                if ((status = btm_set_inq_event_filter(p_inq->inqparms.filter_cond_type, &p_inq->inqparms.filter_cond)) == BTM_CMD_STARTED) {
                    p_inq->state = BTM_INQ_SET_FILT_STATE;
                } else {
                    p_inq->inqfilt_active = FALSE;

                    btm_process_inq_complete(BTM_ERR_PROCESSING);
                }
            } else {
                p_inq->state = BTM_INQ_ACTIVE_STATE;
                p_inq->inqfilt_active = FALSE;
                btm_initiate_inquiry(p_inq);
            }
        }
    }
}

static void btm_initiate_inquiry(tBTM_INQUIRY_VAR_ST* p_inq) {
    LAP const* lap;
    tBTM_INQ_PARMS* p_inqparms = &p_inq->inqparms;

    p_inqparms->max_resps = p_inqparms->max_resps <= BTM_INQ_DB_SIZE ? p_inqparms->max_resps : BTM_INQ_DB_SIZE;

    lap = p_inq->inq_active & BTM_LIMITED_INQUIRY_ACTIVE ? &limited_inq_lap : &general_inq_lap;

    if (p_inq->inq_active & BTM_PERIODIC_INQUIRY_ACTIVE) {
        if (!btsnd_hcic_per_inq_mode(p_inq->per_max_delay, p_inq->per_min_delay, *lap, p_inqparms->duration, p_inqparms->max_resps)) {
            btm_process_inq_complete(BTM_NO_RESOURCES);
        }
    } else {
        btm_clr_inq_result_flt();

        if ((p_inq->p_bd_db = GKI_getbuf(GKI_MAX_BUF_SIZE)) != NULL) {
            p_inq->max_bd_entries = GKI_MAX_BUF_SIZE / sizeof *p_inq->p_bd_db;
            memset(p_inq->p_bd_db, 0, GKI_MAX_BUF_SIZE);
        }

        if (!btsnd_hcic_inquiry(*lap, p_inqparms->duration, 0))
            btm_process_inq_complete(BTM_NO_RESOURCES);
    }
}

void btm_process_inq_results(UINT8* p, BOOLEAN with_rssi) {
    UINT8 num_resp;
    UINT8 xx;
    BD_ADDR bda;
    tINQ_DB_ENT* p_i;
    tBTM_INQ_RESULTS* p_cur;
    BOOLEAN is_new = TRUE;
    tBTM_INQUIRY_VAR_ST* p_inq = &btm_cb.btm_inq_vars;
    tBTM_INQ_RESULTS_CB* p_inq_results_cb = p_inq->p_inq_results_cb;
    UINT8 page_scan_rep_mode = 0;
    UINT8 page_scan_per_mode = 0;
    UINT8 page_scan_mode = 0;
    UINT8 rssi = 0;
    DEV_CLASS dc;
    UINT16 clock_offset;

    if (!p_inq->inq_active)
        return;

    STREAM_TO_UINT8(p, &num_resp);

    for (xx = 0; xx < num_resp; ++xx) {
        STREAM_TO_BDADDR(p, bda);
        STREAM_TO_UINT8(p, &page_scan_rep_mode);
        STREAM_TO_UINT8(p, &page_scan_per_mode);

        if (!with_rssi)
            STREAM_TO_UINT8(p, &page_scan_mode);

        STREAM_TO_DEVCLASS(p, dc);
        STREAM_TO_UINT16(p, &clock_offset);

        if (with_rssi)
            STREAM_TO_UINT8(p, &rssi);

        if (btm_inq_find_bdaddr(bda))
            continue;

        if (btm_cb.p_inq_filter_cb) {
            if (!(*btm_cb.p_inq_filter_cb)(bda, dc))
                continue;
        }

        p_i = btm_inq_db_find(bda);
        if (!p_i) {
            p_i = btm_inq_db_new(bda);
            is_new = TRUE;
        } else if (p_i->inq_count == p_inq->inq_counter) {
            is_new = FALSE;
        }

        if (is_new == TRUE) {
            p_cur = &p_i->inq_info.results;
            p_cur->page_scan_rep_mode = page_scan_rep_mode;
            p_cur->page_scan_per_mode = page_scan_per_mode;
            p_cur->page_scan_mode = page_scan_mode;
            p_cur->dev_class[0] = dc[0];
            p_cur->dev_class[1] = dc[1];
            p_cur->dev_class[2] = dc[2];
            p_cur->clock_offset = clock_offset | BTM_CLOCK_OFFSET_VALID;

            if (with_rssi)
                p_cur->rssi = (INT8)rssi;
            else
                p_cur->rssi = BTM_INQ_RES_IGNORE_RSSI;

            p_i->time_of_resp = GKI_get_tick_count();

            p_i->inq_count = p_inq->inq_counter;
            ++p_inq->inq_cmpl_info.num_resp;

            if (!(p_inq->inq_active & BTM_PERIODIC_INQUIRY_ACTIVE) && p_inq->inqparms.max_resps &&
                p_inq->inq_cmpl_info.num_resp == p_inq->inqparms.max_resps) {
                btsnd_hcic_inq_cancel();
            }

            p_i->inq_info.appl_knows_rem_name = FALSE;

            if (p_inq_results_cb)
                (p_inq_results_cb)(p_cur);

            if (p_inq->p_inq_change_cb)
                (*p_inq->p_inq_change_cb)(&p_i->inq_info, TRUE);
        }
    }
}

void btm_process_inq_complete(UINT8 status) {
    tBTM_CMPL_CB* p_inq_cb = btm_cb.btm_inq_vars.p_inq_cmpl_cb;
    tBTM_INQUIRY_VAR_ST* p_inq = &btm_cb.btm_inq_vars;

    if (p_inq->inq_active) {
        ++p_inq->inq_counter;

        p_inq->inq_cmpl_info.status = status == HCI_SUCCESS ? BTM_SUCCESS : BTM_ERR_PROCESSING;

        if (!(p_inq->inq_active & BTM_PERIODIC_INQUIRY_ACTIVE)) {
            p_inq->p_inq_results_cb = NULL;
            p_inq->inq_active = BTM_INQUIRY_INACTIVE;
            p_inq->state = BTM_INQ_INACTIVE_STATE;
            p_inq->p_inq_cmpl_cb = NULL;

            btm_clr_inq_result_flt();

            BTM_TRACE(DEBUG, "BTM Inq Compl Callback: status 0x%02x, num results %d", p_inq->inq_cmpl_info.status, p_inq->inq_cmpl_info.num_resp);

            if (p_inq_cb)
                (p_inq_cb)(&p_inq->inq_cmpl_info);
        }
    }
}

void btm_inq_check_ageing(void) {
    /* ... */
}

tBTM_STATUS btm_initiate_rem_name(BD_ADDR remote_bda, tBTM_INQ_INFO* p_cur, tBTM_RMT_NAME_ACTIVE_FLAGS origin, UINT32 timeout, tBTM_CMPL_CB* p_cb) {
    tBTM_INQUIRY_VAR_ST* p_inq = &btm_cb.btm_inq_vars;
    BOOLEAN send_to_controller = TRUE;
    BOOLEAN cmd_ok = TRUE;
    tBTM_STATUS status = BTM_CMD_STARTED;

    if (!BTM_IsDeviceUp())
        return BTM_WRONG_MODE;

    if (!(p_inq->remname_active & origin)) {
        if (origin == BTM_RMT_NAME_EXT) {
            if (p_inq->remname_active) {
                if (memcmp(remote_bda, p_inq->remname_bda, BD_ADDR_LEN) == 0)
                    send_to_controller = FALSE;
                else
                    return BTM_BUSY;
            }

            p_inq->p_remname_cmpl_cb = p_cb;
        } else if (p_inq->remname_active != BTM_RMT_NAME_INACTIVE) {
            return BTM_BUSY;
        }

        if (send_to_controller) {
            memcpy(p_inq->remname_bda, remote_bda, BD_ADDR_LEN);

            btu_start_timer(&p_inq->rmt_name_timer_ent, BTU_TTYPE_BTM_RMT_NAME, timeout);

            if (p_cur) {
                cmd_ok = btsnd_hcic_rmt_name_req(remote_bda, p_cur->results.page_scan_rep_mode, p_cur->results.page_scan_mode,
                                                 p_cur->results.clock_offset | BTM_CLOCK_OFFSET_VALID);
            } else {
                cmd_ok = btsnd_hcic_rmt_name_req(remote_bda, HCI_PAGE_SCAN_REP_MODE_R1, HCI_MANDATARY_PAGE_SCAN_MODE, 0);
            }
        }

        if (cmd_ok)
            p_inq->remname_active |= origin;
        else
            status = BTM_NO_RESOURCES;
    } else {
        status = BTM_BUSY;
    }

    return status;
}

void btm_process_remote_name(BD_ADDR bda, BD_NAME bdn, UINT16 evt_len, UINT8 hci_status) {
    tBTM_REMOTE_DEV_NAME rem_name;
    tBTM_INQUIRY_VAR_ST* p_inq = &btm_cb.btm_inq_vars;
    tBTM_CMPL_CB* p_cb = p_inq->p_remname_cmpl_cb;
    UINT8* p_n1;
    UINT8 active_flag = p_inq->remname_active;
    UINT16 temp_evt_len;

    btu_stop_timer(&p_inq->rmt_name_timer_ent);
    p_inq->remname_active = BTM_RMT_NAME_INACTIVE;

    if (hci_status == HCI_SUCCESS) {
        p_n1 = rem_name.remote_bd_name;

        rem_name.length = evt_len < BD_NAME_LEN ? evt_len : BD_NAME_LEN;
        rem_name.status = BTM_SUCCESS;
        temp_evt_len = rem_name.length;

        while (temp_evt_len > 0) {
            *p_n1++ = *bdn++;
            --temp_evt_len;
        }
    } else {
        rem_name.status = BTM_BAD_VALUE_RET;
        rem_name.length = 0;
        rem_name.remote_bd_name[0] = '\0';
    }

    memset(p_inq->remname_bda, 0, BD_ADDR_LEN);

    if (active_flag & BTM_RMT_NAME_EXT) {
        p_inq->p_remname_cmpl_cb = NULL;

        if (p_cb)
            (*p_cb)(&rem_name);
    }
}

void btm_inq_rmt_name_failed(void) {
    btm_process_remote_name(NULL, NULL, 0, HCI_ERR_UNSPECIFIED);

    btm_sec_rmt_name_request_complete(NULL, NULL, HCI_ERR_UNSPECIFIED);
}
