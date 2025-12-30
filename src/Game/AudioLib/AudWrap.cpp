#include "Game/AudioLib/AudWrap.hpp"
#include "Game/AudioLib/AudBgmMgr.hpp"
#include "Game/AudioLib/AudRemixMgr.hpp"
#include "Game/AudioLib/AudSystem.hpp"

namespace AudWrap {
    AudSystem* getSystem() {
        return AudSystem::msBasic;
    }

    // getSoundInfo

    AudSceneMgr* getSceneMgr() {
        return getSystem()->mSceneMgr;
    }

    AudBgmMgr* getBgmMgr() {
        return &getSystem()->mBgmMgr;
    }

    AudBgm* getStageBgm() {
        return getBgmMgr()->_0[0];
    }

    AudBgm* getSubBgm() {
        return getBgmMgr()->_0[1];
    }

    void startStageBgm(u32 param1, bool param2) {
        getBgmMgr()->start(0, param1, param2);
    }

    void startSubBgm(u32 param1, bool param2) {
        getBgmMgr()->start(1, param1, param2);
    }

    void setNextIdStageBgm(u32 param1) {
        getBgmMgr()->setNextBGM(0, param1);;
    }

    void startLastStageBgm() {
        getBgmMgr()->startLastBGM(0);;
    }

    AudSoundObject* getSystemSeObject() {
        return getSystem()->mSystemSeObject;
    }

    AudSoundObject* getAtmosphereSeObject() {
        return getSystem()->mAtmosphereSeObject;
    }

    AudSoundObjHolder* getSoundObjHolder() {
        return getSystem()->mSoundObjHolder;
    }

    AudRhythmMeSystem* getRhythmMeSystem() {
        return getSystem()->mRhythmMeSystem;
    }

    AudMeObject* getSystemMeObject() {
        return getSystem()->mSystemMeObject;
    }

    AudRemixMgr* getRemixMgr() {
        return getSystem()->mRemixMgr;
    }

    AudRemixSequencer* getRemixSequencer() {
        return getRemixMgr()->mRemixSeq;
    }

    AudSoundObject* getRemixSeqObject() {
        return getRemixMgr()->mSoundObj;
    }
};
