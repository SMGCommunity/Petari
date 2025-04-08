#include "Game/Screen/THPSimplePlayerWrapper.hpp"
#include "Game/Util.hpp"
#include "revolution/os.h"
#include "revolution/os/OSInterrupt.h"
#include "revolution/thp.h"

static s32 WorkBuffer[16] __attribute__((aligned(32)));

namespace NrvTHPSimplePlayerWrapper {
    NEW_NERVE(HostTypeWait, THPSimplePlayerWrapper, Wait);
    NEW_NERVE(HostTypeReadHeader, THPSimplePlayerWrapper, ReadHeader);
    NEW_NERVE(HostTypeReadFrameComp, THPSimplePlayerWrapper, ReadFrameComp);
    NEW_NERVE(HostTypeReadVideoComp, THPSimplePlayerWrapper, ReadVideoComp);
    NEW_NERVE(HostTypeReadAudioComp, THPSimplePlayerWrapper, ReadAudioComp);
    NEW_NERVE(HostTypeReadPreLoad, THPSimplePlayerWrapper, ReadPreLoad);
};

u32 THPSimplePlayerWrapper::calcNeedMemory() {
    if (!mOpen) {
        return 0;
    }

    u32 size = OSRoundUp32B(mHeader.bufSize) * 20;
    size += OSRoundUp32B(mVideoInfo.xSize * mVideoInfo.ySize);
    size += OSRoundUp32B(mVideoInfo.xSize * mVideoInfo.ySize / 4);
    size += OSRoundUp32B(mVideoInfo.xSize * mVideoInfo.ySize / 4);

    if (mAudioExist) {
        size += (OSRoundUp32B(mHeader.audioMaxSamples * 4) * 20);
    }

    size += 0x1000;
    return size;
}

bool THPSimplePlayerWrapper::setBuffer(u8* pBuffer) {
    if (mOpen && !mPreFetchState) {
        if (mLoop == 1) {
            return false;
        }

        u32 ysize = OSRoundUp32B(mVideoInfo.xSize * mVideoInfo.ySize);
        u32 uvsize = OSRoundUp32B(mVideoInfo.xSize * mVideoInfo.ySize / 4);
        mTextureSet[0].ytexture = pBuffer;
        DCInvalidateRange(pBuffer, ysize);
        pBuffer += ysize;
        mTextureSet[0].utexture = pBuffer;
        DCInvalidateRange(pBuffer, uvsize);
        pBuffer += uvsize;
        mTextureSet[0].vtexture = pBuffer;
        DCInvalidateRange(pBuffer, uvsize);
        pBuffer += uvsize;
        mTextureSet[1].ytexture = pBuffer;
        DCInvalidateRange(pBuffer, ysize);
        pBuffer += ysize;
        mTextureSet[1].utexture = pBuffer;
        DCInvalidateRange(pBuffer, uvsize);
        pBuffer += uvsize;
        mTextureSet[1].vtexture = pBuffer;
        DCInvalidateRange(pBuffer, uvsize);
        pBuffer += uvsize;

        for (s32 i = 0; i < 20; i++) {
            mReadBuffer[i].ptr = pBuffer;
            pBuffer += OSRoundUp32B(mHeader.bufSize);
            mReadBuffer[i].isValid = 0;
        }

        if (mAudioExist) {
            for (s32 i = 0; i < 20; i++) {
                mAudioBuffer[i].buffer = (s16*)pBuffer;
                mAudioBuffer[i].curPtr = (s16*)pBuffer;
                mAudioBuffer[i].validSample = 0;
                pBuffer += OSRoundUp32B(mHeader.audioMaxSamples * 4);
            }
        }

        mTHPWork = (void*)pBuffer;
    }

    return true;
}

bool THPSimplePlayerWrapper::preLoad(s32 loop) {
    if (!mOpen || mPreFetchState) {
        return false;
    }

    mLoop = loop;
    _314 = 20;
    if (!mLoop) {
        if (mHeader.numFrames < 0x14) {
            _314 = mHeader.numFrames;
        }
    }

    setNerve(&NrvTHPSimplePlayerWrapper::HostTypeReadPreLoad::sInstance);
    return true;
}

bool THPSimplePlayerWrapper::loadStop() {
    if (mOpen && !mAudioState) {
        mPreFetchState = 0;

        if (mReadProgress) {
            DVDCancel(&mFileInfo.cb);
            mReadProgress = 0;
        }
    
        for (s32 i = 0; i < 0x14; i++) {
            mReadBuffer[i].isValid = 0;
        }
    
        for (s32 i = 0 ; i < 0x14; i++) {
            mAudioBuffer[i].validSample = 0;
        }
    
        mTextureSet[0].frameNumber = -1;
        mTextureSet[1].frameNumber = -1;
        mCurOffset = mHeader.movieDataOffsets;
        mReadSize = mHeader.firstFrameSize;
        mReadIndex = 0;
        mTotalReadFrame = 0;
        mDvdError = 0;
        mNextDecodeIndex = 0;
        mAudioDecodeIndex = 0;
        mAudioOutputIndex = 0;
        mCurrentVolume = mTargetVolume;
        mRampCount = 0;
        _310 = 0;
        return true;
    }

    return false;
}

bool THPSimplePlayerWrapper::videoDecode(u8 *pFile) {
    if (THPVideoDecode(pFile, 
        mTextureSet[_310].ytexture,
        mTextureSet[_310].utexture,
        mTextureSet[_310].vtexture,
        mTHPWork)) {
            return false;
        }

    mTextureSet[_310].frameNumber = mReadBuffer[mNextDecodeIndex].frameNumber;
    return true;
}

