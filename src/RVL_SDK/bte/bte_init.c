#include "bte.h"

/* Original source:
 * bluedroid <android.googlesource.com/platform/external/bluetooth/bluedroid>
 * main/bte_init.c
 */

/******************************************************************************
 *
 *  Copyright (C) 2000-2012 Broadcom Corporation
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

#include "gap_api.h" // GAP_Init
#include "hidd_api.h" // HID_DevInit
#include "hidh_api.h" // HID_HostInit
#include "port_api.h" // RFCOMM_Init

/*******************************************************************************
 * functions
 */

void BTE_InitStack(void)
{
	RFCOMM_Init();
	GAP_Init();
	HID_DevInit();
	HID_HostInit();
}
