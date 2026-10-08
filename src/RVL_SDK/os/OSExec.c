#include <cstdio>
#include "private/flipper.h"
#include "revolution/dvd.h"
#include "revolution/nand.h"
#include "revolution/os.h"
#include "revolution/os/OSExecParams.h"
#include "revolution/os/OSNandbootinfo.h"
#include "revolution/os/OSTIme.h"
#include "revolution/os/OSTitle.h"
#include "revolution/types.h"
#include <wstring.h>

static volatile BOOL Prepared;
extern BOOL __OSIsGcam;
extern BOOL __OSInIPL;
extern BOOL __OSInNandBoot;

extern BOOL __OSIsDiag;

BOOL __OSInReboot;
static volatile u32 DVDLowIntType;
static DVDDiskID id ATTRIBUTE_ALIGN(32);
u32 __OSNextPartitionType = 0;

OSExecParams* __OSExecParamsAddr : OS_BASE_CACHED + 0x30F0;
s32 __OSCurrentTGCOffset : OS_BASE_CACHED + 0x30F4;
vu32 __OSLaunchPartitionType : (OS_BASE_CACHED | 0x3194);
vu8 __OSLockedFlag : (OS_BASE_CACHED | 0x3187);

#if (VERSION == RMGJ01 || VERSION == RMGE01)
#define TitleIdLo(t) ((u32)(((u64)t)&0xffffffff))
#define TitleIdHi(t) ((u32)((((u64)t)>>32)&0xffffffff))
#endif

extern void __DVDShowFatalMessageForSystem(u32 command, u32 intType);

extern u16 OSSetFontEncode(u16);

static void __ExecShowFatalMessage(char* string, s32 rv) {
    GXColor bg = {0, 0, 0, 0};
    GXColor fg = {255, 255, 255, 0};

    char diag[256];

    OSSetFontEncode(1);
    snprintf(diag, 256,
             "Error occurred.\n"
             "%s : %d\n",
             string, rv);
    OSFatal(fg, bg, diag);
}

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
    } else {
        numArgs = argc;
        ptr = bootInfo2 + 0x2000;
        while (--argc >= 0) {
            ptr -= (strlen(argv[argc]) + 1);
            strcpy(ptr, argv[argc]);
            argv[argc] = (char*)(ptr - bootInfo2);
        }

        list = (char**)(bootInfo2 + ((ptr - bootInfo2) & ~3));
        list -= numArgs + 1;

        for (i = 0; i < numArgs + 1; i++) {
            list[i] = argv[i];
        }

        *(s32*)(list - 1) = numArgs;
        *(u32*)&bootInfo2[8] = (u32)((char*)(list - 1) - bootInfo2);
    }

    return TRUE;
}

static BOOL Utf16ToArg(char* dstArg, u16* srcName) {
    char *srcPtr, *dstPtr;
    u8 i, mask, shift;

    if (srcName != 0) {
        srcPtr = (char*)srcName;
        dstPtr = dstArg;

        while (*srcPtr || *(srcPtr + 1)) {
            for (i = 0; i < 4; i++) {
                mask = (u8)((i & 0x1) ? 0xF : 0xF0);
                shift = (u8)((i & 0x1) ? 0 : 4);

                if (0 <= ((*srcPtr & mask) >> shift) && ((*srcPtr & mask) >> shift) < 0xA) {
                    *dstPtr = (char)(((*srcPtr & mask) >> shift) + 0x30);
                } else if (0xA <= ((*srcPtr & mask) >> shift) && ((*srcPtr & mask) >> shift) < 0x10) {
                    *dstPtr = (char)(((*srcPtr & mask) >> shift) + 0x57);
                } else {
                    return FALSE;
                }

                dstPtr++;

                if (i & 1) {
                    srcPtr++;
                }
            }
        }

        *dstPtr++ = 0;
        return TRUE;
    }

    return FALSE;
}

