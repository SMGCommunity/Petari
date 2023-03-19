#include "revolution/exi.h"
#include "private/flipper.h"
#include "private/io_reg.h"
#include <cstring>

#define CPR_EXIINTMSK_MASK  EXI_0CPR_EXIINTMSK_MASK
#define CPR_EXIINT_MASK     EXI_0CPR_EXIINT_MASK
#define CPR_TCINTMSK_MASK   EXI_0CPR_TCINTMSK_MASK
#define CPR_TCINT_MASK      EXI_0CPR_TCINT_MASK
#define CPR_EXTINTMSK_MASK  EXI_0CPR_EXTINTMSK_MASK
#define CPR_EXTINT_MASK     EXI_0CPR_EXTINT_MASK
#define CPR_EXT_MASK        EXI_0CPR_EXT_MASK

#define CPR_ALLINTMSK_MASK  (CPR_EXTINTMSK_MASK | CPR_EXIINTMSK_MASK | CPR_TCINTMSK_MASK)
#define CPR_ALLINT_MASK     (CPR_EXTINT_MASK | CPR_EXIINT_MASK | CPR_TCINT_MASK)
#define CPR_CLK_MASK        EXI_0CPR_CLK_MASK
#define CPR_CS_MASK         (EXI_0CPR_CS0B_MASK | EXI_0CPR_CS1B_MASK | EXI_0CPR_CS2B_MASK)

#define EXI_0CR_TSTART_SHIFT    0
#define EXI_0CR_DMA_SHIFT       1
#define EXI_0CR_RW_SHIFT        2
#define EXI_0CR_TLEN_SHIFT      4

#define EXI_0CR_GET_TSTART(exi_0cr) \
        ((((unsigned long)(exi_0cr)) & 0x00000001) >> EXI_0CR_TSTART_SHIFT)

#define EXI_0CR(tstart, dma, rw, tlen) \
        ((((unsigned long)(tstart)) << 0) | \
         (((unsigned long)(dma)) << 1) | \
         (((unsigned long)(rw)) << 2) | \
         (((unsigned long)(tlen)) << 4))

extern BOOL __OSInIPL;

typedef struct EXIControl {
    EXICallback exiCallback;
    EXICallback tcCallback;
    EXICallback extCallback;
    vu32 state;
    int immLen;
    u8* immBuf;
    u32 dev;
    u32 id;
    s32 idTime;
    int items;

    struct {
        u32 device;
        EXICallback cb;
    } queue[3];
} EXIControl;

static EXIControl Ecb[3];
s32 __EXIProbeStartTime[2] : (OS_BASE_CACHED | 0x30C0);
static u32 IDSerialPort1;

static const char* __EXIVersion = "<< RVL_SDK - EXI \trelease build: Aug  8 2007 01:59:22 (0x4199_60831) >>";

#define REG(chan, idx) (__EXIRegs[((chan) * 5) + (idx)])

void SetExiInterruptMask(s32 chan, EXIControl* exi) {
    EXIControl* exi2;
    exi2 = &Ecb[2];

    switch (chan) {
        case 0:
            if ((exi->exiCallback == 0 && exi2->exiCallback == 0) || (exi->state & 0x10)) {
                __OSMaskInterrupts(0x410000);
            }
            else {
                __OSUnmaskInterrupts(0x410000);
            }
            break;

        case 1:
            if (exi->exiCallback == 0 || (exi->state & 0x10)) {
                __OSMaskInterrupts(0x80000);
            }
            else {
                __OSUnmaskInterrupts(0x80000);
            }
            break;

        case 2:
            if (__OSGetInterruptHandler(25) == NULL || (exi->state & 0x10)) {
                __OSMaskInterrupts(0x40);
            }
            else {
                __OSUnmaskInterrupts(0x40);
            }
            break;
    }
}

static void CompleteTransfer(s32 chan) {
    EXIControl* exi = &Ecb[chan];
    u8* buf;
    u32 data;
    int i;
    int len;

    if (exi->state & 3) {
        if ((exi->state & 2) && (exi->immLen != 0)) {
            len = exi->immLen;
            buf = exi->immBuf;
            data = REG(chan, 4);

            for (i = 0; i < len; i++) {
                *buf++ = (u8)((data >> ((3 - i) * 8)) & 0xFF);
            }
        }

        exi->state &= ~3;
    }
}

