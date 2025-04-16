#pragma once

#include "Game/AudioLib/AudBgm.hpp"
#include <revolution.h>

// temp
class AudBgmMgr {
public:
    u8 _0[0x10];
    u32 _10;
};

class AudSystem;

class AudWrap {
public:
    static AudBgmMgr* getBgmMgr();
    static AudBgm* getStageBgm();

    static AudSystem* getSystem();

    static void setNextIdStageBgm(u32);

    static void startStageBgm(u32, bool);
};
