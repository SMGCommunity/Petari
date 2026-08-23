#include "Game/Camera/CameraManEvent.hpp"
#include "Game/Camera/CamTranslatorAnim.hpp"
#include "Game/Camera/Camera.hpp"
#include "Game/Camera/CameraDirector.hpp"
#include "Game/Camera/CameraHeightArrange.hpp"
#include "Game/Camera/CameraHolder.hpp"
#include "Game/Camera/CameraLocalUtil.hpp"
#include "Game/Camera/CameraParamChunk.hpp"
#include "Game/Camera/CameraParamChunkHolder.hpp"
#include "Game/Camera/CameraParamChunkID.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/MathUtil.hpp"

void CameraManEvent_FORCE_MATCH_SDATA2() {
    (void)1.0f;
}

namespace {
    static const f32 sMinDistance = 300.0f;
    static const s32 sDefaultBlendFrame = 60;
};  // namespace

CameraManEvent::CameraManEvent(CameraHolder* pHolder, CameraParamChunkHolder* pChunkHolder, const char* pName)
    : CameraMan(pName), mHolder(pHolder), mChunkHolder(pChunkHolder), mCamera(nullptr), mChunk(), _B8(), mRequestReset() {
    for (u32 i = 0; i < NR_FIFO_ITEMS; i++) {
        mItems[i].mFirst.mChunk = nullptr;
        mItems[i].mSecond.mChunk = nullptr;
    }
}

CameraManEvent::ChunkFIFOItem::ChunkFIFOItem() {
}

void CameraManEvent::init(const JMapInfoIter& rIter) {
}

void CameraManEvent::calc() {
    updateChunkFIFO();
    applyChunk();
    changeCamera();
    resetCameraIfRequested();
    CameraTargetObj* target = mCamera->calc();
    CameraLocalUtil::setUsedTarget(this, target);
    setSafePose();
}

void CameraManEvent::start(s32 zoneID, const char* pName, const CameraTargetArg& rTargetArg, s32 frame) {
    CameraParamChunkEvent* chunk = findChunk(zoneID, pName);
    requestChunk(chunk, static_cast< EPriority >(chunk->mEvPriority), rTargetArg, frame);
    rTargetArg.setTarget();
}

void CameraManEvent::end(s32 zoneID, const char* pName, s32 frame) {
    CameraParamChunkEvent* chunk = findChunk(zoneID, pName);
    searchPriority(chunk);

    if (cleanChunkFIFO(chunk)) {
        sendFinishInterpolateFrame(chunk, frame);
    }

    if (isChunkFIFOEmpty()) {
        mChunk = nullptr;
        MR::cleanEventCameraTarget_temporally();
    }
}

bool CameraManEvent::isEventActive(s32 zoneID, const char* pName) const {
    if (mCamera != nullptr) {
        CameraParamChunkID_Tmp chunkID = CameraParamChunkID_Tmp();
        chunkID.createEventID(zoneID, pName);

        CameraParamChunkEvent* chunk = reinterpret_cast< CameraParamChunkEvent* >(mChunkHolder->getChunk(chunkID));

        // Nice loop, probably they wanted i--
        for (s32 i = NR_FIFO_ITEMS - 1; i >= 0; i++) {
            if (mItems[i].mSecond.mChunk != nullptr) {
                return mItems[i].mSecond.mChunk == chunk;
            }

            if (mItems[i].mFirst.mChunk != nullptr) {
                return mItems[i].mFirst.mChunk == chunk;
            }
        }
    }

    return false;
}

bool CameraManEvent::isActive() const {
    return !isChunkFIFOEmpty();
}

bool CameraManEvent::isInterpolationOff() const {
    if (mCamera != nullptr && mCamera->isInterpolationOff()) {
        return true;
    }

    if (mChunk != nullptr && mChunk->isAntiBlurOff()) {
        return true;
    }

    return false;
}

bool CameraManEvent::isCollisionOff() const {
    if (mCamera != nullptr && mCamera->isCollisionOff()) {
        return true;
    }

    if (mChunk != nullptr && mChunk->isCollisionOff()) {
        return true;
    }

    return false;
}

bool CameraManEvent::isZeroFrameMoveOff() const {
    if (mCamera != nullptr) {
        return mCamera->isZeroFrameMoveOff();
    }

    return false;
}

