#include "Game/System/GameDataFunction.hpp"
#include "Game/SingletonHolder.hpp"
#include "Game/System/GalaxyStatusAccessor.hpp"
#include "Game/System/GameDataHolder.hpp"
#include "Game/System/GameDataTemporaryInGalaxy.hpp"
#include "Game/System/GameEventFlagTable.hpp"
#include "Game/System/GameSequenceDirector.hpp"
#include "Game/System/GameSequenceProgress.hpp"
#include "Game/System/GameSystem.hpp"
#include "Game/System/SaveDataHandleSequence.hpp"
#include "Game/System/ScenarioDataParser.hpp"
#include "Game/System/SysConfigFile.hpp"
#include "Game/System/UserFile.hpp"
#include <cstdio>

namespace {
    SaveDataHandleSequence* getSaveDataHandleSequence() NO_INLINE {
        return SingletonHolder< GameSystem >::get()->mSequenceDirector->mSaveDataHandleSequence;
    }

    GameDataTemporaryInGalaxy* getGameDataTemporaryInGalaxy() {
        return SingletonHolder< GameSystem >::get()->mSequenceDirector->mGameDataTemporaryInGalaxy;
    }

    GameSequenceProgress* getGameSequenceProgress() NO_INLINE {
        return SingletonHolder< GameSystem >::get()->mSequenceDirector->mGameSequenceProgress;
    }
};  // namespace

namespace GameDataFunction {
    bool isDataMario() {
        return getCurrentGameDataHolder()->isDataMario();
    }

    bool hasPowerStar(const char* pGalaxyName, s32 scenarioNo) {
        return getCurrentGameDataHolder()->hasPowerStar(pGalaxyName, scenarioNo);
    }

    bool hasGrandStar(int id) {
        return getCurrentGameDataHolder()->hasGrandStar(id);
    }

    void setGameFlagPowerStarSuccess(const char* pGalaxyName, s32 scenarioNo, bool isOwned) {
        getCurrentGameDataHolder()->setPowerStar(pGalaxyName, scenarioNo, isOwned);
    }
    /*
    GameDataSomeScenarioAccessor makeGalaxyScenarioAccessor(const char* pGalaxyName, s32 scenarioNo) {
        return getCurrentGameDataHolder()->makeGalaxyScenarioAccessor(pGalaxyName, scenarioNo);
    }
    */
    bool isOnGalaxyScenarioFlagAlreadyVisited(const char* pGalaxyName, s32 scenarioNo) {
        return getCurrentGameDataHolder()->isOnGalaxyScenarioFlagAlreadyVisited(pGalaxyName, scenarioNo);
    }

    void onGalaxyScenarioFlagAlreadyVisited(const char* pGalaxyName, s32 scenarioNo) {
        getCurrentGameDataHolder()->onGalaxyScenarioFlagAlreadyVisited(pGalaxyName, scenarioNo);
    }

    bool isAppearGalaxy(const char* pGalaxyName) {
        return getCurrentGameDataHolder()->isAppearGalaxy(pGalaxyName);
    }

    s32 calcCurrentPowerStarNum() {
        return getCurrentGameDataHolder()->calcCurrentPowerStarNum();
    }

    s32 getPowerStarNumOwned(const char* pGalaxyName) {
        return getCurrentGameDataHolder()->getPowerStarNumOwned(pGalaxyName);
    }

    s32 getPowerStarNumMax() {
        s32 powerStarNumMax = 0;

        for (ScenarioDataIter iter = MR::makeBeginScenarioDataIter(); !iter.isEnd(); iter.goNext()) {
            powerStarNumMax += iter.makeAccessor().getPowerStarNum();
        }

        return powerStarNumMax;
    }

    s32 getPictureBookChapterCanRead() {
        return getCurrentGameDataHolder()->getPictureBookChapterCanRead();
    }

    u16 getPictureBookChapterAlreadyRead() {
        return getCurrentGameDataHolder()->getPictureBookChapterAlreadyRead();
    }

    void setPictureBookChapterAlreadyRead(int chapterAlreadyRead) {
        getCurrentGameDataHolder()->setPictureBookChapterAlreadyRead(chapterAlreadyRead);
    }

    void onGameEventFlag(const char* pName) {
        getCurrentGameDataHolder()->tryOnGameEventFlag(pName);
    }

    bool canOnGameEventFlag(const char* pName) {
        return getCurrentGameDataHolder()->canOnGameEventFlag(pName);
    }

