#include "Game/Player/MarioSwim.hpp"

#include "Game/AreaObj/AreaObj.hpp"
#include "Game/AreaObj/WaterArea.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/LiveActor.hpp"

#include "Game/Map/HitInfo.hpp"
#include "Game/Map/WaterInfo.hpp"

#include "Game/MapObj/DashRing.hpp"

#include "Game/Player/MarineSnow.hpp"
#include "Game/Player/MarioActor.hpp"
#include "Game/Player/MarioAnimator.hpp"
#include "Game/Player/MarioConst.hpp"
#include "Game/Player/MarioModule.hpp"

#include "Game/Screen/GameSceneLayoutHolder.hpp"

#include "Game/Util/AreaObjUtil.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/SceneUtil.hpp"

#include "JSystem/JGeometry/TVec.hpp"
#include "math_types.hpp"
#include "revolution/types.h"
#include <cmath>
#include <cstddef>

static const f32 cAnimSpeeds[] = {1.0f, 1.0f, 1.0f, 1.0f};
namespace {
    static f32 cFrontAcc[] = {-0.2f, -0.2f, -0.2f, -0.1f, -0.1f, -0.1f, -0.1f, -0.1f, -0.0f, -0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                              0.0f,  0.0f,  0.0f,  0.2f,  0.2f,  0.2f,  0.3f,  0.3f,  0.4f,  0.5f,  0.5f, 0.5f, 0.5f, 0.5f,
                              0.4f,  0.4f,  0.3f,  0.3f,  0.2f,  0.2f,  0.1f,  0.1f,  0.1f,  0.1f,  0.1f, 0.0f};

    static f32 cFrontAccSpin[] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 8.0f,  6.0f,  4.0f,  2.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,
                                  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f};

    static f32 cFrontAccSpinSurface[] = {0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  8.0f,  6.0f,  4.0f,  2.0f,  1.0f,
                                         -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f};
    static f32 cWeightTable[] = {
        1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    };

    static f32 cSwimWeightTable[] = {0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};
    static f32 cWeightTableSP[] = {0.0f, 0.0f, 0.75f, 0.25f};
    static f32 cLimitAngleSink = JGeometry::TUtil< float >::PI() / 1.0001f;
    static f32 cNeutralAngleWait = (JGeometry::TUtil< float >::PI() / 2.0f) - (JGeometry::TUtil< float >::PI() / 6.0f);
    static f32 cLimitAngleWait = JGeometry::TUtil< float >::PI() / 1.5f;
    static f32 cUpperAngleWait = JGeometry::TUtil< float >::PI() / 100.0f;
    static f32 cWaterSurfaceRange = 160.0f;
    static f32 cTurnMotionSpeed = 5.0f;

}  // namespace

inline f32 get100Deg() {
    return DEG_TO_RAD(100);
}

static inline f32 getTwoPi() {
    return 6.28318548203f;
}

static inline f32 funConversions1(u32 a, u16 tmp) {
    return a * getTwoPi() / tmp;
}

static inline f32 funVecTime(const MarioSwim& self) {
    return self.getTrans().y - self.mWaterDepth;
}

static inline f32 funCalcTime() {
    f32 tmp = 0.0f;  // This line is included only to fix a register allocation mismatch
    return 40.0f;
}

inline f32 procAngle(f32 l, f32 r) {
    return l * (1.0f - r);
}

// possibly related to other calculations that take similar form? (eg getSwimValue)
inline f32 calculate(f32 stick) {
    return cLimitAngleSink + (cLimitAngleWait - cLimitAngleSink) * stick;
}

static inline TVec3f createAndScale(f32 scalar, const TVec3f& v) {
    TVec3f ret(v);
    ret.scale(scalar);
    return ret;
}

f32 getSwimValue(f32 stick, u32 index, const MarioConstTable* table) {
    if (index == 2) {
        return table->mSwimRotXIne;
    }
    if (index == 3) {
        return table->mSwimRotZIne;
    }
    return 0.0f;
}

bool Mario::isSwimming() const {
    if (isStatusActive(6)) {
        return true;
    }
    return isStatusActive(24);
}

bool Mario::forceStartSwimAndShoot(const TVec3f& rKnockbackVec) {
    WaterInfo waterInfo;
    if (!MR::getWaterAreaObj(&waterInfo, mActor->mPosition)) {
        return false;
    }
    startSwim();
    mSwim->setDamage(rKnockbackVec, 30);
    return true;
}

bool Mario::forceExitSwim() {
    if (!isStatusActive(6)) {
        return false;
    }
    mSwim->mNextAction = MarioSwim::EXIT_ACTION_SURFACE;
    closeStatus(mSwim);
    return true;
}

void MarioSwim::setDamage(const TVec3f& rKnockbackVec, u16 damage) {
    mKnockbackVel = rKnockbackVec;
    mKnockbackTimer = damage;
    mStickInputYInertia = 0.0f;
    mStickInputXInertia = 0.0f;
    mForwardSpeed = 0.0f;
    mSpinDashTimer = 0;
    mSpinTimer = 0;
    mJumpDiveTimer = 0;
    mSinkTimer = 0;
    mZSinkTimer = 0;
    mActionLockTimer = damage;
    mSwimYAccTimer = 0;
    mStateTimer = 0x12C;
    if (mIsOnSurface) {
        mIsOnSurface = false;
    }
}

bool Mario::checkStartSwim() {
    if (isStatusActive(6)) {
        return false;
    }
    if (isStatusActive(29)) {
        return false;
    }
    if (_10._7 && getMovementStates()._1) {
        getAnimator()->waterToGround();
        _10._7 = false;
    }
    if (mSwim->checkWaterCube(false)) {
        if (getPlayerMode() == 3) {
            addTrans((-*getGravityVec()).scaleInline(100.0f), nullptr);
            if (getPlayer()->getMovementStates().jumping) {
                stopJump();
            }
            if (getPlayer()->isStatusActive(1)) {
                getPlayer()->closeStatus(0);
            }
            if (getPlayer()->isStatusActive(4)) {
                getPlayer()->closeStatus(0);
            }
            return false;
        }
        startSwim();
        return true;
    }
    mSwim->updateUnderWater();
    return false;
}

void Mario::startSwim() {
    if (!isStatusActive(6)) {
        if (getPlayer()->isDamaging()) {
            mSwim->_9D = 4;
            playSound("水落下突入", -1);
        } else if (getPlayer()->getMovementStates().jumping) {
            mSwim->_9D = 1;
            playSound("水落下突入", -1);
        } else {
            mSwim->_9D = 0;
            playSound("水歩行突入", -1);
        }

        changeStatus(mSwim);
        clearSlope();
        stopWalk();
        stopJump();
        _735 = 0;

        mMovementStates._3E = 0;
        mMovementStates._12 = false;
        if (mMovementStates._8 || mMovementStates._1A || mMovementStates._19) {
            addVelocity(getWallNorm(), 50.0f);
        }

        getPlayer()->lockGroundCheck(mSwim, true);
        checkBaseTransBall();
        mActor->_F44 = false;
    }
}

MarioSwim::MarioSwim(MarioActor* pActor) : MarioState(pActor, 6), mWaterInfo() {
    _18 = 0;
    mIsOnSurface = false;
    mEnteredWater = false;
    _1B = false;
    _1C = false;
    _1D = false;
    _1E = 0;
    _1F = false;
    mIsSwimmingAtSurface = false;
    mIsSinking = false;
    _22 = false;
    mStateTimer = 0;
    mSurfacePaddleHoldTimer = 0;
    mActionLockTimer = 0;
    mSinkTimer = 0;
    mSwimYAccTimer = 0;
    mZSinkTimer = 0;
    mJumpDiveTimer = 0;
    mDashTimer = 0;
    _38 = 0;
    mBuoyancyDelayTimer = 0;
    mIdleWaitTimer = 0;
    mWaveFlowTimer = 0;
    mJetCooldownTimer = 0;
    mFloorContactTimer = 0;
    _44 = 0;
    mSurfaceOffset = 0.0f;
    mStickInputYInertia = 0.0f;
    mStickInputXInertia = 0.0f;
    mForwardSpeed = 0.0f;
    mBuoyancySpeed = 0.0f;
    mVerticalAngle = 0.0f;

    mFrontVec.x = 1.0f;
    mFrontVec.y = 0.0f;
    mFrontVec.z = 0.0f;

    mUpVec.x = 0.0f;
    mUpVec.y = 1.0f;
    mUpVec.z = 0.0f;

    _78 = false;
    mSpinTimer = 0;
    mSpinDashTimer = 0;
    _80 = 0.0f;
    _84 = 0.0f;
    _88 = 0;
    mJetTimer = 0;
    mRingDashTimer = 0;
    mRingDashChargeTimer = 0;
    mRingDashMaxDuration = 0;
    mRingDashSpeedScale = 0.0f;
    mBlurOffset = 0.0f;
    _9C = false;
    _9D = 0;
    mNextAction = EXIT_ACTION_NONE;
    mEffectState = 0xFF;
    mKnockbackVel.zero();
    mDamageType = 0;
    _AD = 0;
    mKnockbackTimer = 0;
    PSMTXIdentity(mUpperBodyJointMtx.toMtxPtr());
    mDrownTimer = 0;
    mCurrentTiltX = 0.0f;
    mCurrentTiltY = 0.0f;
    mOxygenWarningTimer = 0;
    mOxygenDecreaseDelay = 0x78;
    mColdWaterDamageInterval = 0;
    mSwimState = SWIM_STATE_NONE;
    _148.zero();
    mAreaFollowMtxPos.zero();
    mSurfacePos.set(0.0f, 1.0f, 0.0f);
    mSurfaceNorm.zero();
    _178.zero();
    mWaterStreamVel.zero();
    mWaterStreamDir.zero();
    mWallStickTimer = 0;
    mWaterDepth = 0.0f;
    _1A0 = 0.0f;
    mDistToFloor = 0.0f;
    _1A8 = 0.0f;
    mDistanceToWaterSurface = 0.0f;
    _1B2 = false;
    mWaterDistanceTarget = 0.0f;
    _1B8 = 0.0f;
    mMarineSnow = new MarineSnow();
}

void MarioSwim::init() {
    mOxygen = mActor->getConst().getTable()->mOxygenMax;
}

bool MarioSwim::start() {
    mSpinDashTimer = 0;
    mStickInputYInertia = 0.0f;
    mStickInputXInertia = 0.0f;
    mForwardSpeed = 0.0f;
    mSpinTimer = 0;
    mDrownTimer = 0;
    mOxygen = mActor->getConst().getTable()->mOxygenMax;
    mOxygenDecreaseDelay = 0x78;
    mColdWaterDamageInterval = 0;
    mDamageType = 0;
    _44 = 0;
    mOxygenWarningTimer = 180;
    mFloorContactTimer = 0;
    mNextAction = EXIT_ACTION_NONE;
    mRingDashTimer = 0;
    mRingDashChargeTimer = 0;
    mBlurOffset = 0.0f;
    mIdleWaitTimer = 0;
    _80 = 0.0f;
    mEffectState = 0xFF;
    mKnockbackTimer = 0;
    mKnockbackVel.zero();
    mSurfacePaddleHoldTimer = 0;
    mCurrentTiltX = 0.0f;
    mCurrentTiltY = 0.0f;
    mWallStickTimer = 0;
    mStateTimer = 0;
    mSurfaceOffset = 0.0f;
    mWaveFlowTimer = 0;
    mWaterStreamVel.zero();
    mWaterStreamDir.zero();
    _1B2 = false;
    mWaterDistanceTarget = 0.0f;
    _1F = false;
    _22 = false;
    mBuoyancySpeed = 0.0f;

    if (!getPlayer()->getMovementStates().jumping || getPlayer()->mVerticalSpeed <= 100.0f) {
        mBuoyancySpeed = mActor->getConst().getTable()->mSwimUpMaxSpeed;
    }

    _9C = true;
    if (checkLvlA()) {
        _9C = false;
    }

    mVerticalAngle = DEG_TO_RAD(30);
    onSurface();
    if ((mWaterDepth < -200.0f || mActor->_F74) && mIsOnSurface) {
        mIsOnSurface = false;
    }

    _1B = false;
    u32 r1e = 0, r1d = 0;
    _1D = false;
    _1E = 0;
    if (isAnimationRun("飛び込みジャンプ")) {
        r1e = 1;
    }
    if (isAnimationRun("後方飛び込みジャンプ")) {
        r1e = 1;
        getPlayer()->setFrontVecKeepUp(-getFrontVec(), -1.0f);
    }
    if (!mActor->_468) {
        mJetTimer = 0;
    }
    if (_9D == 4) {
        r1d = 1;
    }
    if (mJetTimer != 0 && getPlayer()->getMovementStates().jumping) {
        if (!getPlayer()->_10.digitalJump) {
            r1e = 2;
        } else {
            getPlayer()->mMovementStates.jumping = false;
        }
    }
    getPlayer()->_10.digitalJump = false;

    if (isAnimationRun("リングダッシュ")) {
        r1e = 2;
    }
    if (isAnimationRun("水泳スピン移動")) {
        r1e = 3;
    }
    if (isAnimationRun("水泳スピンジャンプ")) {
        r1e = 3;
    }

    mFrontVec = getFrontVec();
    MR::normalize(&mFrontVec);
    stopAnimationUpper(nullptr, nullptr);
    // Basic Swimming
    changeAnimation(nullptr, "水泳基本");
    changeAnimationNonStop("水泳基本");
    getPlayer()->mMovementStates._1 = false;
    f32 fr1f = mDistToFloor;

    mActionLockTimer = 0;
    mZSinkTimer = 0;
    mSinkTimer = 0;
    mJumpDiveTimer = 0;
    mDashTimer = 0;
    _38 = 0;
    mBuoyancyDelayTimer = 0;
    _1C = false;

    if (fr1f < -mWaterDepth) {
        MarioActor* actor = mActor;
        actor->emitEffectWaterColumn(-mSurfacePos, getTrans() - mSurfacePos.scaleInline(fr1f));
    } else {
        mActor->emitEffectWaterColumn(mSurfacePos, mSurfaceNorm);
    }
    if (getPlayer()->getMovementStates().jumping && getPlayer()->mVerticalSpeed > 100.0f || r1e) {
        TVec3f stack_44;
        mStateTimer = 0;
        Mario* mario = getPlayer();
        MR::vecKillElement(mario->mJumpVec, getGravityVec(), &stack_44);
        fr1f = stack_44.length();

        if (getFrontVec().dot(stack_44) < 0.0f) {
            fr1f = 0.0f;
        }
        fr1f *= 0.1f;
        fr1f = MR::clamp(fr1f, 0.0f, 1.0f);

        if (mIsOnSurface) {
            mIsOnSurface = false;
        }
        if (mSwimState == SWIM_STATE_SURFACE) {
            f32 ftmp = mActor->getConst().getTable()->mSwimFrontMaxSpeed;
            f32 ftmp2 = (0.2f + 0.1f * fr1f);
            mJumpDiveTimer = 0x3C;
            mForwardSpeed = ftmp2 * ftmp;
        } else if (getPlayer()->getMovementStates()._B || r1e) {
            mForwardSpeed = 10.0f;

            if (mJetTimer != 0) {
                changeAnimationNonStop("水泳ジェット");
            } else {
                changeAnimation("水泳ジャンプダイブ回転", static_cast< const char* >(nullptr));
            }
            playEffect("水面Ｚ沈降");

            switch (r1e) {
            case 0:
            case 1:
                mActionLockTimer = mActor->getConst().getTable()->mSwimSpecStartTimerA[0];
                mSinkTimer = mActor->getConst().getTable()->mSwimSpecStartTimerB[0];
                break;
            case 2:
                mActionLockTimer = mActor->getConst().getTable()->mSwimSpecStartTimerA[1];
                mSinkTimer = mActor->getConst().getTable()->mSwimSpecStartTimerB[1];
                break;
            case 3:
                mActionLockTimer = mActor->getConst().getTable()->mSwimSpecStartTimerA[2];
                mSinkTimer = mActor->getConst().getTable()->mSwimSpecStartTimerB[2];
                break;
            }

            // Possibly related to similar operations? (eg getSwimValue(), etc)
            f32 ftmp2 = cLimitAngleSink * (1.0f - fr1f);
            mVerticalAngle = fr1f * get100Deg() + ftmp2;
            TVec3f stack_38(mFrontVec);
            stack_38.y = -10.0f;
            MR::normalize(&stack_38);
            getPlayer()->forceSetHeadVecKeepSide(stack_38);
        } else if (r1d) {
            changeAnimation("水上ダメージ着水", static_cast< const char* >(nullptr));
            mSinkTimer = mActor->getConst().getTable()->mJumpDamageSinkTimer;
            mKnockbackTimer = 0x78;
        } else {
            mForwardSpeed = (0.2f + 0.1f * fr1f) * mActor->getConst().getTable()->mSwimFrontMaxSpeed;
            changeAnimation("水泳ジャンプダイブ", static_cast< const char* >(nullptr));

            mActor->setBlendMtxTimer(8);
            mActionLockTimer = 15;
            mSinkTimer = mActor->getConst().getTable()->mJumpSinkTimer;

            mIsSwimmingAtSurface = true;
            mVerticalAngle = 1.49599659443f;
            if (getPlayer()->mVerticalSpeed < 200.0f) {
                f32 ftmp = (getPlayer()->mVerticalSpeed / 200.0f);
                mActionLockTimer *= ftmp;
                mSinkTimer *= ftmp;
                mActor->setBlendMtxTimer(8.0f * ftmp);
            }
        }
    } else if (mIsOnSurface) {
        mKnockbackTimer = 0x5A;
        MR::vecKillElement(mActor->getLastMove(), mUpVec, &mKnockbackVel);
        mKnockbackVel.mult(0.5f);
    }
    if (mActor->_468) {
        if (mJetTimer == 0) {
            if (mActor->getCarrySensor()->isType(15) || mActor->getCarrySensor()->isType(16)) {
                u32 r1b = 0;
                if (mActor->getCarrySensor()->isType(16)) {
                    r1b = 1;
                }
                startJet(r1b);

                r1e = 2;
                mIsSwimmingAtSurface = false;
                mVerticalAngle = 2.09439516068f;

                if (mIsOnSurface) {
                    mIsOnSurface = false;
                }
            }
        } else {
            r1e = 2;
        }
    }
    if (r1e == 2) {
        if (!isAnimationRun("水泳ジェット")) {
            changeAnimation("水泳ジェット開始", "水泳ジェット");
        } else {
            changeAnimation(nullptr, "水泳ジェット");
        }
    }

    updateLifeByTime();
    if (!getPlayer()->_1C._3) {
        MR::getGameSceneLayoutHolder()->changeLifeMeterModeWater();
    }

    mNextAction = EXIT_ACTION_NONE;
    return true;
}

