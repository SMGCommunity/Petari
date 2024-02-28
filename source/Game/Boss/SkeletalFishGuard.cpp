#include "Game/Boss/SkeletalFishGuard.hpp"
#include "Game/Boss/SkeletalFishBoss.hpp"
#include <JSystem/JMath.hpp>

void SkeletalFishGuard::init(const JMapInfoIter &rIter) {
    initModelManagerWithAnm("SkeletalFishGuard", nullptr, false);
    MR::addToAttributeGroupSearchTurtle(this);
    MR::connectToSceneEnemy(this);
    initNerve(&::SkeletalFishGuardNrvWait::sInstance);
    initSensor();
    MR::initLightCtrl(this);
    MR::initShadowVolumeFlatModel(this, "SkeletalFishGuardShadow");
    initEffectKeeper(1, nullptr, false);
    MR::addEffect(this, "PointerTouchManual");
    MR::setEffectHostMtx(this, "PointerTouchManual", MR::getJointMtx(this, "Head"));
    initSound(6, false);
    initBinder(120.0f, 0.0f, 0);
    MR::invalidateClipping(this);
    MR::setBinderExceptActor(this, mFishBoss->getHeadActor());
    MR::initStarPointerTargetAtJoint(this, "Joint00", 170.0f, TVec3f(0.0f, 0.0f, 0.0f));
    MR::declareStarPiece(this, 128);
}

void SkeletalFishGuard::initAfterPlacement() {
    calcTransAndFront();
    makeActorDead();
}

void SkeletalFishGuard::appear() {
    LiveActor::appear();
    MR::validateShadow(this, nullptr);
}

// SkeletalFishGuard::control

void SkeletalFishGuard::exeAppear() {
    if (MR::isFirstStep(this)) {
        MR::showModel(this);
        MR::startBck(this, "Swim", nullptr);
        MR::startBrk(this, "Base");
        MR::startSound(this, "SE_BM_SKL_GUARD_APPEAR", -1, -1);
        MR::validateHitSensors(this);
        MR::clearHitSensors(this);
        MR::offBind(this);
        _C8 = 0.0f;
        TVec3f v27;
        JGeometry::negateInternal((f32*)&mGravity, (f32*)&v27);
        _D0.set(v27);
        _10C.set(mPosition);
        f32 v2 = MR::getRandom(-1.0f, 1.0f);
        f32 v3 = MR::getRandom(-1.0f, 1.0f);
        f32 v4 = MR::getRandom(-1.0f, 1.0f);
        _DC.set(v4, v3, v2);
    }

    TVec3f v32;
    MR::calcGravityVector(this, _10C, &v32, nullptr, false);
    TVec3f v26(v32);
    v26.scale(1000.0f);
    TVec3f v31(_10C);
    JMathInlineVEC::PSVECSubtract(v31.toCVec(), v26.toCVec(), v31.toVec());
    s32 v5 = getNerveStep();
    calcTarget(&_E8, &_F4, &_100, 300 - v5);
    TVec3f v30(_DC);
    f32 v6 = v32.dot(v30);
    JMAVECScaleAdd(v32.toCVec(), v30.toCVec(), v30.toVec(), -v6);
    MR::normalizeOrZero(&v30);
    TVec3f v25(v30);
    v25.scale(500.0f);
    JMathInlineVEC::PSVECAdd(v31.toCVec(), v25.toCVec(), v31.toVec());
    TVec3f v24(0.0f, 1.0f, 0.0f);
    JMathInlineVEC::PSVECAdd(v30.toCVec(), v24.toCVec(), v30.toVec());
    MR::normalize(&v30);
    TVec3f v29(mPosition);

    if (getNerveStep() < 99) {
        s32 v7 = getNerveStep();
        TVec3f v21(0.0f, 1.0f, 0.0f);
        TVec3f v22(v21);
        v22.scale(0.0f);
        TVec3f v23(v22);
        v23.scale(v7);
        TVec3f v19(v30);
        v19.scale(10.0f);
        TVec3f v20(v19);
        v20.scale(v7);
        mPosition.cubic<f32>(_10C, v23, v20, v31, v7);
    }
    else {
        f32 v9 = getNerveStep() - (const int)0x63;
        f32 v11 = MR::getRailCoordSpeed(mFishBoss->getCurrentBossRail());
        TVec3f v17(v30);
        v17.scale(10.0f);
        TVec3f v18(v17);
        v18.scale(v9);
        TVec3f v15(_100);
        v15.scale(v11);
        TVec3f v16(v15);
        v16.scale(v9);
        mPosition.cubic<f32>(v31, v18, v16, _F4, v9);
    }

    TVec3f v28(mPosition);
    JMathInlineVEC::PSVECSubtract(v28.toCVec(), v29.toCVec(), v28.toVec());

    if (!MR::isNearZero(v28, 0.001f)) {
        _D0.setInlinePS(v28);
        MR::normalize(&v28);
    }

    MR::calcGravity(this);
    MR::startLevelSound(this, "SE_BM_LV_SKL_GUARD_SWIM_NORMAL", -1, -1, -1);
    MR::setNerveAtStep(this, &::SkeletalFishGuardNrvNormal::sInstance, 300);
}

