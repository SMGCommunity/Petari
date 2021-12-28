#include "Game/Camera/Camera.h"
#include "Game/Camera/CamTranslatorAnim.h"
#include "Game/Camera/CameraDirector.h"
#include "Game/Camera/CameraHeightArrange.h"
#include "Game/Camera/CameraLocalUtil.h"
#include "Game/Camera/CameraManEvent.h"
#include "Game/Camera/CameraHolder.h"
#include "Game/Camera/CameraParamChunk.h"
#include "Game/Camera/CameraParamChunkHolder.h"

#ifdef NON_MATCHING
// Constructor for mTargetArg is called. The constructor exists in the symbol so 
// it's not fully inlined. It also exists in the code (not inlined), but the code below
// matches the constructor exactly.
CameraManEvent::ChunkFIFOItem::ChunkFIFOItem() {
    mTargetArg.mTargetObj = NULL;
    mTargetArg.mTargetMtx = NULL;
    mTargetArg.mLiveActor = NULL;
    mTargetArg.mMarioActor = NULL;
}
#endif

CameraManEvent::CameraManEvent(CameraHolder *pHolder, CameraParamChunkHolder *pChunkHolder, const char *pName) :
    CameraMan(pName), mHolder(pHolder), mChunkHolder(pChunkHolder), mCamera(NULL) {
    mChunk = NULL;
    _B8 = 0;
    _BC = false;

    for (u32 i = 0; i < NR_FIFO_ITEMS; i++) {
        mItems[i].mFirst.mChunk = NULL;
        mItems[i].mSecond.mChunk = NULL;
    }
}

CameraManEvent::~CameraManEvent() {

}

void CameraManEvent::init(const JMapInfoIter &rIter) {

}

void CameraManEvent::calc() {
    updateChunkFIFO();
    applyChunk();
    changeCamera();
    resetCameraIfRequested();
    CameraTargetObj *target = mCamera->calc();
    CameraLocalUtil::setUsedTarget(this, target);
    setSafePose();
}

void CameraManEvent::notifyActivate() {
    _BC = true;
}

void CameraManEvent::notifyDeactivate() {
    mCamera = NULL;
}

bool CameraManEvent::isInterpolationOff() const {
    if (mCamera != NULL && mCamera->isInterpolationOff()) {
        return true;
    }

    if (mChunk != NULL && mChunk->isAntiBlurOff()) {
        return true;
    }

    return false;
}

bool CameraManEvent::isCollisionOff() const {
    if (mCamera != NULL && mCamera->isCollisionOff()) {
        return true;
    }

    if (mChunk != NULL && mChunk->isCollisionOff()) {
        return true;
    }

    return false;
}

bool CameraManEvent::isZeroFrameMoveOff() const {
    if (mCamera != NULL) {
        return mCamera->isZeroFrameMoveOff();
    }

    return false;
}

bool CameraManEvent::isCorrectingErpPositionOff() const {
    bool off = false;

    if (mCamera != NULL && mCamera->isCorrectingErpPositionOff()) {
        off = true;
    }

    return off;
}

void CameraManEvent::start(long zoneID, const char *pName, const CameraTargetArg &rTargetArg, long a4) {
    CameraParamChunkEvent *chunk = findChunk(zoneID, pName);
    requestChunk(chunk, static_cast<EPriority>(chunk->mEvPriority), rTargetArg, a4);
    rTargetArg.setTarget();
}

void CameraManEvent::end(long zoneID, const char *pName, long a3) {
    CameraParamChunkEvent *chunk = findChunk(zoneID, pName);
    searchPriority(chunk);

    if (cleanChunkFIFO(chunk)) {
        sendFinishInterpolateFrame(chunk, a3);
    }

    if (isChunkFIFOEmpty()) {
        mChunk = NULL;
        MR::cleanEventCameraTarget_temporally();
    }
}