f32 MarioSwim::getSurface() const {
    f32 fr1f = funVecTime(*this);
    f32 fr1e = funCalcTime();
    f32 fr1d = 0.01f;
    f32 fr1c = 0.03f;
    u16 tmp = 1800;
    fr1d = 1.0f + fr1d * MR::sin(funConversions1(mStateTimer, tmp));
    tmp = 500;
    f32 ftmp = fr1c * MR::sin(funConversions1(mStateTimer, tmp));
    tmp = 180;

    // I love sin composition
    return fr1f + fr1d * (fr1e * (MR::sin(ftmp + funConversions1(mStateTimer, tmp)) - 1.0f));
}

bool MarioSwim::update() {
    bool r1e = false;
    mStateTimer++;
    if (_22) {
        mNextAction = EXIT_ACTION_SLIDE;
        return false;
    }
    if (!checkWaterCube(false)) {
        if (mNextAction == EXIT_ACTION_JUMP || mNextAction == EXIT_ACTION_SLIDE) {
            return false;
        }

        TVec3f stack_194;
        MR::vecKillElement(mSurfaceNorm - getTrans(), getAirGravityVec(), &stack_194);
        if (stack_194.length() > 100.0f || -mWaterDepth >= getPlayer()->mVerticalSpeed || mKnockbackTimer != 0 || mWallStickTimer != 0) {
            if (!isAnimationRun("水泳ジャンプダイブ")) {
                if (mJetTimer != 0) {
                    doJetJump(1);
                    return false;
                } else {
                    mNextAction = EXIT_ACTION_FALL;
                    return false;
                }
            }
        } else {
            mSwimState = SWIM_STATE_ENTERING;
        }
    }

    if (getPlayer()->mDrawStates._F && mWaterDepth > 0.963775992393f &&
        MR::diffAngleAbsHorizontal(-getPlayer380(), getWorldPadDir(), getPlayer()->getAirGravityVec()) < 0.196349546313f) {
        if (calcAngleD(getPlayer380()) >= 30.0f) {
            mNextAction = EXIT_ACTION_SLIDE;
            return false;
        } else {
            mNextAction = EXIT_ACTION_JUMP;
            return false;
        }
    }

    updateLifeByTime();
    if (_18) {
        onSurface();
        mSinkTimer = 0;
        mJumpDiveTimer = 0;
        mZSinkTimer = 0;
        mActionLockTimer = 0;
        mStickInputYInertia = 0.0f;
    }

    mUpVec = -getPlayer()->getAirGravityVec();
    f32 fTmp = getSurface() - 30.0f;
    if (mWallStickTimer != 0) {
        mWallStickTimer--;
    }

    if (mIsOnSurface || (mIsSwimmingAtSurface && checkTrgA())) {
        if (mSpinTimer != 0 || mSpinDashTimer != 0) {
            _9C = false;
        } else {
            if (mJetTimer != 0) {
                if (checkTrgA()) {
                    doJetJump(0);
                    return false;
                }

                if (getStickY() <= 0) {
                    mVerticalAngle = 0.00000381469726562f;
                    mStickInputYInertia = 1.0f;
                }
            } else {
                u32 tmp = 0x1E;
                // Surface Paddle
                if (isAnimationRun("水上一掻き")) {
                    tmp = 0x1E;
                }

                if (checkLvlA()) {
                    if (_9C && (!isAnimationRun("水上一掻き") || !mSurfacePaddleHoldTimer)) {
                        mSurfacePaddleHoldTimer++;
                    }
                    if ((mWallStickTimer != 0 || mJetTimer != 0 || getPlayer()->mMovementStates._8 || getPlayer()->mMovementStates._32) &&
                        checkTrgA()) {
                        mSurfacePaddleHoldTimer = tmp;
                    }
                } else {
                    _9C = true;
                    if (mSurfacePaddleHoldTimer && mSurfacePaddleHoldTimer < tmp) {
                        surfacePaddle();
                    }

                    mSurfacePaddleHoldTimer = 0;
                    _1C = false;
                }
                if (mActor->isRequestJump2P()) {
                    mSurfacePaddleHoldTimer = tmp;
                    getPlayer()->_1C._A = true;
                    getPlayer()->_10._1F = true;
                }

                bool tmp2 = false;
                if ((!mZSinkTimer || (mSpinTimer != 0 || mSpinDashTimer != 0)) && (mSurfacePaddleHoldTimer >= tmp || _1C)) {
                    tmp2 = true;
                }
                if (mActor->isRequestSpin()) {
                    tmp2 = true;
                }
                if (tmp2) {
                    if (mStateTimer > 6) {
                        f32 res = MR::clamp(mForwardSpeed / mActor->getConst().getTable()->mSwimToWalkSpd, 0.0f, 1.0f);
                        getPlayer()->mMovementStates._5 = false;
                        getPlayer()->_278 = res;
                        getPlayer()->tryJump();
                        mNextAction = EXIT_ACTION_POWER_JUMP;
                        return false;
                    }
                    _1C = true;
                }
            }
        }

        flowOnWave(fTmp);
    } else if (mWaterDepth > -30.0f) {
        if (!mActionLockTimer && !mZSinkTimer && !mSinkTimer && mJetTimer == 0) {
            getAnimator()->forceSetBlendWeight(cWeightTable);
            changeAnimation("炊泳上昇呼吸", static_cast< const char* >(nullptr));
        }

        if (!mSinkTimer) {
            onSurface();
            mActionLockTimer = 0;
        }

        if (mSpinDashTimer > 10) {
            f32 res = MR::clamp(mForwardSpeed / mActor->getConst().getTable()->mSwimToWalkSpd, 0.0f, 1.0f);
            getPlayer()->mMovementStates._5 = false;
            getPlayer()->_278 = res;

            TVec3f stack_188;
            mActor->getLastMove(&stack_188);
            f32 res2 = MR::vecKillElement(stack_188, getGravityVec(), &stack_188);
            if (res2 > -10.0f) {
                res2 = -10.0f;
            }

            stack_188 += getGravityVec().scaleInline(res2);
            getPlayer()->tryForcePowerJump(stack_188, false);
            mNextAction = EXIT_ACTION_POWER_JUMP;
            if (isAnimationRun("水泳スピン移動")) {
                changeAnimationNonStop("水泳スピン移動");
            } else {
                changeAnimationNonStop("水泳スピンジャンプ");
            }
            return false;
        }
        if (mJetTimer != 0 && (getStickY() < -0.1f || checkLvlA()) && getPlayer()->mVerticalSpeed - mWaterDepth > 200.0f) {
            doJetJump(0);
            return false;
        }
        if (mRingDashTimer != 0 && getPlayer()->mVerticalSpeed - mWaterDepth > 200.0f) {
            f32 res = MR::clamp(mForwardSpeed / mActor->getConst().getTable()->mSwimToWalkSpd, 0.0f, 1.0f);

            getPlayer()->mMovementStates._5 = false;
            getPlayer()->_278 = res;
            getPlayer()->tryForcePowerJump(mActor->getLastMove().scaleInline(2.0f).translate(mUpVec.scaleInline(10.0f)), false);

            mNextAction = EXIT_ACTION_POWER_JUMP;
            changeAnimationNonStop("リングダッシュ");
            return false;
        }
    } else {
        checkWaterBottom();
        if (getStickY() >= 0.0f && (_1F || (mDistToFloor < 60.0f && mForwardSpeed > 1.0f))) {
            const TVec3f& rShadowNorm = getPlayer()->getShadowNorm();
            TVec3f stack_17C;
            PSVECCrossProduct(&mFrontVec, &rShadowNorm, &stack_17C);
            TVec3f stack_170;
            PSVECCrossProduct(&rShadowNorm, &stack_17C, &stack_170);

            if (-mWaterDepth + mDistToFloor > 1000.0f) {
                mEnteredWater = true;
            }
            f32 dot1 = mFrontVec.dot(stack_170);
            f32 dot2 = mFrontVec.dot(rShadowNorm);
            if (dot2 >= 0.0f) {
                mVerticalAngle = DEG_TO_RAD(90) + 0.9f * marioAcos(dot1);
            } else if (mForwardSpeed > 1.0f) {
                mVerticalAngle = DEG_TO_RAD(90) - 0.5f * marioAcos(dot1);
            } else {
                f32 angle = marioAcos(dot1);
                _38 = 0x3C;
                mVerticalAngle = DEG_TO_RAD(90) - angle;
            }
            if (mJetTimer != 0 && getPlayer()->mVerticalSpeed < 100.0f) {
                mFloorContactTimer = 2;
                playEffect("水底接触");
                mForwardSpeed *= mActor->getConst().getTable()->mTurboReductionHitFloor;
            }
        }
        _1F = false;
    }
    if (mFloorContactTimer != 0) {
        playSound("水底接触", -1);
        if (--mFloorContactTimer == 0) {
            stopEffect("水底接触");
        }
    }
    if (mZSinkTimer != 0 && --mZSinkTimer == 0) {
        r1e = true;
        mVerticalAngle = mActor->getConst().getTable()->mZsinkAngleX;
        mActionLockTimer = mActor->getConst().getTable()->mZsinkMoveTimer;
        mSinkTimer = mActor->getConst().getTable()->mZsinkSinkTimer;
        mForwardSpeed = mActor->getConst().getTable()->mZsinkFrontSpeed;
        _38 = 0x14;
    }
    f32 fr1f = 1.0f - MR::clamp(mForwardSpeed / mActor->getConst().getTable()->mSwimFrontMaxSpeed, 0.0f, 1.0f);
    if (mDistToFloor - mWaterDepth < 400.0f) {
        fr1f = 0.0f;
    }
    if (_38 != 0) {
        _38--;
    } else {
        f32 res = fr1f;
        if (mIsOnSurface && !mZSinkTimer) {
            if (!_18) {
                if (mJetTimer != 0) {
                    if (getStickY() > 0.707000017166f) {
                        mVerticalAngle = mActor->getConst().getTable()->mZsinkAngleX;
                        mIsSwimmingAtSurface = false;
                        _1E = 0;
                    }

                    MarioConstTable* table = mActor->getConst().getTable();
                    f32 a = getStickY();
                    f32 b = table->mSwimRotXIneT;
                    mStickInputYInertia = mStickInputYInertia * b + procAngle(a, b);

                    mStickInputXInertia = 2.5f * (getStickX() * (1.0f - mActor->getConst().getTable()->mSwimRotZIneT));
                    f32 fr1c = 1.0f;
                    if (getStickY() < 0.0f) {
                        fr1c += 2.0f * -getStickY();
                    }
                    mStickInputXInertia *= fr1c;
                } else if (checkTrgZ() && !check7Aand7C()) {
                    stopAnimationUpper("水泳水面初期移動", nullptr);
                    changeAnimation("水泳潜り", static_cast< const char* >(nullptr));
                    playEffect("水面Ｚ沈降");
                    playSound("水面潜り", -1);
                    mZSinkTimer = mActor->getConst().getTable()->mZsinkStartTimer;
                } else if (!checkLvlZ()) {
                    res = 0.0f;
                } else {
                    MarioConstTable* table = mActor->getConst().getTable();
                    f32 a = getStickP();
                    f32 b = table->mSwimRotXIne;
                    mStickInputYInertia = mStickInputYInertia * b + procAngle(a, b);
                }
            }
            mStickInputXInertia *= mActor->getConst().getTable()->mSwimRotZIne;
        } else {
            if (mRingDashTimer == 0 && mDistToFloor > 200.0f && checkTrgZ() && !mZSinkTimer && !mSinkTimer && mJetTimer == 0 && !check7Aand7C()) {
                stopAnimation(nullptr, static_cast< const char* >(nullptr));
                changeAnimation("水泳潜り", static_cast< const char* >(nullptr));
                playSound("水中潜り", -1);
                playEffect("水面Ｚ沈降");
                mZSinkTimer = mActor->getConst().getTable()->mZsinkStartTimer;
            }
            if (mJetTimer != 0) {
                MarioConstTable* table = mActor->getConst().getTable();
                f32 a = getStickY();
                f32 b = table->mSwimRotXIneT;
                // b = getSwimValue(a = getStickY(), 2, mActor->getConst().getTable());
                mStickInputYInertia = mStickInputYInertia * b + procAngle(a, b);
                table = mActor->getConst().getTable();
                a = getStickX();
                b = table->mSwimRotZIneT;
                mStickInputXInertia = mStickInputXInertia * b + procAngle(a, b);
            } else {
                MarioConstTable* table = mActor->getConst().getTable();
                f32 a = getStickY();
                f32 b = table->mSwimRotXIne;

                // b = getSwimValue(a = getStickY(), 0, mActor->getConst().getTable());
                mStickInputYInertia = mStickInputYInertia * b + procAngle(a, b);
                table = mActor->getConst().getTable();
                a = getStickX();

                // b = getSwimValue(a = getStickX(), 1, mActor->getConst().getTable());
                b = table->mSwimRotZIne;

                mStickInputXInertia = mStickInputXInertia * b + procAngle(a, b);
            }
        }
        if (!mIsSwimmingAtSurface && !mIdleWaitTimer && mActionLockTimer != 0) {
            mVerticalAngle += res * (mStickInputYInertia * mActor->getConst().getTable()->mSwimRotSpeedX);
        }

        if ((mForwardSpeed < cTurnMotionSpeed || mIsOnSurface || checkLvlA() || checkLvlZ()) && isAnimationRun("水泳一掻き") &&
            isAnimationTerminate("水泳一掻き")) {
            stopAnimation(nullptr, static_cast< const char* >(nullptr));
        }

        if (!checkLvlA() && !checkLvlZ() && !mActionLockTimer && mForwardSpeed < cTurnMotionSpeed) {
            bool worthless;
            bool& worthlesser = worthless;
            funReferenceTime(worthlesser);
            if (!mIsSwimmingAtSurface && mKnockbackTimer == 0 && mWaterDepth < -400.0f) {
                mIdleWaitTimer++;
                if (MR::getRandom() < 0.03f) {
                    playSound("水中ウエイト", -1);
                }
            }
            if (mIdleWaitTimer >= 0x78) {
                mIdleWaitTimer = 0x78;
            }
        } else {
            mIdleWaitTimer = 0;
        }
        f32 fr1d;
        if (MR::isNearZero(getStickY(), 0.1f)) {
            f32 num = mIdleWaitTimer / 120.0f;
            if (num > 1.0f) {
                num = 1.0f;
            }
            fr1d = mVerticalAngle + num * (cNeutralAngleWait - mVerticalAngle);
        } else if (getStickY() > 0.0f) {
            f32 fr4 = 0.0f;
            f32 num = mIdleWaitTimer / 120.0f;
            if (mForwardSpeed < 2.0f) {
                fr4 = 1.0f;
            } else {
                bool confusion;
                bool& confusionion = confusion;
                if (mForwardSpeed > 10.0f) {
                    confusionion = true;
                } else {
                    fr4 = 1.0f - (mForwardSpeed - 2.0f) / 8;
                }
            }
            if (num < fr4) {
                num = fr4;
            }
            f32 numClamped = MR::clamp(num, 0.0f, 1.0f);
            f32 fr1c = numClamped * cLimitAngleWait + (1.0f - numClamped) * cLimitAngleSink;
            if (mDistToFloor < 300.0f && -mWaterDepth < 100.0f) {
                fr1c = cNeutralAngleWait;
            }
            fr1d = cNeutralAngleWait + (fr1c - cNeutralAngleWait) * getStickY();
        } else if (getStickY() < 0.0f) {
            fr1d = cLimitAngleSink + (cLimitAngleWait - cLimitAngleSink) * -getStickY();
        }
        f32 fr1e = 0.05f;
        if (mForwardSpeed > 5.0f && (fr1e -= fr1e * (5.0f / mForwardSpeed)) < 0.0f) {
            fr1e = 0.0f;
        }
        if (mJetTimer != 0) {
            switch (_88) {
            case 0:
                fr1e *= mActor->getConst().getTable()->mSwimXJetRotRatio;
                break;
            default:
                fr1e *= mActor->getConst().getTable()->mSwimXJetRotRatioRed;
                break;
            }
        }
        if (mIsSwimmingAtSurface) {
            fr1d = 0.523598790169f;
            fr1e = 0.01f;
        }
        if (mIsOnSurface) {
            fr1d = 0.523598790169f;
        }
        if (mZSinkTimer != 0) {
            fr1e = 0.15f;
            fr1d = mActor->getConst().getTable()->mZsinkAngleX;
        }
        bool r1b = true;
        mVerticalAngle = mVerticalAngle * (1.0f - fr1e) + fr1d * fr1e;
        if (mIdleWaitTimer != 0 && mKnockbackTimer == 0 && !mZSinkTimer && getStickY() > 0.0f) {
            f32 fr1c = PI / mActor->getConst().getTable()->mSwimTiltReal;
            f32 tmpsticky = getStickY();
            fr1d = 0.523598790169f + tmpsticky * (fr1c - 0.523598790169f);
            if (mVerticalAngle < fr1d) {
                mVerticalAngle =
                    mVerticalAngle * mActor->getConst().getTable()->mSwimTiltSpd + fr1d * (1.0f - mActor->getConst().getTable()->mSwimTiltSpd);
            } else {
                mVerticalAngle = mVerticalAngle * (1.0f - fr1e) + fr1d * fr1e;
            }
            if (!check7Aand7C() && mJetTimer == 0) {
                changeAnimation("水泳ターン下", static_cast< const char* >(nullptr));
            }
            r1b = false;
        }
        if (r1b) {
            stopAnimation("水泳ターン下", static_cast< const char* >(nullptr));
        }
        if (mZSinkTimer != 0) {
            mVerticalAngle = MR::clamp(mVerticalAngle, 0.523598790169f, mActor->getConst().getTable()->mZsinkAngleX);
        } else {
            mVerticalAngle = MR::clamp(mVerticalAngle, cUpperAngleWait, cLimitAngleSink);
        }
        if (MR::isStageSwimAngleLimit()) {
            mVerticalAngle = MR::clamp(mVerticalAngle, 0.872664690018f, 2.26892805099f);
        }
    }

    if ((mIsSwimmingAtSurface || (mIsOnSurface && isStickOn())) && mJetTimer == 0) {
        TVec3f stack_164(getWorldPadDir());
        f32 tmp;
        if (mIsOnSurface) {
            tmp = 0.01f * (12.0f - mForwardSpeed);
            if (tmp <= 0.01f) {
                tmp = 0.01f;
            }
        } else {
            tmp = 0.01f * (10.0f - mForwardSpeed);
        }
        if (mJetTimer != 0) {
            tmp *= 3.0f;
        }
        f32 res = MR::clamp(tmp * mActor->getConst().getTable()->mSwimRotSpeedRatioSurface, 0.0f, 1.0f);
        f32 diffAngle = MR::diffAngleAbs(mFrontVec, stack_164);
        if (diffAngle > 0.0f) {
            if (diffAngle < res) {
                mFrontVec = stack_164;
            } else {
                MR::vecBlendSphere(mFrontVec, stack_164, &mFrontVec, res / diffAngle);
            }
        }
    } else {
        f32 tmp = fr1f + mActor->getConst().getTable()->mSwimRotSpeedZStop;
        MR::rotAxisVecRad(mFrontVec, -mUpVec, &mFrontVec, tmp * (mStickInputXInertia * mActor->mConst->getTable()->mSwimRotSpeedZ));
    }
    TVec3f stack_158(mFrontVec);
    MR::vecKillElement(mFrontVec, mUpVec, &mFrontVec);
    TVec3f stack_14C;
    TVec3f stack_140;
    if (!MR::normalizeOrZero(&mFrontVec)) {
        PSVECCrossProduct(&mUpVec, &mFrontVec, &stack_140);
        MR::normalize(&stack_140);
        getPlayer()->setSideVec(stack_140);
        MR::rotAxisVecRad(mFrontVec, stack_140, &stack_14C, mVerticalAngle);
        TVec3f stack_134(getPlayer()->mHeadVec);
        TVec3f stack_128(getFrontVec());
        f32 something = 0.1f;
        if (r1e) {
            something = 1.0f;
        }
        TVec3f stack_11C;
        MR::vecBlendSphere(stack_128, stack_14C, &stack_11C, something);
        getPlayer()->setFrontVecKeepSide(stack_11C);
    } else {
        stack_14C = getFrontVec();
        stack_140 = getPlayer()->mSideVec;
    }
    spin();
    TVec3f stack_110;
    stack_110 = getPlayer()->_1FC;
    if (mJumpDiveTimer != 0 || mSinkTimer != 0 || isAnimationRun("水泳ジャンプダイブ回転") || isAnimationRun("水泳潜り")) {
        PSVECCrossProduct(&stack_14C, &stack_140, &stack_110);
    }
    if (mJetTimer == 0) {
        decideVelocity();
    }
    TVec3f stack_104 = stack_110.scaleInline(mForwardSpeed);
    if (mIsOnSurface && mUpVec.dot(stack_110) > 0.0f) {
        f32 mag = stack_104.length();
        MR::vecKillElement(stack_104, mUpVec, &stack_104);
        stack_104.setLength(mag);
    }
    f32 element = MR::vecKillElement(stack_104, mUpVec, &stack_104);
    f32 fr2;
    if (mSwimYAccTimer > 0x19) {
        fr2 = (50 - mSwimYAccTimer) / 25.0f;
    } else {
        fr2 = mSwimYAccTimer / 25.0f;
    }
    f32 fr1d = fr2 + (1.0f - fr2) * mActor->getConst().getTable()->mSwimSpdYratio;

    stack_104 += mUpVec.scaleInline(element).scaleInline(fr1d);
    addVelocity(stack_104);
    u16 fr1b = mSinkTimer;
    if (fr1b) {
        addVelocity(getGravityVec(), 0.25f * fr1b);
        mSinkTimer--;
    }

    if (mJumpDiveTimer != 0) {
        mJumpDiveTimer--;
    }
    if (!mIsOnSurface && !mActionLockTimer && mJetTimer == 0) {
        procBuoyancy();
    } else if (mUpVec.dot(stack_110) < 0.0f) {
        if (mIsOnSurface) {
            mIsOnSurface = false;
        }
    } else if (mIsOnSurface) {
        AreaObj* obj = MR::getAreaObj("WaterArea", getTrans().translate(getGravityVec().scaleInline(100.0f)));
        if (obj != nullptr) {
            TPos3f* followMtx = obj->getFollowMtx();
            if (followMtx) {
                TVec3f stack_F8;
                MR::extractMtxTrans(followMtx->toMtxPtr(), &stack_F8);

                bool cond = JGeometry::TUtil< f32 >::epsilonEquals(stack_F8.x, mAreaFollowMtxPos.x, 0.000003814697265625f) &&
                            JGeometry::TUtil< f32 >::epsilonEquals(stack_F8.y, mAreaFollowMtxPos.y, 0.000003814697265625f) &&
                            JGeometry::TUtil< f32 >::epsilonEquals(stack_F8.z, mAreaFollowMtxPos.z, 0.000003814697265625f);
                if (!cond) {
                    // TVec3f diff = stack_F8 - _154;
                    if ((stack_F8 - mAreaFollowMtxPos).length() < 10.0f) {
                        addVelocity(stack_F8 - mAreaFollowMtxPos);
                    }
                    mAreaFollowMtxPos = stack_F8;
                }
            }
        }
    }
    if (mKnockbackTimer != 0) {
        mKnockbackTimer--;
        addVelocity(mKnockbackVel);
        if (mKnockbackTimer < 0x78) {
            mKnockbackVel.mult(0.98f);
        }
    }
    if (mJetTimer != 0) {
        addVelocity(mWaterStreamVel, mActor->getConst().getTable()->mTurboReductionStream);
    } else {
        addVelocity(mWaterStreamVel);
    }

    TVec3f stack_EC(mWaterStreamVel);
    f32 fr1dv2 = stack_EC.length();
    if (!MR::normalizeOrZero(&stack_EC)) {
        if (MR::diffAngleAbs(stack_EC, mFrontVec) < PI / 2) {
            MR::vecBlendSphere(mFrontVec, stack_EC, &mFrontVec, 0.001f * fr1dv2);
        } else {
            MR::vecBlendSphere(mFrontVec, -stack_EC, &mFrontVec, 0.001f * fr1dv2);
        }
    }

    TVec3f stack_E0;
    if (MR::calcWhirlPoolAccelInfo(getTrans(), &stack_E0) && !MR::isNearZero(stack_E0)) {
        mWaterStreamVel += stack_E0;
        TVec3f stack_D4(stack_E0);
        MR::normalizeOrZero(&stack_D4);
        MR::vecBlendSphere(mFrontVec, stack_D4, &mFrontVec, 0.1f);
    }

    if (mJetTimer == 0 && !mIsOnSurface && !MR::isNearZero(mWaterStreamDir)) {
        f32 angleDiff = MR::diffAngleSignedHorizontal(mWaterStreamDir, mFrontVec, getPlayer()->getAirGravityVec());
        f32 cmp = DEG_TO_RAD(60);
        if (angleDiff > cmp) {
            Mtx stack_1D0;
            PSMTXRotAxisRad(stack_1D0, &mUpVec, angleDiff - DEG_TO_RAD(60));
            PSMTXMultVecSR(stack_1D0, &mFrontVec, &mFrontVec);
        } else if (angleDiff < -cmp) {
            Mtx stack_1A0;
            PSMTXRotAxisRad(stack_1A0, &mUpVec, -(-angleDiff - DEG_TO_RAD(60)));
            PSMTXMultVecSR(stack_1A0, &mFrontVec, &mFrontVec);
        }
    }

    decideAnimation();
    decideEffect(false);

    if (mActionLockTimer != 0) {
        mActionLockTimer--;
    }
    if (mSwimYAccTimer != 0) {
        mSwimYAccTimer--;
    }
    if (_44 != 0) {
        _44--;
    }

    updateTilt();
    jet();
    pushedByWaterWall();
    return true;
}

