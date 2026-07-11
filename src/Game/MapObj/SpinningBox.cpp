#include "Game/MapObj/SpinningBox.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MapUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"
#include "JSystem/JGeometry/TUtil.hpp"

namespace {
    static const f32 hForceBindOffset = 65.0f;
    static const f32 hForceBindMul = 0.5f;
    static const f32 sShadowSize = 160.0f;
    // static const _32 hSpinningTime = ;
    static const f32 hGravityVel = 3.0f;
    static const f32 hVelHMin = 10.0f;
    static const f32 hReflectRate = 0.5f;
    static const f32 hSpinningAttackStrongVel = 10.0f;
    static const f32 hStopCheckVel = 0.1f;
    static const f32 hGroundCosine = 0.4f;
    static const s32 hWaitTime = 30;
    // static const _32 hKickableFrontDotCheck =
    static const f32 hPlayerIsSideDotCheck = 0.25f;
    static const f32 hSpinningAngle = 2.0f;
    static const f32 hDampVelHWait = 0.6f;
    static const f32 hDampVelVWait = 0.6f;
    static const f32 hTurnYDirRatio = 0.125f;
    // static const _32 hHitVelocity =
    // static const _32 hSpinningKind =
    // static const _32 hScVelCheck =
    static const f32 hPointedRotDamp = 0.8f;
    // static const _32 hWalkAngle =
    // static const _32 hWalkVelocity =
    static const f32 hSlidingVelocitySub = 0.08f;
    static const f32 hSlidingVelMax = 17.0f;
    static const f32 hSlidingRotateSub = 0.01f;
    static const f32 hSlidingRotVelMin = 0.02f;
    static const f32 hSlidingRotVelMax = 0.02f;
    static const s32 hKickableTime = 30;
    static const f32 hSpinningVelocitySub = 0.01f;
    static const f32 hSpinningVelMin = 0.0f;
    static const f32 hSpinningVelMax = 25.0f;
    static const f32 hSpinningRotateSub = 0.001f;
    static const f32 hSpinningRotVelMin = 0.1f;
    static const f32 hSpinningRotVelMax = 0.5f;
    static const f32 hPlayerVelocityCheck = 10.0f;
    static const f32 hIceInitBlowVelV = 100.0f;
    static const f32 hKickCheckOffsetUp = 0.0f;
    static const f32 hIceInitBlowVelH = 0.0f;
};  // namespace

namespace NrvSpinningBox {
    NEW_NERVE(HostTypeNrvFall, SpinningBox, Fall);
    NEW_NERVE_ONEND(HostTypeNrvWait, SpinningBox, Wait, Wait);
    NEW_NERVE_ONEND(HostTypeNrvSliding, SpinningBox, Sliding, Sliding);
    NEW_NERVE_ONEND(HostTypeNrvSpinning, SpinningBox, Spinning, Spinning);
    NEW_NERVE_ONEND(HostTypeNrvPointed, SpinningBox, Pointed, Pointed);
};  // namespace NrvSpinningBox

void FORCE_MATCH_SDATA2() {
    f32 f1 = 1.0f;
    f32 f2 = 0.0f;
    f32 f3 = JGeometry::TUtil< f32 >::epsilon();
    f32 f4 = 0.5f;
    f32 f5 = 3.0f;
    f32 f6 = 2.0f;
}

SpinningBox::SpinningBox(const char* pName)
    : LiveActor(pName), mRotate(0.0f, 0.0f, 0.0f, 1.0f), mRotateSpeed(0.0f), mSlideSpeed(0.0f), mSlideDir(1.0f, 0.0f, 0.0f), mIsOnGround(false),
      mIsIceBox(false), mSpinAngle(0.0f) {
}

