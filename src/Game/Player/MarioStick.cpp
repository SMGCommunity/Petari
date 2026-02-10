#include "Game/Player/MarioStick.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/Map/HitInfo.hpp"
#include "Game/Player/Mario.hpp"
#include "Game/Player/MarioActor.hpp"
#include "Game/Player/MarioConst.hpp"

void Mario::checkBeeStick() {
    return;
}

void Mario::tryBeeStick(const HitSensor* pSensor) {
    bool res = mStick->setStickSensor(pSensor);
    if (res) {
        changeStatus(mStick);
    }
}

MarioStick::MarioStick(MarioActor* pActor) : MarioState(pActor, 0x16) {
    _4C = 0;
    _50 = 0;
    _54.zero();
    _48 = new Triangle();
    _14.zero();
    _20.zero();
    _2C.zero();
    _38.zero();
    _51 = 0;
    _44 = 0.0f;
    _60.zero();
    _6C.zero();
    _78.zero();
}

bool MarioStick::startJump() {
    TVec3f v7(_2C);
    v7.scale(mActor->getConst().getTable()->mBeeStickJumpPower);
    f32 v2 = -mActor->getConst().getTable()->mBeeStickJumpBonus;
    TVec3f v6(getGravityVec());
    v6.scale(v2);
    v7 += v6;
    getPlayer()->tryStickJump(v7);
    getPlayer()->_408 = 0;
    return true;
}

bool MarioStick::postureCtrl(MtxPtr mtx) {
    MR::makeMtxFrontUp((TPos3f*)mtx, _6C, _78);
    return true;
}

bool MarioStick::setStickSensor(const HitSensor* pSensor) {
    if (getPlayer()->_3BC < 0x78) {
        return false;
    }

    if (getPlayer()->isStatusActive(22)) {
        return false;
    }

    if (!pSensor->isType(83)) {
        return false;
    }

    TVec3f v12;
    MR::getRotatedAxisY(&v12, pSensor->mHost->mRotation);
    TVec3f v11;
    v11 = -mActor->_2A0;
    MR::normalizeOrZero(&v11);

    if (v12.dot(v11) < mActor->getConst().getTable()->mBeeStickAngleLimit) {
        return false;
    }

    if (mActor->getLastMove().dot(v11) >= 0.0f) {
        return false;
    }

    v11.setLength(pSensor->mRadius);
    _14 = pSensor->mPosition + (v11 * 0.89999998f);
    getPlayer()->setTrans(_14, "Stick");
    clearVelocity();
    MR::normalize(&v11);
    _20 = v11;
    _2C = v12;
    _38 = pSensor->mPosition;
    _44 = pSensor->mRadius;
    return true;
}

bool MarioStick::start() {
    _4C = 0;
    changeAnimation("ハチ壁くっつき", "ハチ壁くっつき中");
    startPadVib(0ul);
    playEffectTrans("ハチ花くっつき", _38);
    playSound("ハチ壁くっつき", -1);
    mActor->_F44 = 0;
    getPlayer()->setFrontVecKeepSide(-_20);
    _6C = -_20;
    _78 = getPlayer()->mHeadVec;
    getPlayer()->mMovementStates.jumping = 0;
    getPlayer()->mMovementStates._1 = 0;
    return true;
}

// MarioStick::update

bool MarioStick::close() {
    stopAnimation("ハチ壁くっつき", "落下");

    if (getPlayer()->mMovementStates._1) {
        changeAnimation(nullptr, "基本");
    }

    getPlayer()->mMovementStates._38 = 0;
    mActor->_F44 = 1;
    playEffectTrans("ハチ花くっつき", _38);
}

bool MarioStick::notice() {
    return false;
}
