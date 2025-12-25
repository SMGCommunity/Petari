#pragma once

#include "Game/AudioLib/AudBgm.hpp"
#include "Game/AudioLib/AudRemixSequencer.hpp"
#include "Game/AudioLib/AudSoundObject.hpp"
#include "Game/AudioLib/AudSystem.hpp"
#include <revolution.h>

class AudSceneMgr;

class AudWrap {
public:
    static AudBgmMgr* getBgmMgr();
    static AudBgm* getStageBgm();

    static AudSystem* getSystem();

    static AudSceneMgr* getSceneMgr();

    static void setNextIdStageBgm(u32);

    static void startStageBgm(u32, bool);

    static AudRemixSequencer* getRemixSequencer();
    static AudSoundObject* getRemixSeqObject();
    static AudSoundObject* getSystemSeObject();
};
