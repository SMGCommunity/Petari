#include "Game/Enemy/Takobo.hpp"
#include "Game/Enemy/AnimScaleController.hpp"
#include "Game/Enemy/SpinHitController.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/Map/HitInfo.hpp"
#include "Game/MapObj/SpinningBox.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "JSystem/JMath/JMath.hpp"

namespace NrvTakobo {
    NEW_NERVE_ONEND(HostTypeNrvWait, Takobo, Wait, Wait);
    NEW_NERVE_ONEND(HostTypeNrvMove, Takobo, Move, Move);
    NEW_NERVE(HostTypeNrvPress, Takobo, Press);
    NEW_NERVE_ONEND(HostTypeNrvAttack, Takobo, Attack, Attack);
    NEW_NERVE(HostTypeNrvHitPunch, Takobo, HitPunch);
    NEW_NERVE(HostTypeNrvStunStart, Takobo, StunStart);
    NEW_NERVE(HostTypeNrvStun, Takobo, Stun);
    NEW_NERVE(HostTypeNrvStunEnd, Takobo, StunEnd);
    NEW_NERVE(HostTypeNrvHitReaction, Takobo, HitReaction);
    NEW_NERVE(HostTypeNrvIce, Takobo, Ice);
    NEW_NERVE_ONEND(HostTypeNrvDpdPointed, Takobo, DpdPointed, DpdPointed);
};  // namespace NrvTakobo

Takobo::Takobo(const char* pName) : LiveActor(pName), _8C(0), _90(0, 0, 1), _A0(0), _9C(-1), _A1(0), _B0(1, 0, 0) {
    _BC = 0;
    _C0 = 5.0f;
    _C4 = 500.0f;
    _C8 = -500.0f;
    _CC = 500.0f;
    _D0 = 0;
    mBox = nullptr;
    mScaleController = nullptr;
    mSpinController = nullptr;
}

void Takobo::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    _A4.set< f32 >(mPosition);
    s32 arg = -1;
    MR::getJMapInfoArg0NoInit(rIter, &arg);

    if (arg > 0) {
        _A1 = 1;
    } else {
        _A1 = 0;
    }

    MR::useStageSwitchWriteDead(this, rIter);
    initModelManagerWithAnm("Takobo", nullptr, false);
    MR::connectToSceneEnemy(this);

    TPos3f mtx;
    mtx.setInline(getBaseMtx());
    TVec3f zDir;
    mtx.getZDir(zDir);

    TVec3f stack_24;
    stack_24.set< f32 >(mtx(0, 1), mtx(1, 1), mtx(2, 1));

    TVec3f stack_18;
    JMathInlineVEC::PSVECNegate(&stack_24, &stack_18);
    mGravity.set< f32 >(stack_18);

    s32 dir = -1;
    MR::getJMapInfoArg1NoInit(rIter, &dir);

    if (dir == 0) {
        mtx.getXDir(_B0);
        _BC = 0;
    } else if (dir == 1) {
        mtx.getXDir(_B0);
        _BC = 1;
    } else if (dir == 3) {
        mtx.getZDir(_B0);
        _BC = 1;
    } else {
        mtx.getZDir(_B0);
        _BC = 0;
    }

    arg = -1;
    MR::getJMapInfoArg2NoInit(rIter, &arg);

    if (arg != -1) {
        _C0 = arg;
    }

    arg = -1;
    MR::getJMapInfoArg3NoInit(rIter, &arg);

    if (arg != -1) {
        _C4 = arg;
    }

    MR::declareStarPiece(this, 3);
    MR::declareCoin(this, 1);
    initSound(4, false);
    initBinder(80.0f * mScale.y, 60.0f * mScale.y, 0);
    initEffectKeeper(0, nullptr, false);
    initSensor();
    MR::initShadowVolumeSphere(this, 70.0f * mScale.y);
    MR::onCalcShadow(this, nullptr);
    initNerve(&NrvTakobo::HostTypeNrvWait::sInstance);

    mSpinController = new SpinHitController(this, 22, 21, 3, 1.5f, 20.0f, 35.0f, 3, false);
    MR::initStarPointerTarget(this, 110.0f, TVec3f(0.0f, 80.0f, 0.0f));
    mScaleController = new AnimScaleController(nullptr);
    MR::initLightCtrl(this);
    appear();
    getSensor("attack")->invalidate();
    mBox = new SpinningBox("スピニングボックス");
    mBox->_B1 = 1;
    mBox->initWithoutIter();
}

void Takobo::initAfterPlacement() {
    MR::calcGravity(this);
    MR::offBind(this);
}

