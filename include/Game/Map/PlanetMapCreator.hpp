#pragma once

#include "Game/Map/PlanetMap.hpp"

class NameObjArchiveListCollector;

struct PlanetMapData {
    /* 0x00 */ const char* mPlanetName;
    /* 0x04 */ const char* mData[5];
    /* 0x18 */ const char* mForceScenarioData[8];
};

struct UniqueEntry {
    /* 0x00 */ const char* _0;
    /* 0x04 */ const char* _4;
};

typedef NameObj* (*CreatorFuncPtr)(const char*);

struct PlanetEntry {
    /* 0x0 */ const char* mName;
    /* 0x4 */ CreatorFuncPtr mCreateFunc;
};

class PlanetMapCreator : public NameObj {
public:
    PlanetMapCreator(const char*);

    CreatorFuncPtr getCreateFunc(const char*);
    void makeArchiveListPlanet(NameObjArchiveListCollector*, const JMapInfoIter&, const char*);
    void createPlanetMapDataTable(JMapInfo*);
    void addTableData(const JMapInfo*, s32);
    PlanetMapData* getTableData(const char*) const;
    bool isScenarioForceLow(const PlanetMapData*) const;

    /* 0x0C */ PlanetMapData** mPlanetMapData;
    /* 0x10 */ s32 mTableCount;
};

class PlanetMapFarClippable : public PlanetMap {
public:
    PlanetMapFarClippable(const char* pName) : PlanetMap(pName) {
    }

    f32 getFarClipDistance() const {
        return 50.0f;
    }
};

class PlanetMapCreatorFunction {
public:
    static void makeArchiveList(NameObjArchiveListCollector*, const JMapInfoIter&, const char*);

    static bool isLoadArchiveAfterScenarioSelected(const char*);
    static bool isRegisteredObj(const char*);

    static CreatorFuncPtr getPlanetMapCreator(const char*);
};
