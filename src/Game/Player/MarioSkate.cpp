#include "Game/Player/MarioSkate.hpp"
#include "Game/Map/CollisionCode.hpp"
#include "Game/Player/Mario.hpp"
#include "Game/Player/MarioActor.hpp"
#include "Game/Player/MarioAnimator.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"

void MarioSkate_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)0.5f;
    (void)-1.0f;
    (void)3.1415927f;
    (void)20.0f;
    (void)50.0f;
    (void)1.25f;
    (void)1.5f;
    (void)1.2f;
    (void)0.04f;
    (void)0.1f;
    (void)-0.1f;
    (void)-0.3926991f;
    (void)0.3926991f;
    (void)-0.5235988f;
    (void)0.5235988f;
    (void)0.8f;
    (void)0.9f;
    (void)0.95f;
    (void)0.05f;
}

bool Mario::isSkatableFloor() const {
    if (_960 == CollisionFloorCode_Ice) {
        return true;
    }

    return _960 == CollisionFloorCode_GlassIce;
}

bool Mario::doSkate() {
    changeStatus(mSkate);
    return true;
}

MarioSkate::MarioSkate(MarioActor* pActor) : MarioState(pActor, MarioStatus_Skate) {
    _14 = 0;
    _20 = 0.0f;
    _18 = 0;
    _19 = 0;
    _1A = 0;
    _1B = 0;
    _1C = 0;
    _1D = 0;
    _24 = 0.0f;
}

bool MarioSkate::postureCtrl(MtxPtr pMtx) {
    getPlayer()->postureCtrl(pMtx);
    f32 rotation = _20;
    rotation *= 3.1415927f;
    PSMTXConcat(pMtx, MR::tmpMtxRotYRad(rotation), pMtx);
    PSMTXConcat(pMtx, MR::tmpMtxRotZRad(_24), pMtx);
    Mtx direction;
    getPlayer()->createDirectionMtx(direction);
    PSMTXConcat(direction, pMtx, pMtx);
    return true;
}

void MarioSkate::exitJump() {
    _18 = 1;
    getPlayer()->tryJump();
    playSound("スケートジャンプ");
}

bool MarioSkate::start() {
    _14 = 0;
    _1B = 0;
    _1C = 0;
    _18 = 0;
    _1D = 1;
    _19 = 0;
    _24 = 0.0f;
    _1A = 0;

    if (isAnimationRun("スケートアクセルジャンプ") || isAnimationRun("スケートジャンプ2") || isAnimationRun("スケートジャンプ3")) {
        _1D = 1;
        changeAnimation(nullptr, "基本");
        changeAnimationNonStop("スケート着地");
        playEffect("スケート右");
        playEffect("スケート左");
        playSound("スケート着地");
        _19 = 0;
        _20 = 0.0f;
    } else {
        _20 = 0.0f;
        if (getPlayer()->mTargetWalkSpeedIndex < 3) {
            _1A = 1;
            changeAnimationNonStop("アイスひねり静止");
        } else {
            changeAnimationNonStop("アイスひねり");
            playEffect("スケート左");
        }

        playSound("スケートスピン");
        playSound("スピンジャンプ");
        playSound("声スピン");
    }

    return true;
}