void MarioSwim::procBuoyancy() {
    if (mNextAction != EXIT_ACTION_NONE) {
        return;
    }

    if (mKnockbackTimer != 0) {
        mBuoyancySpeed = 0.0f;
    } else {
        mIsSinking = false;
        if (mEnteredWater) {
            mEnteredWater = false;
            mBuoyancyDelayTimer = 120;
        }

        f32 waterDepthNeg = -mWaterDepth;
        f32 buoyancyScale = 1.0f;
        const MarioConstTable* table = mActor->getConst().getTable();
        bool isBelowStrongSwimDepth = table->mSwimUpStrongDist > waterDepthNeg;

        f32 swimUpWeakDist = table->mSwimUpWeakDist;
        f32 swimUpBottomDist = table->mSwimUpBottomDist;
        f32 floorDepthSum = mDistToFloor + waterDepthNeg;

        if (floorDepthSum < 2000.0f) {
            f32 ratio = floorDepthSum / 2000.0f;
            swimUpWeakDist = table->mSwimUpWeakDist * ratio;
            swimUpBottomDist = table->mSwimUpBottomDist * ratio;
        }

        bool shouldSink = (swimUpWeakDist > mDistToFloor);
        if (mForwardSpeed > 2.0f && mVerticalAngle > PI / 2) {
            shouldSink = false;
            mIsSinking = false;
        }

        if (!(mDistToFloor < 300.0f) || !(-mWaterDepth < 100.0f)) {
            if (shouldSink) {
                if (mStateTimer > 300) {
                    f32 distRatio = mDistToFloor / swimUpWeakDist;
                    if (distRatio < 0.0f) {
                        distRatio = 0.0f;
                    }
                    if (swimUpBottomDist > mDistToFloor) {
                        distRatio = 0.0f;
                    }

                    buoyancyScale =
                        mActor->getConst().getTable()->mSwimUpWeakRatio + (distRatio * (1.0f - mActor->getConst().getTable()->mSwimUpWeakRatio));
                } else {
                    buoyancyScale = 1.0f + (1.0f - (static_cast< f32 >(mStateTimer) / 300.0f));
                }
                mIsSinking = true;
            } else if (isBelowStrongSwimDepth) {
                const MarioConst* marioConst = &mActor->getConst();
                MarioConstTable* table = mActor->getConst().getTable();

                if (waterDepthNeg > table->mSwimUpSurfaceDist) {
                    buoyancyScale = table->mSwimUpStrongRatio;
                } else {
                    f32 strongRatio = (waterDepthNeg - table->mSwimUpSurfaceDist) / (table->mSwimUpStrongDist - table->mSwimUpSurfaceDist);
                    if (strongRatio < 0.0f) {
                        strongRatio = 0.0f;
                    }
                    buoyancyScale = strongRatio + ((1.0f - strongRatio) * marioConst->getTable()->mSwimUpStrongRatio);
                }
            }
        }

        if (mBuoyancyDelayTimer != 0) {
            mBuoyancyDelayTimer--;
        } else {
            f32 ratio = 1.0f;
            mBuoyancySpeed += (buoyancyScale * (ratio * mActor->getConst().getTable()->mSwimUpAcc));
        }
        if (mBuoyancySpeed > mActor->getConst().getTable()->mSwimUpMaxSpeed) {
            mBuoyancySpeed = mActor->getConst().getTable()->mSwimUpMaxSpeed;
        }

        if (!MR::isNearZero(mWaterStreamVel)) {
            buoyancyScale = 1.0f;
        }

        if (mIsSinking) {
            if (mDistToFloor < mActor->getConst().getTable()->mSwimUpBottomDist) {
                f32 clampRatio = MR::clamp(1.0f - (static_cast< f32 >(mActionLockTimer) / 50.0f), 0.0f, 1.0f);
                f32 sinkFactor = ((1.0f - clampRatio) + (clampRatio * mActor->getConst().getTable()->mSwimReverseSinkRatio));
                sinkFactor *= 0.1f;

                if (!MR::isNearZero(mWaterStreamVel)) {
                    sinkFactor = 1.0f;
                }
                addVelocity(-mUpVec, (sinkFactor * (mBuoyancySpeed * buoyancyScale)));
            } else {
                addVelocity(-mUpVec, mBuoyancySpeed * buoyancyScale);
            }
        } else {
            addVelocity(mUpVec, mBuoyancySpeed * buoyancyScale);
        }
    }
}

