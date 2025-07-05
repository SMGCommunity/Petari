#include "Game/Player/MarioStick.hpp"
#include "Game/Player/Mario.hpp"
#include "Game/Player/MarioActor.hpp"
#include "Game/Player/MarioConst.hpp"
#include "Game/Map/HitInfo.hpp"

MarioStick::MarioStick(MarioActor *pActor) : MarioState(pActor, 0x16) {
    _4C = 0;
    _50 = false;
    _54.zero();
    _48 = new Triangle();
    _14.zero();
    _20.zero();
    _2C.zero();
    _38.zero();
    _51 = false;
    _44 = 0.0f;
    _60.zero();
    _6C.zero();
    _78.zero();
}

bool MarioStick::postureCtrl(MtxPtr mtx) {
    MR::makeMtxFrontUp((TPos3f*)mtx, _6C, _78);
    return true;
}

/*
bool MarioStick::setStickSensor(const HitSensor *pSensor) {
    TVec3f stack_2c;
    if (getPlayer()->_3BC < 0x78) {
        return false;
    }
    if (getPlayer()->isStatusActive(0x16)) {
        return false;
    }
    if (pSensor->isType(0x53)) {
        MR::getRotatedAxisY(&stack_2c, pSensor->mActor->mRotation);
        TVec3f stack_20 = mActor->_2A0 - pSensor->mPosition;
        MR::normalizeOrZero(&stack_20);
        if (stack_2c.dot(stack_20) < mActor->mConst->mTable[mActor->mConst->mCurrentTable]->mBeeStickAngleLimit) {
            return false;
        }
        if (mActor->getLastMove().dot(stack_20) >= 0.0f) {
            return false;
        }
        stack_20.setLength(pSensor->mRadius);
        //TVec3f stack_8(stack_20);
        //stack_8.scale(0.9f);
        //TVec3f stack_14(pSensor->mPosition);
        //stack_14 += stack_20.scaleInline(0.9f);
        _14 = pSensor->mPosition.translate(stack_20.scaleInline(0.9f));
        getPlayer()->setTrans(_14, "Stick");
        clearVelocity();
        MR::normalize(&stack_20);
        _20 = stack_20;
        _2C = stack_2c;
        _38 = pSensor->mPosition;
        _44 = pSensor->mRadius;
        return true;
    }
    return false;
} */

bool MarioStick::start() {
    _4C = 0;
    changeAnimation("ハチ壁くっつき", "ハチ壁くっつき中");
    startPadVib((u32)0);
    playEffectTrans("ハチ花くっつき", _38);
    playSound("ハチ壁くっつき", -1);
    mActor->_F44 = false;
    TVec3f stack_14 = -_20;
    getPlayer()->setFrontVecKeepSide(stack_14);
    TVec3f stack_8 = -_20;
    _6C = stack_8;
    _78 = getPlayer()->mHeadVec;
    getPlayer()->mMovementStates._0 = 0;
    getPlayer()->mMovementStates._1 = 0;
    return true;
}

/*
bool MarioStick::update() {
    Mtx stack_68;
    Mtx stack_38;
    getPlayer()->setTrans(_14, nullptr);
    if (checkTrgA() && !isAnimationRun("ハチ壁くっつき") && startJump()) {
        return false;
    }
    PSMTXRotAxisRad(stack_38, &getPlayer()->mSideVec, getStickY() * 0.01f);
    TVec3f stack_2c = _14 - _38;
    PSMTXMultVec(stack_38, &stack_2c, &stack_2c);
    if (MR::diffAngleAbs(stack_2c, _2C) < 1.308997f) {
        TVec3f stack_20 = -stack_2c;
        if (!MR::normalizeOrZero(&stack_20)) {
            _6C = stack_20;
        }
        getPlayer()->setFrontVecKeepSide(stack_20);
    }
    PSMTXRotAxisRad(stack_68, getPlayer()->getGravityVec(), -getStickX() * 0.01f);
    PSMTXMultVec(stack_68, &stack_2c, &stack_2c);
    if (getStickP() > 0.0f) {
        changeAnimation("ハチ花移動", (char*)nullptr);
    }
    else {
        stopAnimation("ハチ花移動", (char*)nullptr);
    }
    if (MR::diffAngleAbs(stack_2c, _2C) < 1.308997f) {
        TVec3f stack_14 = -stack_2c;
        if (!MR::normalizeOrZero(&stack_14)) {
            _6C = stack_14;
        }
        getPlayer()->setFrontVecKeepUp(stack_14);
        stack_2c.setLength(_44);
        TVec3f stack_8(stack_2c);
        stack_8 += _38;
        _14 = stack_8;
    }
    _6C = getFrontVec();
    _78 = getPlayer()->mHeadVec;
    getPlayer()->incAirWalkTimer();
    getPlayer()->incAirWalkTimer();
    getPlayer()->incAirWalkTimer();
    getPlayer()->incAirWalkTimer();
    return true;
}
*/

bool MarioStick::close() {
    stopAnimation("ハチ壁くっつき", "落下");
    if (getPlayer()->mMovementStates._1) {
        changeAnimation(nullptr, "基本");
    }
    getPlayer()->mMovementStates._38 = 0;
    mActor->_F44 = true;
    playEffectTrans("ハチ花くっつき", _38);
    return true;
}

bool MarioStick::notice() {
    return false;
}

bool MarioStick::startJump() {
    TVec3f jumpDir(_2C);
    jumpDir.scale(mActor->mConst->mTable[mActor->mConst->mCurrentTable]->mBeeStickJumpPower);
    TVec3f jumpBonus = getGravityVec().scaleInline(-mActor->mConst->mTable[mActor->mConst->mCurrentTable]->mBeeStickJumpBonus);
    jumpDir += jumpBonus;
    getPlayer()->tryStickJump(jumpDir);
    getPlayer()->_408 = 0;
    return true;
}

// This function is matched. Yes, it does nothing.
bool Mario::checkBeeStick() {

}

void Mario::tryBeeStick(const HitSensor *pSensor) {
    if (mStick->setStickSensor(pSensor)) {
        changeStatus(mStick);
    }
}
