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
#include <cstddef>

namespace {
    f32 cFrontAcc[] = {-0.2f, -0.2f, -0.2f, -0.1f, -0.1f, -0.1f, -0.1f, -0.1f, -0.0f, -0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                       0.0f,  0.0f,  0.0f,  0.2f,  0.2f,  0.2f,  0.3f,  0.3f,  0.4f,  0.5f,  0.5f, 0.5f, 0.5f, 0.5f,
                       0.4f,  0.4f,  0.3f,  0.3f,  0.2f,  0.2f,  0.1f,  0.1f,  0.1f,  0.1f,  0.1f, 0.0f};

    f32 cFrontAccSpin[] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 8.0f,  6.0f,  4.0f,  2.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,
                           0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f};

    f32 cFrontAccSpinSurface[] = {0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  8.0f,  6.0f,  4.0f,  2.0f,  1.0f,
                                  -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f};
    f32 cWeightTable[] = {
        1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    };

    f32 cWeightTableSP[] = {0.0f, 0.0f, 0.75f, 0.25f};
    f32 cSwimWeightTable[] = {0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};
    f32 cAnimSpeeds[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    f32 cLimitAngleSink = 0.0f;
    f32 cNeutralAngleWait = 0.0f;
    f32 cLimitAngleWait = 0.0f;
    f32 cUpperAngleWait = 0.0f;
    f32 cWaterSurfaceRange = 160.0f;
    f32 cTurnMotionSpeed = 5.0f;
}  // namespace

