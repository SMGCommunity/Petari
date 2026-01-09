#include "Game/Player/MarioSwim.hpp"
#include "Game/AreaObj/AreaObj.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/Map/HitInfo.hpp"
#include "Game/Map/WaterInfo.hpp"
#include "Game/Player/MarineSnow.hpp"
#include "Game/Player/MarioActor.hpp"
#include "Game/Player/MarioAnimator.hpp"
#include "Game/Player/MarioConst.hpp"
#include "Game/Screen/GameSceneLayoutHolder.hpp"
#include "Game/Util/AreaObjUtil.hpp"
#include "Game/Util/SceneUtil.hpp"
#include "JSystem/JGeometry/TVec.hpp"
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

    f32 cLimitAngleSink = 0.0f;
    f32 cNeutralAngleWait = 0.0f;
    f32 cLimitAngleWait = 0.0f;
    f32 cUpperAngleWait = 0.0f;
    f32 cWaterSurfaceRange = 160.0f;
    f32 cTurnMotionSpeed = 5.0f;
}  // namespace

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

/*
bool Mario::checkStartSwim()
{
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
            TVec3f stack_14(-getGravityVec() % 100.0f);
            addTrans(stack_14, nullptr);
            if (getPlayer()->getMovementStates()._0) {
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
    mSwim->updateUnderwater();
    return false;
}
*/

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
    _20 = 0;
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
    mIsNormalRingDash = 0;
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
    _1AC = 0.0f;
    _1B2 = 0;
    _1B4 = 0.0f;
    _1B8 = 0.0f;
    _14 = new MarineSnow();
}

void MarioSwim::init() {
    mOxygen = mActor->getConst().getTable()->mOxygenMax;
}

