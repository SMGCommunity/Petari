#include "gki.h"

/* Original source:
 * bluedroid <android.googlesource.com/platform/external/bluetooth/bluedroid>
 * gki/common/gki_time.c
 */

/******************************************************************************
 *
 *  Copyright (C) 1999-2012 Broadcom Corporation
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

#include "data_types.h"
#include "gki_target.h"

#include "gki_common.h"
#include "gki_int.h"

/*******************************************************************************
 * macros
 */

#define GKI_NO_NEW_TMRS_STARTED	0x7fffffffl

#define GKI_UNUSED_LIST_ENTRY	0x80000000l

#define GKI_MAX_INT32			0x7fffffffl // ...ok, i guess

/*******************************************************************************
 * functions
 */

void gki_timers_init(void)
{
	UINT8 tt;

	gki_cb.com.OSTicksTilExp = 0;
	gki_cb.com.OSNumOrigTicks = 0;

	for (tt = 0; tt < GKI_MAX_TASKS; ++tt)
	{
		gki_cb.com.OSWaitTmr[tt] = 0;
		gki_cb.com.OSTaskTmr0[tt] = 0;
		gki_cb.com.OSTaskTmr0R[tt] = 0;
		gki_cb.com.OSTaskTmr1[tt] = 0;
		gki_cb.com.OSTaskTmr1R[tt] = 0;
	}
}

UINT32 GKI_get_tick_count(void)
{
	return gki_cb.com.OSTicks;
}

INT32 GKI_ready_to_sleep(void)
{
	return gki_cb.com.OSTicksTilExp;
}

void GKI_start_timer(UINT8 tnum, INT32 ticks, BOOLEAN is_continuous)
{
	INT32 reload;
	INT32 orig_ticks;
	UINT8 task_id = GKI_get_taskid();
	BOOLEAN bad_timer = FALSE;

	if (ticks <= 0)
		ticks = 1;

	orig_ticks = ticks;

	if (is_continuous)
		reload = ticks;
	else
		reload = 0;

	GKI_disable();

	if (GKI_MAX_INT32 - (gki_cb.com.OSNumOrigTicks - gki_cb.com.OSTicksTilExp)
	    > ticks)
	{
		ticks += gki_cb.com.OSNumOrigTicks - gki_cb.com.OSTicksTilExp;
	}
	else
	{
		ticks = GKI_MAX_INT32;
	}

	switch (tnum)
	{
	case TIMER_0:
		gki_cb.com.OSTaskTmr0R[task_id] = reload;
		gki_cb.com.OSTaskTmr0[task_id] = ticks;
		break;

	case TIMER_1:
		gki_cb.com.OSTaskTmr1R[task_id] = reload;
		gki_cb.com.OSTaskTmr1[task_id] = ticks;
		break;

	default:
		bad_timer = TRUE;
	}

	if (!bad_timer)
		gki_adjust_timer_count(orig_ticks);

	GKI_enable();
}

void GKI_stop_timer(UINT8 tnum)
{
	UINT8 task_id = GKI_get_taskid();

	switch (tnum)
	{
	case TIMER_0:
		gki_cb.com.OSTaskTmr0R[task_id] = 0;
		gki_cb.com.OSTaskTmr0[task_id] = 0;
		break;

	case TIMER_1:
		gki_cb.com.OSTaskTmr1R[task_id] = 0;
		gki_cb.com.OSTaskTmr1[task_id] = 0;
		break;
	}
}