inline f32 getSpecialNumber() {
    return 1.74532938004f;
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

inline f32 getSwimValue(f32 stick, u32 index, const MarioConstTable* table) {
    if (index == 2)
        return table->mSwimRotXIne;
    if (index == 3)
        return table->mSwimRotZIne;
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
    _A0 = rKnockbackVec;
    _AE = damage;
    _4C = 0.0f;
    _50 = 0.0f;
    _54 = 0.0f;
    _7C = 0;
    _7A = 0;
    _34 = 0;
    _2E = 0;
    _32 = 0;
    _2C = damage;
    _30 = 0;
    _24 = 0x12C;
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
    _1A = 0;
    _1B = 0;
    _1C = 0;
    _1D = 0;
    _1E = 0;
    _1F = 0;
    mIsSwimmingAtSurface = false;
    _21 = 0;
    _22 = 0;
    _24 = 0;
    _28 = 0;
    _2C = 0;
    _2E = 0;
    _30 = 0;
    _32 = 0;
    _34 = 0;
    _36 = 0;
    _38 = 0;
    _3A = 0;
    _3C = 0;
    _3E = 0;
    _40 = 0;
    _42 = 0;
    _44 = 0;
    mSurfaceOffset = 0.0f;
    _4C = 0.0f;
    _50 = 0.0f;
    _54 = 0.0f;
    _58 = 0.0f;
    _5C = 0.0f;

    _60.x = 1.0f;
    _60.y = 0.0f;
    _60.z = 0.0f;

    mUpVec.x = 0.0f;
    mUpVec.y = 1.0f;
    mUpVec.z = 0.0f;

    _78 = 0;
    _7A = 0;
    _7C = 0;
    _80 = 0.0f;
    _84 = 0.0f;
    _88 = 0;
    mJetTimer = 0;
    mRingDashTimer = 0;
    mRingDashChargeTimer = 0;
    mRingDashMaxDuration = 0;
    mRingDashSpeedScale = 0.0f;
    mBlurOffset = 0.0f;
    _9C = 0;
    _9D = 0;
    mNextAction = EXIT_ACTION_NONE;
    _9F = 0xFF;
    _A0.zero();
    _AC = 0;
    _AD = 0;
    _AE = 0;
    PSMTXIdentity(_B0.toMtxPtr());
    _E8 = 0;
    _E0 = 0.0f;
    _E4 = 0.0f;
    _EC = 0;
    _EE = 0x78;
    _F0 = 0;
    mSwimState = SWIM_STATE_NONE;
    _148.zero();
    _154.zero();
    mSurfacePos.set(0.0f, 1.0f, 0.0f);
    mSurfaceNorm.zero();
    _178.zero();
    _184.zero();
    _190.zero();
    _1B0 = 0;
    mWaterDepth = 0.0f;
    _1A0 = 0.0f;
    mDistToFloor = 0.0f;
    _1A8 = 0.0f;
    mDistanceToWaterSurface = 0.0f;
    _1B2 = 0;
    _1B4 = 0.0f;
    _1B8 = 0.0f;
    mMarineSnow = new MarineSnow();
}

void MarioSwim::init() {
    mOxygen = mActor->getConst().getTable()->mOxygenMax;
}

bool MarioSwim::start() {
    _7C = 0;
    _4C = 0.0f;
    _50 = 0.0f;
    _54 = 0.0f;
    _7A = 0;
    _E8 = 0;
    mOxygen = mActor->getConst().getTable()->mOxygenMax;
    _EE = 0x78;
    _F0 = 0;
    _AC = 0;
    _44 = 0;
    _EC = 0xB4;
    _42 = 0;
    mNextAction = EXIT_ACTION_NONE;
    mRingDashTimer = 0;
    mRingDashChargeTimer = 0;
    mBlurOffset = 0.0f;
    _3C = 0;
    _80 = 0.0f;
    _9F = 0xFF;
    _AE = 0;
    _A0.zero();
    _28 = 0;
    _E0 = 0.0f;
    _E4 = 0.0f;
    _1B0 = 0;
    _24 = 0;
    mSurfaceOffset = 0.0f;
    _3E = 0;
    _184.zero();
    _190.zero();
    _1B2 = 0;
    _1B4 = 0.0f;
    _1F = 0;
    _22 = 0;
    _58 = 0.0f;

    if (!getPlayer()->getMovementStates().jumping || getPlayer()->mVerticalSpeed <= 100.0f) {
        _58 = mActor->getConst().getTable()->mSwimUpMaxSpeed;
    }

    _9C = 1;
    if (checkLvlA()) {
        _9C = 0;
    }

    _5C = 0.523598790169f;
    onSurface();
    if ((mWaterDepth < -200.0f || mActor->_F74) && mIsOnSurface) {
        mIsOnSurface = false;
    }

    _1B = 0;
    u32 r1e = 0, r1d = 0;
    _1D = 0;
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

    _60 = getFrontVec();
    MR::normalize(&_60);
    stopAnimationUpper(nullptr, nullptr);
    // Basic Swimming
    changeAnimation(nullptr, "水泳基本");
    changeAnimationNonStop("水泳基本");
    getPlayer()->mMovementStates._1 = false;
    f32 fr1f = mDistToFloor;

    _2C = 0;
    _32 = 0;
    _2E = 0;
    _34 = 0;
    _36 = 0;
    _38 = 0;
    _3A = 0;
    _1C = 0;
    

    if (fr1f < -mWaterDepth) {
        MarioActor* actor = mActor;
        actor->emitEffectWaterColumn(-mSurfacePos, getTrans() - mSurfacePos.scaleInline(fr1f));
    } else {
        mActor->emitEffectWaterColumn(mSurfacePos, mSurfaceNorm);
    }
    if (getPlayer()->getMovementStates().jumping && getPlayer()->mVerticalSpeed > 100.0f || r1e) {
        TVec3f stack_44;
        _24 = 0;
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
            _34 = 0x3C;
            _54 = ftmp2 * ftmp;
        } else if (getPlayer()->getMovementStates()._B || r1e) {
            _54 = 10.0f;

            if (mJetTimer != 0) {
                changeAnimationNonStop("水泳ジェット");
            } else {
                changeAnimation("水泳ジャンプダイブ回転", static_cast< const char* >(nullptr));
            }
            playEffect("水面Ｚ沈降");

            switch (r1e) {
            case 0:
            case 1:
                _2C = mActor->getConst().getTable()->mSwimSpecStartTimerA[0];
                _2E = mActor->getConst().getTable()->mSwimSpecStartTimerB[0];
                break;
            case 2:
                _2C = mActor->getConst().getTable()->mSwimSpecStartTimerA[1];
                _2E = mActor->getConst().getTable()->mSwimSpecStartTimerB[1];
                break;
            case 3:
                _2C = mActor->getConst().getTable()->mSwimSpecStartTimerA[2];
                _2E = mActor->getConst().getTable()->mSwimSpecStartTimerB[2];
                break;
            }

            // Possibly related to similar operations? (eg getSwimValue(), etc)
            f32 ftmp2 = cLimitAngleSink * (1.0f - fr1f);
            _5C = fr1f * getSpecialNumber() + ftmp2;
            TVec3f stack_38(_60);
            stack_38.y = -10.0f;
            MR::normalize(&stack_38);
            getPlayer()->forceSetHeadVecKeepSide(stack_38);
        } else if (r1d) {
            changeAnimation("水上ダメージ着水", static_cast< const char* >(nullptr));
            _2E = mActor->getConst().getTable()->mJumpDamageSinkTimer;
            _AE = 0x78;
        } else {
            _54 = (0.2f + 0.1f * fr1f) * mActor->getConst().getTable()->mSwimFrontMaxSpeed;
            changeAnimation("水泳ジャンプダイブ", static_cast< const char* >(nullptr));

            mActor->setBlendMtxTimer(8);
            _2C = 15;
            _2E = mActor->getConst().getTable()->mJumpSinkTimer;

            mIsSwimmingAtSurface = true;
            _5C = 1.49599659443f;
            if (getPlayer()->mVerticalSpeed < 200.0f) {
                f32 ftmp = (getPlayer()->mVerticalSpeed / 200.0f);
                _2C *= ftmp;
                _2E *= ftmp;
                mActor->setBlendMtxTimer(8.0f * ftmp);
            }
        }
    } else if (mIsOnSurface) {
        _AE = 0x5A;
        MR::vecKillElement(mActor->getLastMove(), mUpVec, &_A0);
        _A0.mult(0.5f);
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
                _5C = 2.09439516068f;

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
    fr1d = 1.0f + fr1d * MR::sin(funConversions1(_24, tmp));
    tmp = 500;
    f32 ftmp = fr1c * MR::sin(funConversions1(_24, tmp));
    tmp = 180;

    // I love sin composition
    return fr1f + fr1d * (fr1e * (MR::sin(ftmp + funConversions1(_24, tmp)) - 1.0f));
}

bool MarioSwim::update() {
    bool r1e = false;
    _24++;
    if (_22) {
        mNextAction = EXIT_ACTION_SLIDE;
        return false;
    }
    if (!checkWaterCube(false)) {
        if (mNextAction == EXIT_ACTION_JUMP || mNextAction == EXIT_ACTION_SLIDE) {
            return false;
        }

        const TVec3f& rAirGravityVec = getAirGravityVec();
        TVec3f stack_194;
        MR::vecKillElement(mSurfaceNorm - getTrans(), rAirGravityVec, &stack_194);
        if (stack_194.length() > 100.0f || -mWaterDepth >= getPlayer()->mVerticalSpeed || _AE || _1B0) {
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
        _2E = 0;
        _34 = 0;
        _32 = 0;
        _2C = 0;
        _4C = 0.0f;
    }

    mUpVec = -getPlayer()->getAirGravityVec();
    f32 fTmp = getSurface() - 30.0f;
    if (_1B0) {
        _1B0--;
    }

    if (mIsOnSurface || (mIsSwimmingAtSurface && checkTrgA())) {
        if (_7A || _7C) {
            _9C = 0;
        } else {
            if (mJetTimer != 0) {
                if (checkTrgA()) {
                    doJetJump(0);
                    return false;
                }

                if (getStickY() <= 0) {
                    _5C = 0.00000381469726562f;
                    _4C = 1.0f;
                }
            } else {
                u32 tmp = 0x1E;
                // Surface Paddle
                if (isAnimationRun("水上一掻き")) {
                    tmp = 0x1E;
                }

                if (checkLvlA()) {
                    if (_9C && (!isAnimationRun("水上一掻き") || !_28)) {
                        _28++;
                    }
                    if ((_1B0 || mJetTimer != 0 || getPlayer()->mMovementStates._8 || getPlayer()->mMovementStates._32) && checkTrgA()) {
                        _28 = tmp;
                    }
                } else {
                    _9C = 1;
                    if (_28 && _28 < tmp) {
                        surfacePaddle();
                    }

                    _28 = 0;
                    _1C = 0;
                }
                if (mActor->isRequestJump2P()) {
                    _28 = tmp;
                    getPlayer()->_1C._A = true;
                    getPlayer()->_10._1F = true;
                }

                bool tmp2 = false;
                if ((!_32 || (_7A || _7C)) && (_28 >= tmp || _1C)) {
                    tmp2 = true;
                }
                if (mActor->isRequestSpin()) {
                    tmp2 = true;
                }
                if (tmp2) {
                    if (_24 > 6) {
                        f32 res = MR::clamp(_54 / mActor->getConst().getTable()->mSwimToWalkSpd, 0.0f, 1.0f);
                        getPlayer()->mMovementStates._5 = false;
                        getPlayer()->_278 = res;
                        getPlayer()->tryJump();
                        mNextAction = EXIT_ACTION_POWER_JUMP;
                        return false;
                    }
                    _1C = 1;
                }
            }
        }

        flowOnWave(fTmp);
    } else if (mWaterDepth > -30.0f) {
        if (!_2C && !_32 && !_2E && mJetTimer == 0) {
            getAnimator()->forceSetBlendWeight(cWeightTable);
            changeAnimation("炊泳上昇呼吸", static_cast< const char* >(nullptr));
        }

        if (!_2E) {
            onSurface();
            _2C = 0;
        }

        if (_7C > 10) {
            f32 res = MR::clamp(_54 / mActor->getConst().getTable()->mSwimToWalkSpd, 0.0f, 1.0f);
            getPlayer()->mMovementStates._5 = false;
            getPlayer()->_278 = res;

            TVec3f stack_188;
            mActor->getLastMove(&stack_188);
            f32 res2 = MR::vecKillElement(stack_188, getGravityVec(), &stack_188);
            if (res2 > -10.0f) {
                res2 = -10.0f;
            }

            TVec3f gravityVec = getGravityVec();
            gravityVec.scale(res2);
            stack_188 += gravityVec;
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
            f32 res = MR::clamp(_54 / mActor->getConst().getTable()->mSwimToWalkSpd, 0.0f, 1.0f);
            getPlayer()->mMovementStates._5 = false;
            getPlayer()->_278 = res;

            TVec3f upOffset = mUpVec;
            upOffset.scale(10.0f);
            TVec3f jumpVel = mActor->getLastMove();
            jumpVel.scale(2.0f);
            TVec3f jumpVelScaled = jumpVel;
            jumpVelScaled += upOffset;
            getPlayer()->tryForcePowerJump(jumpVelScaled, false);

            mNextAction = EXIT_ACTION_POWER_JUMP;
            changeAnimationNonStop("リングダッシュ");
            return false;
        }
    } else {
        checkWaterBottom();
        if (getStickY() >= 0.0f && (_1F || (mDistToFloor < 60.0f && _54 > 1.0f))) {
            const TVec3f& rShadowNorm = getPlayer()->getShadowNorm();
            TVec3f stack_17C, stack_170;
            PSVECCrossProduct(&_60, &rShadowNorm, &stack_17C);
            PSVECCrossProduct(&rShadowNorm, &stack_17C, &stack_170);
            if (-mWaterDepth + mDistToFloor > 1000.0f) {
                _1A = 1;
            }
            f32 dot1 = _60.dot(stack_170);
            f32 dot2 = _60.dot(rShadowNorm);
            if (dot2 >= 0.0f) {
                _5C = 1.57079637051f + 0.9f * marioAcos(dot1);
            } else if (_54 > 1.0f) {
                _5C = 1.57079637051f - 0.5f * marioAcos(dot1);
            } else {
                f32 angle = marioAcos(dot1);
                _38 = 0x3C;
                _5C = 1.57079637051f - angle;
            }
            if (mJetTimer != 0 && getPlayer()->mVerticalSpeed < 100.0f) {
                _42 = 2;
                playEffect("水底接触");
                _54 *= mActor->getConst().getTable()->mTurboReductionHitFloor;
            }
        }
        _1F = 0;
    }
    if (_42) {
        playSound("水底接触", -1);
        if (--_42 == 0) {
            stopEffect("水底接触");
        }
    }
    if (_32 && --_32 == 0) {
        r1e = true;
        _5C = mActor->getConst().getTable()->mZsinkAngleX;
        _2C = mActor->getConst().getTable()->mZsinkMoveTimer;
        _2E = mActor->getConst().getTable()->mZsinkSinkTimer;
        _54 = mActor->getConst().getTable()->mZsinkFrontSpeed;
        _38 = 0x14;
    }
    f32 fr1f = 1.0f - MR::clamp(_54 / mActor->getConst().getTable()->mSwimFrontMaxSpeed, 0.0f, 1.0f);
    if (mDistToFloor - mWaterDepth < 400.0f) {
        fr1f = 0.0f;
    }
    if (_38) {
        _38--;
    } else {
        f32 res = fr1f;
        if (mIsOnSurface && !_32) {
            if (!_18) {
                if (mJetTimer != 0) {
                    if (getStickY() > 0.707000017166f) {
                        _5C = mActor->getConst().getTable()->mZsinkAngleX;
                        mIsSwimmingAtSurface = false;
                        _1E = 0;
                    }

                    f32 a;
                    f32 b;
                    b = getSwimValue(a = getStickY(), 2, mActor->getConst().getTable());
                    _4C = _4C * b + procAngle(a, b);

                    _50 = 2.5f * (getStickX() * (1.0f - mActor->getConst().getTable()->mSwimRotZIneT));
                    f32 fr1c = 1.0f;
                    if (getStickY() < 0.0f) {
                        fr1c += 2.0f * -getStickY();
                    }
                    _50 *= fr1c;
                } else if (checkTrgZ() && !check7Aand7C()) {
                    stopAnimationUpper("水泳水面初期移動", nullptr);
                    changeAnimation("水泳潜り", static_cast< const char* >(nullptr));
                    playEffect("水面Ｚ沈降");
                    playSound("水面潜り", -1);
                    _32 = mActor->getConst().getTable()->mZsinkStartTimer;
                } else if (checkLvlZ() == 0) {
                    res = 0.0f;
                } else {
                    f32 a, b;
                    b = getSwimValue(a = getStickP(), 0, mActor->getConst().getTable());
                    _4C = _4C * b + procAngle(a, b);
                }
            }
            _50 *= mActor->getConst().getTable()->mSwimRotZIne;
        } else {
            if (mRingDashTimer == 0 && mDistToFloor > 200.0f && checkTrgZ() && !_32 && !_2E && mJetTimer == 0 && !check7Aand7C()) {
                stopAnimation(nullptr, static_cast< const char* >(nullptr));
                changeAnimation("水泳潜り", static_cast< const char* >(nullptr));
                playSound("水中潜り", -1);
                playEffect("水面Ｚ沈降");
                _32 = mActor->getConst().getTable()->mZsinkStartTimer;
            }
            if (mJetTimer != 0) {
                f32 a;
                f32 b;
                b = getSwimValue(a = getStickY(), 2, mActor->getConst().getTable());
                _4C = _4C * b + procAngle(a, b);
                b = getSwimValue(a = getStickX(), 3, mActor->getConst().getTable());
                _50 = _50 * b + procAngle(a, b);
            } else {
                f32 a, b;
                b = getSwimValue(a = getStickY(), 0, mActor->getConst().getTable());
                _4C = _4C * b + procAngle(a, b);
                b = getSwimValue(a = getStickX(), 1, mActor->getConst().getTable());
                _50 = _50 * b + procAngle(a, b);
            }
        }
        if (!mIsSwimmingAtSurface && !_3C && _2C) {
            _5C += res * (_4C * mActor->getConst().getTable()->mSwimRotSpeedX);
        }

        if ((_54 < cTurnMotionSpeed || mIsOnSurface || checkLvlA() || checkLvlZ()) && isAnimationRun("水泳一掻き") &&
            isAnimationTerminate("水泳一掻き")) {
            stopAnimation(nullptr, static_cast< const char* >(nullptr));
        }

        if (!checkLvlA() && !checkLvlZ() && !_2C && _54 < cTurnMotionSpeed) {
            bool worthless;
            bool& worthlesser = worthless;
            funReferenceTime(worthlesser);
            if (!mIsSwimmingAtSurface && !_AE && mWaterDepth < -400.0f) {
                _3C++;
                if (MR::getRandom() < 0.03f) {
                    playSound("水中ウエイト", -1);
                }
            }
            if (_3C >= 0x78) {
                _3C = 0x78;
            }
        } else {
            _3C = 0;
        }
        f32 fr1d;
        if (MR::isNearZero(getStickY(), 0.1f)) {
            f32 num = _3C / 120.0f;
            if (num > 1.0f) {
                num = 1.0f;
            }
            fr1d = _5C + num * (cNeutralAngleWait - _5C);
        } else if (getStickY() > 0.0f) {
            f32 fr4 = 0.0f;
            f32 num = _3C / 120.0f;
            if (_54 < 2.0f) {
                fr4 = 1.0f;
            } else {
                bool confusion;
                bool& confusionion = confusion;
                if (_54 > 10.0f) {
                    confusionion = true;
                } else {
                    fr4 = 1.0f - (_54 - 2.0f) / 8;
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
        if (_54 > 5.0f && (fr1e -= fr1e * (5.0f / _54)) < 0.0f) {
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
        if (_32) {
            fr1e = 0.15f;
            fr1d = mActor->getConst().getTable()->mZsinkAngleX;
        }
        bool r1b = true;
        _5C = _5C * (1.0f - fr1e) + fr1d * fr1e;
        if (_3C && !_AE && !_32 && getStickY() > 0.0f) {
            f32 fr1c = 3.14159274101f / mActor->getConst().getTable()->mSwimTiltReal;
            f32 tmpsticky = getStickY();
            fr1d = 0.523598790169f + tmpsticky * (fr1c - 0.523598790169f);
            if (_5C < fr1d) {
                _5C = _5C * mActor->getConst().getTable()->mSwimTiltSpd + fr1d * (1.0f - mActor->getConst().getTable()->mSwimTiltSpd);
            } else {
                _5C = _5C * (1.0f - fr1e) + fr1d * fr1e;
            }
            if (!check7Aand7C() && mJetTimer == 0) {
                changeAnimation("水泳ターン下", static_cast< const char* >(nullptr));
            }
            r1b = false;
        }
        if (r1b) {
            stopAnimation("水泳ターン下", static_cast< const char* >(nullptr));
        }
        if (_32) {
            _5C = MR::clamp(_5C, 0.523598790169f, mActor->getConst().getTable()->mZsinkAngleX);
        } else {
            _5C = MR::clamp(_5C, cLimitAngleWait, cUpperAngleWait);
        }
        if (MR::isStageSwimAngleLimit()) {
            _5C = MR::clamp(_5C, 0.872664690018f, 2.26892805099f);
        }
    }

    if ((mIsSwimmingAtSurface || (mIsOnSurface && isStickOn())) && mJetTimer == 0) {
        TVec3f stack_164(getWorldPadDir());
        f32 tmp;
        if (mIsOnSurface) {
            tmp = 0.01f * (12.0f - _54);
            if (tmp <= 0.01f) {
                tmp = 0.01f;
            }
        } else {
            tmp = 0.01f * (10.0f - _54);
        }
        if (mJetTimer != 0) {
            tmp *= 3.0f;
        }
        f32 res = MR::clamp(tmp * mActor->getConst().getTable()->mSwimRotSpeedRatioSurface, 0.0f, 1.0f);
        f32 diffAngle = MR::diffAngleAbs(_60, stack_164);
        if (diffAngle > 0.0f) {
            if (diffAngle < res) {
                _60 = stack_164;
            } else {
                MR::vecBlendSphere(_60, stack_164, &_60, res / diffAngle);
            }
        }
    } else {
        f32 tmp = fr1f + mActor->getConst().getTable()->mSwimRotSpeedZStop;
        MR::rotAxisVecRad(_60, -mUpVec, &_60, tmp * (_50 * mActor->mConst->getTable()->mSwimRotSpeedZ));
    }
    TVec3f stack_158(_60);
    MR::vecKillElement(_60, mUpVec, &_60);
    TVec3f stack_14C;
    TVec3f stack_110;
    TVec3f stack_140;
    if (!MR::normalizeOrZero(&_60)) {
        PSVECCrossProduct(&mUpVec, &_60, &stack_140);
        MR::normalize(&stack_140);
        getPlayer()->setSideVec(stack_140);
        MR::rotAxisVecRad(_60, stack_140, &stack_14C, _5C);
        TVec3f stack_134(getPlayer()->mHeadVec);
        TVec3f stack_128(getFrontVec());
        TVec3f stack_11C;
        f32 something = 0.1f;
        if (r1e) {
            something = 1.0f;
        }
        MR::vecBlendSphere(stack_128, stack_14C, &stack_11C, something);
        getPlayer()->setFrontVecKeepSide(stack_11C);
    } else {
        stack_14C = getFrontVec();
        stack_140 = getPlayer()->mSideVec;
    }
    spin();
    stack_110 = getPlayer()->_1FC;
    if (_34 || _2E || isAnimationRun("水泳ジャンプダイブ回転") || isAnimationRun("水泳潜り")) {
        PSVECCrossProduct(&stack_14C, &stack_140, &stack_110);
    }
    if (mJetTimer == 0) {
        decideVelocity();
    }
    TVec3f stack_104 = createAndScale(_54, stack_110);
    if (mIsOnSurface && mUpVec.dot(stack_110) > 0.0f) {
        f32 mag = stack_104.length();
        MR::vecKillElement(stack_104, mUpVec, &stack_104);
        stack_104.setLength(mag);
    }
    f32 element = MR::vecKillElement(stack_104, mUpVec, &stack_104);
    f32 fr2;
    if (_30 > 0x19) {
        fr2 = (50 - _30) / 25.0f;
    } else {
        fr2 = _30 / 25.0f;
    }
    f32 fr1d = fr2 + (1.0f - fr2) * mActor->getConst().getTable()->mSwimSpdYratio;
    
    stack_104 += mUpVec.scaleInline(element).scaleInline(fr1d);
    addVelocity(stack_104);
    u16 fr1b = _2E;
    if (fr1b) {
        addVelocity(getGravityVec(), 0.25f * fr1b);
        _2E--;
    }

    if (_34) {
        _34--;
    }
    if (!mIsOnSurface && !_2C && mJetTimer == 0) {
        procBuoyancy();
    } else if (mUpVec.dot(stack_110) < 0.0f) {
        if (mIsOnSurface) {
            mIsOnSurface = false;
        }
    } else if (mIsOnSurface) {
        TVec3f gravityVec = getGravityVec();
        gravityVec.scale(100.0f);

        AreaObj* obj = MR::getAreaObj("WaterArea", getTrans().translate(gravityVec));
        if (obj) {
            TPos3f* followMtx = obj->getFollowMtx();
            if (followMtx) {
                TVec3f stack_F8;
                MR::extractMtxTrans(followMtx->toMtxPtr(), &stack_F8);

                bool cond = JGeometry::TUtil< f32 >::epsilonEquals(stack_F8.x, _154.x, 0.000003814697265625f) &&
                            JGeometry::TUtil< f32 >::epsilonEquals(stack_F8.y, _154.y, 0.000003814697265625f) &&
                            JGeometry::TUtil< f32 >::epsilonEquals(stack_F8.z, _154.z, 0.000003814697265625f);
                if (!cond) {
                    TVec3f diff = stack_F8 - _154;
                    if (diff.length() < 10.0f) {
                        addVelocity(stack_F8 - _154);
                    }
                    _154 = stack_F8;
                }
            }
        }
    }
    if (_AE) {
        _AE--;
        addVelocity(_A0);
        if (_AE < 0x78) {
            _A0.mult(0.98f);
        }
    }
    if (mJetTimer != 0) {
        addVelocity(_184, mActor->getConst().getTable()->mTurboReductionStream);
    } else {
        addVelocity(_184);
    }

    TVec3f stack_EC(_184);
    f32 fr1dv2 = stack_EC.length();
    if (!MR::normalizeOrZero(&stack_EC)) {
        if (MR::diffAngleAbs(stack_EC, _60) < PI / 2) {
            MR::vecBlendSphere(_60, stack_EC, &_60, 0.001f * fr1dv2);
        } else {
            MR::vecBlendSphere(_60, -stack_EC, &_60, 0.001f * fr1dv2);
        }
    }

    TVec3f stack_E0;
    if (MR::calcWhirlPoolAccelInfo(getTrans(), &stack_E0) && !MR::isNearZero(stack_E0)) {
        _184 += stack_E0;
        TVec3f stack_D4(stack_E0);
        MR::normalizeOrZero(&stack_D4);
        MR::vecBlendSphere(_60, stack_D4, &_60, 0.1f);
    }

    if (mJetTimer == 0 && !mIsOnSurface && !MR::isNearZero(_190)) {
        f32 angleDiff = MR::diffAngleSignedHorizontal(_190, _60, getPlayer()->getAirGravityVec());
        f32 cmp = 1.04719758034f;
        if (angleDiff > cmp) {
            Mtx stack_1D0;
            PSMTXRotAxisRad(stack_1D0, &mUpVec, angleDiff - 1.04719758034f);
            PSMTXMultVecSR(stack_1D0, &_60, &_60);
        } else if (angleDiff < -cmp) {
            Mtx stack_1A0;
            PSMTXRotAxisRad(stack_1A0, &mUpVec, -(-angleDiff - 1.04719758034f));
            PSMTXMultVecSR(stack_1A0, &_60, &_60);
        }
    }

    decideAnimation();
    decideEffect(false);

    if (_2C) {
        _2C--;
    }
    if (_30) {
        _30--;
    }
    if (_44) {
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

    if (_AE != 0) {
        _58 = 0.0f;
    } else {
        _21 = 0;
        if (_1A != 0) {
            _1A = 0;
            _3A = 120;
        }

        f32 buoyancyScale = 1.0f;
        f32 floorThreshold = 2000.0f;
        f32 waterDepthNeg = -mWaterDepth;
        bool isBelowStrongSwimDepth = mActor->getConst().getTable()->mSwimUpStrongDist > waterDepthNeg;
        f32 swimUpBottomDist = mActor->getConst().getTable()->mSwimUpBottomDist;
        f32 swimUpWeakDist = mActor->getConst().getTable()->mSwimUpWeakDist;
        f32 floorDepthSum = mDistToFloor + waterDepthNeg;

        if (floorDepthSum < floorThreshold) {
            f32 ratio = floorDepthSum / floorThreshold;
            swimUpWeakDist *= ratio;
            swimUpBottomDist *= ratio;
        }

        bool isNearWeakDist = (swimUpWeakDist > mDistToFloor);
        if (_54 > 2.0f && _5C > PI / 2) {
            isNearWeakDist = false;
            _21 = 0;
        }

        if (!(mDistToFloor < 300.0f) || !(-mWaterDepth < 100.0f)) {
            if (isNearWeakDist) {
                if (_24 > 300) {
                    f32 distRatio = mDistToFloor / swimUpWeakDist;
                    if (distRatio < 0.0f)
                        distRatio = 0.0f;
                    if (swimUpBottomDist > mDistToFloor)
                        distRatio = 0.0f;

                    buoyancyScale =
                        mActor->getConst().getTable()->mSwimUpWeakRatio + (distRatio * (1.0f - mActor->getConst().getTable()->mSwimUpWeakRatio));
                } else {
                    buoyancyScale = 1.0f + (1.0f - ((f32)_24 / 300.0f));
                }
                _21 = 1;
            } else if (isBelowStrongSwimDepth) {
                if (mActor->getConst().getTable()->mSwimUpSurfaceDist > -mWaterDepth) {
                    buoyancyScale = mActor->getConst().getTable()->mSwimUpStrongRatio;
                } else {
                    f32 strongRatio = (-mWaterDepth - mActor->getConst().getTable()->mSwimUpSurfaceDist) /
                                      (mActor->getConst().getTable()->mSwimUpStrongDist - mActor->getConst().getTable()->mSwimUpSurfaceDist);
                    if (strongRatio < 0.0f)
                        strongRatio = 0.0f;

                    buoyancyScale = strongRatio + ((1.0f - strongRatio) * mActor->getConst().getTable()->mSwimUpStrongRatio);
                }
            }
        }

        if (_3A != 0) {
            _3A--;
        } else {
            f32 acc = mActor->getConst().getTable()->mSwimUpAcc;
            _58 += (1.0f * (1.0f * mActor->getConst().getTable()->mSwimUpAcc));
        }

        if (_58 > mActor->getConst().getTable()->mSwimUpMaxSpeed) {
            _58 = mActor->getConst().getTable()->mSwimUpMaxSpeed;
        }

        if (!MR::isNearZero(_184, 0.001f)) {
            buoyancyScale = 1.0f;
        }

        if (_21 != 0) {
            if (mDistToFloor < mActor->getConst().getTable()->mSwimUpBottomDist) {
                f32 clampRatio = MR::clamp(1.0f - ((f32)_2C / 50.0f), 0.0f, 1.0f);
                f32 sinkFactor = ((1.0f - clampRatio) + (clampRatio * mActor->getConst().getTable()->mSwimReverseSinkRatio));
                sinkFactor *= 0.1f;

                if (!MR::isNearZero(_184, 0.001f)) {
                    sinkFactor = 1.0f;
                }
                addVelocity(-mUpVec, (sinkFactor * (_58 * buoyancyScale)));
            } else {
                addVelocity(-mUpVec, _58 * buoyancyScale);
            }
        } else {
            addVelocity(mUpVec, _58 * buoyancyScale);
        }
    }
}

void MarioSwim::flowOnWave(f32 unused) {
    if (_2C) {
        return;
    }

    if (mWaterDepth < -cWaterSurfaceRange) {
        if (mIsOnSurface) {
            mIsOnSurface = false;
        }

        return;
    } else if (mIsOnSurface) {
        if (mWaterDepth < -25.0f) {
            addVelocity(mSurfacePos, _58);
        }
        if (mWaterDepth > 0.0f) {
            TVec3f inverseFlow = -mSurfacePos;
            addVelocity(inverseFlow, 5.0f + mWaterDepth);
        }
    }
    _3E++;
}

void MarioSwim::decideVelocity() {
    if (mRingDashTimer != 0) {
        return;
    }

    if (MarioModule::checkLvlZ() || (MarioModule::checkLvlA() && _9C)) {
        mIsSwimmingAtSurface = false;
    }

    if (mWaterDepth < -400.0f && _2E == 0) {
        mIsSwimmingAtSurface = false;
    }

    if ((mIsOnSurface && (MarioModule::isStickOn() || _2C != 0)) || (mIsSwimmingAtSurface && MarioModule::isStickOn())) {
        if (_2C == 0 && mJetTimer == 0) {
            f32 angleDiff = MR::diffAngleAbs(_60, getWorldPadDir());
            f32 angleFactor = (PI - angleDiff) / PI;
            f32 targetSpeedScale = angleFactor * MarioModule::getStickP();
            MarioConstTable* table = mActor->getConst().getTable();
            f32 inertia = table->mSwimFrontIne;
            if (_7C == 0 && _54 >= (targetSpeedScale * table->mSwimFrontOnWaterSpeed)) {
                inertia = 0.95f;
            }
            f32 invInertia = (1.0f - inertia);
            f32 frontSpeed = table->mSwimFrontOnWaterSpeed;
            f32 term1 = invInertia * (targetSpeedScale * frontSpeed);
            f32 term2 = _54 * inertia;
            _54 = term1 + term2;
        }
    } else if (mIsOnSurface && _36 != 0) {
        f32 inertia = mActor->getConst().getTable()->mSwimFrontIne;
        _54 = (_54 * inertia) + ((1.0f - inertia) * mActor->getConst().getTable()->mSwimFrontOnWaterSpeed);
    } else {
        if (!mIsOnSurface && checkTrgA() && _AE == 0) {
            bool isSpinning = false;

            if (!_7A) {
                if (_7C) {
                    isSpinning = false;
                }
                if (!isSpinning) {
                    f32 stickX = __fabsf(MarioModule::getStickX());
                    f32 stickFactor = 1.0f - (0.3f * stickX);
                    f32 threshold = 0.75f * stickFactor;

                    if (_5C < PI / 2 && getStickY() > threshold && _2C == 0 && _1E != 0) {
                        f32 stickY = getStickY();
                        f32 normalizedY = (stickY - threshold) / (1.0f - threshold);
                        _5C = ((1.0f - normalizedY) * 2.0943952f) + (normalizedY * 2.6179938f);
                    } else if (_5C < PI / 2 && __fabsf(getStickY()) < 0.1f && _2C == 0 && _1E != 0) {
                        _5C = PI / 2;
                    }

                    _1E = 0;

                    bool isTerminated = MarioModule::isAnimationTerminate("水泳一掻き");

                    bool inRange;
                    if (isTerminated || !_1D) {
                        const MarioConstTable* table = mActor->getConst().getTable();
                        f32 frame = getAnimator()->getFrame();

                        bool inRange = MR::isInRange(frame, (f32)table->mSwimConnectIn, (f32)table->mSwimConnectOut);
                        if (!inRange) {
                            _1D = 1;
                            return;
                        } else {
                            inRange = false;
                            isTerminated = false;
                            _1D = 0;
                            _2C = 60;
                            _30 = 50;

                            if (!MarioModule::isAnimationRun("水泳潜り") && !MarioModule::isAnimationRun("水泳ジャンプダイブ回転")) {
                                getAnimator()->forceSetBlendWeight(cWeightTableSP);
                                stopAnimation(static_cast< const char* >(nullptr), static_cast< const char* >(nullptr));
                                changeAnimationNonStop("水泳一掻き");
                                playSound("水中一掻き", -1);
                                _36 = 40;
                            }
                        }
                    }
                }
            }

        } else {
            if (!mIsOnSurface && MarioModule::checkLvlA()) {
                _54 = (_54 * mActor->getConst().getTable()->mSwimFrontIne) +
                      (mActor->getConst().getTable()->mSwimFrontNormalSpeed * (1.0f - mActor->getConst().getTable()->mSwimFrontIne));
                _58 = 0.0f;
            } else {
                if (mIsOnSurface) {
                    _54 *= mActor->getConst().getTable()->mSwimStopIneSurface;
                } else {
                    _54 *= mActor->getConst().getTable()->mSwimStopIne;
                }

                if (MarioModule::isAnimationRun("水泳上昇呼吸")) {
                    _54 *= 0.95f;
                }

                if (mDistToFloor > mActor->getConst().getTable()->mSwimUpBottomDist) {
                    _54 *= 0.99f;
                } else if (mDistToFloor > mActor->getConst().getTable()->mSwimUpWeakDist) {
                    _54 *= 0.995f;
                }
            }
        }
    }

    if (_36 != 0) {
        bool isSpinning = (_7A != 0 || _7C != 0);

        if (isSpinning) {
            if (mIsOnSurface) {
                if (_36 < 20) {
                    f32 acc = cFrontAccSpinSurface[20 - _36];

                    if (acc < 0.0f) {
                        _54 += acc * mActor->getConst().getTable()->mSwimSpinSurfaceAccRatioR;
                    } else {
                        _54 += acc * mActor->getConst().getTable()->mSwimSpinSurfaceAccRatio;
                    }

                    playSound("水面スピン", -1);
                } else {
                    _54 *= 0.96f;
                }
            } else {
                if (_36 < 30) {
                    f32 acc = cFrontAccSpin[30 - _36];
                    if (acc < 0.0f) {
                        _54 += acc * mActor->getConst().getTable()->mSwimSpinAccRatioR;
                    } else {
                        _54 += acc * mActor->getConst().getTable()->mSwimSpinAccRatio;
                    }
                    playSound("水中スピン", -1);
                } else {
                    _54 *= 0.96f;
                }
            }
        } else if (_36 < 40) {
            f32 accRatio = mActor->getConst().getTable()->mSwimAccRatio;
            f32 accBase = cFrontAcc[40 - _36];
            f32 currentAcc = accBase * accRatio;

            f32 maxSpeed = mActor->getConst().getTable()->mSwimFrontMaxSpeed;
            f32 minRatio = mActor->getConst().getTable()->mSwimAccMinRatio;

            f32 speedFactor = _54 / maxSpeed;
            _54 += (minRatio + (speedFactor * (1.0f - minRatio))) * currentAcc;
        }

        _36--;

        bool canClamp = false;

        if (_7A != 0 || _7C != 0) {
            canClamp = true;
        }

        if (canClamp) {
            if (_54 > mActor->getConst().getTable()->mSwimFrontSpinSpeed)
                _54 = mActor->getConst().getTable()->mSwimFrontSpinSpeed;
        } else {
            if (_54 > mActor->getConst().getTable()->mSwimFrontMaxSpeed)
                _54 = mActor->getConst().getTable()->mSwimFrontMaxSpeed;
        }
    }

    if (_32 != 0) {
        _54 *= 0.95f;
    }
}

void MarioSwim::decideAnimation() {
    // This is the only way I could achieve a block copy
    struct AnimSpeedData {
        f32 speeds[4];
    };
    AnimSpeedData animSpeeds = *(AnimSpeedData*)cAnimSpeeds;

    const MarioConstTable* table = mActor->getConst().getTable();
    animSpeeds.speeds[1] = table->mSwimSurfaceSpeed;
    animSpeeds.speeds[2] = table->mSwimWaterSpeed;
    animSpeeds.speeds[3] = table->mSwimDriftSpeed;

    if (mJetTimer != 0)
        return;
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

    if (_2C != 0) {
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
            } else if (__fabsf(getStickX()) > 0.1f && _5C < 1.6534699f && _54 < cTurnMotionSpeed) {
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
                    _1B = 1;
                }
            }

            animIndex = 1;

            if (_1B != 0) {
                getAnimator()->forceSetBlendWeight(&cWeightTable[4]);
                changeAnimationUpper("水泳水面初期移動", static_cast< const char* >(nullptr));
                _1B = 0;
                return;
            }
        } else {
            if (_36 == 0 && _54 < 2.0f) {
                _1B = 1;
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
    u8 oldState = _9F;
    if (mIsOnSurface) {
        if (!isAnimationRun("水泳水面初期移動")) {
            if (_54 > 2.5f) {
                _9F = 1;
            } else {
                _9F = 0;
            }
        }
    } else {
        if (checkLvlZ() || checkLvlA() || _2C != 0 || isAnimationRun(nullptr)) {
            _9F = 2;
        } else {
            _9F = 3;
        }
    }

    if (isReset == true) {
        _9F = 0xFF;
    }
    u8 newState = _9F;
    if (oldState != _9F) {
        switch (oldState) {
        case 0:
            if (_9F != 1) {
                stopEffect("水面ウエイト波紋");
            }
            break;

        case 1:
            stopEffect("水面泳ぎ");
            if (_9F != 0) {
                stopEffect("水面ウエイト波紋");
            }
            break;

        case 2:
            stopEffect("水中身体泡");
            break;
        }

        if (_9F >= 2 || oldState >= 2 || isReset) {
            stopEffect("水中口泡");
        }

        switch (_9F) {
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

    if (_9F <= 1) {
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

            getPlayer()->tryForcePowerJump(_60.scaleInline(5.0f) - getAirGravityVec().scaleInline(15.0f), false);
            getPlayer()->_43A = 120;
        } else {
            // Snap to ground
            addTrans(groundPos - getTrans(), "Module");
            getPlayer()->_43A = 60;
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

        _1B2 = 1;
        _1B4 = 0.0f;
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

        MR::vecKillElement(_184, getGravityVec(), &_184);

        getPlayer()->mJumpVec = getPlayer()->mJumpVec.translate(_184.scaleInline(0.3f));
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
        _1B4 = 0.0f;
    }

    u32 searchLimit = 2;
    if (MarioModule::getPlayerMode() == 3) {
        searchLimit = 10;
    }

    TVec3f searchDir = MarioModule::getFrontVec();
    if (!MR::isNearZero(mActor->getLastMove(), 0.001f)) {
        searchDir = mActor->getLastMove();
        MR::vecKillElement(searchDir, getGravityVec(), &searchDir);
        MR::normalizeOrZero(&searchDir);
    }

    const f32 kOffset = 100.0f;

    for (u32 i = 0; i < searchLimit; ++i) {
        TVec3f checkPos = MarioModule::getTrans().translate(searchDir.scaleInline(kOffset)).translate(getGravityVec().scaleInline(_1B4 - kOffset));

        WaterInfo waterInfo;
        MR::getWaterAreaObj(&waterInfo, checkPos);

        if (waterInfo.isInWater()) {
            _1B8 = _1B4;

            if (_1B4 > 300.0f) {
                getWaterAreaInfo(&waterInfo, checkPos, nullptr);
                TVec3f streamVec = waterInfo.mStreamVec;
                _1B8 = (streamVec - getTrans()).dot(getGravityVec());
                _1B2 = 1;
                _1B4 = 0.0f;
                return;
            }

            if (_1B4 < 210.0f && MarioModule::getPlayerMode() == 3) {
                if (!MR::checkStrikePointToMap(checkPos, nullptr)) {
                    getWaterAreaInfo(&waterInfo, getTrans(), nullptr);
                    TVec3f streamVec = waterInfo.mStreamVec;

                    if (streamVec.x < 100.0f) {
                        if (!(getPlayer()->mMovementStates._1) || getPlayer()->_71C) {
                            mActor->createIceFloor(waterInfo.mSurfacePos.translate(searchDir.scaleInline(170.0f)));
                        }
                        _1B4 = 0.0f;
                        return;
                    }
                }
            }
            _1B4 = 0.0f;
            return;
        }

        if (MR::checkStrikePointToMap(checkPos, nullptr)) {
            _1B2 = 0;
            _1B4 = 0.0f;
            return;
        }

        _1B4 += 40.0f;
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

    if (pOutVec) {
        pOutVec->x = -pInfo->mCamWaterDepth;
        pOutVec->y = pInfo->_4;
    }

    if (mIsOnSurface) {
        if (mSurfaceOffset > 0.0f) {
            mSurfaceOffset = (mSurfaceOffset * 0.97f) + (pInfo->mWaveHeight * 0.03f);
        } else {
            mSurfaceOffset = (mSurfaceOffset * 0.99f) + ((pInfo->mWaveHeight * 0.5f) * 0.01f);
        }
    } else {
        mSurfaceOffset *= 0.8f;
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
    TVec3f smoothedFlow(currentFlow);
    smoothedFlow.scale(0.95f);

    TVec3f newFlowScaled(_184);
    newFlowScaled.scale(0.05f);
    TVec3f newFlowScaledCopy(newFlowScaled);
    newFlowScaledCopy += smoothedFlow;

    _184 = newFlowScaledCopy;
    _190 = currentFlow;

    return waterAreaInfo;
}

const TVec3f& MarioSwim::getGravityVec() const {
    return MarioModule::getGravityVec();
}

bool MarioSwim::surfacePaddle() {
    if (_36) {
        return false;
    }

    stopAnimation(static_cast< const char* >(nullptr), static_cast< const char* >(nullptr));
    changeAnimation("水上一掻き", static_cast< const char* >(nullptr));
    // Surface Paddle
    playSound("水面一掻き", -1);
    _36 = 40;

    return true;
}

void MarioSwim::spin() {
    if (mJetTimer != 0) {
        if (mActor->isRequestSpin()) {
            if (_7A == 0 && _7C == 0 && _40 == 0) {
                dropJet(false);
            }
        }
        return;
    }

    if (_AE != 0) {
        return;
    }

    if (_32 != 0) {
        return;
    } else if (checkTrgA()) {
        if (_7A != 0) {
            _7A = 0;
            startSpinDash();
            return;
        }
    }
    if (_7A != 0) {
        _7A--;
        _54 = 0.95f * _54;
        if (_7A == 0) {
            startSpinDash();
        }
    }

    if (_7C != 0) {
        _7C--;
    }

    if (!mActor->isRequestSpin()) {
        return;
    }
    if (_7A) {
        return;
    }
    if (_7C) {
        return;
    }
    if (_7C) {
        return;
    }

    if (_7A) {
        _7A = mActor->getConst().getTable()->mSwimSpinMissOfsTime;
        _54 = 0.5f * _54;

    } else {
        _7A = 1;
    }
    _36 = 0;
    _84 = mActor->getConst().getTable()->mSwimSpinSpeed;
    _7C = 0;
    _78 = 0;

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
    stopEffect("startSpinDash");

    if (_78) {
        stopAnimation("水泳スピン", static_cast< const char* >(nullptr));
        stopAnimation("水上スピン", static_cast< const char* >(nullptr));
        return;
    }

    _7C = mActor->getConst().getTable()->mSwimDashTime;
    _2C = mActor->getConst().getTable()->mSwimSpinTime;

    if (mIsOnSurface != 0) {
        _36 = 20;
        _30 = 50;
        _7C = 45;
        _2C = 30;
    } else {
        _36 = mActor->getConst().getTable()->mSwimSpinFrontTime;
        _30 = mActor->getConst().getTable()->mSwimSpinYspeedTime;
    }

    if (mIsOnSurface == 0) {
        changeAnimation("水上スピン移動", static_cast< const char* >(nullptr));
    } else {
        changeAnimation("水泳スピン移動", static_cast< const char* >(nullptr));
    }
    mActor->setPunchHitTimer(_7C);

    if (gIsLuigi) {
        decOxygen(mActor->getConst().getTable()->mOxygenMax / 16);
    }
}

void MarioSwim::hitPunch(const TVec3f& rPunchDir) {
    if (_7A) {
        _AE = 30;
        _54 = 0.0f;
        _A0 = rPunchDir;
        _78 = 1;

        stopAnimation("アッパーパンチ", static_cast< const char* >(nullptr));
    }
}

void MarioSwim::jet() {
    if (mActor->getCarrySensor() == nullptr) {
        mJetTimer = 0;
        _40 = 0;
    }

    if (_40 != 0) {
        _40--;
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
            if (checkTrgZ() || _24 < 2) {
                playSound("亀ブレーキ", -1);
                MR::emitEffect(mActor->getCarrySensor()->mHost, "BrakeLamp");
            }
            getPlayer()->_1C._9 = 1;

            if (_54 > targetSpeed) {
                _54 *= 0.95f;
                return;
            }

            if (_54 > (0.1f * targetSpeed)) {
                _54 *= 0.96f;
                return;
            }

        } else {
            playSound("亀ジェット泳ぎ", -1);
            MR::forceDeleteEffect(mActor->getCarrySensor()->mHost, "BrakeLamp");
        }

        if (_54 < targetSpeed) {
            if (_54 < 1.0f) {
                _54 = 1.0f;
            }
            _54 *= 1.02f;

            if (mRingDashTimer != 0) {
                _54 *= 1.5f;
            }
        } else {
            if (_54 > targetSpeed) {
                _54 *= 0.98f;
            } else {
                _54 = targetSpeed;
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

    if (_54 > mActor->getConst().getTable()->mSwimFrontMaxSpeed) {
        _54 = mActor->getConst().getTable()->mSwimFrontMaxSpeed;
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

    _AE = 15;
    _A0.zero();

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

    _40 = 20;
    return true;
}

void MarioSwim::forceStopSpin() {
    _7C = 0;
    _7A = 0;
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

    if (_AC == 0) {
        Mario* player = getPlayer();
        player->_41E = 150;
    }

    dropJet(false);
    _AE = mActor->getConst().getTable()->mWaterInnerFaintTime;

    changeAnimation("水泳ダメージ", static_cast< const char* >(nullptr));
    playSound("水中ダメージ", -1);
    playSound("声水中ダメージ", -1);
    playSound("ダメージ", -1);

    startPadVib(2);
    _A0 = rFaintDir;
    _A0.setLength(mActor->getConst().getTable()->mSwimFaintSpeed);
    _2C = 0;
    forceStopSpin();
    _54 = 0.0f;

    if (_AC == 0) {
        mActor->decLife(0);
        if (mActor->mHealth == 0) {
            mActor->forceGameOverNonStop();
        }
    }

    _AC = 0;
}

void MarioSwim::addDamage(const TVec3f& rDamageDir) {
    if (getPlayer()->getDamageAfterTimer() != 0) {
        return;
    }
    if (_AC == 0) {
        getPlayer()->_41E = 180;
    }

    dropJet(false);
    _54 = 0.0f;

    if (_AC == 0) {
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

        _60 = -surfaceKnockback;
        mNextAction = EXIT_ACTION_POWER_JUMP;
        getPlayer()->closeStatus(this);

        if (_AC == 0) {
            mActor->decLife(0);
            if (mActor->mHealth == 0) {
                mActor->forceGameOverNonStop();
            }
        }
        _AC = 0;
    } else {
        _AE = mActor->getConst().getTable()->mWaterInnerDamageTime;
        changeAnimation("水泳ダメージ中", static_cast< const char* >(nullptr));

        _A0 = rDamageDir;
        _A0.setLength(mActor->getConst().getTable()->mSwimDamageSpeed);

        playSound("水中ダメージ", -1);
        playSound("声水中ダメージ", -1);

        if (_AC == 0) {
            decLife();
        }
        _AC = 0;
    }

    if (_AD != 0) {
        _A0.setLength(mActor->getConst().getTable()->mSwimDamageSpeedEx1);
        _AE = mActor->getConst().getTable()->mSwimDamageTimerEx1;
        _AD = 0;
    }

    startPadVib(3);
    playEffect("ダメージ");
    _2C = 0;
    forceStopSpin();
    _54 = 0.0f;

    TVec3f postureDir;
    MR::vecKillElement(rDamageDir, getGravityVec(), &postureDir);

    if (MR::normalizeOrZero(&postureDir) == 0) {
        _60 = -postureDir;
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
            f32 dot = _60.dot(padDir);
            f32 scale = MR::clamp(1.0f - dot, 0.0f, 1.0f);

            targetTiltX = absAngle * scale;

            TVec3f cross;
            PSVECCrossProduct(&_60, &padDir, &cross);
            if (cross.dot(mUpVec) > 0.0f) {
                targetTiltX = -targetTiltX;
            }

            if (__fabsf(_E0) >= __fabsf(targetTiltX)) {
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
        if (_3C == 0) {
            f32 scaledTilt = (getStickY() * PI);
            targetTiltY = scaledTilt / 12.0f;

            if (getStickY() > 0.0f && _5C <= 2.0f / 3.0f * PI) {
                targetTiltY = getStickY() * PI * 0.125f;
            }
        } else {
            if (getStickY() < 0.0f) {
                f32 divisor = mActor->getConst().getTable()->mSwimTiltZdown;
                targetTiltY = (getStickY() * PI) / divisor;
            } else if (getStickY() > 0.0f) {
                f32 divisor = mActor->getConst().getTable()->mSwimTiltZup;
                targetTiltY = (getStickY() * PI) / divisor;
            } else {
                if (_3C != 0) {
                    f32 fade = (f32)_3C / 120.0f;
                    if (fade > 1.0f)
                        fade = 1.0f;
                    targetTiltY = targetTiltY - (fade * (PI * 0.1f));
                } else {
                    targetTiltY = 0.0f;
                }
            }
        }
    }

    _E0 = (blendX * targetTiltX) + ((1.0f - blendX) * _E0);
    _E4 = (0.1f * targetTiltY) + (0.9f * _E4);

    Mtx mtxRotX, mtxRotZ;
    PSMTXRotRad(mtxRotX, 'x', _E0);
    PSMTXRotRad(mtxRotZ, 'z', _E4);
    PSMTXConcat(mtxRotX, mtxRotZ, _B0);

    u16 jointID = getAnimator()->getUpperJointID();
    setJointGlobalMtx(jointID, _B0);
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
                        MR::vecKillElement(newHead, getGravityVec(), &_60);

                        Mario* player = getPlayer();
                        _5C = MR::diffAngleAbs(player->mHeadVec, -getGravityVec());

                    } else {
                        MR::vecBlendSphere(_60, pushDir, &_60, 0.2f);
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
            if (MR::normalizeOrZero(&direction) == 0) {
                _60 = direction;
            }
        }
    }

    TVec3f pushBack = getTrans() - pHit->mHitPos;
    MR::vecKillElement(pushBack, *pHit->mParentTriangle.getNormal(0), &pushBack);
    pushBack.setLength(5.0f);
    addVelocity(pushBack);
}

void MarioSwim::hitPoly(u8 type, const TVec3f& rNormal, HitSensor* pSensor) {
    if (_24 <= 120) {
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

    f32 angle = MR::diffAngleAbsHorizontal(negNormal, _60, swimGravity);
    if (invDot >= 0.5f && angle <= PI / 4) {
        _22 = true;
    }
}

void MarioSwim::hitWall(const TVec3f& rNormal, HitSensor* pSensor) {
    if (mActor->_424 == 0) {
        f32 floorDot = -rNormal.dot(getPlayer()->getAirGravityVec());

        if (!mIsOnSurface && floorDot > 0.70700002f) {
            _1F = 1;
            return;
        } else if (mIsOnSurface) {
            if (MR::diffAngleAbsHorizontal(-getAirGravityVec(), _60, rNormal) < 0.78539819f) {
                _1B0 = 6;
            }

            if (_24 <= 0x78) {
                return;
            }

            // If on surface and hitting something slope-like, treat as slide/floor interaction
            if (floorDot >= 0.5f) {
                _22 = 1;
                return;
            }
        }

        bool isHit = false;

        if (!mIsOnSurface && _54 > 1.0f) {
            TVec3f projectedVelocity;
            MR::vecKillElement(getPlayer()->_1FC, rNormal, &projectedVelocity);

            if (projectedVelocity.dot(mUpVec) > 0.0f) {
                if (getStickY() <= 0.0f) {
                    _5C -= 0.1f;
                }
            } else {
                if (getStickY() >= 0.0f) {
                    _5C += 0.1f;
                }
            }
        }

        _1B0 = 6;
        _36 = 0;

        if (mJetTimer != 0 || (!MR::isNearZero(_190, 0.001f) && _2C != 0)) {
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
                    if (rNormal.dot(getGravityVec()) > 0.1f && _5C < 1.5707964f) {
                        _5C = 3.1415927f - _5C;
                    }

                    _AE = 20;
                    _A0 = rNormal.scaleInline(6.0f);

                    if (MR::isNearZero(_190, 0.001f)) {
                        f32 impactAngle = MR::diffAngleAbsHorizontal(_60, rNormal, getAirGravityVec());

                        if (impactAngle >= 2.6179941f) {
                            f32 headAngle = MR::diffAngleAbs(getPlayer()->mHeadVec, rNormal);

                            if (headAngle <= 2.6179941f) {
                                if (headAngle >= 6.2831855f) {
                                    TVec3f cross1;
                                    PSVECCrossProduct(&rNormal, &getPlayer()->mHeadVec, &cross1);
                                    MR::normalizeOrZero(&cross1);

                                    TVec3f cross2;
                                    PSVECCrossProduct(&getPlayer()->mHeadVec, &cross1, &cross2);
                                    MR::normalizeOrZero(&cross2);

                                    getPlayer()->forceSetHeadVecKeepSide(cross2);

                                    Mario* player = getPlayer();
                                    _5C = MR::diffAngleAbs(player->mHeadVec, -getAirGravityVec());
                                }
                                isReflect = false;

                            } else {
                                _AE = 60;
                                _A0 = rNormal.scaleInline(16.0f);
                            }
                        } else {
                            TVec3f glanceDir;
                            MR::vecKillElement(_60, rNormal, &glanceDir);
                            if (!MR::normalizeOrZero(&glanceDir)) {
                                bool isTurn = false;
                                _60 = glanceDir;
                                speedReduction = 0.8f;
                                isReflect = false;
                            }
                        }
                    } else {
                        if (velocityAfterHit.dot(_190) > 0.0f) {
                            _60 = velocityAfterHit;
                        } else {
                            _AE = 60;
                            _A0 = rNormal.scaleInline(16.0f);
                        }
                    }
                }

                if (mJetTimer != 0 && isReflect) {
                    playEffectTrans("壁ヒット", getPlayer()->getWallPos());
                    playSound("亀壁ヒット", -1);
                    changeAnimation("水泳ジェット壁ターン", static_cast< const char* >(nullptr));
                }

                _54 *= speedReduction;
                isHit = true;
            }
        }

        // Spin Attack Collision
        if (_7C != 0) {
            if (!sendPunch(pSensor, true)) {
                _54 = 0.0f;
                forceStopSpin();
                changeAnimation("水泳壁ヒット", static_cast< const char* >(nullptr));
                playEffectTrans("壁ヒット", getPlayer()->getWallPos());
                startCamVib(0);
                _AE = 60;
                _A0 = rNormal.scaleInline(6.0f);
            }
            isHit = true;
        } else if (_7A != 0) {
            if (!sendPunch(pSensor, false)) {
                _7A = 0;
                _54 = 0.0f;
                playEffectTrans("壁ヒット", getPlayer()->getWallPos());
                startCamVib(2);
                _AE = 30;
                _A0 = rNormal.scaleInline(6.0f);
            }
            isHit = true;
        }

        // Ring Dash Collision
        if (mRingDashTimer != 0) {
            if (mJetTimer != 0) {
                dropJet(true);
            }
            _54 = 0.0f;
            changeAnimation("水泳壁ヒット", static_cast< const char* >(nullptr));
            playEffectTrans("壁ヒット", getPlayer()->getWallPos());
            startCamVib(0);
            _AE = 60;
            _A0 = rNormal.scaleInline(6.0f);
            isHit = true;
        }

        mRingDashTimer = 0;
        mRingDashChargeTimer = 0;

        if (isHit) {
            startPadVib(2);
        }

        // Wall Push / Kick Logic check
        if (rNormal.dot(getGravityVec()) > 0.80000001f) {
            TVec3f* worldPadDir = &getWorldPadDir();
            if (getStickP() != 0.0f) {
                if (checkTrgA()) {
                    if (_AE == 0) {
                        _AE = 60;
                    }
                    TVec3f gravVecScaled = getGravityVec().scaleInline(10.0f);
                    TVec3f worldPadDirScaled = (*worldPadDir).scaleInline(10.0f);
                    _A0 = gravVecScaled.translate(worldPadDirScaled);
                }
            }
        }

        stopEffect("水泳スピン");
    }
}

void MarioSwim::decLife() {
    _E8 = 0;
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
    _E8 = 0;
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

    _EC = 120;
    _EE = 120;
}

void MarioSwim::decOxygen(u16 amount) {
    if (mOxygen == 0)
        return;

    MarioActor* actor = mActor;

    if (actor->_934 != 0)
        return;
    if (actor->_EA4 != 0)
        return;

    if (isStatusActiveID(0x22))
        return;

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
        _F0 = 120;
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
                _F0 = 120;
            }
            getPlayer()->_1C._3 = 1;

            if (!_F0) {
                _F0 = 120;
            } else {
                _F0--;
                if (_F0 == 0) {
                    doDecLifeByCold();
                    _F0 = 120;
                }
            }
        } else {
            _F0 = 0;
        }

        if (isColdWater || mIsOnSurface) {
            _E8 = 0;
            _EE = 120;

            if (mOxygen != mActor->getConst().getTable()->mOxygenMax) {
                mOxygen += 32;
                if (mOxygen > mActor->getConst().getTable()->mOxygenMax) {
                    playSound("酸素回復最大", -1);
                    mOxygen = mActor->getConst().getTable()->mOxygenMax;
                } else {
                    playSound("酸素回復", -1);
                }

                _EC = 180;
            }
        } else {
            if (mOxygen) {
                if (_EE) {
                    _EE--;
                } else {
                    decOxygen(1);
                }
            } else {
                _E8++;
                u32 damageInterval = mActor->getConst().getTable()->mWaterDecInterval;
                if (_E8 >= damageInterval) {
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

        if (_54 < targetSpeed) {
            if (_54 < 1.0f) {
                _54 = 1.0f;
            }

            f32 ratio = targetSpeed / _54;
            if (ratio > 1.5f) {
                _54 = 1.5f * _54;
            } else {
                _54 = targetSpeed;
            }
        } else {
            if (_54 > targetSpeed) {
                f32 ratio = targetSpeed / _54;
                if (ratio < 0.98f) {
                    _54 = 0.98f * _54;
                } else {
                    _54 = targetSpeed;
                }
            } else {
                _54 = targetSpeed;
            }
        }
    }
}

void MarioSwim::resetAndFixPose() {
    _2C = 0;
    _2E = 0;
    _30 = 0;
    _32 = 0;
    _3C = 0;
    _3A = 0;
    _36 = 0;
    _7C = 0;
    _7A = 0;

    _54 = 0.0f;
    _4C = 0.0f;
    _50 = 0.0f;
    _58 = 0.0f;

    MR::vecKillElement(getFrontVec(), getGravityVec(), &_60);

    Mario* player = getPlayer();
    _5C = MR::diffAngleAbs(player->mHeadVec, -getGravityVec());
}

f32 MarioSwim::getStickY() const {
    f32 stickY = MarioModule::getStickY();

    if (_21 != 0 && stickY < 0.0f) {
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
        _EC = 180;
    }
    if (mOxygen == (mActor->getConst().getTable()->mOxygenMax / 4)) {
        playSound("酸素減少警告", -1);
    }
    if (mOxygen == (mActor->getConst().getTable()->mOxygenMax / 2)) {
        _EC = 180;
        playSound("酸素減少警告", -1);
    }

    if (_EC != 0) {
        _EC--;
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
        f32 speedRatio = MR::clamp(_54 / table->mSwimToWalkSpd, 0.0f, 1.0f);

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

/*
 *
 * ***********************************************
 * IMPORTANT: THIS FUNCTION SHOULD BE DELETED ONCE
 * THIS OBJECT FILE MATCHES
 * ***********************************************
 *
 * This function is only present so that the
 * compiler does not cull these variables
 *
 */
void dummyFunction() {
    cFrontAcc[0] = 1.0f;
    cFrontAccSpin[0] = 1.0f;
    cFrontAccSpinSurface[0] = 1.0f;
    cWeightTable[0] = 1.0f;
    cWeightTableSP[0] = 1.0f;
};