void SkeletalFishGuard::exeNormal() {
    if (MR::isFirstStep(this)) {
        MR::onBind(this);
    }

    calcTransAndFront();
    MR::calcGravity(this);
    MR::startLevelSound(this, "SE_BM_LV_SKL_GUARD_SWIM_NORMAL", -1, -1, -1);
    tryShiftApart();
}

// SkeletalFishGuard::exeApart
// SkeletalFishGuard::exeFollow
// SkeletalFishGuard::exeStraight

void SkeletalFishGuard::exeKill() {
    if (MR::isFirstStep(this)) {
        MR::invalidateHitSensors(this);
        MR::invalidateShadow(this, nullptr);
        MR::startBck(this, "Break", nullptr);
        MR::startSound(this, "SE_BM_SKL_GUARD_EXPLODE", -1, -1);
    }

    if (MR::isBckOneTimeAndStopped(this)) {
        MR::hideModel(this);
        kill();
    }
}

void SkeletalFishGuard::exeNumb() {
    if (MR::isFirstStep(this)) {
        MR::startDPDHitSound();
        MR::stopBck(this);
        mEffectKeeper->forceDeleteEmitterAll();
        MR::emitEffectWithScale(this, "PointerTouchManual", 1.5f, -1);
        mScaleController->startDpdHitVibration();
    }

    MR::startDPDFreezeLevelSound(this);

    if (!MR::isStarPointerPointing2POnPressButton(this, "弱", true, false)) {
        setNerve(_CC);
    }
}

void SkeletalFishGuard::appearNaturally() {
    appear();
    setNerve(&::SkeletalFishGuardNrvWait::sInstance);
    MR::hideModel(this);
}

void SkeletalFishGuard::appearForce() {
    makeActorAppeared();
    MR::startSound(this, "SE_BM_SKL_GUARD_APPEAR", -1, -1);
    setNerve(&::SkeletalFishGuardNrvNormal::sInstance);
    MR::showModel(this);
}

void SkeletalFishGuard::killNaturally() {
    if (!MR::isDead(this)) {
        if (isNerve(&::SkeletalFishGuardNrvWait::sInstance)) {
            makeActorDead();
        }
        else if (!isNerve(&::SkeletalFishGuardNrvKill::sInstance)) {
            setNerve(&::SkeletalFishGuardNrvKill::sInstance);
        }
    }
}

void SkeletalFishGuard::waitAttack(s32 time) {
    mAttackDelay = time;
}