void MarioSwim::flowOnWave(f32 unused) {
    if (mActionLockTimer != 0) {
        return;
    }

    if (mWaterDepth < -cWaterSurfaceRange) {
        if (mIsOnSurface) {
            mIsOnSurface = false;
        }

        return;
    } else if (mIsOnSurface) {
        if (mWaterDepth < -25.0f) {
            addVelocity(mSurfacePos, mBuoyancySpeed);
        }
        if (mWaterDepth > 0.0f) {
            TVec3f inverseFlow = -mSurfacePos;
            addVelocity(inverseFlow, 5.0f + mWaterDepth);
        }
    }
    mWaveFlowTimer++;
}

void MarioSwim::decideVelocity() {
    if (mRingDashTimer != 0) {
        return;
    }

    if (MarioModule::checkLvlZ() || (MarioModule::checkLvlA() && _9C)) {
        mIsSwimmingAtSurface = false;
    }

    if (mWaterDepth < -400.0f && mSinkTimer == 0) {
        mIsSwimmingAtSurface = false;
    }

    if ((mIsOnSurface && (MarioModule::isStickOn() || mActionLockTimer != 0)) || (mIsSwimmingAtSurface && MarioModule::isStickOn())) {
        if (mActionLockTimer == 0 && mJetTimer == 0) {
            f32 angleDiff = MR::diffAngleAbs(mFrontVec, getWorldPadDir());
            f32 angleFactor = (PI - angleDiff) / PI;
            f32 targetSpeedScale = angleFactor * MarioModule::getStickP();
            const MarioConst* marioConst = &mActor->getConst();
            MarioConstTable* table = marioConst->getTable();
            f32 inertia = table->mSwimFrontIne;
            if (mSpinDashTimer == 0 && mForwardSpeed >= (targetSpeedScale * table->mSwimFrontOnWaterSpeed)) {
                inertia = 0.95f;
            }
            f32 invInertia = (1.0f - inertia);
            f32 frontSpeed = marioConst->getTable()->mSwimFrontOnWaterSpeed;
            f32 term1 = invInertia * (targetSpeedScale * frontSpeed);
            f32 term2 = mForwardSpeed * inertia;
            mForwardSpeed = term1 + term2;
        }
    } else if (mIsOnSurface && mDashTimer != 0) {
        f32 inertia = mActor->getConst().getTable()->mSwimFrontIne;
        mForwardSpeed = (mForwardSpeed * inertia) + ((1.0f - inertia) * mActor->getConst().getTable()->mSwimFrontOnWaterSpeed);
    } else {
        if (!mIsOnSurface && checkTrgA() && mKnockbackTimer == 0) {
            bool isSpinning = false;

            if (mSpinTimer == 0) {
                if (mSpinDashTimer != 0) {
                    isSpinning = false;
                }
                if (!isSpinning) {
                    f32 stickX = __fabsf(MarioModule::getStickX());
                    f32 stickFactor = 1.0f - (0.3f * stickX);
                    f32 threshold = 0.75f * stickFactor;

                    if (mVerticalAngle < PI / 2 && getStickY() > threshold && mActionLockTimer == 0 && _1E != 0) {
                        f32 stickY = getStickY();
                        f32 normalizedY = (stickY - threshold) / (1.0f - threshold);
                        mVerticalAngle = ((1.0f - normalizedY) * DEG_TO_RAD(120)) + (normalizedY * DEG_TO_RAD(150));
                    } else if (mVerticalAngle < PI / 2 && __fabsf(getStickY()) < 0.1f && mActionLockTimer == 0 && _1E != 0) {
                        mVerticalAngle = PI / 2;
                    }

                    _1E = 0;

                    bool isTerminated = MarioModule::isAnimationTerminate("水泳一掻き");

                    bool inRange;
                    if (isTerminated || !_1D) {
                        const MarioConst* marioConst = &mActor->getConst();
                        f32 frame = getAnimator()->getFrame();
                        MarioConstTable* table = marioConst->getTable();

                        bool inRange = MR::isInRange(frame, static_cast< f32 >(table->mSwimConnectIn), static_cast< f32 >(table->mSwimConnectOut));
                        if (!inRange) {
                            _1D = true;
                            return;
                        } else {
                            inRange = false;
                            isTerminated = false;
                            _1D = false;
                            mActionLockTimer = 60;
                            mSwimYAccTimer = 50;

                            if (!MarioModule::isAnimationRun("水泳潜り") && !MarioModule::isAnimationRun("水泳ジャンプダイブ回転")) {
                                getAnimator()->forceSetBlendWeight(cWeightTableSP);
                                stopAnimation(static_cast< const char* >(nullptr), static_cast< const char* >(nullptr));
                                changeAnimationNonStop("水泳一掻き");
                                playSound("水中一掻き", -1);
                                mDashTimer = 40;
                            }
                        }
                    }
                }
            }

        } else {
            if (!mIsOnSurface && MarioModule::checkLvlA()) {
                mForwardSpeed = (mForwardSpeed * mActor->getConst().getTable()->mSwimFrontIne) +
                                (mActor->getConst().getTable()->mSwimFrontNormalSpeed * (1.0f - mActor->getConst().getTable()->mSwimFrontIne));
                mBuoyancySpeed = 0.0f;
            } else {
                if (mIsOnSurface) {
                    mForwardSpeed *= mActor->getConst().getTable()->mSwimStopIneSurface;
                } else {
                    mForwardSpeed *= mActor->getConst().getTable()->mSwimStopIne;
                }

                if (MarioModule::isAnimationRun("水泳上昇呼吸")) {
                    mForwardSpeed *= 0.95f;
                }

                if (mDistToFloor > mActor->getConst().getTable()->mSwimUpBottomDist) {
                    mForwardSpeed *= 0.99f;
                } else if (mDistToFloor > mActor->getConst().getTable()->mSwimUpWeakDist) {
                    mForwardSpeed *= 0.995f;
                }
            }
        }
    }

    if (mDashTimer != 0) {
        bool isSpinning = (mSpinTimer != 0 || mSpinDashTimer != 0);

        if (isSpinning) {
            if (mIsOnSurface) {
                if (mDashTimer < 20) {
                    f32 acc = cFrontAccSpinSurface[20 - mDashTimer];

                    if (acc < 0.0f) {
                        mForwardSpeed += acc * mActor->getConst().getTable()->mSwimSpinSurfaceAccRatioR;
                    } else {
                        mForwardSpeed += acc * mActor->getConst().getTable()->mSwimSpinSurfaceAccRatio;
                    }

                    playSound("水面スピン", -1);
                } else {
                    mForwardSpeed *= 0.96f;
                }
            } else {
                if (mDashTimer < 30) {
                    f32 acc = cFrontAccSpin[30 - mDashTimer];
                    if (acc < 0.0f) {
                        mForwardSpeed += acc * mActor->getConst().getTable()->mSwimSpinAccRatioR;
                    } else {
                        mForwardSpeed += acc * mActor->getConst().getTable()->mSwimSpinAccRatio;
                    }
                    playSound("水中スピン", -1);
                } else {
                    mForwardSpeed *= 0.96f;
                }
            }
        } else if (mDashTimer < 40) {
            f32 accRatio = mActor->getConst().getTable()->mSwimAccRatio;
            f32 accBase = cFrontAcc[40 - mDashTimer];
            f32 currentAcc = accBase * accRatio;

            f32 maxSpeed = mActor->getConst().getTable()->mSwimFrontMaxSpeed;
            f32 minRatio = mActor->getConst().getTable()->mSwimAccMinRatio;

            f32 speedFactor = mForwardSpeed / maxSpeed;
            mForwardSpeed += (minRatio + (speedFactor * (1.0f - minRatio))) * currentAcc;
        }

        mDashTimer--;
        bool canClamp = false;

        if (mSpinTimer != 0 || mSpinDashTimer != 0) {
            canClamp = true;
        }

        if (canClamp) {
            if (mForwardSpeed > mActor->getConst().getTable()->mSwimFrontSpinSpeed) {
                mForwardSpeed = mActor->getConst().getTable()->mSwimFrontSpinSpeed;
            }
        } else {
            if (mForwardSpeed > mActor->getConst().getTable()->mSwimFrontMaxSpeed) {
                mForwardSpeed = mActor->getConst().getTable()->mSwimFrontMaxSpeed;
            }
        }
    }

    if (mZSinkTimer != 0) {
        mForwardSpeed *= 0.95f;
    }
}

