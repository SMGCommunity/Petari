#include "gap_int.h"

/* Original source:
 * bluedroid <android.googlesource.com/platform/external/bluetooth/bluedroid>
 * stack/gap/gap_utils.c
 */

/******************************************************************************
 *
 *  Copyright (C) 2009-2013 Broadcom Corporation
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

#include <macros.h>

#include "bt_trace.h"
#include "bt_types.h"
#include "data_types.h"

#include "btm_api.h"
#include "gap_api.h"
#include "profiles_api.h"

/*******************************************************************************
 * macros
 */

/*******************************************************************************
 * types
 */

/*******************************************************************************
 * local function declarations
 */

static void btm_cback(UINT16 index, void* p_data);

/*******************************************************************************
 * variables
 */

// .bss
tGAP_CB gap_cb;

/*******************************************************************************
 * functions
 */

static void btm_cback(UINT16 index, void* p_data) {
    tGAP_INFO* p_cb;
    tGAP_INQ_CMPL inq_cmpl;

    if (index >= GAP_MAX_BLOCKS)
        return;

    if (!gap_cb.blk[index].in_use)
        return;

    p_cb = &gap_cb.blk[index];

    switch (p_cb->event) {
    case GAP_EVT_INQUIRY_COMPLETE:
        inq_cmpl.num_results = ((tBTM_INQUIRY_CMPL*)p_data)->num_resp;

        inq_cmpl.status = (((tBTM_INQUIRY_CMPL*)p_data)->status == BTM_SUCCESS) ? BT_PASS : GAP_ERR_PROCESSING;

        p_data = &inq_cmpl;

        GAP_TRACE(EVENT, "   GAP Inquiry Complete Event (Status 0x%04x, Result(s) %d)", inq_cmpl.status, inq_cmpl.num_results);
        break;

    case GAP_EVT_DISCOVERY_COMPLETE:
        if (*(UINT16*)p_data) {
            GAP_TRACE(EVENT, "   GAP Discovery Complete Event(SDP Result: 0x%04x)", *(UINT16*)p_data);
        } else {
            GAP_TRACE(EVENT, "   GAP Discovery Successfully Completed");
        }

        break;

    case GAP_EVT_REM_NAME_COMPLETE:
        ((tGAP_REMOTE_DEV_NAME*)p_data)->status = gap_convert_btm_status(((tBTM_REMOTE_DEV_NAME*)p_data)->status);

        GAP_TRACE(EVENT, "   GAP Remote Name Complete Event (status 0x%04x)", ((tGAP_REMOTE_DEV_NAME*)p_data)->status);

        break;
    };

    if (p_cb->gap_cback)
        (*p_cb->gap_cback)(p_cb->event, p_data);

    gap_free_cb(p_cb);
}

void gap_btm_cback0(void* p1) {
    btm_cback(0, p1);
}

void gap_btm_cback1(void* p1) {
    btm_cback(1, p1);
}

void gap_inq_results_cb(tBTM_INQ_RESULTS* p_results) {
    tGAP_INFO* p_cb;
    UINT8 index;

    GAP_TRACE(EVENT, "GAP Inquiry Results Callback (bdaddr [%02x%02x%02x%02x%02x%02x])", p_results->remote_bd_addr[0], p_results->remote_bd_addr[1],
              p_results->remote_bd_addr[2], p_results->remote_bd_addr[3], p_results->remote_bd_addr[4], p_results->remote_bd_addr[5]);

    GAP_TRACE(EVENT, "                             (COD [%02x%02x%02x], clkoff 0x%04x)", p_results->dev_class[0], p_results->dev_class[1],
              p_results->dev_class[2], p_results->clock_offset);

    /* explicitly post-increment */
    for (index = 0, p_cb = &gap_cb.blk[0]; index < GAP_MAX_BLOCKS; index++, p_cb++) {
        if (p_cb->in_use && p_cb->event == GAP_EVT_INQUIRY_COMPLETE) {
            if (p_cb->gap_inq_rslt_cback)
                (*p_cb->gap_inq_rslt_cback)(GAP_EVT_INQUIRY_RESULTS, p_results);
        }
    }
}

