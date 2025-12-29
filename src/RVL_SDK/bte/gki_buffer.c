#include "gki.h"

/* Original source:
 * bluedroid <android.googlesource.com/platform/external/bluetooth/bluedroid>
 * gki/common/gki_buffer.c
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
 * local function declarations
 */

static void gki_init_free_queue(UINT8 id, UINT16 size, UINT16 total,
                                void *p_mem);

static void gki_add_to_pool_list(UINT8 pool_id);
static void gki_remove_from_pool_list(UINT8 pool_id);

/*******************************************************************************
 * functions
 */

static void gki_init_free_queue(UINT8 id, UINT16 size, UINT16 total,
                                void *p_mem)
{
	UINT16 i;
	UINT16 act_size;
	BUFFER_HDR_T *hdr;
	BUFFER_HDR_T *hdr1 = NULL;
	UINT32 *magic;
	INT32 tempsize = size;
	tGKI_COM_CB *p_cb = &gki_cb.com;

	tempsize = ALIGN_POOL(size);
	act_size = tempsize + BUFFER_PADDING_SIZE;

	p_cb->pool_start[id] = (UINT8 *)p_mem;
	p_cb->pool_end[id] = (UINT8 *)p_mem + act_size * total;

	p_cb->pool_size[id] = act_size;

	p_cb->freeq[id].size = tempsize;
	p_cb->freeq[id].total = total;
	p_cb->freeq[id].cur_cnt = 0;
	p_cb->freeq[id].max_cnt = 0;

	hdr = (BUFFER_HDR_T *)p_mem;
	p_cb->freeq[id].p_first = hdr;

	for (i = 0; i < total; ++i)
	{
		hdr->task_id = GKI_INVALID_TASK;
		hdr->q_id = id;
		hdr->status = BUF_STATUS_FREE;

		magic = (UINT32 *)((UINT8 *)(hdr + 1) + tempsize);
		*magic = MAGIC_NO;

		hdr1 = hdr;
		hdr = (BUFFER_HDR_T *)((UINT8 *)hdr + act_size);
		hdr1->p_next = hdr;
	}

	hdr1->p_next = NULL;
	p_cb->freeq[id].p_last = hdr1;
}

void gki_buffer_init(void)
{
	UINT8 i;
	UINT8 tt;
	UINT8 mb;
	tGKI_COM_CB *p_cb = &gki_cb.com;

	for (tt = 0; tt < GKI_MAX_TASKS; ++tt)
	{
		for (mb = 0; mb < NUM_TASK_MBOX; ++mb)
		{
			p_cb->OSTaskQFirst[tt][mb] = NULL;
			p_cb->OSTaskQLast[tt][mb] = NULL;
		}
	}

	for (tt = 0; tt < GKI_NUM_TOTAL_BUF_POOLS; ++tt)
	{
		p_cb->pool_start[tt] = NULL;
		p_cb->pool_end[tt] = NULL;
		p_cb->pool_size[tt] = 0;

		p_cb->freeq[tt].p_first = 0;
		p_cb->freeq[tt].p_last = 0;
		p_cb->freeq[tt].size = 0;
		p_cb->freeq[tt].total = 0;
		p_cb->freeq[tt].cur_cnt = 0;
		p_cb->freeq[tt].max_cnt = 0;
	}

	p_cb->pool_access_mask = GKI_DEF_BUFPOOL_PERM_MASK;

	gki_init_free_queue(0, GKI_BUF0_SIZE, GKI_BUF0_MAX, p_cb->bufpool0);
	gki_init_free_queue(1, GKI_BUF1_SIZE, GKI_BUF1_MAX, p_cb->bufpool1);
	gki_init_free_queue(2, GKI_BUF2_SIZE, GKI_BUF2_MAX, p_cb->bufpool2);
	gki_init_free_queue(3, GKI_BUF3_SIZE, GKI_BUF3_MAX, p_cb->bufpool3);
	gki_init_free_queue(4, GKI_BUF4_SIZE, GKI_BUF4_MAX, p_cb->bufpool4);

	for (i = 0; i < GKI_NUM_FIXED_BUF_POOLS; ++i)
		p_cb->pool_list[i] = i;

	p_cb->curr_total_no_of_pools = GKI_NUM_FIXED_BUF_POOLS;
}