void MarioSwim::decideAnimation() {
    // This is the only way I could achieve a block copy
    struct AnimSpeedData {
        f32 speeds[4];
    };
    AnimSpeedData animSpeeds = *reinterpret_cast< const AnimSpeedData* >(cAnimSpeeds);

    const MarioConstTable* table = mActor->getConst().getTable();
    animSpeeds.speeds[1] = table->mSwimSurfaceSpeed;
    animSpeeds.speeds[2] = table->mSwimWaterSpeed;
    animSpeeds.speeds[3] = table->mSwimDriftSpeed;

    if (mJetTimer != 0) {
        return;
    }
    if (isAnimationRun("水泳水面初期移動")) {
        if (getStickP() < 0.1f) {
            stopAnimationUpper(nullptr, nullptr);
        }
    }

    if (isAnimationRun(nullptr) && !isAnimationRun("水泳上昇呼吸")) {
        if (__fabsf(getStickX()) < 0.1f) {
            stopAnimation("水泳ターン左", static_cast< const char* >(nullptr));
            stopAnimation("水泳ターン右", static_cast< const char* >(nullptr));
        }
        return;
    }

    if (mActionLockTimer != 0) {
        getAnimator()->setBlendWeight(&cWeightTable[4], mActor->getConst().getTable()->mWeightBlendRatioSwim);
        return;
    }

    u32 animIndex = 0;

    if (!mIsOnSurface) {
        if (checkLvlA()) {
            playSound("水中バタ足", -1);
            animIndex = 2;
        } else {
            animIndex = 3;
            if (mIsSwimmingAtSurface != false) {
                if (getStickP() > 0.1f) {
                    animIndex = 1;
                }
            } else if (__fabsf(getStickX()) > 0.1f && mVerticalAngle < 1.6534699f && mForwardSpeed < cTurnMotionSpeed) {
                if (getStickX() < 0.0f) {
                    changeAnimation("水泳ターン左", static_cast< const char* >(nullptr));
                } else {
                    changeAnimation("水泳ターン右", static_cast< const char* >(nullptr));
                }
                return;
            }
        }
    } else {
        // Surface logic
        if (getStickP() > 0.1f) {
            playSound("水面バタ足", -1);

            if (isAnimationRun("水泳上昇呼吸")) {
                stopAnimation("水泳上昇呼吸", static_cast< const char* >(nullptr));
                if (mIsSwimmingAtSurface == false) {
                    _1B = true;
                }
            }

            animIndex = 1;

            if (_1B) {
                getAnimator()->forceSetBlendWeight(&cWeightTable[4]);
                changeAnimationUpper("水泳水面初期移動", static_cast< const char* >(nullptr));
                _1B = false;
                return;
            }
        } else {
            if (mDashTimer == 0 && mForwardSpeed < 2.0f) {
                _1B = true;
            }
            animIndex = 0;
            if (isAnimationRun(nullptr)) {
                return;
            }
        }
    }

    if (animIndex == 1) {
        getAnimator()->setBlendWeight(&cWeightTable[animIndex * 4], mActor->getConst().getTable()->mWeightBlendRatioSwimB);
    } else {
        getAnimator()->setBlendWeight(&cWeightTable[animIndex * 4], mActor->getConst().getTable()->mWeightBlendRatioSwim);
    }
    getAnimator()->setSpeed(animSpeeds.speeds[animIndex]);
}

void MarioSwim::decideEffect(bool isReset) {
    u8 oldState = mEffectState;
    if (mIsOnSurface) {
        if (isAnimationRun("水泳水面初期移動") || mForwardSpeed > 2.5f) {
                mEffectState = 1;
            } else {
                mEffectState = 0;
            }
        
    } else {
        if (checkLvlZ() || checkLvlA() || mActionLockTimer != 0 || isAnimationRun(nullptr)) {
            mEffectState = 2;
        } else {
            mEffectState = 3;
        }
    }

    if (isReset == true) {
        mEffectState = 0xFF;
    }
    u8 newState = mEffectState;
    if (oldState != mEffectState) {
        switch (oldState) {
        case 0:
            if (mEffectState != 1) {
                stopEffect("水面ウエイト波紋");
            }
            break;

        case 1:
            stopEffect("水面泳ぎ");
            if (mEffectState != 0) {
                stopEffect("水面ウエイト波紋");
            }
            break;

        case 2:
            stopEffect("水中身体泡");
            break;
        }

        if (mEffectState >= 2 || oldState >= 2 || isReset) {
            stopEffect("水中口泡");
        }

        switch (mEffectState) {
        case 0: {
            TVec3f v1 = mSurfacePos;
            v1.scale(30.0f);

            f32 waterDepth = mWaterDepth;

            TVec3f v2 = mSurfacePos;
            v2.scale(waterDepth);
            TVec3f unusedPos = (getTrans() - v2);
            TVec3f copyPos = unusedPos;

            copyPos += v1;
            break;
        }
        case 1:
            if (mSurfaceOffset > -10.0f) {
                TVec3f projected;
                MR::vecKillElement(mActor->mVelocity - mSurfaceNorm, mSurfacePos, &projected);

                const TVec3f& lastMove = mActor->getLastMove();
                TVec3f step1 = mSurfaceNorm;
                step1 += projected;
                TVec3f finalPos = step1;
                finalPos += lastMove;

                playEffectRTW("水面泳ぎ", mSurfacePos, finalPos);
            } else {
                stopEffect("水面泳ぎ");
            }
            break;
        case 2:
            playEffect("水中身体泡");

        case 3:
        case 4:
            playEffect("水中口泡");
            break;
        }
    }

    if (mEffectState <= 1) {
        TVec3f vDiff = (mActor->mVelocity - mSurfaceNorm);
        TVec3f projected;
        MR::vecKillElement(vDiff, mSurfacePos, &projected);

        const TVec3f& lastMove = mActor->getLastMove();
        TVec3f tempPos = mSurfaceNorm;
        tempPos += projected;
        TVec3f ripplePos = tempPos;
        ripplePos += lastMove;

        playEffectRTW("水面ウエイト波紋", mSurfacePos, ripplePos);

        if (MR::getRandom() < 0.03f) {
            playSound("水面ウエイト", -1);
        }
    }
}

bool MarioSwim::notice() {
    if (MarioState::getNoticedStatus() == 18) {
        return true;
    }

    return getNoticedStatus() == 11;
}

bool MarioSwim::close() {
    if (mIsOnSurface) {
        mIsOnSurface = false;
    }

    stopAnimationUpper(nullptr, nullptr);
    stopAnimation("水上スピン", static_cast< const char* >(nullptr));        // Surface Spin
    stopAnimation("水泳スピン", static_cast< const char* >(nullptr));        // Swim Spin
    stopAnimation("水泳ジェット開始", static_cast< const char* >(nullptr));  // Swim Jet Start
    stopEffect("水底接触");                                                  // Water Bottom Contact
    stopEffect("水泳スピン");                                                // Swim Spin Effect

    if (mActor->getCarrySensor()) {
        LiveActor* shell = mActor->getCarrySensor()->mHost;
        MR::forceDeleteEffect(shell, "BrakeLamp");
    }

    setYangleOffset(0.0f);
    u16 jointID = getAnimator()->getUpperJointID();
    setJointGlobalMtx(jointID, nullptr);

    decideEffect(true);

    if (mNextAction != EXIT_ACTION_SURFACE) {
        // If not surfacing, decide between Fall or Basic based on vertical state
        if (getPlayer()->mMovementStates.jumping) {
            stopAnimation("水泳基本", "基本");
        } else {
            stopAnimation("水泳基本", "落下");  // Fall
        }
    } else {
        if (isDefaultAnimationRun("水泳基本")) {
            changeAnimation(nullptr, "基本");  // Transition to Basic/Wait
        }
    }

    mWaterInfo.clear();

    if (mNextAction == EXIT_ACTION_JUMP || mNextAction == EXIT_ACTION_SLIDE) {
        mActor->setBlendMtxTimer(16);
        changeAnimation("水泳陸うちあげ", static_cast< const char* >(nullptr));  // Landing from water
        playSound("水歩行脱出", -1);

        f32 stickP = getStickP();
        getPlayer()->_278 = stickP;

        getPlayer()->mMovementStates._B = false;
        getPlayer()->mMovementStates._1 = true;
        getPlayer()->updateGroundInfo();
        getPlayer()->_10.digitalJump = true;
        getPlayer()->mMovementStates._B = false;
        getPlayer()->mMovementStates._1 = true;

        TVec3f groundPos;
        mActor->getGroundPos(&groundPos);
        TVec3f distToGround = getTrans() - groundPos;

        if (distToGround.length() >= 100.0f || isSlipPolygon(getGroundPolygon())) {
            mNextAction = EXIT_ACTION_POWER_JUMP;

            getPlayer()->tryForcePowerJump(mFrontVec.scaleInline(5.0f) - getAirGravityVec().scaleInline(15.0f), false);
            Mario* player = getPlayer();
            player->_43A = 120;
        } else {
            // Snap to ground
            addTrans(groundPos - getTrans(), "Module");
            Mario* player = getPlayer();
            player->_43A = 60;
        }
    } else {
        getPlayer()->forceSetHeadVecKeepSide(-getGravityVec());
        changeAnimationInterpoleFrame(6);
        mActor->setBlendMtxTimer(4);
    }

    if (mNextAction == EXIT_ACTION_POWER_JUMP) {
        TVec3f spawnPos = (getTrans() - mUpVec.scaleInline(mWaterDepth)).translate(mUpVec.scaleInline(10.0f));

        playEffectRT("水面ジャンプ水柱", mSurfacePos, mSurfaceNorm);
        playSound("水ジャンプ脱出", -1);

        _1B2 = true;
        mWaterDistanceTarget = 0.0f;
        _1B8 = 0.0f;

        getPlayer()->setGroundNorm(-getAirGravityVec());
    }

    if (mNextAction == EXIT_ACTION_FALL) {
        TVec3f trans = getTrans();

        playEffectRT("水面ジャンプ水柱", -mSurfacePos, trans.translate(mSurfacePos.scaleInline(mDistToFloor)));

        getPlayer()->setGroundNorm(-getAirGravityVec());

        if (mJetTimer != 0) {
            changeAnimationNonStop("水泳ジェット");
        }
    }

    if (mNextAction == EXIT_ACTION_SLIDE) {
        getPlayer()->connectToClimb();
    }

    if (mNextAction == EXIT_ACTION_NONE) {
        getPlayer()->tryDrop();

        MR::vecKillElement(mWaterStreamVel, getGravityVec(), &mWaterStreamVel);

        getPlayer()->mJumpVec = getPlayer()->mJumpVec.translate(mWaterStreamVel.scaleInline(0.3f));
    }

    getPlayer()->unlockGroundCheck(this);
    mActor->_F44 = true;
    mSwimState = SWIM_STATE_NONE;

    mActor->resetWaterLife();
    MR::getGameSceneLayoutHolder()->changeLifeMeterModeGround();

    mBlurOffset = 0.0f;
    if (mJetTimer != 0) {
        getPlayer()->_10._7 = true;
    }

    getPlayer()->forceSetHeadVecKeepSide(-getAirGravityVec());

    return true;
}

bool MarioSwim::checkWaterCube(bool checkOnly) {
    mUpVec = -getPlayer()->getAirGravityVec();
    WaterInfo bodyWaterInfo;
    MR::getWaterAreaObj(&bodyWaterInfo, getTrans());
    if ((getPlayer()->mMovementStates.jumping & 1) && getPlayer()->isRising()) {
        bodyWaterInfo.clear();
    }

    WaterInfo headWaterInfo;
    if (mSwimState == SWIM_STATE_NONE) {
        if (bodyWaterInfo.isInWater()) {
            getPlayer()->mDrawStates.mIsUnderwater = true;
            getPlayer()->touchWater();

            MR::getWaterAreaInfo(&bodyWaterInfo, getTrans(), getPlayer()->getAirGravityVec(), false);

            getPlayer()->_738 = bodyWaterInfo.mCamWaterDepth;
            getPlayer()->_73C = bodyWaterInfo.mSurfacePos;
        }

        TVec3f offset(mUpVec);
        offset.scale(110.0f);
        TVec3f checkPos(getTrans());
        checkPos += offset;

        MR::getWaterAreaObj(&headWaterInfo, checkPos);
        if (!bodyWaterInfo.isInWater() || !headWaterInfo.isInWater()) {
            return false;
        }
    } else {
        TVec3f offset(mUpVec);
        offset.scale(80.0f);
        TVec3f checkPos(getTrans());
        checkPos += offset;

        MR::getWaterAreaObj(&headWaterInfo, checkPos);
    }

    if (bodyWaterInfo.isInWater()) {
        if (headWaterInfo.isInWater()) {
            mSwimState = SWIM_STATE_UNDERWATER;
            mWaterInfo = bodyWaterInfo;
        } else {
            if (getPlayer()->mVerticalSpeed - mWaterDepth < 80.0f) {
                if (mSwimState != SWIM_STATE_NONE) {
                    const TVec3f& shadowNorm = getPlayer()->getShadowNorm();
                    if (calcAngleD(shadowNorm) >= 30.0f) {
                        mNextAction = EXIT_ACTION_SLIDE;
                    } else {
                        mNextAction = EXIT_ACTION_JUMP;
                    }
                }

                mSwimState = SWIM_STATE_NONE;
                return false;
            }

            mSwimState = SWIM_STATE_ENTERING;
            mWaterInfo = bodyWaterInfo;
        }

        TVec2f depthVec;
        getWaterAreaInfo(&bodyWaterInfo, getTrans(), &depthVec);
        mWaterDepth = depthVec.x;
        mDistToFloor = depthVec.y;
        mSurfacePos = bodyWaterInfo.mSurfacePos;
        mSurfaceNorm = bodyWaterInfo.mSurfaceNormal;
        return true;
    } else {
        if ((getPlayer()->mMovementStates.jumping & 1) && !getPlayer()->isRising()) {
            bodyWaterInfo.clear();
        }

        if (headWaterInfo.isInWater()) {
            TVec2f depthVec;
            getWaterAreaInfo(&headWaterInfo, getTrans(), &depthVec);
            mSwimState = SWIM_STATE_SURFACE;
            mWaterDepth = depthVec.x;
            mDistToFloor = depthVec.y;
            mWaterInfo = headWaterInfo;

            mSurfacePos = bodyWaterInfo.mSurfacePos;
            mSurfaceNorm = bodyWaterInfo.mSurfaceNormal;

            getPlayer()->touchWater();
            return true;
        }
    }

    if (!checkOnly) {
        if (mWaterDepth > 100.0f && getPlayer()->mVerticalSpeed < 200.0f) {
            return true;
        }
    }

    mSwimState = SWIM_STATE_NONE;
    mNextAction = EXIT_ACTION_FALL;

    if (mWaterInfo.isInWater()) {
        TVec2f depthVec;
        getWaterAreaInfo(&mWaterInfo, getTrans(), &depthVec);
        mWaterDepth = depthVec.x;
    }

    if (!(mWaterDepth < -500.0f)) {
        if (mIsOnSurface && getPlayer()->mVerticalSpeed < 30.0f) {
            mNextAction = EXIT_ACTION_SLIDE;
        } else if (!MR::isStageSwimAngleLimit()) {
            mNextAction = EXIT_ACTION_SLIDE;
        }
    }

    return false;
}

