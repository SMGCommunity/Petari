#include "bte.h"

/* Original source:
 * bluedroid <android.googlesource.com/platform/external/bluetooth/bluedroid>
 * main/bte_logmsg.c
 */

/******************************************************************************
 *
 *  Copyright (C) 2001-2012 Broadcom Corporation
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

#include <cstdio>
// #include <stdarg.h>

#include "bt_trace.h"
#include "data_types.h"  // UINT32

#define IS_BTE
#include <context_rvl.h>

/*******************************************************************************
 * macros
 */

#define BTE_LOG_BUF_SIZE 2000

#ifndef BTE_LOG_BUF_SIZE
#define BTE_LOG_BUF_SIZE 1024
#endif

/*******************************************************************************
 * functions
 */

/*
void LogMsg(UINT32 trace_set_mask, char const* fmt_str, ...) {
    static char tmp[BTE_LOG_BUF_SIZE];

    va_list ap;

    va_start(ap, fmt_str);

    vsprintf(tmp, fmt_str, ap);
    sprintf(tmp, "%s\n", tmp);  // add newline

    OSReport(tmp);

    va_end(ap);

    (void)trace_set_mask;
}
    */

void LogMsg_0(UINT32 trace_set_mask, char const* fmt_str) {
    if (bte_target_mode == BTE_MODE_SERIAL_APP)
        LogMsg(trace_set_mask, fmt_str);
}

void LogMsg_1(UINT32 trace_set_mask, char const* fmt_str, UINT32 p1) {
    if (bte_target_mode == BTE_MODE_SERIAL_APP)
        LogMsg(trace_set_mask, fmt_str, p1);
}

void LogMsg_2(UINT32 trace_set_mask, char const* fmt_str, UINT32 p1, UINT32 p2) {
    if (bte_target_mode == BTE_MODE_SERIAL_APP)
        LogMsg(trace_set_mask, fmt_str, p1, p2);
}

void LogMsg_3(UINT32 trace_set_mask, char const* fmt_str, UINT32 p1, UINT32 p2, UINT32 p3) {
    if (bte_target_mode == BTE_MODE_SERIAL_APP)
        LogMsg(trace_set_mask, fmt_str, p1, p2, p3);
}

void LogMsg_4(UINT32 trace_set_mask, char const* fmt_str, UINT32 p1, UINT32 p2, UINT32 p3, UINT32 p4) {
    if (bte_target_mode == BTE_MODE_SERIAL_APP)
        LogMsg(trace_set_mask, fmt_str, p1, p2, p3, p4);
}

void LogMsg_5(UINT32 trace_set_mask, char const* fmt_str, UINT32 p1, UINT32 p2, UINT32 p3, UINT32 p4, UINT32 p5) {
    if (bte_target_mode == BTE_MODE_SERIAL_APP)
        LogMsg(trace_set_mask, fmt_str, p1, p2, p3, p4, p5);
}

void LogMsg_6(UINT32 trace_set_mask, char const* fmt_str, UINT32 p1, UINT32 p2, UINT32 p3, UINT32 p4, UINT32 p5, UINT32 p6) {
    if (bte_target_mode == BTE_MODE_SERIAL_APP)
        LogMsg(trace_set_mask, fmt_str, p1, p2, p3, p4, p5, p6);
}
