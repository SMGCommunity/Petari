#include "Game/LiveActor/HitSensor.hpp"
#include "Game/Player/MarioActor.hpp"
#include "Game/Player/MarioAnimator.hpp"
#include "Game/Player/MarioState.hpp"
#include "Game/Player/RushEndInfo.hpp"
#include "Game/Screen/GameSceneLayoutHolder.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "revolution/types.h"

void MarioActor::addRushSensor(HitSensor* pSensor, bool myBool) {
    if (!MR::isSensorAutoRush(pSensor) && !MR::isSensorRush(pSensor)) {
        return;
    }

    _7E4[_930] = pSensor;
    _8E4[_930] = myBool;
    _930++;
}

bool MarioActor::tryStandardRush() {
    if (mMario->mMovementStates.debugMode) {
        return false;
    }
    bool automatic = true;
    if (isRequestRush()) {
        automatic = false;
    } else {
        HitSensor* target = getNearestRushTarget(false);
        if (target != nullptr) {
            if (_468 != 0 && selectHandyRush(target)) {
                return false;
            }
            if (target->receiveMessage(ACTMES_IS_RUSH_REQUEST, getSensor("body"))) {
                automatic = false;
            }
        }
    }
    if (tryStartRush(automatic)) {
        resetSensorCount();
        mVelocity.zero();
        beginRush();
        return true;
    }
    return false;
}

void MarioActor::checkPriorRushTarget() {
    s32 maxVal = 0;
    s32 val = 0;
    for (s32 idx = 0; idx < _930; idx++) {
        if (isFixJumpRushSensor(_7E4[idx])) {
            continue;
        }

        u32 v = 0;
        switch (_7E4[idx]->mType) {
        case ATYPE_COCO_NUT_BIND:
            v = 10;
            break;
        }

        if (maxVal >= v) {
            continue;
        }

        maxVal = v;
        val = idx;
    }

    if (maxVal != 0) {
        _7E4[0] = _7E4[val];
        _930 = 1;
    }
}

HitSensor* MarioActor::getNearestRushTarget(bool myBool) const {
    if (!_930) {
        return nullptr;
    }

    f32 maxRadius = 10000.0f;
    HitSensor* out = nullptr;

    if (!isEnableNerveChange()) {
        return nullptr;
    }

    if (!mMario->isEnableRush()) {
        HitSensor* target = getHighPriorityTarget();
        if (target == nullptr) {
            target = getRescueTarget();
        }

        if (target != nullptr) {
            TVec3f targetPos(target->mPosition);
            f32 size2;
            f32 size1 = (targetPos - mPosition).length();
            size2 = (targetPos - _2A0).length();
            f32 size3 = (targetPos - _2AC).length();

            if (size1 > size2) {
                size1 = size2;
            }

            if (size1 > size3) {
                size1 = size3;
            }

            if (myBool && target->mRadius < size1) {
                return nullptr;
            }

            return target;
        }

        if (!mMario->isHanging() && !mMario->isDamaging()) {
            for (s32 idx = 0; idx < _930; idx++) {
                if (selectHipDropRush(_7E4[idx])) {
                    return _7E4[idx];
                }
            }
        }

        return nullptr;
    }
    if (mHealth == 0) {
        return nullptr;
    }

    for (s32 idx = 0; idx < _930; idx++) {
        if (isFixJumpRushSensor(_7E4[idx])) {
            continue;
        }

        if (myBool && !MR::isSensorAutoRush(_7E4[idx])) {
            continue;
        }

        TVec3f pos(_7E4[idx]->mPosition);
        f32 size2;
        f32 size1 = (pos - mPosition).length();
        size2 = (pos - _2A0).length();
        f32 size3 = (pos - _2AC).length();

        if (size1 > size2) {
            size1 = size2;
        }

        if (size1 > size3) {
            size1 = size3;
        }

        if ((_8E4[idx] || !myBool || !(_7E4[idx]->mRadius + 20.0f < size1)) && size1 < maxRadius) {
            out = _7E4[idx];
            maxRadius = size1;
        }
    }

    return out;
}

HitSensor* MarioActor::getNearestJumpTarget() const {
    if (_930 == 0) {
        return nullptr;
    }
    if (mHealth == 0) {
        return nullptr;
    }
    f32 nearestDistance = 100000.0f;
    HitSensor* nearest = nullptr;
    for (u32 i = 0; i < _930; i++) {
        if (!isFixJumpRushSensor(_7E4[i])) {
            continue;
        }
        if (_924 == _7E4[i]) {
            continue;
        }
        TVec3f position(_7E4[i]->mPosition);
        f32 centerDistance;
        f32 distance = (position - mPosition).length();
        centerDistance = (position - _2A0).length();
        f32 headDistance = (position - _2AC).length();
        if (distance > centerDistance) {
            distance = centerDistance;
        }
        if (distance > headDistance) {
            distance = headDistance;
        }
        if ((_8E4[i] || !(_7E4[i]->mRadius < distance)) && distance < nearestDistance) {
            nearest = _7E4[i];
            nearestDistance = distance;
        }
    }
    return nearest;
}