void gap_find_addr_name_cb(tBTM_REMOTE_DEV_NAME* p) {
    tGAP_FINDADDR_CB* p_cb = &gap_cb.findaddr_cb;
    tGAP_FINDADDR_RESULTS* p_result = &p_cb->results;

    if (!p_cb->in_use)
        return;

    if (p->status == BTM_SUCCESS) {
        GAP_TRACE(EVENT,
                  "   GAP: FindAddrByName Rem Name Cmpl Evt (Status "
                  "0x%04x, Name [%s])",
                  p->status, p->remote_bd_name);

        if (!strncmp((char*)p_result->devname, (char*)p->remote_bd_name, strlen((char*)p_result->devname))) {
            memcpy(p_result->bd_addr, p_cb->p_cur_inq->results.remote_bd_addr, BD_ADDR_LEN);

            p_result->status = BT_PASS;
        } else {
            if ((p_cb->p_cur_inq = BTM_InqDbNext(p_cb->p_cur_inq)) != NULL) {
                if (BTM_ReadRemoteDeviceName(p_cb->p_cur_inq->results.remote_bd_addr, (tBTM_CMPL_CB*)&gap_find_addr_name_cb) == BTM_CMD_STARTED) {
                    return;
                } else {
                    p_result->status = gap_convert_btm_status((tBTM_STATUS)p->status);
                }
            } else {
                p_result->status = GAP_EOINQDB;
            }
        }
    } else {
        GAP_TRACE(EVENT, "   GAP: FindAddrByName Rem Name Cmpl Evt (Status 0x%04x)", p->status);
        p_result->status = gap_convert_btm_status((tBTM_STATUS)p->status);
    }

    if (p_cb->p_cback)
        (*p_cb->p_cback)(GAP_EVT_FIND_ADDR_COMPLETE, p_result);

    p_cb->in_use = FALSE;
    p_cb->p_cback = NULL;
}

void gap_find_addr_inq_cb(tBTM_INQUIRY_CMPL* p) {
    tGAP_FINDADDR_CB* p_cb = &gap_cb.findaddr_cb;
    tGAP_FINDADDR_RESULTS* p_result = &p_cb->results;

    if (!p_cb->in_use)
        return;

    GAP_TRACE(EVENT, "   GAP: FindAddrByName Inq Cmpl Evt (Status 0x%04x, Result(s) %d)", p->status, p->num_resp);

    if (p->status == BTM_SUCCESS) {
        if ((p_result->status = gap_find_local_addr_by_name(p_result->devname, p_result->bd_addr)) == GAP_NO_DATA_AVAIL) {
            if ((p_cb->p_cur_inq = BTM_InqDbFirst()) != NULL) {
                if ((BTM_ReadRemoteDeviceName(p_cb->p_cur_inq->results.remote_bd_addr, (tBTM_CMPL_CB*)&gap_find_addr_name_cb)) == BTM_CMD_STARTED) {
                    return;
                } else {
                    p_result->status = gap_convert_btm_status(p->status);
                }
            } else {
                p_result->status = GAP_EOINQDB;
            }
        }
    } else {
        p_result->status = gap_convert_btm_status(p->status);
    }

    if (p_cb->p_cback)
        (*p_cb->p_cback)(GAP_EVT_FIND_ADDR_COMPLETE, p_result);

    p_cb->in_use = FALSE;
    p_cb->p_cback = NULL;
}

UINT16 gap_find_local_addr_by_name(tBTM_BD_NAME const devname, BD_ADDR bd_addr) {
    return GAP_NO_DATA_AVAIL;
}

tGAP_INFO* gap_allocate_cb(void) {
    tGAP_INFO* p_cb = &gap_cb.blk[0];
    UINT8 x;

    /* explicitly post-increment */
    for (x = 0; x < GAP_MAX_BLOCKS; x++, p_cb++) {
        if (!p_cb->in_use) {
            memset(p_cb, 0, sizeof *p_cb);

            p_cb->in_use = TRUE;
            p_cb->index = x;
            p_cb->p_data = NULL;

            return p_cb;
        }
    }

    return NULL;
}

void gap_free_cb(tGAP_INFO* p_cb) {
    if (p_cb) {
        p_cb->gap_cback = NULL;
        p_cb->in_use = FALSE;
    }
}

BOOLEAN gap_is_service_busy(UINT16 request) {
    tGAP_INFO* p_cb = &gap_cb.blk[0];
    UINT8 x;

    /* explicitly post-increment */
    for (x = 0; x < GAP_MAX_BLOCKS; x++, p_cb++) {
        if (p_cb->in_use && p_cb->event == request)
            return TRUE;
    }

    return FALSE;
}

#if 0
// Range: 0x730 -> 0x794
unsigned short gap_convert_btm_status(unsigned char btm_status /* r3 */) {}
#endif

UINT16 gap_convert_btm_status(tBTM_STATUS btm_status) {
    switch (btm_status) {
    case BTM_SUCCESS:
        return BT_PASS;

    case BTM_CMD_STARTED:
        return GAP_CMD_INITIATED;

    case BTM_BUSY:
        return GAP_ERR_BUSY;

    case BTM_MODE_UNSUPPORTED:
    case BTM_ILLEGAL_VALUE:
        return GAP_ERR_ILL_PARM;

    case BTM_WRONG_MODE:
        return GAP_DEVICE_NOT_UP;

    case BTM_UNKNOWN_ADDR:
        return GAP_BAD_BD_ADDR;

    case BTM_DEVICE_TIMEOUT:
        return GAP_ERR_TIMEOUT;

    default:
        return GAP_ERR_PROCESSING;
    }
}
