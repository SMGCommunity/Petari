#include "Game/System/HeapMemoryWatcher.hpp"
#include "Game/Util/SingletonHolder.hpp"
#include "JSystem/JKernel/JKRHeap.hpp"
#include "JSystem/JKernel/JKRExpHeap.hpp"
#include "Kamek.hpp"
#include "revolution/dvd.h"
#include "revolution/gx/GXStruct.h"
#include "revolution/os.h"
#include "revolution/types.h"

#define CUSTOM_CODE_PATH "/CustomCode/CustomCode.bin"
#define KREL_Addr32 1
#define KREL_Addr16Lo 4
#define KREL_Addr16Hi 5
#define KREL_Addr16Ha 6
#define KREL_Rel24 10
#define KREL_Write32 32
#define KREL_Write16 33
#define KREL_Write8 34
#define KREL_CondWritePointer 35
#define KREL_CondWrite32 36
#define KREL_CondWrite16 37
#define KREL_CondWrite8 38
#define KREL_Branch 64
#define KREL_BranchLink 65

extern "C" {
    void* memset(void *, int, int);
    void TRK_flush_cache(u32, int);
    void __OSStopAudioSystem();
}

typedef void (*CtorFuncPtr)();

struct KamekHeader {
    /* 0x00 */ u32 mMagic1;
    /* 0x04 */ u16 mMagic2;
    /* 0x06 */ u16 mVersion;
    /* 0x08 */ u32 mBssSize;
    /* 0x0C */ u32 mCodeSize;
    /* 0x10 */ u32 mCtorStart;
    /* 0x14 */ u32 mCtorEnd;
    /* 0x18 */ u8 mPadding[8];
};

struct KamekRelCmdHeader {
    /* 0x00 */ unsigned mCmdType : 8;
    /* 0x01 */ unsigned mAddress : 24;
};

// TODO clean up (or not)
namespace {
    inline u32 resolveAddress(u32 text, u32 address) {
        return address & 0x80000000 ? address : (text + address);
    }

    inline u8* cmdAddr32(u8 *pInput, u32 text, u32 address) {
        u32 target = resolveAddress(text, *(const u32 *)pInput);
        *(u32 *)address = target;
        return pInput + 4;
    }

    inline u8* cmdAddr16Lo(u8 *pInput, u32 text, u32 address) {
        u32 target = resolveAddress(text, *(const u32 *)pInput);
        *(u16 *)address = target & 0xFFFF;
        return pInput + 4;
    }

    inline u8* cmdAddr16Hi(u8 *pInput, u32 text, u32 address) {
        u32 target = resolveAddress(text, *(const u32 *)pInput);
        *(u16 *)address = target & 0xFFFF;
        return pInput + 4;
    }

    inline u8* cmdAddr16Ha(u8 *pInput, u32 text, u32 address) {
        u32 target = resolveAddress(text, *(const u32 *)pInput);
        *(u16 *)address = target >> 16;
        if (target & 0x8000)
            *(u16 *)address += 1;
        return pInput + 4;
    }

    inline u8* cmdRel24(u8 *pInput, u32 text, u32 address) {
        u32 target = resolveAddress(text, *(const u32 *)pInput);
        u32 delta = target - address;
        *(u32 *)address &= 0xFC000003;
        *(u32 *)address |= (delta & 0x3FFFFFC);
        return pInput + 4;
    }

    inline u8* cmdWrite32(u8 *pInput, u32 text, u32 address) {
        u32 value = *(const u32 *)pInput;
        *(u32 *)address = value;
        return pInput + 4;
    }

    inline u8* cmdWrite16(u8 *pInput, u32 text, u32 address) {
        u32 value = *(const u32 *)pInput;
        *(u16 *)address = value & 0xFFFF;
        return pInput + 4;
    }

