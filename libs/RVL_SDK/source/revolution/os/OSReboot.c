#include "revolution/os.h"

extern OSExecParams __OSRebootParams;
extern u32 __OSNextPartitionType;
vu32 __OSLaunchPartitionType : 0x80003194;
static void* SaveStart = NULL;
static void* SaveEnd = NULL;

void __OSReboot(u32 resetCode, u32 bootDol) {
    char* argvToPass;
    OSDisableInterrupts();
    OSSetArenaLo((void*)0x81280000);
    OSSetArenaHi((void*)0x812F0000);
    argvToPass = NULL;
    __OSNextPartitionType = __OSLaunchPartitionType;
    __OSBootDol(bootDol, resetCode | 0x80000000, &argvToPass);
}

void OSGetSaveRegion(void **start, void **end) {
    *start = SaveStart;
    *end = SaveEnd;
}