void GKI_timer_update(INT32 ticks_since_last_update)
{
	UINT8 task_id;
	INT32 next_expiration;

	gki_cb.com.OSTicks += ticks_since_last_update;
	gki_cb.com.OSTicksTilExp -= ticks_since_last_update;

	if (gki_cb.com.timer_nesting)
		return;

	gki_cb.com.timer_nesting = TRUE;

	if (gki_cb.com.OSTicksTilExp > 0)
	{
		gki_cb.com.timer_nesting = FALSE;
		return;
	}

	next_expiration = GKI_NO_NEW_TMRS_STARTED;

	gki_cb.com.OSNumOrigTicks -= gki_cb.com.OSTicksTilExp;

	for (task_id = 0; task_id < GKI_MAX_TASKS; ++task_id)
	{
		if (gki_cb.com.OSWaitTmr[task_id] > 0)
		{
			gki_cb.com.OSWaitTmr[task_id] -= gki_cb.com.OSNumOrigTicks;

			if (gki_cb.com.OSWaitTmr[task_id] <= 0)
				gki_cb.com.OSRdyTbl[task_id] = TASK_READY;
		}

		if (gki_cb.com.OSTaskTmr0[task_id] > 0)
		{
			gki_cb.com.OSTaskTmr0[task_id] -= gki_cb.com.OSNumOrigTicks;

			if (gki_cb.com.OSTaskTmr0[task_id] <= 0)
			{
				GKI_send_event(task_id, TIMER_0_EVT_MASK);

				gki_cb.com.OSTaskTmr0[task_id] =
					gki_cb.com.OSTaskTmr0R[task_id];

			}
		}

		if (gki_cb.com.OSTaskTmr0[task_id] > 0
		    && gki_cb.com.OSTaskTmr0[task_id] < next_expiration)
		{
			next_expiration = gki_cb.com.OSTaskTmr0[task_id];
		}

		if (gki_cb.com.OSTaskTmr1[task_id] > 0)
		{
			gki_cb.com.OSTaskTmr1[task_id] -= gki_cb.com.OSNumOrigTicks;

			if (gki_cb.com.OSTaskTmr1[task_id] <= 0)
			{
				GKI_send_event(task_id, TIMER_1_EVT_MASK);

				gki_cb.com.OSTaskTmr1[task_id] =
					gki_cb.com.OSTaskTmr1R[task_id];
			}
		}

		if (gki_cb.com.OSTaskTmr1[task_id] > 0
		    && gki_cb.com.OSTaskTmr1[task_id] < next_expiration)
		{
			next_expiration = gki_cb.com.OSTaskTmr1[task_id];
		}
	}

	if (next_expiration < GKI_NO_NEW_TMRS_STARTED)
		gki_cb.com.OSTicksTilExp = gki_cb.com.OSNumOrigTicks = next_expiration;
	else
		gki_cb.com.OSTicksTilExp = gki_cb.com.OSNumOrigTicks = 0;

	gki_cb.com.timer_nesting = FALSE;
}

void GKI_init_timer_list(TIMER_LIST_Q *p_timer_listq)
{
	p_timer_listq->p_first = NULL;
	p_timer_listq->p_last = NULL;
	p_timer_listq->last_ticks = 0;
}

void GKI_init_timer_list_entry(TIMER_LIST_ENT *p_tle)
{
	p_tle->p_next = NULL;
	p_tle->p_prev = NULL;
	p_tle->ticks = GKI_UNUSED_LIST_ENTRY;
	p_tle->in_use = FALSE;
}

UINT16 GKI_update_timer_list(TIMER_LIST_Q *p_timer_listq,
                             INT32 num_units_since_last_update)
{
	TIMER_LIST_ENT *p_tle;
	UINT16 num_time_out = 0;
	INT32 rem_ticks;
	INT32 temp_ticks;

	p_tle = p_timer_listq->p_first;

	while (p_tle && p_tle->ticks <= 0)
	{
		++num_time_out;

		p_tle = p_tle->p_next;
	}

	rem_ticks = num_units_since_last_update;

	while (p_tle && rem_ticks > 0)
	{
		temp_ticks = p_tle->ticks;
		p_tle->ticks -= rem_ticks;

		if (p_tle->ticks <= 0)
		{
			p_tle->ticks = 0;

			++num_time_out;
		}

		rem_ticks -= temp_ticks;
		p_tle = p_tle->p_next;
	}

	if (p_timer_listq->last_ticks > 0)
	{
		p_timer_listq->last_ticks -= num_units_since_last_update;

		if (p_timer_listq->last_ticks < 0)
			p_timer_listq->last_ticks = 0;
	}

	return num_time_out;
}

