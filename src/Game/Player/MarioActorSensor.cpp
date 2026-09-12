#include "Game/LiveActor/HitSensor.hpp"
#include "Game/MapObj/CollectCounter.hpp"
#include "Game/Player/Mario.hpp"
#include "Game/Player/MarioActor.hpp"
#include "Game/Player/MarioAnimator.hpp"
#include "Game/Player/MarioConst.hpp"
#include "Game/Player/MarioFoo.hpp"
#include "Game/Player/MarioRabbit.hpp"
#include "Game/Player/MarioSwim.hpp"
#include "Game/Player/MarioWall.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/EventUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"
#include <cstring>

void MarioActor::setupSensors() {
    initHitSensor(4);

    MR::addHitSensorCallback(this, "body", ATYPE_PLAYER, 32, 100.0f);
    MR::addHitSensorCallback(this, "ex-eye", ATYPE_PUSH, 32, 100.0f);
    MR::addHitSensorCallback(this, "eye", ATYPE_EYE, 64, 2000.0f);

    MR::addHitSensor(this, "dummy", ATYPE_PLAYER, 1, 0.0f, TVec3f(0.0f, 0.0f, 0.0f));

    getSensor("dummy")->invalidate();

    _468 = 0;
    _46C = nullptr;

    _7DC = 0;

    for (s32 idx = 0; idx < ARRAY_SIZE(_4D0); idx++) {
        _4D0[idx] = nullptr;
    }

    for (s32 idx = 0; idx < ARRAY_SIZE(_428); idx++) {
        _428[idx] = nullptr;
    }

    initScouter();

    _3E5 = false;
    _3E8 = false;

    MR::initStarPointerTarget(this, 80.0f, TVec3f(0.0f, 80.0f, 0.0f));
}

void MarioActor::updateHitSensor(HitSensor* sensor) {
    switch (sensor->mType) {
    case ATYPE_PLAYER:
        if (mMario->isStatusActive(MarioStatus_Hang)) {
            getRealPos("Spine1", &sensor->mPosition);
            sensor->mRadius = 60.0f;
            return;
        }
        sensor->mPosition.set(_2A0);
        if (mMario->mMovementStates._B && !mMario->mMovementStates._1) {
            sensor->mPosition.add(mMario->mJumpVec);
        }
        sensor->mRadius = 100.0f;
        return;
    case ATYPE_PUSH:
        sensor->setType(ATYPE_EYE);
        return;
    case ATYPE_EYE:
        if (sensor == getSensor("ex-eye")) {
            updateScouter();
            return;
        }
        f32 radius = 600.0f;
        sensor->mPosition.set(_2A0);
        if (mMario->mMovementStates._F) {
            radius = 1000.0f;
        }
        if (_468) {
            radius = 2000.0f;
        }
        if (mMario->isSwimming()) {
            radius = 2000.0f;
        }
        sensor->mRadius = radius;
        _3E5 = false;
        _3E6 = false;
        if (strcmp(mMarioAnim->mXanimePlayer->getCurrentBckName(), "spin2nd") == 0) {
            _3E5 = true;
            _3E6 = true;
            if (_944 <= 2) {
                if (!_944) {
                    _945 = 0;
                    _974 = 0;
                }
                _944 = 2;
            }
        }
        if (!mMario->isAnimationTerminate(nullptr)) {
            if (mMario->_430 == 8 && isJumping()) {
                _3E5 = true;
            }
            if (isAnimationRun("地上ひねり")) {
                _3E5 = true;
            }
            if (isAnimationRun("サマーソルト")) {
                _3E5 = true;
            }
            if (isAnimationRun("水泳スピン")) {
                _3E5 = true;
            }
            if (isAnimationRun("水上スピン")) {
                _3E5 = true;
            }
            if (isAnimationRun("しゃがみスピン")) {
                _3E5 = true;
            }
            if (isAnimationRun("フーファイタースピン")) {
                _3E5 = true;
            }
            if (isAnimationRun("ハチスピン")) {
                _3E5 = true;
            }
            if (isAnimationRun("ハチスピン空中")) {
                _3E5 = true;
            }
            if (isAnimationRun("アイスひねり空中")) {
                _3E5 = true;
            }
            if (isAnimationRun("ファイアスピン空中")) {
                _3E5 = true;
            }
            if (isAnimationRun("ファイアスピン")) {
                _3E5 = true;
            }
            if (isAnimationRun("アイスひねり")) {
                _3E5 = true;
            }
            if (isAnimationRun("アイスひねり移動")) {
                _3E5 = true;
            }
            if (isAnimationRun("アイスひねり静止")) {
                _3E5 = true;
            }
            if (isAnimationRun("ハンマー投げリリース")) {
                _3E5 = true;
            }
            if (mMario->isSwimming() && mMario->mSwim->check7Aand7C()) {
                _3E5 = true;
            }
            if (mMario->isStatusActive(MarioStatus_Foo) && mMario->mFoo->_4C) {
                _3E5 = true;
            }
            if (_3E5) {
                _3E6 = true;
            }
        }
        if (sensor->isValid()) {
            attackOrPushPolygons();
        }
        if (_424) {
            tryTornadoPull(_424);
        }
        return;
    }
}