    inline u8* cmdWrite8(u8 *pInput, u32 text, u32 address) {
        u32 value = *(const u32 *)pInput;
        *(u8 *)address = value & 0xFF;
        return pInput + 4;
    }

    inline u8* cmdCondWritePointer(u8 *pInput, u32 text, u32 address) {
        u32 target = resolveAddress(text, *(const u32 *)pInput);
        u32 original = ((const u32 *)pInput)[1];
        if (*(u32 *)address == original)
            *(u32 *)address = target;
        return pInput + 8;
    }

    inline u8* cmdCondWrite32(u8 *pInput, u32 text, u32 address) {
        u32 value = *(const u32 *)pInput;
        u32 original = ((const u32 *)pInput)[1];
        if (*(u32 *)address == original)
            *(u32 *)address = value;
        return pInput + 8;
    }

    inline u8* cmdCondWrite16(u8 *pInput, u32 text, u32 address) {
        u32 value = *(const u32 *)pInput;
        u32 original = ((const u32 *)pInput)[1];
        if (*(u16 *)address == (original & 0xFFFF))
            *(u16 *)address = value & 0xFFFF;
        return pInput + 8;
    }

    inline u8* cmdCondWrite8(u8 *pInput, u32 text, u32 address) {
        u32 value = *(const u32 *)pInput;
        u32 original = ((const u32 *)pInput)[1];
        if (*(u8 *)address == (original & 0xFF))
            *(u8 *)address = value & 0xFF;
        return pInput + 8;
    }

    inline u8* cmdBranch(u8 *pInput, u32 text, u32 address) {
        *(u32 *)address = 0x48000000;
        return cmdRel24(pInput, text, address);
    }

    inline u8* cmdBranchLink(u8 *pInput, u32 text, u32 address) {
        *(u32 *)address = 0x48000001;
        return cmdRel24(pInput, text, address);
    }
}

static void error(const char* pMessage) {
    GXColor fg = { 0xFF, 0xFF, 0xFF, 0xFF };
    GXColor bg = { 0x00 };
    OSFatal(fg, bg, pMessage);
}

