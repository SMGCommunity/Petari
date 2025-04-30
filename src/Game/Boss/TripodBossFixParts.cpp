#include "Game/Boss/TripodBossFixParts.hpp"
#include "Game/Boss/TripodBossAccesser.hpp"
#include "Game/Boss/TripodBossFixPartsBase.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"

namespace NrvTripodBossFixParts {
    NEW_NERVE(TripodBossFixPartsNrvNonActive, TripodBossFixParts, NonActive);
    NEW_NERVE(TripodBossFixPartsNrvStartDemo, TripodBossFixParts, StartDemo);
    NEW_NERVE(TripodBossFixPartsNrvWait, TripodBossFixParts, Wait);
    NEW_NERVE(TripodBossFixPartsNrvBreak, TripodBossFixParts, Break);
};

struct TripodBossSetting {
    const char* mPartName;          // 0x00
    s32 mType;                      // 0x04
};

namespace {
    static TripodBossSetting sTripodBossSettingTable[5] = {
        { "TripodBossBodyABloom", 3  },
        { "TripodBossFootBloom", 3  },
        { "TriPodBossLeg2ABloom", 3  },
        { "TripodBossEyeLight", 2  },
        { "TripodBossEyeLightBloom", 3  }
    };

    TripodBossSetting* getSetting(const char *pObjName) {
        for (u32 i = 0; i < 5; i++) {
            const char* partName = sTripodBossSettingTable[i].mPartName;
            if (MR::isEqualString(pObjName, partName)) {
                return &sTripodBossSettingTable[i];
            }
        }

        return nullptr;
    }
};

TripodBossFixParts::TripodBossFixParts(const char *pName) : TripodBossFixPartsBase(pName) {
    _E4 = -1;
    _E8 = 180;
    mHasCollision = false;
    _ED = 1;
    _EE = 0;
    mIsPlayingWarning = false;
}

// https://decomp.me/scratch/UtuYP
void TripodBossFixParts::init(const JMapInfoIter &rIter) {
    const char* objName;
    MR::getObjectName(&objName, rIter);
    setName(objName);
    TripodBossFixPartsBase::init(rIter);
    bool val = false;
    TripodBossSetting* setting = getSetting(objName);

    if (setting != nullptr) {
        val = (setting->mType & 0x1);
        _ED = !(setting->mType & 0x2);
    }

    char mapPartsID[0x100];
    MR::getMapPartsObjectNameIfExistShapeID(mapPartsID, sizeof(mapPartsID), rIter);
    initModelManagerWithAnm(mapPartsID, nullptr, false);

    if (val) {
        MR::connectToScene(this, 0x23, 0xB, 0x1E, -1);
    }
    else {
        MR::connectToScene(this, 0x23, 0xB, 0x1F, -1);
    }

    if (MR::isEqualString(objName, "TriPodBossBodyA")) {
        TVec3f clip;
        clip.x = 250.0f;
        clip.y = 1000.0f;
        clip.z = 0.0f;
        setClippingSphere(clip, 3750.0f);
    }
    else {
        initClippingSphere();
    }

    MR::getJMapInfoArg1NoInit(rIter, &_E4);
    MR::getJMapInfoArg2NoInit(rIter, &_E8);

    if (_ED) {
        initBreakMovement(_E4);
    }

    mHasCollision = MR::isExistKcl(this, mapPartsID);

    if (mHasCollision) {
        initSensor(rIter);
        MR::initCollisionParts(this, mapPartsID, getSensor("body"), nullptr);
    }

    initEffectKeeper(0, "TripodBoss", false);
    initSound(5, false);
    initNerve(&NrvTripodBossFixParts::TripodBossFixPartsNrvNonActive::sInstance);

    if (mHasCollision) {
        MR::invalidateCollisionParts(this);
    }

    makeActorDead();
}

