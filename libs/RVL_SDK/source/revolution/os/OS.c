#include <revolution/os.h>
#include <revolution/db.h>
#include <revolution/os/OSBootInfo.h>
#include <cstring>
#include "private/flipper.h"

#include <__ppc_eabi_linker.h>

static __OSExceptionHandler* OSExceptionTable;

static u32  __OSExceptionLocations[] = {
    0x100,
    0x200,
    0x300,
    0x400,
    0x500,
    0x600,
    0x700,
    0x800,
    0x900,
    0xC00,
    0xD00,
    0xF00,
    0x1300,
    0x1400,
    0x1700
};


void __OSEVStart(void);
void __OSEVEnd(void);
void __OSEVSetNumber(void);
void __DBVECTOR(void);
void __OSDBINTSTART(void);
void __OSDBINTEND(void);
void __OSDBJUMPSTART(void);
void __OSDBJUMPEND(void);

static f64 ZeroF;
static f32 ZeroPS[2];

__declspec(weak) BOOL __OSIsGcam = FALSE;
OSTime __OSStartTime;
BOOL __OSInIPL = FALSE;
BOOL __OSInNandBoot = FALSE;
extern BOOL __OSInReboot;
static OSBootInfo* BootInfo;
static u32* BI2DebugFlag;
static u32 BI2DebugFlagHolder;

vu16 __OSDeviceCode : (OS_BASE_CACHED | 0x30E6);

extern u8   __ArenaHi[];
extern u8   __ArenaLo[];
extern u32  __DVDLongFileNameFlag;
extern u32  __PADSpec;
static BOOL AreWeInitialized = FALSE;
OSExecParams __OSRebootParams;

static void OSExceptionInit(void);
void OSDefaultExceptionHandler( __OSException exception, OSContext* context );

static const char* __OSVersion = "<< RVL_SDK - OS \trelease build: Jan 30 2008 01:38:43 (0x4199_60831) >>";

asm void __OSFPRInit(void) {
    nofralloc
    mfmsr r3
    ori r3, r3, 0x2000
    mtmsr r3

    mfspr r3, 0x398
    rlwinm. r3, r3, 3, 31, 31
    beq skipPairedSingleInit

    lis r3, ZeroPS@ha
    addi r3, r3, ZeroPS@l
    psq_l fp0, 0(r3), 0, 0
    ps_mr fp1, fp0
    ps_mr fp2, fp0
    ps_mr fp3, fp0
    ps_mr fp4, fp0
    ps_mr fp5, fp0
    ps_mr fp6, fp0
    ps_mr fp7, fp0
    ps_mr fp8, fp0
    ps_mr fp9, fp0
    ps_mr fp10, fp0
    ps_mr fp11, fp0
    ps_mr fp12, fp0
    ps_mr fp13, fp0
    ps_mr fp14, fp0
    ps_mr fp15, fp0
    ps_mr fp16, fp0
    ps_mr fp17, fp0
    ps_mr fp18, fp0
    ps_mr fp19, fp0
    ps_mr fp20, fp0
    ps_mr fp21, fp0
    ps_mr fp22, fp0
    ps_mr fp23, fp0
    ps_mr fp24, fp0
    ps_mr fp25, fp0
    ps_mr fp26, fp0
    ps_mr fp27, fp0
    ps_mr fp28, fp0
    ps_mr fp29, fp0
    ps_mr fp30, fp0
    ps_mr fp31, fp0

skipPairedSingleInit:
    lfd fp0, ZeroF
    fmr fp1, fp0
    fmr fp2, fp0
    fmr fp3, fp0
    fmr fp4, fp0
    fmr fp5, fp0
    fmr fp6, fp0
    fmr fp7, fp0
    fmr fp8, fp0
    fmr fp9, fp0
    fmr fp10, fp0
    fmr fp11, fp0
    fmr fp12, fp0
    fmr fp13, fp0
    fmr fp14, fp0
    fmr fp15, fp0
    fmr fp16, fp0
    fmr fp17, fp0
    fmr fp18, fp0
    fmr fp19, fp0
    fmr fp20, fp0
    fmr fp21, fp0
    fmr fp22, fp0
    fmr fp23, fp0
    fmr fp24, fp0
    fmr fp25, fp0
    fmr fp26, fp0
    fmr fp27, fp0
    fmr fp28, fp0
    fmr fp29, fp0
    fmr fp30, fp0
    fmr fp31, fp0

    mtfsf 0xFF, fp0
    blr
}

