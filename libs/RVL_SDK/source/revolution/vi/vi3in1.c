#include <revolution/vi.h>

static VIVideo Vdac_Flag_Region;
volatile u32 Vdac_Flag_Changed = 0;

extern s32 __VISendI2CData(u8, u8 *, s32);
extern void WaitMicroTime(s32);

static void __VISetVideoMode(VIVideo vmode, VIBool outsel) {
    u8 buffer[2];
    Vdac_Flag_Region = vmode;
    buffer[0] = 1;
    buffer[1] = (u8)((outsel << 5) | Vdac_Flag_Region);
    __VISendI2CData(0xE0, buffer, 2);
    WaitMicroTime(2);
}

void __VISetRGBModeImm(void) {
    __VISetVideoMode(VI_VMODE_RGB, VI_DISABLE);
}