#include "Game/System/ScenarioProgressTestRun.hpp"
#include "Game/System/GalaxyStatusAccessor.hpp"
#include "Game/System/GameDataHolder.hpp"
#include "Game/System/GameEventFlagTable.hpp"
#include "Game/System/ScenarioDataParser.hpp"
#include "Game/Util/StringUtil.hpp"
#include <cstdio>

bool ScenarioProgressTestRun::tryTakeNextPowerStar(GalaxyScenarioInfo* pInfo, u32 preference) {
    switch (preference) {
    case 0:
        return tryTakeNextPowerStarPreferScenario(pInfo);
    case 1:
        return tryTakeNextPowerStarPreferGalaxy(pInfo);
    default:
        return false;
    }
}

bool ScenarioProgressTestRun::tryTakePowerStarsToPlayGalaxyScenario(const GalaxyScenarioInfo* pInfo, bool a2) {
    GalaxyScenarioInfo info;

    if (!GameEventFlagTable::isExist(pInfo->mStageName)) {
        info.mStageName = "EggStarGalaxy";
        info.mScenarioNo = 1;
        pInfo = &info;
    }

    GalaxyStatusAccessor accessor = MR::makeGalaxyStatusAccessor(pInfo->mStageName);

    if (pInfo->mScenarioNo < 1 || accessor.getPowerStarNum() < pInfo->mScenarioNo || canTakePowerStar(pInfo)) {
        mGameDataHolder->trySyncStoryEventByGameEventFlag();
        return true;
    }

    if (!tryTakePowerStarsToOpenGalaxy(1, pInfo->mStageName, true)) {
        return false;
    }

    s32 starNum = a2 ? pInfo->mScenarioNo - 1 : pInfo->mScenarioNo;

    for (s32 starNo = 1; starNo <= starNum; starNo++) {
        GalaxyScenarioInfo info;
        info.mStageName = pInfo->mStageName;
        info.mScenarioNo = starNo;

        GalaxyStatusAccessor accessor = MR::makeGalaxyStatusAccessor(info.mStageName);

        if (!canTakePowerStar(&info)) {
            if (accessor.getNormalScenarioNum() >= info.mScenarioNo) {
                return false;
            }

            continue;
        }

        if (canTakePowerStar(&info)) {
            mGameDataHolder->setPowerStar(info.mStageName, info.mScenarioNo, true);
        }

        tryOnGameEventFlagsDependedAnother(&info);

        if (mGameDataHolder->getGalaxyNumCanOpen() > 0) {
            for (ScenarioDataIter iter = MR::makeBeginScenarioDataIter(); !iter.isEnd(); iter.goNext()) {
                GalaxyStatusAccessor accessor = iter.makeAccessor();

                if (accessor.getPowerStarNum() == 0) {
                    continue;
                }

                if (!GameEventFlagTable::isExist(accessor.getName())) {
                    continue;
                }

                if (!mGameDataHolder->canOnGameEventFlag(accessor.getName())) {
                    continue;
                }

                if (mGameDataHolder->isOnGameEventFlag(accessor.getName())) {
                    continue;
                }

                mGameDataHolder->tryOnGameEventFlag(accessor.getName());
            }
        }

        mGameDataHolder->trySyncStoryEventByGameEventFlag();
    }

    return true;
}

bool ScenarioProgressTestRun::tryOnGameEventFlagsDependedAnother(const GalaxyScenarioInfo* pInfo) {
    const char* flagName = GameEventFlagTable::getEventFlagNameSpecialPowerStar(pInfo->mStageName, pInfo->mScenarioNo);

    if (flagName == nullptr) {
        return false;
    }

    return mGameDataHolder->tryOnGameEventFlagsDependedAnother(flagName);
}