void SkeletalFishGuard::calcAndSetBaseMtx() {
    TVec3f stack_64;
    stack_64.multPS(mScale, mScaleController->_C);
    MR::setBaseScale(this, stack_64);

    if (isNerve(&::SkeletalFishGuardNrvAppear::sInstance)) {
        TVec3f gravityVec;
        MR::calcGravityVector(this, _F4, &gravityVec, nullptr, 0);
        JGeometry::negateInternal((f32*)&gravityVec, (f32*)&gravityVec);
        TVec3f stack_4C;
        PSVECCrossProduct(gravityVec.toCVec(), _100.toCVec(), stack_4C.toVec());
        MR::normalize(&stack_4C);
        PSVECCrossProduct(_100.toCVec(), stack_4C.toCVec(), gravityVec.toVec());
        TQuat4f quat;
        quat.setRotate(_100, _D0);
        quat.transform(gravityVec);
        quat.transform(stack_4C);
        TPos3f stack_D0;
        stack_D0.setXDir(stack_4C);
        stack_D0.setYDir(gravityVec);
        stack_D0.setZDir(_D0);
        stack_D0.setTrans(mPosition);
        MR::setBaseTRMtx(this, stack_D0);
    }
    else {
        TVec3f stack_30;
        TVec3f stack_24;
        JGeometry::negateInternal((f32*)&mScale, (f32*)&stack_30);
        PSVECCrossProduct(stack_30.toCVec(), _D0.toCVec(), stack_24.toVec());

        if (MR::isNearZero(stack_24, 0.001f)) {
            MtxPtr mtx = getBaseMtx();
            TPos3f stack_A0;
            f32 x, y, z;
            stack_A0.setInline_2(mtx);
            x = stack_A0.mMtx[0][0];
            y = stack_A0.mMtx[1][0];
            z = stack_A0.mMtx[2][0];
            
            stack_24.set(x, y, z);
            z = stack_A0.mMtx[2][1];
            y = stack_A0.mMtx[1][1];
            x = stack_A0.mMtx[0][1];
            stack_30.set(x, y, z);
            TVec3f stack_18;
            z = stack_A0.mMtx[2][2];
            y = stack_A0.mMtx[1][2];
            x = stack_A0.mMtx[0][2];
            stack_18.set(x, y, z);
            TQuat4f stack_8;
            stack_8.setRotate(stack_18, _D0);
            stack_8.transform(stack_24);
            stack_8.transform(stack_30);
        }
        else {
            MR::normalize(&stack_24);
            PSVECCrossProduct(_D0.toCVec(), stack_24.toCVec(), stack_30.toVec());
        }

        TPos3f stack_70;
        stack_70.setXDir(stack_24);
        stack_70.setYDir(stack_30);
        stack_70.setZDir(_D0);
        stack_70.setTrans(mPosition);
        MR::setBaseTRMtx(this, stack_70);
    }
}

void SkeletalFishGuard::exeWait() {
    MR::setNerveAtStep(this, &::SkeletalFishGuardNrvAppear::sInstance, _A0);
}

// SkeletalFishGuard::attackSensor
// SkeletalFishGuard::receiveMsgPlayerAttack

void SkeletalFishGuard::initSensor() {
    initHitSensor(2);
    MR::addHitSensorAtJointEnemy(this, "head", "Joint00", 16, 120.0f, TVec3f(0.0f, 0.0f, 0.0f));
    MR::addHitSensorAtJointEnemy(this, "body", "Joint02", 16, 120.0f, TVec3f(0.0f, 0.0f, 0.0f));
}

// SkeletalFishGuard::calcTransAndFront
// SkeletalFishGuard::rotateHorizontal
// SkeletalFishGuard::rotateVertical
// SkeletalFishGuard::tryShiftApart
// SkeletalFishGuard::tryShiftStraight
// SkeletalFishGuard::tryShiftKill
// SkeletalFishGuard::turn
// SkeletalFishGuard::lookToPlayer
// SkeletalFishGuard::calcTarget
// SkeletalFishGuard::isInScreen
// SkeletalFishGuard::isPlayerInAttackRange
// SkeletalFishGuard::isLineOfSightClear
// SkeletalFishGuard::tryShiftNumb

SkeletalFishGuard::~SkeletalFishGuard() {

}

namespace {
    INIT_NERVE(SkeletalFishGuardNrvWait);
    INIT_NERVE(SkeletalFishGuardNrvAppear);
    INIT_NERVE(SkeletalFishGuardNrvNormal);
    INIT_NERVE(SkeletalFishGuardNrvApart);
    INIT_NERVE(SkeletalFishGuardNrvFollow);
    INIT_NERVE(SkeletalFishGuardNrvStraight);
    INIT_NERVE(SkeletalFishGuardNrvDefence);
    INIT_NERVE(SkeletalFishGuardNrvKill);
    INIT_NERVE(SkeletalFishGuardNrvNumb);
};