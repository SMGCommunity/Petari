#include "Game/System/GalaxyStatusAccessor.hpp"
#include "Game/System/GameDataFunction.hpp"
#include "Game/System/GameDataHolder.hpp"
#include "Game/System/GameEventFlagTable.hpp"
#include "Game/System/StageResultSequenceChecker.hpp"

void StageResultSequenceCheckList::init() {
    mTicoGalaxyNum = 0;
    mPictureBookChapterCanRead = 0;
    mGalaxyNumCanOpen = 0;
    mPowerStarNum = 0;
    mIsCompleteGalaxy = false;
}

StageResultSequenceChecker::StageResultSequenceChecker() {
    mCheckListPrev.init();
    mCheckListAfter.init();
}

void StageResultSequenceChecker::check() {
    fillCheckList(&mCheckListPrev, GameDataFunction::getSceneStartGameDataHolder());
    fillCheckList(&mCheckListAfter, GameDataFunction::getCurrentGameDataHolder());
}

void StageResultSequenceChecker::checkIfComplete(const char* pGalaxyName) {
    fillCheckListIfComplete(&mCheckListPrev, GameDataFunction::getSceneStartGameDataHolder(), pGalaxyName);
    fillCheckListIfComplete(&mCheckListAfter, GameDataFunction::getCurrentGameDataHolder(), pGalaxyName);
}

bool StageResultSequenceChecker::isJustAppearTicoGalaxy() const {
    return mCheckListPrev.mTicoGalaxyNum < mCheckListAfter.mTicoGalaxyNum;
}

bool StageResultSequenceChecker::isAddPictureBook() const {
    return mCheckListPrev.mPictureBookChapterCanRead < mCheckListAfter.mPictureBookChapterCanRead;
}

bool StageResultSequenceChecker::isJustOpenLibraryRoom() const {
    return mCheckListPrev.mPictureBookChapterCanRead == 0
        && mCheckListPrev.mPictureBookChapterCanRead < mCheckListAfter.mPictureBookChapterCanRead;
}

bool StageResultSequenceChecker::isJustGetGreenStarFirst() const {
    return mCheckListPrev.mGreenStarNum == 0
        && mCheckListAfter.mGreenStarNum == 1;
}

bool StageResultSequenceChecker::isJustCompleteGalaxy() const {
    return mCheckListPrev.mIsCompleteGalaxy == false
        && mCheckListAfter.mIsCompleteGalaxy == true;
}

s32 StageResultSequenceChecker::getPrevPowerStarNum() const {
    return mCheckListPrev.mPowerStarNum;
}

s32 StageResultSequenceChecker::getAfterPowerStarNum() const {
    return mCheckListAfter.mPowerStarNum;
}

void StageResultSequenceChecker::fillCheckList(StageResultSequenceCheckList* pCheckList, const GameDataHolder* pGameDataHolder) {
    pCheckList->init();
    pCheckList->mTicoGalaxyNum = GameDataFunction::calcTicoGalaxyNum(pGameDataHolder);
    pCheckList->mPictureBookChapterCanRead = pGameDataHolder->getPictureBookChapterCanRead();
    pCheckList->mGalaxyNumCanOpen = pGameDataHolder->getGalaxyNumCanOpen();
    pCheckList->mPowerStarNum = pGameDataHolder->calcCurrentPowerStarNum();
    pCheckList->mGreenStarNum = GameDataFunction::calcGreenStarNum(pGameDataHolder);
}

void StageResultSequenceChecker::fillCheckListIfComplete(StageResultSequenceCheckList* pCheckList, const GameDataHolder* pGameDataHolder, const char* pGalaxyName) {
    s32 powerStarNum = MR::makeGalaxyStatusAccessor(pGalaxyName).getPowerStarNum();
    s32 powerStarNumOwned = pGameDataHolder->getPowerStarNumOwned(pGalaxyName);

    pCheckList->mIsCompleteGalaxy = powerStarNumOwned == powerStarNum;
}

bool StageResultSequenceChecker::isJustOpenGalaxyWithoutChallengeGalaxy() const {
    if (mCheckListPrev.mGalaxyNumCanOpen >=  mCheckListAfter.mGalaxyNumCanOpen) {
        return false;
    }

    if (GameDataFunction::isOnJustGameEventFlag("SpecialStarGreenAll")) {
        return mCheckListPrev.mGalaxyNumCanOpen < mCheckListAfter.mGalaxyNumCanOpen - GameEventFlagTable::calcGreenPowerStarNum();
    }

    return true;
}
