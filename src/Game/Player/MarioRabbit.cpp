#include "Game/Player/MarioRabbit.hpp"
#include "Game/Player/Mario.hpp"
#include "Game/Player/MarioActor.hpp"
#include "Game/Player/MarioConst.hpp"
#include "Game/Util/JointUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"

void Mario::startRabbitMode() {
    if (!isStatusActive(MarioStatus_Rabbit)) {
        changeStatus(mRabbit);
    }
}

void Mario::endRabbitMode() {
    if (isStatusActive(MarioStatus_Rabbit)) {
        closeStatus(mRabbit);
    }
}

MarioRabbit::MarioRabbit(MarioActor* actor) : MarioState(actor, MarioStatus_Rabbit), _14(0.0f), _18(0.0f) {
    _24 = 0;
    _25 = 0;
    _26 = 0;
    _27 = 0;
    _28 = 0;
    _68 = 0;
    _69 = 0;
    _2A = 0;
    PSMTXIdentity(_2C);
    _5C.set(1.0f, 0.0f, 0.0f);
    _6A = 0;
}

bool MarioRabbit::start() {
    _24 = 0;
    _28 = 0;
    _25 = 0;
    _68 = 0;
    _69 = 0;
    Mario* player = getPlayer();
    MR::vecKillElement(player->mJumpVec, getAirGravityVec(), &_18);
    _5C = getFrontVec();
    if (_26 || getPlayer()->mMovementStates.jumping) {
        _6A = 1;
        return true;
    }
    stopAnimationUpper(nullptr, nullptr);
    _14 = -mActor->getConst().getTable()->mRabbitFirstJump;
    _26 = 0;
    _2A = 0;
    if (getPlayer()->mDrawStates._10) {
        _14 *= 0.25f;
        _18 = getPlayer()->getWallNorm() * mActor->getConst().getTable()->mRabbitFirstJump * 0.5f;
        getPlayer()->setFrontVecKeepUp(getPlayer()->getWallNorm());
        _25 = 1;
        _2A = 60;
    }
    impact();
    return true;
}

void MarioRabbit::hop() {
    if (_14 < 0.0f) {
        _14 = 0.5f * -mActor->getConst().getTable()->mRabbitFirstJump;
    } else {
        _14 = 0.3f * -mActor->getConst().getTable()->mRabbitFirstJump;
    }
    _68 = 0;
}

void MarioRabbit::forceJump() {
    _26 = 1;
    if (getPlayer()->_430 != 12) {
        _68 = 1;
    }
}

void MarioRabbit::impact() {
    if (!getPlayer()->mDrawStates._10) {
        if (_18.length() > 2.0f * mActor->getConst().getTable()->mRabbitMoveSpeed) {
            _18.setLength(2.0f * mActor->getConst().getTable()->mRabbitMoveSpeed);
        }
        _18.setLength(0.5f * _18.length());
    }
    getPlayer()->stopJump();
    getPlayer()->mMovementStates._1 = false;
    getPlayer()->mMovementStates.jumping = true;
    getPlayer()->initJumpParam();
    Mario* player = getPlayer();
    player->_42A = 0;
    player->_430 = 0;
    getPlayer()->mMovementStates._22 = false;
    getPlayer()->mMovementStates._2B = false;
    if (_28) {
        switch (_68) {
        case 0:
            changeAnimationNonStop("ホッパーハイジャンプA");
            break;
        case 1:
            changeAnimationNonStop("ホッパーハイジャンプB");
            break;
        }
        _68 = 1 - _68;
    } else if (!MR::isNearZero(getStickP())) {
        switch (_68) {
        case 0:
            changeAnimationNonStop("ホッパー移動A");
            break;
        case 1:
            changeAnimationNonStop("ホッパー移動B");
            break;
        }
        _68 = 1 - _68;
    } else {
        switch (_68) {
        case 0:
            changeAnimationNonStop("ホッパージャンプA");
            break;
        case 1:
            changeAnimationNonStop("ホッパージャンプB");
            break;
        }
    }
    _27 = 1;
}