void __OSGetIOSRev(OSIOSRev* rev) {
    u32 iosVer, iosBuild;
    iosVer = *(u32*)OSPhysicalToUncached(0x3140);
    iosBuild = *(u32*)OSPhysicalToUncached(0x3144);

    rev->reserved = ((iosVer >> 24) & 0xFF);
    rev->major = ((iosVer >> 16) & 0xFF);
    rev->minor = ((iosVer >> 8) & 0xFF);
    rev->micro = (iosVer & 0xFF);

    rev->month = (((iosBuild >> 20) & 0xF) * 10 + ((iosBuild >> 16) & 0xF));
    rev->date = (((iosBuild >> 12) & 0xF) * 10 + ((iosBuild >> 8) & 0xF));
    rev->year = (((iosBuild >> 4) & 0xF) * 10 + (iosBuild & 0xF) + 2000);
    return;
}

u32 __OSGetHollywoodRev(void) {
    return (*(u32*)OSPhysicalToCached((0x3138)));
} 

u32 OSGetConsoleType(void) {
    u32 hwRev;
    u32 gddrSize;

    if (BootInfo == NULL || BootInfo->consoleType == 0) {
        return 0x10000002;
    }

    hwRev = __OSGetHollywoodRev();

    if (__OSDeviceCode & 0x8000) {
        switch (__OSDeviceCode & ~0x8000) {
            case 2:
            case 3:
            case 0x203:
                switch (hwRev) {
                    case 0:
                        return 0x10;
                    case 1:
                        return 0x11;
                    case 2:
                        return 0x12;
                    case 0x10:
                        return 0x20;
                    case 0x11:
                        return 0x21;
                }

                if (hwRev > 0x11) {
                    return 0x21;
                }

            case 0x202:
            case 0x201:
                switch (hwRev) {
                    case 0:
                        return 0x10000010;
                    case 1:
                        return 0x10000011;
                    case 2:
                        return 0x10000012;
                    case 0x10:
                        return 0x10000020;
                    case 0x11:
                        return 0x10000021;
                }

                if (hwRev > 0x11) {
                    return 0x10000031;
                }

            case 0x300:
                return 0x100;

            default:
                break;
        }
    }

    gddrSize = OSGetPhysicalMem2Size();

    switch (hwRev) {
        case 0:
            if (gddrSize == 0x4000000) {
                return 0x10;
            }
            else {
                return 0x10000010;
            }

        case 0x1:
            if (gddrSize == 0x4000000) {
                return 0x11;
            }
            else {
                return 0x10000011;
            }

        case 0x2:
            if (gddrSize == 0x4000000) {
                return 0x12;
            }
            else {
                return 0x10000012;
            }

        case 0x10:
            if (gddrSize == 0x4000000) {
                return 0x20;
            }
            else {
                return 0x10000020;
            }

        case 0x11:
            if (gddrSize == 0x4000000) {
                return 0x21;
            }
            else {
                return 0x10000021;
            }
    }

    if (hwRev > 0x11) {
        if (gddrSize == 0x4000000) {
            return 0x21;
        }
        else {
            return 0x10000031;
        }
    }

    return BootInfo->consoleType;
}


// ClearArena
// ClearMEM2Arena
// InquiryCallback

static void DisableWriteGatherPipe(void) {
    u32 hid2 = PPCMfhid2();
    hid2 &= 0xBFFFFFFF;
    PPCMthid2(hid2);
}

