#include "Game/Player/MarioRecovery.hpp"
#include "Game/Player/Mario.hpp"
#include "Game/Player/MarioActor.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include <revolution/mtx.h>

bool Mario::doRecovery() {
    if (isStatusActive(MarioStatus_Foo)) {
        return false;
    }
    if (isStatusActive(MarioStatus_Recovery)) {
        return false;
    }
    if (isStatusActive(MarioStatus_Warp)) {
        return false;
    }
    if (mActor->_934) {
        return false;
    }
    if (mActor->_EA4) {
        return false;
    }

    cancelSquatMode();
    stopWalk();
    if (mRecovery->_12) {
        changeStatus(mRecovery);
        stopJump();
    } else {
        TVec3f gravity;
        TVec3f position(*getLastSafetyTrans(&gravity));
        position += gravity * 160.0f;
        TVec3f direction(mPosition - position);
        MR::normalizeOrZero(&direction);
        doPointWarpRecovery(position, direction);
    }
    return true;
}

MarioRecovery::MarioRecovery(MarioActor* actor) : MarioState(actor, MarioStatus_Recovery) {
    _16 = 0;
    _1A = 0xFFFF;
    _58.zero();
    _12 = 0;
    _11 = 0;
    _14 = 0;
    _18 = 0;
    _1C.zero();
    _28.zero();
    _34.zero();
    _40.zero();
    _4C.zero();
    _64.zero();
    _70.zero();
    _88 = 0;
    _7C = 0.0f;
    _80 = 0.0f;
    _84 = 0.0f;
}

bool MarioRecovery::calcFirstVector() {
    _40 = getTrans();
    MarioActor* actor = mActor;
    f32 vertical = MR::vecKillElement(actor->getLastMove(), getGravityVec(), &_1C);
    _28 = getGravityVec() * vertical;
    TVec3f offset(getTrans() - _34);
    offset.setLength(10.0f);
    _1C += offset;
    _14 = 60;
    _11 = 0;
    _70 = getTrans() - _34;
    f32 distance = _70.length();
    _84 = _70.length();
    if (MR::normalizeOrZero(&_70)) {
        return false;
    }

    TVec3f horizontal;
    MR::vecKillElement(_70, getGravityVec(), &horizontal);
    if (_12) {
        TVec3f alternate;
        MR::vecKillElement(_4C - _34, getGravityVec(), &alternate);
        horizontal = alternate;
    }
    if (MR::normalizeOrZero(&horizontal)) {
        return false;
    }
    PSVECCrossProduct(&getGravityVec(), &horizontal, &_64);
    if (MR::normalizeOrZero(&_64)) {
        return false;
    }

    if (_12) {
        f32 angle = MR::diffAngleAbs(horizontal, -_70);
        angle += 3.1415927f;
        _7C = angle;
    } else {
        _7C = 6.2831855f - MR::diffAngleAbs(horizontal, _70);
    }
    _18 = 200;
    f32 extra = (distance - 500.0f) / 20.0f;
    if (extra > 0.0f) {
        _18 = static_cast< s32 >(extra) + 200;
    }
    _88 = _18;
    _80 = _7C / _18;
    _7C = 0.0f;
    mActor->_F44 = false;
    const TVec3f& position = mActor->mPosition;
    addTrans(position - getTrans(), "Module");
    update();
    _12 = 0;
    return true;
}

void MarioRecovery::updateJump() {
    Mtx rotation;
    PSMTXRotAxisRad(rotation, &_64, _7C);
    TVec3f rotated;
    PSMTXMultVec(rotation, &_70, &rotated);
    if (_88 > 60 && _88 < 120) {
        if (_7C < 4.712389f) {
            f32 frames = _88 - 60;
            _84 += -_84 / frames;
        }
        _7C += 0.5f * _80;
    } else {
        _7C += _80;
    }
    if (_88) {
        _88--;
    }
    f32 radius = _84;
    if (radius < 300.0f) {
        radius = 300.0f;
    }
    rotated.setLength(radius);
    TVec3f target(_34 + rotated);
    TVec3f velocity(target - getTrans());
    velocity = velocity * (25.0f / (25.0f + _88));
    u32 elapsed = _18 - _88;
    if (elapsed < 60) {
        f32 factor = 1.0f - elapsed / 60.0f;
        factor *= 0.8f;
        velocity += _1C * factor;
        velocity += _28 * factor;
    }
    f32 vertical = MR::vecKillElement(velocity, getGravityVec(), &velocity);
    if (velocity.length() > 30.0f) {
        velocity.setLength(30.0f);
    }
    velocity += getGravityVec() * vertical;
    addTrans(velocity, "Module");
}

