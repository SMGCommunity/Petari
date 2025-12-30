#ifndef BTE_BTA_AV_INT_H
#define BTE_BTA_AV_INT_H

/* Original source:
 * bluedroid <android.googlesource.com/platform/external/bluetooth/bluedroid>
 * bta/av/bta_av_int.h
 */

/******************************************************************************
 *
 *  Copyright (C) 2004-2012 Broadcom Corporation
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

#include "bta_av_api.h"

/*******************************************************************************
 * external globals
 */

#ifdef __cplusplus
	extern "C" {
#endif

extern tBTA_AV_CFG const bta_av_cfg;
extern tBTA_AV_CFG *p_bta_av_cfg;

extern UINT16 const bta_av_rc_id[];
extern UINT16 *p_bta_av_rc_id;

#ifdef __cplusplus
	}
#endif

#endif // BTE_BTA_AV_INT_H