static void ReportOSInfo(void) {
    u32 consoleType;
    u32 sysMemSize;
    OSIOSRev ios;

    OSReport("\nRevolution OS\n");
    OSReport("Kernel built : %s %s\n", "Jan 30 2008", "01:38:43");
    OSReport("Console Type : " );

    consoleType = OSGetConsoleType();

    switch (consoleType & 0xF0000000) {
        case 0:
            switch (consoleType) {
                case 0x11:
                    OSReport("Pre-production board 1\n");
                    break;
                case 0x12:
                    OSReport("Pre-production board 2-1\n");
                    break;
                case 0x20:
                    OSReport("Pre-production board 2-2\n");
                    break;
                case 0x100:
                    OSReport("RVA 1\n");
                    break;
                default:
                    OSReport("Retail %d\n", consoleType);
                    break;
            }

            break;

        case 0x10000000:
            switch (consoleType) {
                case 0x10000021:
                    OSReport("NDEV 2.1\n");
                    break;
                case 0x10000020:
                    OSReport("NDEV 2.0\n");
                    break;
                case 0x10000012:
                    OSReport("NDEV 1.2\n");
                    break;
                case 0x10000011:
                    OSReport("NDEV 1.1\n");
                    break;
                case 0x10000010:
                    OSReport("NDEV 1.0\n");
                    break;
                case 0x10000008:
                    OSReport("Revolution Emulator\n");
                    break;
                default:
                    OSReport("Emulation platform (%08x)\n", consoleType);
                    break;
            }

            break;

        case 0x20000000:
            OSReport("TDEV-based emulation HW%d\n", (consoleType & ~0xF0000000) - 3);
            break;
        default:
            OSReport("%08x\n", consoleType);
            break;
    }


    __OSGetIOSRev(&ios);
    OSReport("Firmware     : %d.%d.%d ", ios.major, ios.minor, ios.micro);
    OSReport("(%d/%d/%d)\n", ios.month, ios.date, ios.year);
    sysMemSize = (OSGetConsoleSimulatedMem1Size() + OSGetConsoleSimulatedMem2Size());
    OSReport("Memory %d MB\n", sysMemSize/(1024*1024));

    OSReport("MEM1 Arena : 0x%x - 0x%x\n", OSGetMEM1ArenaLo(), OSGetMEM1ArenaHi());
    OSReport("MEM2 Arena : 0x%x - 0x%x\n", OSGetMEM2ArenaLo(), OSGetMEM2ArenaHi());
}

void OSInit(void) {
    void* bi2StartAddr;
    void* arenaAddr;

    if (AreWeInitialized == TRUE) {
        return;
    }

    AreWeInitialized = TRUE;
    __OSStartTime = __OSGetSystemTime();
    OSDisableInterrupts();
    __OSGetExecParams(&__OSRebootParams);

    PPCMtmmcr0(0);
    PPCMtmmcr1(0);
    PPCMtpmc1(0);
    PPCMtpmc2(0);
    PPCMtpmc3(0);
    PPCMtpmc4(0);
    PPCMthid4(0x83900000);
    PPCDisableSpeculation();
    PPCSetFpNonIEEEMode();

    BootInfo = (OSBootInfo*)OSPhysicalToCached(0);
    BI2DebugFlag = 0;
    __DVDLongFileNameFlag = 0;

    bi2StartAddr = *(void**)OSPhysicalToCached(0xF4);

    if (bi2StartAddr != 0) {
        BI2DebugFlag = (u32*)((u32)bi2StartAddr + 0xC);
        __PADSpec = *(u32*)((u32)bi2StartAddr + 0x24);

        *(u8*)OSPhysicalToCached(0x30E8) = (u8)*BI2DebugFlag;
        *(u8*)OSPhysicalToCached(0x30E9) = (u8)__PADSpec;
    }
    else {
        if (BootInfo->arenaHi != 0) {
            BI2DebugFlagHolder = (u32)*(u8*)OSPhysicalToCached(0x30E8);
            BI2DebugFlag = &BI2DebugFlagHolder;
            __PADSpec = (u32)*(u8*)OSPhysicalToCached(0x30E9);
        }
    }

    __DVDLongFileNameFlag = 1;

    arenaAddr = (void*)*(u32*)OSPhysicalToCached(0x310C);

    if (arenaAddr == 0) {
        if (OSIsMEM1Region((void*)__ArenaLo)) {
            arenaAddr = BootInfo->arenaLo == 0 ? (void*)__ArenaLo : BootInfo->arenaLo;
        
            if ((BootInfo->arenaLo == 0) && (BI2DebugFlag && (*(BI2DebugFlag) <2))) {
                arenaAddr = (void*)OSRoundUp32B(_stack_addr);
            }
        }
        else {
            arenaAddr = (void*)0x80004000;
        }
    }

    OSSetMEM1ArenaLo(arenaAddr);

    arenaAddr = (void*)*(u32*)OSPhysicalToCached(0x3110);

    if (arenaAddr == NULL) {
        arenaAddr = BootInfo->arenaHi == NULL ? (void*)__ArenaHi : BootInfo->arenaHi;
    }

    OSSetMEM1ArenaHi(arenaAddr);

    arenaAddr = (void*)*(u32*)OSPhysicalToCached(0x3124);

    if (arenaAddr != 0) {
        if (OSIsMEM2Region((void*)__ArenaLo)) {
            arenaAddr = (void*)__ArenaLo;

            if ((BI2DebugFlag && (*(BI2DebugFlag) < 2))) {
                arenaAddr = (void*)OSRoundUp32B(_stack_addr);
            }
        }
        else {
            if ((u32)arenaAddr >= 0x90000000 && (u32)arenaAddr < 0x90000800) {
                arenaAddr = (void*)0x90000800;
            }
        }

        OSSetMEM2ArenaLo(arenaAddr);
    }


    arenaAddr = (void*)*(u32*)OSPhysicalToCached(0x3128);

    if (arenaAddr != 0) {
        OSSetMEM2ArenaHi(arenaAddr);
    }

    __OSInitIPCBuffer();
    OSExceptionInit();
    __OSInitSystemCall();
    __OSInitAlarm();
    __OSModuleInit();
    __OSInterruptInit();
    __OSContextInit();
    __OSCacheInit();
    EXIInit();
    SIInit();
    __OSInitSram();
    __OSThreadInit();
    __OSInitAudioSystem();
    DisableWriteGatherPipe();

    if (!__OSInIPL) {
        __OSInitMemoryProtection();
    }

    ReportOSInfo();
    OSRegisterVersion(__OSVersion);
}

