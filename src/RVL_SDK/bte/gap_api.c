#include "gap_api.h"

/* Original source:
 * bluedroid <android.googlesource.com/platform/external/bluetooth/bluedroid>
 * stack/gap/gap_api.c
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

#include "bt_target.h"
#include "bt_trace.h"
#include "bt_types.h"
#include "data_types.h"

#include "btm_api.h"
#include "gap_int.h"
#include "profiles_api.h"
#include "sdp_api.h"

/*******************************************************************************
 * functions
 */

UINT16 GAP_SetDiscoverableMode(UINT16 mode, UINT16 duration, UINT16 interval) {
    tBTM_STATUS status = BTM_SetDiscoverability(mode, duration, interval);

    return gap_convert_btm_status(status);
}

UINT16 GAP_ReadDiscoverableMode(UINT16* duration, UINT16* interval) {
    return BTM_ReadDiscoverability(duration, interval);
}

UINT16 GAP_SetConnectableMode(UINT16 mode, UINT16 duration, UINT16 interval) {
    tBTM_STATUS status = BTM_SetConnectability(mode, duration, interval);

    return gap_convert_btm_status(status);
}

UINT16 GAP_FindAddrByName(BD_NAME devname, tGAP_INQ_PARMS* p_inq_parms, tGAP_CALLBACK* p_addr_cb, BD_ADDR bd_addr) {
    UINT16 status;
    tBTM_STATUS btm_status;

    if ((status = gap_find_local_addr_by_name(devname, bd_addr)) != BT_PASS) {
        if (!gap_cb.findaddr_cb.in_use) {
            gap_cb.findaddr_cb.p_cback = p_addr_cb;
            gap_cb.findaddr_cb.p_cur_inq = NULL;
            BCM_STRNCPY_S((char*)gap_cb.findaddr_cb.results.devname, BTM_MAX_REM_BD_NAME_LEN + 1, (char*)devname, BD_NAME_LEN);

            btm_status = BTM_StartInquiry(p_inq_parms, NULL, (tBTM_CMPL_CB*)gap_find_addr_inq_cb);
            gap_cb.findaddr_cb.in_use = TRUE;

            if ((status = gap_convert_btm_status(btm_status)) == GAP_CMD_INITIATED)
                gap_cb.findaddr_cb.in_use = TRUE;
        } else {
            status = GAP_ERR_BUSY;
        }
    }

    return status;
}

UINT16 GAP_ReadConnectableMode(UINT16* duration, UINT16* interval) {
    return BTM_ReadConnectability(duration, interval);
}

void GAP_SetSecurityMode(UINT8 sec_mode) {
    BTM_SetSecurityMode(sec_mode);
}

UINT8 GAP_Bond(BD_ADDR bd_addr, UINT8 pin_len, UINT8* p_pin, UINT32* trusted_mask) {
    return (UINT8)BTM_SecBond(bd_addr, pin_len, p_pin, trusted_mask);
}

BOOLEAN GAP_SecRegister(tBTM_APPL_INFO* p_cb_info) {
    return BTM_SecRegister(p_cb_info);
}

void GAP_PinRsp(BD_ADDR bd_addr, UINT8 res, UINT8 pin_len, UINT8* p_pin, UINT32* trusted_mask) {
    BTM_PINCodeReply(bd_addr, res, pin_len, p_pin, trusted_mask);
}

void GAP_AuthorizeRsp(BD_ADDR bd_addr, UINT8 res, UINT32* trusted_mask) {
    BTM_DeviceAuthorized(bd_addr, res, trusted_mask);
}

UINT16 GAP_SetPairableMode(UINT16 mode, BOOLEAN connect_only_paired) {
    tBTM_STATUS btm_status;
    UINT16 status = BT_PASS;

    if (mode == GAP_ALLOW_PAIRING) {
        btm_status = BTM_SetConnectability(BTM_CONNECTABLE, 0, 0);

        if ((status = gap_convert_btm_status(btm_status)) == BT_PASS)
            BTM_SetPairableMode(TRUE, connect_only_paired);
    } else if (mode == GAP_DISALLOW_PAIRING) {
        BTM_SetPairableMode(FALSE, connect_only_paired);
    } else {
        GAP_TRACE(ERROR, "GAP_SetPairableMode: illegal mode %d", mode);
        status = GAP_ERR_ILL_MODE;
    }

    return (status);
}

