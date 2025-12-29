#include "btm_api.h"

/* Original source:
 * bluedroid <android.googlesource.com/platform/external/bluetooth/bluedroid>
 * stack/btm/btm_dev.c
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

#include "bt_target.h"
#include "bt_types.h"
#include "data_types.h"
#include "revolution/types.h"
#include <cstring>

#include "btm_int.h"
#include "hcidefs.h"  // tHCI_HANDLE

/*******************************************************************************
 * local function declarations
 */

static tBTM_SEC_DEV_REC* btm_find_oldest_dev(void);

/*******************************************************************************
 * functions
 */

BOOLEAN BTM_SecAddDevice(BD_ADDR bd_addr, DEV_CLASS dev_class, BD_NAME bd_name, BD_FEATURES features, UINT32* trusted_mask, LINK_KEY link_key) {
    tBTM_SEC_DEV_REC* p_dev_rec;

    if ((p_dev_rec = btm_find_dev(bd_addr)) == NULL) {
        if ((p_dev_rec = btm_sec_alloc_dev(bd_addr)) == NULL)
            return FALSE;
    } else {
        p_dev_rec->timestamp = btm_cb.dev_rec_count++;
    }

    if (dev_class)
        memcpy(p_dev_rec->dev_class, dev_class, DEV_CLASS_LEN);

    memset(p_dev_rec->sec_bd_name, 0, sizeof p_dev_rec->sec_bd_name);

    if (bd_name && bd_name[0] != '\0') {
        p_dev_rec->sec_flags |= BTM_SEC_NAME_KNOWN;

        BCM_STRNCPY_S((char*)p_dev_rec->sec_bd_name, BTM_MAX_REM_BD_NAME_LEN + 1, (char*)bd_name, BTM_MAX_REM_BD_NAME_LEN);
    }

    if (features)
        memcpy(p_dev_rec->features, features, sizeof p_dev_rec->features);
    else
        memset(p_dev_rec->features, 0, sizeof p_dev_rec->features);

    BTM_SEC_COPY_TRUSTED_DEVICE(trusted_mask, p_dev_rec->trusted_mask);

    if (link_key) {
        p_dev_rec->sec_flags |= BTM_SEC_LINK_KEY_KNOWN;

        memcpy(p_dev_rec->link_key, link_key, LINK_KEY_LEN);
    }

    return TRUE;
}

BOOLEAN BTM_SecDeleteDevice(BD_ADDR bd_addr) {
    tBTM_SEC_DEV_REC* p_dev_rec;

    if ((p_dev_rec = btm_find_dev(bd_addr)) == NULL)
        return FALSE;

    btm_sec_free_dev(p_dev_rec);

    BTM_DeleteStoredLinkKey(bd_addr, NULL);

    return TRUE;
}

char* BTM_SecReadDevName(BD_ADDR bd_addr) {
    char* p_name = NULL;
    tBTM_SEC_DEV_REC* p_srec;

    if ((p_srec = btm_find_dev(bd_addr)) != NULL)
        p_name = (char*)p_srec->sec_bd_name;

    return p_name;
}

tBTM_SEC_DEV_REC* btm_sec_alloc_dev(BD_ADDR bd_addr) {
    tBTM_SEC_DEV_REC* p_dev_rec = NULL;
    tBTM_INQ_INFO* p_inq_info;
    int i;

    for (i = 0; i < BTM_SEC_MAX_DEVICE_RECORDS; ++i) {
        if (!(btm_cb.sec_dev_rec[i].sec_flags & BTM_SEC_IN_USE)) {
            p_dev_rec = &btm_cb.sec_dev_rec[i];
            break;
        }
    }

    if (!p_dev_rec)
        p_dev_rec = btm_find_oldest_dev();

    memset(p_dev_rec, 0, sizeof(tBTM_SEC_DEV_REC));

    p_dev_rec->sec_flags = BTM_SEC_IN_USE;
    p_dev_rec->sec_acl_connected = FALSE;

    if ((p_inq_info = BTM_InqDbRead(bd_addr)) != NULL) {
        memcpy(p_dev_rec->dev_class, p_inq_info->results.dev_class, DEV_CLASS_LEN);
    } else {
        memcpy(p_dev_rec->dev_class, btm_cb.connecting_dc, DEV_CLASS_LEN);
    }

    memcpy(p_dev_rec->bd_addr, bd_addr, BD_ADDR_LEN);

    p_dev_rec->hci_handle = BTM_GetHCIConnHandle(bd_addr);
    p_dev_rec->timestamp = btm_cb.dev_rec_count++;

    return p_dev_rec;
}

void btm_sec_free_dev(tBTM_SEC_DEV_REC* p_dev_rec) {
    p_dev_rec->sec_flags = 0;
}

tBTM_SEC_DEV_REC* btm_find_dev_by_handle(tHCI_HANDLE handle) {
    tBTM_SEC_DEV_REC* p_dev_rec = btm_cb.sec_dev_rec;
    int i;

    /* explicitly post-increment */
    for (i = 0; i < BTM_SEC_MAX_DEVICE_RECORDS; i++, p_dev_rec++) {
        if (p_dev_rec->sec_flags & BTM_SEC_IN_USE && p_dev_rec->hci_handle == handle) {
            return p_dev_rec;
        }
    }

    return NULL;
}

tBTM_SEC_DEV_REC* btm_find_dev(BD_ADDR bd_addr) {
    tBTM_SEC_DEV_REC* p_dev_rec = btm_cb.sec_dev_rec;
    int i;

    /* explicitly post-increment */
    for (i = 0; i < BTM_SEC_MAX_DEVICE_RECORDS; i++, p_dev_rec++) {
        if (p_dev_rec->sec_flags & BTM_SEC_IN_USE && memcmp(p_dev_rec->bd_addr, bd_addr, BD_ADDR_LEN) == 0) {
            return p_dev_rec;
        }
    }

    return NULL;
}

static tBTM_SEC_DEV_REC* btm_find_oldest_dev(void) {
    tBTM_SEC_DEV_REC* p_dev_rec = btm_cb.sec_dev_rec;
    tBTM_SEC_DEV_REC* p_oldest = p_dev_rec;
    UINT32 ot = 0xffffffff;
    int i;

    /* explicitly post-increment */
    for (i = 0; i < BTM_SEC_MAX_DEVICE_RECORDS; i++, p_dev_rec++) {
        if (p_dev_rec->sec_flags & BTM_SEC_IN_USE && !(p_dev_rec->sec_flags & BTM_SEC_LINK_KEY_KNOWN)) {
            if (p_dev_rec->timestamp < ot) {
                p_oldest = p_dev_rec;
                ot = p_dev_rec->timestamp;
            }
        }
    }

    if (ot != 0xffffffff)
        return p_oldest;

    p_dev_rec = btm_cb.sec_dev_rec;

    /* explicitly post-increment */
    for (i = 0; i < BTM_SEC_MAX_DEVICE_RECORDS; i++, p_dev_rec++) {
        if (p_dev_rec->sec_flags & BTM_SEC_IN_USE) {
            if (p_dev_rec->timestamp < ot) {
                p_oldest = p_dev_rec;
                ot = p_dev_rec->timestamp;
            }
        }
    }

    return p_oldest;
}
