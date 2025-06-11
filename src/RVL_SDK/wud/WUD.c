#include "revolution/wud.h"
#include "revolution/os/OSInterrupt.h"
#include <cstring>

void WUDiMoveBottomStdDevInfoPtr(WUDDeviceInfo *pInfo) {
    WUDControlBlock* block = &_wcb;
    s32 i;
    BOOL en = OSDisableInterrupts();

    for (i = 0; i < 10; i++) {
        if (!memcmp(block->stdList[i].info->bd_addr, pInfo->bd_addr, 6)) {
            if (memcmp(block->stdListTail->info->bd_addr, block->stdList[i].info->bd_addr, 6)) {
                ((DeviceList*)block->stdList[i].next)->prev = block->stdList[i].prev;
                if (!memcmp(block->stdListHead->info->bd_addr, block->stdList[i].info->bd_addr, 6)) {
                    block->stdListHead = (DeviceList*)block->stdList[i].next;
                }
                else {
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

void WUDiMoveTopOfDisconnectedStdDevice(WUDDeviceInfo *pInfo) {
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
                    }
                    else {
                        ((DeviceList*)block->stdList[i].prev)->next = block->stdList[i].next;
                    }

                    ((DeviceList*)block->stdList[i].next)->prev = block->stdList[i].prev;

                    if (list != block->stdListHead) {
                        block->stdList[i].prev = list->prev;
                        block->stdList[i].next = list;
                        ((DeviceList*)list->prev)->next = &block->stdList[i];
                        list->prev = &block->stdList[i];
                    }
                    else {
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

u8* _WUDGetDevAddr(u8 handle) {
    u8* addr;
    BOOL en = OSDisableInterrupts();
    if (handle >= 0 && handle < 16) {
        addr = _dev_handle_to_bda[handle];
    }
    else {
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
    }
    else {
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
    }
    else {
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