UINT8 GKI_set_pool_permission(UINT8 pool_id, UINT8 permission)
{
	tGKI_COM_CB *p_cb = &gki_cb.com;

	if (pool_id < GKI_NUM_TOTAL_BUF_POOLS)
	{
		if (permission == GKI_RESTRICTED_POOL)
		{
			p_cb->pool_access_mask =
				p_cb->pool_access_mask | 1 << pool_id;
		}
		else
		{
			p_cb->pool_access_mask =
				p_cb->pool_access_mask & ~(1 << pool_id);
		}

		return GKI_SUCCESS;
	}
	else
	{
		return GKI_INVALID_POOL;
	}
}

static void gki_add_to_pool_list(UINT8 pool_id)
{
	INT32 i;
	INT32 j;
	tGKI_COM_CB *p_cb = &gki_cb.com;

	for (i = 0; i < p_cb->curr_total_no_of_pools; ++i)
	{
		if (p_cb->freeq[pool_id].size <= p_cb->freeq[p_cb->pool_list[i]].size)
			break;
	}

	for (j = p_cb->curr_total_no_of_pools; j > i; --j)
		p_cb->pool_list[j] = p_cb->pool_list[j - 1];

	p_cb->pool_list[i] = pool_id;
}

static void gki_remove_from_pool_list(UINT8 pool_id)
{
	tGKI_COM_CB *p_cb = &gki_cb.com;
	UINT8 i;

	for (i = 0; i < p_cb->curr_total_no_of_pools; ++i)
	{
		if (pool_id == p_cb->pool_list[i])
			break;
	}

	while (i < p_cb->curr_total_no_of_pools - 1)
	{
		p_cb->pool_list[i] = p_cb->pool_list[i + 1];
		++i;
	}
}

void GKI_init_q(BUFFER_Q *p_q)
{
	p_q->p_first = p_q->p_last = NULL;
	p_q->count = 0;
}

void *GKI_getbuf(UINT16 size)
{
	UINT8 i;
	FREE_QUEUE_T *Q;
	BUFFER_HDR_T *p_hdr;
	tGKI_COM_CB *p_cb = &gki_cb.com;

	if (size == 0)
	{
		GKI_exception(GKI_ERROR_BUF_SIZE_ZERO, "getbuf: Size is zero");
		return NULL;
	}

	for (i = 0; i < p_cb->curr_total_no_of_pools; ++i)
		if (size <= p_cb->freeq[p_cb->pool_list[i]].size)
			break;

	if (i == p_cb->curr_total_no_of_pools)
	{
		GKI_exception(GKI_ERROR_BUF_SIZE_TOOBIG, "getbuf: Size is too big");
		return NULL;
	}

	GKI_disable();

	for (; i < p_cb->curr_total_no_of_pools; ++i)
	{
		if (1 << p_cb->pool_list[i] & p_cb->pool_access_mask)
			continue;

		Q = &p_cb->freeq[p_cb->pool_list[i]];

		if (Q->cur_cnt < Q->total)
		{
			p_hdr = Q->p_first;
			Q->p_first = p_hdr->p_next;

			if (!Q->p_first)
				Q->p_last = NULL;

			if (++Q->cur_cnt > Q->max_cnt)
				Q->max_cnt = Q->cur_cnt;

			GKI_enable();

			p_hdr->task_id = GKI_get_taskid();
			p_hdr->status = BUF_STATUS_UNLINKED;
			p_hdr->p_next = NULL;
			p_hdr->type = 0;

			return p_hdr + 1;
		}
	}

	GKI_enable();

	return NULL;
}

void *GKI_getpoolbuf(UINT8 pool_id)
{
	FREE_QUEUE_T *Q;
	BUFFER_HDR_T *p_hdr;
	tGKI_COM_CB *p_cb = &gki_cb.com;

	if (pool_id >= GKI_NUM_TOTAL_BUF_POOLS)
		return NULL;

	GKI_disable();

	Q = &p_cb->freeq[pool_id];

	if (Q->cur_cnt < Q->total)
	{
		p_hdr = Q->p_first;
		Q->p_first = p_hdr->p_next;

		if (!Q->p_first)
			Q->p_last = NULL;

		if (++Q->cur_cnt > Q->max_cnt)
			Q->max_cnt = Q->cur_cnt;

		GKI_enable();

		p_hdr->task_id = GKI_get_taskid();

		p_hdr->status = BUF_STATUS_UNLINKED;
		p_hdr->p_next = NULL;
		p_hdr->type = 0;

		return p_hdr + 1;
	}

	GKI_enable();

	return GKI_getbuf(p_cb->freeq[pool_id].size);
}