void MarioActor::doTrampleJump(HitSensor* sensor) {
    if (mMario->_1C._6) {
        return;
    }
    if (mMario->isStatusActive(MarioStatus_Wall)) {
        mMario->closeStatus(mMario->mWall);
    }
    mMario->_402 = getConst().getTable()->mAirWalkTimeTornado;
    mMario->mMovementStates._2B = false;
    switch (sensor->mType) {
    case 0x1F:
        _988 = 0;
        trampleJump(getConst().getTable()->mTrampleBegoma, getConst().getTable()->mTrampleLong);
        changeAnimationNonStop("ヘリコプタージャンプ");
        mMario->startPadVib(2UL);
        playSound("ヘリコプタージャンプ", -1);
        mMario->startRotationTask(4);
        mMario->_430 = 11;
        break;
    default:
        if (strcmp(sensor->mHost->mName, "砲弾") == 0) {
            mMario->playSoundTrampleCombo(_989);
            _989++;
            if (_989 > 1 && _989 < 5) {
                _1B8->setCount(_989);
            }
            if (_989 == 5) {
                _1B8->kill();
                _989 = 0;
                MR::requestOneUp();
                MR::incPlayerLeft();
            }
        }
        if (strcmp(sensor->mHost->mName, "全滅用クリボー") == 0) {
            mMario->playSoundTrampleCombo(_989);
            _989++;
            if (_989 > 1 && _989 < 8) {
                _1B8->setCount(_989);
            }
            if (_989 >= 8) {
                _1B8->kill();
                MR::requestOneUp();
                MR::incPlayerLeft();
            }
        }
        trampleJump(getConst().getTable()->mTrampleNormal, getConst().getTable()->mTrampleLong);
        break;
    }
}

void MarioActor::trampleJump(f32 normal, f32 extra) {
    if (mMario->isStatusActive(MarioStatus_Foo)) {
        return;
    }
    TVec3f velocity(mMario->mJumpVec);
    MR::vecKillElement(velocity, getGravityVec(), &velocity);
    velocity.add(-getGravityVec() * normal);
    switch (_988) {
    case 2:
        if (mMario->checkLvlA()) {
            velocity *= 1.2f;
        } else {
            velocity *= 1.5f;
        }
        break;
    }
    if (mMario->checkLvlA()) {
        velocity.add(-getGravityVec() * extra);
    }
    mMario->tryForceFreeJump(velocity);
    mMario->popTask(&Mario::taskOnHipDropSlide);
    if (mPlayerMode != PlayerMode_Hopper) {
        if (!mMario->mMovementStates._A) {
            _988++;
            if (_988 == 1) {
                changeAnimation("ジャンプふみ1", nullptr);
            } else if (_988 == 2) {
                changeAnimation("ジャンプふみ2", nullptr);
            } else if (_988 >= 3) {
                changeAnimation("ジャンプふみ3", nullptr);
                _988 = 0;
            }
        }
    } else {
        stopAnimation(nullptr);
        switch (mMario->mRabbit->_68) {
        case 0:
            changeAnimation("ホッパーふみジャンプA", nullptr);
            break;
        case 1:
            changeAnimation("ホッパーふみジャンプB", nullptr);
            break;
        }
    }
    playSound("声踏み", -1);
    playEffect("ふみつぶし");
    mMario->startPadVib(0UL);
    mMario->mMovementStates._2F = false;
    mMario->mMovementStates._22 = false;
    mMario->mMovementStates._3E = 0;
}