void Takobo::initSensor() {
    f32 yScale = mScale.y;
    initHitSensor(4);
    TVec3f offs;
    offs.x = 0.0f;
    offs.y = 70.0f * yScale;
    offs.z = 0.0f;
    MR::addHitSensor(this, "body", 26, 32, offs.y, offs);
    MR::addHitSensorAtJointEnemyAttack(this, "attack", "Hear", 0x10, (80.0f * yScale), TVec3f(10.0f, 2.0f, 0.0f));
}

void Takobo::kill() {
    MR::emitEffect(this, "TakoboDeath");
    MR::startSound(this, "SE_EM_EXPLODE_S", -1, -1);

    if (MR::isValidSwitchDead(this)) {
        MR::onSwitchDead(this);
    }

    LiveActor::kill();
}

void Takobo::control() {
    mScaleController->updateNerve();

    if (isNerve(&NrvTakobo::HostTypeNrvWait::sInstance) || isNerve(&NrvTakobo::HostTypeNrvMove::sInstance) ||
        isNerve(&NrvTakobo::HostTypeNrvAttack::sInstance) || isNerve(&NrvTakobo::HostTypeNrvHitReaction::sInstance)) {
        if (MR::isStarPointerPointing2POnPressButton(this, "弱", true, false)) {
            setNerve(&NrvTakobo::HostTypeNrvDpdPointed::sInstance);
        }
    } else if (_A1) {
        HitInfo info;
        if (Collision::checkStrikePointToMap(mPosition, &info)) {
            if (MR::isGroundCodeDamageFire(&info.mParentTriangle)) {
                TVec3f v7(*info.mParentTriangle.getFaceNormal());
                v7 *= 28.0f;
                TVec3f v6(mGravity);
                v6 *= 28.0f;
                v7 -= v6;
                mVelocity.setPS(v7);

                if (MR::isOnGround(this)) {
                    TVec3f v5(*info.mParentTriangle.getFaceNormal());
                    v5 *= 5.0f;
                    mPosition += v5;
                }

                setNerve(&NrvTakobo::HostTypeNrvHitPunch::sInstance);
            }
        }
    }
}

void Takobo::generateCoin() {
    TVec3f v3(mGravity);
    v3 *= 80.0f;
    TVec3f v4(mPosition);
    v4 -= v3;
    MR::appearCoinPop(this, v4, 1);
}

bool Takobo::tryPress() {
    if (!isNerve(&NrvTakobo::HostTypeNrvPress::sInstance)) {
        MR::startSound(this, "SE_EM_TAKOBO_STOMPED", -1, -1);
        setNerve(&NrvTakobo::HostTypeNrvPress::sInstance);
        return true;
    }

    return false;
}

void Takobo::exeMove() {
    if (MR::isFirstStep(this)) {
        TVec3f v21(mPosition);
        f32 val = _C4;
        TVec3f v18(_B0);
        v18 *= val;
        TVec3f v19(_A4);
        v19 -= v18;
        v21 -= v19;

        if (_BC) {
            _C8 = -_C4;
            f32 v6 = v21.dot(_B0);
            f32 c0 = _C0;
            f32 v8 = (-_C4 + v6);
            f32 v9 = ((-_C4 + v6) - _C8);
            _CC = v8;
            _D0 = v9 / c0;
        } else {
            f32 v3 = v21.dot(_B0);
            f32 v4 = _C4;
            f32 v5 = _C0;
            _CC = _C4;
            _C8 = -v4 + v3;
            _D0 = ((v4 - (-v4 + v3)) / v5);
        }

        _D0 *= 1.3f;
    }

    f32 rate = MR::calcNerveRate(this, _D0);

    if (_BC) {
        rate -= 1.0f;
    }

    f32 ease = MR::getEaseInOutValue(rate, _C8, _CC, 1.0f);
    TVec3f v15(_B0);
    v15 *= ease;
    TVec3f v16(_A4);
    v16 += v15;
    TVec3f v17(v16);
    v17 -= mPosition;
    mVelocity.set< f32 >(v17);

    if (MR::isGreaterStep(this, _D0)) {
        _BC = _BC == false;
        setNerve(&NrvTakobo::HostTypeNrvMove::sInstance);
    } else {
        TVec3f v20(*MR::getPlayerPos());
        v20 -= mPosition;
        f32 mag = PSVECMag(&v20);
        MR::vecKillElement(v20, mGravity, &v20);
        MR::normalizeOrZero(&v20);

        if (mag < 250.0f) {
            if (MR::turnVecToVecRadian(&_90, _90, v20, 0.050000001f, mGravity) < 0.1f) {
                setNerve(&NrvTakobo::HostTypeNrvAttack::sInstance);
            }
        } else {
            if (mag < 800.0f) {
                MR::turnVecToVecRadian(&_90, _90, v20, 0.05f, mGravity);
            }
        }
    }
}

