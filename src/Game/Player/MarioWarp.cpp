#include "Game/Player/MarioWarp.hpp"
#include "Game/AreaObj/WarpCube.hpp"
#include "Game/MapObj/WarpPod.hpp"
#include "Game/Player/MarioActor.hpp"
#include "Game/Player/MarioConst.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/AreaObjUtil.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/MathUtil.hpp"

namespace JGeometry {
    TVec3< f32 > TVec3< f32 >::operator*(f32) const NO_INLINE;
}

bool Mario::doObjWarp(LiveActor* pActor) {
    if (getPlayer()->getMovementStates().debugMode) {
        return false;
    }

    if (isStatusActive(MarioStatus_Warp)) {
        return false;
    }

    if (isStatusActive(MarioStatus_Warp)) {
        return false;
    }

    WarpPod* pPod = MR::getWarpPodManager()->getPairPod(pActor);

    if (pPod != nullptr) {
        mWarp->_4C = pActor;

        mVelocity.zero();

        stopWalk();

        mActor->setBlendMtxTimer(16);

        TVec3f axisY;
        TVec3f pairAxisY;

        MR::calcActorAxisY(&axisY, pActor);
        MR::calcActorAxisY(&pairAxisY, pPod);

        mWarp->_14 = pPod->mPosition + pairAxisY * 160.0f;
        mWarp->_20 = pActor->mPosition + axisY * 160.0f;

        MR::getRotatedAxisZ(&mWarp->_38, pPod->mRotation);
        MR::getRotatedAxisY(&mWarp->_2C, pPod->mRotation);

        mWarp->_45 = pPod->mArg1;
        mWarp->_44 = pPod->mArg2;

        stopWalk();

        changeStatus(mWarp);

        stopJump();

        return true;
    }

    return false;
}

bool Mario::doPointWarp(const TVec3f& rVec1, const TVec3f& rVec2, s32 myInt) {
    if (getPlayer()->getMovementStates().debugMode) {
        return false;
    }

    if (isStatusActive(MarioStatus_Warp)) {
        return false;
    }

    if (isStatusActive(MarioStatus_Warp)) {
        return false;
    }

    mVelocity.zero();

    stopWalk();

    mActor->setBlendMtxTimer(16);

    mWarp->_14 = rVec1;
    mWarp->_20 = mPosition;
    mWarp->_38 = rVec2;
    mWarp->_45 = 2;
    mWarp->_44 = false;

    if (myInt != 0) {
        mWarp->_48 = myInt;
    } else {
        mWarp->_48 = 90;
    }

    changeStatus(mWarp);

    stopJump();

    return true;
}

bool Mario::isVisibleRecoveryWarpBubble() const {
    if (isStatusActive(MarioStatus_Warp) && mWarp->_45 == 3) {
        return true;
    }

    return false;
}

MarioWarp::MarioWarp(MarioActor* pActor) : MarioState(pActor, MarioStatus_Warp), _52(), _58(-1) {
    _4C = nullptr;
    _48 = 0;

    _14.zero();
    _20.zero();
    _2C.zero();
    _38.zero();

    _44 = false;
    _45 = 0;
    _50 = false;

    _54 = 0;
    _56 = 0;

    _5C.zero();
    _68.zero();
    _74.zero();

    _88 = 0;
    _80 = 0.0f;
    _84 = 0.0f;
    _8C = 0;
}

void MarioWarp::calcAxis() {
    TVec3f axis;
    TVec3f normal;
    TVec3f displacement = _14 - getTrans();
    f32 distance = displacement.length();
    axis.cross(displacement, getGravityVec());
    MR::normalizeOrZero(&axis);
    TVec3f midpoint = getTrans() + displacement * 0.5f;
    normal.cross(axis, displacement);
    MR::normalizeOrZero(&normal);
    f32 angle = PI / 4.0f;
    if (_45 == 3) {
        angle = PI * 0.4f;
    }
    f32 radius = (0.5f * distance) / MR::sin(angle);
    f32 height = MR::sqrt(radius * radius - 0.5f * (0.5f * distance * distance));
    _5C = axis;
    _68 = midpoint + normal * height;
    _74 = -normal * radius;
    _84 = angle;
    _80 = -angle;
    _88 = radius / 25.0f;
    if (_88 == 0) {
        _88 = 1;
    }
    if (_88 < 120) {
        _88 = 120;
    }
    if (_45 == 2) {
        _88 = _48;
    }
    _8C = _88;
}

