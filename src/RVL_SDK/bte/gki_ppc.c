#include "gki.h"

/* Original source:
 * bluedroid <android.googlesource.com/platform/external/bluetooth/bluedroid>
 * gki/ulinux/gki_ulinux.c
 */

/******************************************************************************
 *
 *  Copyright (C) 2009-2012 Broadcom Corporation
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

#include <cstring>
#include <mem.h>

#include "data_types.h"
#include "gki_target.h"

#include "gki_common.h"
#include "gki_int.h"

#include <revolution/os.h>
#include <revolution/types.h>

#define IS_BTE
#include <context_rvl.h>

/*******************************************************************************
 * local function declarations
 */

static UINT8 GKI_init_btu_task(UINT8 task_id);

extern void* App_MEMalloc(size_t size);
extern int App_MEMfree(void* ptr);

/*******************************************************************************
 * variables
 */

// .bss
tGKI_CB gki_cb;

/*******************************************************************************
 * functions
 */

void GKI_init(void) {
    memset(&gki_cb, 0, sizeof gki_cb);

    gki_buffer_init();
    gki_timers_init();

    GKI_init_btu_task(PPC_TASK);

    gki_cb.os.int_index = 0;
    gki_cb.os.int_state[gki_cb.os.int_index] = OSEnableInterrupts();
    ++gki_cb.os.int_index;
}

void GKI_shutdown(void) {
    GKI_exit_task(PPC_TASK);

    while (gki_cb.os.int_index > 0)
        GKI_enable();
}

static UINT8 GKI_init_btu_task(UINT8 task_id) {
    if (task_id >= GKI_MAX_TASKS)
        return GKI_FAILURE;

    gki_cb.com.OSRdyTbl[task_id] = TASK_READY;
    gki_cb.com.OSTName[task_id] = NULL;
    gki_cb.com.OSWaitTmr[task_id] = 0;
    gki_cb.com.OSWaitEvt[task_id] = 0;

    return GKI_SUCCESS;
}

UINT8 GKI_create_task(TASKPTR task_entry, UINT8 task_id, INT8* taskname, UINT16* stack, UINT16 stacksize) {
    return GKI_SUCCESS;
}

void GKI_run(void* p_task_id) {
    /* ... */
}

UINT8 GKI_suspend_task(UINT8 task_id) {
    return GKI_SUCCESS;
}

UINT8 GKI_resume_task(UINT8 task_id) {
    return GKI_SUCCESS;
}

void GKI_exit_task(UINT8 task_id) {
    GKI_disable();

    gki_cb.com.OSRdyTbl[task_id] = TASK_DEAD;

    GKI_enable();
}

void GKI_sched_lock(void) {
    /* ... */
}

void GKI_sched_unlock(void) {
    /* ... */
}

UINT16 GKI_wait(UINT16 flag, UINT32 timeout) {
    UINT16 evt;
    UINT8 rtask = GKI_get_taskid();

    gki_cb.com.OSWaitForEvt[rtask] = flag;

    if (gki_cb.com.OSTaskQFirst[rtask][0])
        gki_cb.com.OSWaitEvt[rtask] |= TASK_MBOX_0_EVT_MASK;

    if (gki_cb.com.OSTaskQFirst[rtask][1])
        gki_cb.com.OSWaitEvt[rtask] |= TASK_MBOX_1_EVT_MASK;

    if (gki_cb.com.OSTaskQFirst[rtask][2])
        gki_cb.com.OSWaitEvt[rtask] |= TASK_MBOX_2_EVT_MASK;

    if (gki_cb.com.OSTaskQFirst[rtask][3])
        gki_cb.com.OSWaitEvt[rtask] |= TASK_MBOX_3_EVT_MASK;

    if (!(gki_cb.com.OSWaitEvt[rtask] & flag)) {
        gki_cb.com.OSWaitTmr[rtask] = timeout;
        gki_adjust_timer_count(timeout);
    }

    gki_cb.com.OSWaitForEvt[rtask] = 0;

    evt = gki_cb.com.OSWaitEvt[rtask] & flag;
    gki_cb.com.OSWaitEvt[rtask] &= ~flag;

    return evt;
}

void GKI_delay(UINT32 timeout) {
    /* ... */
}

UINT8 GKI_send_event(UINT8 task_id, UINT16 event) {
    if (task_id >= GKI_MAX_TASKS)
        return GKI_FAILURE;

    GKI_disable();

    gki_cb.com.OSWaitEvt[task_id] |= event;

    GKI_enable();

    return GKI_SUCCESS;
}

UINT8 GKI_isend_event(UINT8 task_id, UINT16 event) {
    if (task_id >= GKI_MAX_TASKS)
        return GKI_FAILURE;

    gki_cb.com.OSWaitEvt[task_id] |= event;

    return GKI_SUCCESS;
}

UINT8 GKI_get_taskid(void) {
    return PPC_TASK;
}

INT8* GKI_map_taskname(UINT8 task_id) {
    if (task_id < GKI_MAX_TASKS)
        return gki_cb.com.OSTName[task_id];
    else if (task_id == GKI_MAX_TASKS)
        return gki_cb.com.OSTName[GKI_get_taskid()];
    else
        return (INT8*)"BAD";
}

void GKI_enable(void) {
    if (gki_cb.os.int_index == 0)
        OSAssert_Line(586, gki_cb.os.int_index == 0);

    --gki_cb.os.int_index;

    OSRestoreInterrupts(gki_cb.os.int_state[gki_cb.os.int_index]);
}

void GKI_disable(void) {
    BOOL intrStatus;

    // NOTE the ge compare vs. gt compare
    if (gki_cb.os.int_index >= MAX_INT_STATE)
        OSAssert_Line(611, gki_cb.os.int_index > MAX_INT_STATE);

    intrStatus = OSDisableInterrupts();

    gki_cb.os.int_state[gki_cb.os.int_index] = intrStatus;
    ++gki_cb.os.int_index;
}

void GKI_exception(UINT16 code, char* msg) {
    /* ... */
}

INT8* GKI_get_time_stamp(INT8* tbuf) {
    UINT32 ms_time;
    UINT32 s_time;
    UINT32 m_time;
    UINT32 h_time;
    INT8* p_out = tbuf;

    ms_time = gki_cb.com.OSTicks * 10;
    s_time = ms_time / 100;
    m_time = s_time / 60;
    h_time = m_time / 60;

    ms_time -= s_time * 100;
    s_time -= m_time * 60;
    m_time -= h_time * 60;

    *p_out++ = h_time / 10 + '0';
    *p_out++ = h_time % 10 + '0';
    *p_out++ = ':';
    *p_out++ = m_time / 10 + '0';
    *p_out++ = m_time % 10 + '0';
    *p_out++ = ':';
    *p_out++ = s_time / 10 + '0';
    *p_out++ = s_time % 10 + '0';
    *p_out++ = ':';
    *p_out++ = ms_time / 10 + '0';
    *p_out++ = ms_time % 10 + '0';
    *p_out++ = ':';
    *p_out = '\0';

    return tbuf;
}

void GKI_register_mempool(void* p_mem) {
    gki_cb.com.p_user_mempool = p_mem;
}

void* GKI_os_malloc(UINT32 size) {
    void* mem = NULL;

    mem = App_MEMalloc(size);

    if (mem == 0)
        return NULL;

    return mem;
}

void GKI_os_free(void* p_mem) {
    App_MEMfree(p_mem);
}