// THPSimplePlayerWrapper::readFrameAsync

void THPSimplePlayerWrapper::checkPrefetch() {
    BOOL in = OSDisableInterrupts();
    bool isValid = mReadBuffer[mReadIndex].isValid == true;
    if (!isValid && !mReadProgress) {
        readFrameAsync();
    }

    OSRestoreInterrupts(in);
}

/*s32 THPSimplePlayerWrapper::getNextBuffer(u32 idx) const {
    idx++;
    return (idx) < 0x14 ? idx : 0;
}*/

bool THPSimplePlayerWrapper::tryDvdOpen(const char *pFileName) {
    if (!_8) {
        return false;
    }

    if (mOpen) {
        return false;
    }

    MR::zeroMemory(&mVideoInfo, sizeof(THPVideoInfo));
    MR::zeroMemory(&mAudioInfo, sizeof(THPAudioInfo));

    if (!DVDOpen(pFileName, &mFileInfo)) {
        return false;
    }

    _C = 0;
    _9 = 0;
    return true;
}

void THPSimplePlayerWrapper::setupParams() {
    mCurOffset = mHeader.movieDataOffsets;
    mReadSize = mHeader.firstFrameSize;
    mReadIndex = 0;
    mTotalReadFrame = 0;
    mDvdError = 0;
    mTextureSet[0].frameNumber = -1;
    mTextureSet[1].frameNumber = -1;
    mNextDecodeIndex = 0;
    mPreFetchState = 0;
    mAudioState = 0;
    mLoop = 0;
    mOpen = 1;
    _310 = 0;
    resetAudioParams();
}

namespace {
    void readAsyncCallBackFunc(s32 a1, DVDFileInfo *pInfo) {
        THPSimplePlayerWrapper* player = (THPSimplePlayerWrapper*)pInfo->cb.userData;
        player->readAsyncCallBack(a1);
    }
};

void THPSimplePlayerWrapper::exeReadHeader() {
    if (MR::isFirstStep(this)) {
        mFileInfo.cb.userData = this;
        DVDReadAsyncPrio(&mFileInfo, WorkBuffer, 64, _C, ::readAsyncCallBackFunc, 2);
    }
}

void THPSimplePlayerWrapper::exeReadFrameComp() {
    if (MR::isFirstStep(this)) {
        mFileInfo.cb.userData = this;
        _C = mHeader.compInfoDataOffsets;
        DVDReadAsyncPrio(&mFileInfo, WorkBuffer, 32, _C, ::readAsyncCallBackFunc, 2);
    }
}

void THPSimplePlayerWrapper::exeReadVideoComp() {
    if (MR::isFirstStep(this)) {
        mFileInfo.cb.userData = this;
        DVDReadAsyncPrio(&mFileInfo, WorkBuffer, 32, _C, ::readAsyncCallBackFunc, 2);
    }
}

void THPSimplePlayerWrapper::exeReadAudioComp() {
    if (MR::isFirstStep(this)) {
        mFileInfo.cb.userData = this;
        DVDReadAsyncPrio(&mFileInfo, WorkBuffer, 32, _C, ::readAsyncCallBackFunc, 2);
    }
}

void THPSimplePlayerWrapper::endReadHeader() {
    memcpy(&mHeader, WorkBuffer, sizeof(mHeader));
    if (strcmp(mHeader.magic, "THP")) {
        DVDClose(&mFileInfo);
        return;
    }

    if (mHeader.version != 0x11000) {
        DVDClose(&mFileInfo);
    }
}

void THPSimplePlayerWrapper::endReadFrameComp() {
    memcpy(&mFrameComp, WorkBuffer, sizeof(mFrameComp));
    mAudioExist = 0;
    _C += 0x14;
}

void THPSimplePlayerWrapper::endReadVideoComp() {
    memcpy(&mVideoInfo, WorkBuffer, sizeof(mVideoInfo));
    _C += 12;
    _10++;
}

void THPSimplePlayerWrapper::endReadAudioComp() {
    memcpy(&mAudioInfo, WorkBuffer, sizeof(mAudioInfo));
    mAudioExist = 1;
    _C += 0x10;
    _10++;
}

void THPSimplePlayerWrapper::exeReadPreLoad() {
    if (MR::isFirstStep(this)) {
        DVDReadAsyncPrio(&mFileInfo, mReadBuffer[mReadIndex].ptr, mReadSize, mCurOffset, ::readAsyncCallBackFunc, 2);
    }

    if (!_314) {
        mPreFetchState = 1;
        setNerve(&NrvTHPSimplePlayerWrapper::HostTypeWait::sInstance);
    }
}

void THPSimplePlayerWrapper::endReadPreLoadOne() {
    mCurOffset += mReadSize;
    mReadSize = *(s32*)mReadBuffer[mReadIndex].ptr;
    mReadBuffer[mReadIndex].isValid = 1;
    mReadBuffer[mReadIndex].frameNumber = mTotalReadFrame;
    mReadIndex = getNextBuffer(mReadIndex);
    mTotalReadFrame++;

    if (mTotalReadFrame > mHeader.numFrames - 1 && mLoop == 1) {
        mTotalReadFrame = 0;
        mCurOffset = mHeader.movieDataOffsets;
        mReadSize = mHeader.firstFrameSize;
    }

    _314--;
}
