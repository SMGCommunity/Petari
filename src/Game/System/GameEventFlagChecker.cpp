#include "Game/System/GameEventFlagChecker.hpp"
#include "Game/SingletonHolder.hpp"
#include "Game/System/GameDataConst.hpp"

GameEventFlagChecker::GameEventFlagChecker(GameDataHolder* pHolder) : mDataHolder(pHolder), mFlagStorage(nullptr) {
    SingletonHolder< GameEventFlagTableInstance >::init();

    mFlagStorage = new GameEventFlagStorage();
}

bool GameEventFlagChecker::canOn(const char* pFlagName) const {
    const GameEventFlag* pFlag = GameEventFlagTable::findFlag(pFlagName);

    switch (pFlag->mType) {
    case GameEventFlag::Type_0:
        return true;
    case GameEventFlag::Type_1:
        return mDataHolder->calcCurrentPowerStarNum() >= pFlag->mStarNum;
    case GameEventFlag::Type_GalaxyOpenStar:
        s32 currentPowerStarNum = mDataHolder->calcCurrentPowerStarNum();
        s32 powerStarOpenNum = GameDataConst::getPowerStarNumToOpenGalaxy(pFlag->mGalaxyName);

        return powerStarOpenNum <= currentPowerStarNum;
    case GameEventFlag::Type_SpecialStar:
        return mDataHolder->hasPowerStar(pFlag->mGalaxyName, pFlag->mStarID);
    case GameEventFlag::Type_4:
        const char* pRequirement1 = pFlag->mRequirement1;
        const char* pRequirement2 = pFlag->mRequirement2;
        bool isOnRequirement1 = pRequirement1 != nullptr ? isOn(pRequirement1) : true;
        bool isOnRequirement2 = pRequirement2 != nullptr ? isOn(pRequirement2) : true;

        return isOnRequirement1 && isOnRequirement2;
    case GameEventFlag::Type_5:
        return mDataHolder->isPassedStoryEvent(pFlag->mEventValueName);
    case GameEventFlag::Type_Galaxy:
        return isOnGalaxy(pFlagName);
    case GameEventFlag::Type_Comet:
        return isOnComet(pFlag);
    case GameEventFlag::Type_11:
        return isOn(pFlag->mEventValueName);
    case GameEventFlag::Type_StarPiece:
        GameEventFlagAccessor accessor1 = GameEventFlagAccessor(pFlag);
        s32 needStarPieceNum = accessor1.getNeedStarPieceNum();

        return mDataHolder->getStarPieceNumGivingToTicoSeed(pFlag->mStarPieceIndex + 8) >= needStarPieceNum;
    case GameEventFlag::Type_EventValueIsZero:
        GameEventFlagAccessor accessor2 = GameEventFlagAccessor(pFlag);

        if (isOn(accessor2.getRequirement()) == false) {
            return false;
        }

        return mDataHolder->getGameEventValue(accessor2.getEventValueName()) == 0;
    case GameEventFlag::Type_10:
        return mDataHolder->isCompleteMarioAndLuigi();
    default:
        return false;
    }
}
bool GameEventFlagChecker::isOn(const char* pFlagName) const {
    const GameEventFlag* pFlag = GameEventFlagTable::findFlag(pFlagName);

    if ((pFlag->mSaveFlag & 0x1) != 0) {
        return canOn(pFlagName);
    }

    switch (pFlag->mType) {
    case GameEventFlag::Type_0:
    case GameEventFlag::Type_1:
    case GameEventFlag::Type_GalaxyOpenStar:
    case GameEventFlag::Type_SpecialStar:
    case GameEventFlag::Type_4:
    case GameEventFlag::Type_5:
    case GameEventFlag::Type_Galaxy:
    case GameEventFlag::Type_EventValueIsZero:
        return mFlagStorage->isOn(pFlag);
    case GameEventFlag::Type_Comet:
    case GameEventFlag::Type_StarPiece:
    case GameEventFlag::Type_10:
    case GameEventFlag::Type_11:
    default:
        return false;
    }
}

bool GameEventFlagChecker::tryOn(const char* pFlagName) {
    if (!canOn(pFlagName)) {
        return false;
    }

    const GameEventFlag* pFlag = GameEventFlagTable::findFlag(pFlagName);

    if ((pFlag->mSaveFlag & 0x1) != 0) {
        return false;
    }

    switch (pFlag->mType) {
    case GameEventFlag::Type_SpecialStar:
    case GameEventFlag::Type_5:
    case GameEventFlag::Type_Comet:
    case GameEventFlag::Type_StarPiece:
    case GameEventFlag::Type_10:
    case GameEventFlag::Type_11:
        return false;
    case GameEventFlag::Type_0:
    case GameEventFlag::Type_1:
    case GameEventFlag::Type_GalaxyOpenStar:
    case GameEventFlag::Type_4:
    case GameEventFlag::Type_Galaxy:
    case GameEventFlag::Type_EventValueIsZero:
    default:
        mFlagStorage->set(pFlag, true);
        return true;
    }
}

void GameEventFlagChecker::reset() {
    mFlagStorage->initializeData();
}

GameEventFlagStorage* GameEventFlagChecker::getChunk() {
    return mFlagStorage;
}

bool GameEventFlagChecker::isOnGalaxy(const char* pGalaxyName) const {
    if (mDataHolder->calcCurrentPowerStarNum() < GameDataConst::getPowerStarNumToOpenGalaxy(pGalaxyName)) {
        return false;
    }

    return isOnGalaxyDepended(pGalaxyName);
}

bool GameEventFlagChecker::isOnGalaxyDepended(const char* pGalaxyName) const {
    const char* galaxyDependedFlags[3];

    s32 length =
        GameEventFlagTable::getGalaxyDependedFlags(galaxyDependedFlags, sizeof(galaxyDependedFlags) / sizeof(*galaxyDependedFlags), pGalaxyName);
    bool isGalaxyDepended = true;

    for (s32 i = 0; i < length; i++) {
        if (isOn(galaxyDependedFlags[i]) == false) {
            isGalaxyDepended = false;
            break;
        }
    }

    return isGalaxyDepended;
}

bool GameEventFlagChecker::isOnComet(const GameEventFlag* pFlag) const {
    const char* pGalaxyName = pFlag->mGalaxyName;

    if (isOn(pFlag->mRequirement) == false) {
        return false;
    }

    return mDataHolder->hasPowerStar(pGalaxyName, pFlag->mStarID) != false;
}
