#include "Game/System/GameEventFlagChecker.hpp"
#include "Game/System/GameDataConst.hpp"

#include "Game/SingletonHolder.hpp"

GameEventFlagChecker::GameEventFlagChecker(GameDataHolder* holder) {
    mDataHolder = holder;
    mFlagStorage = nullptr;
    SingletonHolder< GameEventFlagTableInstance >::init();
    mFlagStorage = new GameEventFlagStorage();
}

bool GameEventFlagChecker::canOn(const char* flagName) const {
    const GameEventFlag* flag = GameEventFlagTable::findFlag(flagName);

    switch (flag->mType) {
    case 0:
        return true;
    case 1:
        return mDataHolder->calcCurrentPowerStarNum() >= flag->mStarNum;
    case 2:
        s32 currentPowerStarNum = mDataHolder->calcCurrentPowerStarNum();
        s32 powerStarOpenNum = GameDataConst::getPowerStarNumToOpenGalaxy(flag->mGalaxyName);
        return powerStarOpenNum <= currentPowerStarNum;
    case 3:
        return mDataHolder->hasPowerStar(flag->mGalaxyName, flag->mStarID);
    case 4:
        const char* condition1 = flag->mRequirement1;
        const char* condition2 = flag->mRequirement2;
        bool condition1True = condition1 != nullptr ? isOn(condition1) : true;
        bool condition2True = condition2 != nullptr ? isOn(condition2) : true;
        return condition1True && condition2True;
    case 5:
        return mDataHolder->isPassedStoryEvent(flag->mEventValueName);
    case 6:
        return isOnGalaxy(flagName);
    case 7:
        return isOnComet(flag);
    case 11:
        return isOn(flag->mEventValueName);
    case 8:
        GameEventFlagAccessor accessor = GameEventFlagAccessor(flag);
        s32 needStarPieceNum = accessor.getNeedStarPieceNum();
        return mDataHolder->getStarPieceNumGivingToTicoSeed(flag->StarPieceIndex + 8) >= needStarPieceNum;
    case 9:
        GameEventFlagAccessor accessor2 = GameEventFlagAccessor(flag);

        if (isOn(accessor2.getRequirement()) == false) {
            return false;
        }

        return mDataHolder->getGameEventValue(accessor2.getEventValueName()) == false;
    case 10:
        return mDataHolder->isCompleteMarioAndLuigi();
    default:
        return false;
    }
}
bool GameEventFlagChecker::isOn(const char* flagName) const {
    const GameEventFlag* flag = GameEventFlagTable::findFlag(flagName);

    if (flag->saveFlag & 0x1) {
        return canOn(flagName);
    }

    switch (flag->mType) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 9:
        return mFlagStorage->isOn(flag);
    case 7:
    case 8:
    case 10:
    case 11:
    default:
        return false;
    }
}

bool GameEventFlagChecker::tryOn(const char* flagName) {
    if (canOn(flagName) == false) {
        return false;
    }

    const GameEventFlag* flag = GameEventFlagTable::findFlag(flagName);

    if (flag->saveFlag & 0x1) {
        return false;
    }

    switch (flag->mType) {
    case 3:
    case 5:
    case 7:
    case 8:
    case 10:
    case 11:
        return false;
    case 0:
    case 1:
    case 2:
    case 4:
    case 6:
    case 9:
    default:
        mFlagStorage->set(flag, true);
        return true;
    }
}

void GameEventFlagChecker::reset() {
    mFlagStorage->initializeData();
}

GameEventFlagStorage* GameEventFlagChecker::getChunk() {
    return mFlagStorage;
}

bool GameEventFlagChecker::isOnGalaxy(const char* galaxyName) const {
    if (mDataHolder->calcCurrentPowerStarNum() < GameDataConst::getPowerStarNumToOpenGalaxy(galaxyName)) {
        return false;
    }
    return isOnGalaxyDepended(galaxyName);
}

bool GameEventFlagChecker::isOnGalaxyDepended(const char* galaxyName) const {
    const char* dependedFlags[3];

    s32 length = GameEventFlagTable::getGalaxyDependedFlags(dependedFlags, 3, galaxyName);
    bool isDependedOn = true;

    for (s32 i = 0; i < length; i++) {
        if (isOn(dependedFlags[i]) == false) {
            isDependedOn = false;
            break;
        }
    }
    return isDependedOn;
}

bool GameEventFlagChecker::isOnComet(const GameEventFlag* flag) const {
    const char* galaxyName = flag->mGalaxyName;

    if (isOn(flag->mRequirement) == false) {
        return false;
    }

    return mDataHolder->hasPowerStar(galaxyName, flag->mStarID) != false;
}
