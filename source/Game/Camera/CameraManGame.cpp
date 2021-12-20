#include "Game/Boss/SkeletalFishGuard.h"
#include "Game/Camera/Camera.h"
#include "Game/Camera/CameraDirector.h"
#include "Game/Camera/CameraHolder.h"
#include "Game/Camera/CameraLocalUtil.h"
#include "Game/Camera/CameraManGame.h"
#include "Game/Camera/CameraParamChunk.h"
#include "Game/Camera/CameraParamChunkHolder.h"
#include "Game/Camera/CameraTargetObj.h"
#include "Game/Camera/CamKarikariEffector.h"
#include "Game/Camera/CamHeliEffector.h"

const char *CameraParamChunk::getClassName() const {
    return "Base";
}

bool CameraTargetObj::isWaterMode() const {
    return false;
}

bool CameraTargetObj::isOnWaterSurface() const {
    return false;
}

bool CameraTargetObj::isFooFighterMode() const {
    return false;
}

CameraManGame::CameraManGame(CameraHolder *pHolder, CameraParamChunkHolder *pChunkHolder, const char *pName) : CameraMan(pName) {
    mHolder = pHolder;
    mChunkHolder = pChunkHolder;
    _58 = 0;
    _5C = 0;
    mKarikari = new CamKarikariEffector();
    mHeli = new CamHeliEffector();
    _68 = 0;
    _6C = 0;
    _70 = 0;
    mZoomedIn = false;
    mCamera = mHolder->getDefaultCamera();
    mChunk = NULL;
    CameraLocalUtil::setWatchPos(this, TVec3f(0.0f, 0.0f, 300.0f));
}

CameraManGame::~CameraManGame() {

}

void CameraManGame::init(const JMapInfoIter &rIter) {

}

void CameraManGame::calc() {
    selectCameraChunk();
    checkReset();
    CameraTargetObj *target = mCamera->calc();
    CameraLocalUtil::setUsedTarget(this, target);
    setSafePose();
    mKarikari->update(this);
    mHeli->update(this);
}

void CameraManGame::notifyActivate() {
    _58 = 1;
}

void CameraManGame::notifyDeactivate() {

}

bool CameraManGame::isInterpolationOff() const {
    if (mCamera != NULL && mCamera->isInterpolationOff()) {
        return true;
    }

    if (mChunk != NULL && mChunk->isAntiBlurOff()) {
        return true;
    }

    return false;
}

bool CameraManGame::isCollisionOff() const {
    if (mCamera != NULL && mCamera->isCollisionOff()) {
        return true;
    }

    if (mChunk != NULL && mChunk->isCollisionOff()) {
        return true;
    }

    return false;
}

bool CameraManGame::isZeroFrameMoveOff() const {
    if (mCamera != NULL) {
        return mCamera->isZeroFrameMoveOff();
    }

    return false;
}

bool CameraManGame::isSubjectiveCameraOff() const {
    if (mCamera != NULL && mCamera->isSubjectiveCameraOff()) {
        return true;
    }

    if (mChunk != NULL && mChunk->isSubjectiveCameraOff()) {
        return true;
    }

    return false;
}

bool CameraManGame::isCorrectingErpPositionOff() const {
    bool off = false;

    if (mCamera != NULL && mCamera->isCorrectingErpPositionOff()) {
        off = true;
    }

    return off;
}

bool CameraManGame::isEnableToRoundLeft() const {
    if (mCamera != NULL) {
        return mCamera->isEnableToRoundLeft();
    }

    return false;
}

bool CameraManGame::isEnableToRoundRight() const {
    if (mCamera != NULL) {
        return mCamera->isEnableToRoundRight();
    }

    return false;
}

bool CameraManGame::isEnableToReset() const {
    if (mCamera != NULL) {
        return mCamera->isEnableToReset();
    }

    return false;
}

void CameraManGame::closeCreatingCameraChunk() {
    createDefaultCamera();
    createDefaultWaterCamera();
    createDefaultWaterSurfaceCamera();
    createDefaultFooFighterCamera();
    createStartAnimCamera();
    createZoomCamera();
}

void CameraManGame::startStartPosCamera(bool a1) {
    _6C = 1;
    _70 = a1 ? 0 : 5;
}

void CameraManGame::endStartPosCamera() {
    _6C = 0;
    _70 = 0;
}

void CameraManGame::zoomIn() {
    mZoomedIn = true;

    CameraParamChunkID_Tmp chunkID = CameraParamChunkID_Tmp();
    chunkID.createOtherID(0, "ズームカメラ");

    CameraParamChunk *chunk = mChunkHolder->getChunk(chunkID);

    TVec3f *pos = CameraLocalUtil::getPos(this);
    TVec3f *watchPos = CameraLocalUtil::getWatchPos(this);

    f32 distance = PSVECDistance(reinterpret_cast<Vec *>(watchPos), reinterpret_cast<Vec *>(pos));
    f32 dVar3 = JMAAsinRadian(100.0f / distance);
    f32 var2 = 1.5f;
    f32 var1 = dVar3 * var2;

    chunk->mExParam.mFovy = (180.0f * (2 * var1)) / 3.1415927f;
}

void CameraManGame::ZoomOut() {
    mZoomedIn = false;
}