BOOL EXIImm(s32 chan, void* buf, s32 len, u32 type, EXICallback callback) {
    EXIControl* exi = &Ecb[chan];
    BOOL enabled;

    enabled = OSDisableInterrupts();

    if ((exi->state & 3) || !(exi->state & 4)) {
        OSRestoreInterrupts(enabled);
        return FALSE;
    }

    exi->tcCallback = callback;

    if (exi->tcCallback) {
        EXIClearInterrupts(chan, FALSE, TRUE, FALSE);
        __OSUnmaskInterrupts(0x200000u >> (3 * chan));
    }

    exi->state |= 2;

    if (type != 0) {
        u32 data;
        int i;

        data = 0;

        for (i = 0 ; i < len ; i++) {
            data |= ((u8*)buf)[i] << ((3 - i) * 8);
        }

        REG(chan, 4) = data;
    }

    exi->immBuf = buf;
    exi->immLen = (type != 1) ? len : 0;
    REG(chan, 3) = EXI_0CR(1, 0, type, len-1);
    OSRestoreInterrupts(enabled);
    return TRUE;
}

BOOL EXIImmEx(s32 chan, void* buf, s32 len, u32 mode) {
    s32 xLen;

    while (len != 0) {
        xLen = (len < 4) ? len : 4;

        if (!EXIImm(chan, buf, xLen, mode, 0)) {
            return FALSE;
        }

        if (!EXISync(chan)) {
            return FALSE;
        }

        (u8*)buf += xLen;
        len -= xLen;
    }

    return TRUE;
}

u32 EXIClearInterrupts(s32 channel, BOOL exi, BOOL tc, BOOL ext) {
    u32 cpr;
    u32 prev;

    prev = cpr = REG(channel, EXI_0CPR_IDX);
    cpr &= 0x7F5;

    if (exi) {
        cpr |= 2;
    }

    if (tc) {
        cpr |= 8;
    }

    if (ext) {
        cpr |= 0x800;
    }

    REG(channel, EXI_0CPR_IDX) = cpr;
    return prev;
}

BOOL EXIDma(s32 channel, void *buffer, s32 length, u32 type, EXICallback cb) {
    EXIControl* exi = &Ecb[channel];
    BOOL isEnabled;

    isEnabled = OSDisableInterrupts();

    if ((exi->state & 3) || !(exi->state & 4)) {
        OSRestoreInterrupts(isEnabled);
        return FALSE;
    }

    exi->tcCallback = cb;

    if (exi->tcCallback) {
        EXIClearInterrupts(channel, FALSE, TRUE, FALSE);
        __OSUnmaskInterrupts(0x200000u >> (3 * channel));
    }

    exi->state |= 1;
    REG(channel, 1) = (u32)buffer & 0xFFFFFFE0;
    REG(channel, 2) = length;
    REG(channel, 3) = (4 * type) | 3;
    OSRestoreInterrupts(isEnabled);
    return TRUE;
}

extern u32 __OSGetDIConfig(void);
vu16 __OSDeviceCode : (OS_BASE_CACHED | 0x30E6);

BOOL EXISync(s32 chan) {
    EXIControl* exi = &Ecb[chan];
    BOOL rc = FALSE;
    BOOL enabled;

    while (exi->state & 4) {
        if (EXI_0CR_GET_TSTART(REG(chan, 3)) == 0) {
            enabled = OSDisableInterrupts();

            if (exi->state & 4)
            {
                CompleteTransfer(chan);
                if (__OSGetDIConfig() != 0xFF || ((OSGetConsoleType() & 0xf0000000) == 0x20000000) || exi->immLen != 4 || (REG(chan, 0) & CPR_CLK_MASK) != 0 ||
                    (REG(chan, 4) != 0x1010000 && REG(chan, 4) != 0x5070000 && REG(chan, 4) != 0x4220001) ||
                    __OSDeviceCode == (0x8000 | 0x200))
                {
                    rc = TRUE;
                }
            }

            OSRestoreInterrupts(enabled);
            break;
        }
    }

    return rc;
}