void SpinningBox::init(const JMapInfoIter& rIter) {
    if (MR::isValidInfo(rIter)) {
        MR::initDefaultPos(this, rIter);
    }

    if (mIsIceBox) {
        initModelManagerWithAnm("IceBox", nullptr, false);
    } else {
        initModelManagerWithAnm("SpinningBox", nullptr, false);
    }

    MR::connectToSceneMapObjStrongLight(this);
    initNerve(&NrvSpinningBox::HostTypeNrvFall::sInstance);

    f32 scale = getScale();
    initHitSensor(2);
    MR::addHitSensorMapObj(this, "body", 32, scale * 160.0f, TVec3f(0.0f, -40.0f, 0.0f));

    if (mIsIceBox) {
        MR::initCollisionParts(this, "IceBox", getSensor("body"), nullptr);
        setName("アイスボックス");
    } else {
        MR::initCollisionPartsAutoEqualScaleOne(this, "SpinningBox", getSensor("body"), nullptr);
    }

    initBinder(140.0f, 80.0f, 0);
    MR::setBinderExceptActor(this, this);
    initEffectKeeper(0, "SpinningBox", false);
    initSound(6, false);

    MR::onCalcGravity(this);
    MR::initShadowVolumeBox(this, TVec3f(::sShadowSize, -70.0f, ::sShadowSize) * scale, getBaseMtx());
    MR::excludeCalcShadowToMyCollision(this, nullptr);

    if (MR::isValidInfo(rIter)) {
        MR::setGroupClipping(this, rIter, 32);
    }

    MR::addToAttributeGroupReflectSpinningBox(this);
    MR::initStarPointerTarget(this, 110.0f, TVec3f(0.0f, 80.0f, 0.0f));
    MR::declareStarPiece(this, 3);
    if (mIsIceBox) {
        makeActorDead();
    } else {
        MR::startBrk(this, "SpinningBox");
        makeActorAppeared();
    }
}

void SpinningBox::kill() {
    MR::startSound(this, "SE_OJ_SPIN_BOX_BREAK");
    MR::startSound(this, "SE_OJ_STAR_PIECE_BURST");
    LiveActor::kill();
    MR::appearStarPiece(this, mPosition - mGravity * 50.0f, MR::getDeclareRemnantStarPieceCount(this), 10.0f, 40.0f, false);
    MR::emitEffect(this, "Break");
}

void SpinningBox::control() {
    if (MR::isInDeath(this, TVec3f(0.0f, 0.0f, 0.0f)) || MR::isInWater(mPosition)) {
        kill();
        return;
    }

    if (MR::isBindedDamageElectric(this)) {
        kill();
        return;
    }

    TVec3f slideDir = mSlideDir;
    TVec3f norm;
    if (MR::isBindedGround(this)) {
        norm.set(*MR::getGroundNormal(this));
        calcPlanarProjectedVec(&slideDir, norm, mGravity);
    } else {
        norm.set(-mGravity);
        MR::vecKillElement(slideDir, norm, &slideDir);
    }

    MR::normalizeOrZero(&slideDir);
    if (!MR::isNearZero(slideDir)) {
        mSlideDir.set(slideDir);
    }

    TVec3f up;
    mRotate.getYDir(up);
    TQuat4f rotY;
    rotY.setRotate(up, norm, ::hTurnYDirRatio);
    mRotate.mult(rotY);
    mRotate.getYDir(up);
    rotY.setRotate(up, -mRotateSpeed);
    mRotate.mult(rotY);
    mRotate.normalize();

    if (MR::isOnPlayer(getSensor("body")) && !isNerve(&NrvSpinningBox::HostTypeNrvWait::sInstance)) {
        setNerve(&NrvSpinningBox::HostTypeNrvWait::sInstance);
    }
}

void SpinningBox::exeFall() {
    if (MR::isFirstStep(this)) {
        MR::invalidateClipping(this);
    }

    MR::reboundVelocityFromCollision(this, 0.0f, 0.0f, 1.0f);
    MR::applyVelocityDampAndGravity(this, ::hGravityVel, ::hDampVelHWait, ::hDampVelHWait, ::hDampVelVWait, ::hVelHMin);

    if (MR::isOnGround(this) && MR::isGreaterStep(this, ::hWaitTime)) {
        setNerve(&NrvSpinningBox::HostTypeNrvWait::sInstance);
    }
}

void SpinningBox::exeWait() {
    if (MR::isFirstStep(this)) {
        mIsOnGround = false;
        mRotateSpeed = 0.0f;
        MR::validateClipping(this);
    }

    if (MR::isStarPointerPointing2POnPressButton(this, "弱", true, false)) {
        setNerve(&NrvSpinningBox::HostTypeNrvPointed::sInstance);
        return;
    }

    TVec3f originalGravity = mGravity;
    if (MR::isOnGround(this)) {
        mIsOnGround = true;
        mGravity.set(-*MR::getGroundNormal(this));
    }

    MR::reboundVelocityFromCollision(this, 0.0f, 0.0f, 1.0f);

    if (mIsOnGround) {
        MR::applyVelocityDampAndGravity(this, ::hGravityVel, 0.0f, 0.0f, ::hDampVelVWait, ::hVelHMin);
    } else {
        MR::applyVelocityDampAndGravity(this, ::hGravityVel, ::hDampVelHWait, ::hDampVelHWait, ::hDampVelVWait, ::hVelHMin);
    }

    mGravity.set(originalGravity);  // "gravity" was only used to update velocity, revert to initial
}