void GKI_add_to_timer_list(TIMER_LIST_Q *p_timer_listq, TIMER_LIST_ENT *p_tle)
{
	UINT32 nr_ticks_total;
	TIMER_LIST_ENT *p_temp;

	if (p_tle->ticks < 0)
		return;

	if (p_tle->ticks >= p_timer_listq->last_ticks)
	{
		if (!p_timer_listq->p_first)
		{
			p_timer_listq->p_first = p_tle;
		}
		else
		{
			if (p_timer_listq->p_last)
				p_timer_listq->p_last->p_next = p_tle;

			p_tle->p_prev = p_timer_listq->p_last;
		}

		p_tle->p_next = NULL;
		p_timer_listq->p_last = p_tle;

		nr_ticks_total = p_tle->ticks;
		p_tle->ticks -= p_timer_listq->last_ticks;
		p_timer_listq->last_ticks = nr_ticks_total;
	}
	else
	{
		for (p_temp = p_timer_listq->p_first; p_tle->ticks > p_temp->ticks;
		     p_temp = p_temp->p_next)
		{
			if (p_temp->ticks > 0)
				p_tle->ticks -= p_temp->ticks;
		}

		if (p_temp == p_timer_listq->p_first)
		{
			p_tle->p_next = p_timer_listq->p_first;
			p_timer_listq->p_first->p_prev = p_tle;
			p_timer_listq->p_first = p_tle;
		}
		else
		{
			p_temp->p_prev->p_next = p_tle;
			p_tle->p_prev = p_temp->p_prev;
			p_temp->p_prev = p_tle;
			p_tle->p_next = p_temp;
		}

		p_temp->ticks -= p_tle->ticks;
	}

	p_tle->in_use = TRUE;
}

void GKI_remove_from_timer_list(TIMER_LIST_Q *p_timer_listq,
                                TIMER_LIST_ENT *p_tle)
{
	if (!p_tle || !p_tle->in_use || !p_timer_listq->p_first)
		return;

	if (p_tle->p_next)
		p_tle->p_next->ticks += p_tle->ticks;
	else
		p_timer_listq->last_ticks -= p_tle->ticks;

	if (p_timer_listq->p_first == p_tle)
	{
		p_timer_listq->p_first = p_tle->p_next;

		if (p_timer_listq->p_first)
			p_timer_listq->p_first->p_prev = NULL;

		if (p_timer_listq->p_last == p_tle)
			p_timer_listq->p_last = NULL;
	}
	else if (p_timer_listq->p_last == p_tle)
	{
		p_timer_listq->p_last = p_tle->p_prev;

		if (p_timer_listq->p_last)
			p_timer_listq->p_last->p_next = NULL;
	}
	else
	{
		if (p_tle->p_next && p_tle->p_next->p_prev == p_tle)
			p_tle->p_next->p_prev = p_tle->p_prev;
		else
			return;

		if (p_tle->p_prev && p_tle->p_prev->p_next == p_tle)
			p_tle->p_prev->p_next = p_tle->p_next;
		else
			return;
	}

	p_tle->p_next = p_tle->p_prev = NULL;
	p_tle->ticks = GKI_UNUSED_LIST_ENTRY;
	p_tle->in_use = FALSE;
}

void gki_adjust_timer_count(INT32 ticks)
{
	if (ticks <= 0)
		return;

	if (gki_cb.com.OSNumOrigTicks == 0
	    || (ticks < gki_cb.com.OSTicksTilExp && gki_cb.com.OSTicksTilExp > 0))
	{
		gki_cb.com.OSNumOrigTicks =
			gki_cb.com.OSNumOrigTicks - gki_cb.com.OSTicksTilExp + ticks;

		gki_cb.com.OSTicksTilExp = ticks;
	}
}
