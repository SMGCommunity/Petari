#include "revolution.h"
#include "revolution/dvd.h"
#include "revolution/esp.h"
#include "revolution/nand.h"
#include "revolution/os.h"
#include "revolution/os/OSExec.h"
#include "revolution/os/OSNandbootinfo.h"
#include "revolution/os/OSPlayTime.h"
#include "revolution/os/OSReset.h"
#include "revolution/os/OSStateFlags.h"
#include "revolution/os/OSStateTM.h"
#include "revolution/os/OSTitle.h"
#include "revolution/sc.h"
#include "revolution/vi.h"
#include <cstdio>
#include <mem.h>

#define TitleIdLo(t) ((u32)(((u64)t)&0xffffffff))
#define TitleIdHi(t) ((u32)((((u64)t)>>32)&0xffffffff))

extern BOOL __OSInIPL;
extern BOOL __OSInNandBoot;

u32 OSLaunchNoReturnFlag;

static BOOL PackArgs(void* addr, s32 argc, char* argv[]) {
    s32 numArgs;
    char* bootInfo2;
    char* ptr;
    char** list;
    u32 i;

    bootInfo2 = (char*)addr;
    memset(bootInfo2, 0, 0x2000);

    if (argc == 0) {
        *(u32*)&bootInfo2[8] = 0;
    }
    else {
        numArgs = argc;
        ptr = bootInfo2 + 0x2000;
        while (--argc >= 0) {
            ptr -= (strlen(argv[argc]) + 1);
            strcpy(ptr, argv[argc]);
            argv[argc] = (char*)(ptr - bootInfo2);
        }

        ptr = bootInfo2 + ( (ptr - bootInfo2) & ~3 );
        ptr -= sizeof(char**) * (numArgs + 1);
        list = (char**)ptr;
        for(i = 0; i < numArgs + 1; i++) {
            list[i] = argv[i];
        }
        
        ptr -= sizeof(s32);
        *(s32*)ptr = numArgs;
        *(u32*)&bootInfo2[8] = (u32)(ptr - bootInfo2);
    }

    return TRUE;
}

BOOL __OSCheckTmdCountryCode(ESTmdView* tmd) {
    u32 TmdCountryCode = 0;
    TmdCountryCode = *((u32*)tmd->head.reserved);

    if (TmdCountryCode == 3) {
        return TRUE;
    }
    else {
        return (TmdCountryCode == SCGetProductGameRegion()) ? TRUE : FALSE;
    }

    return FALSE;
}

static BOOL __OSCheckTmdDriveSpinFlag(ESTmdView* tmd) {
    u8 TmdDriveSpinFlag = 0;
    TmdDriveSpinFlag = *(u8*)((u32)tmd->head.reserved + 24);

    if(TmdDriveSpinFlag & 1) {
        return TRUE;
    }
    else {
        return FALSE;
    }

    return FALSE;
}

BOOL __OSCheckCompanyCode(ESTitleId titleId, BOOL diskApp) {
    NANDStatus status;
    char home[65];
    char data[65];
    u16 currentGid, launchedGid;
    s32 rv = 0;

    rv = NANDGetHomeDir(home);

    if (rv != 0) {
        return FALSE;
    }

    rv = NANDGetStatus(home, &status);
    if (rv != 0) {
        return FALSE;
    }

    currentGid = status.groupId;

    if (currentGid == 2) {
        if (OSGetAppType() == 0x80) {
            if ((OSGetConsoleType() & 0xF0000000) == 0x10000000) {
                DVDDiskID* diskId = DVDGetCurrentDiskID();
                currentGid = *(u16*)diskId->company;
            }
        }
    }
    
    if (diskApp) {
        DVDDiskID* diskId = DVDGetCurrentDiskID();
        launchedGid = *(u16*)diskId->company;
    }
    else {
        sprintf(data, "/title/%08x/%08x/data", TitleIdHi(titleId), TitleIdLo(titleId));
        rv = NANDGetStatus(data, &status);
        if (rv != 0) {
            return FALSE;
        }

        launchedGid = status.groupId;
    }

    return (currentGid == launchedGid) ? TRUE : FALSE;
}

