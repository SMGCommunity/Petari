#include "Game/Camera/CameraAnim.hpp"
#include "Game/Camera/CamTranslatorAnim.hpp"
#include "Game/Camera/CameraLocalUtil.hpp"
#include "Game/Camera/CameraTargetObj.hpp"
#include "Game/Util/MathUtil.hpp"

void CameraAnim_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)MR::pi();
}

void CamAnmDataAccessor::set(void* pInfo, void* pValues) {
    mInfo = reinterpret_cast< CanmFrameInfo* >(pInfo);
    mValues = reinterpret_cast< f32* >(pValues);
}

void CamAnmDataAccessor::getPos(TVec3f* pPos, f32 key) const {
    pPos->set< f32 >(get(key, mInfo->mPosX.mOffset, mInfo->mPosX.mCount),  //
                     get(key, mInfo->mPosY.mOffset, mInfo->mPosY.mCount),  //
                     get(key, mInfo->mPosZ.mOffset, mInfo->mPosZ.mCount));
}

void CamAnmDataAccessor::getWatchPos(TVec3f* pWatchPos, f32 key) const {
    pWatchPos->set< f32 >(get(key, mInfo->mWatchPosX.mOffset, mInfo->mWatchPosX.mCount),
                          get(key, mInfo->mWatchPosY.mOffset, mInfo->mWatchPosY.mCount),
                          get(key, mInfo->mWatchPosZ.mOffset, mInfo->mWatchPosZ.mCount));
}

f32 CamAnmDataAccessor::getTwist(f32 key) const {
    CamnFrameComponentInfo& info = mInfo->mTwist;

    return get(key, info.mOffset, info.mCount);
}

f32 CamAnmDataAccessor::getFovy(f32 key) const {
    CamnFrameComponentInfo& info = mInfo->mFovy;

    return get(key, info.mOffset, info.mCount);
}

f32 CamAnmDataAccessor::get(f32 key, u32 offset, u32 count) const {
    // FIXME: regswap
    // https://decomp.me/scratch/7ESHr

    u32 keyFloor = key;

    f32 rate = MR::clamp(key - keyFloor, 0.0f, 1.0f);

    if (keyFloor < count - 1) {
        return (1.0f - rate) * mValues[offset + keyFloor] + rate * mValues[offset + keyFloor + 1];
    } else {
        s32 idx = offset + count;
        return getValue(idx - 1);
    }
}

void KeyCamAnmDataAccessor::set(void* pInfo, void* pValues) {
    mInfo = reinterpret_cast< CanmKeyFrameInfo* >(pInfo);
    mValues = reinterpret_cast< f32* >(pValues);
}

void KeyCamAnmDataAccessor::getPos(TVec3f* pPos, f32 key) const {
    pPos->set< f32 >(get(key, mInfo->mPosX.mOffset, mInfo->mPosX.mCount, mInfo->mPosX.mType),
                     get(key, mInfo->mPosY.mOffset, mInfo->mPosY.mCount, mInfo->mPosY.mType),
                     get(key, mInfo->mPosZ.mOffset, mInfo->mPosZ.mCount, mInfo->mPosZ.mType));
}

void KeyCamAnmDataAccessor::getWatchPos(TVec3f* pWatchPos, f32 key) const {
    pWatchPos->set< f32 >(get(key, mInfo->mWatchPosX.mOffset, mInfo->mWatchPosX.mCount, mInfo->mWatchPosX.mType),
                          get(key, mInfo->mWatchPosY.mOffset, mInfo->mWatchPosY.mCount, mInfo->mWatchPosY.mType),
                          get(key, mInfo->mWatchPosZ.mOffset, mInfo->mWatchPosZ.mCount, mInfo->mWatchPosZ.mType));
}

f32 KeyCamAnmDataAccessor::getTwist(f32 key) const {
    CanmKeyFrameComponentInfo& info = mInfo->mTwist;

    return get(key, info.mOffset, info.mCount, info.mType);
}

f32 KeyCamAnmDataAccessor::getFovy(f32 key) const {
    CanmKeyFrameComponentInfo& info = mInfo->mFovy;

    return get(key, info.mOffset, info.mCount, info.mType);
}

f32 KeyCamAnmDataAccessor::get(f32 key, u32 offset, u32 count, u32 type) const {
    if (count == 1) {
        return mValues[offset];
    }

    if (type == 0) {
        return get3f(key, offset, count);
    } else {
        return get4f(key, offset, count);
    }
}

u32 KeyCamAnmDataAccessor::searchKeyFrameIndex(f32 key, u32 offset, u32 count, u32 stride) const {
    u32 low = 0;
    u32 high = count;

    while (low < high) {
        u32 middle = (low + high) / 2;

        if (mValues[offset + middle * stride] <= key) {
            low = middle + 1;
        } else {
            high = middle;
        }
    }

    return low - 1;
}

f32 KeyCamAnmDataAccessor::get3f(f32 key, u32 offset, u32 count) const {
    u32 index = searchKeyFrameIndex(key, offset, count, 3);
    f32* values = mValues + offset + index * 3;

    return calcHermite(key, values[0], values[1], values[2], values[3], values[4], values[5]);
}

f32 KeyCamAnmDataAccessor::get4f(f32 key, u32 offset, u32 count) const {
    u32 index = searchKeyFrameIndex(key, offset, count, 4);
    f32* values = mValues + offset + index * 4;

    return calcHermite(key, values[0], values[1], values[3], values[4], values[5], values[6]);
}

f32 KeyCamAnmDataAccessor::calcHermite(f32 key, f32 tA, f32 pA, f32 mA, f32 tB, f32 pB, f32 mB) const {
    return JMAHermiteInterpolation(key, tA, pA, mA / 30.0f, tB, pB, mB / 30.0f);
}

