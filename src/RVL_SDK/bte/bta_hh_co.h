#ifndef BTE_BTA_HH_CO_H
#define BTE_BTA_HH_CO_H

/* Original source:
 * bluedroid <android.googlesource.com/platform/external/bluetooth/bluedroid>
 * bta/include/bta_hh_co.h
 */

/******************************************************************************
 *
 *  Copyright (C) 2005-2012 Broadcom Corporation
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

#include "data_types.h"

#include "bta_hh_api.h"

/*******************************************************************************
 * types
 */

#ifdef __cplusplus
	extern "C" {
#endif

extern void bta_hh_co_open(UINT8 dev_handle, UINT8 sub_class, UINT16 attr_mask,
                           UINT8 app_id);
extern void bta_hh_co_data(UINT8 dev_handle, UINT8 *p_rpt, UINT16 len,
                           tBTA_HH_PROTO_MODE mode, UINT8 sub_class,
                           UINT8 app_id);
extern void bta_hh_co_close(UINT8 dev_handle, UINT8 app_id);

#ifdef __cplusplus
	}
#endif

#endif // BTE_BTA_HH_CO_H