BOOL __OSCheckTmdSysVersion(ESTmdView* tmd) {
    ESSysVersion systemId = 0x100000003;
    char syspath[64 + 1] __attribute__ ((aligned (32)));
    s32 rv = 0;
    u32 numTicket = 0, numBlock = 0, numInode = 0;
    systemId = tmd->head.sysVersion;
    rv = ESP_GetTicketViews(systemId, NULL, &numTicket);
    if (rv != 0 || numTicket != 1) {
        return FALSE;
    }

    sprintf(syspath,  "/title/%08x/%08x/content", TitleIdHi(systemId), TitleIdLo(systemId));
    rv = NANDSecretGetUsage(syspath, &numBlock, &numInode);
    if (rv != 0 || numInode <= 2) {
        return FALSE;
    }

    return TRUE;
}

s32 __OSGetValidTicketIndex(ESTicketView *ticketViewList, u32 numTickets) {
    s32 rv = 0;
    s32 i = 0;
    s32 bestIdx = 0;
    BOOL permanent = FALSE;
    u32 maxBit = 0, maxTime = 0;

    if (ticketViewList == NULL) {
        OSReport("NULL pointer detected: line %d in %s\n", 0x183, __FILE__);
        return -1017;
    }

    for (i = 0; i < numTickets; i++) {
        u32 playTime;
        u16 accessMask;
        __OSPlayTimeType type;
        rv = __OSGetPlayTime(&ticketViewList[i], &type, &playTime);
        if (rv > 0) {
            rv = 0;
            continue;
        }
        else if (rv != 0) {
            return rv;
        }

        switch (type) {
            case OSPLAYTIME_PERMANENT:
            {
                u32 j;
                u32 bits = 0;
                if (!permanent) {
                    bestIdx = i;
                    permanent = TRUE;
                }

                accessMask = (u16)((ticketViewList[i].sysAccessMask[1] << 8) | ticketViewList[i].sysAccessMask[0]);

                for (j = 0; j < 16; j++) {
                    if (accessMask & (1 << j)) bits++;
                }

                if (bits > maxBit) {
                    bestIdx = i;
                    maxBit = bits;
                }
            }
            break;
        case OSPLAYTIME_LIMITED:
            if (!permanent) {
                if (playTime > maxTime) {
                    bestIdx = i;
                    maxTime = playTime;
                }
            }
            break;
        case OSPLAYTIME_OTHER:
            break;
        default:
            break;
        }
    }

    if (!permanent && maxTime == 0) {
        return -1;
    }

    return bestIdx;
}