bool ScenarioProgressTestRun::canTakePowerStar(const GalaxyScenarioInfo* pInfo) const {
    if (mGameDataHolder->hasPowerStar(pInfo->mStageName, pInfo->mScenarioNo)) {
        return false;
    }

    char flagName[0x40];
    snprintf(flagName, sizeof(flagName), "Play%s%1d", pInfo->mStageName, pInfo->mScenarioNo);

    if (GameEventFlagTable::isExist(flagName) && !mGameDataHolder->isOnGameEventFlag(flagName)) {
        return false;
    }

    if (pInfo->mScenarioNo == 4) {
        snprintf(flagName, sizeof(flagName), "NormalComet%s", pInfo->mStageName);

        if (GameEventFlagTable::isExist(flagName) && !mGameDataHolder->isOnGameEventFlag(flagName)) {
            return false;
        }
    }

    if (pInfo->mScenarioNo == 5) {
        snprintf(flagName, sizeof(flagName), "Coin100Comet%s", pInfo->mStageName);

        if (GameEventFlagTable::isExist(flagName) && !mGameDataHolder->isOnGameEventFlag(flagName)) {
            return false;
        }
    }

    return mGameDataHolder->isOnGameEventFlag(pInfo->mStageName) != false;
}

ScenarioProgressTestRun::ScenarioProgressTestRun(GameDataHolder* pGameDataHolder) : mGameDataHolder(pGameDataHolder) {
}

bool ScenarioProgressTestRun::tryTakeNextPowerStarAndOnEventFlag(u32 preference) {
    GalaxyScenarioInfo info;
    bool isTakeNextPowerStar = tryTakeNextPowerStar(&info, preference);
    if (!isTakeNextPowerStar) {
        return false;
    }

    bool isOnFlagGalaxy;
    bool isOnFlagAny = tryOnGameEventFlagsDependedAnother(&info) != false;

    if (mGameDataHolder->getGalaxyNumCanOpen() > 0) {
        isOnFlagGalaxy = false;

        for (ScenarioDataIter iter = MR::makeBeginScenarioDataIter(); !iter.isEnd(); iter.goNext()) {
            GalaxyStatusAccessor accessor = iter.makeAccessor();

            if (accessor.getPowerStarNum() == 0) {
                continue;
            }

            if (!GameEventFlagTable::isExist(accessor.getName())) {
                continue;
            }

            if (!mGameDataHolder->canOnGameEventFlag(accessor.getName())) {
                continue;
            }

            if (mGameDataHolder->isOnGameEventFlag(accessor.getName())) {
                continue;
            }

            mGameDataHolder->tryOnGameEventFlag(accessor.getName());
            isOnFlagGalaxy = true;
        }

        isOnFlagAny |= isOnFlagGalaxy;
    }

    isOnFlagAny |= mGameDataHolder->trySyncStoryEventByGameEventFlag();
    return isTakeNextPowerStar | isOnFlagAny;
}

// FIXME: stack ordering
bool ScenarioProgressTestRun::tryTakePowerStarsToOpenGalaxy(u32 preference, const char* pStageName, bool ignoreTargetGalaxyFlag) {
    if (mGameDataHolder->canOnGameEventFlag(pStageName)) {
        return true;
    }

    bool isAdvance = true;

    while (isAdvance) {
        GalaxyScenarioInfo info;

        bool isTakeNextPowerStar = tryTakeNextPowerStar(&info, preference);
        if (!isTakeNextPowerStar) {
            return false;
        }

        const char* flagName = GameEventFlagTable::getEventFlagNameSpecialPowerStar(info.mStageName, info.mScenarioNo);
        if (flagName != nullptr) {
            mGameDataHolder->tryOnGameEventFlagsDependedAnother(flagName);
        }

        if (mGameDataHolder->getGalaxyNumCanOpen() > 0) {
            bool canOnFlag = mGameDataHolder->canOnGameEventFlag(pStageName);
            if (canOnFlag) {
                isAdvance = false;
            }

            const char* stageName = nullptr;
            if (!ignoreTargetGalaxyFlag && canOnFlag) {
                stageName = info.mStageName;
            }

            for (ScenarioDataIter iter = MR::makeBeginScenarioDataIter(); !iter.isEnd(); iter.goNext()) {
                GalaxyStatusAccessor accessor = iter.makeAccessor();

                if (accessor.getPowerStarNum() == 0) {
                    continue;
                }

                if (stageName != nullptr && MR::isEqualString(stageName, accessor.getName())) {
                    continue;
                }

                if (!GameEventFlagTable::isExist(accessor.getName())) {
                    continue;
                }

                if (!mGameDataHolder->canOnGameEventFlag(accessor.getName())) {
                    continue;
                }

                if (mGameDataHolder->isOnGameEventFlag(accessor.getName())) {
                    continue;
                }

                mGameDataHolder->tryOnGameEventFlag(accessor.getName());
            }
        }

        mGameDataHolder->trySyncStoryEventByGameEventFlag();
    }

    return true;
}

