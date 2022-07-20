#pragma once

#include "Game/NameObj/NameObj.h"
#include "Game/NameObj/NameObjArchiveListCollector.h"
#include "Game/MapObj.h"
#include "Game/Util.h"

struct PlanetMapData {
    const char* mPlanetName;            // _0
    const char* mData[5];               // _4
    const char* mForceScenarioData[8];  // _18
};

struct UniqueEntry {
    const char* _0;
    const char* _4;
};

typedef NameObj* (*CreationFuncPtr)(const char *);

struct PlanetEntry {
    const char* mName;          // _0
    CreationFuncPtr mFunction;  // _4
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

    PlanetMapData** mPlanetMapData; // _C
    s32 mTableCount;                // _10
};

class PlanetMapCreatorFunction {
public:
    static void makeArchiveList(NameObjArchiveListCollector *, const JMapInfoIter &, const char *);

    static bool isLoadArchiveAfterScenarioSelected(const char *);
    static bool isRegisteredObj(const char *);

    static CreationFuncPtr getPlanetMapCreator(const char *);
};