void SpinningBox::endWait() {
}

void SpinningBox::exeSliding() {
    if (MR::isFirstStep(this)) {
        mRotateSpeed = ::hSlidingRotVelMax;
        MR::invalidateClipping(this);
    }

    if (MR::isStarPointerPointing2POnPressButton(this, "弱", true, false)) {
        setNerve(&NrvSpinningBox::HostTypeNrvPointed::sInstance);
        return;
    }

    mRotateSpeed = MR::clamp(mRotateSpeed - ::hSlidingRotateSub, ::hSlidingRotVelMin, ::hSlidingRotVelMax);
    mSlideSpeed = MR::clamp(mSlideSpeed - ::hSlidingVelocitySub, 0.0f, ::hSlidingVelMax);
    f32 slideSoundLvl = (mSlideSpeed / ::hSlidingVelMax) * 100.0f;
    if (slideSoundLvl > 100.0f) {
        slideSoundLvl = 100.0f;
    }
    MR::startLevelSound(this, "SE_OJ_LV_SPIN_BOX_SLIDE", slideSoundLvl);

    if (mSlideSpeed <= ::hStopCheckVel) {
        setNerve(&NrvSpinningBox::HostTypeNrvWait::sInstance);
        return;
    }

    MR::reboundVelocityFromCollision(this, 0.0f, 0.0f, 1.0f);

    MR::vecKillElement(mVelocity, mSlideDir, &mVelocity);
    mVelocity = mSlideDir * mSlideSpeed;

    if (!MR::isOnGround(this)) {
        MR::forceBindOnGround(this, ::hForceBindOffset, ::hForceBindMul);
    }

    TVec3f norm;
    if (MR::isBindedWallOrSlopeGround(this, ::hGroundCosine, &norm)) {
        MR::startSound(this, "SE_OJ_SPIN_BOX_BOUND");
        MR::calcReflectionVector(&mSlideDir, norm, 1.0f, 0.0f);
        mSlideSpeed *= ::hReflectRate;
        MR::normalizeOrZero(&mSlideDir);
    }
}

void SpinningBox::endSliding() {
    MR::deleteEffect(this, "Blur");
}

void SpinningBox::exeSpinning() {
    if (MR::isFirstStep(this)) {
        MR::emitEffect(this, "Blur");
        mSpinAngle = 0.0f;
        MR::invalidateClipping(this);
    }

    if (MR::isStarPointerPointing2POnPressButton(this, "弱", true, false)) {
        setNerve(&NrvSpinningBox::HostTypeNrvPointed::sInstance);
        return;
    }

    mRotateSpeed = MR::clamp(mRotateSpeed - ::hSpinningRotateSub, ::hSpinningRotVelMin, ::hSpinningRotVelMax);
    mSlideSpeed = MR::clamp(mSlideSpeed - ::hSpinningVelocitySub, ::hSpinningVelMin, ::hSpinningVelMax);
    mSpinAngle += mRotateSpeed;

    if (mSpinAngle > ::hSpinningAngle) {
        while (mSpinAngle > ::hSpinningAngle) {
            mSpinAngle -= ::hSpinningAngle;
        }

        f32 spinSoundLvl = (mRotateSpeed / ::hSpinningRotVelMax) * 100.0f;
        if (spinSoundLvl > 100.0f) {
            spinSoundLvl = 100.0f;
        }
        MR::startSound(this, "SE_OJ_SPIN_BOX_SPIN", spinSoundLvl);
    }

    f32 slideSoundLvl = (mSlideSpeed / ::hSlidingVelMax) * 100.0f;
    if (slideSoundLvl > 100.0f) {
        slideSoundLvl = 100.0f;
    }
    MR::startLevelSound(this, "SE_OJ_LV_SPIN_BOX_SLIDE", slideSoundLvl);

    if (mSlideSpeed <= ::hStopCheckVel) {
        setNerve(&NrvSpinningBox::HostTypeNrvWait::sInstance);
        return;
    }

    MR::reboundVelocityFromCollision(this, 0.0f, 0.0f, 1.0f);

    MR::vecKillElement(mVelocity, mSlideDir, &mVelocity);
    mVelocity = mSlideDir * mSlideSpeed;

    if (!MR::isOnGround(this)) {
        MR::forceBindOnGround(this, ::hForceBindOffset, ::hForceBindMul);
    }

    if (MR::isBindedWallOrSlopeGround(this, ::hGroundCosine, nullptr)) {
        kill();
    }
}