HitSensor* MarioActor::getHighPriorityTarget() const {
    if (_930 == 0) {
        return nullptr;
    }

    for (s32 idx = 0; idx < _930; idx++) {
        if (_7E4[idx]->isType(ATYPE_POWER_STAR_BIND)) {
            return _7E4[idx];
        }

        if (mHealth == 0) {
            continue;
        }

        if (_7E4[idx]->isType(ATYPE_QUESTION_COIN_BIND)) {
            return _7E4[idx];
        }

        if (_7E4[idx]->isType(ATYPE_PRIOR_BINDER)) {
            return _7E4[idx];
        }

        if (_7E4[idx]->isType(ATYPE_SCENARIO_STARTER_BIND)) {
            return _7E4[idx];
        }
    }

    return nullptr;
}

HitSensor* MarioActor::getRescueTarget() const {
    if (_930 == 0) {
        return nullptr;
    }

    if (mHealth == 0) {
        return nullptr;
    }

    for (s32 idx = 0; idx < _930; idx++) {
        if (_7E4[idx]->isType(ATYPE_G_CAPTURE_BIND)) {
            return _7E4[idx];
        }

        if (selectOnDamageRush(_7E4[idx]->mHost->mName)) {
            return _7E4[idx];
        }
    }

    return nullptr;
}

bool MarioActor::tryStartRush(bool automatic) {
    if (mMario->isStatusActive(MarioStatus_Bury)) {
        return false;
    }
    if (mMario->isStatusActive(MarioStatus_Sukekiyo)) {
        return false;
    }
    HitSensor* target = getNearestRushTarget(automatic);
    if (getMovementStates()._F && mMario->isRising() && target == _928) {
        return false;
    }
    if (target != nullptr) {
        for (;;) {
            u8 autoBind;
            if (automatic && selectAutoBind(target->mHost->mName, &autoBind)) {
                if (autoBind == 0) {
                    return false;
                }
                if (autoBind == 2) {
                    setPlayerMode(PlayerMode_Normal, true);
                }
            }
            if (target == _928 && selectRebindTimer(target) && _37C - _92C < 120) {
                return false;
            }
            _7E4[0] = target;
            bool started;
            if (automatic) {
                started = target->receiveMessage(ACTMES_AUTORUSH_BEGIN, getSensor("body"));
            } else {
                started = target->receiveMessage(ACTMES_RUSH_BEGIN, getSensor("body"));
            }
            if (!started) {
                HitSensor* priority = getHighPriorityTarget();
                if (priority != nullptr && priority != target) {
                    target = priority;
                    continue;
                }
            }
            return started;
        }
    }
    return false;
}

bool MarioActor::tryJumpRush() {
    if (mHealth == 0) {
        return false;
    }

    if (mMario->getMovementStates().debugMode) {
        return false;
    }

    if (mMarioAnim->isLandingAnimationRun()) {
        return false;
    }

    if (mMario->isStatusActive(MarioStatus_13)) {
        return false;
    }

    if (mMario->isStatusActive(MarioStatus_Bury)) {
        return false;
    }

    HitSensor* jumpTarget = getNearestJumpTarget();
    if (jumpTarget != nullptr) {
        unsigned char autoBind;
        bool b = selectAutoBind(jumpTarget->mHost->mName, &autoBind);
        if (!b && !isActionOk("ジャンプ系バインド")) {
            return false;
        }

        if (_468 != 0 && selectHandyRush(jumpTarget)) {
            return false;
        }

        if (b && autoBind == 0) {
            return false;
        }

        _7E4[0] = jumpTarget;

        bool out = jumpTarget->receiveMessage(ACTMES_AUTORUSH_BEGIN, getSensor("body"));

        if (out) {
            resetSensorCount();
            mVelocity.zero();
            beginRush();

            if (b && autoBind == 2) {
                setPlayerMode(0, true);
            }
        }

        return out;
    }

    return false;
}

void MarioActor::tryRushInRush() {
    HitSensor* target = getHighPriorityTarget();
    if (target == nullptr) {
        target = getNearestJumpTarget();
    }

    _930 = 0;

    if (target == _924) {
        return;
    }

    if (target == nullptr) {
        return;
    }

    if (!target->receiveMessage(ACTMES_IS_RUSH_TAKEOVER, getSensor("body"))) {
        return;
    }

    HitSensor* sensor924 = _924;
    if (!_924->receiveMessage(ACTMES_RUSH_CANCEL, getSensor("body"))) {
        return;
    }

    if (!target->receiveMessage(ACTMES_AUTORUSH_BEGIN, getSensor("body"))) {
        if (_934) {
            const RushEndInfo rushEndInfo = RushEndInfo(nullptr, 4, TVec3f(0.0f, 0.0f, 0.0f), false, 0);
            endRush(&rushEndInfo);
        }

        return;
    }

    if (isFixJumpRushSensor(target) && isLandEffectRushSensor(sensor924)) {
        mMario->mMovementStates._3E = false;

        playEffect("スーパースピンドライバ終了");
    }

    if (_934) {
        _924 = target;

        if (mPlayerMode == 4 && selectHideFlyMeter(target)) {
            MR::getGameSceneLayoutHolder()->changeLifeMeterModeGround();
        }
    } else {
        _7E4[0] = target;

        beginRush();
    }
}