    bool canOnAndIsOffGameEventFlag(const char* pName) {
        return getCurrentGameDataHolder()->canOnGameEventFlag(pName)
            && !getCurrentGameDataHolder()->isOnGameEventFlag(pName);
    }

    bool isOnGameEventFlag(const char* pName) {
        return getCurrentGameDataHolder()->isOnGameEventFlag(pName);
    }

    u16 getGameEventValue(const char* pName) {
        return getCurrentGameDataHolder()->getGameEventValue(pName);
    }

    void setGameEventValue(const char* pName, u16 value) {
        getCurrentGameDataHolder()->setGameEventValue(pName, value);
    }

    bool isOnGameEventValueForBit(const char* pName, int index) {
        return getCurrentGameDataHolder()->isOnGameEventValueForBit(pName, index);
    }

    void setGameEventValueForBit(const char* pName, int index, bool isOn) {
        getCurrentGameDataHolder()->setGameEventValueForBit(pName, index, isOn);
    }

    s32 setupAlreadyDoneFlag(const char* pZoneName, const JMapInfoIter& rIter, u32* pParam3) {
        return getGameDataTemporaryInGalaxy()->setupAlreadyDoneFlag(pZoneName, rIter, pParam3);
    }

    void updateAlreadyDoneFlag(int param1, u32 param2) {
        getGameDataTemporaryInGalaxy()->updateAlreadyDoneFlag(param1, param2);
    }

    void restoreGalaxyCometStatus(int cometId, u16* pCometStatus, u16* pPastSecond) {
        char eventValueName[20];

        snprintf(eventValueName, sizeof(eventValueName), "Comet%1dStatus", cometId);

        *pCometStatus = getCurrentGameDataHolder()->getGameEventValue(eventValueName);
    }

    void setRaceBestTime(const char* pRaceName, u32 bestTime) {
        getCurrentGameDataHolder()->setRaceBestTime(pRaceName, bestTime);
    }

    s32 getRaceBestTime(const char* pRaceName) {
        return getCurrentGameDataHolder()->getRaceBestTime(pRaceName);
    }

    void addMissPoint(int num) {
        getCurrentGameDataHolder()->addMissPoint(num);
    }

    void resetMissPoint() {
        getCurrentGameDataHolder()->resetMissPoint();
    }

    bool isPointCollectForLetter() {
        return getCurrentGameDataHolder()->isPointCollectForLetter();
    }

    void incPlayerMissNum() {
        getCurrentGameDataHolder()->incPlayerMissNum();
    }

    s32 registerStorageSpinDriverPathDrawRange(const char* pGalaxyName, int scenarioNo, int zoneId, int param4, f32* pParam5) {
        return getCurrentGameDataHolder()->setupSpinDriverPathStorage(pGalaxyName, scenarioNo, zoneId, param4, pParam5);
    }

    void updateStorageSpinDriverPathDrawRange(const char* pGalaxyName, int scenarioNo, int param3, f32 param4) {
        getCurrentGameDataHolder()->updateSpinDriverPathStorage(pGalaxyName, scenarioNo, param3, param4);
    }

    s32 getStarPieceNumGivingToTicoSeed(int index) {
        return getCurrentGameDataHolder()->getStarPieceNumGivingToTicoSeed(index);
    }

    s32 getStarPieceNumMaxGivingToTicoSeed(int index) {
        return getCurrentGameDataHolder()->getStarPieceNumMaxGivingToTicoSeed(index);
    }

    void addStarPieceGivingToTicoSeed(int index, int num) {
        getCurrentGameDataHolder()->addStarPieceGivingToTicoSeed(index, num);
    }

    bool isPassedStoryEvent(const char* pStoryEventName) {
        return getCurrentGameDataHolder()->isPassedStoryEvent(pStoryEventName);
    }

    void followStoryEventByName(const char* pStoryEventName) {
        if (getCurrentGameDataHolder()->isPassedStoryEvent(pStoryEventName)) {
            return;
        }

        getCurrentGameDataHolder()->followStoryEventByName(pStoryEventName);
    }

    s32 getPlayerLeft() {
        return getCurrentGameDataHolder()->getPlayerLeft();
    }

    void addPlayerLeft(int num) {
        getCurrentGameDataHolder()->addPlayerLeft(num);
    }

    bool isPlayerLeftSupply() {
        return getCurrentGameDataHolder()->isPlayerLeftSupply();
    }

