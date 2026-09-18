#include "Game/Effect/SyncBckEffectChecker.hpp"
#include "Game/Animation/XanimePlayer.hpp"
#include "Game/Effect/SyncBckEffectInfo.hpp"
#include "Game/Util/MathUtil.hpp"

SyncBckEffectChecker::SyncBckEffectChecker(XanimePlayer* pPlayer) : mPlayer(pPlayer), mPrevFrame(), mIsReset(), mCurrentBckName(), mPrevBckName() {
}

void SyncBckEffectChecker::updateBefore() {
    bool isPlaying;

    if (!mPlayer->_24[mPlayer->_54].checkState(1)) {
        isPlaying = 0.0f != mPlayer->getFrameCtrl()->getRate() || mPrevFrame != mPlayer->getFrameCtrl()->getFrame();
    } else {
        isPlaying = false;
    }

    mCurrentBckName = isPlaying ? mPlayer->getCurrentBckName() : nullptr;
}

void SyncBckEffectChecker::updateAfter() {
    mPrevBckName = mCurrentBckName;
    mIsReset = false;
    mPrevFrame = mPlayer->getFrameCtrl()->getFrame();
}

void SyncBckEffectChecker::reset() {
    mIsReset = true;
    mPrevFrame = 0.0f;
}

bool SyncBckEffectChecker::isCreate(const SyncBckEffectInfo* pInfo, bool checkFrame) const {
    if (!pInfo->isRegisteredBck(mCurrentBckName)) {
        return false;
    }

    if (!checkFrame) {
        return true;
    }

    if (!pInfo->isRegisteredBck(mCurrentBckName)) {
        return false;
    }

    const f32 frame = pInfo->mStartFrame;
    const J3DFrameCtrl* pFrameCtrl = mPlayer->getFrameCtrl();

    if (mIsReset && 0.0f != pFrameCtrl->getRate() && MR::isNearZero(frame + pFrameCtrl->getRate() - pFrameCtrl->getFrame())) {
        return true;
    }

    return checkPass(frame);
}

bool SyncBckEffectChecker::isDelete(const SyncBckEffectInfo* pInfo) const {
    if (!pInfo->isRegisteredBck(mCurrentBckName)) {
        if (pInfo->isBckLoop(mCurrentBckName)) {
            return mCurrentBckName != mPrevBckName;
        }

        if (pInfo->mContinueAnimEnd) {
            const char* pName = mPlayer->getCurrentBckName();

            if (pName == nullptr) {
                return false;
            }

            if (!pInfo->isRegisteredBck(pName)) {
                return mPlayer->isTerminate(pName);
            }
        } else {
            return mCurrentBckName != mPrevBckName;
        }
    }

    if (!MR::Effect::isExistSyncBckDeleteFrame(pInfo)) {
        return false;
    }

    return checkPass(pInfo->mEndFrame);
}

bool SyncBckEffectChecker::checkPass(f32 frame) const {
    if (0.0f == mPlayer->getFrameCtrl()->getRate()) {
        return checkPassIfRate0(frame);
    }

    return mPlayer->checkPass(frame) == true;
}

bool SyncBckEffectChecker::checkPassIfRate0(f32 frame) const {
    const J3DFrameCtrl* pFrameCtrl = mPlayer->getFrameCtrl();
    const f32 currentFrame = pFrameCtrl->getFrame();

    if (pFrameCtrl->getAttribute() == J3DFrameCtrl::EMode_LOOP && currentFrame < mPrevFrame) {
        if ((mPrevFrame <= frame && frame < pFrameCtrl->getEnd()) || (pFrameCtrl->getLoop() <= frame && frame < currentFrame)) {
            return true;
        }
    } else if (mPrevFrame <= currentFrame) {
        if (mPrevFrame <= frame && frame < currentFrame) {
            return true;
        }
    } else {
        if (currentFrame <= frame && frame < mPrevFrame) {
            return true;
        }
    }

    return false;
}
