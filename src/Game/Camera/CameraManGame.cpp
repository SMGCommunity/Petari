#include "Game/AreaObj/CubeCamera.hpp"
#include "Game/Boss/SkeletalFishGuard.hpp"
#include "Game/Camera/Camera.hpp"
#include "Game/Camera/Camera.hpp"
#include "Game/Camera/CameraDirector.hpp"
#include "Game/Camera/CameraHeightArrange.hpp"
#include "Game/Camera/CameraHolder.hpp"
#include "Game/Camera/CameraLocalUtil.hpp"
#include "Game/Camera/CameraManGame.hpp"
#include "Game/Camera/CameraParamChunk.hpp"
#include "Game/Camera/CameraParamChunkHolder.hpp"
#include "Game/Camera/CameraTargetObj.hpp"
#include "Game/Camera/CamKarikariEffector.hpp"
#include "Game/Camera/CamHeliEffector.hpp"
#include "Game/MapObj/GCapture.hpp"
#include <cstring>

namespace {
    const char *gDefaultCameraName = "デフォルトカメラ";
    const char *gDefaultWaterCameraName = "デフォルト水中カメラ";
    const char *gDefaultWaterSurfaceCameraName = "デフォルト水面カメラ";
    const char *gDefaultFooFighterCameraName = "デフォルトフーファイターカメラ";
    const char *gDefaultStartAnimCameraName = "スタートアニメカメラ";
}

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

CubeCameraArea *CameraTargetObj::getCubeCameraArea() const {
    return nullptr;
}

Triangle *CameraTargetObj::getGroundTriangle() const {
    return nullptr;
}

CameraManGame::CameraManGame(CameraHolder *pHolder, CameraParamChunkHolder *pChunkHolder, const char *pName) : CameraMan(pName) {
    mHolder = pHolder;
    mChunkHolder = pChunkHolder;
    _58 = 0;
    _5C = 0;
    mKarikari = new CamKarikariEffector();
    mHeli = new CamHeliEffector();
    mTypeState = 0;
    _6C = 0;
    _70 = 0;
    mZoomedIn = false;
    mCamera = mHolder->getDefaultCamera();
    mChunk = nullptr;
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
    if (mCamera != nullptr && mCamera->isInterpolationOff()) {
        return true;
    }

    if (mChunk != nullptr && mChunk->isAntiBlurOff()) {
        return true;
    }

    return false;
}

bool CameraManGame::isCollisionOff() const {
    if (mCamera != nullptr && mCamera->isCollisionOff()) {
        return true;
    }

    if (mChunk != nullptr && mChunk->isCollisionOff()) {
        return true;
    }

    return false;
}

bool CameraManGame::isZeroFrameMoveOff() const {
    if (mCamera != nullptr) {
        return mCamera->isZeroFrameMoveOff();
    }

    return false;
}

bool CameraManGame::isSubjectiveCameraOff() const {
    if (mCamera != nullptr && mCamera->isSubjectiveCameraOff()) {
        return true;
    }

    if (mChunk != nullptr && mChunk->isSubjectiveCameraOff()) {
        return true;
    }

    return false;
}

bool CameraManGame::isCorrectingErpPositionOff() const {
    bool off = false;

    if (mCamera != nullptr && mCamera->isCorrectingErpPositionOff()) {
        off = true;
    }

    return off;
}

bool CameraManGame::isEnableToRoundLeft() const {
    if (mCamera != nullptr) {
        return mCamera->isEnableToRoundLeft();
    }

    return false;
}

bool CameraManGame::isEnableToRoundRight() const {
    if (mCamera != nullptr) {
        return mCamera->isEnableToRoundRight();
    }

    return false;
}

