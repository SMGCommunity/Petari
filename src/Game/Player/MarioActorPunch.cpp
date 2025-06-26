#include "Game/Player/MarioActor.hpp"
#include "Game/Player/MarioFlip.hpp"
#include "Game/Player/MarioStun.hpp"
#include "Game/Player/MarioSwim.hpp"

// mMario needs to be manually loaded in a few places
bool MarioActor::trySpinPunch() {
    bool ret = true;
    if (mMario->mMovementStates._0) {
        changeAnimation("空パンチ", nullptr);
        setPunchHitTimer(0xf);
        mMario->_278 = 0.0f;
    }
    else if (isEnableSpinPunch()) {
        if (mMario->isStatusActive(0x1b)) {
            playSound("スケキヨ終了スピン", -1);
        }
        if (mMario->mMovementStates._A) {
            if (mMario->calcDistToCeil(false) > 160.0f) {
                changeAnimation("サマーソルト", nullptr);
                playSound("スピンジャンプ", -1);
                playSound("声スピン", -1);
            }
            else {
                return false;
            }
        }
        else {
            mMario->startMagic();
        }
        setPunchHitTimer(0x19);
    }
    else {
        ret = false;
    }
    tryReleaseBombTeresa();
    _950 = 0;
    return ret;
}

bool MarioActor::isEnableSpinPunch() {
    bool ret;
    if (mMario->mMovementStates._23) {
        ret = false;
    }
    else if (mMario->getCurrentStatus() == 7) { // mMario needs to be manually loaded here
        ret = false;
    }
    else if (mMario->getCurrentStatus() == 6) {
        ret = false;
    }
    else if (mMario->getCurrentStatus() == 0x14) {
        ret = !(mMario->mFlip->_14 < 2);
    }
    else if (mMario->getCurrentStatus() == 0xe) {
        ret = false;
        if (mMario->mStun->_12 != 0 || mMario->mStun->_14 < 0xf) {
            ret = true;
        }
    }
    else if (mMario->mMovementStates._A && mMario->calcDistToCeil(false) < 160.0f) { // and here as well
        ret = false;
    }
    else {
        ret = true;
    }
    return ret;
}

// regswap
void MarioActor::reactionPunch(HitSensor *pSensor) {
    if (mPlayerMode != 1) {
        if (isJumping()) {
            _38C = 5;
        }
        else {
            _38C = 5;
            if (mMario->_278 > 0.1f) {
                mMario->_278 = 0.1f;
                mMario->_71C = 1;
            }
        }
        if (!mMario->mMovementStates._2B) {
            jumpHop();
        }
        if (!MR::sendArbitraryMsg(0x1c, pSensor, getSensor("body"))) {
            printHitMark(pSensor);
        }
        mMario->startPadVib(2);
        if (pSensor->isType(0x1d)) {
            mMario->stopWalk();
            TVec3f stack_14 = _2A0 - pSensor->mPosition;
            TVec3f* pGravity = &getGravityVector();
            MR::vecKillElement(stack_14, *pGravity, pGravity);
            stack_14.setLength(20.0f);
            mMario->addReaction(stack_14);
        }
        if (mMario->isSwimming()) {
            TVec3f stack_8 = _2A0 - pSensor->mPosition;
            stack_8.setLength(5.0f);
            mMario->mSwim->hitPunch(stack_8);
        }
    }
}

bool MarioActor::sendBodyAttack(HitSensor *pSensor) {
    bool b1 = true;
    if (mMario->getCurrentStatus() != 0 && (s32)mMario->getCurrentStatus() != 0x1f) {
        b1 = false;
    }
    if (b1 && !mMario->mMovementStates._8 && !isDamaging() && !mMario->mMovementStates._B) {
        TVec3f stack_8 = pSensor->mPosition - getSensor("body")->mPosition;
        TVec3f* pFront = &mMario->mFrontVec;
        if (MR::diffAngleAbsHorizontal(stack_8, *pFront, getGravityVector()) >= HALF_PI) {
            return false;
        }
        if (MR::sendArbitraryMsg(0x2b, pSensor, getSensor("body"))) {
            playSound("声ランニングキック", -1);
            changeAnimation("ランニングキック", nullptr);
            mMario->startPadVib(2);
            if (!MR::sendArbitraryMsg(0x1c, pSensor, getSensor("body"))) {
                printHitMark(pSensor);
            }
            return true;
        }
    }
    if (mPlayerMode == 1 && MR::sendArbitraryMsg(0xb, pSensor, getSensor("body"))) {
        mMario->startPadVib(2);
        printHitMark(pSensor);
        return true;
    }
    return false;
}

bool MarioActor::doFreezeAttack(HitSensor *pReceiver) {
    return MR::sendArbitraryMsg(0xa, pReceiver, getSensor("body"));
}

void MarioActor::doFreezeAttack() {
    if (!isAnimationRun("アイスひねり空中")) {
        if (isJumping()) {
            if (mMario->_430 != 0xd) {
                changeAnimation("アイスひねり空中", nullptr);
                playSound("スピンジャンプ", -1);
                playSound("声スピン", -1);
                if (mMario->_42C == 0) {
                    jumpHop();
                    mMario->_42C++;
                }
            }
        }
        else if (mMario->isSkatableFloor()) {
            mMario->doSkate();
        }
        else {
            if (!isAnimationRun("アイスひねり")) {
                playSound("スケートスピン", -1);
                playSound("スピンジャンプ", -1);
                playSound("声パンチ", -1);
            }
            changeAnimation("アイスひねり", nullptr);
        }
    }
}

bool MarioActor::sendPunch(HitSensor *pSensor, bool a2) {
    if (isDamaging()) {
        _944 = 0;
        return false;
    }
    if (a2 && mPlayerMode == 3) {
        bool ret = doFreezeAttack(pSensor);
        if (ret) {
            if (!selectCustomEffectSpinHitSound(pSensor->mActor->mName)) {
                MR::startSpinHitSound(this);
            }
        }
        return ret;
    }
    _FCC = true;
    bool ret2 = MR::sendMsgPlayerPunch(pSensor, getSensor("body"));
    _FCC = false;
    if (a2 && ret2) {
        if (!selectCustomEffectSpinHitSound(pSensor->mActor->mName)) {
            MR::startSpinHitSound(this);
        }
    }
    return ret2;
}

/* NON-MATCHING
bool MarioActor::tryPunchAirAfter(HitSensor *pSensor) {
    if (pSensor->isType(0x71)) {
        return false;
    }
    u32 i = 0;
    u32 j;
    for (j = _974; j > 0; j--) {
        if (_954[i] == pSensor) {
            return false;
        }
        i++;
    }
    if (!(j < 8)) {
        return false;
    }
    if (!_3E5 && mPlayerMode != 1) {
        TVec3f stack_8 = pSensor->mPosition - getSensor("body")->mPosition; // needs to be un-inlined
        TVec3f* pGravity = &getGravityVector();
        MR::vecKillElement(stack_8, *pGravity, pGravity);
        if (MR::diffAngleAbs(stack_8, mMario->mFrontVec) >= 0.7853982f) {
            return false;
        }
    }
    if (sendPunch(pSensor, true)) {
        _954[_974] = pSensor;
        _924 = pSensor;
        _974++;
        reactionPunch(pSensor);
        return true;
    }
    return false;
}
*/