bool MarioRecovery::start() {
    _16 = 0;
    _1A = 0;
    _34 = *getPlayer()->getLastSafetyTrans(nullptr);
    changeAnimationNonStop("引き戻し");
    playSound("声慌て");
    playEffect("引き戻し泡");
    if (_12) {
        _1A = 3;
        _16 = 180;
        mActor->_F44 = false;
        if (MR::diffAngleAbs(_34 - _4C, _58) >= 1.5707964f) {
            TVec3f direction(_34 - _4C);
            if (MR::normalizeOrZero(&direction)) {
                direction.set< f32 >(1.0f, 0.0f, 0.0f);
            }
            TVec3f cross;
            PSVECCrossProduct(&_58, &direction, &cross);
            if (MR::normalizeOrZero(&cross)) {
                cross.set< f32 >(0.0f, 0.0f, 1.0f);
            }
            PSVECCrossProduct(&cross, &_58, &direction);
            MR::normalizeOrZero(&direction);
            _34 = _4C + _58 * 600.0f + direction * 600.0f;
        }
    } else if (!calcFirstVector()) {
        return false;
    }
    MR::startGlobalEventCameraNoTarget("引き戻し", -1);
    return true;
}

bool MarioRecovery::update() {
    clearVelocity();
    switch (_1A) {
    case 0:
        _1A++;
        _16 = _18 + 60;
    case 1: {
        s32 volume;
        if (!_16) {
            volume = 0;
        } else {
            u16 duration = _18;
            volume = _16 / 2;
            if (_16 > duration) {
                volume = 100 - (_16 - duration);
            }
        }
        if (volume < 0) {
            volume = 0;
        }
        if (volume > 100) {
            volume = 100;
        }
        playSound("引き戻し基本");
        playSound("引き戻し浮遊", volume);
        getPlayer()->setFrontVecKeepUp(_1C, 0.1f);
        if (_14) {
            _14--;
            if (!_14) {
                mActor->_F44 = true;
            }
        }
        if (_16) {
            _16--;
        }
        if (!_16 || (_34 - getTrans()).length() < 20.0f) {
            addTrans(_34 - getTrans(), "Module");
            return false;
        }
        updateJump();
        bool passed = false;
        bool distant = true;
        TVec3f shadow(getShadowPos());
        TVec3f shadowOffset(shadow - _34);
        TVec3f initialOffset(_40 - _34);
        if (MR::isNearZero(shadowOffset)) {
            passed = true;
        }
        if (shadowOffset.length() < 50.0f) {
            distant = false;
        }
        MR::normalizeOrZero(&shadowOffset);
        MR::normalizeOrZero(&initialOffset);
        if (shadowOffset.dot(initialOffset) < 0.0f) {
            passed = true;
        }
        if (passed) {
            if (distant && (shadow - getTrans()).length() > 300.0f) {
                break;
            }
            stopEffectForce("引き戻し泡");
            playEffect("引き戻し泡破裂");
            playSound("引き戻し泡破裂");
            _1A++;
        }
        break;
    }
    case 2:
        getPlayer()->_10._2 = true;
        return false;
    case 3:
        if (_16) {
            _16--;
        }
        if (!_16) {
            _1A = 0;
            calcFirstVector();
        } else {
            TVec3f horizontal;
            f32 distance = MR::vecKillElement(_34 - getTrans(), _58, &horizontal);
            if (__fabsf(distance) < 10.0f) {
                addTrans(_58 * distance, "Module");
                _1A = 0;
                calcFirstVector();
            } else {
                addTrans(_58 * MR::clamp(distance, -10.0f, 10.0f), "Module");
            }
            playSound("引き戻し基本");
        }
        break;
    }
    return true;
}

bool MarioRecovery::close() {
    MR::endGlobalEventCamera("引き戻し", -1, true);
    getPlayer()->mMovementStates._1 = true;
    getPlayer()->mMovementStates.jumping = false;
    Mario* player = getPlayer();
    player->_420 = 16;
    stopEffectForce("引き戻し泡");
    playEffect("引き戻し泡破裂");
    stopAnimation(nullptr);
    changeAnimation(nullptr, "基本");
    mActor->_F44 = true;
    return true;
}