bool CameraManEvent::isEventActive(long zoneID, const char *pName) const {
    if (mCamera != NULL) {
        CameraParamChunkID_Tmp chunkID = CameraParamChunkID_Tmp();
        chunkID.createEventID(zoneID, pName);

        CameraParamChunkEvent *chunk = reinterpret_cast<CameraParamChunkEvent *>(mChunkHolder->getChunk(chunkID));

        // Nice loop, probably they wanted i--
        for (s32 i = NR_FIFO_ITEMS - 1; i >= 0; i++) {
            if (mItems[i].mSecond.mChunk != NULL) {
                return mItems[i].mSecond.mChunk == chunk;
            }

            if (mItems[i].mFirst.mChunk != NULL) {
                return mItems[i].mFirst.mChunk == chunk;
            }
        }
    }

    return false;
}

bool CameraManEvent::isActive() const {
    return !isChunkFIFOEmpty();
}

bool CameraManEvent::doesNextChunkHaveInterpolation() const {
    const ChunkFIFOItem *item = NULL;

    for (u32 i = 0; i < NR_FIFO_ITEMS; i++) {
        if (mItems[i].mSecond.mChunk != NULL) {
            item = &mItems[i].mSecond;
            break;
        }
        
        if (mItems[i].mFirst.mChunk != NULL) {
            break;
        }
    }

    if (item != NULL && getInterpolateFrame(item->mChunk, item->_14) > 0) {
        return true;
    }

    return false;
}

u32 CameraManEvent::getAnimCameraFrame(long zoneID, const char *pName) const {
    mHolder->getIndexOf("CAM_TYPE_ANIM");
    CameraParamChunkEvent *chunk = findChunk(zoneID, pName);
    CamTranslatorAnim *translator = reinterpret_cast<CamTranslatorAnim *>(mHolder->getTranslator(chunk->mCameraTypeIndex));

    return translator->getAnimFrame(chunk);
}

void CameraManEvent::pauseOnAnimCamera(long zoneID, const char *pName) {
    mHolder->getIndexOf("CAM_TYPE_ANIM");
    CameraParamChunkEvent *chunk = findChunk(zoneID, pName);

    if (chunk == mChunk && mCamera != NULL) {
        CameraAnim *animCamera = reinterpret_cast<CameraAnim *>(mCamera);
        animCamera->_7C = 1;
    }
}

void CameraManEvent::pauseOffAnimCamera(long zoneID, const char *pName) {
    mHolder->getIndexOf("CAM_TYPE_ANIM");
    CameraParamChunkEvent *chunk = findChunk(zoneID, pName);

    if (chunk == mChunk && mCamera != NULL) {
        CameraAnim *animCamera = reinterpret_cast<CameraAnim *>(mCamera);
        animCamera->_7C = 0;
    }
}

#ifdef NON_MATCH
// LWZ wrong instruction order, register mismatch
void CameraManEvent::updateChunkFIFO() {
    for (u32 i = 0; i < NR_FIFO_ITEMS; i++) {
        CameraParamChunkEvent *chunk = mItems[i].mSecond.mChunk;

        if (chunk != NULL) {
            mItems[i].mFirst.mChunk = chunk;

            CameraTargetMtx *mtx = mItems[i].mSecond.mTargetArg.mTargetMtx;
            CameraTargetObj *obj = mItems[i].mSecond.mTargetArg.mTargetObj;

            mItems[i].mFirst.mTargetArg.mTargetObj = obj;
            mItems[i].mFirst.mTargetArg.mTargetMtx = mtx;

            const LiveActor *liveActor = mItems[i].mSecond.mTargetArg.mLiveActor;
            MarioActor *marioActor = mItems[i].mSecond.mTargetArg.mMarioActor;

            mItems[i].mFirst.mTargetArg.mLiveActor = liveActor;
            mItems[i].mFirst.mTargetArg.mMarioActor = marioActor;

            mItems[i].mFirst._14 = mItems[i].mSecond._14;

            mItems[i].mSecond.mChunk = NULL;
        }
    }
}
#endif