EXICallback EXISetExiCallback(s32 chan, EXICallback exiCallback) {
    EXIControl* exi = &Ecb[chan];
    EXICallback prev;
    BOOL enabled;

    enabled = OSDisableInterrupts();
    prev = exi->exiCallback;
    exi->exiCallback = exiCallback;

    if (chan != 2) {
        SetExiInterruptMask(chan, exi);
    }
    else {
        SetExiInterruptMask(0, &Ecb[0]);
    }

    OSRestoreInterrupts(enabled);
    return prev;
}

void EXIProbeReset(void) {
    __EXIProbeStartTime[0] = __EXIProbeStartTime[1] = 0;
    Ecb[0].idTime = Ecb[1].idTime = 0;

    __EXIProbe(0);
    __EXIProbe(1);
}

static BOOL __EXIProbe(s32 chan) {
    EXIControl* exi = &Ecb[chan];
    BOOL enabled;
    BOOL rc;
    u32 cpr;
    s32 t;

    if (chan == 2) {
        return TRUE;
    }

    rc = TRUE;
    enabled = OSDisableInterrupts();
    cpr = REG(chan, 0);

    if (!(exi->state & 8)) {
        if (cpr & 0x800) {
            EXIClearInterrupts(chan, FALSE, FALSE, TRUE);
            __EXIProbeStartTime[chan] = exi->idTime = 0;
        }

        if (cpr & 0x1000) {
            t = (s32)(OSTicksToMilliseconds(OSGetTime()) / 100) + 1;

            if (__EXIProbeStartTime[chan] == 0) {
                __EXIProbeStartTime[chan] = t;
            }

            if (t - __EXIProbeStartTime[chan] < 300 / 100) {
                rc = FALSE;
            }
        }
        else {
            __EXIProbeStartTime[chan] = exi->idTime = 0;
            rc = FALSE;
        }
    }
    else if (!(cpr & 0x1000) || (cpr & 0x800)) {
        __EXIProbeStartTime[chan] = exi->idTime = 0;
        rc = FALSE;
    }

    OSRestoreInterrupts(enabled);
    return rc;
}

BOOL EXIProbe(s32 chan) {
    EXIControl* exi = &Ecb[chan];
    BOOL rc;
    u32 id;

    rc = __EXIProbe(chan);

    if (rc && exi->idTime == 0) {
        rc = EXIGetID(chan, 0, &id) ? TRUE : FALSE;
    }

    return rc;
}

static BOOL __EXIAttach(s32 chan, EXICallback extCallback) {
    EXIControl* exi = &Ecb[chan];
    BOOL enabled;

    enabled = OSDisableInterrupts();

    if ((exi->state & 8) || __EXIProbe(chan) == FALSE) {
        OSRestoreInterrupts(enabled);
        return FALSE;
    }

    EXIClearInterrupts(chan, TRUE, FALSE, FALSE);
    exi->extCallback = extCallback;
    __OSUnmaskInterrupts(0x100000u >> (3 * chan));
    exi->state |= 8;
    OSRestoreInterrupts(enabled);
    return TRUE;
}

BOOL EXIAttach(s32 chan, EXICallback extCallback) {
    EXIControl* exi = &Ecb[chan];
    BOOL enabled;
    BOOL rc;

    EXIProbe(chan);
    enabled = OSDisableInterrupts();

    if (exi->idTime == 0) {
        OSRestoreInterrupts(enabled);
        return FALSE;
    }

    rc = __EXIAttach(chan, extCallback);
    OSRestoreInterrupts(enabled);
    return rc;
}

BOOL EXIDetach(s32 chan) {
    EXIControl* exi = &Ecb[chan];
    BOOL enabled;

    enabled = OSDisableInterrupts();

    if (!(exi->state & 8)) {
        OSRestoreInterrupts(enabled);
        return TRUE;
    }

    if ((exi->state & 0x10) && exi->dev == 0) {
        OSRestoreInterrupts(enabled);
        return FALSE;
    }

    exi->state &= 0xFFFFFFF7;
    __OSMaskInterrupts(0x500000u >> (3 * chan));
    OSRestoreInterrupts(enabled);
    return TRUE;
}

