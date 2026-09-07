#include "Game/MapObj/GCaptureTarget.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/GCapture.hpp"
#include "Game/MapObj/MapPartsRailMover.hpp"
#include "Game/Util/ActorCameraUtil.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/Functor.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "revolution/types.h"

void GCaptureTarget_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)0.5f;
}

namespace NrvGCaptureTarget {
    NEW_NERVE(GCaptureTargetNrvTryDemoAppear, GCaptureTarget, TryDemoAppear);
    NEW_NERVE(GCaptureTargetNrvAppear, GCaptureTarget, Appear);
    NEW_NERVE(GCaptureTargetNrvWait, GCaptureTarget, Wait);
    NEW_NERVE(GCaptureTargetNrvPointable, GCaptureTarget, Pointable);
    NEW_NERVE(GCaptureTargetNrvHitPointer, GCaptureTarget, HitPointer);
    NEW_NERVE(GCaptureTargetNrvActive, GCaptureTarget, Active);
};  // namespace NrvGCaptureTarget

GCaptureTarget::GCaptureTarget(const char* pName)
    : LiveActor(pName), mRailMover(), mStarAnimSpeed(1.0f), mCameraInfo(), mReleaseDistance(-1.0f), mPointableRange(2500.0f), mFarAwayColor(true) {
    MR::createGCapture();
}

void GCaptureTarget::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("GCaptureTarget", nullptr, false);
    MR::connectToSceneNoSilhouettedMapObjStrongLight(this);
    MR::getJMapInfoArg0WithInit(rIter, &mReleaseDistance);
    MR::getJMapInfoArg1NoInit(rIter, &mPointableRange);
    initEffectKeeper(0, nullptr, false);
    initSound(4, false);
    initNerve(&NrvGCaptureTarget::GCaptureTargetNrvWait::sInstance);
    MR::initShadowVolumeSphere(this, 50.0f);
    MR::setShadowDropLength(this, nullptr, 300.0f);
    MR::onCalcShadowOneTime(this, nullptr);
    MR::calcGravity(this);

    if (MR::isConnectedWithRail(rIter)) {
        initHitSensor(1);
        MR::addHitSensorMapObj(this, "body", 8, 0.0f, TVec3f(0.0f, 0.0f, 0.0f));
        MR::invalidateHitSensor(this, "body");
        initRailRider(rIter);
        mRailMover = new MapPartsRailMover(this);
        mRailMover->init(rIter);
    }

    MR::initStarPointerTarget(this, 200.0f, TVec3f(0.0f, 0.0f, 0.0f));

    if (MR::tryRegisterDemoCast(this, rIter)) {
        makeActorDead();
    } else if (MR::useStageSwitchReadAppear(this, rIter)) {
        MR::joinToGroupArray(this, rIter, "ブルースター出現グループ", 32);
        MR::syncStageSwitchAppear(this);
        MR::initActorCamera(this, rIter, &mCameraInfo);
        makeActorDead();
    } else {
        makeActorAppeared();
    }

    if (MR::useStageSwitchReadB(this, rIter)) {
        MR::listenStageSwitchOnB(this, MR::Functor_Inline(this, &GCaptureTarget::exeActive));
    }

    s32 arg;
    MR::getJMapInfoArg2WithInit(rIter, &arg);
    mFarAwayColor = arg != 0;
    MR::startBck(this, "Wait", nullptr);
    MR::startBrk(this, "Switch");
    MR::setBrkFrameAndStop(this, 1.0f);
}

void GCaptureTarget::initAfterPlacement() {
}

void GCaptureTarget::appear() {
    LiveActor::appear();
    MR::invalidateClipping(this);
    MR::deleteEffectAll(this);
    MR::hideModel(this);
    MR::startSystemSE("SE_SY_GCAPTURE_APPEAR");
    setNerve(&NrvGCaptureTarget::GCaptureTargetNrvAppear::sInstance);

    if (MR::isExistActorCamera(mCameraInfo)) {
        MR::requestStartDemo(this, "出現", &NrvGCaptureTarget::GCaptureTargetNrvAppear::sInstance,
                             &NrvGCaptureTarget::GCaptureTargetNrvTryDemoAppear::sInstance);
    }
}