bool CameraManEvent::doesNextChunkHaveInterpolation() const {
    const ChunkFIFOItem* item = nullptr;

    for (u32 i = 0; i < NR_FIFO_ITEMS; i++) {
        if (mItems[i].mSecond.mChunk != nullptr) {
            item = &mItems[i].mSecond;
            break;
        }

        if (mItems[i].mFirst.mChunk != nullptr) {
            break;
        }
    }

    if (item != nullptr && getInterpolateFrame(item->mChunk, item->mFrame) > 0) {
        return true;
    }

    return false;
}

bool CameraManEvent::isCorrectingErpPositionOff() const {
    return mCamera != nullptr && mCamera->isCorrectingErpPositionOff();
}

u32 CameraManEvent::getAnimCameraFrame(s32 zoneID, const char* pName) const {
    mHolder->getIndexOf("CAM_TYPE_ANIM");
    CameraParamChunkEvent* chunk = findChunk(zoneID, pName);

    return reinterpret_cast< CamTranslatorAnim* >(mHolder->getTranslator(chunk->mCameraTypeIndex))->getAnimFrame(chunk);
}

void CameraManEvent::pauseOnAnimCamera(s32 zoneID, const char* pName) {
    mHolder->getIndexOf("CAM_TYPE_ANIM");
    CameraParamChunkEvent* chunk = findChunk(zoneID, pName);

    if (chunk == mChunk && mCamera != nullptr) {
        reinterpret_cast< CameraAnim* >(mCamera)->mIsPaused = true;
    }
}

void CameraManEvent::pauseOffAnimCamera(s32 zoneID, const char* pName) {
    mHolder->getIndexOf("CAM_TYPE_ANIM");
    CameraParamChunkEvent* chunk = findChunk(zoneID, pName);

    if (chunk == mChunk && mCamera != nullptr) {
        reinterpret_cast< CameraAnim* >(mCamera)->mIsPaused = false;
    }
}

void CameraManEvent::updateChunkFIFO() {
    // FIXME: LWZ wrong instruction order, register mismatch
    // https://decomp.me/scratch/qHkVX

    for (u32 i = 0; i < NR_FIFO_ITEMS; i++) {
        CameraParamChunkEvent* chunk = mItems[i].mSecond.mChunk;

        if (chunk != nullptr) {
            mItems[i].mFirst.mChunk = chunk;

            CameraTargetMtx* mtx = mItems[i].mSecond.mTargetArg.mTargetMtx;
            CameraTargetObj* obj = mItems[i].mSecond.mTargetArg.mTargetObj;

            mItems[i].mFirst.mTargetArg.mTargetObj = obj;
            mItems[i].mFirst.mTargetArg.mTargetMtx = mtx;

            const LiveActor* liveActor = mItems[i].mSecond.mTargetArg.mLiveActor;
            MarioActor* marioActor = mItems[i].mSecond.mTargetArg.mMarioActor;

            mItems[i].mFirst.mTargetArg.mLiveActor = liveActor;
            mItems[i].mFirst.mTargetArg.mMarioActor = marioActor;

            mItems[i].mFirst.mFrame = mItems[i].mSecond.mFrame;

            mItems[i].mSecond.mChunk = nullptr;
        }
    }
}

void CameraManEvent::applyChunk() {
    ChunkFIFOItem* item = nullptr;

    for (u32 i = 0; i < NR_FIFO_ITEMS; i++) {
        if (mItems[i].mFirst.mChunk != nullptr) {
            item = &mItems[i].mFirst;
            break;
        }
    }

    checkReset(item);
    mChunk = item->mChunk;
}

void CameraManEvent::checkReset(ChunkFIFOItem* pItem) {
    CameraParamChunkEvent* itemChunk = pItem->mChunk;

    if (mChunk != itemChunk) {
        mRequestReset = true;
        sendStartInterpolateFrame(pItem->mChunk, pItem->mFrame);
        pItem->mTargetArg.setTarget();
    } else {
        u8 cameraIndex = itemChunk->mCameraTypeIndex;

        if (cameraIndex != mHolder->getIndexOf(mCamera)) {
            mRequestReset = true;
        }
    }
}

void CameraManEvent::changeCamera() {
    mCamera = mHolder->getCameraInner(mChunk->mCameraTypeIndex);
    CamTranslatorBase* translator = mHolder->getTranslator(mChunk->mCameraTypeIndex);
    translator->setParam(mChunk);
    setExtraParam();
    setVPanParam();
}