void MarioSwim::updateUnderWater() {
    if (MarioModule::getPlayerMode() != 4 && MarioModule::getStickP() > 0.1f) {
        mWaterDistanceTarget = 0.0f;
    }

    u32 searchLimit = 2;
    if (MarioModule::getPlayerMode() == 3) {
        searchLimit = 10;
    }

    TVec3f searchDir = MarioModule::getFrontVec();
    if (!MR::isNearZero(mActor->getLastMove())) {
        searchDir = mActor->getLastMove();
        MR::vecKillElement(searchDir, getGravityVec(), &searchDir);
        MR::normalizeOrZero(&searchDir);
    }

    const f32 kOffset = 100.0f;

    for (u32 i = 0; i < searchLimit; ++i) {
        TVec3f checkPos =
            MarioModule::getTrans().translate(searchDir.scaleInline(kOffset)).translate(getGravityVec().scaleInline(mWaterDistanceTarget - kOffset));

        WaterInfo waterInfo;
        MR::getWaterAreaObj(&waterInfo, checkPos);

        if (waterInfo.isInWater()) {
            _1B8 = mWaterDistanceTarget;

            if (mWaterDistanceTarget > 300.0f) {
                getWaterAreaInfo(&waterInfo, checkPos, nullptr);
                TVec3f surfacePos = waterInfo.mSurfacePos;
                _1B8 = (surfacePos - getTrans()).dot(getGravityVec());
                _1B2 = true;
            }

            else if (mWaterDistanceTarget < 210.0f && MarioModule::getPlayerMode() == 3) {
                if (!MR::checkStrikePointToMap(checkPos, nullptr)) {
                    getWaterAreaInfo(&waterInfo, getTrans(), nullptr);
                    TVec3f surfacePos = waterInfo.mSurfacePos;

                    if (waterInfo.mCamWaterDepth < 100.0f) {
                        if (!(getPlayer()->mMovementStates._1) || getPlayer()->_71C) {
                            mActor->createIceFloor(surfacePos.translate(searchDir.scaleInline(170.0f)));
                        }
                        mWaterDistanceTarget = 0.0f;
                    }
                }
            } else {
                mWaterDistanceTarget = 0.0f;
            }
            mWaterDistanceTarget = 0.0f;
            return;
        }

        if (MR::checkStrikePointToMap(checkPos, nullptr)) {
            _1B2 = false;
            mWaterDistanceTarget = 0.0f;
            return;
        }

        mWaterDistanceTarget += 40.0f;
    }
}

f32 MarioSwim::checkUnderWaterFull(const TVec3f& rVec) {
    f32 currentDist = 0.0f;
    f32 offset = 100.0f;
    f32 step = 40.0f;
    f32 maxDist = 10000.0f;

    while (currentDist < maxDist) {
        TVec3f checkPos = getTrans().translate(rVec.scaleInline(offset)).translate(getGravityVec().scaleInline(currentDist - offset));
        if (MR::getAreaObj("ForbidWaterSearchCube", checkPos) != nullptr) {
            return -1.0f;
        }

        WaterInfo waterInfo;
        MR::getWaterAreaObj(&waterInfo, checkPos);
        if (waterInfo.isInWater()) {
            return currentDist;
        }

        if (MR::checkStrikePointToMap(checkPos, nullptr)) {
            return -1.0f;
        }

        currentDist += step;
    }

    return -1.0f;
}

bool MarioSwim::checkWaterBottom() {
    TVec3f upVecScaled(mUpVec);
    upVecScaled.scale(50.0f);
    TVec3f rayStart(getTrans());
    rayStart += upVecScaled;

    TVec3f negatedUp = -mUpVec;
    TVec3f rayDir(negatedUp);
    rayDir.scale(2000.0f);

    u32 hitCount = MR::getNearPolyOnLineSort(rayStart, rayStart, rayDir, nullptr);
    for (u32 i = 0; i < hitCount; ++i) {
        Triangle tri;
        TVec3f hitPos;

        MR::getSortedPoly(&hitPos, &tri, i);

        if (!MR::isWaterPolygon(&tri)) {
            mDistToFloor = (getTrans() - hitPos).dot(mUpVec);

            return mDistToFloor > 200.0f;
        }
    }

    return true;
}

AreaInfo* MarioSwim::getWaterAreaInfo(WaterInfo* pInfo, const TVec3f& rPos, TVec2f* pOutVec) {
    AreaInfo* waterAreaInfo = MR::getWaterAreaInfo(pInfo, rPos, getPlayer()->getAirGravityVec(), false);

    if (pOutVec != nullptr) {
        pOutVec->x = -pInfo->mCamWaterDepth;
        pOutVec->y = pInfo->_4;
    }

    if (mIsOnSurface) {
        if (mSurfaceOffset > 0.0f) {
            mSurfaceOffset = (0.97f * mSurfaceOffset) + (0.03f * pInfo->mWaveHeight);
        } else {
            mSurfaceOffset = (0.99f * mSurfaceOffset) + (0.01f * (pInfo->mWaveHeight / 2.0f));
        }
    } else {
        mSurfaceOffset = 0.8f * mSurfaceOffset;
    }
    _178 = pInfo->mEdgePos;
    mDistanceToWaterSurface = pInfo->mEdgeDistance;
    TVec3f currentFlow(pInfo->mStreamVec);

    f32 depthFactor = 1.0f;
    if (mWaterDepth > -500.0f) {
        depthFactor = -mWaterDepth / 500.0f;
    }

    f32 finalScale = MR::clamp(depthFactor, 0.0f, 1.0f);
    if (mIsOnSurface) {
        finalScale = 0.0f;
    }

    currentFlow.scale(finalScale);
    TVec3f smoothedFlow = currentFlow.scaleInline(0.05);

    mWaterStreamVel = mWaterStreamVel.scaleInline(0.95).translate(smoothedFlow);
    mWaterStreamDir = smoothedFlow;

    return waterAreaInfo;
}

const TVec3f& MarioSwim::getGravityVec() const {
    return MarioModule::getGravityVec();
}

bool MarioSwim::surfacePaddle() {
    if (mDashTimer != 0) {
        return false;
    }

    stopAnimation(static_cast< const char* >(nullptr), static_cast< const char* >(nullptr));
    changeAnimation("水上一掻き", static_cast< const char* >(nullptr));
    // Surface Paddle
    playSound("水面一掻き", -1);
    mDashTimer = 40;

    return true;
}

void MarioSwim::spin() {
    if (mJetTimer != 0) {
        if (mActor->isRequestSpin()) {
            if (mSpinTimer == 0 && mSpinDashTimer == 0 && mJetCooldownTimer == 0) {
                dropJet(false);
            }
        }
        return;
    }

    if (mKnockbackTimer != 0) {
        return;
    }

    if (mZSinkTimer != 0) {
        return;
    } else if (checkTrgA()) {
        if (mSpinTimer != 0) {
            mSpinTimer = 0;
            startSpinDash();
            return;
        }
    }
    if (mSpinTimer != 0) {
        mSpinTimer--;
        mForwardSpeed = 0.95f * mForwardSpeed;
        if (mSpinTimer == 0) {
            startSpinDash();
        }
    }

    if (mSpinDashTimer != 0) {
        mSpinDashTimer--;
    }

    if (!mActor->isRequestSpin()) {
        return;
    }
    if (mSpinTimer != 0) {
        return;
    }
    if (mSpinDashTimer != 0) {
        return;
    }
    if (mSpinDashTimer != 0) {
        return;
    }

    if (mSpinTimer != 0) {
        mSpinTimer = mActor->getConst().getTable()->mSwimSpinMissOfsTime;
        mForwardSpeed = 0.5f * mForwardSpeed;

    } else {
        mSpinTimer = 1;
    }
    mDashTimer = 0;
    _84 = mActor->getConst().getTable()->mSwimSpinSpeed;
    mSpinDashTimer = 0;
    _78 = false;

    stopAnimationUpper(nullptr, nullptr);
    if (!mIsOnSurface) {
        changeAnimation("水上スピン", static_cast< const char* >(nullptr));
        playSound("水面スピン開始", -1);
    } else {
        changeAnimation("水泳スピン", static_cast< const char* >(nullptr));
        playSound("水中スピン開始", -1);
    }

    mActor->setPunchHitTimer(40);
    playEffect("水泳スピン");
}

void MarioSwim::startSpinDash() {
    stopEffect("水泳スピン");

    if (_78) {
        stopAnimation("水泳スピン", static_cast< const char* >(nullptr));
        stopAnimation("水上スピン", static_cast< const char* >(nullptr));
        return;
    }

    mSpinDashTimer = mActor->getConst().getTable()->mSwimDashTime;
    mActionLockTimer = mActor->getConst().getTable()->mSwimSpinTime;

    if (mIsOnSurface) {
        mDashTimer = 20;
        mSwimYAccTimer = 50;
        mSpinDashTimer = 45;
        mActionLockTimer = 30;
    } else {
        mDashTimer = mActor->getConst().getTable()->mSwimSpinFrontTime;
        mSwimYAccTimer = mActor->getConst().getTable()->mSwimSpinYspeedTime;
    }

    if (!mIsOnSurface) {
        changeAnimation("水上スピン移動", static_cast< const char* >(nullptr));
    } else {
        changeAnimation("水泳スピン移動", static_cast< const char* >(nullptr));
    }
    mActor->setPunchHitTimer(mSpinDashTimer);

    if (gIsLuigi) {
        decOxygen(mActor->getConst().getTable()->mOxygenMax / 16);
    }
}

void MarioSwim::hitPunch(const TVec3f& rPunchDir) {
    if (mSpinTimer != 0) {
        mKnockbackTimer = 30;
        mForwardSpeed = 0.0f;
        mKnockbackVel = rPunchDir;
        _78 = true;

        stopAnimation("アッパーパンチ", static_cast< const char* >(nullptr));
    }
}

void MarioSwim::jet() {
    if (mActor->getCarrySensor() == nullptr) {
        mJetTimer = 0;
        mJetCooldownTimer = 0;
    }

    if (mJetCooldownTimer != 0) {
        mJetCooldownTimer--;
    }

    if (mJetTimer != 0) {
        MR::setStageBGMStateBit(4);

        f32 accFactor = calcRingAcc();
        f32 targetSpeed;

        if (accFactor != 1.0f) {
            targetSpeed = accFactor * mActor->getConst().getTable()->mSwimFrontMaxSpeed;
            if (_88 == 0) {
                if (targetSpeed < mActor->getConst().getTable()->mSwimFrontJetSpeed) {
                    targetSpeed = mActor->getConst().getTable()->mSwimFrontJetSpeed;
                }
            } else {
                if (targetSpeed < mActor->getConst().getTable()->mSwimFrontJetSpeedSlow) {
                    targetSpeed = mActor->getConst().getTable()->mSwimFrontJetSpeedSlow;
                }
            }
        } else {
            if (_88 == 0) {
                targetSpeed = mActor->getConst().getTable()->mSwimFrontJetSpeed;
            } else {
                targetSpeed = mActor->getConst().getTable()->mSwimFrontJetSpeedSlow;
            }
        }

        targetSpeed = 1.0f * targetSpeed;  // okay nintendo

        if (mJetTimer == 1) {
            if (checkLvlZ()) {
                MR::emitEffect(mActor->getCarrySensor()->mHost, "BrakeLamp");
                getPlayer()->_1C._9 = 1;
            }
        }

        mJetTimer++;

        if (checkLvlZ()) {
            if (checkTrgZ() || mStateTimer < 2) {
                playSound("亀ブレーキ", -1);
                MR::emitEffect(mActor->getCarrySensor()->mHost, "BrakeLamp");
            }
            getPlayer()->_1C._9 = 1;

            if (mForwardSpeed > targetSpeed) {
                mForwardSpeed *= 0.95f;
                return;
            }

            if (mForwardSpeed > (0.1f * targetSpeed)) {
                mForwardSpeed *= 0.96f;
                return;
            }

        } else {
            playSound("亀ジェット泳ぎ", -1);
            MR::forceDeleteEffect(mActor->getCarrySensor()->mHost, "BrakeLamp");
        }

        if (mForwardSpeed < targetSpeed) {
            if (mForwardSpeed < 1.0f) {
                mForwardSpeed = 1.0f;
            }
            mForwardSpeed *= 1.02f;

            if (mRingDashTimer != 0) {
                mForwardSpeed *= 1.5f;
            }
        } else {
            if (mForwardSpeed > targetSpeed) {
                mForwardSpeed *= 0.98f;
            } else {
                mForwardSpeed = targetSpeed;
            }
        }
    } else {
        ringDash();
    }
}

void MarioSwim::resetJet() {
    if (mJetTimer != 0) {
        if (isStatusActiveS(this)) {
            getAnimator()->changeDefault("水泳基本");
        }
    }
    mJetTimer = 0;
}

void MarioSwim::dropJet(bool isDamage) {
    if (mJetTimer == 0) {
        return;
    }
    mJetTimer = 0;

    if (mForwardSpeed > mActor->getConst().getTable()->mSwimFrontMaxSpeed) {
        mForwardSpeed = mActor->getConst().getTable()->mSwimFrontMaxSpeed;
    }

    if (isDamage) {
        changeAnimation("水泳ジェット終了", "水泳基本");
    } else {
        changeAnimation("水泳亀投げ", "水泳基本");
    }

    if (mActor->getCarrySensor()) {
        LiveActor* shell = mActor->getCarrySensor()->mHost;
        MR::forceDeleteEffect(shell, "BrakeLamp");
    }

    if (isDamage) {
        mActor->damageDropThrowMemoSensor();
    } else {
        mActor->releaseThrowMemoSensor();
    }

    mKnockbackTimer = 15;
    mKnockbackVel.zero();

    startPadVib(static_cast< u32 >(0));
}

bool MarioSwim::startJet(u32 type) {
    if (mJetTimer != 0) {
        return false;
    }

    _88 = type;
    mJetTimer = 1;

    forceStopSpin();
    // Swim Jet
    if (!isAnimationRun("水泳ジェット")) {
        // Swim Jet Start
        changeAnimation("水泳ジェット開始", "水泳ジェット");
    } else {
        changeAnimation(nullptr, "水泳ジェット");
    }

    mJetCooldownTimer = 20;
    return true;
}

void MarioSwim::forceStopSpin() {
    mSpinDashTimer = 0;
    mSpinTimer = 0;
    setYangleOffset(0.0f);
    _80 = 0.0f;
    _84 = 0.0f;

    // Swim Spin
    stopAnimation("水泳スピン", static_cast< const char* >(nullptr));
    stopAnimation("水泳スピン移動", static_cast< const char* >(nullptr));
    // Surface Spin
    stopAnimation("水上スピン", static_cast< const char* >(nullptr));
    stopAnimation("水上スピン移動", static_cast< const char* >(nullptr));

    stopEffect("水泳スピン");
}

void MarioSwim::addFaint(const TVec3f& rFaintDir) {
    if (getPlayer()->getDamageAfterTimer() != 0) {
        return;
    }

    if (mDamageType == 0) {
        Mario* player = getPlayer();
        player->_41E = 150;
    }

    dropJet(false);
    mKnockbackTimer = mActor->getConst().getTable()->mWaterInnerFaintTime;

    changeAnimation("水泳ダメージ", static_cast< const char* >(nullptr));
    playSound("水中ダメージ", -1);
    playSound("声水中ダメージ", -1);
    playSound("ダメージ", -1);

    startPadVib(2);
    mKnockbackVel = rFaintDir;
    mKnockbackVel.setLength(mActor->getConst().getTable()->mSwimFaintSpeed);
    mActionLockTimer = 0;
    forceStopSpin();
    mForwardSpeed = 0.0f;

    if (mDamageType == 0) {
        mActor->decLife(0);
        if (mActor->mHealth == 0) {
            mActor->forceGameOverNonStop();
        }
    }

    mDamageType = 0;
}

