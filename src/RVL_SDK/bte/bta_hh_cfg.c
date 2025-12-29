#include "bta_hh_int.h"

/* Original source:
 * bluedroid <android.googlesource.com/platform/external/bluetooth/bluedroid>
 * bta/hh/bta_hh_cfg.c
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
 * variables
 */

tBTA_HH_SPT_TOD p_devt_list[];

// .rodata
tBTA_HH_CFG const bta_hh_cfg =
{
	4,
	p_devt_list,
	0x400
};

// .sdata
tBTA_HH_SPT_TOD p_devt_list[] =
{
	{0x20, 0x01},
	{0x10, 0x02},
	{0x01, 0x03},
	{0x31, 0x02},
};

tBTA_HH_CFG *p_bta_hh_cfg = (tBTA_HH_CFG *)&bta_hh_cfg;