CameraAnim::CameraAnim(const char* pName)
    : Camera(pName), _4C(), _50(1), mNrFrames(), mIsKey(), mSpeed(1.0f), mFileDataAccessor(), mDataAccessor(new CamAnmDataAccessor()),
      mKeyDataAccessor(new KeyCamAnmDataAccessor()), mNrValues(), _74(), mFileData(), mIsPaused() {
}

void CameraAnim::reset() {
    CameraLocalUtil::setFovy(this, CameraLocalUtil::getFovy(mCameraMan));
    CameraLocalUtil::setWatchPos(this, CameraLocalUtil::getWatchPos(mCameraMan));
    CameraLocalUtil::setPos(this, CameraLocalUtil::getPos(mCameraMan));
    CameraLocalUtil::setUpVec(this, CameraLocalUtil::getUpVec(mCameraMan));
    mIsPaused = false;
    mCurrentFrame = 0.0f;
}

CameraTargetObj* CameraAnim::calc() {
    // FIXME: out of order load
    // https://decomp.me/scratch/1E1xy

    TPos3f mtx;
    mtx.identity();
    mtx.setXDir(CameraLocalUtil::getTarget(this)->getSideVec());
    mtx.setYDir(CameraLocalUtil::getTarget(this)->getUpVec());
    mtx.setZDir(CameraLocalUtil::getTarget(this)->getFrontVec());
    mtx.setTrans(CameraLocalUtil::getTarget(this)->getPosition());

    if (mNrFrames != 0 && mNrFrames > mCurrentFrame) {
        TVec3f pos;
        mFileDataAccessor->getPos(&pos, mCurrentFrame);
        mtx.mult(pos, pos);
        CameraLocalUtil::setPos(this, pos);

        TVec3f watchPos;
        mFileDataAccessor->getWatchPos(&watchPos, mCurrentFrame);
        mtx.mult(watchPos, watchPos);
        CameraLocalUtil::setWatchPos(this, watchPos);

        TVec3f up;
        mtx.getYDir(up);
        CameraLocalUtil::setUpVec(this, up);
        CameraLocalUtil::setWatchUpVec(this, up);

        CameraLocalUtil::setRoll(this, mFileDataAccessor->getTwist(mCurrentFrame) * MR::pi() / 180.0f);
        CameraLocalUtil::setFovy(this, mFileDataAccessor->getFovy(mCurrentFrame));

        if (!mIsPaused) {
            mCurrentFrame += mSpeed;
        }
    } else {
        // FIXME: out of order
        CameraLocalUtil::setRoll(this, mFileDataAccessor->getTwist(mNrFrames - 1) * MR::pi() / 180.0f);
        CameraLocalUtil::setFovy(this, mFileDataAccessor->getFovy(mNrFrames - 1));
    }

    return nullptr;
}

void CameraAnim::setParam(u8* pFile, f32 speed) {
    loadBin(pFile);
    mFileData = pFile;
    mSpeed = speed;
}

bool CameraAnim::isAnimEnd() const {
    return mNrFrames == 0 || mCurrentFrame >= mNrFrames;
}

u32 CameraAnim::getAnimFrame(u8* pFile) {
    if (pFile == nullptr) {
        return 0;
    }

    CanmFileHeader* pHeader = reinterpret_cast< CanmFileHeader* >(pFile);

    if (pHeader->mMagic[0] != 'A' || pHeader->mMagic[1] != 'N' || pHeader->mMagic[2] != 'D' || pHeader->mMagic[3] != 'O') {
        return 0;
    }

    if ((pHeader->mType[0] != 'C' || pHeader->mType[1] != 'A' || pHeader->mType[2] != 'N' || pHeader->mType[3] != 'M') &&
        (pHeader->mType[0] != 'C' || pHeader->mType[1] != 'K' || pHeader->mType[2] != 'A' || pHeader->mType[3] != 'N')) {
        return 0;
    }

    if (pHeader->_8 == 0) {
        return 0;
    }

    return pHeader->mNrFrames;
}

CamTranslatorBase* CameraAnim::createTranslator() {
    return new CamTranslatorAnim(this);
}

bool CameraAnim::loadBin(u8* pFile) {
    CanmFileHeader* pHeader = reinterpret_cast< CanmFileHeader* >(pFile);

    if (pHeader->mMagic[0] != 'A' || pHeader->mMagic[1] != 'N' || pHeader->mMagic[2] != 'D' || pHeader->mMagic[3] != 'O') {
        return false;
    }

    if (pHeader->mType[0] == 'C' && pHeader->mType[1] == 'A' && pHeader->mType[2] == 'N' && pHeader->mType[3] == 'M') {
        mIsKey = false;
        mFileDataAccessor = mDataAccessor;
    } else if (pHeader->mType[0] == 'C' && pHeader->mType[1] == 'K' && pHeader->mType[2] == 'A' && pHeader->mType[3] == 'N') {
        mIsKey = true;
        mFileDataAccessor = mKeyDataAccessor;
    } else {
        return false;
    }

    if (pHeader->_8 == 0) {
        return false;
    }

    u8* pEntry = reinterpret_cast< u8* >(pFile + sizeof(CanmFileHeader));
    _4C = pHeader->_C;
    _50 = pHeader->_10;
    mNrFrames = pHeader->mNrFrames;

    u32 valueOffset = pHeader->mValueOffset;

    mNrValues = *(reinterpret_cast< u32* >(&pEntry[valueOffset])) / sizeof(f32);

    mFileDataAccessor->set(pEntry, pEntry + valueOffset + 4);

    return true;
}