    void offPlayerLeftSupply() {
        getCurrentGameDataHolder()->offPlayerLeftSupply();
    }

    s32 getStockedStarPieceNum() {
        return getCurrentGameDataHolder()->getStockedStarPieceNum();
    }

    s32 getStarPieceNum() {
        return getGameDataTemporaryInGalaxy()->getStarPieceNum();
    }

    void addStarPiece(int num) {
        getGameDataTemporaryInGalaxy()->addStarPiece(num);
    }

    void setLast1upStarPieceNum(int last1upStarPieceNum) {
        getGameDataTemporaryInGalaxy()->setLast1upStarPieceNum(last1upStarPieceNum);
    }

    s32 getLast1upStarPieceNum() {
        return getGameDataTemporaryInGalaxy()->mLast1upStarPieceNum;
    }

    const wchar_t* getUserName() {
        return getSaveDataHandleSequence()->getCurrentUserFile()->mUserName;
    }

    u16 getUserFileIndex() {
        const char* pGameDataName = getSaveDataHandleSequence()->getCurrentUserFile()->getGameDataName();
        s32 index = strlen(pGameDataName) - 1;
        char ch = pGameDataName[index];

        if (ch <= '0' || ch > '9') {
            return 1;
        }

        return ch - '0';
    }

    void resetAllGameData() {
        getCurrentGameDataHolder()->resetAllData();
    }

    bool isOnJustGameEventFlag(const char* pName) {
        return getCurrentGameDataHolder()->isOnGameEventFlag(pName)
            && !getSceneStartGameDataHolder()->isOnGameEventFlag(pName);
    }

    bool canOnJustGameEventFlag(const char* pName) {
        return getCurrentGameDataHolder()->canOnGameEventFlag(pName)
            && !getSceneStartGameDataHolder()->canOnGameEventFlag(pName);
    }

    bool isOnJustAppearGalaxy(const char* pGalaxyName) {
        return getCurrentGameDataHolder()->isAppearGalaxy(pGalaxyName)
            && !getSceneStartGameDataHolder()->isAppearGalaxy(pGalaxyName);
    }

    bool isEqualJustPowerStarNum(int powerStarNum) {
        return getCurrentGameDataHolder()->calcCurrentPowerStarNum() == powerStarNum
            && getSceneStartGameDataHolder()->calcCurrentPowerStarNum() != powerStarNum;
    }

    GameDataHolder* getCurrentGameDataHolder() {
        return getSaveDataHandleSequence()->getCurrentUserFile()->mGameDataHolder;
    }

    GameDataHolder* getSceneStartGameDataHolder() {
        return getSaveDataHandleSequence()->getBackupUserFile()->mGameDataHolder;
    }

    s32 calcTicoGalaxyNum(const GameDataHolder* pGameDataHolder) {
        s32 ticoGalaxyNum = 0;
        s32 exclamationGalaxyNum = GameEventFlagTable::calcExclamationGalaxyNum();

        for (s32 i = 0; i < exclamationGalaxyNum; i++) {
            const char* pExclamationGalaxyName = GameEventFlagTable::getExclamationGalaxyNameFromIndex(i);

            if (pGameDataHolder->isAppearGalaxy(pExclamationGalaxyName)) {
                ticoGalaxyNum++;
            }
        }

        return ticoGalaxyNum;
    }

    s32 calcGrandStarNum(const GameDataHolder* pGameDataHolder) {
        s32 grandStarNum = 0;

        for (int i = 1; i <= 7; i++) {
            if (pGameDataHolder->hasGrandStar(i)) {
                grandStarNum++;
            }
        }

        return grandStarNum;
    }

    void resetGameDataGoToGalaxyFirst() {
        getGameDataTemporaryInGalaxy()->resetRaceBestTime();
        getGameDataTemporaryInGalaxy()->resetPlayerRestartIdInfo();
        getGameDataTemporaryInGalaxy()->_4 = false;
        getGameDataTemporaryInGalaxy()->clearAlreadyDoneFlag();
        getGameDataTemporaryInGalaxy()->resetStageResultStarPieceParam();
        getGameDataTemporaryInGalaxy()->resetStageResultCoinParam();
    }

    void resetGameDataGoToGalaxyRetry() {
        getGameDataTemporaryInGalaxy()->resetRaceBestTime();
        getGameDataTemporaryInGalaxy()->_4 = true;
    }