void CameraManEvent::applyChunk() {
    ChunkFIFOItem *item = NULL;

    for (u32 i = 0; i < NR_FIFO_ITEMS; i++) {
        if (mItems[i].mFirst.mChunk != NULL) {
            item = &mItems[i].mFirst;
            break;
        }
    }

    checkReset(item);
    mChunk = item->mChunk;
}

void CameraManEvent::checkReset(ChunkFIFOItem *pItem) {
    CameraParamChunkEvent *itemChunk = pItem->mChunk;

    if (mChunk != itemChunk)  {
        _BC = true;
        sendStartInterpolateFrame(pItem->mChunk, pItem->_14);
        pItem->mTargetArg.setTarget();
    }
    else {
        u8 cameraIndex = itemChunk->mCameraTypeIndex;

        if (cameraIndex != mHolder->getIndexOf(mCamera)) {
            _BC = true;
        }
    }
}

void CameraManEvent::changeCamera() {
    mCamera = mHolder->getCameraInner(mChunk->mCameraTypeIndex);
    CamTranslatorDummy *translator = mHolder->getTranslator(mChunk->mCameraTypeIndex);
    translator->setParam(mChunk);
    setExtraParam();
    setVPanParam();
}

void CameraManEvent::setExtraParam() {
    s32 zoneID = mChunk->getZoneID();
    mCamera->setZoneMtx(zoneID);

    CameraLocalUtil::setGlobalOffset(mCamera, mChunk->mExParam.mWOffset);

    TVec3f *localOffset = CameraLocalUtil::getLocalOffset(this);
    CameraLocalUtil::setLocalOffset(mCamera, *localOffset);

    CameraLocalUtil::setFrontOffset(mCamera, mChunk->mExParam.mLOffset);
    CameraLocalUtil::setUpperOffset(mCamera, mChunk->mExParam.mLOffsetV);

    if (mChunk->isOnUseFovy()) {
        CameraLocalUtil::setFovy(mCamera, mChunk->mExParam.mFovy);
    }
    else {
        f32 fovy = mDirector->getDefaultFovy();
        CameraLocalUtil::setFovy(mCamera, fovy);
    }

    if (mChunk->isLOfsErpOff()) {
        mCamera->mIsLOfsErpOff = true;
    }
    else {
        mCamera->mIsLOfsErpOff = false;
    }

    CameraLocalUtil::setRoll(mCamera, mChunk->mExParam.mRoll);
}