static BOOL PackInstallerArgs(void* addr, s32 argc, char* argv[]) {
    s32 numArgs;
    char* bootInfo2;
    char* ptr;
    char** list;
    u32 i;

    bootInfo2 = (char*)addr;
    memset(bootInfo2, 0, 0x2000);

    if (argc == 0) {
        *(u32*)&bootInfo2[8] = 0;
    } else {
        numArgs = argc;
        ptr = bootInfo2 + 0x2000;

        while (--argc >= 0) {
            if (argc < 2 || (argc % 2)) {
                char* val = argv[argc];
                ptr -= (strlen(argv[argc]) + 1);
                strcpy(ptr, val);
                argv[argc] = (char*)(ptr - bootInfo2);
            } else {
                u16* val = (u16*)argv[argc];
                ptr -= (wcslen(val) * 4 + 1);
                Utf16ToArg(ptr, val);
                argv[argc] = (char*)(ptr - bootInfo2);
            }
        }

        list = (char**)(bootInfo2 + ((ptr - bootInfo2) & ~3));
        list -= numArgs + 1;

        for (i = 0; i < numArgs + 1; i++) {
            list[i] = argv[i];
        }

        *(s32*)(list - 1) = numArgs;

        *(u32*)&bootInfo2[8] = (u32)((char*)(list - 1) - bootInfo2);
    }

    return TRUE;
}

// clang-format off
static asm void Run(register void* entryPoint) {
    fralloc
    bl      ICFlashInvalidate
    sync
    isync
    mtctr    entryPoint
    bctr
    frfree
    blr
}
// clang-format on

static void Callback(s32 result, DVDCommandBlock* block) {
    Prepared = TRUE;
}

void __OSGetExecParams(OSExecParams* params) {
    if (0x80000000 <= (u32)__OSExecParamsAddr) {
        memcpy(params, __OSExecParamsAddr, 0x1C);
    } else {
        params->valid = FALSE;
    }
}

void __OSSetExecParams(const OSExecParams* params, OSExecParams* addr) {
    memcpy(addr, params, sizeof(OSExecParams));
    __OSExecParamsAddr = addr;
}

extern void __OSInitIPCBuffer(void);
extern void IPCReInit(void);
extern IOSError IPCCltReInit(void);

static void callback(u32 intType) {
    DVDLowIntType = intType;
}

