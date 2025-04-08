#include "Game/Screen/THPSimplePlayerWrapper.hpp"
#include "Game/System/NerveExecutor.hpp"
#include "Game/Util.hpp"
#include "revolution/dvd.h"
#include "revolution/gx/GXStruct.h"
#include "revolution/os.h"
#include "revolution/os/OSInterrupt.h"
#include "revolution/thp.h"
#include "revolution/sc.h"
#include <JSystem/JAudio2/JASAiCtrl.hpp>

static u16 VolumeTable[] =
{
    0,     2,     8,    18,    32,    50,    73,    99,
    130,   164,   203,   245,   292,   343,   398,   457,
    520,   587,   658,   733,   812,   895,   983,  1074,
    1170,  1269,  1373,  1481,  1592,  1708,  1828,  1952,
    2080,  2212,  2348,  2488,  2632,  2781,  2933,  3090,
    3250,  3415,  3583,  3756,  3933,  4114,  4298,  4487,
    4680,  4877,  5079,  5284,  5493,  5706,  5924,  6145,
    6371,  6600,  6834,  7072,  7313,  7559,  7809,  8063,
    8321,  8583,  8849,  9119,  9394,  9672,  9954, 10241,
    10531, 10826, 11125, 11427, 11734, 12045, 12360, 12679,
    13002, 13329, 13660, 13995, 14335, 14678, 15025, 15377,
    15732, 16092, 16456, 16823, 17195, 17571, 17951, 18335,
    18723, 19115, 19511, 19911, 20316, 20724, 21136, 21553,
    21974, 22398, 22827, 23260, 23696, 24137, 24582, 25031,
    25484, 25941, 26402, 26868, 27337, 27810, 28288, 28769,
    29255, 29744, 30238, 30736, 31238, 31744, 32254, 32768
};

static s32 WorkBuffer[16] __attribute__((aligned(32)));

namespace NrvTHPSimplePlayerWrapper {
    NEW_NERVE(HostTypeWait, THPSimplePlayerWrapper, Wait);
    NEW_NERVE(HostTypeReadHeader, THPSimplePlayerWrapper, ReadHeader);
    NEW_NERVE(HostTypeReadFrameComp, THPSimplePlayerWrapper, ReadFrameComp);
    NEW_NERVE(HostTypeReadVideoComp, THPSimplePlayerWrapper, ReadVideoComp);
    NEW_NERVE(HostTypeReadAudioComp, THPSimplePlayerWrapper, ReadAudioComp);
    NEW_NERVE(HostTypeReadPreLoad, THPSimplePlayerWrapper, ReadPreLoad);
};

THPSimplePlayerStaticAudio THPSimplePlayerWrapper::mStaticAudioPlayer;

namespace {
    void dvdCallBackFunc(long a1, DVDFileInfo *pFileInfo) {
        THPSimplePlayerWrapper* player = (THPSimplePlayerWrapper*)pFileInfo->cb.userData;
        player->dvdCallBack(a1);
    }
};

THPSimplePlayerWrapper::THPSimplePlayerWrapper(const char *pName) : NerveExecutor(pName) {
    _8 = 0;
    _9 = 0;
    _C = 0;
    _10 = 0;
    mTHPWork = nullptr;
    mOpen = 0;
    mPreFetchState = 0;
    mAudioState = 0;
    mLoop = 0;
    mAudioExist = 0;
    mCurOffset = 0;
    mDvdError = 0;
    mReadProgress = 0;
    mNextDecodeIndex = 0;
    mReadIndex = 0;
    mReadSize = 0;
    mTotalReadFrame = 0;
    mCurrentVolume = 0.0f;
    mTargetVolume = 0.0f;
    mDeltaVolume = 0.0f;
    mRampCount = 0;
    mAudioDecodeIndex = 0;
    mAudioOutputIndex = 0;
    _2F0 = 0;
    _2F4 = 1.0f;
    _2F8 = 0.0f;
    _2FC = 0;
    mSoundBufferIndex = 0;
    _30C = 0;
    _310 = 0;
    mSoundBuffer[0] = new(32) s32[0x460];
    mSoundBuffer[1] = new(32) s32[0x8C0];
    MR::zeroMemory(mSoundBuffer[0], 0x8C0);
    MR::zeroMemory(mSoundBuffer[1], 0x8C0);
    DCFlushRange(mSoundBuffer[0], 0x8C0);
    DCFlushRange(mSoundBuffer[1], 0x8C0);
    MR::zeroMemory(&mFileInfo, 0x3C);
    MR::zeroMemory(&mHeader, 0x30);
    MR::zeroMemory(&mFrameComp, 0x14);
    MR::zeroMemory(&mVideoInfo, 0xC);
    MR::zeroMemory(&mAudioInfo, 0x10);
    MR::zeroMemory(mReadBuffer, 0xF0);
    MR::zeroMemory(&mTextureSet[0], 0x10);
    MR::zeroMemory(&mTextureSet[1], 0x10);
    MR::zeroMemory(mAudioBuffer, 0xF0);
    initNerve(&NrvTHPSimplePlayerWrapper::HostTypeWait::sInstance);
}

