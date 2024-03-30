#include "Game/MapObj/ChipGroup.hpp"
#include "Game/MapObj/ChipBase.hpp"

// ChipGroup::ChipGroup
// ChipGroup::updateUIRange
// ChipGroup::movement

void ChipGroup::registerChip(ChipBase *pChip) {
    if (_6C) {
        pChip->makeActorDead();
    }

    if (_64) {
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
    s32 count = mTotalCount;
    for (s32 i = 0; i < count; i++) {
        if (!mChips[i].mIsGotten) {
            return false;
        }
    }

    return true;
}