void MarioSwim::addDamage(const TVec3f& rDamageDir) {
    if (getPlayer()->getDamageAfterTimer() != 0) {
        return;
    }
    if (mDamageType == 0) {
        getPlayer()->_41E = 180;
    }

    dropJet(false);
    mForwardSpeed = 0.0f;

    if (mDamageType == 0) {
        playSound("ダメージ", -1);
    } else {
        playSound("小ダメージ", -1);
    }

    if (mIsOnSurface) {
        TVec3f surfaceKnockback;
        MR::vecKillElement(rDamageDir, mUpVec, &surfaceKnockback);
        surfaceKnockback.setLength(mActor->getConst().getTable()->mSwimSurfaceDamageSpeed);

        TVec3f jumpVel = mUpVec.scaleInline(mActor->getConst().getTable()->mWaterSurfaceDamageJump).translate(surfaceKnockback);
        getPlayer()->tryForcePowerJump(jumpVel, false);
        changeAnimationNonStop("水上ダメージ中");
        playSound("水面ダメージ", -1);
        playSound("声小ダメージ", -1);

        mFrontVec = -surfaceKnockback;
        mNextAction = EXIT_ACTION_POWER_JUMP;
        getPlayer()->closeStatus(this);

        if (mDamageType == 0) {
            mActor->decLife(0);
            if (mActor->mHealth == 0) {
                mActor->forceGameOverNonStop();
            }
        }
        mDamageType = 0;
    } else {
        mKnockbackTimer = mActor->getConst().getTable()->mWaterInnerDamageTime;
        changeAnimation("水泳ダメージ中", static_cast< const char* >(nullptr));

        mKnockbackVel = rDamageDir;
        mKnockbackVel.setLength(mActor->getConst().getTable()->mSwimDamageSpeed);

        playSound("水中ダメージ", -1);
        playSound("声水中ダメージ", -1);

        if (mDamageType == 0) {
            decLife();
        }
        mDamageType = 0;
    }

    if (_AD != 0) {
        mKnockbackVel.setLength(mActor->getConst().getTable()->mSwimDamageSpeedEx1);
        mKnockbackTimer = mActor->getConst().getTable()->mSwimDamageTimerEx1;
        _AD = 0;
    }

    startPadVib(3);
    playEffect("ダメージ");
    mActionLockTimer = 0;
    forceStopSpin();
    mForwardSpeed = 0.0f;

    TVec3f postureDir;
    MR::vecKillElement(rDamageDir, getGravityVec(), &postureDir);

    if (MR::normalizeOrZero(&postureDir) == 0) {
        mFrontVec = -postureDir;
    }
}

void MarioSwim::updateTilt() {
    f32 targetTiltX = 0.0f;
    f32 blendX = 0.1f;

    if (mIsOnSurface) {
        if (getStickP() > 0.1f) {
            f32 stickX = getStickX();

            f32 angle = (stickX * PI) / 6.0f;
            f32 absAngle = __fabsf(angle);

            TVec3f padDir = getWorldPadDir();
            f32 dot = mFrontVec.dot(padDir);
            f32 scale = MR::clamp(1.0f - dot, 0.0f, 1.0f);

            targetTiltX = absAngle * scale;

            TVec3f cross;
            PSVECCrossProduct(&mFrontVec, &padDir, &cross);
            if (cross.dot(mUpVec) > 0.0f) {
                targetTiltX = -targetTiltX;
            }

            if (__fabsf(mCurrentTiltX) < __fabsf(targetTiltX)) {
                blendX = 0.01f;
            } else {
                blendX = 0.03f;
            }
        }
    } else {
        targetTiltX = (getStickX() * PI) / 5.0f;
    }

    if (isAnimationRun("水泳ターン左") || isAnimationRun("水泳ターン右") || isAnimationRun("水泳ターン下")) {
        f32 stickX = getStickX();
        targetTiltX = (MR::clamp(stickX / 0.7f, -1.0f, 1.0f) * PI) * 0.125f;
    }

    f32 targetTiltY = 0.0f;

    if (!mIsOnSurface) {
        if (mIdleWaitTimer == 0) {
            f32 scaledTilt = (getStickY() * PI);
            targetTiltY = scaledTilt / 12.0f;

            if (getStickY() > 0.0f && mVerticalAngle <= 2.0f / 3.0f * PI) {
                targetTiltY = getStickY() * PI * 0.125f;
            }
        } else if (getStickY() < 0.0f) {
            f32 divisor = mActor->getConst().getTable()->mSwimTiltZup;
            targetTiltY = (getStickY() * PI) / divisor;
        } else if (getStickY() > 0.0f) {
            f32 divisor = mActor->getConst().getTable()->mSwimTiltZdown;
            targetTiltY = (getStickY() * PI) / divisor;
        } else if (mIdleWaitTimer != 0) {
            f32 fade = static_cast< f32 >(mIdleWaitTimer) / 120.0f;
            if (fade > 1.0f) {
                fade = 1.0f;
            }
            targetTiltY = targetTiltY - (fade * (PI * 0.1f));
        } else {
            targetTiltY = 0.0f;
        }
    }

    mCurrentTiltX = (blendX * targetTiltX) + ((1.0f - blendX) * mCurrentTiltX);
    mCurrentTiltY = (0.1f * targetTiltY) + (0.9f * mCurrentTiltY);

    Mtx mtxRotX, mtxRotZ;
    PSMTXRotRad(mtxRotX, 'X', mCurrentTiltX);
    PSMTXRotRad(mtxRotZ, 'Z', mCurrentTiltY);
    PSMTXConcat(mtxRotX, mtxRotZ, mUpperBodyJointMtx);

    u16 jointID = getAnimator()->getUpperJointID();
    setJointGlobalMtx(jointID, mUpperBodyJointMtx);
}

void MarioSwim::pushedByWaterWall() {
    if (!(mDistanceToWaterSurface < 0.0f)) {
        f32 wallDistanceLimit = 80.0f;
        if (mDistanceToWaterSurface < wallDistanceLimit) {
            TVec3f pushDir = getTrans() - _178;
            pushDir.setLength(wallDistanceLimit - mDistanceToWaterSurface);
            getPlayer()->push(pushDir);

            if (MR::normalizeOrZero(&pushDir) == 0) {
                const TVec3f& headVec = getPlayer()->mHeadVec;
                f32 dot = pushDir.dot(-headVec);

                if (_44 != 0) {
                    dot = 1.0f;
                }

                if (!(dot < 0.0f)) {
                    if (dot < 0.995f) {
                        TVec3f newHead;
                        MR::vecKillElement(headVec, pushDir, &newHead);
                        MR::normalize(&newHead);

                        MR::vecBlendSphere(getPlayer()->mHeadVec, newHead, &newHead, 0.2f);
                        getPlayer()->forceSetHeadVecKeepSide(newHead);
                        MR::vecKillElement(newHead, getGravityVec(), &mFrontVec);

                        Mario* player = getPlayer();
                        mVerticalAngle = MR::diffAngleAbs(player->mHeadVec, -getGravityVec());

                    } else {
                        MR::vecBlendSphere(mFrontVec, pushDir, &mFrontVec, 0.2f);
                        _44 = 30;
                    }
                }
            }
        }
    }
}

bool MarioSwim::tryJetAttack(HitSensor* pHitSensor) {
    if (mActor->tryJetAttack(pHitSensor) == true) {
        dropJet(1);

        const TVec3f& trans = getTrans();
        TVec3f damageDir = trans - pHitSensor->mPosition;
        addDamage(damageDir);

        return true;
    } else {
        return false;
    }
}

void MarioSwim::hitHead(const HitInfo* pHit) {
    if (mJetTimer != 0) {
        if (sendPunch(pHit->mParentTriangle.mSensor, false) == 1) {
            startCamVib(0);
            return;
        }

        if (tryJetAttack(pHit->mParentTriangle.mSensor)) {
            return;
        }

        const TVec3f* normal = pHit->mParentTriangle.getNormal(0);
        if (normal->dot(getGravityVec()) > 0.0f) {
            Mario* player = getPlayer();
            TVec3f direction;
            TVec3f velocityPart;

            f32 element = MR::vecKillElement(player->_1FC, -*normal, &velocityPart);
            velocityPart += normal->scaleInline(element).scaleInline(1.5f);

            if (velocityPart.length() < 10.0f) {
                velocityPart.setLength(10.0f);
            }
            MR::vecKillElement(velocityPart, getGravityVec(), &direction);

            direction.length();
            if (MR::normalizeOrZero(&direction) == false) {
                mFrontVec = direction;
            }
        }
    }

    TVec3f pushBack = getTrans() - pHit->mHitPos;
    MR::vecKillElement(pushBack, *pHit->mParentTriangle.getNormal(0), &pushBack);
    pushBack.setLength(5.0f);
    addVelocity(pushBack);
}

void MarioSwim::hitPoly(u8 type, const TVec3f& rNormal, HitSensor* pSensor) {
    if (mStateTimer <= 120) {
        return;
    }
    if (!mIsOnSurface) {
        return;
    }

    const TVec3f& gravity = getPlayer()->getAirGravityVec();
    f32 dot = rNormal.dot(gravity);
    f32 invDot = -dot;

    const TVec3f& swimGravity = getAirGravityVec();
    TVec3f negNormal = -rNormal;

    f32 angle = MR::diffAngleAbsHorizontal(negNormal, mFrontVec, swimGravity);
    if (invDot >= 0.5f && angle <= PI / 4) {
        _22 = true;
    }
}

void MarioSwim::hitWall(const TVec3f& rNormal, HitSensor* pSensor) {
    if (mActor->_424 == 0) {
        f32 floorDot = -rNormal.dot(getPlayer()->getAirGravityVec());

        if (!mIsOnSurface && floorDot > 0.70700002f) {
            _1F = true;
            return;
        } else if (mIsOnSurface) {
            if (MR::diffAngleAbsHorizontal(-getAirGravityVec(), mFrontVec, rNormal) < RAD_TO_DEG(45)) {
                mWallStickTimer = 6;
            }

            if (mStateTimer <= 0x78) {
                return;
            }

            // If on surface and hitting something slope-like, treat as slide/floor interaction
            if (floorDot >= 0.5f) {
                _22 = true;
                return;
            }
        }

        bool isHit = false;

        if (!mIsOnSurface && mForwardSpeed > 1.0f) {
            TVec3f projectedVelocity;
            MR::vecKillElement(getPlayer()->_1FC, rNormal, &projectedVelocity);

            if (projectedVelocity.dot(mUpVec) > 0.0f) {
                if (getStickY() <= 0.0f) {
                    mVerticalAngle -= 0.1f;
                }
            } else {
                if (getStickY() >= 0.0f) {
                    mVerticalAngle += 0.1f;
                }
            }
        }

        mWallStickTimer = 6;
        mDashTimer = 0;

        if (mJetTimer != 0 || (!MR::isNearZero(mWaterStreamDir) && mActionLockTimer != 0)) {
            isHit = false;
            if (mJetTimer != 0 && sendPunch(pSensor, false)) {
                startCamVib(0);
                isHit = true;
            } else {
                if (mJetTimer != 0) {
                    if (tryJetAttack(pSensor)) {
                        return;
                    }
                }
                TVec3f tangent;
                Mario* player = getPlayer();
                f32 speedIntoWall = MR::vecKillElement(player->_1FC, -rNormal, &tangent);

                TVec3f velocityAfterHit;
                tangent += rNormal.scaleInline(speedIntoWall).scaleInline(1.5f);
                MR::vecKillElement(tangent, getGravityVec(), &velocityAfterHit);
                f32 velocityAfterHitLength = velocityAfterHit.length();

                bool isReflect = true;
                f32 speedReduction = mActor->getConst().getTable()->mTurboReductionHitWall;

                if (!MR::normalizeOrZero(&velocityAfterHit)) {
                    if (rNormal.dot(getGravityVec()) > 0.1f && mVerticalAngle < 1.5707964f) {
                        mVerticalAngle = PI - mVerticalAngle;
                    }

                    mKnockbackTimer = 20;
                    mKnockbackVel = rNormal.scaleInline(6.0f);

                    if (MR::isNearZero(mWaterStreamDir)) {
                        f32 impactAngle = MR::diffAngleAbsHorizontal(mFrontVec, rNormal, getAirGravityVec());

                        if (impactAngle >= 2.6179941f) {
                            f32 headAngle = MR::diffAngleAbs(getPlayer()->mHeadVec, rNormal);

                            if (headAngle <= DEG_TO_RAD(150)) {
                                if (headAngle >= 2 * PI) {
                                    TVec3f cross1;
                                    PSVECCrossProduct(&rNormal, &getPlayer()->mHeadVec, &cross1);
                                    MR::normalizeOrZero(&cross1);

                                    TVec3f cross2;
                                    PSVECCrossProduct(&getPlayer()->mHeadVec, &cross1, &cross2);
                                    MR::normalizeOrZero(&cross2);

                                    getPlayer()->forceSetHeadVecKeepSide(cross2);

                                    Mario* player = getPlayer();
                                    mVerticalAngle = MR::diffAngleAbs(player->mHeadVec, -getAirGravityVec());
                                }
                                isReflect = false;

                            } else {
                                mKnockbackTimer = 60;
                                mKnockbackVel = rNormal.scaleInline(16.0f);
                            }
                        } else {
                            TVec3f glanceDir;
                            MR::vecKillElement(mFrontVec, rNormal, &glanceDir);
                            if (!MR::normalizeOrZero(&glanceDir)) {
                                bool isTurn = false;
                                mFrontVec = glanceDir;
                                speedReduction = 0.8f;
                                isReflect = false;
                            }
                        }
                    } else {
                        if (velocityAfterHit.dot(mWaterStreamDir) > 0.0f) {
                            mFrontVec = velocityAfterHit;
                        } else {
                            mKnockbackTimer = 60;
                            mKnockbackVel = rNormal.scaleInline(16.0f);
                        }
                    }
                }

                if (mJetTimer != 0 && isReflect) {
                    playEffectTrans("壁ヒット", getPlayer()->getWallPos());
                    playSound("亀壁ヒット", -1);
                    changeAnimation("水泳ジェット壁ターン", static_cast< const char* >(nullptr));
                }

                mForwardSpeed *= speedReduction;
                isHit = true;
            }
        }

        // Spin Attack Collision
        if (mSpinDashTimer != 0) {
            if (!sendPunch(pSensor, true)) {
                mForwardSpeed = 0.0f;
                forceStopSpin();
                changeAnimation("水泳壁ヒット", static_cast< const char* >(nullptr));
                playEffectTrans("壁ヒット", getPlayer()->getWallPos());
                startCamVib(0);
                mKnockbackTimer = 60;
                mKnockbackVel = rNormal.scaleInline(6.0f);
            }
            isHit = true;
        } else if (mSpinTimer != 0) {
            if (!sendPunch(pSensor, false)) {
                mSpinTimer = 0;
                mForwardSpeed = 0.0f;
                playEffectTrans("壁ヒット", getPlayer()->getWallPos());
                startCamVib(2);
                mKnockbackTimer = 30;
                mKnockbackVel = rNormal.scaleInline(6.0f);
            }
            isHit = true;
        }

        // Ring Dash Collision
        if (mRingDashTimer != 0) {
            if (mJetTimer != 0) {
                dropJet(true);
            }
            mForwardSpeed = 0.0f;
            changeAnimation("水泳壁ヒット", static_cast< const char* >(nullptr));
            playEffectTrans("壁ヒット", getPlayer()->getWallPos());
            startCamVib(0);
            mKnockbackTimer = 60;
            mKnockbackVel = rNormal.scaleInline(6.0f);
            isHit = true;
        }

        mRingDashTimer = 0;
        mRingDashChargeTimer = 0;

        if (isHit) {
            startPadVib(2);
        }

        // Wall Push / Kick Logic check
        if (rNormal.dot(getGravityVec()) > 0.8f) {
            TVec3f* worldPadDir = &getWorldPadDir();
            if (getStickP() != 0.0f) {
                if (checkTrgA()) {
                    if (mKnockbackTimer == 0) {
                        mKnockbackTimer = 60;
                    }
                    TVec3f gravVecScaled = getGravityVec().scaleInline(10.0f);
                    TVec3f worldPadDirScaled = (*worldPadDir).scaleInline(10.0f);
                    mKnockbackVel = gravVecScaled.translate(worldPadDirScaled);
                }
            }
        }

        stopEffect("水泳スピン");
    }
}