void GCaptureTarget::makeActorAppeared() {
    LiveActor::makeActorAppeared();
    MR::emitEffect(this, "TargetLight");
    setNerve(&NrvGCaptureTarget::GCaptureTargetNrvWait::sInstance);

    if (mRailMover != nullptr) {
        mRailMover->start();
    }
}

void GCaptureTarget::makeActorDead() {
    if (mRailMover != nullptr) {
        mRailMover->end();
    }

    LiveActor::makeActorDead();
    MR::deleteEffectAll(this);
    MR::unrequestGCaptureTarget(this);
}

void GCaptureTarget::startClipped() {
    LiveActor::startClipped();
    MR::deleteEffectAll(this);
}

void GCaptureTarget::endClipped() {
    LiveActor::endClipped();
    emitNerveEffect();
}

void GCaptureTarget::control() {
    if (mRailMover != nullptr) {
        mRailMover->movement();
        mPosition.set(mRailMover->_28);
    }
}

void GCaptureTarget::exeTryDemoAppear() {
}

void GCaptureTarget::exeAppear() {
    if (MR::isFirstStep(this)) {
        if (MR::isExistActorCamera(mCameraInfo)) {
            MR::startActorCameraTargetSelf(this, mCameraInfo, 30);
            MR::callRequestMovementOnAllGroupMember(this);
        }
        MR::setBrkFrameAndStop(this, 1.0f);
    }

    if (MR::isStep(this, 30)) {
        emitNerveEffect();
        MR::showModel(this);
        MR::startBck(this, "Appear", nullptr);
        MR::startSound(this, "SE_OJ_GCAPTURE_APPEAR");
    }

    if (MR::isBckStopped(this)) {
        MR::startBck(this, "Wait", nullptr);
    }

    if (MR::isGreaterStep(this, 150)) {
        if (MR::isExistActorCamera(mCameraInfo)) {
            MR::endDemoWaitCameraInterpolating(this, "出現");
            MR::endActorCamera(this, mCameraInfo, false, -1);
        }
        setNerve(&NrvGCaptureTarget::GCaptureTargetNrvWait::sInstance);
    }
}

void GCaptureTarget::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::validateClipping(this);
        emitNerveEffect();

        f32 frame;
        if (mFarAwayColor) {
            frame = 0.0f;
        } else {
            frame = 1.0f;
        }

        MR::setBrkFrameAndStop(this, frame);
    }

    mStarAnimSpeed -= 0.05f;
    if (mStarAnimSpeed < 0.0f) {
        mStarAnimSpeed = 0.0f;
    }
    MR::setBckRate(this, mStarAnimSpeed);

    if (MR::isNearPlayer(this, mPointableRange)) {
        setNerve(&NrvGCaptureTarget::GCaptureTargetNrvPointable::sInstance);
    }
}

void GCaptureTarget::exePointable() {
    if (MR::isFirstStep(this)) {
        emitNerveEffect();
        MR::setBrkFrameAndStop(this, 1.0f);
    }

    if (!MR::isJudgedToClipFrustum(mPosition, 50.0f)) {
        MR::noticeInTouchableRange();
    }

    mStarAnimSpeed += 0.05f;
    if (mStarAnimSpeed > 0.5f) {
        mStarAnimSpeed = 0.5f;
    }
    MR::setBckRate(this, mStarAnimSpeed);

    if (MR::isStarPointerPointing(this, 0, true, "弱") && MR::requestGCaptureTarget(this)) {
        MR::invalidateClipping(this);
        setNerve(&NrvGCaptureTarget::GCaptureTargetNrvHitPointer::sInstance);
    } else if (!MR::isNearPlayer(this, mPointableRange)) {
        setNerve(&NrvGCaptureTarget::GCaptureTargetNrvWait::sInstance);
    }
}