void __OSLaunchNextFirmware(void) {
    u8 i;
    s32 rc = -1;
    u32 ticketCnt = 1;
    ESTicketView *t, *t2;
    ESSysVersion version = 0x0000000100000003;
    u32 simGddr3Size;
    u32 gddrProtAddr;

    DVDGameTOC* gameToc;
    DVDPartitionInfo* partInfo;
    DVDPartitionInfo* bootGameInfo;
    ESTitleMeta* tmd;
    u32 numTmdBytes = 0;
    DVDPartitionInfo* partInfoPtr;

    gameToc = (DVDGameTOC*)OSAllocFromMEM1ArenaLo(OSRoundUp32B(sizeof(DVDGameTOC)), 32);
    partInfo = (DVDPartitionInfo*)OSAllocFromMEM1ArenaLo(OSRoundUp32B(sizeof(DVDPartitionInfo) * 256), 32);
    tmd = (ESTitleMeta*)OSAllocFromMEM1ArenaLo(OSRoundUp32B(sizeof(ESTitleMeta)), 64);
    t2 = (ESTicketView*)OSAllocFromMEM1ArenaLo(OSRoundUp32B(sizeof(ESTicketView)), 32);

    if (__OSNextPartitionType == __OSLaunchPartitionType && *(u32*)OSPhysicalToCached(0x3198)) {
        rc = ESP_InitLib();
        if (rc == 0) {
            rc = ESP_DiGetTicketView(NULL, t2);
        }
        if (rc == 0) {
            rc = ESP_DiGetTmd(NULL, &numTmdBytes);
        }
        if (rc == 0) {
            rc = ESP_DiGetTmd(tmd, &numTmdBytes);
        }
        ESP_CloseLib();

        if (OSPlayTimeIsLimited()) {
            __OSPlayTimeType type = OSPLAYTIME_PERMANENT;
            u32 remain = 0xffffffff;

            __OSGetPlayTime(t2, &type, &remain);

            if (remain == 0) {
                __OSWriteExpiredFlag();
                __OSReturnToMenuForError();
            }
        }
    }

    if (rc == 0) {
        bootGameInfo = partInfo;
        bootGameInfo->type = *((u32*)OSPhysicalToCached(0x3194));
        bootGameInfo->gamePartition = (DVDGamePartition*)*((u32*)OSPhysicalToCached(0x3198));
    } else {
        DVDLowIntType = 0;
        DVDLowClosePartition(callback);

        while (!DVDLowIntType) {
        }

        if (DVDLowIntType != 1) {
            OSReport("\nOSExec(): Failed to exec %d in %d\n", DVDLowIntType, 0x395);
            __OSReturnToMenuForError();
        }

        DVDLowIntType = 0;
        DVDLowUnencryptedRead(gameToc, OSRoundUp32B(sizeof(DVDGameTOC)), 0x40000 >> 2, callback);

        while (!DVDLowIntType) {
        }

        if (DVDLowIntType != 1) {
            OSReport("\nOSExec(): Failed to exec %d in %d\n", DVDLowIntType, 0x39F);
            __OSReturnToMenuForError();
        }

        DVDLowIntType = 0;
        DVDLowUnencryptedRead(partInfo, OSRoundUp32B(sizeof(DVDPartitionInfo) * 256), (u32)gameToc->partitionInfos, callback);

        while (!DVDLowIntType) {
        }

        if (DVDLowIntType != 1) {
            OSReport("\nOSExec(): Failed to exec %d in %d\n", DVDLowIntType, 0x3AB);
            __OSReturnToMenuForError();
        }

        bootGameInfo = NULL;
        partInfoPtr = partInfo;
        for (i = 0; i < gameToc->numGamePartitions; i++) {
            if (partInfoPtr->type == __OSNextPartitionType) {
                bootGameInfo = partInfoPtr;
            }

            partInfoPtr++;
        }

        if (!bootGameInfo) {
            gameToc++;
            partInfoPtr = partInfo + 4;
            for (i = 0; i < gameToc->numGamePartitions; i++) {
                if (partInfoPtr->type == __OSNextPartitionType) {
                    bootGameInfo = partInfoPtr;
                }

                partInfoPtr++;
            }
        }

        if (!bootGameInfo) {
            OSReport("\nOSExec(): The specified game doesn't exist in the disc\n");
            __OSReturnToMenuForError();
        }

        *((u32*)OSPhysicalToCached(0x3194)) = bootGameInfo->type;
        *((u32*)OSPhysicalToCached(0x3198)) = (u32)bootGameInfo->gamePartition;

        DVDLowIntType = 0;
        if (__OSLockedFlag == 0x80) {
            DVDLowOpenPartitionWithTmdAndTicketView((u32)bootGameInfo->gamePartition, t2, numTmdBytes, tmd, 0, NULL, callback);
        } else {
            DVDLowOpenPartition((u32)bootGameInfo->gamePartition, NULL, 0, 0, tmd, callback);
        }

        while (!DVDLowIntType) {
        }

        if (DVDLowIntType != 1) {
            OSReport("\nOSExec(): Failed to exec %d in %d\n", DVDLowIntType, 0x3E8);
            __OSReturnToMenuForError();
        }

        DVDLowIntType = 0;
        DVDLowClosePartition(callback);

        while (!DVDLowIntType) {
        }

        if (DVDLowIntType != 1) {
            OSReport("\nOSExec(): Failed to exec %d in %d\n", DVDLowIntType, 0x3F2);
            __OSReturnToMenuForError();
        }
    }

    version = tmd->head.sysVersion;

    rc = ESP_InitLib();
    if (rc != 0) {
        OSReport("\nOSExec(): Failed to exec %d in %d\n", rc, 0x3FD);
        __OSHotResetForError();
    }

    rc = ESP_GetTicketViews(version, NULL, &ticketCnt);
    if (ticketCnt != 1 || rc != 0) {
        OSReport("\nOSExec(): Failed to exec %d in %d\n", rc, 0x406);
        __OSHotResetForError();
    }

    t = (ESTicketView*)OSAllocFromMEM1ArenaLo(OSRoundUp32B(sizeof(ESTicketView) * ticketCnt), 32);
    rc = ESP_GetTicketViews(version, t, &ticketCnt);
    if (rc != 0) {
        OSReport("\nOSExec(): Failed to exec %d in %d\n", rc, 0x40F);
        __OSHotResetForError();
    }

    DVDLowFinalize();

    simGddr3Size = *(u32*)OSPhysicalToCached(0x311C);
    gddrProtAddr = *(u32*)OSPhysicalToCached(0x3120);
    DCStoreRange(OSPhysicalToCached(0x3100), 0x100);

    rc = ESP_LaunchTitle(version, t);
    if (rc != 0) {
        OSReport("\nOSExec(): Failed to exec %d in %d\n", rc, 0x41F);
        __OSHotResetForError();
    }

    ESP_CloseLib();

    DCInvalidateRange(OSPhysicalToCached(0x3100), 0x100);

    if (simGddr3Size < *(u32*)OSPhysicalToCached(0x311C)) {
        *(u32*)OSPhysicalToCached(0x3120) = simGddr3Size - (*(u32*)OSPhysicalToCached(0x311C) - *(u32*)OSPhysicalToCached(0x3120));
        *(u32*)OSPhysicalToCached(0x3128) = simGddr3Size - (*(u32*)OSPhysicalToCached(0x311C) - *(u32*)OSPhysicalToCached(0x3128));
        *(u32*)OSPhysicalToCached(0x3130) = simGddr3Size - (*(u32*)OSPhysicalToCached(0x311C) - *(u32*)OSPhysicalToCached(0x3130));
        *(u32*)OSPhysicalToCached(0x3134) = simGddr3Size - (*(u32*)OSPhysicalToCached(0x311C) - *(u32*)OSPhysicalToCached(0x3134));
        *(u32*)OSPhysicalToCached(0x311C) = simGddr3Size;
    }

    if (gddrProtAddr < *(u32*)OSPhysicalToCached(0x3120)) {
        __OSInitMemoryProtection();
    }

    __OSInitIPCBuffer();
    IPCReInit();
    IPCCltReInit();

    DVDLowInit();
    DVDLowIntType = 0;
    DVDLowReadDiskID(&id, callback);

    while (!DVDLowIntType) {
    }

    if (DVDLowIntType != 1) {
        OSReport("\nOSExec(): Failed to exec %d in %d\n", DVDLowIntType, 0x44B);
        __OSReturnToMenuForError();
    }

    DVDLowIntType = 0;
    if (__OSLockedFlag == 0x80) {
        DVDLowOpenPartitionWithTmdAndTicketView((u32)bootGameInfo->gamePartition, t2, numTmdBytes, tmd, 0, NULL, callback);
    } else {
        DVDLowOpenPartition((u32)bootGameInfo->gamePartition, NULL, 0, 0, tmd, callback);
    }

    while (!DVDLowIntType) {
    }

    if (DVDLowIntType != 1) {
        OSReport("\nOSExec(): Failed to exec %d in %d\n", DVDLowIntType, 0x462);
        __OSReturnToMenuForError();
    }
}

