#include <revolution/base/PPCArch.h>
#include <revolution/os.h>

union Fpscr {
    f64 f;

    struct {
        u32 _0;
        u32 fpscr;
    } u;
};

asm u32 PPCMfmsr(void) {
    nofralloc
    mfmsr r3
    blr
}

asm void PPCMtmsr(register u32 val) {
    nofralloc
    mtmsr val
    blr
}

asm u32 PPCMfhid0(void) {
    nofralloc
    mfspr r3, 0x3F0
    blr
}

asm void PPCMthid0(register u32 val) {
    nofralloc
    mtspr 0x3F0, val
    blr
}

asm u32 PPCMfl2cr(void) {
    nofralloc
    mfspr r3, 0x3F9
    blr
}

asm void PPCMtl2cr(register u32 val) {
    nofralloc
    mtspr 0x3F9, val
    blr
}

asm void PPCMtdec(register u32 val) { 
    nofralloc
    mtdec val
    blr
}

asm void PPCSync(void) {
    nofralloc
    sc
    blr
}

asm void PPCHalt(void) {
    nofralloc
    sync

loop:
    nop
    li r3, 0
    nop
    b loop
}

asm void PPCMtmmcr0(register u32 val) {
    nofralloc
    mtspr 0x3B8, val
    blr
}

asm void PPCMtmmcr1(register u32 val) {
    nofralloc
    mtspr 0x3BC, val
    blr
}

asm void PPCMtpmc1(register u32 val) {
    nofralloc
    mtspr 0x3B9, val
    blr
}

asm void PPCMtpmc2(register u32 val) {
    nofralloc
    mtspr 0x3BA, val
    blr
}

asm void PPCMtpmc3(register u32 val) {
    nofralloc
    mtspr 0x3BD, val
    blr
}

asm void PPCMtpmc4(register u32 val) {
    nofralloc
    mtspr 0x3BE, val
    blr
}

u32 PPCMffpscr(void) {
    union Fpscr m;

    asm {
        mffs fp31
        stfd fp31, m.f;
    }

    return m.u.fpscr;
}

void PPCMtfpscr(register u32 val) {
    union Fpscr m;

    asm {
        li r4, 0
        stw r4, m.u._0;
        stw val, m.u.fpscr
        lfd fp31, m.f
        mtfsf 0xFF, fp31
    };
}

asm u32 PPCMfhid2(void) {
    nofralloc
    mfspr r3, 0x398
    blr
}

asm void PPCMthid2(u32) {
    nofralloc
    mtspr 0x398, r3
    blr
}

asm u32 PPCMfwpar(void) {
    nofralloc
    sync
    mfspr r3, 0x399
    blr
}

asm void PPCMtwpar(u32) {
    mtspr 0x399, r3
    blr
}

void PPCDisableSpeculation(void) {
    PPCMthid0(PPCMfhid0() | 0x200);
}

asm void PPCSetFpNonIEEEMode(void) {
    nofralloc
    mtfsb1 29
    blr
}

void PPCMthid4(register u32 hid) {
    if (hid & 0x80000000) {
        asm { mtspr 0x3F3, hid }
    }
    else {
        OSReport("H4A should not be cleared because of Broadway errata.\n");
        hid |= 0x80000000;
        asm { mtspr 0x3F3, hid }
    }
}