bool MarioSkate::update() {
    f32 speed;
    if (!getPlayer()->mMovementStates._1 || getPlayer()->mMovementStates.jumping) {
        return false;
    }

    if (checkTrgA() || getPlayer()->mMovementStates._38) {
        exitJump();
        return false;
    }

    if (!getPlayer()->isSkatableFloor()) {
        return false;
    }

    if (!_1A) {
        getPlayer()->mainMove();
    } else {
        return !isAnimationTerminate(nullptr);
    }

    if (getPlayer()->mMovementStates._10) {
        getPlayer()->mMovementStates._10 = false;
        getPlayer()->_3D2 = 0;
    }

    if (checkTrgZ()) {
        bool reverse = !_19;
        _14 = 20;
        _19 = reverse;
        _1C = 1;
        _1B = 1 - _1B;
        playSound("声壁押し");
    }

    getPlayer()->updateWalkSpeed();
    if (isAnimationRun("スケート着地")) {
        if (isAnimationTerminate(nullptr)) {
            _1C = 1;
        }
    } else if (_1D == 1) {
        f32 stick = getStickP();
        f32 frame = 20.0f + 50.0f * (1.0f - stick);
        if (getAnimator()->getFrame() > frame && getStickP()) {
            _1C = 1;
        }
    }

    if (mActor->isRequestSpin()) {
        if (_1D && (!isAnimationRun("アイスひねり移動") || isAnimationTerminate(nullptr))) {
            stopAnimation(nullptr);
            changeAnimationNonStop("アイスひねり移動");
            playSound("スケートスピン");
            playSound("スピンジャンプ");
            playSound("声パンチ");
            if (getPlayer()->mWalkSpeed < 1.25f) {
                getPlayer()->mWalkSpeed = 1.5f * getPlayer()->mWalkSpeed;
            }
        }

        if (getAnimator()->getFrame() > 20.0f) {
            _1C = 1;
        }
    }

    speed = getPlayer()->mWalkSpeed;
    if (speed < 1.2f * getStickP()) {
        getPlayer()->mWalkSpeed = 1.2f * getStickP();
    }

    if (speed > 0.0f && !isAnimationRun("基本")) {
        playSound("スケート滑り");
    }

    if (_19) {
        _20 += 0.04f;
        _20 = MR::clamp(_20, -1.0f, 1.0f);
    } else {
        if (_20 == 1.0f) {
            _20 = -1.0f;
        }

        bool negative = false;
        if (_20 <= 0.0f) {
            negative = true;
        }

        _20 += 0.04f;
        if (negative) {
            _20 = -MR::clamp(-_20, 0.0f, 1.0f);
        } else {
            _20 = MR::clamp(_20, -1.0f, 1.0f);
        }
    }

    f32 angle = MR::diffAngleAbsHorizontal(getWorldPadDir(), getFrontVec(), getGravityVec());
    TVec3f cross;
    const TVec3f& rPad = getWorldPadDir();
    cross.cross(getFrontVec(), rPad);
    if (cross.dot(getGravityVec()) < 0.0f) {
        angle = -angle;
    }

    if (mActor->_3E5) {
        _14 = 15;
    } else {
        _14++;
    }

    u32 interval = 30;
    if (_19) {
        interval = 60;
    }

    if (_14 >= interval) {
        if (_1C) {
            if (!(angle >= 0.1f && _1B == 1) && !(angle <= -0.1f && _1B == 0)) {
                _1C = 0;
                _14 = 0;
                _1B = 1 - _1B;
                if (_1D < 1) {
                    _1D++;
                } else {
                    switch (_1B) {
                    case 0:
                        if (_19) {
                            changeAnimationNonStop("氷上後行右");
                        } else {
                            changeAnimationNonStop("氷上力行左");
                        }

                        playEffect("スケート左");
                        stopEffect("スケート右");
                        playSound("スケート足");
                        break;
                    case 1:
                        if (_19) {
                            changeAnimationNonStop("氷上後行左");
                        } else {
                            changeAnimationNonStop("氷上力行右");
                        }

                        playEffect("スケート右");
                        stopEffect("スケート左");
                        playSound("スケート足");
                        break;
                    }
                }
            }
        } else if (_14 > 90) {
            return false;
        }
    }

    f32 animationSpeed = 1.0f;
    f32 animationScale = 1.0f - 0.5f * (1.0f - getStickP());
    if (!isAnimationRun("スケート着地")) {
        getAnimator()->setSpeed(animationSpeed * animationScale);
    }

    f32 tilt;
    if (_19) {
        tilt = MR::clamp(angle, -0.3926991f, 0.3926991f);
    } else {
        tilt = MR::clamp(angle, -0.5235988f, 0.5235988f);
    }

    f32 targetTilt = tilt * (1.0f - 0.8f * (1.0f - getStickP()));
    if (MR::abs(targetTilt) > MR::abs(_24)) {
        _24 = 0.9f * _24 + 0.1f * targetTilt;
    } else {
        _24 = 0.95f * _24 + 0.05f * targetTilt;
    }

    return true;
}

bool MarioSkate::close() {
    if (_18) {
        if (!_1A) {
            switch (getPlayer()->_430) {
            case 1:
                changeAnimationNonStop("スケートジャンプ2");
                break;
            case 2:
                changeAnimationNonStop("スケートジャンプ3");
                break;
            default:
                changeAnimationNonStop("スケートアクセルジャンプ");
                break;
            }

            TVec3f velocity;
            Mario* pPlayer = getPlayer();
            f32 vertical = MR::vecKillElement(pPlayer->mJumpVec, getGravityVec(), &velocity);
            velocity *= 1.5f;
            velocity += getGravityVec() * vertical;
            getPlayer()->setJumpVec(velocity);
        }
    } else if (getPlayer()->mMovementStates._1) {
        stopAnimation(nullptr, "基本");
    } else {
        stopAnimation(nullptr, "落下");
    }

    stopEffect("スケート左");
    stopEffect("スケート右");
    return true;
}
