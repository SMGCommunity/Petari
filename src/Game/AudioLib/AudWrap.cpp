#include "Game/AudioLib/AudWrap.hpp"
#include "Game/AudioLib/AudBgmMgr.hpp"
#include "Game/AudioLib/AudRemixMgr.hpp"
#include "Game/AudioLib/AudSoundInfo.hpp"
#include "Game/AudioLib/AudSystem.hpp"

namespace AudWrap {
    AudSystem* getSystem() {
        return AudSystem::msBasic;
    }

    AudSoundInfo* getSoundInfo() {
        return static_cast< AudSoundInfo* >(JASGlobalInstance< JAUSoundInfo >::getInstance());
    }

    AudSceneMgr* getSceneMgr() {
        return getSystem()->mSceneMgr;
    }

    AudBgmMgr* getBgmMgr() {
        return &getSystem()->mBgmMgr;
    }

    AudBgm* getStageBgm() {
        return getBgmMgr()->mCurrentBgm[0];
    }

    AudBgm* getSubBgm() {
        return getBgmMgr()->mCurrentBgm[1];
    }

    JAISoundHandle* startStageBgm(u32 param1, bool param2) {
        return getBgmMgr()->start(0, param1, param2);
    }

    JAISoundHandle* startSubBgm(u32 param1, bool param2) {
        return getBgmMgr()->start(1, param1, param2);
    }

    void setNextIdStageBgm(u32 param1) {
        getBgmMgr()->setNextBGM(0, param1);
    }

    JAISoundHandle* startLastStageBgm() {
        return getBgmMgr()->startLastBGM(0);
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
};  // namespace AudWrap
