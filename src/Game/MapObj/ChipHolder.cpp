#include "Game/MapObj/ChipHolder.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/ChipBase.hpp"
#include "Game/MapObj/ChipCounter.hpp"
#include "Game/MapObj/ChipGroup.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
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

ChipGroup* ChipHolder::findChipGroup(s32 groupId) const {
    if (groupId < 0) {
        return nullptr;
    }

    for (s32 i = 0; i < mNumChipGroups; i++) {
        if (mChipGroups[i]->_4C == groupId) {
            return mChipGroups[i];
        }
    }

    return nullptr;
}

void MR::createChipHolder(s32 chipType) {
    switch (chipType) {
    case ChipBase::Type_Blue:
        createSceneObj(SceneObj_BlueChipHolder);
        break;
    case ChipBase::Type_Yellow:
        createSceneObj(SceneObj_YellowChipHolder);
        break;
    }
}

ChipHolder* MR::getChipHolder(s32 chipType) {
    switch (chipType) {
    case ChipBase::Type_Blue:
        return getSceneObj< ChipHolder >(SceneObj_BlueChipHolder);
    case ChipBase::Type_Yellow:
        return getSceneObj< ChipHolder >(SceneObj_YellowChipHolder);
    default:
        return nullptr;
    }
}

void MR::registerChipGroup(s32 chipType, ChipGroup* pChipGroup) {
    getChipHolder(chipType)->registerChipGroup(pChipGroup);
}

void MR::registerChip(s32 chipType, ChipBase* pChip, s32 groupId) {
    getChipHolder(chipType)->findChipGroup(groupId)->registerChip(pChip);
}

void MR::noticeGetChip(s32 chipType, ChipBase* pChip, s32 groupId) {
    ChipHolder* holder = getChipHolder(chipType);
    ChipGroup* group = holder->findChipGroup(groupId);
    group->noticeGet(pChip);
    holder->mChipCounter->setCount(group->getGotCount());
    incPlayerOxygen(8);
}

s32 MR::showChipCounter(s32 chipType, s32 param2) {
    ChipHolder* holder = getChipHolder(chipType);
    return holder->mChipCounter->requestShow(param2, holder->findChipGroup(param2)->getGotCount());
}

void MR::hideChipCounter(s32 chipType, s32 param2) {
    getChipHolder(chipType)->mChipCounter->requestHide(param2);
}

void MR::requestStartChipCompleteDemo(s32 chipType, s32 param2) {
    getChipHolder(chipType)->mChipCounter->requestComplete(param2);
}

void MR::noticeEndChipCompleteDemo(s32 chipType, s32 groupId) {
    getChipHolder(chipType)->findChipGroup(groupId)->noticeEndCompleteDemo();
}

s32 MR::getGotChipCount(s32 chipType, s32 groupId) {
    return getChipHolder(chipType)->findChipGroup(groupId)->getGotCount();
}

void MR::activateChipLayout() {
    if (isExistSceneObj(SceneObj_BlueChipHolder)) {
        getChipHolder(ChipBase::Type_Blue)->mChipCounter->requestActive();
    }

    if (isExistSceneObj(SceneObj_YellowChipHolder)) {
        getChipHolder(ChipBase::Type_Yellow)->mChipCounter->requestActive();
    }
}

void MR::deactivateChipLayout() {
    if (isExistSceneObj(SceneObj_BlueChipHolder)) {
        getChipHolder(ChipBase::Type_Blue)->mChipCounter->requestDeactive();
    }

    if (isExistSceneObj(SceneObj_YellowChipHolder)) {
        getChipHolder(ChipBase::Type_Yellow)->mChipCounter->requestDeactive();
    }
}

ChipHolder::~ChipHolder() {
}

void ChipHolder::init(const JMapInfoIter& rIter) {
    mChipGroups = new ChipGroup*[64];

    switch (mChipType) {
    case ChipBase::Type_Blue:
        mChipCounter = new ChipCounter("ブルーチップカウンター", mChipType);
        break;
    case ChipBase::Type_Yellow:
        mChipCounter = new ChipCounter("イエローチップカウンター", mChipType);
        break;
    }

    mChipCounter->initWithoutIter();
}