// FIXME: stack ordering
bool ScenarioProgressTestRun::followGameEventFlagGalaxyOpenAndStoryProgress(const GalaxyScenarioInfo* pInfo) {
    bool isOnFlagGalaxy;
    bool isOnFlagAny = tryOnGameEventFlagsDependedAnother(pInfo) != false;

    if (mGameDataHolder->getGalaxyNumCanOpen() > 0) {
        isOnFlagGalaxy = false;

        for (ScenarioDataIter iter = MR::makeBeginScenarioDataIter(); !iter.isEnd(); iter.goNext()) {
            GalaxyStatusAccessor accessor = iter.makeAccessor();

            if (accessor.getPowerStarNum() == 0) {
                continue;
            }

            if (!GameEventFlagTable::isExist(accessor.getName())) {
                continue;
            }

            if (!mGameDataHolder->canOnGameEventFlag(accessor.getName())) {
                continue;
            }

            if (mGameDataHolder->isOnGameEventFlag(accessor.getName())) {
                continue;
            }

            mGameDataHolder->tryOnGameEventFlag(accessor.getName());
            isOnFlagGalaxy = true;
        }

        isOnFlagAny |= isOnFlagGalaxy;
    }

    return isOnFlagAny | mGameDataHolder->trySyncStoryEventByGameEventFlag();
}

bool ScenarioProgressTestRun::tryTakeNextPowerStarPreferGalaxy(GalaxyScenarioInfo* pInfo) {
    for (s32 starNo = 1;; starNo++) {
        bool foundGalaxy = false;

        for (ScenarioDataIter iter = MR::makeBeginScenarioDataIter(); !iter.isEnd(); iter.goNext()) {
            GalaxyStatusAccessor accessor = iter.makeAccessor();

            if (accessor.getPowerStarNum() == 0) {
                continue;
            }

            if (!mGameDataHolder->isOnGameEventFlag(accessor.getName())) {
                continue;
            }

            if (accessor.getPowerStarNum() < starNo) {
                continue;
            }

            foundGalaxy = true;

            GalaxyScenarioInfo info;
            info.mStageName = accessor.getName();
            info.mScenarioNo = starNo;

            if (!canTakePowerStar(&info)) {
                continue;
            }

            if (canTakePowerStar(&info)) {
                mGameDataHolder->setPowerStar(info.mStageName, info.mScenarioNo, true);
            }

            pInfo->mStageName = info.mStageName;
            pInfo->mScenarioNo = info.mScenarioNo;
            return true;
        }

        if (!foundGalaxy) {
            break;
        }
    }

    return false;
}

bool ScenarioProgressTestRun::tryTakeNextPowerStarPreferScenario(GalaxyScenarioInfo* pInfo) {
    for (ScenarioDataIter iter = MR::makeBeginScenarioDataIter(); !iter.isEnd(); iter.goNext()) {
        GalaxyStatusAccessor accessor = iter.makeAccessor();

        if (accessor.getPowerStarNum() == 0) {
            continue;
        }

        if (!mGameDataHolder->isOnGameEventFlag(accessor.getName())) {
            continue;
        }

        for (s32 starNo = 1; starNo <= accessor.getPowerStarNum(); starNo++) {
            GalaxyScenarioInfo info;
            info.mStageName = accessor.getName();
            info.mScenarioNo = starNo;

            if (!canTakePowerStar(&info)) {
                continue;
            }

            if (canTakePowerStar(&info)) {
                mGameDataHolder->setPowerStar(info.mStageName, info.mScenarioNo, true);
            }

            pInfo->mStageName = info.mStageName;
            pInfo->mScenarioNo = info.mScenarioNo;
            return true;
        }
    }

    return false;
}
