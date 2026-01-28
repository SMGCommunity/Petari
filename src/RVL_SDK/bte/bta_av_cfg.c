#include "bta_av_int.h"

/* Original source:
 * bluedroid <android.googlesource.com/platform/external/bluetooth/bluedroid>
 * bta/av/bta_av_cfg.c
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

#include "bta_av_api.h"

/*******************************************************************************
 * variables
 */

// dummy
extern signed bta_av_vdp_action;
extern signed bta_av_reg_vdp;

// .rodata
tBTA_AV_CFG const bta_av_cfg =
{
	0x00, 0x00, 0x10, 0x18,
	0x00, 0x30, 0x00, 0x02,
	0x00, 0x01, 0x02, 0xa0,
	0x03, 0xf0, 0x00, 0xc8,
	0x00, 0x06, 0x03, 0xf0,
	0x00, 0xc8, 0x00, 0x00,
	&bta_av_vdp_action,
	&bta_av_reg_vdp
};

UINT16 const bta_av_rc_id[] =
{
	0x021f,
	0x0000,
	0x0000,
	0x0003,
	0x1870,
	0x0000,
	0x0000,
	0x0000,
};

// .sdata
tBTA_AV_CFG *p_bta_av_cfg = (tBTA_AV_CFG *)&bta_av_cfg;
UINT16 *p_bta_av_rc_id = (UINT16 *)bta_av_rc_id;

