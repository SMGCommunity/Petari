#pragma once

#include "JSystem/JAudio2/JAISoundParams.hpp"
#include "JSystem/JAudio2/JASHeapCtrl.hpp"

struct JASTrack;

struct JAISoundChild : JASPoolAllocObject< JAISoundChild > {
    void init();
    void mixOut(JASTrack*);
    void calc();

    JAISoundChild() {
        init();
    }

    /* 0x00 */ JAISoundParamsMove mMove;
    /* 0x50 */ JASSoundParams mParams;
};
