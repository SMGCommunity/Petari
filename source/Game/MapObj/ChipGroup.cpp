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

BlueChipGroup::BlueChipGroup(const char *pName) : ChipGroup(pName, 0) {

}

ChipGroup::~ChipGroup() {

}

YellowChipGroup::YellowChipGroup(const char *pName) : ChipGroup(pName, 1) {

}

// ChipGroup::init
// ChipGroup::noticeGet
// ChipGroup::receiveAppearRequest

BlueChipGroup::~BlueChipGroup() {

}

YellowChipGroup::~YellowChipGroup() {

}
