#include "Game/AreaObj/WarpCube.hpp"
#include "Game/LiveActor/ActorCameraInfo.hpp"
#include "Game/Util/DirectDraw.hpp"
#include <cstdio>
#include <cstring>

WarpCube::~WarpCube() { 

}

// WarpCubeMgr::getPairCube

void WarpCubeMgr::setInvalidateTimer(AreaObj *pObj, u16 a2) {
    reinterpret_cast<WarpCube*>(pObj)->_48 = a2;
    reinterpret_cast<WarpCube*>(pObj)->mValid = false;
}

void WarpCubeMgr::startEventCamera(const AreaObj *pAreaObj) {
    WarpCube* cube = (WarpCube*)pAreaObj;
    MR::startEventCameraNoTarget(cube->mCameraInfo, cube->mEventName, -1);
    mWarpCube = cube;
}

void WarpCubeMgr::endEventCamera() {
    if (mWarpCube != nullptr) {
        MR::endEventCamera(mWarpCube->mCameraInfo, mWarpCube->mEventName, true, -1);
        mWarpCube = nullptr;
    }
}

void WarpCube::movement() {
    if (_48 != 0) {
        if (_48 == 1) {
            TVec3f pos;
            MR::calcCubePos(this, &pos);

            if (MR::calcDistanceToPlayer(pos) < 300.0f) {
                return;
            }
        }

        if (--_48 == 0) {
            mValid = true;
        }
    }
}

void WarpCube::draw() const {
    TDDraw::setup(0, 1, 0);
    GXSetCullMode(GX_CULL_NONE);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);

    TVec3f rotate;
    TVec3f up;

    up.x = 0.0f;
    up.y = 1.0f;
    up.z = 0.0f;

    MR::calcCubeRotate(this, &rotate);

    Mtx matrix;
    MR::makeMtxTR(matrix, 0.0f, 0.0f, 0.0f, rotate.x, rotate.y, rotate.z);
    PSMTXMultVecSR(matrix, reinterpret_cast<Vec *>(&up), reinterpret_cast<Vec *>(&up));

    TVec3f pos;
    MR::calcCubePos(this, &pos);

    u32 uVar2 = 0x1FFF080;

    bool enabled = false;

    if (mValid && _15 && mAwake) {
        enabled = true;
    }

    if (!enabled) {
        uVar2 = 0x1FCF0010;
    }

    TDDraw::drawSphere(MR::createVecAndScaleByAndAdd(up, pos), 120.0f, uVar2, 16);
}

void WarpCube::init(const JMapInfoIter &rIter) {
    AreaObj::init(rIter);

    s32 groupID = -1;
    MR::getJMapInfoGroupID(rIter, &groupID);

    if (groupID >= 0) {
        mMapIdInfo = new JMapIdInfo(groupID, rIter);
    }

    mCameraInfo = new ActorCameraInfo(rIter);

    s32 local118;
    MR::getJMapInfoArg0WithInit(rIter, &local118);

    char eventName[0x100];
    sprintf(eventName, "ワープカメラ %d-%c", groupID, local118 + 65);

    MR::declareEventCamera(mCameraInfo, eventName);

    s32 eventNameLength = strlen(&eventName[0]);
    mEventName = new char[eventNameLength + 1];
    strcpy(mEventName, &eventName[0]);

    MR::connectToScene(this, 13, -1, -1, 24);
    _48 = 0;
}