void MarioSwim::decLife() {
    mDrownTimer = 0;
    mActor->decLife(0);
    if (mActor->mHealth == 0) {
        mActor->forceGameOver();

        TVec3f gravityVec = -getGravityVec();
        getPlayer()->forceSetHeadVecKeepSide(gravityVec);

        u16 upperJoint = getAnimator()->getUpperJointID();
        setJointGlobalMtx(upperJoint, nullptr);
    }
}

void MarioSwim::incLife() {
    mDrownTimer = 0;
    mActor->incLife(1);
    incOxygen();
}

void MarioSwim::incOxygen() {
    if (mOxygen == mActor->getConst().getTable()->mOxygenMax) {
        return;
    }
    mOxygen += (mActor->getConst().getTable()->mOxygenMax / 8);

    if (mOxygen > mActor->getConst().getTable()->mOxygenMax) {
        // 0x64: sth r0, 0xea(r3)
        mOxygen = mActor->getConst().getTable()->mOxygenMax;
    }

    if (mOxygen == mActor->getConst().getTable()->mOxygenMax) {
        // Play "Full" sound
        playSound("酸素回復最大", -1);
    } else {
        // Play "Recovering" sound
        playSound("酸素回復", -1);
    }

    mOxygenWarningTimer = 120;
    mOxygenDecreaseDelay = 120;
}

void MarioSwim::decOxygen(u16 amount) {
    if (mOxygen == 0) {
        return;
    }

    MarioActor* actor = mActor;

    if (actor->_934 != 0) {
        return;
    }
    if (actor->_EA4 != 0) {
        return;
    }

    if (isStatusActiveID(0x22)) {
        return;
    }

    if (mOxygen < amount) {
        mOxygen = 0;
    } else {
        mOxygen -= amount;
    }

    if (mOxygen == 0) {
        playSound("無酸素警告", -1);
    }
}

void MarioSwim::draw3D() const {
    WaterInfo camWaterInfo;
    MR::getWaterAreaObj(&camWaterInfo, getCamPos());
    if (camWaterInfo.isInWater()) {
        mMarineSnow->view();
    } else {
        mMarineSnow->clear();
    }

    mMarineSnow->draw(getTrans(), mUpVec, -mWaterDepth);
}

void MarioSwim::onSurface() {
    if (!mIsOnSurface) {
        mIsOnSurface = true;
    }
}

void MarioSwim::updateLifeByTime() {
    updateOxygenWatch();

    if (mActor->_EA4) {
        mColdWaterDamageInterval = 120;
    } else {
        bool isColdWater = false;
        const WaterArea* waterArea = mWaterInfo.mWaterArea;
        s32 objArg = -1;
        if (waterArea) {
            objArg = MR::getAreaObjArg(waterArea, 1);
        }
        if (objArg == 1) {
            isColdWater = true;
        }
        if (isColdWater) {
            if (getPlayer()->isDamaging()) {
                mColdWaterDamageInterval = 120;
            }
            getPlayer()->_1C._3 = 1;

            if (!mColdWaterDamageInterval) {
                mColdWaterDamageInterval = 120;
            } else {
                mColdWaterDamageInterval--;
                if (mColdWaterDamageInterval == 0) {
                    doDecLifeByCold();
                    mColdWaterDamageInterval = 120;
                }
            }
        } else {
            mColdWaterDamageInterval = 0;
        }

        if (isColdWater || mIsOnSurface) {
            mDrownTimer = 0;
            mOxygenDecreaseDelay = 120;

            if (mOxygen != mActor->getConst().getTable()->mOxygenMax) {
                mOxygen += 32;
                if (mOxygen > mActor->getConst().getTable()->mOxygenMax) {
                    playSound("酸素回復最大", -1);
                    mOxygen = mActor->getConst().getTable()->mOxygenMax;
                } else {
                    playSound("酸素回復", -1);
                }

                mOxygenWarningTimer = 180;
            }
        } else {
            if (mOxygen) {
                if (mOxygenDecreaseDelay) {
                    mOxygenDecreaseDelay--;
                } else {
                    decOxygen(1);
                }
            } else {
                mDrownTimer++;
                u32 damageInterval = mActor->getConst().getTable()->mWaterDecInterval;
                if (mDrownTimer >= damageInterval) {
                    doDecLifeByCold();
                }
            }
        }
    }
}

void MarioSwim::doDecLifeByCold() {
    if (mActor->mHealth != 0) {
        decLife();

        if (mJetTimer != 0) {
            changeAnimation("水泳ダメージ亀", static_cast< const char* >(nullptr));
        } else {
            changeAnimation("水泳ダメージ", static_cast< const char* >(nullptr));
        }

        playSound("水中ダメージ", -1);
        playSound("声冷水ダメージ", -1);
        playSound("ダメージ", -1);
        startPadVib(2);

        mActor->_BC4 = 16;
    }
}

bool MarioSwim::passRing(const HitSensor* pSensor) {
    const TVec3f& ringPos = pSensor->mPosition;
    if (mRingDashTimer == 0) {
        mRingDashChargeTimer = mActor->getConst().getTable()->mSwimRingDashChargeTime;

        if (mJetTimer == 0) {
            changeAnimation("リングダッシュ準備", static_cast< const char* >(nullptr));
        } else {
            changeAnimation("カメ持ちリング準備", static_cast< const char* >(nullptr));
        }
    }

    if (mRingDashChargeTimer != 0) {
        getPlayer()->push((ringPos - getTrans()).scaleInline(0.25f));
    }

    DashRing* host = static_cast< DashRing* >(pSensor->mHost);
    mRingDashMaxDuration = host->_AC;
    mRingDashSpeedScale = host->_B0;
    mRingDashTimer = host->_A8;

    playSound("亀加速", -1);

    return true;
}

f32 MarioSwim::calcRingAcc() {
    if (mRingDashTimer != 0) {
        if (mRingDashChargeTimer != 0) {
            mRingDashChargeTimer--;

            if (mRingDashChargeTimer == 1) {
                MarioActor* actor = mActor;
                MarioConstTable* table = actor->mConst->getTable();

                actor->_1AA = table->mStarPieceFogTime;
                actor->_1AC = 0.0f;
                actor->_1B0.r = 0xFF;
                actor->_1B0.g = 0xFF;
                actor->_1B0.b = 0xFF;
                actor->_1B0.a = 0;
                actor->_1B5 = 0;
            }
            if (mRingDashChargeTimer == 0) {
                startPadVib(3);
                if (mJetTimer == 0) {
                    changeAnimation("カメ持ちリング", static_cast< const char* >(nullptr));
                } else {
                    changeAnimation("リングダッシュ", static_cast< const char* >(nullptr));
                }
            }

            return 1.0f;
        } else {
            mBlurOffset += 0.5f;
            if (mBlurOffset > 30.0f) {
                mBlurOffset = 30.0f;
            }

            f32 result = mRingDashSpeedScale;
            if (mRingDashTimer < mRingDashMaxDuration) {
                f32 ratio = static_cast< f32 >(mRingDashTimer) / static_cast< f32 >(mRingDashMaxDuration);
                result = (1.0f - ratio) + (mRingDashSpeedScale * ratio);
            }

            mRingDashTimer--;
            return result;
        }
    } else {
        if (mBlurOffset > 0.0f) {
            mBlurOffset -= 0.5f;
        } else {
            mBlurOffset = 0.0f;
        }
        return 1.0f;
    }
}

void MarioSwim::ringDash() {
    f32 accFactor = calcRingAcc();

    if (mRingDashTimer != 0) {
        const MarioConstTable* table = mActor->getConst().getTable();
        f32 targetSpeed = accFactor * mActor->getConst().getTable()->mSwimFrontMaxSpeed;

        if (mForwardSpeed < targetSpeed) {
            if (mForwardSpeed < 1.0f) {
                mForwardSpeed = 1.0f;
            }

            f32 ratio = targetSpeed / mForwardSpeed;
            if (ratio > 1.5f) {
                mForwardSpeed = 1.5f * mForwardSpeed;
            } else {
                mForwardSpeed = targetSpeed;
            }
        } else {
            if (mForwardSpeed > targetSpeed) {
                f32 ratio = targetSpeed / mForwardSpeed;
                if (ratio < 0.98f) {
                    mForwardSpeed = 0.98f * mForwardSpeed;
                } else {
                    mForwardSpeed = targetSpeed;
                }
            } else {
                mForwardSpeed = targetSpeed;
            }
        }
    }
}

void MarioSwim::resetAndFixPose() {
    mActionLockTimer = 0;
    mSinkTimer = 0;
    mSwimYAccTimer = 0;
    mZSinkTimer = 0;
    mIdleWaitTimer = 0;
    mBuoyancyDelayTimer = 0;
    mDashTimer = 0;
    mSpinDashTimer = 0;
    mSpinTimer = 0;

    mForwardSpeed = 0.0f;
    mStickInputYInertia = 0.0f;
    mStickInputXInertia = 0.0f;
    mBuoyancySpeed = 0.0f;

    MR::vecKillElement(getFrontVec(), getGravityVec(), &mFrontVec);

    Mario* player = getPlayer();
    mVerticalAngle = MR::diffAngleAbs(player->mHeadVec, -getGravityVec());
}

f32 MarioSwim::getStickY() const {
    f32 stickY = MarioModule::getStickY();

    if (mIsSinking && stickY < 0.0f) {
        if (stickY > -0.5f) {
            stickY = 0.0f;
        } else {
            stickY = (0.5f + stickY) / 0.5f;
        }
    }

    return stickY;
}

void MarioSwim::updateOxygenWatch() {
    if (mOxygen <= (mActor->getConst().getTable()->mOxygenMax / 4)) {
        mOxygenWarningTimer = 180;
    }
    if (mOxygen == (mActor->getConst().getTable()->mOxygenMax / 4)) {
        playSound("酸素減少警告", -1);
    }
    if (mOxygen == (mActor->getConst().getTable()->mOxygenMax / 2)) {
        mOxygenWarningTimer = 180;
        playSound("酸素減少警告", -1);
    }

    if (mOxygenWarningTimer != 0) {
        mOxygenWarningTimer--;
    }
}

void MarioSwim::addVelocity(const TVec3f& rVelocity) {
    if (mIsOnSurface && mNextAction != EXIT_ACTION_NONE) {
        const TVec3f& gravity = getAirGravityVec();
        TVec3f horizontalVel;
        f32 verticalMag = MR::vecKillElement(rVelocity, gravity, &horizontalVel);
        if (verticalMag < 0.0f) {
            MarioModule::addVelocity(horizontalVel);
        } else {
            MarioModule::addVelocity(rVelocity);
        }
    } else {
        MarioModule::addVelocity(rVelocity);
    }
}

void MarioSwim::addVelocity(const TVec3f& rVelocity, f32 scale) {
    if (mIsOnSurface && mNextAction != EXIT_ACTION_NONE && mWaterDepth >= 0.0f) {
        const TVec3f& gravity = getAirGravityVec();
        TVec3f horizontalVel;
        f32 verticalMag = MR::vecKillElement(rVelocity, gravity, &horizontalVel);

        if (verticalMag < 0.0f) {
            MarioModule::addVelocity(horizontalVel, scale);
        } else {
            MarioModule::addVelocity(rVelocity, scale);
        }
    } else {
        MarioModule::addVelocity(rVelocity, scale);
    }
}

void MarioSwim::doJetJump(u8 type) {
    if (!getPlayer()->_1C._7 && mRingDashTimer == 0) {
        f32 verticalBoost = 15.0f;
        f32 horizontalSpeed = 12.0f;

        if (checkLvlA()) {
            horizontalSpeed = 18.0f;
            verticalBoost = 30.0f;
        } else if (type == 0) {
            getPlayer()->_10.digitalJump = true;
        }

        const MarioConstTable* table = mActor->getConst().getTable();
        f32 speedRatio = MR::clamp(mForwardSpeed / table->mSwimToWalkSpd, 0.0f, 1.0f);

        mUpVec.dot(getPlayer()->mHeadVec);
        f32 verticalScale = 1.0f;
        f32 horizontalScale = 1.25f;

        TVec3f jumpDir;
        MR::vecKillElement(mActor->getLastMove(), mUpVec, &jumpDir);
        MR::normalizeOrZero(&jumpDir);

        getPlayer()->mMovementStates._5 = 0;
        getPlayer()->mMovementStates._2F = 0;
        getPlayer()->_278 = speedRatio;

        TVec3f finalVelocity =
            jumpDir.scaleInline(horizontalSpeed).scaleInline(horizontalScale).translate(mUpVec.scaleInline(verticalBoost).scaleInline(verticalScale));

        getPlayer()->tryForcePowerJump(finalVelocity, false);

        mNextAction = EXIT_ACTION_POWER_JUMP;
        changeAnimationNonStop("水泳ジェット");
    }
}

f32 MarioSwim::getBlurOffset() const {
    return mBlurOffset;
}
