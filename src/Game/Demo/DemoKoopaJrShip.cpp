#include "Game/Demo/DemoKoopaJrShip.hpp"
#include "Game/Demo/DemoFunction.hpp"
#include "Game/LiveActor/ActorCameraInfo.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/NPC/KoopaJr.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/JointUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

namespace {
    struct Anim {
        /* 0x00 */ const char* mEntryAnimName;
        /* 0x04 */ const char* mLeaveAnimName;
    };

    static const TVec3f sKoopaJrPos = TVec3f(135.0f, 188.0f, 0.0f);
    static const char* sPartName = "クッパＪｒ．デモ";
    static const char* sJointNameKoopaJrPos = "obj";
    static const s32 sBgmStartStep = 309;
    static const Anim sAnim[] = {
        {
            "KoopaJrEntryTriPodDemo",
            "KoopaJrLeaveTriPodDemo",
        },
        {
            "KoopaJrEntryOtaKingDemo",
            "KoopaJrLeaveOtaKingDemo",
        },
    };

    NEW_NERVE(DemoKoopaJrShipNrvAppear, DemoKoopaJrShip, Appear);
    NEW_NERVE(DemoKoopaJrShipNrvTalk, DemoKoopaJrShip, Talk);
    NEW_NERVE(DemoKoopaJrShipNrvFlyAway, DemoKoopaJrShip, FlyAway);
};  // namespace

DemoKoopaJrShip::DemoKoopaJrShip(const char* pName) : LiveActor(pName), mKoopaJrObj(nullptr), mAnimCameraIndex(-1) {}

void DemoKoopaJrShip::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("KoopaJrShip", nullptr, false);
    initAnimID(rIter);
    MR::connectToSceneNpc(this);
    initEffectKeeper(0, nullptr, false);
    initSound(8, false);
    MR::invalidateClipping(this);
    createKoopaJrObj(rIter);
    MR::tryRegisterDemoCast(this, rIter);
    initNerve(&::DemoKoopaJrShipNrvAppear::sInstance);
    makeActorDead();
}

void DemoKoopaJrShip::appear() {
    LiveActor::appear();
    setNerve(&::DemoKoopaJrShipNrvAppear::sInstance);
    mKoopaJrObj->mPosition.set(sKoopaJrPos);
}

void DemoKoopaJrShip::exeAppear() {
    if (MR::isFirstStep(this)) {
        mKoopaJrObj->appear();
        mKoopaJrObj->setStateShipBattleAppear();
        startEntryAnim();
        MR::stopStageBGM(60);
        MR::startSound(this, "SE_BM_KOOPAJR_SHIP_ENTER", -1, -1);
        MR::hideModel(this);
    }

    if (MR::isGreaterEqualStep(this, 1) && MR::isHiddenModel(this)) {
        MR::showModel(this);
    }

    if (MR::isStep(this, ::sBgmStartStep)) {
        MR::startSubBGM("BGM_KOOPA_JR_APPEAR", false);
    }

    MR::startLevelSound(this, "SE_BM_LV_KOOPAJR_SHIP_MOVE", -1, -1, -1);

    if (MR::isBckStopped(this)) {
        setNerve(&::DemoKoopaJrShipNrvTalk::sInstance);
    } else if (tryDemoEnd()) {
    }
}

void DemoKoopaJrShip::exeTalk() {
    if (MR::isFirstStep(this)) {
        mKoopaJrObj->startShipBattleTalk();
        MR::startBck(this, "Wait", nullptr);
    }

    MR::startLevelSound(this, "SE_BM_LV_KOOPAJR_SHIP_MOVE", -1, -1, -1);

    if (!DemoFunction::isPauseTimeKeepDemo(this)) {
        setNerve(&::DemoKoopaJrShipNrvFlyAway::sInstance);
    } else if (tryDemoEnd()) {
    }
}

void DemoKoopaJrShip::exeFlyAway() {
    if (MR::isFirstStep(this)) {
        MR::startSound(mKoopaJrObj, "SE_BV_KOOPAJR_LAUGH_LEAVE", -1, -1);
        MR::stopSubBGM(120);
        startLeaveAnim();
    }

    MR::startLevelSound(this, "SE_BM_LV_KOOPAJR_SHIP_MOVE", -1, -1, -1);

    if (tryDemoEnd()) {
        if (mAnimCameraIndex >= 0) {
            ActorCameraInfo cameraInfo = ActorCameraInfo(-1, 0);
        }

        mKoopaJrObj->kill();
        kill();
    }
}

void DemoKoopaJrShip::control() {
    TMtx34f jointMtx;
    jointMtx.setInline(MR::getJointMtx(this, ::sJointNameKoopaJrPos));

    MR::faceToPoint(jointMtx, *MR::getPlayerPos(), 5.0f);

    TVec3f pos;
    pos.setPS2(::sKoopaJrPos);

    jointMtx.mult(pos, mKoopaJrObj->mPosition);
}

void DemoKoopaJrShip::initAnimID(const JMapInfoIter& rIter) {
    MR::getJMapInfoArg0NoInit(rIter, &mAnimCameraIndex);

    if (mAnimCameraIndex >= sizeof(sAnim) / sizeof(*sAnim)) {
        mAnimCameraIndex = -1;
    }

    if (mAnimCameraIndex >= 0) {
        ActorCameraInfo cameraInfo = ActorCameraInfo(-1, 0);

        MR::initAnimCamera(this, &cameraInfo, ::sAnim[mAnimCameraIndex].mEntryAnimName);
        MR::initAnimCamera(this, &cameraInfo, ::sAnim[mAnimCameraIndex].mLeaveAnimName);
    }
}

void DemoKoopaJrShip::createKoopaJrObj(const JMapInfoIter& rIter) {
    mKoopaJrObj = new KoopaJr("クッパJr");
    mKoopaJrObj->init(rIter);
    mKoopaJrObj->makeActorDead();
}

bool DemoKoopaJrShip::tryDemoEnd() {
    return MR::isDemoPartLastStep(::sPartName);
}

void DemoKoopaJrShip::startEntryAnim() {
    if (mAnimCameraIndex == -1) {
        MR::startBck(this, "Arrival", nullptr);
    } else {
        MR::startBck(this, ::sAnim[mAnimCameraIndex].mEntryAnimName, nullptr);

        ActorCameraInfo cameraInfo = ActorCameraInfo(-1, 0);
        MR::startAnimCameraTargetSelf(this, &cameraInfo, ::sAnim[mAnimCameraIndex].mEntryAnimName, 0, 1.0f);
    }
}

void DemoKoopaJrShip::startLeaveAnim() {
    if (mAnimCameraIndex == -1) {
        MR::startBck(this, "Arrival", nullptr);
        MR::setBckFrame(this, MR::getBckCtrl(this)->mEndFrame - 1.0f);
        MR::setBckRate(this, -1.0f);
    } else {
        MR::startBck(this, ::sAnim[mAnimCameraIndex].mLeaveAnimName, nullptr);

        ActorCameraInfo cameraInfo = ActorCameraInfo(-1, 0);
        MR::startAnimCameraTargetSelf(this, &cameraInfo, ::sAnim[mAnimCameraIndex].mLeaveAnimName, 0, 1.0f);
    }
}
