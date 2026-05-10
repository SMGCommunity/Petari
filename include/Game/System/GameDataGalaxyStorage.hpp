#pragma once

#include "Game/System/BinaryDataChunkHolder.hpp"
#include "Game/Util/Array.hpp"

class GalaxyStatusAccessor;
class BinaryDataContentAccessor;
class BinaryDataContentHeaderSerializer;

class GameDataSomeGalaxyStorage {
public:
    GameDataSomeGalaxyStorage(const GalaxyStatusAccessor&);

    void resetAllData();
    s32 getPowerStarNumOwned() const;
    s32 deserialize(const BinaryDataContentAccessor&, const u8*);
    void serialize(const BinaryDataContentAccessor&, u8*) const;

    inline bool hasPowerStar(int scenarioNo) const {
        return (mPowerStarOwnedFlags & (1 << scenarioNo)) != 0;
    }

    inline bool isAlreadyVisited(int scenarioNo) const {
        return (mAlreadyVisitedFlags & (1 << scenarioNo)) != 0;
    }

    inline u16 getMaxCoinNum(int scenarioNo) const {
        return mMaxCoinNum[scenarioNo];
    }

    inline void setMaxCoinNum(s32 scenarioNo, u16 coinNum) {
        mMaxCoinNum[scenarioNo] = coinNum;
    }

    /* 0x00 */ const char* mGalaxyName;
    /* 0x04 */ s32 mPowerStarNum;
    /* 0x08 */ u8 mPowerStarOwnedFlags;
    /* 0x09 */ u8 mAlreadyVisitedFlags;
    /* 0x0A */ u8 _A;
    /* 0x0B */ u8 _B;
    /* 0x0C */ u16 mMaxCoinNum[8];
};

class GameDataSomeScenarioAccessor {
public:
    GameDataSomeScenarioAccessor(GameDataSomeGalaxyStorage* pStorage, s32 scenarioNum) : mSomeGalaxyStorage(pStorage), mScenarioNum(scenarioNum) {
    }

    bool hasPowerStar() const;
    bool isAlreadyVisited() const;
    s32 getMaxCoinNum() const;
    void setPowerStarFlag(bool);
    void setFlagAlreadyVisited(bool);
    void updateMaxCoinNum(int);

    void setBitFlagAccordingToBool(u8*, bool);

    /* 0x00 */ GameDataSomeGalaxyStorage* mSomeGalaxyStorage;
    /* 0x04 */ u32 mScenarioNum;
};

// NOTE/TODO: This dummy is here to ensure vtable alignment in GameDataAllGalaxyStorage.
// This double-inheritance with BinaryDataChunkBase behavior is repeated for a few
// different classes. What actually is the class that takes this role?
class Dummy {
public:
    Dummy(){};
};

class GameDataAllGalaxyStorage : public Dummy, public BinaryDataChunkBase {
public:
    GameDataAllGalaxyStorage();

    virtual u32 makeHeaderHashCode() const;
    virtual u32 getSignature() const;
    virtual s32 serialize(u8*, u32) const;
    virtual s32 deserialize(const u8*, u32);
    virtual void initializeData();

    bool isExistAccessor(const char*, int) const;
    GameDataSomeScenarioAccessor makeAccessor(const char*, int) const;
    s32 calcCurrentPowerStarNum() const;
    s32 getPowerStarNumOwned(const char*) const;
    s32 getGalaxyNum() const;
    GameDataSomeGalaxyStorage* getGalaxyStorage(int);
    void initHeaderSerializer();
    s32 findIndex(const char*) const;
    s32 findIndex(u16) const;

    BinaryDataContentHeaderSerializer* getSerializer() const {
        return mHeaderSerializer;
    }

    /* 0x08 */ MR::Vector< MR::AssignableArray< GameDataSomeGalaxyStorage* > > mSomeGalaxyStorages;
    /* 0x14 */ BinaryDataContentHeaderSerializer* mHeaderSerializer;
};