#define TITLE_ID ((((u64)1) << 32) | (2))

void __OSLaunchMenu(void) {
    u8 i;
    s32 rc;
    u32 ticketCnt = 1;
    ESTicketView* t;
    ESSysVersion version = 0x0000000100000003;

    OSSetArenaLo((void*)0x81280000);
    OSSetArenaHi((void*)0x812F0000);
    rc = ESP_InitLib();

    if (rc != 0) {
        return;
    }

    rc = ESP_GetTicketViews(TITLE_ID, NULL, &ticketCnt);

    if (ticketCnt != 1 || rc != 0) {
        return;
    }

    t = (ESTicketView*)OSAllocFromMEM1ArenaLo(OSRoundUp32B(sizeof(ESTicketView) * ticketCnt), 32);
    rc = ESP_GetTicketViews(TITLE_ID, t, &ticketCnt);

    if (rc != 0) {
        return;
    }

    rc = ESP_LaunchTitle(TITLE_ID, t);

    if (rc != 0) {
        return;
    }

    while (1) {
    }
}

#if (VERSION == RMGJ01 || VERSION == RMGE01)
    static BOOL __OSCheckTmdCountryCode(ESTmdView* tmd) {
    u32 TmdCountryCode = 0;
    TmdCountryCode = *((u32*)tmd->head.reserved);

    if (TmdCountryCode == 3) {
        return TRUE;
    }
    else {
        return (TmdCountryCode == (s8)SCGetProductGameRegion()) ? TRUE : FALSE;
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

        if(__OSInIPL) {
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

#endif

typedef struct {
    char date[16];
    u32 entry;
    u32 size;
    u32 rebootSize;
    u32 reserved2;

} ImgHeader;

static inline void ReadDisc(void* addr, s32 length, u32 offset) {
    DVDCommandBlock block;
    GXColor bg = {0, 0, 0, 0};
    GXColor fg = {255, 255, 255, 0};

    DVDReadAbsAsyncPrio(&block, addr, length, offset, NULL, 0);

    while (!(DVDGetCommandBlockStatus(&block) == DVD_STATE_END)) {
        if (DVDGetCommandBlockStatus(&block) > 2 || DVDGetCommandBlockStatus(&block) < 0) {
            __OSReturnToMenuForError();
        }
    };
}

static u32 GetApploaderPosition(void) {
    static u32 apploaderPosition = 0;

    if (apploaderPosition) {
        return apploaderPosition;
    }

    if (__OSCurrentTGCOffset) {
        u32* tgcHeader;
        s32 apploaderOffsetInTGC;

        tgcHeader = OSAllocFromMEM1ArenaLo(64, 32);
        ReadDisc(tgcHeader, 64, (u32)(__OSCurrentTGCOffset >> 2));
        apploaderOffsetInTGC = *(s32*)((u32)tgcHeader + 56);
        apploaderPosition = (u32)((__OSCurrentTGCOffset + apploaderOffsetInTGC) >> 2);
    } else {
        apploaderPosition = (0x00002440 >> 2);
    }

    return apploaderPosition;
}

static ImgHeader* LoadApploader(void) {
    ImgHeader* header;

    header = OSAllocFromMEM1ArenaLo(sizeof(ImgHeader), 32);
    ReadDisc(header, sizeof(ImgHeader), GetApploaderPosition());
    ReadDisc((void*)0x81200000, (s32)OSRoundUp32B(header->size), GetApploaderPosition() + (u32)(sizeof(ImgHeader) >> 2));
    ICInvalidateRange((void*)0x81200000, OSRoundUp32B(header->size));

    return header;
}

typedef void (*AppInitFunc)(void (*report)(const char*, ...));
typedef BOOL (*AppGetNextFunc)(void**, u32*, u32*);
typedef void* (*AppGetEntryFunc)(void);

typedef void (*AppInterfaceFunc)(AppInitFunc*, AppGetNextFunc*, AppGetEntryFunc*);

extern u32 __DVDLayoutFormat;

#define NEWAPPLOADER_DATE "2004/02/01"

static BOOL IsNewApploader(ImgHeader* header) {
    if (strncmp((char*)header, NEWAPPLOADER_DATE, 10) > 0) {
        return TRUE;
    } else {
        return FALSE;
    }
}

static void* LoadDol(const OSExecParams* params, AppInterfaceFunc getInterface) {
    AppInitFunc appInit;
    AppGetNextFunc appGetNext;
    AppGetEntryFunc appGetEntry;
    void* addr;
    u32 length;
    u32 offset;
    OSExecParams* paramsWork;

    getInterface(&appInit, &appGetNext, &appGetEntry);
    paramsWork = OSAllocFromMEM1ArenaLo(sizeof(OSExecParams), 1);
    __OSSetExecParams(params, paramsWork);
    appInit(OSReport);
    OSSetArenaLo(paramsWork);

    while (appGetNext(&addr, &length, &offset)) {
        ReadDisc(addr, (s32)length, (u32)(offset >> __DVDLayoutFormat));
    }

    return appGetEntry();
}

static void StartDol(const OSExecParams* params, void* entry) {
    OSExecParams* paramsWork;
    paramsWork = OSAllocFromMEM1ArenaLo(sizeof(OSExecParams), 1);
    __OSSetExecParams(params, paramsWork);
    __PIRegs[0x024 / 4] = 0x00000004 | 0x00000001 | 2;
    OSDisableInterrupts();
    Run(entry);
}

#define BOOT_REGION_START 0x812fdff0
#define BOOT_REGION_END 0x812fdfec

void __OSBootDolSimple(u32 doloffset, u32 restartCode, void* regionStart, void* regionEnd, BOOL argsUseDefault, s32 argc, char* argv[]) {
    OSExecParams* params;
    void* dolEntry;
    ImgHeader* header;
    DVDDiskID* diskId;

    OSDisableInterrupts();

    if (__OSInReboot) {
        __OSNextPartitionType = __OSLaunchPartitionType;
    }

    params = OSAllocFromMEM1ArenaLo(sizeof(OSExecParams), 1);

    params->valid = TRUE;
    params->restartCode = restartCode;
    params->regionStart = regionStart;
    params->regionEnd = regionEnd;

    params->argsUseDefault = argsUseDefault;
    if (!argsUseDefault) {
        params->argsAddr = OSAllocFromMEM1ArenaLo(0x2000, 1);
        if (__OSNextPartitionType == 2 && !__OSInReboot) {
            PackInstallerArgs(params->argsAddr, argc, argv);
        } else {
            PackArgs(params->argsAddr, argc, argv);
        }
    }

    DVDInit();
    DVDSetAutoInvalidation(TRUE);
    DVDResume();

    Prepared = FALSE;
    __DVDPrepareResetAsync(Callback);

    __OSMaskInterrupts(OS_INTERRUPTMASK_MEM | OS_INTERRUPTMASK_DSP | OS_INTERRUPTMASK_AI | OS_INTERRUPTMASK_EXI | OS_INTERRUPTMASK_PI);
    __OSUnmaskInterrupts(OS_INTERRUPTMASK_PI_ACR);
    OSEnableInterrupts();

    while (Prepared != TRUE) {
    }

    __OSLaunchNextFirmware();

    if (0xA0000000 == restartCode && !__OSInReboot) {
        ESTitleId titleId ATTRIBUTE_ALIGN(32);
        s32 rc;
        char* ptr;

        rc = ESP_InitLib();
        if (rc != 0) {
            return;
        }

        rc = ESP_GetTitleId(&titleId);
        if (rc != 0) {
            return;
        }

        rc = ESP_CloseLib();
        if (rc != 0) {
            return;
        }

        ptr = (char*)params->argsAddr + (u32)argv[1];
        snprintf(ptr, 16 + 1, "%016llx", titleId);
    }

    header = LoadApploader();

    if (IsNewApploader(header)) {
        if (doloffset == 0xffffffff) {
            doloffset = GetApploaderPosition() + ((sizeof(ImgHeader) + header->size) >> 2);
        }

        params->bootDol = doloffset;
        dolEntry = LoadDol(params, (AppInterfaceFunc)header->entry);
        diskId = (DVDDiskID*)OSPhysicalToCached(0x0000);
        *((u32*)OSPhysicalToCached(0x3180)) = *((u32*)diskId->gameName);
        *((u8*)OSPhysicalToCached(0x3184)) = 0x80;
        StartDol(params, dolEntry);
    } else {
        *(void**)BOOT_REGION_START = regionStart;
        *(void**)BOOT_REGION_END = regionEnd;
        *(u8*)(OS_BASE_CACHED | 0x30E2) = 1;

        ReadDisc((void*)0x81330000, (s32)OSRoundUp32B(header->rebootSize), (u32)(GetApploaderPosition() + ((sizeof(ImgHeader) + header->size) >> 2)));
        ICInvalidateRange((void*)0x81330000, OSRoundUp32B(header->rebootSize));
        OSDisableInterrupts();
        ICFlashInvalidate();
        Run((void*)0x81330000);
    }
}

void __OSBootDol(u32 doloffset, u32 restartCode, char* const argv[]) {
    char doloffInString[20];
    s32 argvlen;
    char** argvToPass;
    s32 i;
    void* saveStart;
    void* saveEnd;

    OSGetSaveRegion(&saveStart, &saveEnd);
    sprintf(doloffInString, "%d", doloffset);

    argvlen = 0;
    if (argv) {
        while (argv[argvlen]) {
            argvlen++;
        }
    }

    argvlen += 1;
    argvToPass = OSAllocFromMEM1ArenaLo((argvlen + 1) * sizeof(char*), 1);
    argvToPass[0] = doloffInString;

    for (i = 1; i < argvlen; i++) {
        argvToPass[i] = argv[i - 1];
    }

    __OSBootDolSimple(0xffffffff, restartCode, saveStart, saveEnd, FALSE, argvlen, argvToPass);
}