void CameraManEvent::setExtraParam() {
    s32 zoneID = mChunk->getZoneID();
    mCamera->setZoneMtx(zoneID);

    CameraLocalUtil::setGlobalOffset(mCamera, mChunk->mExParam.mWOffset);
    CameraLocalUtil::setLocalOffset(mCamera, CameraLocalUtil::getLocalOffset(this));
    CameraLocalUtil::setFrontOffset(mCamera, mChunk->mExParam.mLOffset);
    CameraLocalUtil::setUpperOffset(mCamera, mChunk->mExParam.mLOffsetV);

    if (mChunk->isOnUseFovy()) {
        CameraLocalUtil::setFovy(mCamera, mChunk->mExParam.mFovy);
    } else {
        CameraLocalUtil::setFovy(mCamera, mDirector->getDefaultFovy());
    }

    if (mChunk->isLOfsErpOff()) {
        mCamera->mIsLOfsErpOff = true;
    } else {
        mCamera->mIsLOfsErpOff = false;
    }

    CameraLocalUtil::setRoll(mCamera, mChunk->mExParam.mRoll);
}

void CameraManEvent::setVPanParam() {
    if (mCamera->doesVPanExist()) {
        CameraHeightArrange* vPan = mCamera->mVPan;
        vPan->resetParameter();

        vPan->mFocalScaleUpper = mChunk->mExParam.mUpper;
        vPan->mFocalScaleLower = mChunk->mExParam.mLower;
        vPan->mGndInt = mChunk->mExParam.mGndInt;
        vPan->mPosOffsetMinRiseLag = mChunk->mExParam.mUPlay;
        vPan->mPosOffsetMinDropLag = mChunk->mExParam.mLPlay;
        vPan->mRiseDelay = mChunk->mExParam.mPushDelay;
        vPan->mDropDelay = mChunk->mExParam.mPushDelayLow;
        vPan->mMaxRiseEaseTime = mChunk->mExParam.mUDown;
        vPan->mVPanUse = mChunk->mExParam.mVPanUse != 0;

        TVec3f axis;
        mChunk->getVPanAxis(&axis);

        vPan->mVPanAxis.set(axis);

        vPan->mUpdateGlobalAxis = true;
    }
}

void CameraManEvent::resetCameraIfRequested() {
    if (mRequestReset) {
        mCamera->mCameraMan = this;
        mCamera->reset();
        mRequestReset = false;
    }
}

void CameraManEvent::setSafePose() {
    TVec3f pos = CameraLocalUtil::getPos(mCamera);
    TVec3f watchPos = CameraLocalUtil::getWatchPos(mCamera);
    TVec3f up = CameraLocalUtil::getUpVec(mCamera);

    TVec3f watchOffset = watchPos - pos;
    f32 dist = watchOffset.length();
    if (dist < ::sMinDistance) {
        if (dist < 1.0f) {
            watchPos.set(pos + CameraLocalUtil::getWatchPos(this) - CameraLocalUtil::getPos(this));
        } else {
            watchOffset.normalize();
            watchPos.set(pos + watchOffset * ::sMinDistance);
        }
    }

    TVec3f camWatchDir = watchPos - pos;
    MR::normalize(&camWatchDir);
    MR::normalizeOrZero(&up);

    if (MR::isNearZero(up) || MR::abs(camWatchDir.dot(up)) > 0.98f) {
        TVec3f watchDir = CameraLocalUtil::getWatchPos(this) - CameraLocalUtil::getPos(this);
        MR::normalize(&watchDir);
        if (MR::abs(camWatchDir.dot(watchDir)) > 0.98f) {
            up.set(CameraLocalUtil::getUpVec(this));
        } else {
            TQuat4f rot;
            rot.setRotate(watchDir, camWatchDir);
            rot.transform(CameraLocalUtil::getUpVec(this), up);
        }
        CameraLocalUtil::recalcUpVec(&up, camWatchDir);
    }

    CameraLocalUtil::setPos(this, pos);
    CameraLocalUtil::setUpVec(this, up);
    CameraLocalUtil::setWatchPos(this, watchPos);
    CameraLocalUtil::setWatchUpVec(this, CameraLocalUtil::getWatchUpVec(mCamera));
    CameraLocalUtil::setGlobalOffset(this, CameraLocalUtil::getGlobalOffset(mCamera));
    CameraLocalUtil::setLocalOffset(this, CameraLocalUtil::getLocalOffset(mCamera));
    CameraLocalUtil::setFovy(this, CameraLocalUtil::getFovy(mCamera));
    CameraLocalUtil::setRoll(this, CameraLocalUtil::getRoll(mCamera));
}