bool MarioRabbit::update() {
    if (getPlayer()->mMorphResetTimer) {
        return false;
    }
    getPlayer()->mMovementStates._30 = false;
    getPlayer()->checkWallStick();
    if (_69) {
        _69--;
    }
    if (!getPlayer()->mMovementStates.jumping && !getPlayer()->mMovementStates._1) {
        getPlayer()->mMovementStates._1 = true;
    }
    if (getPlayer()->mMovementStates._1) {
        if (_6A) {
            playSound("ホッパー跳ね返り");
            _6A = 0;
        }
        if (_27 || isAnimationRun("ホッパー壁ジャンプ") || isAnimationRun("ホッパーヒップドロップ")) {
            stopAnimation(nullptr);
            switch (_68) {
            case 0:
                changeAnimation("ホッパージャンプA", static_cast< const char* >(nullptr));
                break;
            case 1:
                changeAnimation("ホッパージャンプB", static_cast< const char* >(nullptr));
                break;
            }
            _28 = 0;
            _27 = 0;
            if (!getPlayer()->mMovementStates._B) {
                getPlayer()->mMovementStates.jumping = false;
            }
            startPadVib(1UL);
            playEffect("共通着地普通");
            if (!_28) {
                playSound("ホッパー跳ね返り");
            }
        }
        if (getPlayer()->_3CE < mActor->getConst().getTable()->mHopperLandingTime) {
            if (mActor->isRequestJump() || _69) {
                _28 = 1;
                playEffect("共通ハイジャンプ");
                startPadVib("マリオ[ホッパーため]");
                switch (_68) {
                case 0:
                    changeAnimationNonStop("ホッパーハイジャンプA");
                    break;
                case 1:
                    changeAnimationNonStop("ホッパーハイジャンプB");
                    break;
                }
            }
            return true;
        }
        if (_28) {
            if (getPlayer()->_3CE < mActor->getConst().getTable()->mRabbitChargeTime2) {
                playSound("ホッパージャンプ溜め");
                return true;
            }
            _14 = -mActor->getConst().getTable()->mRabbitFirstJump2;
            playSound("声物ジャンプ");
            playSound("ホッパージャンプ");
        } else {
            _14 = -mActor->getConst().getTable()->mRabbitFirstJump;
        }
        impact();
        _26 = 0;
    } else {
        if (_26) {
            getPlayer()->procJump(false);
            return true;
        }
        if (mActor->isRequestHipDrop()) {
            if (getPlayer()->jumpToHipDrop()) {
                _68 = 0;
            }
        }
        if (mActor->isRequestJump()) {
            _69 = 3;
        }
    }
    getPlayer()->mJumpVec = getAirGravityVec() * _14;
    addVelocity(getAirGravityVec(), _14);
    if (_28) {
        if (_14 < 0.0f) {
            _14 += mActor->getConst().getTable()->mRabbitGravityRise2;
        } else {
            _14 += mActor->getConst().getTable()->mRabbitGravityDrop2;
        }
    } else {
        if (_14 < 0.0f) {
            _14 += mActor->getConst().getTable()->mRabbitGravityRise;
        } else {
            _14 += mActor->getConst().getTable()->mRabbitGravityDrop;
        }
    }
    if (_14 > 50.0f) {
        _14 = 50.0f;
    }
    if (getStickP() != 0.0f) {
        if (_2A) {
            _2A--;
        } else {
            const TVec3f& direction = getWorldPadDir();
            if (_28) {
                getPlayer()->setFrontVecKeepUp(direction, mActor->getConst().getTable()->mRabbitTurnRatio2);
                if (_14 < 0.0f) {
                    _18 += getFrontVec() * mActor->getConst().getTable()->mRabbitMoveAcc2;
                } else {
                    _18 += getFrontVec() * mActor->getConst().getTable()->mRabbitMoveAcc3;
                }
            } else {
                getPlayer()->setFrontVecKeepUp(direction, mActor->getConst().getTable()->mRabbitTurnRatio);
                _18 += getFrontVec() * mActor->getConst().getTable()->mRabbitMoveAcc;
            }
        }
    }
    if (!_25) {
        if (_18.length() > mActor->getConst().getTable()->mRabbitMoveSpeed) {
            _18.setLength(mActor->getConst().getTable()->mRabbitMoveSpeed);
        }
    }
    addVelocity(_18);
    f32 angle = MR::diffAngleAbsHorizontal(getFrontVec(), _5C, getAirGravityVec());
    TVec3f cross;
    PSVECCrossProduct(&getFrontVec(), &_5C, &cross);
    if (cross.dot(getAirGravityVec()) < 0.0f) {
        angle = -angle;
    }
    PSMTXCopy(MR::tmpMtxRotXRad(angle), _2C);
    if (__fabsf(angle) >= 1.0471976f) {
        MR::vecBlendSphere(_5C, getFrontVec(), &_5C, 0.2f);
    } else {
        MR::vecBlendSphere(_5C, getFrontVec(), &_5C, 0.05f);
    }
    MR::normalizeOrZero(&_5C);
    switch (_68) {
    case 0:
        setJointGlobalMtx(static_cast< u8 >(MR::getJointIndex(mActor, "Hip")), _2C);
    case 1:
        setJointGlobalMtx(static_cast< u8 >(MR::getJointIndex(mActor, "Spine1")), _2C);
        break;
    }
    return true;
}

bool MarioRabbit::close() {
    stopAnimation(nullptr);
    if (getPlayer()->mMovementStates.jumping) {
        stopAnimation(nullptr, "落下");
    } else {
        stopAnimation(nullptr, "基本");
    }
    setJointGlobalMtx(static_cast< u8 >(MR::getJointIndex(mActor, "Hip")), nullptr);
    setJointGlobalMtx(static_cast< u8 >(MR::getJointIndex(mActor, "Spine1")), nullptr);
    return true;
}
