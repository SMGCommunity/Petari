#include "Game/RhythmLib/AudMePlayer.hpp"
#include "Game/AudioLib/AudSystem.hpp"
#include "Game/AudioLib/AudWrap.hpp"
#include "Game/RhythmLib/AudChordInfo.hpp"
#include "Game/RhythmLib/AudMeHandles.hpp"
#include "Game/RhythmLib/AudMeTrack.hpp"
#include "Game/Util/MathUtil.hpp"
#include "JSystem/JAudio2/JASCriticalSection.hpp"

AudMe::AudMe() : JSULink< AudMe >(this), mHandle(), mTrack(), _18(), mPos(), mMeId(-1), _24(-1), mTimer(), _74(), mData(), _7C(), _80(1.0f) {
    initTimed();
}

AudMe::~AudMe() {
    release();
}

void AudMe::requestStart(void* pData, u32 a1) {
    mData = pData;
    _7C = a1;
    _74 = 1;
}

bool AudMe::start(void* pData, u32 a1) {
    mTrack = new AudMeTrack();
    if (mTrack == nullptr) {
        return false;
    }

    JASCriticalSection crit;
    mTrack->setSeqData(pData, a1);
    updateTimedParams(true);
    mTrack->startSeq();

    return true;
}

void AudMe::stop(s32 time) {
    if (time == -1) {
        if (mTrack != nullptr) {
            mTrack->stopAtCurrentNoteEnd();
        }
        return;
    }

    if (time == -2) {
        doneStop();
        return;
    }

    if (time == 0) {
        time = _94;
    }

    if (time != 0) {
        setTimedParam(MoveParam_DistFilter, 0.0f, time);
        return;
    }
    doneStop();
}

void AudMe::release() {
    if (mTrack != nullptr) {
        if (mTrack->_F0 != 0) {
            stop(0);
        }
        delete mTrack;
        mTrack = nullptr;
        mMeId = -1;
    }
    releaseHandle();
}

void AudMe::attachHandle(AudMeHandle* pHandle) {
    if (mHandle != nullptr) {
        releaseHandle();
    }
    mHandle = pHandle;
    mHandle->mMe = this;
}

void AudMe::releaseHandle() {
    if (mHandle == nullptr) {
        return;
    }
    mHandle->mMe = nullptr;
    mHandle = nullptr;
}

void AudMe::update() {
    if (_74 == 1) {
        start(mData, _7C);
        _74 = 0;
    }

    if (_24 > 0) {
        if (--_24 <= 0) {
            stop(0);
            _24 = -1;
        }
    }

    if (mDistFilterParam.mValue <= 0.0f) {
        doneStop();
    }

    if (mTrack != nullptr) {
        updateTimedParams(false);
        {
            JASCriticalSection crit;
            mTrack->gframeProc();
        }

        if (mTrack->_F0 == 0) {
            delete mTrack;
            mTrack = nullptr;
        }
    }

    mTimer++;
}

void AudMe::setTimedParam(s32 param, f32 value, u32 time) {
    if (time == 0) {
        mMoveParams[param].mValue = value;
        mMoveParams[param].mDelta = 0.0f;
    } else {
        mMoveParams[param].mDelta = (value - mMoveParams[param].mValue) / time;
    }
    mMoveParams[param].mTarget = value;
}

void AudMe::initTimed() {
    mVolumeParam.mValue = 1.0f;
    mPitchParam.mValue = 1.0f;
    mPanParam.mValue = 0.5f;
    mFxMixParam.mValue = 0.0f;
    mDolbyParam.mValue = 0.0f;
    mDistFilterParam.mValue = 1.0f;

    for (s32 i = 0; i < TIMED_PARAMS; i++) {
        mMoveParams[i].mDelta = 0.0f;
        mMoveParams[i].mTarget = mMoveParams[i].mValue;
    }
}

void AudMe::updateTimedParams(bool b) {
    if (!b) {
        for (s32 i = 0; i < TIMED_PARAMS; i++) {
            if (mMoveParams[i].mDelta != 0.0f) {
                MR::converge(mMoveParams[i].mValue, mMoveParams[i].mTarget, mMoveParams[i].mDelta);
            }
        }
    }

    if (mTrack == nullptr) {
        return;
    }

    f32 f1 = _80 * mVolume * mVolumeParam.mValue * mDistFilterParam.mValue;
    s32 n = mTrack->_E8;
    f32 f2 = mDolbyParam.mValue + _8C;
    f32 f3 = (mPanParam.mValue - 0.5f) + (_88 - 0.5f) + 0.5f;

    for (s32 i = 0; i < n; i++) {
        JASCriticalSection crit;
        // if (mTrack->?)
        // TODO: finish with more context from AudMeTrack
    }
}

void AudMe::setOuterPlayingParams(f32 volume, f32 f2, f32 f3, s32 priority, s32 a5) {
    mVolume = volume;
    _88 = f2;
    _8C = f3;
    mPriority = priority;
    _94 = a5;
}

void AudMe::doneStop() {
    if (mTrack != nullptr) {
        mTrack->stopSeq();
        release();
    }

    releaseHandle();
    _24 = -1;
}