void Takobo::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Wait");
        mVelocity.zero();
    }

    if (MR::isGreaterStep(this, 60)) {
        setNerve(&NrvTakobo::HostTypeNrvMove::sInstance);
    }
}

void Takobo::exePress() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Press");
        MR::clearHitSensors(this);
        MR::invalidateHitSensors(this);
    }

    if (MR::isGreaterStep(this, 30)) {
        generateCoin();
        kill();
    }
}

void Takobo::exeHitPunch() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "HitPunch");
        MR::startSound(this, "SE_EM_TAKOBO_HITPUNCH", -1, -1);
        MR::startBlowHitSound(this);
        MR::clearHitSensors(this);
        MR::invalidateHitSensors(this);
        MR::onBind(this);
        MR::onCalcShadow(this, nullptr);
    }

    if (mSpinController->execute(this)) {
        MR::startSound(this, "SE_EM_EXPLODE_S", -1, -1);
    }
}

void Takobo::exeStunEnd() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "StunEnd");
    }

    if (MR::isActionEnd(this)) {
        setNerve(&NrvTakobo::HostTypeNrvWait::sInstance);
    }
}

void Takobo::exeHitReaction() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "HitReaction");
    }

    if (MR::isActionEnd(this)) {
        setNerve(&NrvTakobo::HostTypeNrvWait::sInstance);
    }
}

void Takobo::exeAttack() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Attack");
    }

    mVelocity.zero();

    if (MR::isStep(this, 85)) {
        MR::setBckRate(this, 1.0f);
        getSensor("attack")->validate();
    }

    if (MR::isStep(this, 90)) {
        getSensor("attack")->invalidate();
    }

    if (MR::isActionEnd(this)) {
        setNerve(&NrvTakobo::HostTypeNrvWait::sInstance);
    }
}

void Takobo::exeStunStart() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "StunStart");
        MR::setBckRate(this, 2.0f);
        MR::emitEffect(this, "Hit");
        MR::startSound(this, "SE_EM_TAKOBO_STARPIECE_HIT", -1, -1);
        _A0 = 1;
        mVelocity.zero();
    }

    if (MR::isActionEnd(this)) {
        setNerve(&NrvTakobo::HostTypeNrvStun::sInstance);
    }
}

void Takobo::exeStun() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Stun");
        _A0 = 1;
    }

    MR::startLevelSound(this, "SE_EM_LV_SWOON_S", -1, -1, -1);

    if (MR::isGreaterStep(this, 120)) {
        _A0 = 0;
        setNerve(&NrvTakobo::HostTypeNrvStunEnd::sInstance);
    }
}

void Takobo::exeIce() {
    if (MR::isFirstStep(this)) {
        MR::hideModel(this);
        MR::invalidateHitSensors(this);
    }

    if (MR::isDead(mBox)) {
        mPosition.set< f32 >(mBox->mPosition);
        generateCoin();
        kill();
    }
}

void Takobo::exeDpdPointed() {
    if (MR::isFirstStep(this)) {
        MR::startDPDHitSound();
        MR::stopBck(this);
        mScaleController->startDpdHitVibration();
        MR::emitEffect(this, "Touch");
    }

    MR::startDPDFreezeLevelSound(this);
    if (!MR::isStarPointerPointing2POnPressButton(this, "弱", true, false)) {
        setNerve(&NrvTakobo::HostTypeNrvWait::sInstance);
    } else {
        mVelocity.zero();
    }
}

void Takobo::attackSensor(HitSensor* a1, HitSensor* a2) {
    if (isNerve(&NrvTakobo::HostTypeNrvIce::sInstance)) {
        return;
    }
    if (MR::isSensorEnemyAttack(a1)) {
        if (MR::isSensorPlayer(a2)) {
            MR::sendMsgEnemyAttackStrong(a2, a1);
        }
        return;
    }
    if (MR::isSensorPlayer(a2)) {
        if (MR::isPlayerHipDropFalling() || MR::isPlayerHipDropLand()) {
            return;
        }
        if (isNerve(&NrvTakobo::HostTypeNrvMove::sInstance) || isNerve(&NrvTakobo::HostTypeNrvAttack::sInstance)) {
            if (MR::isPlayerExistUp(this, getSensor("body")->mRadius, 0.25)) {
                MR::sendMsgPush(a2, a1);
                return;
            }
            if (MR::sendMsgEnemyAttack(a2, a1)) {
                mScaleController->startHitReaction();
                setNerve(&NrvTakobo::HostTypeNrvHitReaction::sInstance);
            } else {
                MR::sendMsgPush(a2, a1);
            }

        } else if (!isNerve(&NrvTakobo::HostTypeNrvPress::sInstance) && !isNerve(&NrvTakobo::HostTypeNrvHitPunch::sInstance)) {
            MR::sendMsgPush(a2, a1);
        }
    }

    if (MR::isSensorEnemy(a2) || MR::isSensorMapObj(a2)) {
        MR::sendMsgPush(a2, a1);
    }
}