bool THPSimplePlayerWrapper::init(s32 audio) {
    LCEnable();
    if (!THPInit()) {
        return false;
    }

    s32 old = OSDisableInterrupts();
    mSoundBufferIndex = 0;
    initAudio();
    OSRestoreInterrupts(old);
    MR::zeroMemory(mSoundBuffer[0], 0x8C0);
    MR::zeroMemory(mSoundBuffer[1], 0x8C0);
    DCFlushRange(mSoundBuffer[0], 0x8C0);
    DCFlushRange(mSoundBuffer[1], 0x8C0);
    _2F0 = 0;
    _2F4 = 1.0f;
    _2F8 = 0.0f;
    _2FC = 0;
    _8 = 1;
    return true;
}

void THPSimplePlayerWrapper::quit() {
    LCDisable();
    JASDriver::registerMixCallback(nullptr, (JASMixMode)3);
    THPSimplePlayerStaticAudio::mPlayer = nullptr;
    _8 = 0;
}

bool THPSimplePlayerWrapper::open(const char *pName) {
    if (!tryDvdOpen(pName)) {
        return false;
    }

    setNerve(&NrvTHPSimplePlayerWrapper::HostTypeReadHeader::sInstance);
    return true;
}

bool THPSimplePlayerWrapper::close() {
    if (mOpen) {
        if (mPreFetchState == 0) {
            if (mAudioExist) {
                if (mAudioState == 1) {
                    return false;
                }
            }
            else {
                mAudioState = 0;
            }

            if (!mReadProgress) {
                mOpen = 0;
                DVDClose(&mFileInfo);
                return true;
            }
        }
    }

    return false;
}

s32 THPSimplePlayerWrapper::getUseTextureCount() const {
    return 2;
}

u32 THPSimplePlayerWrapper::calcNeedMemory() {
    if (!mOpen) {
        return 0;
    }

    u32 size = OSRoundUp32B(mHeader.bufSize) * 20;
    size += OSRoundUp32B(mVideoInfo.xSize * mVideoInfo.ySize * 2);
    size += OSRoundUp32B(mVideoInfo.xSize * mVideoInfo.ySize >> 1 & 0x7FFFFFFE);
    size += OSRoundUp32B(mVideoInfo.xSize * mVideoInfo.ySize >> 1 & 0x7FFFFFFE);

    if (mAudioExist) {
        size += (OSRoundUp32B(mHeader.audioMaxSamples * 4) * 20);
    }

    size += 0x1000;
    return size;
}