void *GKI_igetpoolbuf(UINT8 pool_id)
{
	FREE_QUEUE_T *Q;
	BUFFER_HDR_T *p_hdr;

	if (pool_id >= GKI_NUM_TOTAL_BUF_POOLS)
		return NULL;

	Q = &gki_cb.com.freeq[pool_id];
	if (Q->cur_cnt < Q->total)
	{
		p_hdr = Q->p_first;
		Q->p_first = p_hdr->p_next;

		if (!Q->p_first)
			Q->p_last = NULL;

		if (++Q->cur_cnt > Q->max_cnt)
			Q->max_cnt = Q->cur_cnt;

		p_hdr->task_id = GKI_get_taskid();

		p_hdr->status = BUF_STATUS_UNLINKED;
		p_hdr->p_next = NULL;
		p_hdr->type = 0;

		return p_hdr + 1;
	}

	return NULL;
}

void GKI_freebuf(void *p_buf)
{
	FREE_QUEUE_T *Q;
	BUFFER_HDR_T *p_hdr;

	if (!p_buf || gki_chk_buf_damage(p_buf))
	{
		GKI_exception(GKI_ERROR_BUF_CORRUPTED, "Free - Buf Corrupted");
		return;
	}

	p_hdr = (BUFFER_HDR_T *)p_buf - 1;

	if (p_hdr->status != BUF_STATUS_UNLINKED)
	{
		GKI_exception(GKI_ERROR_FREEBUF_BUF_LINKED, "Freeing Linked Buf");
		return;
	}

	if (p_hdr->q_id >= GKI_NUM_TOTAL_BUF_POOLS)
	{
		GKI_exception(GKI_ERROR_FREEBUF_BAD_QID, "Bad Buf QId");
		return;
	}

	GKI_disable();

	Q = &gki_cb.com.freeq[p_hdr->q_id];
	if (Q->p_last)
		Q->p_last->p_next = p_hdr;
	else
		Q->p_first = p_hdr;

	Q->p_last = p_hdr;
	p_hdr->p_next = NULL;
	p_hdr->status = BUF_STATUS_FREE;
	p_hdr->task_id = GKI_INVALID_TASK;

	if (Q->cur_cnt > 0)
		--Q->cur_cnt;

	GKI_enable();
}

UINT16 GKI_get_buf_size(void *p_buf)
{
	BUFFER_HDR_T *p_hdr = (BUFFER_HDR_T *)p_buf - 1;

	if ((UINT32)p_hdr & 1)
		return 0;

	if (p_hdr->q_id < GKI_NUM_TOTAL_BUF_POOLS)
		return gki_cb.com.freeq[p_hdr->q_id].size;
	else
		return 0;
}

UINT16 GKI_poolfreecount(UINT8 pool_id)
{
	FREE_QUEUE_T *Q;

	if (pool_id >= GKI_NUM_TOTAL_BUF_POOLS)
		return 0;

	Q = &gki_cb.com.freeq[pool_id];

	return Q->total - Q->cur_cnt;
}

BOOLEAN gki_chk_buf_damage(void *p_buf)
{
	UINT32 *magic = (UINT32 *)((UINT8 *)p_buf + GKI_get_buf_size(p_buf));

	if ((UINT32)magic & 1)
		return TRUE;

	if (*magic == MAGIC_NO)
		return FALSE;

	return TRUE;
}

void GKI_change_buf_owner(void *p_buf, UINT8 task_id)
{
	BUFFER_HDR_T *p_hdr = (BUFFER_HDR_T *)p_buf - 1;

	p_hdr->task_id = task_id;
}

void GKI_send_msg(UINT8 task_id, UINT8 mbox, void *msg)
{
	BUFFER_HDR_T *p_hdr;
	tGKI_COM_CB *p_cb = &gki_cb.com;

	if (task_id >= GKI_MAX_TASKS || mbox >= NUM_TASK_MBOX
	    || p_cb->OSRdyTbl[task_id] == TASK_DEAD)
	{
		GKI_exception(GKI_ERROR_SEND_MSG_BAD_DEST, "Sending to unknown dest");
		GKI_freebuf(msg);
		return;
	}

	if (gki_chk_buf_damage(msg))
	{
		GKI_exception(GKI_ERROR_BUF_CORRUPTED, "Send - Buffer corrupted");
		return;
	}

	p_hdr = (BUFFER_HDR_T *)msg - 1;

	if (p_hdr->status != BUF_STATUS_UNLINKED)
	{
		GKI_exception(GKI_ERROR_SEND_MSG_BUF_LINKED, "Send - buffer linked");
		return;
	}

	GKI_disable();

	if (p_cb->OSTaskQFirst[task_id][mbox])
		p_cb->OSTaskQLast[task_id][mbox]->p_next = p_hdr;
	else
		p_cb->OSTaskQFirst[task_id][mbox] = p_hdr;

	p_cb->OSTaskQLast[task_id][mbox] = p_hdr;

	p_hdr->p_next = NULL;
	p_hdr->status = BUF_STATUS_QUEUED;
	p_hdr->task_id = task_id;

	GKI_enable();

	GKI_send_event(task_id, EVENT_MASK(mbox));
}