bool Takobo::receiveOtherMsg(u32 msg, HitSensor* a2, HitSensor* a3) {
    if (MR::isMsgPlayerKick(msg) && isNerve(&NrvTakobo::HostTypeNrvStun::sInstance)) {
        mSpinController->start(this, a2->mPosition, a3->mPosition);
        setNerve(&NrvTakobo::HostTypeNrvHitPunch::sInstance);
        return true;
    }

    return false;
}

bool Takobo::receiveMsgPlayerAttack(u32 msg, HitSensor* a2, HitSensor* a3) {
    if (MR::isMsgPlayerTrample(msg)) {
        if (MR::isSensorEnemyAttack(a3)) {
            return 0;
        } else if (isNerve(&NrvTakobo::HostTypeNrvPress::sInstance)) {
            return 0;
        } else {
            return tryPress();
        }
    } else if (MR::isMsgPlayerHipDrop(msg)) {
        if (MR::isSensorEnemyAttack(a3)) {
            return 0;
        } else if (isNerve(&NrvTakobo::HostTypeNrvPress::sInstance)) {
            return 0;
        } else if (isNerve(&NrvTakobo::HostTypeNrvHitPunch::sInstance)) {
            return 0;
        } else {
            MR::startSound(this, "SE_EM_TAKOBO_STOMPED", -1, -1);
            setNerve(&NrvTakobo::HostTypeNrvPress::sInstance);
            return 1;
        }
    } else if (MR::isMsgFreezeAttack(msg)) {
        if (MR::isSensorEnemyAttack(a3)) {
            return 0;
        } else {
            mBox->generateIceBox(a2, a3);
            setNerve(&NrvTakobo::HostTypeNrvIce::sInstance);
            return 1;
        }
    } else if (isNerve(&NrvTakobo::HostTypeNrvHitPunch::sInstance) || isNerve(&NrvTakobo::HostTypeNrvPress::sInstance)) {
        return 0;
    } else if (MR::isMsgLockOnStarPieceShoot(msg)) {
        return 1;
    } else if (MR::isMsgStarPieceAttack(msg)) {
        setNerve(&NrvTakobo::HostTypeNrvStunStart::sInstance);
        return 1;
    } else if (MR::isMsgPlayerHitAll(msg)) {
        setNerve(&NrvTakobo::HostTypeNrvHitPunch::sInstance);
        mSpinController->start(this, a2->mPosition, a3->mPosition);
        return 1;
    } else {
        return 0;
    }
}

bool Takobo::receiveMsgEnemyAttack(u32 msg, HitSensor* a2, HitSensor* a3) {
    if (!a3->isType(26)) {
        return false;
    }

    if (isNerve(&NrvTakobo::HostTypeNrvHitPunch::sInstance) || isNerve(&NrvTakobo::HostTypeNrvPress::sInstance)) {
        return false;
    }

    if (MR::isMsgToEnemyAttackTrample(msg)) {
        if (isNerve(&NrvTakobo::HostTypeNrvPress::sInstance)) {
            return false;
        } else {
            return tryPress();
        }
    } else if (MR::isMsgToEnemyAttackBlow(msg)) {
        mSpinController->startWithoutStopScene(this, a2->mPosition, a3->mPosition);
        setNerve(&NrvTakobo::HostTypeNrvHitPunch::sInstance);
        return true;
    } else if (MR::isMsgExplosionAttack(msg)) {
        mSpinController->startWithoutStopScene(this, a2->mPosition, a3->mPosition);
        setNerve(&NrvTakobo::HostTypeNrvHitPunch::sInstance);
        return true;
    }

    return false;
}

void Takobo::calcAndSetBaseMtx() {
    TPos3f mtx;
    MR::calcMtxFromGravityAndZAxis(&mtx, this, mGravity, _90);
    MR::setBaseTRMtx(this, mtx);
    TVec3f scale;
    JMathInlineVEC::PSVECMultiply(&mScaleController->_C, &mScale, &scale);
    MR::setBaseScale(this, scale);
}

Takobo::~Takobo() {
}

void Takobo::endDpdPointed() {
    mScaleController->startAnim();
    MR::deleteEffect(this, "Touch");
}

void Takobo::endAttack() {
    getSensor("attack")->invalidate();
}

void Takobo::endMove() {
    MR::onCalcAnim(this);
    mVelocity.zero();
}

void Takobo::endWait() {
    MR::onCalcAnim(this);
    mVelocity.zero();
}
