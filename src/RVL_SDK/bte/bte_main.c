#include "bte.h"

/* Original source:
 * bluedroid <android.googlesource.com/platform/external/bluetooth/bluedroid>
 * main/bte_main.c
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

#include <stddef.h>  // NULL

#include <macros.h>  // ATTR_ALIGN

#include "data_types.h"

#include "bta_api.h"
#include "btu.h"
#include "gki.h"
#include "hci.h"
#include "uusb.h"  // wait4hci

#define IS_BTE
#include <context_rvl.h>

/*******************************************************************************
 * local function declarations
 */

static void BTE_InitSys(void);
static void bta_register_os_callbacks(void);

static void BTUInterruptHandler(OSAlarm* alarm, OSContext* context);
static void BTE_BringUpStack(void);

/*******************************************************************************
 * variables
 */

// .bss
static OSAlarm _bte_alarm;
unsigned char __BTUInterruptHandlerStack[0x1000] ATTR_ALIGN(32);

// .sdata
static tHCI_CFG bte_hcisu_h2_cfg = {2652, 8449};

// .sbss
static tBTA_APP_INFO_CBACK* _bte_app_info;
#if !defined(NDEBUG)
static BOOLEAN enable_dbg_msg;
#endif
UINT8 volatile bte_target_mode;

/*******************************************************************************
 * functions
 */

static void BTE_InitSys(void) {
    _bte_app_info = NULL;
}

static void bta_register_os_callbacks(void) {
    OSCreateAlarm(&_bte_alarm);

    OSSetPeriodicAlarm(&_bte_alarm, OSGetTime(), OSMillisecondsToTicks(2), &BTUInterruptHandler);
}

#if !defined(NDEBUG)
void __btu_interrupt_handler_debug_msg(BOOLEAN enable) {
    enable_dbg_msg = enable;
}
#endif

static void BTUInterruptHandler(OSAlarm* alarm, OSContext* context) {
#if !defined(NDEBUG)
    OSTime start;
    OSTime end;
    OSTick elapsed;

    start = __OSGetSystemTime();
#endif

    OSSwitchFiber((u32)&btu_task_msg_handler, (u32)__BTUInterruptHandlerStack + sizeof __BTUInterruptHandlerStack);

#if !defined(NDEBUG)
    end = __OSGetSystemTime();

    if (enable_dbg_msg) {
        elapsed = OSTicksToMicroseconds(OSDiffTick(end, start));
        if (elapsed > 100) {
            OSReport("BTUInterruptHandler: Warning BTU consumes %d microseconds\n", elapsed);
        }
    }
#endif
}

static void BTE_BringUpStack(void) {
    p_hcisu_if = (tHCI_IF*)&hcisu_h2;
    p_hcisu_cfg = &bte_hcisu_h2_cfg;

    bte_hcisu_task(0);

    while (wait4hci)
        GKI_delay(100);

    btu_task_init();
    bta_register_os_callbacks();
}

tBTA_STATUS BTA_Init(void) {
    BTE_InitSys();

    GKI_init();
    GKI_enable();

    BTE_Init();
    BTE_BringUpStack();

    GKI_run(NULL);

    while (!BTA_DmIsDeviceUp())
        GKI_delay(2000);

    return 0;
}

void BTA_CleanUp(tBTA_APP_INFO_CBACK* p_cb) {
    _bte_app_info = p_cb;

    OSCancelAlarm(&_bte_alarm);
    bte_hcisu_close();

    GKI_shutdown();
}

void bta_usb_close_evt(INT8 param_1) {
    if (_bte_app_info) {
        if (param_1 >= 0)
            (*_bte_app_info)(0);
        else
            (*_bte_app_info)(1);
    }
}
