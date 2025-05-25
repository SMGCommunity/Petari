#include "Game/MapObj/ChipCounter.hpp"
#include "Game/MapObj/ChipGroup.hpp"
#include "Game/MapObj/ChipHolder.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/JMapInfo.hpp"

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

ChipGroup* ChipHolder::findChipGroup(s32 chipType) const {
    if (chipType < 0) {
        return nullptr;
    }

    for (s32 i = 0; i < mNumChipGroups; i++) {
        if (mChipGroups[i]->_4C == chipType) {
            return mChipGroups[i];
        }
    }

    return nullptr;
}

ChipHolder* MR::createChipHolder(s32 chipType) {
    ChipHolder* result;
    
    switch (chipType) {
        case ChipHolder::Chip_Blue:
            result = (ChipHolder*)createSceneObj(94);
            break;
        case ChipHolder::Chip_Yellow:
            result = (ChipHolder*)createSceneObj(95);
            break;
    }

    return result;
}

ChipHolder* MR::getChipHolder(s32 chipType) {
    switch (chipType) {
        case ChipHolder::Chip_Blue:
            return (ChipHolder*) getSceneObjHolder()->getObj(94);
        
        case ChipHolder::Chip_Yellow:
            return (ChipHolder*) getSceneObjHolder()->getObj(95);
        
        default:
            return nullptr;
    }
}

void MR::registerChipGroup(s32 chipType, ChipGroup* pChipGroup) {
    getChipHolder(chipType)->registerChipGroup(pChipGroup);
}

void MR::registerChip(s32 chipType, ChipBase* pChip, s32 arg_2) {
    getChipHolder(chipType)->findChipGroup(arg_2)->registerChip(pChip);
}

void MR::noticeGetChip(s32 chipType, ChipBase* pChip, s32 arg_2) {
    ChipHolder* holder = getChipHolder(chipType);
    ChipGroup* group = holder->findChipGroup(arg_2);
    group->noticeGet(pChip);
    holder->mChipCounter->setCount(group->getGotCount());
    incPlayerOxygen(8);
}

s32 MR::showChipCounter(s32 chipType, s32 arg_1) {
    ChipHolder* holder = getChipHolder(chipType);
    return holder->mChipCounter->requestShow(arg_1, holder->findChipGroup(arg_1)->getGotCount());
}

void MR::hideChipCounter(s32 chipType, s32 arg_1) {
    ChipHolder* holder = getChipHolder(chipType);
    holder->mChipCounter->requestHide(arg_1);
}

void MR::requestStartChipCompleteDemo(s32 chipType, s32 arg_1) {
    ChipHolder* holder = getChipHolder(chipType);
    holder->mChipCounter->requestComplete(arg_1);
}

void MR::noticeEndChipCompleteDemo(s32 chipType, s32 arg_1) {
    getChipHolder(chipType)->findChipGroup(arg_1)->noticeEndCompleteDemo();
}

s32 MR::getGotChipCount(s32 chipType, s32 arg_1) {
    return getChipHolder(chipType)->findChipGroup(arg_1)->getGotCount();
}

void MR::activateChipLayout() {
    if (isExistSceneObj(94)) {
        getChipHolder(0)->mChipCounter->requestActive();
    }

    if (isExistSceneObj(95)) {
        getChipHolder(1)->mChipCounter->requestActive();
    }
}

void MR::deactivateChipLayout() {
    if (isExistSceneObj(94)) {
        getChipHolder(0)->mChipCounter->requestDeactive();
    }

    if (isExistSceneObj(95)) {
        getChipHolder(1)->mChipCounter->requestDeactive();
    }
}


ChipHolder::~ChipHolder() {

}

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