void MarioActor::attackSensor(HitSensor* own, HitSensor* other) {
    if (!isEnableNerveChange()) {
        return;
    }
    if (own->mType == ATYPE_PLAYER) {
        if (_934 && !getSensor("eye")->isValid()) {
            addRushSensor(other, false);
        }
    } else {
        if (own == getSensor("eye")) {
            if (MR::isDead(other->mHost)) {
                return;
            }
            if (_934) {
                attackOrPushSensorInRush(other, (other->mPosition - own->mPosition).length());
            } else if (isDamaging()) {
                attackOrPushSensorInDamage(other, (other->mPosition - own->mPosition).length());
            } else {
                attackOrPushSensor(other, (other->mPosition - own->mPosition).length());
            }
        }
        if (own == getSensor("ex-eye")) {
            recordScoutingObject(other);
        }
    }
}

bool MarioActor::sendMsgToSensor(HitSensor* pSensor, u32 msg) {
    return pSensor->receiveMessage(msg, getSensor("body"));
}

void MarioActor::resetSensorCount() {
    _930 = 0;
    _7DC = 0;
    _46C = 0;
}

void MarioActor::recordScoutingObject(HitSensor* sensor) {
    if (sensor == _424) {
        return;
    }
    if (!MR::isSensorEnemy(sensor) && !MR::isSensorMapObj(sensor) && !MR::isSensorRide(sensor)) {
        return;
    }
    if (MR::diffAngleAbsHorizontal(sensor->mPosition - getSensor("ex-eye")->mPosition, mMario->mFrontVec, _240) >= 1.5707964f) {
        return;
    }
    _9D4 = sensor;
    _9D8 = getSensor("ex-eye")->mPosition;
    _9CC = _9D0;
    _9D0 = 60.0f;
}

void MarioActor::updateScouter() {
    HitSensor* previous = _F24;
    _F24 = nullptr;
    if (_468) {
        if (_9D4 && MR::isExistInAttributeGroupSearchTurtle(_9D4->mHost)) {
            if (MR::isSensorEnemy(_9D4)) {
                _F28 = 16;
            } else {
                _F28 = 2;
            }
            _F24 = _9D4;
        } else if (_F28) {
            _F24 = previous;
            _F28--;
        }
    }
    _9D0 += 80.0f;
    if (_9D0 < 100.0f) {
        _9D0 = 100.0f;
    }
    if (_9D0 > _9CC) {
        _9D4 = nullptr;
    }
    f32 maximum = 1000.0f;
    if (mMario->isStatusActive(MarioStatus_Foo)) {
        maximum = 2400.0f;
    }
    if (_468) {
        maximum = 3000.0f;
    }
    if (_9D0 > maximum) {
        _9D0 = 60.0f;
    }
    TVec3f position;
    if (mMario->isSwimming()) {
        position = _2A0 + mUpVec * _9D0;
    } else {
        position = _2A0 + mMario->mFrontVec * _9D0;
    }
    getSensor("ex-eye")->mPosition = position;
    f32 radius = 100.0f;
    if (_9D0 >= 300.0f) {
        radius = 100.0f + (_9D0 - 300.0f) * MR::tanDegree(5.0f);
    }
    if (_9D0 < 200.0f) {
        radius = 40.0f;
    }
    getSensor("ex-eye")->mRadius = radius;
}

void MarioActor::initScouter() {
    _9CC = 0.0f;
    _9D0 = 60.0f;
    _9D4 = 0;
    _9D8.set2(0.0f);

    HitSensor* sensor = getSensor("ex-eye");
    sensor->mRadius = 100.0f;
    getSensor("ex-eye")->validate();
}

void MarioActor::initForJump() {
    _988 = 0;
    _989 = 0;
}