void AudMe::setPauseFlag(bool pause) {
    if (mTrack != nullptr) {
        JASCriticalSection crit;
        mTrack->setPauseFlag(pause);
    }
}

AudMeMgr::AudMeMgr() : JASGlobalInstance< AudMeMgr >(true), mMeSeq(), _50(1.0f) {
    for (s32 i = 0; i < 64; i++) {
        _10[i] = 60;
    }
}

void AudMeMgr::setMeSeq(void* pData) {
    mMeSeq = (AudMeTable*)pData;
}

void AudMeMgr::setMeTable(void* pData) {
    mParams.setResource(pData);
}

bool AudMeMgr::startMe(u32 meId, AudMeHandle* pHandle, const TVec3f* pPos) {
    if (meId == -1) {
        return false;
    }

    if (mMeSeq == nullptr) {
        return false;
    }

    if (!AudWrap::getSystem()->getChordInfo()->isAvailable()) {
        return false;
    }

    if (pHandle != nullptr && pHandle->isMeAttached()) {
        if (pHandle->getMe()->_24 > -1) {
            pHandle->getMe()->stop(0);
        }
    }

    AudMe* me = new AudMe();
    if (me == nullptr) {
        return false;
    }

    if (pHandle != nullptr) {
        me->attachHandle(pHandle);
    }

    if (pPos != nullptr) {
        me->mPos = pPos;
    }

    s32 startPos = getSeqStartPos(meId);
    if (startPos < 0) {
        pHandle->releaseMe();
        delete me;
        return false;
    }

    if (isRequestedMe(startPos)) {
        pHandle->releaseMe();
        delete me;
        return false;
    }

    me->mMeId = meId;
    setOuterPlayingParam(meId, me);
    me->requestStart(mMeSeq, startPos);
    mMeList.append(me);
    me->_80 = _50;
    return true;
}

void AudMeMgr::freeDeadMe() {
    for (JSULink< AudMe >* it = mMeList.getFirst(); it != mMeList.getEnd();) {
        JSULink< AudMe >* next = it->getNext();
        if (it->getObject()->mTrack == nullptr) {
            mMeList.remove(it);
            delete it->getObject();
        }
        it = next;
    }
}

void AudMeMgr::updateEachMe() {
    for (JSULink< AudMe >* it = mMeList.getFirst(); it != mMeList.getEnd(); it = it->getNext()) {
        if (!AudWrap::getSystem()->getChordInfo()->isAvailable()) {
            it->getObject()->stop(0);
        }
        it->getObject()->update();
    }
}

void AudMeMgr::stopAll(s32 time) {
    for (JSULink< AudMe >* it = mMeList.getFirst(); it != mMeList.getEnd(); it = it->getNext()) {
        it->getObject()->stop(time);
    }
}

void AudMeMgr::setPauseFlagAll(bool pause) {
    for (JSULink< AudMe >* it = mMeList.getFirst(); it != mMeList.getEnd(); it = it->getNext()) {
        it->getObject()->setPauseFlag(pause);
    }
}

void AudMeMgr::update() {
    updateEachMe();
    freeDeadMe();
}

s32 AudMeMgr::getSeqStartPos(u32 meId) {
    // FIXME: incorrect load
    // https://decomp.me/scratch/aDHhS

    if (mMeSeq == nullptr) {
        return -1;
    }
    if (meId >= mMeSeq->mNumEntries) {
        return -1;
    }
    return mMeSeq->mSeqStartPos[meId];
}

bool AudMeMgr::isRequestedMe(u32 meId) {
    AudMeTrack* track;
    for (JSULink< AudMe >* it = mMeList.getFirst(); it != mMeList.getEnd(); it = it->getNext()) {
        track = it->getObject()->mTrack;
        if (track != nullptr) {
            JASCriticalSection crit;
            if (meId == track->mMeId) {
                bool b = track->_F0 == 2 || track->_F0 == 1;
                if (b) {
                    return true;
                }
            }
        }
    }
    return false;
}

void AudMeMgr::setOuterPlayingParam(s32 meId, AudMe* pMe) {
    AudMePlayingParams* params = &mParams.mParams[meId];
    f32 vol = params->mChordVolume / 127.0f;
    vol *= vol;
    f32 f2 = params->_2 / 127.0f;
    f32 f3 = params->_3 / 127.0f;
    pMe->setOuterPlayingParams(vol, f2, f3, params->mPriority, params->_4);
}

void AudMePlayingParamsHolder::setResource(void* pRes) {
    s32* cursor = (s32*)pRes;

    s32 numEntries = *cursor++;
    s32 entryOff = *cursor++;
    s32 namesOff = *cursor++;

    mNumEntries = numEntries;

    const char** offsets = (const char**)((s32)pRes + namesOff);
    mParams = (AudMePlayingParams*)((s32)pRes + entryOff);

    for (u32 i = 0; i < numEntries; i++) {
        offsets[i] += (s32)pRes;
    }

    mNames = (const char**)offsets;
}