BOOL EXISelect(s32 chan, u32 dev, u32 freq) {
    EXIControl* exi = &Ecb[chan];
    u32 cpr;
    BOOL enabled;

    enabled = OSDisableInterrupts();

    if ((exi->state & 4) || chan != 2 && (dev == 0 && !(exi->state & 8) && !__EXIProbe(chan) || !(exi->state & 0x10) || (exi->dev != dev))) {
        OSRestoreInterrupts(enabled);
        return FALSE;
    }

    exi->state |= 4;
    cpr = REG(chan, 0);
    cpr &= 0x405;
    cpr |= (1 << dev << 7) | (freq << 4);
    REG(chan, 0) = cpr;

    if (exi->state & 8) {
        switch (chan) {
            case 0:
                __OSMaskInterrupts(0x100000);
                break;
            case 1:
                __OSMaskInterrupts(0x20000);
                break;
        }
    }

    OSRestoreInterrupts(enabled);
    return TRUE;
}

BOOL EXIDeselect(s32 chan) {
    EXIControl* exi = &Ecb[chan];
    u32 cpr;
    BOOL enabled;
    enabled = OSDisableInterrupts();

    if (!(exi->state & 4)) {
        OSRestoreInterrupts(enabled);
        return FALSE;
    }

    exi->state &= 0xFFFFFFFB;
    cpr = REG(chan, 0);
    REG(chan, 0) = cpr & 0x405;

    if (exi->state & 8) {
        switch (chan) {
            case 0:
                __OSUnmaskInterrupts(0x100000);
                break;
            case 1:
                __OSUnmaskInterrupts(0x20000);
                break;
        }
    }

    OSRestoreInterrupts(enabled);

    if (chan != 2 && (cpr & 0x80)) {
        return __EXIProbe(chan) ? TRUE : FALSE;
    }

    return TRUE;
}

static void EXIIntrruptHandler(__OSInterrupt interrupt, OSContext* context) {
    s32 chan;
    EXIControl* exi;
    EXICallback callback;

    chan = (interrupt - 9) / 3;
    exi = &Ecb[chan];
    EXIClearInterrupts(chan, TRUE, FALSE, FALSE);
    callback = exi->exiCallback;

    if (callback) {
        OSContext exceptionContext;
        OSClearContext(&exceptionContext);
        OSSetCurrentContext(&exceptionContext);
        callback(chan, context);
        OSClearContext(&exceptionContext);
        OSSetCurrentContext(context);
    }
}

static void TCIntrruptHandler(__OSInterrupt interrupt, OSContext* context) {
    s32 chan;
    EXIControl* exi;
    EXICallback callback;
    chan = (interrupt - 10) / 3;
    exi = &Ecb[chan];
    __OSMaskInterrupts(0x80000000u >> interrupt);
    EXIClearInterrupts(chan, FALSE, TRUE, FALSE);
    callback = exi->tcCallback;

    if (callback) {
        OSContext exceptionContext;
        exi->tcCallback = 0;
        CompleteTransfer(chan);
        OSClearContext(&exceptionContext);
        OSSetCurrentContext(&exceptionContext);
        callback(chan, context);
        OSClearContext(&exceptionContext);
        OSSetCurrentContext(context);
    }
}

static void EXTIntrruptHandler(__OSInterrupt interrupt, OSContext* context) {
    s32 chan;
    EXIControl* exi;
    EXICallback callback;
    chan = (interrupt - 11) / 3;
    __OSMaskInterrupts(0x500000u >> (3 *  chan));
    exi = &Ecb[chan];
    callback = exi->extCallback;
    exi->state &= 0xFFFFFFF7;

    if (callback) {
        OSContext exceptionContext;
        OSClearContext(&exceptionContext);
        OSSetCurrentContext(&exceptionContext);
        exi->extCallback = 0;
        callback(chan, context);
        OSClearContext(&exceptionContext);
        OSSetCurrentContext(context);
    }
}

void EXIInit(void) {
    u32 id;

    while(EXI_0CR_GET_TSTART(REG(0, 3)) == 1 || EXI_0CR_GET_TSTART(REG(1, 0)) == 1 || EXI_0CR_GET_TSTART(REG(2, 0)) == 1) {

    }

    __OSMaskInterrupts(0x7F8000);
    REG(0, 0) = 0;
    REG(1, 0) = 0;
    REG(2, 0) = 0;
    REG(0, 0) = 0x2000;

    __OSSetInterruptHandler(9, EXIIntrruptHandler);
    __OSSetInterruptHandler(10, TCIntrruptHandler);
    __OSSetInterruptHandler(11, EXTIntrruptHandler);
    __OSSetInterruptHandler(12, EXIIntrruptHandler);
    __OSSetInterruptHandler(13, TCIntrruptHandler);
    __OSSetInterruptHandler(14, EXTIntrruptHandler);
    __OSSetInterruptHandler(15, EXIIntrruptHandler);
    __OSSetInterruptHandler(16, TCIntrruptHandler);

    EXIGetID(0, 2, &IDSerialPort1);

    if (__OSInIPL) {
        EXIProbeReset();
    }
    else {
        if (EXIGetID(0, 0, &id) && id == 0x07010000) {
            __OSEnableBarnacle(1, 0);
        }
        else if (EXIGetID(1, 0, &id) && id == 0x07010000) {
            __OSEnableBarnacle(0, 2);
        }
    }

    OSRegisterVersion(__EXIVersion);
}

