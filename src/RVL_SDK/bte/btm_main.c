#include "btm_int.h"

/* Original source:
 * bluedroid <android.googlesource.com/platform/external/bluetooth/bluedroid>
 * stack/btm/btm_main.c
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

#include <mem.h>  // memset

#include "bt_trace.h"

#include "btm_api.h"  // BTM_SEC_MODE_SERVICE

/*******************************************************************************
 * variables
 */

// .bss
tBTM_CB btm_cb;

/*******************************************************************************
 * functions
 */

void btm_init(void) {
    memset(&btm_cb, 0, sizeof btm_cb);

    btm_cb.trace_level = BT_TRACE_LEVEL_NONE;

    btm_inq_db_init();
    btm_acl_init();
    btm_sec_init(BTM_SEC_MODE_SERVICE);
    btm_sco_init();
    btm_dev_init();
}