void SpinningBox::endSpinning() {
    MR::deleteEffect(this, "Blur");
}

void SpinningBox::exePointed() {
    if (MR::isFirstStep(this)) {
        // TODO: mVelocity.zero();
        mVelocity.z = 0.0f;
        mVelocity.y = 0.0f;
        mVelocity.x = 0.0f;
        MR::startDPDHitSound();
        MR::emitEffect(this, "Touch");
        MR::invalidateClipping(this);
    }

    mRotateSpeed *= ::hPointedRotDamp;
    MR::startDPDFreezeLevelSound(this);

    if (!MR::isStarPointerPointing2POnPressButton(this, "弱", true, false)) {
        setNerve(&NrvSpinningBox::HostTypeNrvWait::sInstance);
    }
}

void SpinningBox::endPointed() {
    MR::deleteEffect(this, "Touch");
}

void SpinningBox::calcPlanarProjectedVec(TVec3f* pProj, const TVec3f& rNorm, const TVec3f& rGrav) {
    pProj->add(rGrav * (-rNorm.dot(*pProj) / rNorm.dot(rGrav)));
}

void SpinningBox::calcHitDirection(const TVec3f& rPos) {
    TVec3f hitDir = mPosition - rPos;
    MR::vecKillElement(hitDir, mGravity, &hitDir);
    MR::normalizeOrZero(&hitDir);

    if (MR::isNearZero(hitDir)) {
        return;
    }

    TVec3f velDir = mVelocity;
    MR::normalizeOrZero(&velDir);
    velDir.add(hitDir);
    MR::normalizeOrZero(&velDir);
    if (MR::isNearZero(velDir)) {
        mSlideDir = hitDir;
    } else {
        mSlideDir = velDir;
    }
}

void SpinningBox::generateIceBox(HitSensor* pSender, HitSensor* pReceiver) {
    appear();
    mPosition.set(pReceiver->mHost->mPosition);
    TPos3f mtx = pReceiver->mHost->getBaseMtx();
    TVec3f up, front;
    mtx.getYDirInline(up);
    mtx.getZDirInline(front);

    MR::makeQuatFromVec(&mRotate, front, up);

    TVec3f grav = pReceiver->mHost->mGravity;
    TVec3f dir = pReceiver->mPosition - pSender->mPosition;
    MR::vecKillElement(dir, grav, &dir);
    dir.setLength(::hIceInitBlowVelH);
    dir.add((-grav) * (::hIceInitBlowVelV));
    mVelocity.set(dir);

    if (::hPlayerVelocityCheck < MR::getPlayerVelocity()->length()) {
        calcHitDirection(pSender->mPosition);
        setNerve(&NrvSpinningBox::HostTypeNrvSpinning::sInstance);
    } else {
        setNerve(&NrvSpinningBox::HostTypeNrvFall::sInstance);
    }
}

void SpinningBox::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (pReceiver->isType(ATYPE_BEGOMAN)) {
        MR::sendMsgEnemyAttack(pReceiver, pSender);
        return;
    }

    if (MR::isSensorPlayer(pReceiver)) {
        if (MR::isPlayerExistSide(this, ::hKickCheckOffsetUp, ::hPlayerIsSideDotCheck) && !MR::isOnPlayer(pSender)) {
            if (isNerve(&NrvSpinningBox::HostTypeNrvSpinning::sInstance)) {
                if (MR::isPlayerSwingAction() || !MR::isOnGroundPlayer()) {
                    return;
                }

                bool hit;
                if (::hSpinningAttackStrongVel < mSlideSpeed) {
                    hit = MR::sendMsgEnemyAttackStrong(pReceiver, pSender);
                } else {
                    hit = MR::sendMsgEnemyAttackFlipWeak(pReceiver, pSender);
                }

                if (hit) {
                    hitReflection(pReceiver, pSender);
                } else {
                    MR::sendMsgPush(pReceiver, pSender);
                }

            } else if (isNerve(&NrvSpinningBox::HostTypeNrvSliding::sInstance)) {
                MR::sendMsgPush(pReceiver, pSender);
            }
        }
        return;
    }

    if (MR::isSensorMapObj(pReceiver) || MR::isSensorEnemy(pReceiver)) {
        if (isNerve(&NrvSpinningBox::HostTypeNrvSpinning::sInstance) || isNerve(&NrvSpinningBox::HostTypeNrvSliding::sInstance)) {
            if (MR::sendMsgToEnemyAttackBlow(pReceiver, pSender)) {
                if (!MR::isExistInAttributeGroupReflectSpinningBox(pReceiver->mHost)) {
                    kill();
                }
            }
        }
    }
}