void __OSRelaunchTitle(u32 resetCode) {
    s32 rc = 0;
    u32 ticketCnt = 1;
    ESTicketView* tik = NULL;
    ESTitleId titleId __attribute__ ((aligned (32)));
    __OSPlayTimeType type = OSPLAYTIME_PERMANENT;
    u32 remain = 0;
    u8* bi2 = NULL;
    OSNandbootInfo* info = NULL;
    OSStateFlags state;

    OSSetArenaLo((void*)0x81280000);
    OSSetArenaHi((void*)0x812F0000);

    rc = ESP_InitLib();
    if (rc != 0) {
        __OSReturnToMenuForError();
    }

    rc = ESP_GetTitleId(&titleId);
    if (rc != 0) {
        __OSReturnToMenuForError();
    }

    tik = (ESTicketView*)(OSAllocFromMEM1ArenaLo)(OSRoundUp32B(sizeof(ESTicketView)), 32);
    if (tik == NULL) {
        __OSReturnToMenuForError();
    }

    memset(tik, 0, OSRoundUp32B(sizeof(ESTicketView)));
    rc = ESP_DiGetTicketView(NULL, tik);
    if (rc == -1017) {
        ESTicketView* tmp = NULL;
        rc = ESP_GetTicketViews(titleId, NULL, &ticketCnt);
        if (rc != 0) {
            __OSReturnToMenuForError();
        }

        tmp = (ESTicketView*)OSAllocFromMEM1ArenaLo(OSRoundUp32B(ticketCnt * sizeof(ESTicketView)), 32);
        if (tmp == NULL) {
            __OSReturnToMenuForError();
        }

        rc = ESP_GetTicketViews(titleId, tmp, &ticketCnt);
        if (rc != 0) {
            __OSReturnToMenuForError();
        }

        memcpy(tik, tmp, sizeof(ESTicketView));
    }
    else if (rc != 0) {
        __OSReturnToMenuForError();
    }
    else {
        if (OSPlayTimeIsLimited()) {
            __OSPlayTimeType type = OSPLAYTIME_PERMANENT;
            u32 remain = 0xFFFFFFFF;
            __OSGetPlayTime(tik, &type, &remain);
            if (remain == 0) {
                __OSWriteExpiredFlag();
                __OSReturnToMenuForError();
            }
        }
    }

    bi2 = (u8*)OSAllocFromMEM1ArenaLo(0x2000, 64);
    info = (OSNandbootInfo*)((u32)bi2 + (0x2000 - sizeof(OSNandbootInfo)));
    memset(bi2, 0, 0x2000);
    info->LastTitleId = titleId;
    info->LastAppType = OSGetAppType();
    info->ReturnType = 1;
    info->ArgValue = resetCode | 0x80000000;

    __OSCreateNandbootInfo();
    __OSWriteNandbootInfo(info);
    __OSReadStateFlags(&state);
    state.lastShutdown = 3;
    __OSWriteStateFlags(&state);
    rc = ESP_LaunchTitle(titleId, tik);

    if (rc != 0) {
        __OSReturnToMenuForError();
    }

    while (1) {

    }
}

void __OSLaunchTitle(OSTitleId titleId) {
    s32 rc;
    ESTicketView* tik;
    ESTicketView* list;
    u32 ticketCnt = 1;

    rc = ESP_InitLib();
    if (rc != 0) {
        return;
    }

    tik = (ESTicketView*)OSAllocFromMEM1ArenaLo(OSRoundUp32B(sizeof(ESTicketView)), 32);

    if (tik == NULL) {
        return;
    }

    memset(tik, 0, OSRoundUp32B(sizeof(ESTicketView)));
    rc = ESP_GetTicketViews(titleId, NULL, &ticketCnt);
    if (rc != 0) {
        return;
    }

    list = (ESTicketView*)OSAllocFromMEM1ArenaLo(OSRoundUp32B(sizeof(ESTicketView) * ticketCnt), 32);
    if (list == NULL) {
        return;
    }

    rc = ESP_GetTicketViews(titleId, list, &ticketCnt);
    if (rc != 0) {
        return;
    }

    rc = __OSGetValidTicketIndex(list, ticketCnt);
    if (rc < 0) {
        return;
    }

    memcpy(tik, &list[rc], sizeof(ESTicketView));
    rc = ESP_LaunchTitle(titleId, tik);
    if (rc != 0) {
        return;
    }

    while (1) {

    }
}