inline f32 getSpecialNumber() {
    return 1.74532938004f;
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
        mIsNormalRingDash = 0;
    }
    if (_9D == 4) {
        r1d = 1;
    }
    if (mIsNormalRingDash && getPlayer()->getMovementStates().jumping) {
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
        TVec3f surfacePosCopy(mSurfacePos);
        surfacePosCopy.scale(fr1f);
        actor->emitEffectWaterColumn(-mSurfacePos, getTrans() - surfacePosCopy);
    } else {
        mActor->emitEffectWaterColumn(mSurfacePos, mSurfaceNorm);
    }
    if (!getPlayer()->getMovementStates().jumping || getPlayer()->mVerticalSpeed > 100.0f || r1e) {
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

            if (mIsNormalRingDash) {
                changeAnimationNonStop("水泳ジェット");
            } else {
                changeAnimation("水泳ジャンプダイブ回転", static_cast<const char*>(nullptr));
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
            changeAnimation("水上ダメージ着水", static_cast<const char*>(nullptr));
            _2E = mActor->getConst().getTable()->mJumpDamageSinkTimer;
            _AE = 0x78;
        } else {
            _54 = (0.2f + 0.1f * fr1f) * mActor->getConst().getTable()->mSwimFrontMaxSpeed;
            changeAnimation("水泳ジャンプダイブ", static_cast<const char*>(nullptr));

            mActor->setBlendMtxTimer(8);
            _2C = 15;
            _2E = mActor->getConst().getTable()->mJumpSinkTimer;

            _20 = 1;
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
        _A0.scaleInline(0.5f);
    }
    if (mActor->_468) {
        if (!mIsNormalRingDash) {
            if (mActor->getCarrySensor().isType(15) || mActor->getCarrySensor().isType(16)) {
                u32 r1b = 0;
                if (mActor->getCarrySensor().isType(16)) {
                    r1b = 1;
                }
                startJet(r1b);

                r1e = 2;
                _20 = 0;
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

inline f32 getSwimValue(f32 stick, int index, const MarioConstTable* table) {
    if (index == 2)
        return table->mSwimRotXIne;
    if (index == 3)
        return table->mSwimRotZIne;
    return 0.0f;
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
                if (mIsNormalRingDash) {
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

    if (mIsOnSurface || (_20 && checkTrgA())) {
        if (_7A || _7C) {
            _9C = 0;
        } else {
            if (mIsNormalRingDash) {
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
                if (isAnimationRun("水上一掻き")) {
                    tmp = 0x1E;
                }

                if (checkLvlA()) {
                    if (_9C && (!isAnimationRun("水上一掻き") || !_28)) {
                        _28++;
                    }
                    if ((_1B0 || mIsNormalRingDash || getPlayer()->mMovementStates._8 || getPlayer()->mMovementStates._32) && checkTrgA()) {
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
        if (!_2C && !_32 && !_2E && !mIsNormalRingDash) {
            getAnimator()->forceSetBlendWeight(cWeightTable);
            changeAnimation("炊泳上昇呼吸", static_cast<const char*>(nullptr));
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
        if (mIsNormalRingDash && (getStickY() < -0.1f || checkLvlA()) && getPlayer()->mVerticalSpeed - mWaterDepth > 200.0f) {
            doJetJump(0);
            return false;
        }
        if (mRingDashTimer && getPlayer()->mVerticalSpeed - mWaterDepth > 200.0f) {
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
            if (mIsNormalRingDash && getPlayer()->mVerticalSpeed < 100.0f) {
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
                if (mIsNormalRingDash) {
                    if (getStickY() > 0.707000017166f) {
                        _5C = mActor->getConst().getTable()->mZsinkAngleX;
                        _20 = 0;
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
                    changeAnimation("水泳潜り", static_cast<const char*>(nullptr));
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
            if (!mRingDashTimer && mDistToFloor > 200.0f && checkTrgZ() && !_32 && !_2E && !mIsNormalRingDash && !check7Aand7C()) {
                stopAnimation(nullptr, static_cast<const char*>(nullptr));
                changeAnimation("水泳潜り", static_cast<const char*>(nullptr));
                playSound("水中潜り", -1);
                playEffect("水面Ｚ沈降");
                _32 = mActor->getConst().getTable()->mZsinkStartTimer;
            }
            if (mIsNormalRingDash) {
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
        if (!_20 && !_3C && _2C) {
            _5C += res * (_4C * mActor->getConst().getTable()->mSwimRotSpeedX);
        }

        if ((_54 < cTurnMotionSpeed || mIsOnSurface || checkLvlA() || checkLvlZ()) && isAnimationRun("水泳一掻き") &&
            isAnimationTerminate("水泳一掻き")) {
            stopAnimation(nullptr, static_cast<const char*>(nullptr));
        }

        if (!checkLvlA() && !checkLvlZ() && !_2C && _54 < cTurnMotionSpeed) {
            bool worthless;
            bool& worthlesser = worthless;
            funReferenceTime(worthlesser);
            if (!_20 && !_AE && mWaterDepth < -400.0f) {
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
        if (mIsNormalRingDash) {
            switch (_88) {
            case 0:
                fr1e *= mActor->getConst().getTable()->mSwimXJetRotRatio;
                break;
            default:
                fr1e *= mActor->getConst().getTable()->mSwimXJetRotRatioRed;
                break;
            }
        }
        if (_20) {
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
            if (!check7Aand7C() && !mIsNormalRingDash) {
                changeAnimation("水泳ターン下", static_cast<const char*>(nullptr));
            }
            r1b = false;
        }
        if (r1b) {
            stopAnimation("水泳ターン下", static_cast<const char*>(nullptr));
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

    if ((_20 || (mIsOnSurface && isStickOn())) && !mIsNormalRingDash) {
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
        if (mIsNormalRingDash) {
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
    TVec3f stack_14C, stack_140;
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
    TVec3f stack_110 = getPlayer()->_1FC;
    if (_34 || _2E || isAnimationRun("水泳ジャンプダイブ回転") || isAnimationRun("水泳潜り")) {
        PSVECCrossProduct(&stack_14C, &stack_140, &stack_110);
    }
    if (!mIsNormalRingDash) {
        decideVelocity();
    }
    TVec3f stack_104 = createAndScale(_54, stack_110);
    if (mIsOnSurface && mUpVec.dot(stack_110) > 0.0f) {
        f32 mag = PSVECMag(&stack_104);
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
    TVec3f buoyancyEffect = mUpVec;
    buoyancyEffect.scale(element * fr1d);
    stack_104 += buoyancyEffect;
    addVelocity(stack_104);
    u16 fr1b = _2E;
    if (fr1b) {
        TVec3f sinkGrav = getGravityVec();
        sinkGrav.scale(0.25f * fr1b);
        addVelocity(sinkGrav);
        _2E--;
    }

    if (_34) {
        _34--;
    }
    if (!mIsOnSurface && !_2C && !mIsNormalRingDash) {
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
                    if (PSVECMag(&diff) < 10.0f) {
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
            _A0.scaleInline(0.98f);
        }
    }
    if (mIsNormalRingDash) {
        addVelocity(_184, mActor->getConst().getTable()->mTurboReductionStream);
    } else {
        addVelocity(_184);
    }

    TVec3f stack_EC(_184);
    f32 fr1dv2 = PSVECMag(&stack_EC);
    if (!MR::normalizeOrZero(&stack_EC)) {
        if (MR::diffAngleAbs(stack_EC, _60) < 1.57079637051f) {
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

    if (!mIsNormalRingDash && !mIsOnSurface && !MR::isNearZero(_190)) {
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

TVec3f& MarioSwim::getGravityVec() const {
    MarioModule::getGravityVec();
}

void MarioSwim::onSurface() {
    if (!mIsOnSurface) {
        mIsOnSurface = true;
    }
}

f32 MarioSwim::getBlurOffset() const {
    return mBlurOffset;
}

f32 MarioSwim::getStickY() const {
    f32 stickY = MarioModule::getStickY();

    if (this->_21 != 0 && stickY < 0.0f) {
        if (stickY > -0.5f) {
            stickY = 0.0f;
        } else {
            stickY = (0.5f + stickY) / 0.5f;
        }
    }

    return stickY;
}

bool MarioSwim::notice() {
    if (MarioState::getNoticedStatus() == 18) {
        return true;
    }

    return getNoticedStatus() == 11;
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

void MarioSwim::incOxygen() {
    MarioActor* actor = mActor;
    u16 currentOxygen = mOxygen;
    MarioConst* mConst = actor->mConst;
    MarioConstTable* table = actor->mConst->getTable();

    if (currentOxygen == table->mOxygenMax) {
        return;
    }
    u32 maxOxygen = actor->mConst->getTable()->mOxygenMax;
    mOxygen = mOxygen + (maxOxygen / 8);
    u32 oxygenCheck = mOxygen;

    MarioConstTable* tableCheck = actor->mConst->getTable();
    u16 maxOxygenCheck = tableCheck->mOxygenMax;
    if (oxygenCheck > maxOxygenCheck) {
        // 0x64: sth r0, 0xea(r3)
        mOxygen = maxOxygenCheck;
    }
    
    MarioConstTable* soundTable = mActor->mConst->->getTable();
    if (mOxygen == soundTable->mOxygenMax) {
        // Play "Full" sound
        playSound("酸素回復最大", -1);
    } else {
        // Play "Recovering" sound
        playSound("酸素回復", -1);
    }

    _EC = 120;  
    _EE = 120;  
}

void MarioSwim::incLife() {
    _E8 = 0;
    mActor->incLife(1);
    incOxygen();
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

f32 MarioSwim::calcRingAcc() {
    if (this->mRingDashTimer != 0) {
        if (this->mRingDashChargeTimer != 0) {
            this->mRingDashChargeTimer--;

            if (this->mRingDashChargeTimer == 1) {
                MarioActor* actor = this->mActor;
                MarioConstTable* table = actor->mConst->getTable();

                actor->_1AA = table->mStarPieceFogTime;
                actor->_1AC = 0.0f;
                actor->_1B0.r = 0xFF;
                actor->_1B0.g = 0xFF;
                actor->_1B0.b = 0xFF;
                actor->_1B0.a = 0;
                actor->_1B5 = 0;
            }
            if (this->mRingDashChargeTimer == 0) {
                MarioModule::startPadVib(3);
                if (!this->mIsNormalRingDash) {
                    MarioModule::changeAnimation("カメ持ちリング", static_cast<const char*>(nullptr));
                } else {
                    MarioModule::changeAnimation("リングダッシュ", static_cast<const char*>(nullptr));
                }
            }

            return 1.0f;
        } else {
            this->mBlurOffset += 0.5f;
            if (this->mBlurOffset > 30.0f) {
                this->mBlurOffset = 30.0f;
            }

            f32 result = this->mRingDashSpeedScale;
            if (this->mRingDashTimer < this->mRingDashMaxDuration) {
                f32 ratio = (f32)this->mRingDashTimer / (f32)this->mRingDashMaxDuration;
                result = (1.0f - ratio) + (this->mRingDashSpeedScale * ratio);
            }

            this->mRingDashTimer--;
            return result;
        }
    } else {
        if (this->mBlurOffset > 0.0f) {
            this->mBlurOffset -= 0.5f;
        } else {
            this->mBlurOffset = 0.0f;
        }
        return 1.0f;
    }
}

void MarioSwim::hitPunch(const TVec3f& rPunchDir) {
    if (this->_7A) {
        this->_AE = 30;
        this->_54 = 0.0f;
        this->_A0 = rPunchDir;
        this->_78 = 1;

        MarioModule::stopAnimation("アッパーパンチ", static_cast<const char*>(nullptr));
    }
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

void MarioSwim::flowOnWave(f32 unused) {
    if (this->_2C) {
        return;
    }

    if (this->mWaterDepth < -cWaterSurfaceRange) {
        if (this->mIsOnSurface) {
            mIsOnSurface = false;
        }

        return;
    } else if (this->mIsOnSurface) {
        if (this->mWaterDepth < -25.0f) {
            addVelocity(this->mSurfacePos, this->_58);
        }
        if (this->mWaterDepth > 0.0f) {
            TVec3f inverseFlow = -this->mSurfacePos;
            addVelocity(inverseFlow, 5.0f + this->mWaterDepth);
        }
    }
    this->_3E++;
}

AreaInfo* MarioSwim::getWaterAreaInfo(WaterInfo* pInfo, const TVec3f& rPos, TVec2f* pOutVec) {
    AreaInfo* waterAreaInfo = MR::getWaterAreaInfo(pInfo, rPos, MarioModule::getPlayer()->getAirGravityVec(), false);

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
    _1AC = pInfo->mEdgeDistance;
    TVec3f currentFlow(pInfo->mStreamVec);

    float depthFactor = 1.0f;
    if (mWaterDepth > -500.0f) {
        depthFactor = -mWaterDepth / 500.0f;
    }

    float finalScale = MR::clamp(depthFactor, 0.0f, 1.0f);
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

bool MarioSwim::tryJetAttack(HitSensor* pHitSensor) {
    if (this->mActor->tryJetAttack(pHitSensor) == true) {
        this->dropJet(1);

        const TVec3f& trans = MarioModule::getTrans();
        TVec3f damageDir = trans - pHitSensor->mPosition;
        this->addDamage(damageDir);

        return true;
    } else {
        return false;
    }
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