static void initCustomCode() {
    DVDFileInfo fileHandle;
    int id = DVDConvertPathToEntrynum(CUSTOM_CODE_PATH);

    if (id < 0) {
        OSReport("[KariponLoader] No code binary found\n");
        return;
    }

    if (!DVDFastOpen(id, &fileHandle)) {
        error("[KariponLoader] Failed to create file handle\n");
        return;
    }

    KamekHeader header ATTRIBUTE_ALIGN(32);
    DVDReadPrio(&fileHandle, &header, sizeof(header), 0, 2);

    if (header.mMagic1 != 'Kame' || header.mMagic2 != 'k\0') {
        error("[KariponLoader] Invalid code binary\n");
        return;
    }

    if (header.mVersion != 2) {
        error("[KariponLoader] Unsupported code binary\n");
        return;
    }

    OSReport("[KariponLoader] Loading binary...\n");
    HeapMemoryWatcher* pHeapMemoryWatcher = SingletonHolder<HeapMemoryWatcher>::get();

    u32 textSize = header.mCodeSize;
    u32 bssSize = header.mBssSize;
    u32 binarySize = ROUND_UP(textSize + bssSize, 32);
    u8* pBinary = new (pHeapMemoryWatcher->mStationedHeapNapa, 32) u8[binarySize];
    u8* pTextStart = pBinary;
    u8* pBssStart = pBinary + textSize;
    DVDReadPrio(&fileHandle, pTextStart, textSize, sizeof(header), 2);
    memset(pBssStart, 0, bssSize);

    u32 hookOffset = sizeof(header) + textSize;
    u32 hookSize = fileHandle.length - hookOffset;
    u8* pHookBuffer = reinterpret_cast<u8*>(ROUND_UP(reinterpret_cast<u32>(pHeapMemoryWatcher->mGameHeapNapa) + 0x200, 32));
    u8* pHookStart = pHookBuffer;
    u8* pHookEnd = pHookStart + hookSize;
    DVDReadPrio(&fileHandle, pHookStart, hookSize, hookOffset, 2);
    DVDClose(&fileHandle);

    OSReport("[KariponLoader] Linking binary...\n");
    while (pHookStart < pHookEnd) {
        const KamekRelCmdHeader* pCmdHeader = reinterpret_cast<const KamekRelCmdHeader*>(pHookStart);
        pHookStart += sizeof(KamekRelCmdHeader);

        u32 address;
        if (pCmdHeader->mAddress == 0xFFFFFE) {
            address = *reinterpret_cast<u32*>(pHookStart);
            pHookStart += sizeof(u32);
        }
        else {
            address = pCmdHeader->mAddress + reinterpret_cast<u32>(pTextStart);
        }

        #define KREL_HandleCmd(name) \
            case KREL_##name: pHookStart = cmd##name(pHookStart, reinterpret_cast<u32>(pTextStart), address); break;

        switch (pCmdHeader->mCmdType) {
        KREL_HandleCmd(Addr32)
        KREL_HandleCmd(Addr16Lo)
        KREL_HandleCmd(Addr16Hi)
        KREL_HandleCmd(Addr16Ha)
        KREL_HandleCmd(Rel24)
        KREL_HandleCmd(Write32)
        KREL_HandleCmd(Write16)
        KREL_HandleCmd(Write8)
        KREL_HandleCmd(CondWritePointer)
        KREL_HandleCmd(CondWrite32)
        KREL_HandleCmd(CondWrite16)
        KREL_HandleCmd(CondWrite8)
        KREL_HandleCmd(Branch)
        KREL_HandleCmd(BranchLink)
        default:
            OSReport("[KariponLoader] Unknown relocation command 0x%02X\n", pCmdHeader->mCmdType);
            break;
        }

        TRK_flush_cache(address & (~31), 0x20);
    }

    memset(pHookBuffer, 0, hookSize);
    
    OSReport("[KariponLoader] Running .ctors...\n");
    CtorFuncPtr* ctorStart = reinterpret_cast<CtorFuncPtr*>(pTextStart + header.mCtorStart);
    CtorFuncPtr* ctorEnd = reinterpret_cast<CtorFuncPtr*>(pTextStart + header.mCtorEnd);

    while (ctorStart < ctorEnd) {
        (*ctorStart)();
        ctorStart++;
    }

    OSReport("[KariponLoader] Binary loaded at 0x%08X, size 0x%X\n", pBinary, binarySize);
}

// Initialize custom code
extern kmSymbol init__19GameSystemExceptionFv;
kmBranch(&init__19GameSystemExceptionFv + 0xB8, initCustomCode);

// Always display the exception log and disable audio during exceptions
extern kmSymbol handleException__19GameSystemExceptionFUsP9OSContextUlUl;
kmCall(&handleException__19GameSystemExceptionFUsP9OSContextUlUl + 0x90, __OSStopAudioSystem);
kmWrite32(&handleException__19GameSystemExceptionFUsP9OSContextUlUl + 0x94, PPC_B(0x6C));
kmSymWrite32(OSPanic, 0x108, PPC_NOP);
kmSymWrite32(__OSUnhandledException, 0x54, PPC_NOP);

// Display file name in file not found crashes
extern kmSymbol loadToMainRAM__10FileRipperFPCcPUcbP7JKRHeapQ210FileRipper14AllocDirection;
kmWrite32(&loadToMainRAM__10FileRipperFPCcPUcbP7JKRHeapQ210FileRipper14AllocDirection + 0x4C, PPC_MR(3, 26)); // mr r3, r26

// Shrink home button heap
extern kmSymbol createHeaps__17HeapMemoryWatcherFv;
kmWrite16(&createHeaps__17HeapMemoryWatcherFv + 0x82, 5);