UINT16 GAP_StartInquiry(tGAP_INQ_PARMS* p_inq_parms, tGAP_CALLBACK* p_results_cb, tGAP_CALLBACK* p_cmpl_cb) {
    tGAP_INFO* p_cb;
    tBTM_STATUS btm_status;
    UINT16 retval;

    if (p_inq_parms->mode != GAP_GENERAL_INQUIRY && p_inq_parms->mode != GAP_LIMITED_INQUIRY) {
        return GAP_ERR_ILL_MODE;
    }

    if (p_inq_parms->duration < GAP_MIN_INQUIRY_LENGTH || p_inq_parms->duration > GAP_MAX_INQUIRY_LENGTH) {
        return GAP_ERR_ILL_INQ_TIME;
    }

    if ((p_cb = gap_allocate_cb()) != NULL) {
        p_cb->gap_cback = p_cmpl_cb;
        p_cb->gap_inq_rslt_cback = p_results_cb;
        p_cb->event = GAP_EVT_INQUIRY_COMPLETE;

        btm_status = BTM_StartInquiry(p_inq_parms, (tBTM_INQ_RESULTS_CB*)&gap_inq_results_cb, gap_cb.btm_cback[p_cb->index]);

        if ((retval = gap_convert_btm_status(btm_status)) != GAP_CMD_INITIATED)
            gap_free_cb(p_cb);
    } else {
        retval = GAP_ERR_NO_CTRL_BLK;
    }

    return retval;
}

UINT16 GAP_StartPeriodicInquiry(tGAP_INQ_PARMS* p_inq_parms, UINT16 min_time, UINT16 max_time, tGAP_CALLBACK* p_results_cb) {
    tGAP_INFO* p_cb;
    tBTM_STATUS btm_status;
    UINT16 retval = BT_PASS;

    if (p_inq_parms->mode != GAP_GENERAL_INQUIRY && p_inq_parms->mode != GAP_LIMITED_INQUIRY) {
        return GAP_ERR_ILL_MODE;
    }

    if (p_inq_parms->duration < GAP_MIN_INQUIRY_LENGTH || p_inq_parms->duration > GAP_MAX_INQUIRY_LENGTH || min_time <= p_inq_parms->duration ||
        min_time < GAP_PER_INQ_MIN_MIN_PERIOD || min_time > GAP_PER_INQ_MAX_MIN_PERIOD || max_time <= min_time ||
        max_time < GAP_PER_INQ_MIN_MAX_PERIOD || max_time > GAP_PER_INQ_MAX_MAX_PERIOD) {
        return GAP_ERR_ILL_INQ_TIME;
    }

    if ((p_cb = gap_allocate_cb()) != NULL) {
        p_cb->gap_inq_rslt_cback = p_results_cb;
        p_cb->event = GAP_EVT_INQUIRY_COMPLETE;

        btm_status = BTM_SetPeriodicInquiryMode(p_inq_parms, max_time, min_time, (tBTM_INQ_RESULTS_CB*)&gap_inq_results_cb);

        if ((retval = gap_convert_btm_status(btm_status)) != GAP_CMD_INITIATED)
            gap_free_cb(p_cb);
    } else {
        retval = GAP_ERR_NO_CTRL_BLK;
    }

    return retval;
}

UINT16 GAP_CancelInquiry(void) {
    tGAP_INFO* p_cb = &gap_cb.blk[0];
    UINT8 x;
    tBTM_STATUS btm_status;
    UINT16 status;

    btm_status = BTM_CancelInquiry();
    if ((status = gap_convert_btm_status(btm_status)) == BT_PASS) {
        /* explicitly post-increment */
        for (x = 0; x < GAP_MAX_BLOCKS; x++, p_cb++) {
            if (p_cb->in_use && p_cb->event == GAP_EVT_INQUIRY_COMPLETE) {
                gap_free_cb(p_cb);
                return BT_PASS;
            }
        }

        status = GAP_ERR_NO_CTRL_BLK;
    }

    return status;
}

