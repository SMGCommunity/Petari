#include "Game/Player/MarioWarp.hpp"
#include "Game/AreaObj/WarpCube.hpp"
#include "Game/MapObj/WarpPod.hpp"
#include "Game/Player/MarioActor.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/AreaObjUtil.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/MathUtil.hpp"

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
    TVec3f vec70(_14 - getTrans());
    TVec3f vec58;
    vec58.cross(vec70, getGravityVec());
    MR::normalizeOrZero(&vec58);

    TVec3f vec88 = vec70 * 0.5f;
    TVec3f vec7C = getTrans() + vec88;

    TVec3f vec64;
    vec64.cross(vec58, vec70);

    MR::normalizeOrZero(&vec64);

    f32 val = _45 == 3 ? _4_PI : PI_4;

    _5C = vec58;
    _68 = vec7C + vec64 * sqrt(sin(val));

    _74 = -vec64 * sin(val);

    _84 = val;
    _80 = -val;

    _88 = 4;

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

        // WarpCube* pPairCube = pWarpCubeMgr->getPairCube(pWarpCube);
        /*
        if (pPairCube == nullptr) {
            return;
        }
        */

        pWarpCubeMgr->startEventCamera(pWarpCube);

        TVec3f cubePos;
        // MR::calcCubePos(pPairCube, &cubePos);

        mVelocity.zero();
        stopWalk();
        mActor->_38C = 10;

        //_56C = pPairCube;
        // mWarp->_14 = cubePos;

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
/* 
void MarioWarp::updateJump() {
}

bool MarioWarp::start() {
}

bool MarioWarp::update() {
}

bool MarioWarp::close() {
}
 */