bool SpinningBox::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgInhaleBlackHole(msg)) {
        kill();
        return true;
    }

    if (MR::isMsgPlayerKick(msg) && MR::isSensorMapObj(pReceiver)) {
        if (isNerve(&NrvSpinningBox::HostTypeNrvSpinning::sInstance)) {
            return false;
        }

        if (isNerve(&NrvSpinningBox::HostTypeNrvSliding::sInstance) && MR::isLessStep(this, ::hKickableTime)) {
            return false;
        }

        if (MR::isOnPlayer(getSensor("body"))) {
            return false;
        }

        if (MR::isPlayerExistSide(this, ::hKickCheckOffsetUp, ::hPlayerIsSideDotCheck)) {
            if (isNerve(&NrvSpinningBox::HostTypeNrvPointed::sInstance)) {
                return false;
            }

            MR::getPlayerFrontVec(&mSlideDir);
            mSlideSpeed = ::hSlidingVelMax;
            setNerve(&NrvSpinningBox::HostTypeNrvSliding::sInstance);
            return true;
        }
    }

    return false;
}

bool SpinningBox::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorEnemyAttack(pReceiver)) {
        return false;
    }

    if (MR::isMsgPlayerHipDrop(msg)) {
        kill();
        return true;
    }

    if (MR::isMsgPlayerHitAll(msg)) {
        if (isNerve(&NrvSpinningBox::HostTypeNrvFall::sInstance) || MR::isOnPlayer(getSensor("body"))) {
            return false;
        }

        MR::getPlayerFrontVec(&mSlideDir);
        MR::vecKillElement(mSlideDir, mGravity, &mSlideDir);
        MR::normalizeOrZero(&mSlideDir);
        mRotateSpeed = ::hSpinningRotVelMax;
        mSlideSpeed = ::hSpinningVelMax;
        MR::startSpinHitSound(this);
        MR::startBlowHitSound(this);
        setNerve(&NrvSpinningBox::HostTypeNrvSpinning::sInstance);
        return true;
    }

    return false;
}

bool SpinningBox::receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isExistInAttributeGroupReflectSpinningBox(pSender->mHost)) {
        if (!isNerve(&NrvSpinningBox::HostTypeNrvSpinning::sInstance) && !isNerve(&NrvSpinningBox::HostTypeNrvSliding::sInstance)) {
            TVec3f dir = mPosition - pSender->mPosition;
            MR::normalizeOrZero(&dir);
            MR::vecKillElement(pSender->mHost->mVelocity, dir, &dir);
            MR::separateScalarAndDirection(&mSlideSpeed, &mSlideDir, dir);
            mSlideSpeed *= ::hReflectRate;
            setNerve(&NrvSpinningBox::HostTypeNrvSliding::sInstance);
        } else {
            hitReflection(pSender, pReceiver);
        }
        return true;
    }

    return false;
}

void SpinningBox::hitReflection(HitSensor* pSender, HitSensor* pReceiver) {
    TVec3f dir = pReceiver->mPosition - pSender->mPosition;
    MR::normalizeOrZero(&dir);
    if (MR::isNearZero(dir)) {
        return;
    }

    MR::calcReflectionVector(&mSlideDir, dir, 1.0f, 0.0f);
    mSlideSpeed = ::hReflectRate;  // BUG, this should be *=, not =
    MR::normalizeOrZero(&mSlideDir);
}

void SpinningBox::calcAndSetBaseMtx() {
    TPos3f mtx;
    mtx.makeQuat(mRotate);
    mtx.setTrans(mPosition);
    MR::setBaseTRMtx(this, mtx);
}

void DUMMY() {
    // This dummy is here to ensure that TVec3f::sub is the deepest call in the operator- chain.
    // Presumably one of the stripped functions would have used sub directly.
    TVec3f a, b;
    a.sub(b);
}