void *GKI_read_mbox(UINT8 mbox)
{
	UINT8 task_id = GKI_get_taskid();
	void *p_buf = NULL;
	BUFFER_HDR_T *p_hdr;

	if (task_id >= GKI_MAX_TASKS || mbox >= NUM_TASK_MBOX)
		return NULL;

	GKI_disable();

	if (gki_cb.com.OSTaskQFirst[task_id][mbox])
	{
		p_hdr = gki_cb.com.OSTaskQFirst[task_id][mbox];
		gki_cb.com.OSTaskQFirst[task_id][mbox] = p_hdr->p_next;

		p_hdr->p_next = NULL;
		p_hdr->status = BUF_STATUS_UNLINKED;

		p_buf = p_hdr + 1;
	}

	GKI_enable();

	return p_buf;
}

void GKI_enqueue(BUFFER_Q *p_q, void *p_buf)
{
	BUFFER_HDR_T *p_hdr;

	if (gki_chk_buf_damage(p_buf))
	{
		GKI_exception(GKI_ERROR_BUF_CORRUPTED, "Enqueue - Buffer corrupted");
		return;
	}

	p_hdr = (BUFFER_HDR_T *)p_buf - 1;

	if (p_hdr->status != BUF_STATUS_UNLINKED)
	{
		GKI_exception(GKI_ERROR_ENQUEUE_BUF_LINKED,
		              "Eneueue - buf already linked");
		return;
	}

	GKI_disable();

	if (p_q->p_first)
	{
		BUFFER_HDR_T *p_last_hdr = (BUFFER_HDR_T *)p_q->p_last - 1;

		p_last_hdr->p_next = p_hdr;
	}
	else
	{
		p_q->p_first = p_buf;
	}

	p_q->p_last = p_buf;
	++p_q->count;

	p_hdr->p_next = NULL;
	p_hdr->status = BUF_STATUS_QUEUED;

	GKI_enable();
}

void GKI_enqueue_head(BUFFER_Q *p_q, void *p_buf)
{
	BUFFER_HDR_T *p_hdr;

	if (gki_chk_buf_damage(p_buf))
	{
		GKI_exception(GKI_ERROR_BUF_CORRUPTED, "Enqueue - Buffer corrupted");
		return;
	}

	p_hdr = (BUFFER_HDR_T *)p_buf - 1;

	if (p_hdr->status != BUF_STATUS_UNLINKED)
	{
		GKI_exception(GKI_ERROR_ENQUEUE_BUF_LINKED,
		              "Eneueue head - buf already linked");
		return;
	}

	GKI_disable();

	if (p_q->p_first)
	{
		p_hdr->p_next = (BUFFER_HDR_T *)p_q->p_first - 1;
		p_q->p_first = p_buf;
	}
	else
	{
		p_q->p_first = p_buf;
		p_q->p_last = p_buf;
		p_hdr->p_next = NULL;
	}

	++p_q->count;

	p_hdr->status = BUF_STATUS_QUEUED;

	GKI_enable();
}

void *GKI_dequeue(BUFFER_Q *p_q)
{
	BUFFER_HDR_T *p_hdr;

	GKI_disable();

	if (!p_q || !p_q->count)
	{
		GKI_enable();
		return NULL;
	}

	p_hdr = (BUFFER_HDR_T *)p_q->p_first - 1;

	if (p_hdr->p_next)
	{
		p_q->p_first = (BUFFER_HDR_T *)p_hdr->p_next + 1;
	}
	else
	{
		p_q->p_first = NULL;
		p_q->p_last = NULL;
	}

	--p_q->count;

	p_hdr->p_next = NULL;
	p_hdr->status = BUF_STATUS_UNLINKED;

	GKI_enable();

	return (BUFFER_HDR_T *)p_hdr + 1;
}

