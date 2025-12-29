#include "bta_api.h"
#include "bta_dm_int.h"

/* Original source:
 * bluedroid <android.googlesource.com/platform/external/bluetooth/bluedroid>
 * bta/dm/bta_dm_cfg.c
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

#include "bt_target.h"

#include "bta_sys.h"
#include "btm_api.h"
#include "hcidefs.h"

/*******************************************************************************
 * macros
 */

#ifndef BTA_DM_LINK_POLICY_SETTINGS
# define BTA_DM_LINK_POLICY_SETTINGS                        \
	 (HCI_ENABLE_MASTER_SLAVE_SWITCH | HCI_ENABLE_HOLD_MODE \
	  | HCI_ENABLE_SNIFF_MODE | HCI_ENABLE_PARK_MODE)
#endif

#ifndef BTA_DM_PAGE_TIMEOUT
# define BTA_DM_PAGE_TIMEOUT	8192
#endif

#define BTA_DM_LINK_TIMEOUT		5000

#ifndef BTA_DM_LINK_TIMEOUT
# define BTA_DM_LINK_TIMEOUT	8000
#endif

#define BTA_DM_NUM_RM_ENTRY		2

#ifndef BTA_DM_SCATTERNET
# define BTA_DM_SCATTERNET		BTA_DM_PARTIAL_SCATTERNET
#endif

#define BTUI_PAN_ID_NAP          1
#define BTUI_PAN_ID_GN           2

#ifndef BTA_AV_ROLE
# define BTA_AV_ROLE BTA_MASTER_ROLE_PREF
#endif

#define BTA_DM_NUM_PM_ENTRY	0

/*******************************************************************************
 * variables
 */

// .rodata/.sbss2
tBTA_DM_CFG const bta_dm_cfg =
{
	BTA_DM_COD,
	BTA_DM_LINK_POLICY_SETTINGS,
	BTA_DM_PAGE_TIMEOUT,
	BTA_DM_LINK_TIMEOUT
};

tBTA_DM_RM const bta_dm_rm_cfg[] =
{
	{BTA_ID_SYS, BTA_DM_NUM_RM_ENTRY, BTA_DM_SCATTERNET},
	{BTA_ID_PAN, BTUI_PAN_ID_NAP, BTA_MASTER_ROLE_ONLY},
	{BTA_ID_PAN, BTUI_PAN_ID_GN, BTA_MASTER_ROLE_ONLY},
	{BTA_ID_AV,  BTA_ALL_APP_ID, BTA_AV_ROLE},
};

tBTA_DM_COMPRESS const bta_dm_compress_cfg[] =
{
	{0, 5, 0},
	{10, 255, 1},
	{8, 255, 1},
	{6, 255, 1},
	{7, 255, 1},
	{18, 255, 2},
};

// this is the one in .sbss2
tBTA_DM_PM_CFG const bta_dm_pm_cfg[] =
{
	{BTA_ID_SYS, BTA_DM_NUM_PM_ENTRY, 0},
};

tBTA_DM_PM_SPEC const bta_dm_pm_spec[] =
{
	{
		3,
		{
			{{2, 5000}, {BTA_DM_PM_NO_ACTION, 0}},
			{{16, 0}, {BTA_DM_PM_NO_ACTION, 0}},
			{{BTA_DM_PM_NO_ACTION, 0}, {BTA_DM_PM_NO_ACTION, 0}},
			{{BTA_DM_PM_NO_ACTION, 0}, {BTA_DM_PM_NO_ACTION, 0}},
			{{BTA_DM_PM_NO_ACTION, 0}, {BTA_DM_PM_NO_ACTION, 0}},
			{{2, 5000}, {BTA_DM_PM_NO_ACTION, 0}},
			{{BTA_DM_PM_NO_ACTION, 0}, {BTA_DM_PM_NO_ACTION, 0}},
			{{BTA_DM_PM_NO_ACTION, 0}, {BTA_DM_PM_NO_ACTION, 0}},
			{{8, 5000}, {BTA_DM_PM_NO_ACTION, 0}},
		}
	},
};

tBTM_PM_PWR_MD const bta_dm_pm_md[] =
{
	{400, 200, 4, 4, BTM_PM_MD_SNIFF},
	{800, 400, 0, 0, BTM_PM_MD_PARK},
};

// .sdata
tBTA_DM_CFG *p_bta_dm_cfg = (tBTA_DM_CFG *)&bta_dm_cfg;
tBTA_DM_RM *p_bta_dm_rm_cfg = (tBTA_DM_RM *)bta_dm_rm_cfg;
tBTA_DM_COMPRESS *p_bta_dm_compress_cfg =
	(tBTA_DM_COMPRESS *)bta_dm_compress_cfg;
tBTA_DM_PM_CFG *p_bta_dm_pm_cfg = (tBTA_DM_PM_CFG *)bta_dm_pm_cfg;
tBTA_DM_PM_SPEC *p_bta_dm_pm_spec = (tBTA_DM_PM_SPEC *)&bta_dm_pm_spec;
tBTM_PM_PWR_MD *p_bta_dm_pm_md = (tBTM_PM_PWR_MD *)&bta_dm_pm_md;