void GCaptureTarget::exeHitPointer() {
    if (MR::isFirstStep(this)) {
        MR::setBrkFrameAndStop(this, 1.0f);
    }

    if (!MR::isRequestedGCaptureTarget(this)) {
        setNerve(&NrvGCaptureTarget::GCaptureTargetNrvWait::sInstance);
    } else {
        MR::requestStarPointerModeBlueStarReady(this);
        if (MR::isFirstStep(this)) {
            emitNerveEffect();
        }

        if (MR::isStep(this, 3)) {
            MR::startCSSound("CS_STAR_POWER", nullptr, 0);
        }

        MR::noticeInTouchableRange();
        MR::startLevelSound(this, "SE_OJ_LV_MAGIC_PNT_G_POINT");

        mStarAnimSpeed += 0.2f;
        if (mStarAnimSpeed > 1.0f) {
            mStarAnimSpeed = 1.0f;
        }
        MR::setBckRate(this, mStarAnimSpeed);

        bool isNear = MR::isNearPlayer(this, mPointableRange);
        if (MR::isStarPointerPointing(this, 0, true, "弱") && isNear) {
            MR::requestGCaptureTarget(this);
        }

        if (!isNear) {
            MR::unrequestGCaptureTarget(this);
        }
    }
}

void GCaptureTarget::exeActive() {
    if (MR::isFirstStep(this)) {
        emitNerveEffect();
        MR::startBck(this, "React", nullptr);
        MR::startSound(this, "SE_OJ_MAGIC_PNT_G_ON");
        MR::setBrkFrameAndStop(this, 1.0f);
    }

    MR::noticeInTouchableRange();
    MR::startLevelSound(this, "SE_OJ_LV_MAGIC_PNT_G_PULL", MR::calcDistanceToPlayer(this));
    MR::startLevelSound(this, "SE_OJ_LV_MAGIC_PNT_G_POINT");
}

void GCaptureTarget::decidedTarget() {
    if (!isNerve(&NrvGCaptureTarget::GCaptureTargetNrvActive::sInstance)) {
        setNerve(&NrvGCaptureTarget::GCaptureTargetNrvActive::sInstance);
    }
}

void GCaptureTarget::releasedTarget() {
    if (!isNerve(&NrvGCaptureTarget::GCaptureTargetNrvWait::sInstance)) {
        MR::startBck(this, "Wait", nullptr);
        setNerve(&NrvGCaptureTarget::GCaptureTargetNrvWait::sInstance);
    }
}

void GCaptureTarget::emitNerveEffect() {
    if (isNerve(&NrvGCaptureTarget::GCaptureTargetNrvAppear::sInstance)) {
        MR::emitEffect(this, "TargetLight");
        MR::deleteEffect(this, "Touch");
        MR::deleteEffect(this, "Active");
    } else if (isNerve(&NrvGCaptureTarget::GCaptureTargetNrvWait::sInstance)) {
        MR::deleteEffect(this, "TargetLight");
        MR::deleteEffect(this, "Touch");
        MR::deleteEffect(this, "Active");
    } else if (isNerve(&NrvGCaptureTarget::GCaptureTargetNrvPointable::sInstance)) {
        MR::emitEffect(this, "TargetLight");
        MR::emitEffect(this, "TouchAble");
        MR::deleteEffect(this, "Touch");
        MR::deleteEffect(this, "Active");
    } else if (isNerve(&NrvGCaptureTarget::GCaptureTargetNrvHitPointer::sInstance)) {
        MR::emitEffect(this, "TargetLight");
        MR::emitEffect(this, "Touch");
        MR::deleteEffect(this, "Active");
    } else if (isNerve(&NrvGCaptureTarget::GCaptureTargetNrvActive::sInstance)) {
        MR::emitEffect(this, "TargetLight");
        MR::deleteEffect(this, "Touch");
        MR::emitEffect(this, "Active");
    }
}

bool GCaptureTarget::isReleaseForce() const {
    return MR::isDead(this);
}

void GCaptureTarget::getTargetPosition(TVec3f* a1) {
    a1->set(mPosition);
}

f32 GCaptureTarget::releaseDistance() const {
    return mReleaseDistance;
}

f32 GCaptureTarget::getPointableRange() const {
    return mPointableRange;
}
