#include "Game/MapObj/ChipHolder.hpp"
#include "Game/MapObj/ChipCounter.hpp"
#include "Game/MapObj/ChipGroup.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/PlayerUtil.hpp"

ChipHolder::ChipHolder(const char* pName, s32 chipType) : NameObj(pName) {
    mChipCounter = nullptr;
    mChipGroups = nullptr;
    mNumChipGroups = 0;
    mChipType = chipType;
}

void ChipHolder::registerChipGroup(ChipGroup* pChipGroup) {
    mChipGroups[mNumChipGroups] = pChipGroup;
    mNumChipGroups++;
}

ChipGroup* ChipHolder::findChipGroup(s32 chipGroupType) const {
    if (chipGroupType < 0) {
        return nullptr;
    }

    for (s32 i = 0; i < mNumChipGroups; i++) {
        if (mChipGroups[i]->_4C == chipGroupType) {
            return mChipGroups[i];
        }
    }

    return nullptr;
}

ChipHolder* MR::createChipHolder(s32 chipHolderType) {
    ChipHolder* result;

    switch (chipHolderType) {
    case ChipHolder::Chip_Blue:
        result = (ChipHolder*)createSceneObj(SceneObj_BlueChipHolder);
        break;
    case ChipHolder::Chip_Yellow:
        result = (ChipHolder*)createSceneObj(SceneObj_YellowChipHolder);
        break;
    }

    return result;
}

ChipHolder* MR::getChipHolder(s32 chipType) {
    switch (chipType) {
    case ChipHolder::Chip_Blue:
        return (ChipHolder*)getSceneObjHolder()->getObj(SceneObj_BlueChipHolder);

    case ChipHolder::Chip_Yellow:
        return (ChipHolder*)getSceneObjHolder()->getObj(SceneObj_YellowChipHolder);

    default:
        return nullptr;
    }
}

void MR::registerChipGroup(s32 chipType, ChipGroup* pChipGroup) {
    getChipHolder(chipType)->registerChipGroup(pChipGroup);
}

void MR::registerChip(s32 chipHolderType, ChipBase* pChip, s32 chipGroupType) {
    getChipHolder(chipHolderType)->findChipGroup(chipGroupType)->registerChip(pChip);
}

void MR::noticeGetChip(s32 chipHolderType, ChipBase* pChip, s32 chipGroupType) {
    ChipHolder* holder = getChipHolder(chipHolderType);
    ChipGroup* group = holder->findChipGroup(chipGroupType);
    group->noticeGet(pChip);
    holder->mChipCounter->setCount(group->getGotCount());
    incPlayerOxygen(8);
}

s32 MR::showChipCounter(s32 chipHolderType, s32 arg_1) {
    ChipHolder* holder = getChipHolder(chipHolderType);
    return holder->mChipCounter->requestShow(arg_1, holder->findChipGroup(arg_1)->getGotCount());
}

void MR::hideChipCounter(s32 chipHolderType, s32 arg_1) {
    ChipHolder* holder = getChipHolder(chipHolderType);
    holder->mChipCounter->requestHide(arg_1);
}

void MR::requestStartChipCompleteDemo(s32 chipHolderType, s32 arg_1) {
    ChipHolder* holder = getChipHolder(chipHolderType);
    holder->mChipCounter->requestComplete(arg_1);
}

void MR::noticeEndChipCompleteDemo(s32 chipHolderType, s32 chipGroupType) {
    getChipHolder(chipHolderType)->findChipGroup(chipGroupType)->noticeEndCompleteDemo();
}

s32 MR::getGotChipCount(s32 chipHolderType, s32 chipGroupType) {
    return getChipHolder(chipHolderType)->findChipGroup(chipGroupType)->getGotCount();
}

void MR::activateChipLayout() {
    if (isExistSceneObj(SceneObj_BlueChipHolder)) {
        getChipHolder(0)->mChipCounter->requestActive();
    }

    if (isExistSceneObj(SceneObj_YellowChipHolder)) {
        getChipHolder(1)->mChipCounter->requestActive();
    }
}

void MR::deactivateChipLayout() {
    if (isExistSceneObj(SceneObj_BlueChipHolder)) {
        getChipHolder(0)->mChipCounter->requestDeactive();
    }

    if (isExistSceneObj(SceneObj_YellowChipHolder)) {
        getChipHolder(1)->mChipCounter->requestDeactive();
    }
}

ChipHolder::~ChipHolder() {}

void ChipHolder::init(const JMapInfoIter& rIter) {
    mChipGroups = new ChipGroup*[0x40];

    switch (mChipType) {
    case Chip_Blue:
        mChipCounter = new ChipCounter("ブルーチップカウンター", mChipType);
        break;
    case Chip_Yellow:
        mChipCounter = new ChipCounter("イエローチップカウンター", mChipType);
        break;
    }

    mChipCounter->initWithoutIter();
}
