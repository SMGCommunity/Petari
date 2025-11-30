#ifndef EXI_H
#define EXI_H

#include "revolution/os.h"
#include "revolution/types.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const u32 __EXIFreq;

typedef void (*EXICallback)(s32 chan, OSContext* context);

BOOL EXISync(s32);
BOOL EXIImm(s32, void*, s32, u32, EXICallback);
BOOL EXIDma(s32, void*, s32, u32, EXICallback);
BOOL EXIAttach(s32, EXICallback);
BOOL EXIDetach(s32);
BOOL EXISelect(s32, u32, u32);
BOOL EXIDeselect(s32);
BOOL EXILock(s32, u32, EXICallback);
BOOL EXIUnlock(s32);
BOOL EXIImmEx(s32, void*, s32, u32);

EXICallback EXISetExiCallback(s32, EXICallback);

BOOL EXIWriteReg(s32, u32, u32, void*, s32);

s32 EXIGetID(s32, u32, u32*);

u32 EXIClearInterrupts(s32, BOOL, BOOL, BOOL);

void __OSEnableBarnacle(s32, u32);

/* exi2 */

typedef struct EXIChannelParam {
    u32 cpr;     // 0x0
    void* mar;   // 0x4
    u32 length;  // 0x8
    u32 cr;      // 0xC
    u32 data;    // 0x10
} EXIChannelParam;

#ifdef __MWERKS__
volatile EXIChannelParam EXI_CHAN_PARAMS[3] : 0xCD006800;
#else
volatile EXIChannelParam EXI_CHAN_PARAMS[3];
#endif

// CPR - Channel Parameter Register
#define EXI_CPR_EXIINTMASK (1 << 0)
#define EXI_CPR_EXIINT (1 << 1)
#define EXI_CPR_TCINTMASK (1 << 2)
#define EXI_CPR_TCINT (1 << 3)
#define EXI_CPR_CLK (1 << 4 | 1 << 5 | 1 << 6)
#define EXI_CPR_CS0B (1 << 7)
#define EXI_CPR_CS1B (1 << 8)
#define EXI_CPR_CS2B (1 << 9)
#define EXI_CPR_EXTINTMASK (1 << 10)
#define EXI_CPR_EXTINT (1 << 11)
#define EXI_CPR_EXT (1 << 12)
#define EXI_CPR_ROMDIS (1 << 13)

// CR - Control Register
#define EXI_CR_TSTART (1 << 0)
#define EXI_CR_DMA (1 << 1)
#define EXI_CR_RW (1 << 2 | 1 << 3)
#define EXI_CR_TLEN (1 << 4 | 1 << 5)

BOOL __EXI2Imm(void* mem, s32 size, u32 type);
void __DBEXIInit(void);
BOOL __DBEXIReadReg(u32 cmd, void* mem, s32 size);
BOOL __DBEXIWriteReg(u32 cmd, const void* mem, s32 size);
BOOL __DBEXIReadRam(u32 cmd, void* mem, s32 size);
BOOL __DBEXIWriteRam(u32 cmd, const void* mem, s32 size);

static u32 ODEMUGenMailData(u32 ofs, u32 size) {
    return (ofs & 0xff) << 0x10 | 0x1f000000 | size & 0x1fff;
}

static u32 ODEMUGetPage(u32 mail) {
    return (mail & 0xFF0000) >> 16;
}

static u32 ODEMUGetPc2NngcOffset(u32 mail) {
    if (!(ODEMUGetPage(mail) & 0x1)) {
        return 0;
    }

    return 0x800;
}

static BOOL ODEMUIsValidMail(u32 mail) {
    return (mail & 0x1F000000) == 0x1F000000;
}

static u32 ODEMUGetSize(u32 mail) {
    return mail & 0x1FFF;
}

static BOOL __DBReadMailbox(u32* mailOut) {
    return __DBEXIReadReg(0x34000200, mailOut, sizeof(*mailOut));
}

static BOOL __DBRead(u32 ofs, void* dest, u32 size) {
    return __DBEXIReadRam(((ofs + 0xD10000) * 0x40) & 0x3FFFFF00, dest, size);
}

static BOOL __DBWriteMailbox(u32 mail) {
    return __DBEXIWriteReg(0xB4000100, &mail, sizeof(mail));
}

static BOOL __DBWrite(u32 ofs, const void* src, u32 size) {
    return __DBEXIWriteRam((((ofs + 0xD10000) * 0x40) & 0x3FFFFF00) | 0x80000000, src, size);
}

static u32 __EXISwap32(u32 val) {
    return val >> 24 & 0x000000FF | val >> 8 & 0x0000FF00 | val << 8 & 0x00FF0000 | val << 24 & 0xFF000000;
}

#ifdef __cplusplus
}
#endif

#endif  // EXI_H
