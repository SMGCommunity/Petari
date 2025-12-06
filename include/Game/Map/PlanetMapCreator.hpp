#pragma once

#include "Game/NameObj/NameObj.hpp"

class NameObjArchiveListCollector;

struct PlanetMapData {
    const char* mPlanetName;            // 0x0
    const char* mData[5];               // 0x4
    const char* mForceScenarioData[8];  // 0x18
};

struct UniqueEntry {
    const char* _0;
    const char* _4;
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

    PlanetMapData** mPlanetMapData;  // 0xC
    s32 mTableCount;                 // 0x10
};

class PlanetMapCreatorFunction {
public:
    static void makeArchiveList(NameObjArchiveListCollector*, const JMapInfoIter&, const char*);

    static bool isLoadArchiveAfterScenarioSelected(const char*);
    static bool isRegisteredObj(const char*);

    static CreatorFuncPtr getPlanetMapCreator(const char*);
};
