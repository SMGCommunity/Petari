#ifndef BTE_GKI_H
#define BTE_GKI_H

/* Original source:
 * bluedroid <android.googlesource.com/platform/external/bluetooth/bluedroid>
 * gki/common/gki.h
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

#include "data_types.h"

/*******************************************************************************
 * macros
 */

#define EVENT_MASK(evt)	((UINT16)(1 << (evt)))

/*******************************************************************************
 * types
 */

#ifdef __cplusplus
	extern "C" {
#endif

typedef UINT8 tGKI_STATUS;
enum
{
	GKI_SUCCESS			= 0,
	GKI_FAILURE			= 1,
	GKI_INVALID_TASK	= 240,
	GKI_INVALID_POOL	= 255,
};

enum
{
	TASK_MBOX_0		= 0,
	TASK_MBOX_2		= 2,

	NUM_TASK_MBOX	= 4
};

enum
{
	TASK_MBOX_0_EVT_MASK	= 1 << 0,
	TASK_MBOX_1_EVT_MASK	= 1 << 1,
	TASK_MBOX_2_EVT_MASK	= 1 << 2,
	TASK_MBOX_3_EVT_MASK	= 1 << 3,
};

enum
{
	TIMER_0	= 0,
	TIMER_1	= 1,
};

enum
{
	TIMER_0_EVT_MASK	= 0x0010,
	TIMER_1_EVT_MASK	= 0x0020,
};

enum
{
	GKI_RESTRICTED_POOL	= 1,
};

typedef void TIMER_CBACK(void *p_tle);

#ifndef TIMER_PARAM_TYPE
# define TIMER_PARAM_TYPE	__typeof__(UINT32)
#endif

#ifndef TASKPTR
# define TASKPTR			__typeof__(void (*)(UINT32))
#endif

typedef struct
{
	void	*p_first;	// size 0x04, offset 0x00
	void	*p_last;	// size 0x04, offset 0x04
	UINT16	count;		// size 0x02, offset 0x08
	/* 2 bytes padding */
} BUFFER_Q; // size 0x0c

typedef struct _tle
{
	struct _tle			*p_next;	// size 0x04, offset 0x00
	struct _tle			*p_prev;	// size 0x04, offset 0x04
	TIMER_CBACK			*p_cback;	// size 0x04, offset 0x08
	INT32				ticks;		// size 0x04, offset 0x0c
	TIMER_PARAM_TYPE	param;		// size 0x04, offset 0x10
	UINT16				event;		// size 0x02, offset 0x14
	UINT8				in_use;		// size 0x01, offset 0x16
	/* 1 byte padding */
} TIMER_LIST_ENT; // size 0x18

typedef struct
{
	TIMER_LIST_ENT	*p_first;	// size 0x04, offset 0x00
	TIMER_LIST_ENT	*p_last;	// size 0x04, offset 0x04
	INT32			last_ticks;	// size 0x04, offset 0x08
} TIMER_LIST_Q; // size 0x0c

/*******************************************************************************
 * functions
 */

UINT8 GKI_set_pool_permission(UINT8 pool_id, UINT8 permission);
/**/
void GKI_init_q(BUFFER_Q *p_q);
void *GKI_getbuf(UINT16 size);
void *GKI_getpoolbuf(UINT8 pool_id);
void *GKI_igetpoolbuf(UINT8 pool_id);
void GKI_freebuf(void *p_buf);
UINT16 GKI_get_buf_size(void *p_buf);
UINT16 GKI_poolfreecount(UINT8 pool_id);
/**/
void GKI_change_buf_owner(void *p_buf, UINT8 task_id);
void GKI_send_msg(UINT8 task_id, UINT8 mbox, void *msg);
void *GKI_read_mbox(UINT8 mbox);
void GKI_enqueue(BUFFER_Q *p_q, void *p_buf);
void GKI_enqueue_head(BUFFER_Q *p_q, void *p_buf);
void *GKI_dequeue(BUFFER_Q *p_q);
void *GKI_remove_from_queue(BUFFER_Q *p_q, void *p_buf);
void *GKI_getfirst(BUFFER_Q *p_q);
void *GKI_getnext(void *p_buf);
BOOLEAN GKI_queue_is_empty(BUFFER_Q *p_q);
void *GKI_find_buf_start(void *p_user_area);
UINT8 GKI_create_pool(UINT16 size, UINT16 count, UINT8 permission,
                      void *p_mem_pool);
void GKI_delete_pool(UINT8 pool_id);

UINT32 GKI_get_tick_count(void);
INT32 GKI_ready_to_sleep(void);
void GKI_start_timer(UINT8 tnum, INT32 ticks, BOOLEAN is_continuous);
void GKI_stop_timer(UINT8 tnum);
void GKI_timer_update(INT32 ticks_since_last_update);
void GKI_init_timer_list(TIMER_LIST_Q *p_timer_listq);
void GKI_init_timer_list_entry(TIMER_LIST_ENT *p_tle);
UINT16 GKI_update_timer_list(TIMER_LIST_Q *p_timer_listq,
                             INT32 num_units_since_last_update);
void GKI_add_to_timer_list(TIMER_LIST_Q *p_timer_listq, TIMER_LIST_ENT *p_tle);
void GKI_remove_from_timer_list(TIMER_LIST_Q *p_timer_listq,
                                TIMER_LIST_ENT *p_tle);

void GKI_init(void);
void GKI_shutdown(void);
/**/
UINT8 GKI_create_task(TASKPTR task_entry, UINT8 task_id, INT8 *taskname,
                      UINT16 *stack, UINT16 stacksize);
void GKI_run(void *p_task_id);
UINT8 GKI_suspend_task(UINT8 task_id);
UINT8 GKI_resume_task(UINT8 task_id);
void GKI_exit_task(UINT8 task_id);
void GKI_sched_lock(void);
void GKI_sched_unlock(void);
UINT16 GKI_wait(UINT16 flag, UINT32 timeout);
void GKI_delay(UINT32 timeout);
UINT8 GKI_send_event(UINT8 task_id, UINT16 event);
UINT8 GKI_isend_event(UINT8 task_id, UINT16 event);
UINT8 GKI_get_taskid(void);
INT8 *GKI_map_taskname(UINT8 task_id);
void GKI_enable(void);
void GKI_disable(void);
void GKI_exception(UINT16 code, char *msg);
INT8 *GKI_get_time_stamp(INT8 *tbuf);
void GKI_register_mempool(void *p_mem);
void *GKI_os_malloc(UINT32 size);
void GKI_os_free(void *p_mem);

#ifdef __cplusplus
	}
#endif

#endif // BTE_GKI_H
