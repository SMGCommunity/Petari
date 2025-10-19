#include "Game/LiveActor/Nerve.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Screen/InformationObserver.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/EventUtil.hpp"
#include "Game/Util/GamePadUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

struct InformationObserverAttribute {
    /* 0x00 */ const char* mMessageId;
    /* 0x04 */ bool _4;
    /* 0x08 */ const char* mFirstMorphBgmName;
    /* 0x0C */ const char* mBgmName;
};

static InformationObserverAttribute sAttr[] = {
    {"InformationObserverBee", true, "BGM_FIRST_MORPH_B", nullptr},
    {"InformationObserverTeresa", true, "BGM_FIRST_MORPH_B", nullptr},
    {"InformationObserverHopper", true, "BGM_FIRST_MORPH_B", nullptr},
    {"InformationObserverFire", true, "BGM_FIRST_MORPH_A", "BGM_FIRE_B"},
    {"InformationObserverIce", true, "BGM_FIRST_MORPH_A", "BGM_ICE_B"},
    {"InformationObserverFlying", true, "BGM_FIRST_MORPH_A", "BGM_FLYING_B"},
    {"InformationObserverInvincible", true, "BGM_FIRST_MORPH_A", "BGM_MUTEKI_B"},
    {"InformationObserverLifeUpMario", false, nullptr, nullptr},
    {"InformationObserverOneUp", false, nullptr, nullptr},
    {"InformationObserverSpin", false, nullptr, nullptr},
};

namespace {
    static s32 sDisplayFramesMin = 30;
};

namespace {
    InformationObserver* getInformationObserver() {
        return MR::getSceneObj<InformationObserver>(SceneObj_InformationObserver);
    }
};

namespace NrvInformationObserver {
    NEW_NERVE(InformationObserverNrvWait, InformationObserver, Wait);
    NEW_NERVE(InformationObserverNrvDisp, InformationObserver, Disp);
};

InformationObserver::InformationObserver() :
    LiveActor("初出監視"),
    mType(Type_Bee),
    _90(false),
    mDisplayFrame(-1)
{}

void InformationObserver::init(const JMapInfoIter& rIter) {
    MR::connectToSceneLayoutMovement(this);
    MR::declareEventCameraProgrammable("初出監視カメラ");
    initNerve(&NrvInformationObserver::InformationObserverNrvWait::sInstance);
    MR::invalidateClipping(this);
    makeActorDead();
}

void InformationObserver::entry(TYPE type, LiveActor* pParam2) {
    _94 = pParam2;
    mType = type;
    _90 = MR::isTimeKeepDemoActive();

    if (_90) {
        MR::pauseTimeKeepDemo(_94);
        setNerve(&NrvInformationObserver::InformationObserverNrvDisp::sInstance);
    }
    else {
        MR::requestStartDemoWithoutCinemaFrame(
            this,
            "初出表示",
            &NrvInformationObserver::InformationObserverNrvDisp::sInstance,
            &NrvInformationObserver::InformationObserverNrvWait::sInstance);
    }

    makeActorAppeared();
}

void InformationObserver::exeWait() {
    
}

void InformationObserver::exeDisp() {
    if (MR::isFirstStep(this)) {
        if (mType == Type_LifeUp && MR::isPlayerLuigi()) {
            MR::appearInformationMessage(MR::getLayoutMessageDirect("InformationObserverLifeUpLuigi"), true);
        }
        else {
            MR::appearInformationMessage(MR::getLayoutMessageDirect(sAttr[mType].mMessageId), true);
        }

        if (sAttr[mType].mFirstMorphBgmName != nullptr) {
            MR::stopSubBGM(0);
            MR::startSubBGM(sAttr[mType].mFirstMorphBgmName, false);
        }

        mDisplayFrame = sDisplayFramesMin;
    }

    if (mDisplayFrame >= 0) {
        mDisplayFrame--;
    }

    if (!MR::testCorePadTriggerA(0) || mDisplayFrame >= 0) {
        return;
    }

    if (_90) {
        MR::resumeTimeKeepDemo(_94);
    }
    else {
        MR::endDemo(this, "初出表示");
    }

    if (sAttr[mType].mBgmName != nullptr) {
        if (mType == Type_Flying) {
            MR::stopSubBGM(5);
            MR::startStageBGM(sAttr[mType].mBgmName, false);
        }
        else {
            MR::startSubBGM(sAttr[mType].mBgmName, false);
        }
    }

    MR::startSystemSE("SE_SY_TALK_OK", -1, -1);
    MR::disappearInformationMessage();

    switch (mType) {
    case Type_Bee:
        MR::onGameEventFlagBeeMarioAtFirst();
        break;
    case Type_Teresa:
        MR::onGameEventFlagTeresaMarioAtFirst();
        break;
    case Type_Hopper:
        MR::onGameEventFlagHopperMarioAtFirst();
        break;
    case Type_Fire:
        MR::onGameEventFlagFireMarioAtFirst();
        break;
    case Type_Ice:
        MR::onGameEventFlagIceMarioAtFirst();
        break;
    case Type_Flying:
        MR::onGameEventFlagFlyingMarioAtFirst();
        break;
    case Type_Invincible:
        MR::onGameEventFlagInvincibleMarioAtFirst();
        break;
    case Type_LifeUp:
        MR::onGameEventFlagLifeUpAtFirst();
        break;
    case Type_OneUp:
        MR::onGameEventFlagOneUpAtFirst();
        break;
    case Type_Spin:
        MR::onGameEventFlagEnableToSpinAndStarPointer();
        break;
    }

    makeActorDead();
}

namespace InformationObserverFunction {
    void explainBee() {
        getInformationObserver()->entry(InformationObserver::Type_Bee, nullptr);
    }

    void explainTeresa() {
        getInformationObserver()->entry(InformationObserver::Type_Teresa, nullptr);
    }

    void explainHopper() {
        getInformationObserver()->entry(InformationObserver::Type_Hopper, nullptr);
    }

    void explainFire() {
        getInformationObserver()->entry(InformationObserver::Type_Fire, nullptr);
    }

    void explainIce() {
        getInformationObserver()->entry(InformationObserver::Type_Ice, nullptr);
    }

    void explainFlying() {
        getInformationObserver()->entry(InformationObserver::Type_Flying, nullptr);
    }

    void explainInvincible() {
        getInformationObserver()->entry(InformationObserver::Type_Invincible, nullptr);
    }

    void explainLifeUp() {
        getInformationObserver()->entry(InformationObserver::Type_LifeUp, nullptr);
    }

    void explainOneUp() {
        getInformationObserver()->entry(InformationObserver::Type_OneUp, nullptr);
    }

    void explainSpin(LiveActor* pParam1) {
        getInformationObserver()->entry(InformationObserver::Type_Spin, pParam1);
    }
};
