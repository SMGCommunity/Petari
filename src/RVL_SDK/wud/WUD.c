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
