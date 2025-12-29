#include "bta_pbs_int.h"

/* Original source:
 * bluedroid <android.googlesource.com/platform/external/bluetooth/bluedroid>
 * bta/pb/bta_pbs_cfg.c
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

#include "bta_pbs_api.h"

/*******************************************************************************
 * macros
 */

#ifndef BTA_PBS_REALM_CHARSET
# define BTA_PBS_REALM_CHARSET	0
#endif

#ifndef BTA_PBS_USERID_REQ
# define BTA_PBS_USERID_REQ		FALSE
#endif

/*******************************************************************************
 * variables
 */

// .sdata2
tBTA_PBS_CFG const bta_pbs_cfg =
{
	BTA_PBS_REALM_CHARSET,
	BTA_PBS_USERID_REQ,
	BTA_PBS_SUPF_DOWNLOAD | BTA_PBS_SURF_BROWSE,
	BTA_PBS_REPOSIT_LOCAL
};

// .sdata
tBTA_PBS_CFG *p_bta_pbs_cfg = (tBTA_PBS_CFG *)&bta_pbs_cfg;