void LaunchCommon(OSTitleId titleId, u32 LaunchCode, char* const argv[], BOOL launchCheck) {
    OSStateFlags state;
    OSNandbootInfo *info;
    u8* bi2;
    s32 argvlen;
    char titleIdString[20];
    char** argvToPass;
    s32 i;
    ESTitleId title __attribute__ ((aligned (32)));
    ESTmdView* tmd = NULL;
    u32 tmdSize = 0;
    s32 rv = 0;

    rv = ESP_InitLib();
    if(rv != 0) {
        __OSReturnToMenuForError();
    }

    rv = ESP_GetTmdView(titleId, NULL, &tmdSize);
    if(rv != 0) {
        __OSReturnToMenuForError();
    }

    tmd = (ESTmdView*)OSAllocFromMEM1ArenaLo(OSRoundUp32B(tmdSize), 64);
    if(tmd == NULL) {
        __OSReturnToMenuForError();
    }

    rv = ESP_GetTmdView(titleId, tmd, &tmdSize);
    if(rv != 0) {
        __OSReturnToMenuForError();
    }

    if(!__OSCheckTmdSysVersion(tmd)) {
        OSReport("OSLaunchTitle(): Firmware is not installed\n");
        __OSReturnToMenuForError();
    }
    
    if (launchCheck) {
        if (!__OSCheckCompanyCode(titleId, FALSE)) {
            OSReport("OSLaunchTitle(): Company code is not correct\n");
            __OSReturnToMenuForError();
        }

        if (!__OSCheckTmdCountryCode(tmd)) {
            OSReport("OSLaunchTitle(): Country code is not correct\n");
            __OSReturnToMenuForError();
        }
    }

    if(!__OSCheckTmdDriveSpinFlag(tmd)) {
        if(__DVDGetCoverStatus() == 2) {
            __DVDResetWithNoSpinup();
        }
    }

    if (argv || LaunchCode) {
        bi2 = (u8*)OSAllocFromMEM1ArenaLo(0x2000, 64);
        info = (OSNandbootInfo*)((u32)bi2 + (0x2000 - sizeof(OSNandbootInfo)));

        sprintf(titleIdString, "%016llx", titleId);
        argvlen = 0;

        if (argv) {
            while (argv[argvlen])
            argvlen++;
        }

        argvlen += 1;
        argvToPass = OSAllocFromMEM1ArenaLo((argvlen+1)*sizeof(char*), 1);
        argvToPass[0] = titleIdString;

        for (i = 1; i < argvlen; i++) {
            argvToPass[i] = argv[i - 1];
        }
        
        PackArgs(bi2, argvlen, argvToPass);

        if(__OSInIPL || OSLaunchNoReturnFlag) {
            title = 0x100000002;
        }
        else {
            rv = ESP_InitLib();
            if(rv != 0) {
                OSReport("\nOSExec(): Failed to exec %d in %d\n", rv, 0x30F);
                __OSHotResetForError();
            }

            rv = ESP_GetTitleId(&title);
            if(rv != 0) {
                OSReport("\nOSExec(): Failed to exec %d in %d\n", rv, 0x317);
                __OSHotResetForError();
            }
        }

        info->LastTitleId = title;
        info->ArgOffset = *(u32*)&bi2[8];
        info->LastAppType = OSGetAppType();
        if(titleId == 0x100000002) {
            info->ReturnType = 4;
        }
        else {
            info->ReturnType = 2;
        }
        info->ArgValue = LaunchCode;
        __OSCreateNandbootInfo();
        __OSWriteNandbootInfo(info);
        __OSReadStateFlags(&state);
        state.lastShutdown  = 3;
        if(titleId == 0x100000002) {
            state.lastDiscState = __OSGetDiscState(state.lastDiscState);
            state.menuMode = 2;
        }
        __OSWriteStateFlags(&state);
    }
    
    OSDisableScheduler();
    __OSShutdownDevices(6);
    OSEnableScheduler();
    __OSLaunchTitle(titleId);
    __OSLaunchMenu();
}

void __OSReturnToMenul(u32 LaunchCode, const char *arg0, ...) {
    va_list vl;
    char* ptr;
    s32 i;
    char** argv;
    __OSUnRegisterStateEvent();
    OSSetArenaLo((void*)0x81280000);
    OSSetArenaHi((void*)0x812F0000);
    argv = OSAllocFromMEM1ArenaLo(sizeof(char*), 0x1000);
    va_start(vl, arg0);
    i = 0;
    ptr = (char*)arg0;
    while ((argv[i++] = ptr) != 0) {
        ptr = va_arg(vl, char*);
    }

    va_end(vl);

    LaunchCommon(0x100000002, LaunchCode, argv, FALSE);
    __OSReturnToMenuForError();
}
