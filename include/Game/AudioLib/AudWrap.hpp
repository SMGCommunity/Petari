#pragma once

#include <revolution/types.h>

class AudBgm;
class AudBgmMgr;
class AudMeObject;
class AudRemixMgr;
class AudRemixSequencer;
class AudRhythmMeSystem;
class AudSceneMgr;
class AudSoundInfo;
class AudSoundObjHolder;
class AudSoundObject;
class AudSystem;

namespace AudWrap {
    AudSystem* getSystem();
    AudSoundInfo* getSoundInfo();
    AudSceneMgr* getSceneMgr();
    AudBgmMgr* getBgmMgr();
    AudBgm* getStageBgm();
    AudBgm* getSubBgm();
    void startStageBgm(u32, bool);
    void startSubBgm(u32, bool);
    void setNextIdStageBgm(u32);
    void startLastStageBgm();
    AudSoundObject* getSystemSeObject();
    AudSoundObject* getAtmosphereSeObject();
    AudSoundObjHolder* getSoundObjHolder();
    AudRhythmMeSystem* getRhythmMeSystem();
    AudMeObject* getSystemMeObject();
    AudRemixMgr* getRemixMgr();
    AudRemixSequencer* getRemixSequencer();
    AudSoundObject* getRemixSeqObject();
};