bool CameraManGame::isEnableToReset() const {
    if (mCamera != nullptr) {
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

void CameraManGame::zoomOut() {
    mZoomedIn = false;
}

void CameraManGame::selectCameraChunk() {
    if (!tryStartPosCamera() && !tryZoomCamera()) {
        checkStateShift();

        switch (mTypeState) {
            case 0:
                updateNormal();
                break;
            case 1:
                updateSwim();
                break;
            case 2:
                updateWaterSurface();
                break;
            case 3:
                updateGCapture();
                break;
            case 4:
                updateFooFighter();
                break;
            case 5:
                break;
        }
    }
}

void CameraManGame::setChunk(const CameraParamChunkID &rChunk) {
    CameraParamChunk *chunk = mChunkHolder->getChunk(rChunk);

    if (chunk == nullptr) {
        setNullCamera();
    }
    else {
        chunk = tryToReplaceChunkToDefault(chunk);
        requestResetIfNecessary(chunk);
        replaceCurrentChunkAndCamera(chunk);
        applyParameter();
    }
}

#ifdef NON_MATCHING
// Register mismatch, r0 used
void CameraManGame::setNullCamera() {
    mChunk = nullptr;
    s32 index = mHolder->getIndexOfDefault();
    mCamera = mHolder->getCameraInner(index);
    
    f32 fovy = mDirector->getDefaultFovy();
    CameraLocalUtil::setFovy(this, fovy);
    CameraLocalUtil::setRoll(this, 0.0f);

    CameraLocalUtil::setGlobalOffset(this, TVec3f(0.0f, 0.0f, 0.0f));
    CameraLocalUtil::setLocalOffset(this, TVec3f(0.0f, 0.0f, 0.0f));
    CameraLocalUtil::setFrontOffset(this, 0.0f);
    CameraLocalUtil::setUpperOffset(this, 0.0f);

    if (mCamera->mVPan != nullptr) {
        mCamera->mVPan->resetParameter();
    }
}
#endif

CameraParamChunk *CameraManGame::tryToReplaceChunkToDefault(CameraParamChunk *pChunk) {
    if (strcmp(pChunk->getClassName(), "Game") != 0) {
        return pChunk;
    }
    
    CameraParamChunkGame *gameChunk = reinterpret_cast<CameraParamChunkGame *>(pChunk);

    if (gameChunk->mThru != 1) {
        return pChunk;
    }

    static const char *name = "デフォルトカメラ";

    CameraParamChunkID_Tmp chunkID = CameraParamChunkID_Tmp();
    chunkID.createOtherID(0, name);

    return mChunkHolder->getChunk(chunkID);
}

void CameraManGame::requestResetIfNecessary(CameraParamChunk *pChunk) {
    if (isNecessaryToReset(pChunk)) {
        _58 = 1;

        if (isZoomCamera()) {
            mDirector->setInterpolation(mChunk->mExParam.mCamInt);
        }
        else if (isCurrentChunkEnableEndInterp()) {
            CameraParamChunkGame *gameChunk = reinterpret_cast<CameraParamChunkGame *>(mChunk);
            mDirector->setInterpolation(gameChunk->mCamEndInt);
        }
        else {
            mDirector->setInterpolation(pChunk->mExParam.mCamInt);
        }
    }
}

bool CameraManGame::isNecessaryToReset(CameraParamChunk *pChunk) const {
    if (mChunk == pChunk) {
        return false;
    }

    u8 cameraTypeIndex = pChunk->mCameraTypeIndex;

    if (cameraTypeIndex != mHolder->getIndexOf(mCamera)) {
        return true;
    }

    if (mChunk != nullptr && !mChunk->isOnNoReset()) {
        return true;
    }

    if (!pChunk->isOnNoReset()) {
        return true;
    }

    return false;
}


bool CameraManGame::isCurrentChunkEnableEndInterp() const {
    bool is;

    if (mChunk != nullptr) {
        is = false;

        if (strcmp(mChunk->getClassName(), "Game") == 0) {
            CameraParamChunkGame *gameChunk = reinterpret_cast<CameraParamChunkGame *>(mChunk);

            if (gameChunk->mEnableEndErpFrame != 0) {
                is = true;
            }
        }
    }
    else {
        is = false;
    }

    return is;
}

void CameraManGame::replaceCurrentChunkAndCamera(CameraParamChunk *pChunk) {
    mCamera = mHolder->getCameraInner(pChunk->mCameraTypeIndex);
    mChunk = pChunk;
}

#ifdef NON_MATCHING
// Register mismatch, r0 used
void CameraManGame::applyParameter() {
    CamTranslatorBase *translator = mHolder->getTranslator(mChunk->mCameraTypeIndex);
    translator->setParam(mChunk);
    Camera *camera = translator->getCamera();

    camera->setZoneMtx(mChunk->getZoneID());

    CameraLocalUtil::setGlobalOffset(mCamera, mChunk->mExParam.mWOffset);

    TVec3f *localOffset = CameraLocalUtil::getLocalOffset(this);
    CameraLocalUtil::setLocalOffset(mCamera, *localOffset);
    CameraLocalUtil::setFrontOffset(mCamera, mChunk->mExParam.mLOffset);
    CameraLocalUtil::setUpperOffset(mCamera, mChunk->mExParam.mLOffsetV);

    if (mChunk->isOnUseFovy()) {
        CameraLocalUtil::setFovy(mCamera, mChunk->mExParam.mFovy);
    }
    else {
        CameraLocalUtil::setFovy(mCamera, mDirector->getDefaultFovy());
    }

    if (mChunk->isLOfsErpOff()) {
        camera->_18 = 1;
    }
    else {
        camera->_18 = 0;
    }

    CameraLocalUtil::setRoll(mCamera, mChunk->mExParam.mRoll);

    if (camera->mVPan != nullptr) {
        CameraHeightArrange *vPan = camera->mVPan;
        vPan->resetParameter();

        vPan->mUpper = mChunk->mExParam.mUpper;
        vPan->mLower = mChunk->mExParam.mLower;
        vPan->mGndInt = mChunk->mExParam.mGndInt;
        vPan->mUPlay = mChunk->mExParam.mUPlay;
        vPan->mLPlay = mChunk->mExParam.mLPlay;
        vPan->mPushDelay = mChunk->mExParam.mPushDelay;
        vPan->mPushDelayLow = mChunk->mExParam.mPushDelayLow;
        vPan->mUDown = mChunk->mExParam.mUDown;
        vPan->mVPanUse = mChunk->mExParam.mVPanUse != 0;

        TVec3f axis;
        mChunk->getVPanAxis(&axis);

        vPan->mVPanAxis.set(axis);
        
        vPan->_60 = 1;
    }
}
#endif

void CameraManGame::checkReset() {
    if (_58 == 0 || mChunk == nullptr || mCamera == nullptr) {
        return;
    }
    
    mCamera->mCameraMan = this;
    mCamera->reset();
    _58 = 0;
}

void CameraManGame::setSafePose() {
    TVec3f pos = TVec3f(*CameraLocalUtil::getPos(mCamera));
    TVec3f watchPos = TVec3f(*CameraLocalUtil::getWatchPos(mCamera));
    TVec3f up = TVec3f(*CameraLocalUtil::getUpVec(mCamera));

    if (MR::isNan(pos) || MR::isNan(watchPos) || MR::isNan(up)) {
        return;
    }

    keepAwayWatchPos(&watchPos, pos);
    calcSafeUpVec(&up, pos, watchPos);

    CameraLocalUtil::setPos(this, pos);
    CameraLocalUtil::setUpVec(this, up);
    CameraLocalUtil::setWatchPos(this, watchPos);

    TVec3f *watchUp = CameraLocalUtil::getWatchUpVec(mCamera);
    CameraLocalUtil::setWatchUpVec(this, *watchUp);

    TVec3f *globalOffset = CameraLocalUtil::getGlobalOffset(mCamera);
    CameraLocalUtil::setGlobalOffset(this, *globalOffset);

    TVec3f *localOffset = CameraLocalUtil::getLocalOffset(mCamera);
    CameraLocalUtil::setLocalOffset(this, *localOffset);

    f32 fovy = CameraLocalUtil::getFovy(mCamera);
    CameraLocalUtil::setFovy(this, fovy);

    f32 roll = CameraLocalUtil::getRoll(mCamera);
    CameraLocalUtil::setRoll(this, roll);
}

#ifdef NON_MATCHING
// Stack issues
void CameraManGame::keepAwayWatchPos(TVec3f *watchPos, const TVec3f &pos) {
    TVec3f dir = *watchPos - pos;
    float length = PSVECMag(reinterpret_cast<Vec *>(&dir));

    if (length < 300.0f) {
        if (length < 1.0f) {
            TVec3f *currentPos = CameraLocalUtil::getPos(this);
            TVec3f *currentWatchPos = CameraLocalUtil::getWatchPos(this);
            
            TVec3f newWatchPos1 = pos + *currentWatchPos;
            TVec3f newWatchPos2 = newWatchPos1 - *currentPos;

            watchPos->set(newWatchPos2);
        }
        else {
            float length2 = PSVECMag(reinterpret_cast<Vec *>(&dir));
            PSVECNormalize(reinterpret_cast<Vec *>(&dir), reinterpret_cast<Vec *>(&dir));

            TVec3f dirCopy = TVec3f(dir);
            dirCopy.x *= 300.0f;
            dirCopy.y *= 300.0f;
            dirCopy.z *= 300.0f;

            watchPos->set(pos + dirCopy);
        }
    }
}
#endif

void CameraManGame::createDefaultCamera() {
    CameraParamChunkID_Tmp chunkID = CameraParamChunkID_Tmp();
    chunkID.createOtherID(0, gDefaultCameraName);
    mChunkHolder->createChunk(chunkID, nullptr);
}

void CameraManGame::createDefaultWaterCamera() {
    CameraParamChunkID_Tmp chunkID = CameraParamChunkID_Tmp();
    chunkID.createOtherID(0, gDefaultWaterCameraName);
    CameraParamChunk *chunk = mChunkHolder->createChunk(chunkID, nullptr);

    chunk->setCameraType("CAM_TYPE_WATER_FOLLOW", mHolder);

    TVec3f wOffset = TVec3f(0.0f, 170.0f, 0.0f);
    chunk->mExParam.mWOffset.set(wOffset);
    chunk->mExParam.mLOffset = 100.0f;
    chunk->mExParam.mFovy = 45.0f;
    chunk->mExParam.mCamInt = 120;
    chunk->mGeneralParam->mAxis.x = 1000.0f;
    chunk->mGeneralParam->mAxis.y = 900.0f;
    chunk->mGeneralParam->mDist = 0.01f;
}

void CameraManGame::createDefaultWaterSurfaceCamera() {
    CameraParamChunkID_Tmp chunkID = CameraParamChunkID_Tmp();
    chunkID.createOtherID(0, gDefaultWaterSurfaceCameraName);
    CameraParamChunk *chunk = mChunkHolder->createChunk(chunkID, nullptr);

    chunk->setCameraType("CAM_TYPE_FOLLOW", mHolder);

    TVec3f wOffset = TVec3f(0.0f, 170.0f, 0.0f);
    chunk->mExParam.mWOffset.set(wOffset);
    chunk->mExParam.mLOffset = 100.0f;
    chunk->mExParam.mFovy = 45.0f;
    chunk->mExParam.mCamInt = 120;
    chunk->mGeneralParam->mAxis.x = 900.0;
    chunk->mGeneralParam->mAxis.y = 600.0f;
    chunk->mGeneralParam->mAngleA = 0.174533f;
    chunk->mGeneralParam->mAngleB = 0.349066f;
    chunk->mGeneralParam->mDist = 0.15f;
    chunk->mGeneralParam->mNum1 = 0;
}

void CameraManGame::createDefaultFooFighterCamera() {
    CameraParamChunkID_Tmp chunkID = CameraParamChunkID_Tmp();
    chunkID.createOtherID(0, gDefaultFooFighterCameraName);
    CameraParamChunk *chunk = mChunkHolder->createChunk(chunkID, nullptr);

    chunk->setCameraType("CAM_TYPE_FOO_FIGHTER", mHolder);

    TVec3f wOffset = TVec3f(0.0f, 170.0f, 0.0f);
    chunk->mExParam.mWOffset.set(wOffset);
    chunk->mExParam.mLOffset = 100.0f;
    chunk->mExParam.mFovy = 45.0f;
    chunk->mExParam.mCamInt = 120;
    chunk->mGeneralParam->mAxis.x = 1200.0;
    chunk->mGeneralParam->mAxis.y = 300.0f;
    chunk->mGeneralParam->mDist = 0.03f;
}

void CameraManGame::createStartAnimCamera() {
    void *data;
    s32 size;
    MR::getCurrentScenarioStartAnimCameraData(&data, &size);

    if (size >= 0) {
        CameraParamChunkID_Tmp chunkID = CameraParamChunkID_Tmp();
        chunkID.createOtherID(0, gDefaultStartAnimCameraName);
        CameraParamChunk *chunk = mChunkHolder->createChunk(chunkID, nullptr);

        CameraDirector *director = CameraLocalUtil::getCameraDirector();
        chunk->setCameraType("CAM_TYPE_ANIM", director->mHolder);

        chunk->mGeneralParam->mNum1 = reinterpret_cast<s32>(data);
        chunk->_64 = true;
    }
}

void CameraManGame::createZoomCamera() {
    CameraParamChunkID_Tmp chunkID = CameraParamChunkID_Tmp();
    chunkID.createOtherID(0, "ズームカメラ");
    CameraParamChunk *chunk = mChunkHolder->createChunk(chunkID, nullptr);

    CameraDirector *director = CameraLocalUtil::getCameraDirector();
    chunk->setCameraType("CAM_TYPE_EYEPOS_FIX_THERE", director->mHolder);

    TVec3f wOffset = TVec3f(0.0f, 0.0f, 0.0f);
    chunk->mExParam.mWOffset.set(wOffset);
    chunk->mExParam.mLOffset = 0.0f;
    chunk->mExParam.mLOffsetV = 100.0f;

    chunk->setUseFovy(true);
    chunk->setLOfsErpOff(true);

    chunk->mExParam.mCamInt = 0;
    chunk->mGeneralParam->mNum1 = 1;
    chunk->mGeneralParam->mNum2 = 1;
    chunk->_64 = 1;
}

void CameraManGame::checkStateShift() {
    if (!tryShiftToGCapture() && !tryShiftToSwimOrWaterSurface() && !tryShiftToFooFighter()) {
        mTypeState = 0;
    }
}

bool CameraManGame::tryShiftToGCapture() {
    bool captured = MR::isPlayerGCaptured();

    if (captured) {
        mTypeState = 3;
        return true;
    }

    return false;
}

bool CameraManGame::tryShiftToSwimOrWaterSurface() {
    CameraTargetObj *target = CameraLocalUtil::getTarget(this);

    if (target->isWaterMode()) {
        CameraTargetObj *target2 = CameraLocalUtil::getTarget(this);

        if (target2->isOnWaterSurface()) {
            mTypeState = 2;
        }
        else {
            mTypeState = 1;
        }

        return true;
    }

    return false;
}

bool CameraManGame::tryShiftToFooFighter() {
    CameraTargetObj *target = CameraLocalUtil::getTarget(this);

    if (target->isFooFighterMode()) {
        mTypeState = 4;
        return true;
    }

    return false;
}

/*void CameraManGame::updateNormal() {
    if (!setCubeChunk(CubeCameraArea::CATEGORY_UNKNOWN_0)) {
        CameraTargetObj *target = CameraLocalUtil::getTarget(this);


    }
}*/

void CameraManGame::updateSwim() {
    if (!setCubeChunk(CubeCameraArea::CATEGORY_UNKNOWN_1)) {
        CameraParamChunkID_Tmp chunkID = CameraParamChunkID_Tmp();
        chunkID.createOtherID(0, gDefaultWaterCameraName);

        setChunk(chunkID);
    }
}

void CameraManGame::updateWaterSurface() {
    if (!setCubeChunk(CubeCameraArea::CATEGORY_UNKNOWN_2)) {
        CameraParamChunkID_Tmp chunkID = CameraParamChunkID_Tmp();
        chunkID.createOtherID(0, gDefaultWaterSurfaceCameraName);

        setChunk(chunkID);
    }
}

void CameraManGame::updateGCapture() {
    CubeCameraArea::setCurrentCategory(CubeCameraArea::CATEGORY_UNKNOWN_3);
    CameraTargetObj *target = CameraLocalUtil::getTarget(this);
    const TVec3f *position = target->getPosition();

    CubeCameraArea *area = reinterpret_cast<CubeCameraArea *>(MR::getAreaObj("CubeCamera", *position));

    if (area != nullptr) {
        CameraParamChunkID_Tmp chunkID = CameraParamChunkID_Tmp();
        chunkID.createCubeID(area->mZoneID, static_cast<u16>(area->mObjArg0));

        setChunk(chunkID);
        CubeCameraArea::setCurrentCategory(CubeCameraArea::CATEGORY_UNKNOWN_0);
    }
    else {
        updateNormal();
    }
}

void CameraManGame::updateFooFighter() {
    if (!setCubeChunk(CubeCameraArea::CATEGORY_UNKNOWN_4)) {
        CameraParamChunkID_Tmp chunkID = CameraParamChunkID_Tmp();
        chunkID.createOtherID(0, gDefaultFooFighterCameraName);

        setChunk(chunkID);
    }
}

bool CameraManGame::setCubeChunk(CubeCameraArea::ECategory category) {
    CubeCameraArea::setCurrentCategory(static_cast<s32>(category));
    CameraTargetObj *target = CameraLocalUtil::getTarget(this);
    CubeCameraArea *area = target->getCubeCameraArea();

    if (area != nullptr) {
        CameraParamChunkID_Tmp chunkID = CameraParamChunkID_Tmp();
        chunkID.createCubeID(area->mZoneID, static_cast<u16>(area->mObjArg0));

        setChunk(chunkID);
        return true;
    }

    return false;
}

bool CameraManGame::tryStartPosCamera() {
    if (_6C == 0) {
        return false;
    }

    CameraParamChunkID_Tmp chunkID = CameraParamChunkID_Tmp();
    u16 startCameraID = static_cast<u16>(MR::getCurrentStartCameraID());
    s32 startZoneID = MR::getCurrentStartZoneID();

    chunkID.createStartID(startZoneID, startCameraID);
    setChunk(chunkID);

    if (_70 > 5) {
        _70 = 5;
    }

    if (_70 < 0) {
        _70 = 0;
    }

    if (_70 > 0) {
        mDirector->setInterpolation(0);
    }

    if (_70 > 0) {
        _70--;
    }

    return true;   
}

bool CameraManGame::tryZoomCamera() {
    if (!mZoomedIn == 0) {
        return false;
    }

    CameraParamChunkID_Tmp chunkID = CameraParamChunkID_Tmp();
    chunkID.createOtherID(0, "ズームカメラ");

    setChunk(chunkID);

    return true;
}

bool CameraManGame::isZoomCamera() const {
    CameraParamChunkID_Tmp chunkID = CameraParamChunkID_Tmp();
    chunkID.createOtherID(0, "ズームカメラ");

    CameraParamChunk *chunk = mChunkHolder->getChunk(chunkID);

    return mChunk == chunk;
}