CameraParamChunkEvent* CameraManEvent::findChunk(s32 zoneID, const char* pName) const {
    CameraParamChunkID_Tmp chunkID = CameraParamChunkID_Tmp();
    chunkID.createEventID(zoneID, pName);

    return reinterpret_cast< CameraParamChunkEvent* >(mChunkHolder->getChunk(chunkID));
}

void CameraManEvent::requestChunk(CameraParamChunkEvent* pChunk, EPriority priority, const CameraTargetArg& rArg, s32 frame) {
    mItems[priority].mSecond.mChunk = pChunk;
    mItems[priority].mSecond.mTargetArg.mTargetObj = rArg.mTargetObj;
    mItems[priority].mSecond.mTargetArg.mTargetMtx = rArg.mTargetMtx;
    mItems[priority].mSecond.mTargetArg.mLiveActor = rArg.mLiveActor;
    mItems[priority].mSecond.mTargetArg.mMarioActor = rArg.mMarioActor;
    mItems[priority].mSecond.mFrame = frame;
}

bool CameraManEvent::cleanChunkFIFO(CameraParamChunk* pChunk) {
    s32 nr = 0;

    for (u32 i = 0; i < NR_FIFO_ITEMS; i++) {
        if (mItems[i].mSecond.mChunk != nullptr && mItems[i].mSecond.mChunk == pChunk) {
            mItems[i].mFirst.mChunk = nullptr;
            mItems[i].mSecond.mChunk = nullptr;
            break;
        }

        if (mItems[i].mFirst.mChunk != nullptr && mItems[i].mFirst.mChunk == pChunk) {
            mItems[i].mFirst.mChunk = nullptr;
            break;
        }

        if (mItems[i].mFirst.mChunk != nullptr || mItems[i].mSecond.mChunk != nullptr) {
            nr++;
        }
    }

    return nr == 0;
}

bool CameraManEvent::isChunkFIFOEmpty() const {
    for (u32 i = 0; i < NR_FIFO_ITEMS; i++) {
        if (mItems[i].mFirst.mChunk != nullptr) {
            return false;
        }

        if (mItems[i].mSecond.mChunk != nullptr) {
            return false;
        }
    }

    return true;
}

void CameraManEvent::sendStartInterpolateFrame(CameraParamChunkEvent* pChunk, s32 frame) {
    u32 frames = getInterpolateFrame(pChunk, frame);

    if (frames == 0) {
        mRequestLOfsReset = true;
    }

    mDirector->setInterpolation(frames);
}

s32 CameraManEvent::getInterpolateFrame(CameraParamChunkEvent* pChunk, s32 frame) const {
    s32 frames = -1;

    if (pChunk->mEnableErpFrame != 0) {
        frames = pChunk->mExParam.mCamInt;
    }

    if (frames < 0 && frame >= 0) {
        frames = frame;
    }

    if (frames < 0) {
        frames = ::sDefaultBlendFrame;
    }

    return frames;
}

void CameraManEvent::sendFinishInterpolateFrame(CameraParamChunkEvent* pChunk, s32 frame) {
    s32 frames = -1;

    if (pChunk->mEnableEndErpFrame) {
        frames = pChunk->mCamEndInt;
    } else if (pChunk->mEnableErpFrame) {
        frames = pChunk->mExParam.mCamInt;
    }

    if (frames < 0 && frame >= 0) {
        frames = frame;
    }

    if (frames < 0) {
        frames = ::sDefaultBlendFrame;
    }

    mDirector->setInterpolation(frames);
}

s32 CameraManEvent::searchPriority(CameraParamChunk* pChunk) const {
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

bool CameraManEvent::isInFIFO(CameraParamChunk* pChunk) const {
    for (u32 i = 0; i < NR_FIFO_ITEMS; i++) {
        if (mItems[i].mFirst.mChunk == pChunk || mItems[i].mSecond.mChunk == pChunk) {
            return true;
        }
    }

    return false;
}

void CameraManEvent::notifyActivate() {
    mRequestReset = true;
}

void CameraManEvent::notifyDeactivate() {
    mCamera = nullptr;
}

CameraManEvent::~CameraManEvent() {
}

bool CameraManEvent::isAnimCameraEnd(s32 zoneID, const char* pName) const {
    mHolder->getIndexOf("CAM_TYPE_ANIM");
    CameraParamChunkEvent* chunk = findChunk(zoneID, pName);

    if (isInFIFO(chunk)) {
        if (chunk == mChunk) {
            if (mCamera != nullptr) {
                return reinterpret_cast< CameraAnim* >(mCamera)->isAnimEnd();
            }
        } else {
            return false;
        }
    }

    return true;
}
