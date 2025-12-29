#include "bta_sys.h"

/* Original source:
 * bluedroid <android.googlesource.com/platform/external/bluetooth/bluedroid>
 * bta/sys/bta_sys_conn.c
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

#include "bt_types.h" // BD_ADDR
#include "data_types.h" // UINT8

#include "bta_sys_int.h" // bta_sys_cb

/*******************************************************************************
 * functions
 */

void bta_sys_rm_register(tBTA_SYS_CONN_CBACK *p_cback)
{
	bta_sys_cb.prm_cb = p_cback;
}

void bta_sys_compress_register(tBTA_SYS_CONN_CBACK *p_cback)
{
	bta_sys_cb.compress_cb = p_cback;
}

void bta_sys_pm_register(tBTA_SYS_CONN_CBACK *p_cback)
{
	bta_sys_cb.ppm_cb = p_cback;
}

void bta_sys_conn_open(UINT8 id, UINT8 app_id, BD_ADDR peer_addr)
{
	if (bta_sys_cb.prm_cb)
		(*bta_sys_cb.prm_cb)(BTA_SYS_CONN_OPEN, id, app_id, peer_addr);

	if (bta_sys_cb.ppm_cb)
		(*bta_sys_cb.ppm_cb)(BTA_SYS_CONN_OPEN, id, app_id, peer_addr);

	if (bta_sys_cb.compress_cb)
		(*bta_sys_cb.compress_cb)(BTA_SYS_CONN_OPEN, id, app_id, peer_addr);
}

void bta_sys_conn_close(UINT8 id, UINT8 app_id, BD_ADDR peer_addr)
{
	if (bta_sys_cb.prm_cb)
		(*bta_sys_cb.prm_cb)(BTA_SYS_CONN_CLOSE, id, app_id, peer_addr);

	if (bta_sys_cb.ppm_cb)
		(*bta_sys_cb.ppm_cb)(BTA_SYS_CONN_CLOSE, id, app_id, peer_addr);

	if (bta_sys_cb.compress_cb)
		(*bta_sys_cb.compress_cb)(BTA_SYS_CONN_CLOSE, id, app_id, peer_addr);
}

void bta_sys_app_open(UINT8 id, UINT8 app_id, BD_ADDR peer_addr)
{
	if (bta_sys_cb.ppm_cb)
		(*bta_sys_cb.ppm_cb)(BTA_SYS_APP_OPEN, id, app_id, peer_addr);
}

void bta_sys_app_close(UINT8 id, UINT8 app_id, BD_ADDR peer_addr)
{
	if (bta_sys_cb.ppm_cb)
		(*bta_sys_cb.ppm_cb)(BTA_SYS_APP_CLOSE, id, app_id, peer_addr);
}

void bta_sys_sco_open(UINT8 id, UINT8 app_id, BD_ADDR peer_addr)
{
	if (bta_sys_cb.ppm_cb)
		(*bta_sys_cb.ppm_cb)(BTA_SYS_SCO_OPEN, id, app_id, peer_addr);
}

void bta_sys_sco_close(UINT8 id, UINT8 app_id, BD_ADDR peer_addr)
{
	if (bta_sys_cb.ppm_cb)
		(*bta_sys_cb.ppm_cb)(BTA_SYS_SCO_CLOSE, id, app_id, peer_addr);
}

void bta_sys_idle(UINT8 id, UINT8 app_id, BD_ADDR peer_addr)
{
	if (bta_sys_cb.ppm_cb)
		(*bta_sys_cb.ppm_cb)(BTA_SYS_CONN_IDLE, id, app_id, peer_addr);
}

void bta_sys_busy(UINT8 id, UINT8 app_id, BD_ADDR peer_addr)
{
	if (bta_sys_cb.ppm_cb)
		(*bta_sys_cb.ppm_cb)(BTA_SYS_CONN_BUSY, id, app_id, peer_addr);
}
