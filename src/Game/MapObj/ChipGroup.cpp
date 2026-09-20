#include "Game/MapObj/ChipGroup.hpp"
#include "Game/Map/StageSwitch.hpp"
#include "Game/MapObj/ChipBase.hpp"
#include "Game/MapObj/ChipHolder.hpp"
#include "Game/Util/EventUtil.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

ChipGroup::ChipGroup(const char* pName, s32 chipType) : NameObj(pName) {
    ChipEntry* pEntry = mChips;

    do {
        pEntry->mChip = nullptr;
        pEntry->mIsGotten = false;
        pEntry++;
    } while (pEntry < mChips + 5);

    mSwitchCtrl = nullptr;
    mGotCount = 0;
    mTotalCount = 0;
    mStageSwitchArg = -1;
    mChipType = chipType;
    mFlashFrame = -1;
    mCounterTimer = -1;
    mUIRange = -1.0f;
    mUIRangeFixed = -1.0f;
    mIsDeactivated = 0;
    mAlreadyDoneFlag = -1;
    mIsListeningAppear = false;
    mIsCounterVisible = false;
}

void ChipGroup::updateUIRange() {
    if (mUIRangeFixed < 0.0f) {
        TBox3f range;

        for (s32 i = 0; i < mTotalCount; i++) {
            TBox3f currBound;
            TVec3f pos = mChips[i].mChip->mPosition;

            TVec3f center = pos;
            TVec3f pad(2000.0f);
            currBound.set(center - pad, center + pad);

            if (i == 0) {
                range.i.set(currBound.i);
                range.f.set(currBound.f);
            } else {
                range.extend(currBound.i, currBound.f);
            }
        }

        range.getCenter(&mUIPos);

        TVec3f diff;
        diff.sub(range.f, range.i);
        mUIRange = diff.length() / 2.0f;
    } else {
        mUIRange = mUIRangeFixed;
    }
}

void ChipGroup::movement() {
    if (!mIsDeactivated) {
        if (mIsCounterVisible) {
            if (mUIPos.distance(*MR::getPlayerPos()) < mUIRange) {
                MR::showChipCounter(mChipType, mStageSwitchArg);
            } else {
                MR::hideChipCounter(mChipType, mStageSwitchArg);
            }
        }

        if (mCounterTimer >= 0) {
            mCounterTimer--;

            if (mCounterTimer < 0) {
                MR::hideChipCounter(mChipType, mStageSwitchArg);
                mCounterTimer = -1;
                mIsCounterVisible = false;
            }
        }
    }
}

void ChipGroup::registerChip(ChipBase* pChip) {
    if (mIsListeningAppear) {
        pChip->makeActorDead();
    }

    if (mIsDeactivated) {
        pChip->deactive();
    }

    mChips[mTotalCount].mChip = pChip;
    mTotalCount++;
    updateUIRange();
}

void ChipGroup::noticeEndCompleteDemo() {
    mSwitchCtrl->onSwitchA();
}

s32 ChipGroup::getGotCount() const {
    return mGotCount;
}

bool ChipGroup::isComplete() const {
    for (s32 i = 0; i < mTotalCount; i++) {
        if (!mChips[i].mIsGotten) {
            return false;
        }
    }

    return true;
}

BlueChipGroup::BlueChipGroup(const char* pName) : ChipGroup(pName, ChipBase::Type_Blue) {
}

ChipGroup::~ChipGroup() {
}

YellowChipGroup::YellowChipGroup(const char* pName) : ChipGroup(pName, ChipBase::Type_Yellow) {
}

void ChipGroup::init(const JMapInfoIter& rIter) {
    MR::getJMapInfoTrans(rIter, &mUIPos);
    MR::getJMapInfoArg0WithInit(rIter, &mStageSwitchArg);
    MR::getJMapInfoArg1WithInit(rIter, &mFlashFrame);
    mUIRangeFixed = -1.0f;
    MR::getJMapInfoArg2NoInit(rIter, &mUIRangeFixed);

    s32 arg3 = 0;
    MR::getJMapInfoArg3NoInit(rIter, &arg3);

    if (arg3 == 1) {
        mIsDeactivated = 0;
        mAlreadyDoneFlag = MR::setupAlreadyDoneFlag("チップ集め済み", rIter, &mIsDeactivated);
    } else {
        mIsDeactivated = 0;
        mAlreadyDoneFlag = -1;
    }

    MR::createChipHolder(mChipType);
    MR::registerChipGroup(mChipType, this);
    mSwitchCtrl = MR::createStageSwitchCtrl(this, rIter);

    if (mSwitchCtrl->isValidSwitchAppear()) {
        MR::listenNameObjStageSwitchOnAppear(this, mSwitchCtrl, MR::Functor(this, &ChipGroup::receiveAppearRequest));
        mIsListeningAppear = true;
    } else {
        mIsListeningAppear = false;
    }

    MR::connectToSceneMapObjMovement(this);
}

void ChipGroup::noticeGet(ChipBase* pChip) {
    if (mIsDeactivated) {
        return;
    }

    MR::showChipCounter(mChipType, mStageSwitchArg);

    if (!mIsListeningAppear || mFlashFrame == -1) {
        mIsCounterVisible = true;
    }

    for (s32 i = 0; i < mTotalCount; i++) {
        bool isMatch = false;

        if (pChip && mChips[i].mChip == pChip) {
            isMatch = true;
        }

        if (isMatch) {
            mChips[i].mIsGotten = true;
            mGotCount++;

            if (isComplete()) {
                MR::requestStartChipCompleteDemo(mChipType, mStageSwitchArg);
                MR::requestMovementOn(pChip);

                if (mAlreadyDoneFlag >= 0) {
                    MR::updateAlreadyDoneFlag(mAlreadyDoneFlag, 1);
                }

                if (mChipType == 0) {
                    MR::startSystemSE("SE_OJ_BLUECHIP_COMPLETE", mGotCount, -1);
                } else if (mChipType == 1) {
                    MR::startSystemSE("SE_OJ_YELLOWCHIP_COMPLETE", mGotCount, -1);
                }

                mCounterTimer = -1;
                mIsCounterVisible = false;
                return;
            }

            return;
        }
    }
}

void ChipGroup::receiveAppearRequest() {
    if (mIsDeactivated) {
        return;
    }

    for (s32 i = 0; i < mTotalCount; i++) {
        if (mFlashFrame >= 0) {
            mChips[i].mChip->appearFlashing(mFlashFrame);
        } else {
            mChips[i].mChip->appearWait();
        }
    }

    MR::showChipCounter(mChipType, mStageSwitchArg);

    if (mFlashFrame >= 0) {
        mCounterTimer = mFlashFrame;
        mIsCounterVisible = false;
    } else {
        mCounterTimer = -1;
        mIsCounterVisible = true;
    }

    if (mChipType == 0) {
        MR::startSystemSE("SE_OJ_BLUECHIP_APPEAR");
    } else if (mChipType == 1) {
        MR::startSystemSE("SE_OJ_YELLOWCHIP_APPEAR");
    }
}

BlueChipGroup::~BlueChipGroup() {
}

YellowChipGroup::~YellowChipGroup() {
}