void *GKI_remove_from_queue(BUFFER_Q *p_q, void *p_buf)
{
	BUFFER_HDR_T *p_prev;
	BUFFER_HDR_T *p_buf_hdr;

	GKI_disable();

	if (p_buf == p_q->p_first)
	{
		GKI_enable();
		return GKI_dequeue(p_q);
	}

	p_buf_hdr = (BUFFER_HDR_T *)p_buf - 1;
	p_prev = (BUFFER_HDR_T *)p_q->p_first - 1;

	for (; p_prev; p_prev = p_prev->p_next)
	{
		if (p_prev->p_next == p_buf_hdr)
		{
			p_prev->p_next = p_buf_hdr->p_next;

			if (p_buf == p_q->p_last)
				p_q->p_last = p_prev + 1;

			--p_q->count;

			p_buf_hdr->p_next = NULL;
			p_buf_hdr->status = BUF_STATUS_UNLINKED;

			GKI_enable();

			return p_buf;
		}
	}

	GKI_enable();

	return NULL;
}

void *GKI_getfirst(BUFFER_Q *p_q)
{
	return p_q->p_first;
}

void *GKI_getnext(void *p_buf)
{
	BUFFER_HDR_T *p_hdr = (BUFFER_HDR_T *)p_buf - 1;

	if (p_hdr->p_next)
		return (BUFFER_HDR_T *)p_hdr->p_next + 1;
	else
		return NULL;
}

BOOLEAN GKI_queue_is_empty(BUFFER_Q *p_q)
{
	return p_q->count == 0;
}

void *GKI_find_buf_start(void *p_user_area)
{
	UINT16 xx;
	UINT16 size;
	UINT32 yy;
	tGKI_COM_CB *p_cb = &gki_cb.com;
	UINT8 *p_ua = p_user_area;

	for (xx = 0; xx < GKI_NUM_TOTAL_BUF_POOLS; ++xx)
	{
		if (p_ua > p_cb->pool_start[xx] && p_ua < p_cb->pool_end[xx])
		{
			yy = p_ua - p_cb->pool_start[xx];
			size = p_cb->pool_size[xx];

			yy = yy / size * size;

			return p_cb->pool_start[xx] + yy + sizeof(BUFFER_HDR_T);
		}
	}

	GKI_exception(GKI_ERROR_BUF_SIZE_ZERO, "GKI_get_buf_start:: bad addr");

	return NULL;
}

UINT8 GKI_create_pool(UINT16 size, UINT16 count, UINT8 permission,
                      void *p_mem_pool)
{
	UINT8 xx;
	UINT32 mem_needed;
	INT32 tempsize = size;
	tGKI_COM_CB *p_cb = &gki_cb.com;

	if (size > MAX_USER_BUF_SIZE)
		return GKI_INVALID_POOL;

	for (xx = 0; xx < GKI_NUM_TOTAL_BUF_POOLS; ++xx)
	{
		if (!p_cb->pool_start[xx])
			break;
	}

	if (xx == GKI_NUM_TOTAL_BUF_POOLS)
		return GKI_INVALID_POOL;

	tempsize =ALIGN_POOL(size);

	mem_needed = (tempsize + BUFFER_PADDING_SIZE) * count;

	if (!p_mem_pool)
		p_mem_pool = GKI_os_malloc(mem_needed);

	if (p_mem_pool)
	{
		gki_init_free_queue(xx, size, count, p_mem_pool);
		gki_add_to_pool_list(xx);
		GKI_set_pool_permission(xx, permission);

		++p_cb->curr_total_no_of_pools;

		return xx;
	}
	else
	{
		return GKI_INVALID_POOL;
	}
}

void GKI_delete_pool(UINT8 pool_id)
{
	FREE_QUEUE_T *Q;
	tGKI_COM_CB *p_cb = &gki_cb.com;

	if (pool_id >= GKI_NUM_TOTAL_BUF_POOLS || !p_cb->pool_start[pool_id])
		return;

	GKI_disable();

	Q = &p_cb->freeq[pool_id];

	if (Q->cur_cnt == 0)
	{
		Q->size = 0;
		Q->total = 0;
		Q->cur_cnt = 0;
		Q->max_cnt = 0;
		Q->p_first = NULL;
		Q->p_last = NULL;

		GKI_os_free(p_cb->pool_start[pool_id]);

		p_cb->pool_start[pool_id] = NULL;
		p_cb->pool_end[pool_id] = NULL;
		p_cb->pool_size[pool_id] = 0;

		gki_remove_from_pool_list(pool_id);

		--p_cb->curr_total_no_of_pools;
	}
	else
	{
		GKI_exception(GKI_ERROR_DELETE_POOL_BAD_QID, "Deleting bad pool");
	}

	GKI_enable();
}