UINT16 GAP_CancelPeriodicInquiry(void) {
    tGAP_INFO* p_cb = &gap_cb.blk[0];
    UINT8 x;
    tBTM_STATUS btm_status;
    UINT16 status;

    btm_status = BTM_CancelPeriodicInquiry();
    if ((status = gap_convert_btm_status(btm_status)) == BT_PASS) {
        /* explicitly post-increment */
        for (x = 0; x < GAP_MAX_BLOCKS; x++, p_cb++) {
            if (p_cb->in_use && p_cb->event == GAP_EVT_INQUIRY_COMPLETE) {
                gap_free_cb(p_cb);
                return BT_PASS;
            }
        }

        status = GAP_ERR_NO_CTRL_BLK;
    }

    return status;
}

UINT16 GAP_GetFirstInquiryResult(tGAP_INQ_RESULTS* p_results) {
    UINT8* ptr;

    gap_cb.cur_inqptr = BTM_InqFirstResult();

    if (gap_cb.cur_inqptr) {
        memcpy(p_results, &gap_cb.cur_inqptr->results, sizeof *p_results);

        ptr = gap_cb.cur_inqptr->results.remote_bd_addr;

        GAP_TRACE(EVENT, "GAP_GetFirstInqResult %02x%02x%02x%02x%02x%02x", ptr[0], ptr[1], ptr[2], ptr[3], ptr[4], ptr[5]);

        return BT_PASS;
    } else {
        GAP_TRACE(EVENT, "GAP_FirstInqResults:  No BD_ADDRs Found");

        memset(p_results, 0, sizeof *p_results);

        return GAP_EOINQDB;
    }
}

UINT16 GAP_GetNextInquiryResult(tGAP_INQ_RESULTS* p_results) {
    UINT8* ptr;

    if (gap_cb.cur_inqptr) {
        gap_cb.cur_inqptr = BTM_InqNextResult(gap_cb.cur_inqptr);
        if (gap_cb.cur_inqptr) {
            memcpy(p_results, &gap_cb.cur_inqptr->results, sizeof *p_results);

            ptr = gap_cb.cur_inqptr->results.remote_bd_addr;

            GAP_TRACE(EVENT, "GAP_GetNextInqResult %02x%02x%02x%02x%02x%02x", ptr[0], ptr[1], ptr[2], ptr[3], ptr[4], ptr[5]);

            return BT_PASS;
        } else {
            GAP_TRACE(EVENT, "GAP_NextInqResults:  No BD_ADDRs Found");

            memset(p_results, 0, sizeof *p_results);

            return GAP_EOINQDB;
        }
    } else {
        return GAP_GetFirstInquiryResult(p_results);
    }
}

UINT16 GAP_ReadLocalDeviceInfo(UINT8* name, BD_ADDR* addr, tGAP_LMP_VERSION* verinfo, tGAP_LMP_FEATURES* features) {
    return GAP_UNSUPPORTED;
}

UINT16 GAP_GetRemoteDeviceName(BD_ADDR addr, tGAP_CALLBACK* callback) {
    tGAP_INFO* p_cb;
    UINT16 retval;
    tBTM_STATUS btm_status;

    if ((p_cb = gap_allocate_cb()) != NULL) {
        p_cb->gap_cback = callback;
        p_cb->event = GAP_EVT_REM_NAME_COMPLETE;

        btm_status = BTM_ReadRemoteDeviceName(addr, gap_cb.btm_cback[p_cb->index]);

        if ((retval = gap_convert_btm_status(btm_status)) != GAP_CMD_INITIATED)
            gap_free_cb(p_cb);
    } else {
        retval = GAP_ERR_NO_CTRL_BLK;
    }

    return retval;
}

void GAP_SetDiscoveryParams(UINT16 num_uuid, tSDP_UUID* p_uuid_list, UINT16 num_attr, UINT16* p_attr_list) {
    BTM_SetDiscoveryParams(num_uuid, p_uuid_list, num_attr, p_attr_list);
}