#ifdef NON_MATCHING
// r0 nonsense
void CameraManEvent::setVPanParam() {
    if (mCamera->mVPan != NULL) {
        CameraHeightArrange *vPan = mCamera->mVPan;
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

void CameraManEvent::resetCameraIfRequested() {
    if (_BC) {
        mCamera->mCameraMan = this;
        mCamera->reset();
        _BC = false;
    }
}

/*void CameraManEvent::setSafePose() {
    TVec3f pos = TVec3f(*CameraLocalUtil::getPos(mCamera));
    TVec3f watchPos = TVec3f(*CameraLocalUtil::getWatchPos(mCamera));
    TVec3f up = TVec3f(*CameraLocalUtil::getUpVec(mCamera));

    TVec3f dir = watchPos - pos;
    
    float length = PSVECMag(reinterpret_cast<Vec *>(&dir));

    if (length < 300.0f) {
        if (length < 1.0f) {

        }
    }
}*/

CameraParamChunkEvent *CameraManEvent::findChunk(long zoneID, const char *pName) const {
    CameraParamChunkID_Tmp chunkID = CameraParamChunkID_Tmp();
    chunkID.createEventID(zoneID, pName);

    return reinterpret_cast<CameraParamChunkEvent *>(mChunkHolder->getChunk(chunkID));
}

void CameraManEvent::requestChunk(CameraParamChunkEvent *pChunk, EPriority priority, const CameraTargetArg &rArg, long a4) {
    mItems[priority].mSecond.mChunk = pChunk;
    mItems[priority].mSecond.mTargetArg.mTargetObj = rArg.mTargetObj;
    mItems[priority].mSecond.mTargetArg.mTargetMtx = rArg.mTargetMtx;
    mItems[priority].mSecond.mTargetArg.mLiveActor = rArg.mLiveActor;
    mItems[priority].mSecond.mTargetArg.mMarioActor = rArg.mMarioActor;
    mItems[priority].mSecond._14 = a4;
}

bool CameraManEvent::cleanChunkFIFO(CameraParamChunk *pChunk) {
    s32 nr = 0;

    for (u32 i = 0; i < NR_FIFO_ITEMS; i++) {
        if (mItems[i].mSecond.mChunk != NULL && mItems[i].mSecond.mChunk == pChunk) {
            mItems[i].mFirst.mChunk = NULL;
            mItems[i].mSecond.mChunk = NULL;
            break;
        }

        if (mItems[i].mFirst.mChunk != NULL && mItems[i].mFirst.mChunk == pChunk) {
            mItems[i].mFirst.mChunk = NULL;
            break;
        }

        if (mItems[i].mFirst.mChunk != NULL || mItems[i].mSecond.mChunk != NULL) {
            nr++;
        }
    }

    return nr == 0;
}

bool CameraManEvent::isChunkFIFOEmpty() const {
    for (u32 i = 0; i < NR_FIFO_ITEMS; i++) {
        if (mItems[i].mFirst.mChunk != NULL) {
            return false;
        }

        if (mItems[i].mSecond.mChunk != NULL) {
            return false;
        }
    }

    return true;
}

void CameraManEvent::sendStartInterpolateFrame(CameraParamChunkEvent *pChunk, long a2) {
    u32 frames = getInterpolateFrame(pChunk, a2); 

    if (frames == 0) {
        _15 = 1;
    }

    mDirector->setInterpolation(frames);
}

s32 CameraManEvent::getInterpolateFrame(CameraParamChunkEvent *pChunk, long a2) const {
    s32 frames = -1;

    if (pChunk->mEnableErpFrame != 0) {
        frames = pChunk->mExParam.mCamInt;
    }
    
    if (frames < 0 && a2 >= 0) {
        frames = a2;
    }
    
    if (frames < 0) {
        frames = 60;
    }
    
    return frames;
}

void CameraManEvent::sendFinishInterpolateFrame(CameraParamChunkEvent *pChunk, long a2) {
    s32 frames = -1;

    if (pChunk->mEnableEndErpFrame) {
        frames = pChunk->mCamEndInt;
    }
    else if (pChunk->mEnableErpFrame) {
        frames = pChunk->mExParam.mCamInt;
    }
    
    if (frames < 0 && a2 >= 0) {
        frames = a2;
    }
    
    if (frames < 0) {
        frames = 60;
    }

    mDirector->setInterpolation(frames);
}

s32 CameraManEvent::searchPriority(CameraParamChunk *pChunk) const {
    s32 i;

    for (i = 0; i < NR_FIFO_ITEMS; i++) {
        if (mItems[i].mFirst.mChunk == pChunk) {
            break;
        }

        if (mItems[i].mSecond.mChunk == pChunk) {
            break;
        }
    }

    return i;
}

bool CameraManEvent::isInFIFO(CameraParamChunk *pChunk) const {
    for (u32 i = 0; i < NR_FIFO_ITEMS; i++) {
        if (mItems[i].mFirst.mChunk == pChunk || mItems[i].mSecond.mChunk == pChunk) {
            return true;
        }
    }

    return false;
}

bool CameraManEvent::isAnimCameraEnd(long zoneID, const char *pName) const {
    mHolder->getIndexOf("CAM_TYPE_ANIM");
    CameraParamChunkEvent *chunk = findChunk(zoneID, pName);

    if (isInFIFO(chunk)) {
        if (chunk == mChunk && mCamera != NULL) {
            return reinterpret_cast<CameraAnim *>(mCamera)->isAnimEnd();
        }
        else {
            return false;
        }
    }

    return true;
}
