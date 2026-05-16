#pragma once

#include "Game/System/BinaryDataChunkHolder.hpp"
#include "Game/Util/Array.hpp"
#include <revolution.h>

class GalaxyStatusAccessor;

class SpinDriverPathStorageOne {
public:
    SpinDriverPathStorageOne();

    void reset();
    f32 registerToUse(int, int);
    void updateValue(f32);
    s32 serialize(u8*) const;
    s32 deserialize(const u8*, int);

    f32 getDrawRange() const {
        return mDrawRange;
    }

    f32 getDrawRangeScale() const {
        return (1.0f / 256.0f);
    }

    /* 0x00 */ s32 mZoneId;
    /* 0x04 */ s32 mSpinDriverId;
    /* 0x08 */ f32 mDrawRange;
};

class SpinDriverPathStorageScenario {
public:
    SpinDriverPathStorageScenario();

    void resetAllData();
    s32 setup(int, int, f32*);
    void updateValue(int, f32);
    s32 serialize(u8*, u32) const;
    s32 deserialize(const u8*, u32);
    s32 calcMaxZoneId() const;

    /* 0x00 */ MR::Vector< MR::AssignableArray< SpinDriverPathStorageOne > > mOneStorage;
};

class SpinDriverPathStorageGalaxy {
public:
    SpinDriverPathStorageGalaxy(const GalaxyStatusAccessor&);

    void resetAllData();
    s32 setup(int, int, int, f32*);
    void updateValue(int, int, f32);
    s32 serialize(u8*, u32) const;
    s32 deserialize(const u8*, u32);

    SpinDriverPathStorageScenario& getScenarioStorage(int scenarioNo) {
        return mScenarioStorage[scenarioNo - 1];
    }

    /* 0x00 */ SpinDriverPathStorageScenario* mScenarioStorage;
    /* 0x04 */ s32 mNumScenarios;
    /* 0x08 */ const char* mGalaxyName;
};

// NOTE/TODO: This dummy is here to ensure vtable alignment in GameDataAllGalaxyStorage.
// This double-inheritance with BinaryDataChunkBase behavior is repeated for a few
// different classes. What actually is the class that takes this role?
class Dummy {
public:
    Dummy(){};
};

class SpinDriverPathStorage : public Dummy, public BinaryDataChunkBase {
public:
    SpinDriverPathStorage();

    virtual u32 makeHeaderHashCode() const;
    virtual u32 getSignature() const;
    virtual s32 serialize(u8*, u32) const;
    virtual s32 deserialize(const u8*, u32);
    virtual void initializeData();

    s32 setup(const char*, int, int, int, f32*);
    void updateValue(const char*, int, int, f32);
    SpinDriverPathStorageGalaxy* findFromGalaxy(const char*);
    SpinDriverPathStorageGalaxy* findFromHashCode(u16);

    /* 0x08 */ MR::Vector< MR::AssignableArray< SpinDriverPathStorageGalaxy* > > mGalaxyStorage;
};
