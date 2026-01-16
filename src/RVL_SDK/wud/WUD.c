#include "revolution/wud.h"
#include "revolution/bte.h"
#include "revolution/os.h"
#include "revolution/os/OSAlarm.h"
#include "revolution/os/OSInterrupt.h"
#include "revolution/sc.h"
#include <cstring>
#include <mem.h>

static BOOL _initialized = FALSE;

SCBtDeviceInfoArray _scArray;
SCBtCmpDevInfoArray _spArray;
extern u8 _scFlush;

extern u8 __WUDHandlerStack[4096];

void* App_MEMalloc(u32 size) {
    WUD_DEBUGPrint("App_MEMalloc\n");
    return _wcb.alloc(size);
}

BOOL App_MEMfree(void* pBlock) {
    WUD_DEBUGPrint("App_MEMfree\n");
    return _wcb.free(pBlock);
}

void WUDiMoveBottomStdDevInfoPtr(WUDDeviceInfo* pInfo) {
    WUDControlBlock* block = &_wcb;
    s32 i;
    BOOL en = OSDisableInterrupts();

    for (i = 0; i < 10; i++) {
        if (!memcmp(block->stdList[i].info->bd_addr, pInfo->bd_addr, 6)) {
            if (memcmp(block->stdListTail->info->bd_addr, block->stdList[i].info->bd_addr, 6)) {
                ((DeviceList*)block->stdList[i].next)->prev = block->stdList[i].prev;
                if (!memcmp(block->stdListHead->info->bd_addr, block->stdList[i].info->bd_addr, 6)) {
                    block->stdListHead = (DeviceList*)block->stdList[i].next;
                } else {
                    ((DeviceList*)block->stdList[i].prev)->next = block->stdList[i].next;
                }

                block->stdList[i].prev = block->stdListTail;
                block->stdListTail->next = &block->stdList[i];
                block->stdListTail = &block->stdList[i];
                block->stdList[i].next = NULL;
            }
            break;
        }
    }

    OSRestoreInterrupts(en);
}

void WUDiMoveTopOfDisconnectedStdDevice(WUDDeviceInfo* pInfo) {
    WUDControlBlock* block = &_wcb;
    int i;
    BOOL en;
    DeviceList* list;

    en = OSDisableInterrupts();
    for (i = 0; i < 10; i++) {
        if (!memcmp(block->stdList[i].info->bd_addr, pInfo->bd_addr, 6)) {
            list = block->stdListHead;
            while (list != NULL) {
                if ((memcmp(list->info->bd_addr, pInfo->bd_addr, 6)) && (list->info->status == 1 || list->info->status == 0)) {
                    if (!memcmp(block->stdListHead->info->bd_addr, block->stdList[i].info->bd_addr, 6)) {
                        if (list == block->stdListHead->next) {
                            break;
                        }

                        block->stdListHead = (DeviceList*)block->stdList[i].next;
                    } else {
                        ((DeviceList*)block->stdList[i].prev)->next = block->stdList[i].next;
                    }

                    ((DeviceList*)block->stdList[i].next)->prev = block->stdList[i].prev;

                    if (list != block->stdListHead) {
                        block->stdList[i].prev = list->prev;
                        block->stdList[i].next = list;
                        ((DeviceList*)list->prev)->next = &block->stdList[i];
                        list->prev = &block->stdList[i];
                    } else {
                        block->stdList[i].prev = list;
                        block->stdList[i].next = list->next;
                        ((DeviceList*)list->next)->prev = &block->stdList[i];
                        list->next = &block->stdList[i];
                    }
                    break;
                }

                list = list->next;
            }
        }
    }

    OSRestoreInterrupts(en);
}

BOOL WUDIsBusy(void) {
    WUDControlBlock* block = &_wcb;
    BOOL en = OSDisableInterrupts();

    if (block->syncStatus == 0 && block->delStatus == 0 && block->profStatus == 4 && block->initStatus == 5) {
        OSRestoreInterrupts(en);
        return FALSE;
    }

    OSRestoreInterrupts(en);
    return TRUE;
}

void CleanupCallback(u8 error) {
    WUDControlBlock* block = &_wcb;
    if (error == 0) {
        _initialized = FALSE;
        block->status = 0;
    } else {
        WUD_DEBUGPrint("WARNING: USB_CLOSE_FAILURE!\n");
    }
}

void BTA_HhEnable(tBTA_SEC, tBTA_HH_CBACK*);

extern void WUDHidHostCallback(tBTA_HH_EVT, tBTA_HH*);

static u8 WUDiCheckRegisteredDeviceInfo();
static u8 WUDiClearUnregisteredDevice();
static u8 WUDiStackSetupComplete();

