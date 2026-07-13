#include "Game/LiveActor/HitSensor.hpp"
#include "Game/Player/MarioActor.hpp"
#include "Game/Player/MarioFlip.hpp"
#include "Game/Player/MarioState.hpp"
#include "Game/Player/MarioStun.hpp"
#include "Game/Player/MarioSwim.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

bool MarioActor::isEnableSpinPunch() {
    if (mMario->getMovementStates()._23) {
        return false;
    }

    if (mMario->getCurrentStatus() == MarioStatus_Slider) {
        return false;
    }

    if (mMario->getCurrentStatus() == MarioStatus_Swim) {
        return false;
    }

    if (mMario->getCurrentStatus() == MarioStatus_Flip) {
        return mMario->mFlip->_14;  // FIXME
    }

    if (mMario->getCurrentStatus() == MarioStatus_Stun) {
        bool out = false;
        if (mMario->mStun->_12 || mMario->mStun->_14 < 15) {
            out = true;
        }

        return out;
    }

    if (mMario->getMovementStates()._A && mMario->calcDistToCeil(false) < 160.0f) {
        return false;
    }

    return true;
}

void MarioActor::reactionPunch(HitSensor* pSensor) {
    if (mPlayerMode == 1) {
        return;
    }

    if (isJumping()) {
        _38C = 5;
    } else {
        _38C = 5;
        if (mMario->_278 > 0.1f) {
            mMario->_278 = 0.1f;
            mMario->_71C = true;
        }
    }

    if (!mMario->getMovementStates()._2B) {
        jumpHop();
    }

    if (!MR::sendArbitraryMsg(ACTMES_HITMARK_EMIT, pSensor, getSensor("body"))) {
        printHitMark(pSensor);
    }

    mMario->startPadVib(2);

    if (pSensor->isType(ATYPE_BEGOMAN)) {
        mMario->stopWalk();

        TVec3f reactDir(_2A0 - pSensor->mPosition);
        MR::vecKillElement(reactDir, getGravityVector(), &reactDir);
        reactDir.setLength(20.0f);
        mMario->addReaction(reactDir);
    }

    if (mMario->isSwimming()) {
        TVec3f punchDir(_2A0 - pSensor->mPosition);
        punchDir.setLength(5.0f);
        mMario->mSwim->hitPunch(punchDir);
    }
}

bool MarioActor::sendPunch(HitSensor* pSensor, bool myBool) {
    if (isDamaging()) {
        _944 = false;
        return false;
    }

    if (myBool && mPlayerMode == 3) {
        bool out = doFreezeAttack(pSensor);

        if (out && !selectCustomEffectSpinHitSound(pSensor->mHost->mName)) {
            MR::startSpinHitSound(this);
        }

        return out;
    }

    _FCC = true;
    bool out = MR::sendMsgPlayerPunch(pSensor, getSensor("body"));
    _FCC = false;

    if (myBool && out && !selectCustomEffectSpinHitSound(pSensor->mHost->mName)) {
        MR::startSpinHitSound(this);
    }

    return out;
}

bool MarioActor::doFreezeAttack(HitSensor* pSensor) {
    return MR::sendArbitraryMsg(ACTMES_FREEZE_ATTACK, pSensor, getSensor("body"));
}

bool MarioActor::trySpinPunch() {
    bool out = true;
    if (mMario->getMovementStates().jumping) {
        changeAnimation("空パンチ", static_cast< const char* >(nullptr));
        setPunchHitTimer(15);
        mMario->_278 = 0.0f;
    } else if (isEnableSpinPunch()) {
        if (mMario->isStatusActive(MarioStatus_Bury)) {
            playSound("スケキヨ終了スピン", -1);
        }

        if (mMario->getMovementStates()._A) {
            if (mMario->calcDistToCeil(false) > 160.0f) {
                changeAnimation("サマーソルト", static_cast< const char* >(nullptr));
                playSound("スピンジャンプ", -1);
                playSound("声スピン", -1);
            } else {
                return false;
            }
        }

        mMario->startMagic();

        setPunchHitTimer(25);
    } else {
        out = false;
    }

    tryReleaseBombTeresa();
    _950 = false;

    return out;
}

void MarioActor::printHitMark(HitSensor* pSensor) {
    TVec3f vec(pSensor->mPosition - _2A0);
    vec.setLength(200.0f);
    playEffectTrans("パンチヒット", _2A0 + vec);
}

bool MarioActor::tryPunchAirAfter(HitSensor* pSensor) {
    if (pSensor->isType(ACTMES_FORCE_KILL)) {
        return false;
    }

    for (u32 idx = 0; idx < _974; idx++) {
        if (_954[idx] == pSensor) {
            return false;
        }
    }

    if (_974 >= 8) {
        return false;
    }

    if (!_3E5 && mPlayerMode != 1) {
        TVec3f vec(pSensor->mPosition - getSensor("body")->mPosition);
        MR::vecKillElement(vec, getGravityVector(), &vec);

        if (MR::diffAngleAbs(vec, mMario->mFrontVec) >= PI_4) {
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

bool MarioActor::sendBodyAttack(HitSensor* pSensor) {
    bool isStatus = true;
    if (mMario->getCurrentStatus() != MarioStatus_None && (s32)mMario->getCurrentStatus() != (s32)MarioStatus_Skate) {
        isStatus = false;
    }

    if (isStatus && !mMario->getMovementStates()._8 && !isDamaging() && !mMario->getMovementStates()._B) {
        TVec3f newPos(pSensor->mPosition - getSensor("body")->mPosition);
        TVec3f* frontVec = &mMario->mFrontVec;
        if (MR::diffAngleAbsHorizontal(newPos, *frontVec, getGravityVector()) >= HALF_PI) {
            return false;
        }

        if (MR::sendArbitraryMsg(ACTMES_KICK, pSensor, getSensor("body"))) {
            playSound("声ランニングキック", -1);
            changeAnimation("ランニングキック", static_cast< const char* >(nullptr));
            mMario->startPadVib(2);

            if (!MR::sendArbitraryMsg(ACTMES_HITMARK_EMIT, pSensor, getSensor("body"))) {
                printHitMark(pSensor);
            }

            return true;
        }
    }

    if (mPlayerMode == 1 && MR::sendArbitraryMsg(ACTMES_INVINCIBLE_ATTACK, pSensor, getSensor("body"))) {
        mMario->startPadVib(2);
        printHitMark(pSensor);

        return true;
    }

    return false;
}

void MarioActor::doFreezeAttack() {
    if (isAnimationRun("アイスひねり空中")) {
        return;
    }

    if (isJumping()) {
        if (mMario->_430 != 13) {
            changeAnimation("アイスひねり空中", static_cast< const char* >(nullptr));
            playSound("スピンジャンプ", -1);
            playSound("声スピン", -1);

            if (mMario->_42C == 0) {
                jumpHop();
                mMario->_42C++;
            }
        }

        return;
    }

    if (mMario->isSkatableFloor()) {
        mMario->doSkate();
        return;
    }

    if (!isAnimationRun("アイスひねり")) {
        playSound("スケートスピン", -1);
        playSound("スピンジャンプ", -1);
        playSound("声パンチ", -1);
    }

    changeAnimation("アイスひねり", static_cast< const char* >(nullptr));
}
