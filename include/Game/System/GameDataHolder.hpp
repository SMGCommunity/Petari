#pragma once

#include <revolution/types.h>

class BinaryDataChunkHolder;
class GameDataAllGalaxyStorage;
class GameDataPlayerStatus;
class GameDataSomeScenarioAccessor;
class GameEventFlagChecker;
class GameEventValueChecker;
class JMapInfo;
class ScenarioProgressTestRun;
class SpinDriverPathStorage;
class StarPieceAlmsStorage;
class UserFile;

class GameDataHolder {
public:
    GameDataHolder(const UserFile*);

    bool isDataMario() const;
    s32 getGalaxyNumCanOpen() const;
    bool canOnGameEventFlag(const char*) const;
    bool isOnGameEventFlag(const char*) const;
    void tryOnGameEventFlag(const char*);
    u16 getGameEventValue(const char*) const;
    void setGameEventValue(const char*, u16);
    bool isOnGameEventValueForBit(const char*, int) const;
    void setGameEventValueForBit(const char*, int, bool);
    s32 getPictureBookChapterCanRead() const;
    u16 getPictureBookChapterAlreadyRead() const;
    void setPictureBookChapterAlreadyRead(int);
    void setRaceBestTime(const char*, u32);
    u32 getRaceBestTime(const char*) const;
    void addMissPoint(int);
    void resetMissPoint();
    bool isPointCollectForLetter() const;
    void incPlayerMissNum();
    s32 getPlayerMissNum() const;
    bool hasPowerStar(const char*, s32) const;
    bool hasGrandStar(int) const;
    void setPowerStar(const char*, s32, bool);
    s32 getPowerStarNumOwned(const char*) const;
    s32 calcCurrentPowerStarNum() const;
    GameDataSomeScenarioAccessor makeGalaxyScenarioAccessor(const char*, s32);
    bool isOnGalaxyScenarioFlagAlreadyVisited(const char*, s32) const;
    void onGalaxyScenarioFlagAlreadyVisited(const char*, s32);
    bool isAppearGalaxy(const char*) const;
    s32 getPlayerLeft() const;
    void addPlayerLeft(int);
    bool isPlayerLeftSupply() const;
    void offPlayerLeftSupply();
    s32 getStockedStarPieceNum() const;
    void addStockedStarPiece(int);
    s32 setupSpinDriverPathStorage(const char*, int, int, int, f32*);
    void updateSpinDriverPathStorage(const char*, int, int, f32);
    s32 getStarPieceNumGivingToTicoSeed(int) const;
    u32 getStarPieceNumMaxGivingToTicoSeed(int) const;
    void addStarPieceGivingToTicoSeed(int, int);
    bool isCompleteMarioAndLuigi() const;
    bool isPassedStoryEvent(const char*) const;
    void followStoryEventByName(const char*);
    void resetAllData();
    s32 makeFileBinary(u8*, u32);
    bool loadFromFileBinary(const char*, const u8*, u32);

    /* 0x00 */ GameEventFlagChecker* mEventFlagChecker;
    /* 0x04 */ GameEventValueChecker* mEventValueChecker;
    /* 0x08 */ GameDataPlayerStatus* mPlayerStatus;
    /* 0x0C */ GameDataAllGalaxyStorage* mAllGalaxyStorage;
    /* 0x10 */ SpinDriverPathStorage* mSpinDriverPathStorage;
    /* 0x14 */ StarPieceAlmsStorage* mStarPieceAlmsStorage;
    /* 0x18 */ JMapInfo* mMapInfo;
    /* 0x1C */ ScenarioProgressTestRun* mScenarioProgressTestRun;
    /* 0x20 */ BinaryDataChunkHolder* mChunkHolder;
    /* 0x24 */ char mName[16];
    /* 0x34 */ const UserFile* mUserFile;
};
