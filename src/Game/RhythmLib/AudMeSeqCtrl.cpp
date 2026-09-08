#include "Game/RhythmLib/AudMeSeqCtrl.hpp"

AudMeSeqParser AudMeSeqCtrl::sDefaultParser = AudMeSeqParser();

AudMeSeqCtrl::AudMeSeqCtrl() : mParser(&sDefaultParser), mIsLocked(), mStopAfterNote(), mProgress(-1) {
    mReader.init();
    mWaitTime = -1;
    mTime = 0;
}

void AudMeSeqCtrl::start(void* pData, u32 addr) {
    mIsLocked = false;
    mReader.init(pData);
    mReader.jump(addr);
}

void AudMeSeqCtrl::wait(s32 waitTime, s32 time) {
    mWaitTime = waitTime;
    mTime = time;
}

void AudMeSeqCtrl::autoWait() {
    if (mProgress < 0) {
        return;
    }
    mWaitTime = mProgress;
    mTime = 1;
}

void AudMeSeqCtrl::inherit(AudMeSeqCtrl* pOther) {
    mProgress = pOther->mProgress;
    autoWait();
}

s32 AudMeSeqCtrl::rhythmProc(AudMeTrack* pTrack, s32 time) {
    if (getBase() == nullptr) {
        return 0;
    }

    if (mWaitTime == time) {
        if (mTime > 0 && --mTime == 0) {
            mWaitTime = -1;
            mTime = 0;
        }
    }

    if (mTime > 0) {
        return 0;
    }

    while (mTime == 0 && !mIsLocked) {
        s32 ret = mParser->parse(pTrack);
        if (ret < 0) {
            return -1;
        }

        if (ret == 1 && mStopAfterNote == true) {
            return 1;
        }
    }

    return 0;
}
