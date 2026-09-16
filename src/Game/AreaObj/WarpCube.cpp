#include "Game/AreaObj/WarpCube.hpp"
#include "Game/LiveActor/ActorCameraInfo.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Util/AreaObjUtil.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/DirectDraw.hpp"
#include "Game/Util/JMapIdInfo.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include <cstdio>

void WarpCube_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
}

WarpCube::~WarpCube() {
}

WarpCube* WarpCubeMgr::getPairCube(const AreaObj* pObj) {
    if (static_cast< const WarpCube* >(pObj)->mMapIdInfo == nullptr) {
        return nullptr;
    }

    for (WarpCube** pCube = reinterpret_cast< WarpCube** >(mArray.begin()); pCube != reinterpret_cast< WarpCube** >(mArray.end()); pCube++) {
        if (*pCube == pObj) {
            continue;
        }

        if ((*pCube)->mMapIdInfo == nullptr) {
            continue;
        }

        if (*(*pCube)->mMapIdInfo == *static_cast< const WarpCube* >(pObj)->mMapIdInfo) {
            return *pCube;
        }
    }

    return nullptr;
}

void WarpCubeMgr::setInvalidateTimer(AreaObj* pAreaObj, u16 a2) {
    WarpCube* pWarpCube = static_cast< WarpCube* >(pAreaObj);

    pWarpCube->mInvalidateTimer = a2;
    pWarpCube->mIsValid = false;
}

void WarpCubeMgr::startEventCamera(const AreaObj* pAreaObj) {
    const WarpCube* pWarpCube = static_cast< const WarpCube* >(pAreaObj);

    MR::startEventCameraNoTarget(pWarpCube->mCameraInfo, pWarpCube->mEventName, -1);

    mWarpCube = pWarpCube;
}

void WarpCubeMgr::endEventCamera() {
    if (mWarpCube == nullptr) {
        return;
    }

    MR::endEventCamera(mWarpCube->mCameraInfo, mWarpCube->mEventName, true, -1);
    mWarpCube = nullptr;
}

void WarpCube::movement() {
    if (mInvalidateTimer == 0) {
        return;
    }

    if (mInvalidateTimer == 1) {
        TVec3f pos;
        MR::calcCubePos(this, &pos);

        if (MR::calcDistanceToPlayer(pos) < 300.0f) {
            return;
        }
    }

    mInvalidateTimer--;

    if (mInvalidateTimer == 0) {
        mIsValid = true;
    }
}

void WarpCube::draw() const {
    TDDraw::setup(0, 1, 0);
    GXSetCullMode(GX_CULL_NONE);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);

    TVec3f cubeRotate;
    TVec3f up(0.0f, 1.0f, 0.0f);
    MR::calcCubeRotate(this, &cubeRotate);

    Mtx matrix;
    MR::makeMtxTR(matrix, 0.0f, 0.0f, 0.0f, cubeRotate.x, cubeRotate.y, cubeRotate.z);
    PSMTXMultVecSR(matrix, &up, &up);

    TVec3f cubePos;
    MR::calcCubePos(this, &cubePos);

    u32 flags = 0x1FFF0080;
    if (!isValid()) {
        flags = 0x1FCF0010;
    }

    TDDraw::drawSphere(cubePos + up * 120.0f, 120.0f, flags, 16);
}

void WarpCube::init(const JMapInfoIter& rIter) {
    AreaObj::init(rIter);

    s32 groupID = -1;
    MR::getJMapInfoGroupID(rIter, &groupID);

    if (groupID >= 0) {
        mMapIdInfo = new JMapIdInfo(groupID, rIter);
    }

    mCameraInfo = new ActorCameraInfo(rIter);

    s32 arg0;
    MR::getJMapInfoArg0WithInit(rIter, &arg0);

    char eventName[0x100];
    sprintf(eventName, "ワープカメラ %d-%c", groupID, arg0 + 65);

    MR::declareEventCamera(mCameraInfo, eventName);

    mEventName = new char[strlen(&eventName[0]) + 1];
    strcpy(mEventName, &eventName[0]);

    MR::connectToScene(this, MR::MovementType_AreaObj, MR::CalcAnimType_None, MR::DrawBufferType_None, MR::DrawType_WarpPodPath);
    mInvalidateTimer = 0;
}