BOOL EXILock(s32 chan, u32 dev, EXICallback unlockedCallback) {
    EXIControl* exi = &Ecb[chan];
    BOOL enabled;
    int i;

    enabled = OSDisableInterrupts();

    if (exi->state & 0x10) {
        if (unlockedCallback) {
            for (i = 0; i < exi->items; i++) {
                if (exi->queue[i].device == dev) {
                    OSRestoreInterrupts(enabled);
                    return FALSE;
                }
            }

            exi->queue[exi->items].cb = unlockedCallback;
            exi->queue[exi->items].device = dev;
            exi->items++;
        }

        OSRestoreInterrupts(enabled);
        return FALSE;
    }

    exi->state |= 0x10;
    exi->dev = dev;
    SetExiInterruptMask(chan, exi);
    OSRestoreInterrupts(enabled);
    return TRUE;
}

BOOL EXIUnlock(s32 chan) {
    EXIControl* exi = &Ecb[chan];
    BOOL enabled;
    EXICallback unlockedCallback;

    enabled = OSDisableInterrupts();

    if (!(exi->state & 0x10)) {
        OSRestoreInterrupts(enabled);
        return FALSE;
    }

    exi->state &= 0xFFFFFFEF;
    SetExiInterruptMask(chan, exi);

    if (0 < exi->items) {
        unlockedCallback = exi->queue[0].cb;

        if (0 < --exi->items) {
            memmove(&exi->queue[0], &exi->queue[1], 8 * exi->items);
        }

        unlockedCallback(chan, 0);
    }

    OSRestoreInterrupts(enabled);
    return TRUE;
}

s32 EXIGetID(s32, u32, u32 *);

static void UnlockedHandler(s32 chan, OSContext* context) {
    u32 id;
    EXIGetID(chan, 0, &id);
}

s32 EXIGetID(s32 chan, u32 dev, u32* id) {
    EXIControl* exi = &Ecb[chan];
    BOOL err;
    u32 cmd;
    s32 startTime;
    BOOL enabled;

    if (chan == 0 && dev == 2 && IDSerialPort1 != 0) {
        *id = IDSerialPort1;
        return 1;
    }

    if (chan < 2 && dev == 0) {
        if (!__EXIProbe(chan)) {
            return 0;
        }

        if (exi->idTime == __EXIProbeStartTime[chan]) {
            *id = exi->id;
            return exi->idTime;
        }

        if (!__EXIAttach(chan, 0)) {
            return 0;
        }

        startTime = __EXIProbeStartTime[chan];
    }

    enabled = OSDisableInterrupts();
    err = !EXILock(chan, dev, (chan < 2 && dev == 0) ? UnlockedHandler : 0);

    if (err == 0) {
        err = !EXISelect(chan, dev, 0);

        if (err == 0) {
            cmd = 0;
            err |= !EXIImm(chan, &cmd, 2, 1, 0);
            err |= !EXISync(chan);
            err |= !EXIImm(chan, id, 4, 0, 0);
            err |= !EXISync(chan);
            err |= !EXIDeselect(chan);
        }

        EXIUnlock(chan);
    }

    OSRestoreInterrupts(enabled);

    if (chan < 2 && dev == 0) {
        EXIDetach(chan);

        enabled = OSDisableInterrupts();
        err |= (startTime != __EXIProbeStartTime[chan]);

        if (err == 0) {
            exi->id = *id;
            exi->idTime = startTime;
        }

        OSRestoreInterrupts(enabled);
        return err ? 0 : exi->idTime;
    }

    return err ? 0 : 1;
}