extern tBTM_STATUS BTM_StartDiscovery(tBTM_CMPL_CB* p_cmpl_cb, BD_ADDR_PTR p_rem_addr);

UINT16 GAP_StartDiscovery(tGAP_CALLBACK* callback) {
    tGAP_INFO* p_cb;
    tBTM_STATUS btm_status;
    UINT16 retval;

    if ((p_cb = gap_allocate_cb()) != NULL) {
        p_cb->gap_cback = callback;
        p_cb->event = GAP_EVT_DISCOVERY_COMPLETE;

        btm_status = BTM_StartDiscovery(gap_cb.btm_cback[p_cb->index], NULL);

        if ((retval = gap_convert_btm_status(btm_status)) != GAP_CMD_INITIATED)
            gap_free_cb(p_cb);
    } else {
        retval = GAP_ERR_NO_CTRL_BLK;
    }

    return retval;
}

UINT16 GAP_SetDeviceClass(tGAP_COD* p_cod, UINT8 cmd) {
    tBTM_STATUS btm_status;
    UINT8* dev;
    UINT16 service;
    UINT8 minor;
    UINT8 major;
    DEV_CLASS dev_class;

    dev = BTM_ReadDeviceClass();
    BTM_GET_COD_SERVICE_CLASS(dev, &service);
    BTM_GET_COD_MINOR_CLASS(dev, &minor);
    BTM_GET_COD_MAJOR_CLASS(dev, &major);

    switch (cmd) {
    case GAP_SET_COD_MAJOR_MINOR:
        minor = p_cod->minor & BTM_COD_MINOR_CLASS_MASK;
        major = p_cod->major & BTM_COD_MAJOR_CLASS_MASK;
        break;

    case GAP_SET_COD_SERVICE_CLASS:
        p_cod->service &= BTM_COD_SERVICE_CLASS_MASK;
        service = service | p_cod->service;
        break;

    case GAP_CLR_COD_SERVICE_CLASS:
        p_cod->service &= BTM_COD_SERVICE_CLASS_MASK;
        service = service & (~p_cod->service);
        break;

    case GAP_SET_COD_ALL:
        minor = p_cod->minor & BTM_COD_MINOR_CLASS_MASK;
        major = p_cod->major & BTM_COD_MAJOR_CLASS_MASK;

        p_cod->service &= BTM_COD_SERVICE_CLASS_MASK;
        service = service | p_cod->service;
        break;

    case GAP_INIT_COD:
        minor = p_cod->minor & BTM_COD_MINOR_CLASS_MASK;
        major = p_cod->major & BTM_COD_MAJOR_CLASS_MASK;

        service = p_cod->service & BTM_COD_SERVICE_CLASS_MASK;
        break;

    default:
        return GAP_ERR_ILL_PARM;
    }

    MAKE_COD_FROM_FIELDS(dev_class, minor, major, service);

    btm_status = BTM_SetDeviceClass(dev_class);
    return gap_convert_btm_status(btm_status);
}

UINT16 GAP_ReadDeviceClass(tGAP_COD* p_cod) {
    UINT8* dev = BTM_ReadDeviceClass();

    BTM_GET_COD_SERVICE_CLASS(dev, &p_cod->service);
    BTM_GET_COD_MINOR_CLASS(dev, &p_cod->minor);
    BTM_GET_COD_MAJOR_CLASS(dev, &p_cod->major);

    return BT_PASS;
}

UINT8 GAP_SetTraceLevel(UINT8 new_level) {
    if (new_level != 0xff)
        gap_cb.trace_level = new_level;

    return gap_cb.trace_level;
}

void GAP_Init(void) {
    memset(&gap_cb, 0, sizeof gap_cb);

    gap_cb.btm_cback[0] = &gap_btm_cback0;
    gap_cb.btm_cback[1] = &gap_btm_cback1;

    gap_cb.trace_level = BT_TRACE_LEVEL_DEBUG;

    gap_conn_init();
}
