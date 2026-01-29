#ifndef BTE_WBT_API_H
#define BTE_WBT_API_H

/* Original source:
 * bluedroid <android.googlesource.com/platform/external/bluetooth/bluedroid>
 * stack/include/wbt_api.h
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

#include "data_types.h"

/*******************************************************************************
 * macros
 */

#define BRCM_EXT_VERSION	1

/*******************************************************************************
 * types
 */

#ifdef __cplusplus
	extern "C" {
#endif

enum
{
	ATTR_ID_EXT_BRCM_VERSION	= 0x8001,
	ATTR_ID_EXT_PIN_CODE		= 0x8002,
};

/*******************************************************************************
 * functions
 */

BOOLEAN WBT_ExtCreateRecord(void);
BOOLEAN WBT_ExtAddPinCode(void);
UINT32 wbt_sdp_show_ext(UINT8 scn, char *service_name, UINT8 pin_code_ext,
                        UINT8 active_sync_ext);

#ifdef __cplusplus
	}
#endif

#endif // BTE_WBT_API_H
