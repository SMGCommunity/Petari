#include "Game/AudioLib/AudWrap.hpp"
#include "Game/AudioLib/AudBgmMgr.hpp"
#include "Game/AudioLib/AudRemixMgr.hpp"
#include "Game/AudioLib/AudSoundInfo.hpp"
#include "Game/AudioLib/AudSoundNameConverter.hpp"
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
        return getBgmMgr()->mBgm[AudBgmMgr::BgmType_Stage];
    }

    AudBgm* getSubBgm() {
        return getBgmMgr()->mBgm[AudBgmMgr::BgmType_Sub];
    }

    JAISoundHandle* startStageBgm(u32 soundID, bool param2) {
        return getBgmMgr()->start(AudBgmMgr::BgmType_Stage, soundID, param2);
    }

    JAISoundHandle* startSubBgm(u32 soundID, bool param2) {
        return getBgmMgr()->start(AudBgmMgr::BgmType_Sub, soundID, param2);
    }

    void setNextIdStageBgm(u32 soundID) {
        getBgmMgr()->setNextBGM(AudBgmMgr::BgmType_Stage, soundID);
    }

    JAISoundHandle* startLastStageBgm() {
        return getBgmMgr()->startLastBGM(AudBgmMgr::BgmType_Stage);
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