    void onCompleteEndingCurrentPlayer() {
        getSaveDataHandleSequence()->getCurrentUserFile()->onCompleteEndingCurrentPlayer();
    }

    bool isOnCompleteEndingMarioAndLuigi() {
        return getSaveDataHandleSequence()->getCurrentUserFile()->isOnCompleteEndingMarioAndLuigi();
    }

    OSTime getSysConfigFileTimeAnnounced() {
        return getSaveDataHandleSequence()->getSysConfigFile()->getTimeAnnounced();
    }

    void updateSysConfigFileTimeAnnounced() {
        getSaveDataHandleSequence()->getSysConfigFile()->updateTimeAnnounced();
    }

    OSTime getSysConfigFileTimeSent() {
        return getSaveDataHandleSequence()->getSysConfigFile()->getTimeSent();
    }

    void setSysConfigFileTimeSent(OSTime timeSent) {
        getSaveDataHandleSequence()->getSysConfigFile()->setTimeSent(timeSent);
    }

    u32 getSysConfigFileSentBytes() {
        return getSaveDataHandleSequence()->getSysConfigFile()->getSentBytes();
    }

    void setSysConfigFileSentBytes(u32 sentBytes) {
        getSaveDataHandleSequence()->getSysConfigFile()->setSentBytes(sentBytes);
    }

    bool isLuigiLeftSupply() {
        return getGameSequenceProgress()->isLuigiLeftSupply();
    }

    void offLuigiLeftSupply() {
        getGameSequenceProgress()->offLuigiLeftSupply();
    }

    bool isOnGameEventFlagNormalComet(const char* pGalaxyName) {
        char eventFlagName[64];

        snprintf(eventFlagName, sizeof(eventFlagName), "NormalComet%s", pGalaxyName);

        return getCurrentGameDataHolder()->isOnGameEventFlag(eventFlagName);
    }

    bool isOnGameEventFlagCoin100Comet(const char* pGalaxyName) {
        char eventFlagName[64];

        snprintf(eventFlagName, sizeof(eventFlagName), "Coin100Comet%s", pGalaxyName);

        return getCurrentGameDataHolder()->isOnGameEventFlag(eventFlagName);
    }

    void updateGalaxyCometStatus(int cometId, u16 cometStatus, u16 pastSecond) {
        char eventValueName[20];

        snprintf(eventValueName, sizeof(eventValueName), "Comet%1dStatus", cometId);

        getCurrentGameDataHolder()->setGameEventValue(eventValueName, cometStatus);
    }

    void addStockedStarPiece(int num) {
        getCurrentGameDataHolder()->addStockedStarPiece(num);

        if (getCurrentGameDataHolder()->getStockedStarPieceNum() != 9999) {
            return;
        }

        const char* pEventFlagName = "StarPieceCounterStop";

        if (getCurrentGameDataHolder()->isOnGameEventFlag(pEventFlagName)) {
            return;
        }

        getCurrentGameDataHolder()->tryOnGameEventFlag(pEventFlagName);
    }

    bool isPowerStarLeftInCometOnly() {
        const char* pGalaxyName;

        for (ScenarioDataIter iter = MR::makeBeginScenarioDataIter(); !iter.isEnd(); iter.goNext()) {
            GalaxyStatusAccessor accessor = iter.makeAccessor();

            if (accessor.getPowerStarNum() == 0) {
                continue;
            }

            for (int i = 1; i <= accessor.getPowerStarNum(); i++) {
                if (accessor.isCometStar(i)) {
                    continue;
                }

                pGalaxyName = accessor.getName();

                if (getCurrentGameDataHolder()->hasPowerStar(pGalaxyName, i)) {
                    continue;
                }

                if (GameEventFlagTable::isPowerStarType(accessor.getName(), i, "SpecialStarFinalChallenge")) {
                    continue;
                }

                return false;
            }
        }

        return true;
    }

    s32 calcGreenStarNum(const GameDataHolder* pGameDataHolder) {
        s32 greenStarNum = 0;
        char eventFlagName[32];

        for (int i = 1; i <= 3; i++) {
            snprintf(eventFlagName, sizeof(eventFlagName), "SpecialStarGreen%1d", i);

            if (pGameDataHolder->isOnGameEventFlag(eventFlagName)) {
                greenStarNum++;
            }
        }

        return greenStarNum;
    }
};  // namespace GameDataFunction
