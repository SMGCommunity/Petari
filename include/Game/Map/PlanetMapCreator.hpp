#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/NameObj/NameObjArchiveListCollector.hpp"
#include "Game/MapObj.hpp"
#include "Game/Util.hpp"

struct PlanetMapData {
    const char* mPlanetName;            // 0x0
    const char* mData[5];               // 0x4
    const char* mForceScenarioData[8];  // 0x18
};

struct UniqueEntry {
    const char* _0;
    const char* _4;
};

typedef NameObj* (*CreationFuncPtr)(const char *);

struct PlanetEntry {
    const char* mName;          // 0x0
    CreationFuncPtr mFunction;  // 0x4
};

namespace {
    static char* sArcName = "PlanetMapDataTable.arc";
    static char* sFileName = "PlanetMapDataTable.bcsv";

    // todo
    static UniqueEntry sUniquePlanetUniqueArchiveName[13] = {
        
    };

    // todo
    void makeSubModelName(const char **, const JMapInfo *, s32, const char *, const char *) {

    }

    static bool isDataForceLow(const PlanetMapData *pMapData) {
        for (int i = 0; i < 8; i++) {
            if (!MR::isEqualString(pMapData->mForceScenarioData[i], "Low")) {
                return true;
            }
        }

        return false;
    }
};


class PlanetMapCreator : public NameObj {
public:
    PlanetMapCreator(const char *);

    virtual ~PlanetMapCreator();

    void makeArchiveListPlanet(NameObjArchiveListCollector *, const JMapInfoIter &, const char *);
    void createPlanetMapDataTable(JMapInfo *);
    void addTableData(const JMapInfo *, s32);
    PlanetMapData* getTableData(const char *) const;
    bool isScenarioForceLow(const PlanetMapData *) const;

    PlanetMapData** mPlanetMapData; // 0xC
    s32 mTableCount;                // 0x10
};

class PlanetMapCreatorFunction {
public:
    static void makeArchiveList(NameObjArchiveListCollector *, const JMapInfoIter &, const char *);

    static bool isLoadArchiveAfterScenarioSelected(const char *);
    static bool isRegisteredObj(const char *);

    static CreationFuncPtr getPlanetMapCreator(const char *);
};