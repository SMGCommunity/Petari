#pragma once

#include <revolution/types.h>

class JAISoundHandle;

class AudTamakoroBgmCtrl {
public:
    AudTamakoroBgmCtrl();

    void init();
    void control(f32, bool, s32);
    void modifyTempo(JAISoundHandle*, f32);
    void modifyPitch(JAISoundHandle*, f32);

    /* 0x00 */ u32 _0;
};