void Mario::doCubeWarp() {
    if (getPlayer()->getMovementStates().debugMode) {
        return;
    }

    if (isStatusActive(MarioStatus_Warp)) {
        return;
    }

    if (isStatusActive(MarioStatus_Warp)) {
        return;
    }

    WarpCube* pWarpCube = static_cast< WarpCube* >(MR::getAreaObj("WarpCube", mActor->_2A0));
    WarpCubeMgr* pWarpCubeMgr = static_cast< WarpCubeMgr* >(MR::getAreaObjManager("WarpCube"));

    if (pWarpCube != nullptr) {
        if (_56C == pWarpCube) {
            return;
        }

        WarpCube* pPairCube = pWarpCubeMgr->getPairCube(pWarpCube);
        if (pPairCube == nullptr) {
            return;
        }

        pWarpCubeMgr->startEventCamera(pWarpCube);

        TVec3f cubePos;
        MR::calcCubePos(pPairCube, &cubePos);

        mVelocity.zero();
        stopWalk();
        mActor->_38C = 10;

        _56C = pPairCube;
        mWarp->_14 = cubePos;

        MR::calcCubePos(pWarpCube, &cubePos);
        mWarp->_20 = cubePos;
        mWarp->_38.zero();
        mWarp->_2C.zero();
        mWarp->_45 = 0;
        mWarp->_44 = 0;

        stopWalk();

        changeStatus(mWarp);

        stopJump();

        return;
    }

    if (_56C != nullptr) {
        pWarpCubeMgr->setInvalidateTimer(_56C, 300);
    }

    _56C = nullptr;
}

bool Mario::doPointWarpRecovery(const TVec3f& rVec1, const TVec3f& rVec2) {
    if (getPlayer()->getMovementStates().debugMode) {
        return false;
    }

    if (isStatusActive(MarioStatus_Warp)) {
        return false;
    }

    if (isStatusActive(MarioStatus_Warp)) {
        return false;
    }

    mVelocity.zero();

    stopWalk();

    mActor->setBlendMtxTimer(16);

    mWarp->_14 = rVec1;
    mWarp->_20 = mPosition;
    mWarp->_38 = rVec2;
    mWarp->_2C.zero();
    mWarp->_45 = 3;
    mWarp->_44 = false;

    changeStatus(mWarp);

    stopJump();

    MR::startGlobalEventCameraNoTarget("引き戻し", -1);

    return true;
}
void MarioWarp::updateJump() {
    if (_88 == 0) {
        return;
    }
    f32 progress = (1.0f + MR::sin(((_8C - _88 - 0.5f * _8C) / _8C) * PI)) / 2.0f;
    if (_45 == 2) {
        progress = 1.0f - static_cast< f32 >(_88 - 1) / _8C;
    }
    Mtx rotation;
    TVec3f offset;
    PSMTXRotAxisRad(rotation, &_5C, _80 * (1.0f - progress) + _84 * progress);
    PSMTXMultVecSR(rotation, &_74, &offset);
    TVec3f position = _68 + offset;
    f32 followRate = 0.5f;
    if (_45 == 2) {
        followRate = 1.0f;
    }
    if (_45 == 3) {
        f32 remaining = static_cast< f32 >(_88) / _8C;
        followRate = (1.0f - remaining) + followRate * remaining;
        position = position * remaining + _14 * (1.0f - remaining);
    }
    addTrans((position - getTrans()) * followRate, "Module");
    if (_45 == 2) {
        getPlayer()->setFrontVecKeepUp(_38, 0.2f);
    }
}

bool MarioWarp::start() {
    _52 = 0;
    _58 = 0;
    switch (_45) {
    case 0:
    case 2:
        _54 = 1;
        break;
    case 1:
        _54 = 60;
        break;
    case 3:
        _54 = 40;
        break;
    }
    _56 = _54;
    if (_45 == 3) {
        playEffect("引き戻し泡");
    }
    if (_45 == 1) {
        playEffect("ワープポッドブラー");
        playSound("ワープポッド入り");
    }
    if (_45 == 2) {
        playSound("声小ジャンプ");
    } else {
        playSound("声慌て");
    }
    switch (_45) {
    case 0:
        break;
    case 3:
        changeAnimationNonStop("引き戻し");
        break;
    case 1:
        changeAnimationNonStop("ポッドワープ開始");
        break;
    case 2:
        changeAnimationNonStop("しゃがみジャンプ");
        break;
    }
    mActor->_F44 = 0;
    const TVec3f& position = mActor->mPosition;
    addTrans(position - getTrans(), "Module");
    update();
    if (_45 == 3 && getPlayerMode() != 1) {
        mActor->_A6E = 2;
    }
    if (!_44) {
        getPlayer()->mMovementStates._2B = true;
    }
    return true;
}