bool THPSimplePlayerWrapper::setBuffer(u8* pBuffer) {
    if (mOpen && !mPreFetchState) {
        if (mAudioState == 1) {
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

s32 THPSimplePlayerWrapper::decode(s32 audio) {
    bool isValid = mReadBuffer[mNextDecodeIndex].isValid == true;
    if (isValid) {
        u32* compSize = (u32*)mReadBuffer[mNextDecodeIndex].ptr + 2;
        u8* ptr = mReadBuffer[mNextDecodeIndex].ptr + mFrameComp.numComponents * 4 + 8;

        if (mAudioExist) {
            if (audio < 0 || audio >= mAudioInfo.sndNumTracks) {
                return 4;
            }

            if (mAudioBuffer[mAudioDecodeIndex].validSample == 0) {
                for (s32 i = 0; i < mFrameComp.numComponents; i++) {
                    switch (mFrameComp.frameComp[i]) {
                        case 0:
                            if (!videoDecode(ptr)) {
                                return 1;
                            }
                            break;
                        case 1:
                            u32 sample = THPAudioDecode(mAudioBuffer[mAudioDecodeIndex].buffer, ptr + (*compSize) * audio, 0);
                            s32 interrupt = OSDisableInterrupts();
                            mAudioBuffer[mAudioDecodeIndex].validSample = sample;
                            mAudioBuffer[mAudioDecodeIndex].curPtr = mAudioBuffer[mAudioDecodeIndex].buffer;
                            OSRestoreInterrupts(interrupt);
                            mAudioDecodeIndex++;

                            if (mAudioDecodeIndex >= 20) {
                                mAudioDecodeIndex = 0;
                            }

                            break;
                    }

                    ptr += *compSize;
                    compSize++;
                }
            }
            else {
                return 3;
            }
        }
        else {
            for (s32 i = 0; i < mFrameComp.numComponents; i++) {
                switch (mFrameComp.frameComp[i]) {
                    case 0:
                        if (!videoDecode(ptr)) {
                            return 1;
                        }
                        break;
                }

                ptr += *compSize;
                compSize++;
            }
        }

        mReadBuffer[mNextDecodeIndex].isValid = 0;
        mNextDecodeIndex = getNextBuffer(mNextDecodeIndex);
        checkPrefetch();
        return 0;
    }

    return 2;
}

s32 THPSimplePlayerWrapper::drawCurrentFrame(_GXRenderModeObj *rmode, u32 x, u32 y, u32 polyW, u32 polyH) {
    if (mTextureSet[_310].frameNumber < 0) {
        return -1;
    }

    THPGXYuv2RgbSetup(rmode);
    THPGXYuv2RgbDraw(mTextureSet[_310].ytexture, mTextureSet[_310].utexture, mTextureSet[_310].vtexture, x, y, mVideoInfo.xSize, mVideoInfo.ySize, polyW, polyH);
    THPGXRestore();
    s32 ret = mTextureSet[_310].frameNumber;
    _310 = _310 == 0;
    return ret;
}

bool THPSimplePlayerWrapper::getVideoInfo(THPVideoInfo *pInfo) const {
    if (!mOpen) {
        return false;
    }

    memcpy(pInfo, &mVideoInfo, sizeof(mVideoInfo));
    return true;
}

f32 THPSimplePlayerWrapper::getFrameRate() const {
    if (mOpen) {
        return mHeader.frameRate;
    }

    return 0.0f;
}

s32 THPSimplePlayerWrapper::getTotalFrame() const {
    if (mOpen) {
        return mHeader.numFrames;
    }

    return 0;
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

void THPSimplePlayerWrapper::readFrameAsync() {
    if (!mDvdError && mPreFetchState != 0) {
        if (mTotalReadFrame > mHeader.numFrames - 1) {
            if (mLoop == 1) {
                mTotalReadFrame = 0;
                mCurOffset = mHeader.movieDataOffsets;
                mReadSize = mHeader.firstFrameSize;
            }
            else {
                return;
            }
        }

        mReadProgress = 1;
        mFileInfo.cb.userData = this;

        if (DVDReadAsyncPrio(&mFileInfo, mReadBuffer[mReadIndex].ptr, mReadSize, mCurOffset, ::dvdCallBackFunc, 2) != 1) {
            mReadProgress = 0;
            mDvdError = 1;
            
        }
    }

    return;
}

void THPSimplePlayerWrapper::checkPrefetch() {
    BOOL in = OSDisableInterrupts();
    bool isValid = mReadBuffer[mReadIndex].isValid == true;
    if (!isValid && !mReadProgress) {
        readFrameAsync();
    }

    OSRestoreInterrupts(in);
}

void THPSimplePlayerWrapper::dvdCallBack(s32 result) {
    if (result == -1) {
        mDvdError = 1;
        return;
    }
    else if (result == -3) {
        return;
    }

    mReadProgress = 0;
    mReadBuffer[mReadIndex].frameNumber = mTotalReadFrame;
    mTotalReadFrame++;
    mReadBuffer[mReadIndex].isValid = 1;
    mCurOffset += mReadSize;
    mReadSize = (s32)mReadBuffer[mReadIndex].ptr;
    mReadIndex = getNextBuffer(mReadIndex);

    if (!mReadBuffer[mReadIndex].isValid) {
        readFrameAsync();
    }
}

void THPSimplePlayerWrapper::readAsyncCallBack(s32 a1) {
    if (a1 < 0) {
        if (!isNerve(&NrvTHPSimplePlayerWrapper::HostTypeReadHeader::sInstance) &&
            !isNerve(&NrvTHPSimplePlayerWrapper::HostTypeReadFrameComp::sInstance) &&
            !isNerve(&NrvTHPSimplePlayerWrapper::HostTypeReadVideoComp::sInstance) &&
            !isNerve(&NrvTHPSimplePlayerWrapper::HostTypeReadAudioComp::sInstance)) {
                isNerve(&NrvTHPSimplePlayerWrapper::HostTypeReadPreLoad::sInstance);
        }
    
        DVDClose(&mFileInfo);
        return;
    }

    if (isNerve(&NrvTHPSimplePlayerWrapper::HostTypeReadHeader::sInstance)) {
        endReadHeader();
        setNerve(&NrvTHPSimplePlayerWrapper::HostTypeReadFrameComp::sInstance);
        return;
    }

    if (isNerve(&NrvTHPSimplePlayerWrapper::HostTypeReadFrameComp::sInstance)) {
        _10 = 0;
        endReadFrameComp();
        checkComponentsInFrame(_10);
        return;
    }

    if (isNerve(&NrvTHPSimplePlayerWrapper::HostTypeReadVideoComp::sInstance)) {
        endReadVideoComp();
        if (tryFinishDvdOpen()) {
            return;
        }

        checkComponentsInFrame(_10);
        return;
    }

    if (isNerve(&NrvTHPSimplePlayerWrapper::HostTypeReadAudioComp::sInstance)) {
        endReadAudioComp();
        if (tryFinishDvdOpen()) {
            return;
        }

        checkComponentsInFrame(_10);
        return;
    }

    if (isNerve(&NrvTHPSimplePlayerWrapper::HostTypeReadPreLoad::sInstance)) {
        endReadPreLoadOne();
        setNerve(&NrvTHPSimplePlayerWrapper::HostTypeReadPreLoad::sInstance);
    }
}

s32 THPSimplePlayerWrapper::getNextBuffer(u32 idx) const {
    return ((idx) + 1  >= 0x14 ? 0 : (idx) + 1);
}

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

bool THPSimplePlayerWrapper::checkComponentsInFrame(s32 comp) {
    if (comp < 0 || mFrameComp.numComponents <= comp) {
        return false;
    }

    switch (mFrameComp.frameComp[comp]) {
        case 0:
            setNerve(&NrvTHPSimplePlayerWrapper::HostTypeReadVideoComp::sInstance);
            return true;
        case 1:
            setNerve(&NrvTHPSimplePlayerWrapper::HostTypeReadAudioComp::sInstance);
            return true;
    }

    return false;
}

bool THPSimplePlayerWrapper::tryFinishDvdOpen() {
    if (_10 < mFrameComp.numComponents) {
        return false;
    }

    setupParams();
    _9 = 1;
    setNerve(&NrvTHPSimplePlayerWrapper::HostTypeWait::sInstance);
    return true;
}

void THPSimplePlayerWrapper::initAudio() {
    if (!SCGetSoundMode()) {
        _30C = 1;
    }
    else {
        _30C = 0;
    }

    THPSimplePlayerStaticAudio::mPlayer = this;
    JASDriver::registerMixCallback(THPSimplePlayerStaticAudio::audioCallback, (JASMixMode)3);
}

bool THPSimplePlayerWrapper::isAudioProcessValid() {
    if (mOpen && mAudioExist) {
        return true;
    }

    return false;
}

s16* THPSimplePlayerWrapper::audioCallback(s32 sample) {
    if (!isAudioProcessValid()) {
        return nullptr;
    }

    s32 en = OSEnableInterrupts();
    mSoundBufferIndex ^= 1;
    mixAudio((s16*)mSoundBuffer[mSoundBufferIndex], sample);
    OSRestoreInterrupts(en);
    return (s16*)mSoundBuffer[mSoundBufferIndex];
}

/* https://decomp.me/scratch/rBNCF */
void THPSimplePlayerWrapper::mixAudio(s16 *pDest, u32 sample) {
    u32 sampleNum, requestSample;
    s32 mix;
    s16 *dst, *libsrc, *thpsrc;
    u16 attenuation;

    if (isAudioProcessValid()) {
        if (_2F0) {
            s32 v7 = mAudioOutputIndex + 1;
            s32 v8 = (mAudioOutputIndex + 1) / 0x14;
            s32 v9 = mAudioOutputIndex + 2;
            _2F0 = 0;
            _2F4 = 0.0f;
            _2F8 = 0.001f;
            _2FC = 4800;

            if (mAudioBuffer[v7 - 0x14 * v8].validSample && mAudioBuffer[v9 % 0x14].validSample) {
                while (1) {
                    if (mAudioBuffer[mAudioOutputIndex].validSample) {
                        if (mAudioBuffer[mAudioOutputIndex].validSample >= sample) {
                            sampleNum = sample;
                        }
                        else {
                            sampleNum = mAudioBuffer[mAudioOutputIndex].validSample;
                        }

                        thpsrc = mAudioBuffer[mAudioOutputIndex].curPtr;

                        for (u32 i = 0; i < sampleNum; i++) {
                            if (mRampCount) {
                                mRampCount--;
                                mCurrentVolume += mDeltaVolume;
                            }
                            else {
                                mCurrentVolume = mTargetVolume;
                            }

                            f32 vol = mCurrentVolume;

                            if (_2FC <= 0) {
                                if (_2F4 < 1.0f) {
                                    vol = mCurrentVolume * _2F4;
                                    f32 v20 = _2F4 + _2F8;
                                    _2F4 += _2F8;

                                    if (v20 >= 1.0f) {
                                        _2F4 = 1.0f;
                                        _2F8 = 0.0f;
                                    }
                                }
                            }
                            else {
                                s32 v19 = _2FC - 1;
                                vol = 0.0f;
                                _2FC = v19;
                                if (v19 < 0) {
                                    _2FC = 0;
                                }
                            }

                            attenuation = VolumeTable[(s32)vol];
                            mix = (*libsrc) + ((attenuation * (*thpsrc)) >> 15);

                            if (mix < -32768) {
                                mix = -32768;
                            }

                            if (mix > 32767) {
                                mix = 32767;
                            }

                            *dst = (s16)mix;
                            dst++;
                            libsrc++;
                            thpsrc++;

                            mix = (*libsrc) + ((attenuation * (*thpsrc)) >> 15);

                            if (mix < -32768)
                            {
                                mix = -32768;
                            }
                            if (mix > 32767)
                            {
                                mix = 32767;
                            }

                            *dst = (s16)mix;
                            dst++;
                            libsrc++;
                            thpsrc++;
                        }

                        requestSample -= sampleNum;
                        mAudioBuffer[mAudioOutputIndex].validSample -= sampleNum;
                        mAudioBuffer[mAudioOutputIndex].curPtr = thpsrc;

                        if (mAudioBuffer[mAudioOutputIndex].validSample == 0) {
                            mAudioOutputIndex++;
                            if (mAudioOutputIndex >= 0x14) {
                                mAudioOutputIndex = 0;
                            }
                        }

                        if (!requestSample) {
                            break;
                        }
                    }
                }
            }

        }
        else {
            MR::zeroMemory(pDest, sample * sizeof(s16*));
        }
    }
    else {
        MR::zeroMemory(pDest, sample * sizeof(s16*));
    }
}

void THPSimplePlayerWrapper::resetAudioParams() {
    mAudioDecodeIndex = 0;
    mAudioOutputIndex = 0;
    MR::zeroMemory(mAudioBuffer, 0xF0);

    for (s32 i = 0; i < 20; i++) {
        mAudioBuffer[i].validSample = 0;
    }

    mRampCount = 0;
    mCurrentVolume = 127.0f;
    mTargetVolume = 127.0f;
}

bool THPSimplePlayerWrapper::setVolume(s32 volume, s32 time) {
    if (!isAudioProcessValid()) {
        return false;
    }

    if (volume > 127) {
        volume = 127;
    }

    if (volume < 0) {
        volume = 0;
    }

    if (time > 60000) {
        time = 60000;
    }

    if (time < 0) {
        time = 0;
    }

    s32 en = OSDisableInterrupts();
    mTargetVolume = (f32)volume;

    if (time) {
        mRampCount = 32 * time;
        mDeltaVolume = (mTargetVolume - mCurrentVolume) / (f32)mRampCount;
    }
    else {
        mRampCount = 0;
        mCurrentVolume = mTargetVolume;
    }

    OSRestoreInterrupts(en);
    return true;
}

bool THPSimplePlayerWrapper::isPreLoading() const {
    return isNerve(&NrvTHPSimplePlayerWrapper::HostTypeReadPreLoad::sInstance);
}

void THPSimplePlayerWrapper::setUnpauseFrameFlag() {
    s32 en = OSDisableInterrupts();
    _2F0 = 1;
    OSRestoreInterrupts(en);
}

s16* THPSimplePlayerStaticAudio::audioCallback(s32 audio) {
    THPSimplePlayerWrapper* player = THPSimplePlayerStaticAudio::mPlayer;
    if (player == nullptr) {
        return nullptr;
    }

    return player->audioCallback(audio);
}


THPSimplePlayerStaticAudio::THPSimplePlayerStaticAudio() {

}