static void OSExceptionInit(void) {
    __OSException exception;
    void* destAddr;
    u32* opCodeAddr;
    u32 oldOpCode;
    u8* handlerStart;
    u32 handlerSize;

    opCodeAddr = (u32*)__OSEVSetNumber;
    oldOpCode = *opCodeAddr;
    handlerStart = (u8*)__OSEVStart;
    handlerSize = (u32)((u8*)__OSEVEnd - (u8*)__OSEVStart);
    destAddr = (void*)OSPhysicalToCached(0x60);

    if (*(u32*)destAddr == 0) {
        DBPrintf("Installing OSDBIntegrator\n");
        memcpy(destAddr, (void*)__OSDBINTSTART, (u32)__OSDBINTEND - (u32)__OSDBINTSTART);
        DCFlushRangeNoSync(destAddr, (u32)__OSDBINTEND - (u32)__OSDBINTSTART);
        __sync();
        ICInvalidateRange(destAddr, (u32)__OSDBINTEND - (u32)__OSDBINTSTART);
    }

    for (exception = 0; exception < 15; exception++) {
        if (BI2DebugFlag && (*BI2DebugFlag >= 2) && __DBIsExceptionMarked(exception)) {
            DBPrintf(">>> OSINIT: exception %d commandeered by TRK\n", exception);
            continue;
        }

        *opCodeAddr = oldOpCode | exception;

        if (__DBIsExceptionMarked(exception)) {
            DBPrintf(">>> OSINIT: exception %d vectored to debugger\n", exception);
            memcpy((void*)__DBVECTOR, (void*)__OSDBJUMPSTART, (u32)__OSDBJUMPEND - (u32)__OSDBJUMPSTART);
        }
        else {
            u32* ops = (u32*)__DBVECTOR;
            int cb;

            for (cb = 0; cb < (u32)__OSDBJUMPEND - (u32)__OSDBJUMPSTART; cb += 4) {
                // set op to NOP
                *ops++ = 0x60000000;
            }
        }

        destAddr = (void*)OSPhysicalToCached(__OSExceptionLocations[exception]);
        memcpy(destAddr, handlerStart, handlerSize);
        DCFlushRangeNoSync(destAddr, handlerSize);
        __sync();
        ICInvalidateRange(destAddr, handlerSize);
    }

    OSExceptionTable = OSPhysicalToCached(0x3000);

    for (exception = 0; exception < 15; exception++) {
        __OSSetExceptionHandler(exception, OSDefaultExceptionHandler);
    }

    *opCodeAddr = oldOpCode;
    DBPrintf("Exceptions initialized...\n");
}

