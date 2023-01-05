#include "revolution/exi.h"
#include "private/flipper.h"
#include "private/io_reg.h"

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

typedef struct EXIControl {
    EXICallback exiCb;
    EXICallback tcCb;
    EXICallback extCb;
    vu32 state;
    int immLen;
    u8* immBuf;
    u32 deviceNum;
    u32 id;
    u32 idTime;
    int items;

    struct {
        u32 device;
        EXICallback cb;
    } queue[3];
} EXIControl;

static EXIControl Ecb[3];

#define REG(chan, idx) (__EXIRegs[((chan) * 5) + (idx)])

#define EXI_0CR(tstart, dma, rw, tlen) \
	((((u32)(tstart)) << 0) | \
	 (((u32)(dma)) << 1) | \
	 (((u32)(rw)) << 2) | \
	 (((u32)(tlen)) << 4))

inline u32 EXIClearInterrupts(s32 channel, BOOL exi, BOOL tc, BOOL ext) {
    u32 cpr;
    u32 prev;

    prev = cpr = REG(channel, EXI_0CPR_IDX);
    cpr &= 0x7F5;

    if (exi) {
        cpr |= CPR_EXIINT_MASK;
    }

    if (tc) {
        cpr |= CPR_TCINT_MASK;
    }

    if (ext) {
        cpr |= CPR_EXTINT_MASK;
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

    exi->tcCb = cb;

    if (exi->tcCb) {
        EXIClearInterrupts(channel, FALSE, TRUE, FALSE);
        __OSUnmaskInterrupts(OS_INTERRUPTMASK_EXI_0_TC >> (3 * channel));
    }

    exi->state |= 1;
    REG(channel, 1) = (u32)buffer & 0xFFFFFFE0;
    REG(channel, 2) = length;
    REG(channel, 3) = (4 * type) | 3;
    OSRestoreInterrupts(isEnabled);
    return TRUE;
}