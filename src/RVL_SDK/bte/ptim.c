#include "ptim.h"

/* Original source:
 * bluedroid <android.googlesource.com/platform/external/bluetooth/bluedroid>
 * bta/sys/ptim.c
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

#include <stddef.h>

#include "gki_target.h"
#include "bt_types.h"
#include "data_types.h"

#include "bta_sys.h"
#include "gki.h"

/*******************************************************************************
 * functions
 */

void ptim_init(tPTIM_CB *p_cb, UINT16 period, UINT8 timer_id)
{
	GKI_init_timer_list(&p_cb->timer_queue);
	p_cb->period = period;
	p_cb->timer_id = timer_id;
}

void ptim_timer_update(tPTIM_CB *p_cb)
{
	TIMER_LIST_ENT *p_tle;
	BT_HDR *p_msg;

	GKI_update_timer_list(&p_cb->timer_queue, p_cb->period);

	while (p_cb->timer_queue.p_first && p_cb->timer_queue.p_first->ticks <= 0)
	{
		p_tle = p_cb->timer_queue.p_first;
		GKI_remove_from_timer_list(&p_cb->timer_queue, p_tle);

		if (p_tle->p_cback)
		{
			(*p_tle->p_cback)(p_tle);
		}
		else if (p_tle->event && (p_msg = GKI_getbuf(sizeof *p_msg)) != NULL)
		{
			p_msg->event = p_tle->event;
			bta_sys_sendmsg(p_msg);
		}
	}

	if (!p_cb->timer_queue.p_first)
		GKI_stop_timer(p_cb->timer_id);
}

void ptim_start_timer(tPTIM_CB *p_cb, TIMER_LIST_ENT *p_tle, UINT16 type,
                      INT32 timeout)
{
	if (!p_cb->timer_queue.p_first)
		GKI_start_timer(p_cb->timer_id, GKI_MS_TO_TICKS(p_cb->period), TRUE);

	GKI_remove_from_timer_list(&p_cb->timer_queue, p_tle);

	p_tle->event = type;
	p_tle->ticks = timeout;

	GKI_add_to_timer_list(&p_cb->timer_queue, p_tle);
}

void ptim_stop_timer(tPTIM_CB *p_cb, TIMER_LIST_ENT *p_tle)
{
	GKI_remove_from_timer_list(&p_cb->timer_queue, p_tle);

	if (!p_cb->timer_queue.p_first)
		GKI_stop_timer(p_cb->timer_id);
}
