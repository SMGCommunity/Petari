#include <revolution/base/PPCArch.h>
#include <revolution/os.h>

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