static asm void __OSDBIntegrator(void) {
    nofralloc

entry __OSDBINTSTART
    li r5, 0x40
    mflr r3
    stw r3, 0xC(r5)
    lwz r3, 0x8(r5)
    oris r3, r3, 0x8000
    mtlr r3
    li r3, 0x30
    mtmsr r3
    blr

entry __OSDBINTEND
}

static asm void __OSDBJump(void) {
    nofralloc
entry __OSDBJUMPSTART
    bla 0x60
entry __OSDBJUMPEND
}

__OSExceptionHandler __OSSetExceptionHandler(__OSException ex, __OSExceptionHandler handler) {
    __OSExceptionHandler cur;
    cur = OSExceptionTable[ex];
    OSExceptionTable[ex] = handler;
    return cur;
}

__OSExceptionHandler __OSGetExceptionHandler(__OSException ex) {
    return OSExceptionTable[ex];
}

static asm void OSExceptionVector(void) {
    nofralloc
entry __OSEVStart
    mtsprg 0, r4
    lwz r4, 0xC0
    stw r3, 12(r4)
    mfsprg  r3, 0
    stw r3, 16(r4)
    stw r5, 20(r4)
    lhz r3, 418(r4)
    ori r3, r3, 2
    sth r3, 418(r4)

    mfcr r3
    stw r3, 128(r4)
    mflr r3
    stw r3, 132(r4)
    mfctr r3
    stw r3, 136(r4)
    mfxer r3
    stw r3, 140(r3)
    mfsrr0 r3
    stw r3, 408(r4)
    mfsrr1 r3
    stw r3, 412(r4)
    mr r5, r3

entry __DBVECTOR
    nop

    mfmsr r3
    ori r3, r3, 0x30
    mtsrr1 r3

entry __OSEVSetNumber
    addi r3, 0, 0

    lwz r4, 0xD4
    rlwinm. r5, r5, 0, 30, 30
    bne recover
    addis r5, 0, OSDefaultExceptionHandler@ha
    addi r5, r5, OSDefaultExceptionHandler@l
    mtsrr0 r5
    rfi

recover:
    rlwinm r5, r3, 2, 22, 29
    lwz r5, 0x3000(r5)
    mtsrr0 r5

    rfi

entry __OSEVEnd
    nop
}

asm void OSDefaultExceptionHandler(register __OSException exception, register OSContext* context) {
    nofralloc
    
    stw r0, 0(context)
    stw r1, 4(context)
    stw r2, 8(context)
    stmw r6, 0x18(context)

    mfspr r0, 0x391
    stw r0, 0x1A8(context)

    mfspr r0, 0x392
    stw r0, 0x1AC(context)

    mfspr r0, 0x393
    stw r0, 0x1B0(context)

    mfspr r0, 0x394
    stw r0, 0x1B4(context)

    mfspr r0, 0x395
    stw r0, 0x1B8(context)

    mfspr r0, 0x396
    stw r0, 0x1BC(context)

    mfspr r0, 0x397
    stw r0, 0x1C0(context)

    mfdsisr r5
    mfdar r6
    stwu r1, -8(r1)
    b __OSUnhandledException
}

void __OSPSInit(void) {
    PPCMthid2(PPCMfhid2() | 0xA0000000);
    ICFlashInvalidate();
    __sync();

    asm {
        li r3, 0
        mtspr 0x390, r3
        mtspr 0x391, r3
        mtspr 0x392, r3
        mtspr 0x393, r3
        mtspr 0x394, r3
        mtspr 0x395, r3
        mtspr 0x396, r3
        mtspr 0x397, r3
    }
}

u32 __OSGetDIConfig(void) {
    return __DIRegs[9] & 0xFF;
}

void OSRegisterVersion(const char *id) {
    OSReport("%s\n", id);
}