#include <revolution/os.h>
#include <revolution/exi.h>

typedef struct SramControl {
    u8 sram[64];
    u32 offset;
    BOOL enabled;
    BOOL locked;
    BOOL sync;
    void (*callback)(void);
} SramControl;

static SramControl Scb __attribute__ ((aligned (32)));

static BOOL ReadSram(void* buffer) {
    BOOL err;
    u32 cmd;

    DCInvalidateRange(buffer, 64);

    if (!EXILock(0, 1, 0)) {
        return FALSE;
    }

    if (!EXISelect(0, 1, 3)) {
        EXIUnlock(0);
        return FALSE;
    }

    cmd = 0x20000000 | 0x100;
    err = FALSE;
    err |= !EXIImm(0, &cmd, 4, 1, NULL);
    err |= !EXISync(0);
    err |= !EXIDma(0, buffer, 64, 0, NULL);
    err |= !EXISync(0);
    err |= !EXIDeselect(0);
    EXIUnlock(0);
    return !err;
}

static BOOL WriteSram(void *, u32, u32);

static void WriteSramCallback(s32 chan, OSContext* context) {
    Scb.sync = WriteSram(Scb.sram + Scb.offset, Scb.offset, 64 - Scb.offset);

    if (Scb.sync) {
        Scb.offset = 64;
    }
}

void __OSInitSram(void) {
    Scb.locked = Scb.enabled = FALSE;
    Scb.sync = ReadSram(Scb.sram);
    Scb.offset = 64;

    OSSetGbsMode(OSGetGbsMode());
}

static BOOL WriteSram(void *buffer, u32 offset, u32 size) {
    BOOL err;
    u32 cmd;

    if (!EXILock(0, 1, WriteSramCallback)) {
        return FALSE;
    }

    if (!EXISelect(0, 1, 3)) {
        EXIUnlock(0);
        return FALSE;
    }

    offset <<= 6;
    cmd = 0xA0000000 | 0x000000100 + offset;
    err = FALSE;
    err |= !EXIImm(0, &cmd, 4, 1, NULL);
    err |= !EXISync(0);
    err |= !EXIImmEx(0, buffer, size, 1);
    err |= !EXIDeselect(0);
    EXIUnlock(0);
    return !err;
}

static void* LockSram(u32 offset) {
    BOOL enabled;
    enabled = OSDisableInterrupts();

    if (Scb.locked) {
        OSRestoreInterrupts(enabled);
        return NULL;
    }

    Scb.enabled = enabled;
    Scb.locked = TRUE;
    return Scb.sram + offset;
}

OSSramEx* __OSLockSramEx(void) {
    return LockSram(sizeof(OSSram));
}

BOOL UnlockSram(BOOL commit, u32 offset) {
    u16* p;

    if (commit) {
        if (offset == 0) {
            OSSram* sram = (OSSram*)Scb.sram;

            if ( 2u < (sram->flags & 3)) {
                sram->flags &= ~3;
            }

            sram->checkSum = sram->checkSumInv = 0;

            for (p = (u16*)&sram->counterBias; p < (u16*)(Scb.sram + sizeof(OSSram)); p++) {
                sram->checkSum += *p;
                sram->checkSumInv += ~*p;
            }
        }

        if (offset < Scb.offset) {
            Scb.offset = offset;
        }

        if (Scb.offset <= sizeof(OSSram)) {
            OSSramEx* sram = (OSSramEx*)(Scb.sram + sizeof(OSSram));

            if ((sram->gbs & 0x7C00) == 0x5000u || (sram->gbs & 0xC0) == 0xC0u) {
                sram->gbs = 0;
            }
        }

        Scb.sync = WriteSram(Scb.sram + Scb.offset, Scb.offset, 64 - Scb.offset);
    
        if (Scb.sync) {
            Scb.offset = 64;
        }
    }

    Scb.locked = FALSE;
    OSRestoreInterrupts(Scb.enabled);
    return Scb.sync;
}

BOOL __OSUnlockSramEx(BOOL commit) {
    return UnlockSram(commit, sizeof(OSSram));
}

BOOL __OSSyncSram(void) {
    return Scb.sync;
}

BOOL __OSReadROM(void* buffer, s32 length, s32 offset) {
    BOOL err;
    u32 cmd;

    DCInvalidateRange(buffer, (u32)length);

    if (!EXILock(0, 1, 0)) {
        return FALSE;
    }

    if (!EXISelect(0, 1, 3)) {
        EXIUnlock(0);
        return FALSE;
    }

    cmd = (u32)(offset << 6);
    err = FALSE;
    err |= !EXIImm(0, &cmd, 4, 1, NULL);
    err |= !EXISync(0);
    err |= !EXIDma(0, buffer, length, 0, NULL);
    err |= !EXISync(0);
    err |= !EXIDeselect(0);
    EXIUnlock(0);
    return !err;
}

u16 OSGetWirelessID(s32 chan) {
    OSSramEx* sram;
    u16 id;
    sram = __OSLockSramEx();
    id = sram->wirelessPadID[chan];
    __OSUnlockSramEx(FALSE);
    return id;
}

void OSSetWirelessID(s32 chan, u16 id) {
    OSSramEx* sram;
    sram = __OSLockSramEx();

    if (sram->wirelessPadID[chan] != id) {
        sram->wirelessPadID[chan] = id;
        __OSUnlockSramEx(TRUE);
    }
    else {
        __OSUnlockSramEx(FALSE);
    }
}

u16 OSGetGbsMode(void) {
    OSSramEx* sram;
    u16 mode;
    sram = __OSLockSramEx();
    mode = sram->gbs;
    __OSUnlockSramEx(FALSE);
    return mode;
}

void OSSetGbsMode(u16 mode) {
    OSSramEx* sram;

    if ((mode & 0x7C00u) == 0x5000 || (mode & 0xC0u) == 0xC0) {
        mode = 0;
    }

    sram = __OSLockSramEx();

    if (mode == sram->gbs) {
        __OSUnlockSramEx(FALSE);
        return;
    }

    sram->gbs = mode;
    __OSUnlockSramEx(TRUE);
}

BOOL __OSGetRTCFlags(u32* flags) {
    BOOL err;
    u32 cmd;

    if (!EXILock(0, 1, 0)) {
        return FALSE;
    }

    if (!EXISelect(0, 1, 3)) {
        EXIUnlock(0);
        return FALSE;
    }

    cmd = 0x21000800;
    err = FALSE;
    err |= !EXIImm(0, &cmd, 4, 1, NULL);
    err |= !EXISync(0);
    err |= !EXIImm(0, &cmd, 4, 0, NULL);
    err |= !EXISync(0);
    err |= !EXIDeselect(0);
    EXIUnlock(0);
    *flags = cmd;
    return !err;
}

BOOL __OSClearRTCFlags(void) {
    BOOL err;
    u32 cmd;
    u32 data = 0;

    if (!EXILock(0, 1, 0)) {
        return FALSE;
    }

    if (!EXISelect(0, 1, 3)) {
        EXIUnlock(0);
        return FALSE;
    }
    
    cmd = 0x21000800;
    err = FALSE;
    err |= !EXIImm(0, &cmd, 4, 1, NULL);
    err |= !EXISync(0);
    err |= !EXIImm(0, &data, 4, 1, NULL);
    err |= !EXISync(0);
    err |= !EXIDeselect(0);
    EXIUnlock(0);

    return !err;
}