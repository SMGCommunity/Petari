#ifndef BTE_H
#define BTE_H

/* Original source:
 * bluedroid <android.googlesource.com/platform/external/bluetooth/bluedroid>
 * include/bte.h
 */

/******************************************************************************
 *
 *  Copyright (C) 2001-2012 Broadcom Corporation
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

#include <decomp.h>

#include "data_types.h"

#include "hci.h"

/*******************************************************************************
 * types
 */

#ifdef __cplusplus
	extern "C" {
#endif

typedef UINT8 tBTE_TARGET_MODE;
enum
{
	BTE_MODE_SERIAL_APP	= 0,
};

/*******************************************************************************
 * external globals
 */

extern char const bte_version_string[];

extern tBTE_TARGET_MODE volatile bte_target_mode;

extern tHCI_IF *p_hcisu_if;
extern tHCI_CFG *p_hcisu_cfg;

/*******************************************************************************
 * functions
 */

void BTE_InitStack(void);

// ---

void bte_hcisu_send(HC_BT_HDR *p_msg, UINT16 event);
void bte_hcisu_task(unk_t);
void bte_hcisu_close(void);
void bta_ci_hci_msg_handler(void *p_data);

void BTE_InitStack(void);

void BTE_LoadStack(void);
void BTE_UnloadStack(void);

#ifdef __cplusplus
	}
#endif

#endif // BTE_H