bool MarioWarp::update() {
    if (!_44) {
        getPlayer()->mMovementStates._2B = true;
        getPlayer()->mMovementStates._22 = true;
    }
    clearVelocity();
    if (_54) {
        if (_45 == 1 || _45 == 3) {
            playSound("引き戻し基本");
        }
        if (_45 == 1) {
            playSound("ワープポッド移動");
        }
        _54--;
        TVec3f displacement = _20 - getTrans();
        addTrans(displacement * (1.0f - static_cast< f32 >(_54) / _56), "Module");
        if (_54 == 0) {
            calcAxis();
            if (_45 == 1 && getPlayerMode() != 6) {
                MarioActor* actor = mActor;
                actor->_481 = true;
                actor->updateHand();
                actor->updateFace();
            }
        }
        if (_4C != nullptr) {
            MR::getWarpPodManager()->startEventCamera(_4C);
        }
        return true;
    }
    updateJump();
    if (_45 == 0) {
        TVec3f offset = getTrans() - _20;
        TVec3f direction = _14 - _20;
        MR::normalize(&direction);
        f32 distance = MR::vecKillElement(offset, direction, &offset);
        addTrans((_20 + direction * distance) - getTrans(), "Module");
    }
    switch (_45) {
    case 2:
        break;
    case 0:
        playSound("惑星貫通中");
        break;
    case 1:
    case 3: {
        s32 volume;
        if (_88 == 0) {
            volume = 0;
        } else {
            f32 halfway = _8C / 2.0f;
            f32 elapsed = _8C - _88;
            if (elapsed < halfway) {
                volume = 100.0f * (elapsed / halfway);
            } else {
                volume = 100.0f * (1.0f - (elapsed - halfway) / halfway);
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
        if (_45 == 1) {
            playSound("ワープポッド移動");
        }
        break;
    }
    }
    if (_58 == 1 && !isAnimationRun("ポッドワープ終了")) {
        return false;
    }
    if (_88 == 0) {
        if (_45 == 1 && getPlayerMode() != 6) {
            if (_58 != 1) {
                playSound("ワープポッド出");
            }
            _58 = 1;
            mActor->_481 = false;
            changeAnimation("ポッドワープ終了", static_cast< const char* >(nullptr));
            WarpPodMgr* manager = MR::getWarpPodManager();
            if (manager != nullptr) {
                manager->endEventCamera();
            }
        } else {
            return false;
        }
    } else {
        _88--;
    }
    return true;
}

bool MarioWarp::close() {
    getPlayer()->mMovementStates._1 = true;
    getPlayer()->mMovementStates.jumping = false;
    Mario* player = getPlayer();
    player->_420 = 16;
    stopAnimation(nullptr);
    changeAnimation(nullptr, "基本");
    switch (_45) {
    case 0:
        playSound("惑星貫通終了");
        break;
    case 3:
        playSound("引き戻し泡破裂");
        stopEffectForce("引き戻し泡");
        playEffect("引き戻し泡破裂");
        break;
    case 1:
        stopEffect("ワープポッドブラー");
        break;
    case 2:
        changeAnimation("しゃがみジャンプ着地", static_cast< const char* >(nullptr));
        break;
    }
    mActor->_F44 = 1;
    WarpCubeMgr* cubes = static_cast< WarpCubeMgr* >(MR::getAreaObjManager("WarpCube"));
    if (cubes != nullptr) {
        cubes->endEventCamera();
    }
    if (_45 != 1) {
        WarpPodMgr* pods = MR::getWarpPodManager();
        if (pods != nullptr) {
            pods->endEventCamera();
        }
    } else {
        if (_88 != 0 || getPlayerMode() == 6) {
            mActor->_481 = false;
            WarpPodMgr* pods = MR::getWarpPodManager();
            if (pods != nullptr) {
                pods->endEventCamera();
            }
        }
        WarpPodMgr* pods = MR::getWarpPodManager();
        if (pods != nullptr) {
            pods->notifyWarpEnd(static_cast< WarpPod* >(_4C));
        }
    }
    switch (_45) {
    case 0: {
        playSound("声中ジャンプ");
        const MarioConstTable* table = mActor->getConst().getTable();
        getPlayer()->tryForcePowerJump(_2C * table->mWarpPodJumpY + _38 * table->mWarpPodJumpX, false);
        break;
    }
    case 3:
        MR::endGlobalEventCamera("引き戻し", -1, true);
        if (getPlayerMode() != 1) {
            mActor->_A6E = 0;
        }
        getPlayer()->mMovementStates._1 = false;
        getPlayer()->mMovementStates.jumping = true;
        getPlayer()->mMovementStates._2B = true;
        getPlayer()->_402 = 0;
        getPlayer()->mJumpVec = TVec3f(0.0f, 0.0f, 0.0f);
        changeAnimation("落下", static_cast< const char* >(nullptr));
        Mario* player = getPlayer();
        player->_42A = 0;
        player->_430 = 0;
        break;
    case 1:
        if (getPlayerMode() != 1) {
            mActor->_A6E = 0;
        }
        if (getPlayerMode() != 6) {
            playSound("声中ジャンプ");
            getPlayer()->tryForcePowerJump(_2C * 12.0f + _38 * 5.0f, false);
        }
        break;
    case 2:
        getPlayer()->setFrontVecKeepUp(_38);
        break;
    }
    if (getPlayerMode() != 6) {
        getPlayer()->mMovementStates._22 = true;
    } else {
        getPlayer()->mMovementStates._22 = false;
    }
    _4C = nullptr;
    return true;
}