void TripodBossFixParts::initSensor(const JMapInfoIter &rIter) {
    initHitSensor(1);
    if (MR::isMapPartsPressOn(rIter)) {
        MR::addBodyMessageSensorMapObjPress(this);
    }
    else {
        MR::addBodyMessageSensorMapObj(this);
    }
}

void TripodBossFixParts::kill() {
    switch (_E4) {
        case 0:
            MR::emitEffect(this, "ExplosionS");
            MR::startSound(this, "SE_BM_TRIPOD_KILL_S", -1, -1);
            break;
        case 1:
            MR::emitEffect(this, "ExplosionM");
            MR::startSound(this, "SE_BM_TRIPOD_KILL_M", -1, -1);
            MR::tryRumblePadVeryWeak(this, 0);
            break;
        case 2:
            MR::emitEffect(this, "ExplosionM");
            MR::startSound(this, "SE_BM_TRIPOD_KILL_M", -1, -1);
            MR::shakeCameraVeryWeak();
            MR::tryRumblePadMiddle(this, 0);
            break;
        case 3:
            MR::emitEffect(this, "ExplosionL");
            MR::startSound(this, "SE_BM_TRIPOD_KILL_L", -1, -1);
            MR::shakeCameraStrong();
            MR::tryRumblePadVeryStrong(this, 0);
            break;
    }

    LiveActor::kill();
}

void TripodBossFixParts::activateTripodBoss() {
    if (isNerve(&NrvTripodBossFixParts::TripodBossFixPartsNrvNonActive::sInstance)) {
        MR::onCalcAnim(this);
        if (mHasCollision) {
            MR::validateCollisionParts(this);
        }

        _E0 = 1;
        _E1 = 0;

        if (MR::isExistAnim(this, "StartDemo")) {
            setNerve(&NrvTripodBossFixParts::TripodBossFixPartsNrvStartDemo::sInstance);
        }
        else {
            setNerve(&NrvTripodBossFixParts::TripodBossFixPartsNrvWait::sInstance);
        }
    }
}

void TripodBossFixParts::exeStartDemo() {
    if (MR::isFirstStep(this)) {
        MR::tryStartAllAnim(this, "StartDemo");
    }

    updateTripodMatrix();

    if (!MR::isStartDemoTripodBoss()) {
        setNerve(&NrvTripodBossFixParts::TripodBossFixPartsNrvWait::sInstance);
    }
}

void TripodBossFixParts::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::tryStartAllAnim(this, "Wait");
        _EE = 0;
    }

    exeWaitOwn();
    updateTripodMatrix();
    if (MR::isRideMarioTripodBoss() && MR::isOnGroundPlayer() && !_EE) {
        MR::tryStartAllAnim(this, "Warning");
        _EE = 1;
        if (MR::isExistAnim(this, "Warning") && !mIsPlayingWarning) {
            MR::startSound(this, "SE_BM_TRIPOD_MARKER", -1, -1);
            mIsPlayingWarning = 1;
        }
    }

    if (MR::isLeaveMarioNowTripodBoss() && _EE) {
        MR::tryStartAllAnim(this, "Wait");
        if (mIsPlayingWarning) {
            MR::stopSound(this, "SE_BM_TRIPOD_MARKER", 60);
        }

        mIsPlayingWarning = false;
        _EE = 0;
    }

    if (MR::isEndBreakDownDemoTripodBoss()) {
        if (_ED) {
            setNerve(&NrvTripodBossFixParts::TripodBossFixPartsNrvBreak::sInstance);
        }
        else {
            kill();
        }
    }
}

void TripodBossFixParts::exeBreak() {
    if (MR::isFirstStep(this)) {
        startBreakMovement();
        if (mHasCollision) {
            MR::invalidateCollisionParts(this);
        }
    }

    updateBreakMovementMatrix();
    if (MR::isGreaterStep(this, _E8)) {
        kill();
    }
}

void TripodBossFixParts::exeNonActive() {
    
}

TripodBossFixParts::~TripodBossFixParts() {

}

void TripodBossFixParts::exeWaitOwn() {
    
}
