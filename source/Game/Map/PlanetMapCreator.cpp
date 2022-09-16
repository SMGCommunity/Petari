#include "Game/Map/PlanetMapCreator.h"
#include "Game/Scene/SceneObjHolder.h"
#include "Game/NameObj/NameObjFactory.h"
#include "Game/Util.h"
#include <cstdio>
#include <cstring>

#ifdef NON_MATCHING
// scheduling issues
PlanetMapCreator::PlanetMapCreator(const char *pName) : NameObj(pName) {
    mPlanetMapData = 0;
    const JMapInfo* iter = MR::createCsvParser(sArcName, sFileName);
    mTableCount = MR::getCsvDataElementNum(iter);
    mPlanetMapData = new PlanetMapData*[mTableCount];
    createPlanetMapDataTable((JMapInfo*)iter);
}
#endif

#ifdef NON_MATCHING
// regalloc issues
void PlanetMapCreator::makeArchiveListPlanet(NameObjArchiveListCollector *pArchiveList, const JMapInfoIter &rIter, const char *pName) {
    PlanetMapData* planetData = getTableData(pName);

    bool isForceLow = isScenarioForceLow(planetData);

    if (isForceLow) {
        pArchiveList->addArchive(planetData->mData[0]);
    }
    else {
        const char* planetName = planetData->mPlanetName;

        pArchiveList->addArchive(planetName);

        const char* tmp_1 = planetData->mData[0];

        if (tmp_1) {
            pArchiveList->addArchive(tmp_1);
        }

        const char* tmp_2 = planetData->mData[1];

        if (tmp_2) {
            pArchiveList->addArchive(tmp_2);
        }

        const char* tmp_3 = planetData->mData[2];

        if (tmp_3) {
            pArchiveList->addArchive(tmp_3);
        }

        const char* tmp_4 = planetData->mData[3];

        if (tmp_4) {
            pArchiveList->addArchive(tmp_4);
        }

        const char* tmp_5 = planetData->mData[4];

        if (tmp_5) {
            pArchiveList->addArchive(tmp_5);
        }

        const char* asdf = planetData->mPlanetName;

        for (int i = 0; i < 0xD; i++) {
            if (MR::isEqualString(asdf, sUniquePlanetUniqueArchiveName[i]._0)) {
                pArchiveList->addArchive(sUniquePlanetUniqueArchiveName[i]._4);
            }
        }
    }
}
#endif

void PlanetMapCreator::createPlanetMapDataTable(JMapInfo *pInfo) {
    for (int i = 0; i < mTableCount; i++) {
        PlanetMapData* data = new PlanetMapData();
        mPlanetMapData[i] = data;
        addTableData(pInfo, i);
    }
}

void PlanetMapCreator::addTableData(const JMapInfo *pInfo, s32 idx) {
    PlanetMapData* curData = mPlanetMapData[idx];
    MR::getCsvDataStr(&curData->mPlanetName, pInfo, "PlanetName", idx);
    makeSubModelName(&curData->mData[0], pInfo, idx, "LowFlag", "Low");
    makeSubModelName(&curData->mData[1], pInfo, idx, "MiddleFlag", "Middle");
    makeSubModelName(&curData->mData[2], pInfo, idx, "BloomFlag", "Bloom");
    makeSubModelName(&curData->mData[3], pInfo, idx, "WaterFlag", "Water");
    makeSubModelName(&curData->mData[4], pInfo, idx, "IndirectFlag", "Indirect");

    for (int i = 0; i < 5; i++) {
        if (curData->mData[i]) {
            if (!MR::isExistModel(curData->mData[i])) {
                curData->mData[i] = 0;
            }
        }
    }

    for (int i = 0; i < 8; i++) {
        char data[0x20];
        snprintf(data, 0x20, "ForceLowScenarioName%d", i);
        MR::getCsvDataStr(&curData->mForceScenarioData[i], pInfo, data, idx);
    }
}

PlanetMapData* PlanetMapCreator::getTableData(const char *pName) const {
    for (int i = 0; i < mTableCount; i++) {
        if (MR::isEqualString(pName, mPlanetMapData[i]->mPlanetName)) {
            return mPlanetMapData[i];
        }
    }

    return 0;
}

bool PlanetMapCreator::isScenarioForceLow(const PlanetMapData *pData) const {
    bool isForce = isDataForceLow(pData);

    if (!isForce) {
        return false;
    }

    char name[0x40];
    snprintf(name, 0x40, "%s_%d", MR::getCurrentStageName(), MR::getCurrentScenarioNo());

    for (int i = 0; i < 8; i++) {
        if (MR::isEqualString(pData->mForceScenarioData[i], "Low")) {
            return false;
        }

        if (MR::isEqualString(name, pData->mForceScenarioData[i])) {
            return true;
        }
    }

    return false;
}

void PlanetMapCreatorFunction::makeArchiveList(NameObjArchiveListCollector *pArchiveList, const JMapInfoIter &rIter, const char *pName) {
    reinterpret_cast<PlanetMapCreator*>(MR::getSceneObjHolder()->getObj(SceneObj_PlanetMapCreator))->makeArchiveListPlanet(pArchiveList, rIter, pName);
}

// PlanetMapCreatorFunction::getPlanetMapCreator

bool PlanetMapCreatorFunction::isLoadArchiveAfterScenarioSelected(const char *pArchive) {
    PlanetMapCreator* creator = reinterpret_cast<PlanetMapCreator*>(MR::getSceneObjHolder()->getObj(SceneObj_PlanetMapCreator));
    bool doesHaveTableData = creator->getTableData(pArchive);

    return (!doesHaveTableData) ? false : isDataForceLow(reinterpret_cast<PlanetMapCreator*>(MR::getSceneObjHolder()->getObj(SceneObj_PlanetMapCreator))->getTableData(pArchive));

    return false;
}

bool PlanetMapCreatorFunction::isRegisteredObj(const char *pArchive) {
    PlanetMapCreator* creator = reinterpret_cast<PlanetMapCreator*>(MR::getSceneObjHolder()->getObj(SceneObj_PlanetMapCreator));
    return creator->getTableData(pArchive);
}

PlanetMapCreator::~PlanetMapCreator() {

}

// this is only here to generate the table until I match functions that use it
const char* someFunction() {
    return sUniquePlanetCreateFuncTable[0].mName;
}