static void EnableStackHandler(OSAlarm* alarm, OSContext* context) {
    WUDControlBlock* block = &_wcb;

    switch (block->profStatus) {
    case 1:
        block->profStatus = WUDiCheckRegisteredDeviceInfo();
        break;
    case 2:
        block->profStatus = WUDiClearUnregisteredDevice();
        break;
    case 3:
        block->profStatus = WUDiStackSetupComplete();
        break;
    }
}

static void EnableStackHandler0(OSAlarm* alarm, OSContext* context) {
    OSSwitchFiberEx((u32)alarm, (u32)context, 0, 0, (u32)EnableStackHandler, (u32)(__WUDHandlerStack + sizeof(__WUDHandlerStack)));
}

void WUDiEnableStack() {
    WUDControlBlock* block = &_wcb;
    BTA_HhEnable(0x12, &WUDHidHostCallback);
    block->profStatus = 1;
    OSCreateAlarm(&block->evtAlarm);
    OSSetPeriodicAlarm(&block->evtAlarm, OSGetTime(), OSMillisecondsToTicks(10), &EnableStackHandler0);
}

void WUDiSetPinCode(u8*);
void BTA_CleanUp(tUSER_CBACK);
u8 WUDiGetLinkNum();

void WUDSecurityCallback(tBTA_DM_SEC_EVT event, tBTA_DM_SEC* pData) {
    WUDControlBlock* p_wcb = &_wcb;
    WUDDeviceInfo* p_info;
    tBTA_DM_ENABLE* p_enable;
    tBTA_DM_LINK_UP* p_linkup;
    tBTA_DM_LINK_DOWN* p_linkdown;
    tBTA_DM_PIN_REQ* p_pinreq;
    tBTA_DM_AUTH_CMPL* p_authcmpl;
    tBTA_DM_AUTHORIZE* p_authorize;
    tBTA_DM_SIG_STRENGTH* p_sigstr;
    tBTA_DM_BUSY_LEVEL* p_busylv;

    WUD_DEBUGPrint("WUDSecurityCallback: ");

    switch (event) {
    case 0:
        p_enable = (tBTA_DM_ENABLE*)pData;
        memcpy(p_wcb->hostAddr, p_enable->bd_addr, 6);

        WUD_DEBUGPrint("BTA_ENABLE_EVT\n");
        WUD_DEBUGPrint("host : %02x:%02x:%02x:%02x:%02x:%02x\n", p_wcb->hostAddr[0], p_wcb->hostAddr[1], p_wcb->hostAddr[2], p_wcb->hostAddr[3],
                       p_wcb->hostAddr[4], p_wcb->hostAddr[5]);

        WUDiEnableStack();
        p_wcb->status = 2;
        break;

    case 1:
        WUD_DEBUGPrint("BTA_DISABLE_EVT\n");
        BTA_CleanUp(&CleanupCallback);
        break;

    case 2:
        WUD_DEBUGPrint("BTA_DM_PIN_REQ_EVT\n");

        p_pinreq = (tBTA_DM_PIN_REQ*)pData;
        WUDiSetPinCode(p_pinreq->bd_addr);
        break;

    case 3:
        p_authcmpl = (tBTA_DM_AUTH_CMPL*)pData;

        WUD_DEBUGPrint("BTA_DM_AUTH_CMPL_EVT\n");
        WUD_DEBUGPrint("  addr : %02x:%02x:%02x:%02x:%02x:%02x\n", p_authcmpl->bd_addr[0], p_authcmpl->bd_addr[1], p_authcmpl->bd_addr[2],
                       p_authcmpl->bd_addr[3], p_authcmpl->bd_addr[4], p_authcmpl->bd_addr[5]);
        WUD_DEBUGPrint("  key  : %02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x\n", p_authcmpl->key[0],
                       p_authcmpl->key[1], p_authcmpl->key[2], p_authcmpl->key[3], p_authcmpl->key[4], p_authcmpl->key[5], p_authcmpl->key[6],
                       p_authcmpl->key[7], p_authcmpl->key[8], p_authcmpl->key[9], p_authcmpl->key[10], p_authcmpl->key[11], p_authcmpl->key[12],
                       p_authcmpl->key[13], p_authcmpl->key[14], p_authcmpl->key[15]);
        WUD_DEBUGPrint("  result = %d\n", p_authcmpl->success);

        if (p_authcmpl->success) {
            p_info = WUDiGetDevInfo(p_authcmpl->bd_addr);
            if (!memcmp(_work.bd_addr, p_authcmpl->bd_addr, 6)) {
                _work.status = 12;
                if (p_info == NULL) {
                    p_info = &_work;
                }
            }
            memcpy(p_info->link_key, p_authcmpl->key, 16);
        } else {
        }
        break;

    case 4:
        p_authorize = (tBTA_DM_AUTHORIZE*)pData;
        WUD_DEBUGPrint("BTA_DM_AUTHORIZE_EVT\n");
        break;
    case 5:
        p_linkup = (tBTA_DM_LINK_UP*)pData;
        WUD_DEBUGPrint("BTA_DM_LINK_UP_EVT\n");
        WUD_DEBUGPrint("   addr : %02x:%02x:%02x:%02x:%02x:%02x\n", p_linkup->bd_addr[0], p_linkup->bd_addr[1], p_linkup->bd_addr[2],
                       p_linkup->bd_addr[3], p_linkup->bd_addr[4], p_linkup->bd_addr[5]);

        p_info = WUDiGetDevInfo(p_linkup->bd_addr);
        if (p_info == NULL) {
            if (!memcmp(p_linkup->bd_addr, _work.bd_addr, 6)) {
                p_info = &_work;
            }
        }
        if ((p_info == NULL) || (WUDiGetLinkNum() == 4)) {
            btm_remove_acl(p_linkup->bd_addr);
            WUD_DEBUGPrint("%s --> %02x:%02x:%02x:%02x:%02x:%02x\n", (p_info == NULL) ? "not paired" : "4 links exist", p_linkup->bd_addr[0],
                           p_linkup->bd_addr[1], p_linkup->bd_addr[2], p_linkup->bd_addr[3], p_linkup->bd_addr[4], p_linkup->bd_addr[5]);
        } else {
            p_info->status = (u8)((p_info->status == 2) ? 12 : 3);
            p_wcb->linkNums++;
        }
        break;

    case 6:
        p_linkdown = (tBTA_DM_LINK_DOWN*)pData;
        WUD_DEBUGPrint("BTA_DM_LINK_DOWN_EVT\n");
        WUD_DEBUGPrint("   addr : %02x:%02x:%02x:%02x:%02x:%02x\n", p_linkdown->bd_addr[0], p_linkdown->bd_addr[1], p_linkdown->bd_addr[2],
                       p_linkdown->bd_addr[3], p_linkdown->bd_addr[4], p_linkdown->bd_addr[5]);
        WUD_DEBUGPrint("result: %d\n", p_linkdown->status);

        p_info = WUDiGetDevInfo(p_linkdown->bd_addr);
        if (p_info) {
            p_info->status = 1;
            p_wcb->linkNums--;
            if (!memcmp(_work.bd_addr, p_linkdown->bd_addr, 6)) {
                p_wcb->syncStatus = 255;
            }

            if (p_linkdown->status == 21) {
                u8 i;
                for (i = 0; i < 4; i++) {
                    if (!memcmp(_scArray.info[i + 10].bd_addr, p_linkdown->bd_addr, 6)) {
                        memset(&_scArray.info[i + 10], 0, sizeof(SCBtDeviceInfoSingle));
                        _scFlush = 1;
                    }
                }
            }
        } else if (!memcmp(_work.bd_addr, p_linkdown->bd_addr, 6)) {
            p_wcb->syncStatus = 255;
            p_wcb->linkNums--;
        } else {
            WUD_DEBUGPrint("this device in not paired\n");
        }

        if (p_wcb->linkNums <= 255 && p_wcb->linkNums >= 250) {
            OSReport("WARNING: link num count is reset.\n");
            p_wcb->linkNums = 0;
        }
        break;

    case 7:
        WUD_DEBUGPrint("BTA_DM_SIG_STRENGTH_EVT\n");
        p_sigstr = (tBTA_DM_SIG_STRENGTH*)pData;
        break;
    case 8:
        WUD_DEBUGPrint("BTA_DM_BUSY_LEVEL_EVT\n");
        p_busylv = (tBTA_DM_BUSY_LEVEL*)pData;
        break;
    default:
        break;
    }
}

u8* _WUDGetDevAddr(u8 handle) {
    u8* addr;
    BOOL en = OSDisableInterrupts();
    if (handle >= 0 && handle < 16) {
        addr = _dev_handle_to_bda[handle];
    } else {
        addr = NULL;
    }

    OSRestoreInterrupts(en);
    return addr;
}

u16 _WUDGetQueuedSize(s8 handle) {
    BOOL en;
    u16 n;

    en = OSDisableInterrupts();
    if (handle >= 0 && handle < 16) {
        n = _dev_handle_queue_size[handle];
    } else {
        n = 0;
    }

    OSRestoreInterrupts(en);
    return n;
}

u16 _WUDGetNotAckedSize(s8 handle) {
    u16 n;
    BOOL en = OSDisableInterrupts();
    if (handle >= 0 && handle < 16) {
        n = _dev_handle_notack_num[handle];
    } else {
        n = 0;
    }

    OSRestoreInterrupts(en);
    return n;
}

u8 _WUDGetLinkNumber(void) {
    BOOL en;
    WUDControlBlock* wcb = &_wcb;
    u8 num;
    en = OSDisableInterrupts();
    num = wcb->linkNums;
    OSRestoreInterrupts(en);
    return num;
}
