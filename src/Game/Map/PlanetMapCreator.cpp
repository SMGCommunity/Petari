#include "Game/Map/PlanetMapCreator.hpp"
#include "Game/AreaObj.hpp"
#include "Game/Boss.hpp"
#include "Game/Enemy.hpp"
#include "Game/Gravity.hpp"
#include "Game/Map.hpp"
#include "Game/MapObj.hpp"
#include "Game/NPC.hpp"
#include "Game/NameObj.hpp"
#include "Game/Ride.hpp"
#include "Game/Scene.hpp"
#include "Game/Util.hpp"
#include <cstdio>
#include <cstring>

namespace {
    static const PlanetEntry sUniquePlanetCreateFuncTable[] = {
        {
            "BeamGoRoundPlanet",
            /* createNameObj<BeamGoRoundPlanet>, */
        },
        {
            "BumpAppearPlanet",
            /* createNameObj<BumpAppearPlanet>, */
        },
        {
            "ChoConveyorPlanetB",
            /* createNameObj<RailPlanetMap>, */
        },
        {
            "ChoConveyorPlanetD",
            createNameObj< RotateMoveObj >,
        },
        {
            "DinoPackunBattlePlanet",
            /* createNameObj<FurPlanetMap>, */
        },
        {
            "DarkHopperPlanetA",
            createNameObj< SimpleClipPartsObj >,
        },
        {
            "DarkHopperPlanetB",
            createNameObj< SimpleClipPartsObj >,
        },
        {
            "DarkHopperPlanetC",
            createNameObj< SimpleClipPartsObj >,
        },
        {
            "DarkHopperPlanetD",
            createNameObj< SimpleClipPartsObj >,
        },
        {
            "DarknessRoomPlanet",
            createNameObj< SimpleClipPartsObj >,
        },
        {
            "FlagDiscPlanetB",
            /* createNameObj<PlanetMapFarClippable>, */
        },
        {
            "FlagDiscPlanetC",
            /* createNameObj<PlanetMapFarClippable>, */
        },
        {
            "FlagDiscPlanetD",
            createNameObj< RotateMoveObj >,
        },
        {
            "FlagDiscPlanetE",
            /* createNameObj<PlanetMapFarClippable>, */
        },
        {
            "HatchWaterPlanet",
            createNameObj< HatchWaterPlanet >,
        },
        {
            "HeavensDoorInsidePlanet",
            createNameObj< SimpleMapObj >,
        },
        {
            "HoneyQueenPlanet",
            /* createNameObj<FurPlanetMap>, */
        },
        {
            "LavaJamboSunPlanet",
            createNameObj< LavaJamboSunPlanet >,
        },
        {
            "LavaRotatePlanet",
            createNameObj< LavaSunPlanet >,
        },
        {
            "LavaDomedPlanet",
            createNameObj< LavaSunPlanet >,
        },
        {
            "MarblePlanet",
            createNameObj< MarblePlanet >,
        },
        {
            "PeachCastleGardenPlanet",
            createNameObj< PeachCastleGardenPlanet >,
        },
        {
            "SandCapsuleInsidePlanet",
            createNameObj< SandCapsuleInsidePlanet >,
        },
        {
            "ScaleDownRelayPlanet",
            /* createNameObj<FlexibleSphere>, */
        },
        {
            "SnowCapsulePlanet",
            createNameObj< SnowCapsulePlanet >,
        },
        {
            "TeresaRoomPlanet",
            /* createNameObj<PlanetMapAnimLow>, */
        },
        {
            "TridentPlanet",
            createNameObj< AstroSimpleObj >,
        },
        {
            "QuestionBoxPlanetA",
            /* createNameObj<QuestionBoxGalleryObj>, */
        },
        {
            "QuestionBoxPlanetB",
            /* createNameObj<QuestionBoxGalleryObj>, */
        },
        {
            "QuestionBoxPlanetC",
            /* createNameObj<QuestionBoxGalleryObj>, */
        },
        {
            "QuestionBoxPlanetD",
            /* createNameObj<QuestionBoxGalleryObj>, */
        },
        {
            "QuestionBoxPlanetE",
            /* createNameObj<QuestionBoxGalleryObj>, */
        },
        {
            "Quicksand2DPlanet",
            /* createNameObj<RailPlanetMap>, */
        },
        {
            "ReverseGravityRoomPlanet",
            createNameObj< ReverseGravityRoomPlanet >,
        },
        {
            "SandStreamHighTowerPlanet",
            /* createNameObj<RailPlanetMap>, */
        },
        {
            "SandStreamJointPlanetA",
            /* createNameObj<RailPlanetMap>, */
        },
        {
            "SandStreamJointPlanetB",
            /* createNameObj<RailPlanetMap>, */
        },
        {
            "StarDustStartPlanet",
            createNameObj< RotateMoveObj >,
        },
        {
            "WormEatenPlanet",
            createNameObj< WormEatenPlanet >,
        },
    };
    static const UniqueEntry sUniquePlanetUniqueArchiveName[] = {
        {
            "BeamGoRoundPlanet",
            "BeamGoRoundBeam",
        },
        {
            "BeamGoRoundPlanet",
            "BeamGoRoundBeamVolume",
        },
        {
            "BeamGoRoundPlanet",
            "BeamGoRoundBeamBloom",
        },
        {
            "MarblePlanet",
            "MarblePlanetCore",
        },
        {
            "MarblePlanet",
            "MarblePlanetElectron",
        },
        {
            "MarblePlanet",
            "MarblePlanetElectronShadow",
        },
        {
            "OceanRingPlanet",
            "OceanRingPlanetLowInWater",
        },
        {
            "WormEatenPlanet",
            "GreenCaterpillarBigFace",
        },
        {
            "WormEatenPlanet",
            "GreenCaterpillarBigFaceLow",
        },
        {
            "WormEatenPlanet",
            "GreenCaterpillarBigBody",
        },
        {
            "WormEatenPlanet",
            "GreenCaterpillarBigBodyLow",
        },
        {
            "WormEatenPlanet",
            "WormEatenHill",
        },
        {
            "WormEatenPlanet",
            "WormEatenPlanetLow",
        },
    };
    static const char* sArcName = "PlanetMapDataTable.arc";
    static const char* sFileName = "PlanetMapDataTable.bcsv";

    void makeSubModelName(const char**, const JMapInfo*, s32, const char*, const char*) {}

    static bool isDataForceLow(const PlanetMapData* pMapData) {
        for (int i = 0; i < 8; i++) {
            if (!MR::isEqualString(pMapData->mForceScenarioData[i], "Low")) {
                return true;
            }
        }

        return false;
    }
};  // namespace

// FIXME: Scheduling issues
PlanetMapCreator::PlanetMapCreator(const char* pName) : NameObj(pName), mPlanetMapData(nullptr) {
    JMapInfo* pIter = MR::createCsvParser(sArcName, sFileName);

    mTableCount = MR::getCsvDataElementNum(pIter);
    mPlanetMapData = new PlanetMapData*[mTableCount];
    createPlanetMapDataTable(pIter);
}

CreatorFuncPtr PlanetMapCreator::getCreateFunc(const char* pParam1) {
    if (isScenarioForceLow(getTableData(pParam1))) {
        return nullptr;  // createNameObj<PlanetMapWithoutHighModel>
    }

    const PlanetEntry* pEntry = nullptr;

    for (u32 i = 0; i < sizeof(sUniquePlanetCreateFuncTable) / sizeof(*sUniquePlanetCreateFuncTable); i++) {
        pEntry = &sUniquePlanetCreateFuncTable[i];

        if (!MR::isEqualString(pParam1, sUniquePlanetCreateFuncTable[i].mName)) {
            break;
        }
    }

    if (pEntry != nullptr) {
        return pEntry->mCreateFunc;
    }

    return nullptr;  // createNameObj<PlanetMap>
}

#ifdef NON_MATCHING
// regalloc issues
void PlanetMapCreator::makeArchiveListPlanet(NameObjArchiveListCollector* pArchiveList, const JMapInfoIter& rIter, const char* pName) {
    PlanetMapData* pTableData = getTableData(pName);

    bool isForceLow = isScenarioForceLow(pTableData);

    if (isForceLow) {
        pArchiveList->addArchive(pTableData->mData[0]);
    } else {
        const char* planetName = pTableData->mPlanetName;

        pArchiveList->addArchive(planetName);

        const char* tmp_1 = pTableData->mData[0];

        if (tmp_1) {
            pArchiveList->addArchive(tmp_1);
        }

        const char* tmp_2 = pTableData->mData[1];

        if (tmp_2) {
            pArchiveList->addArchive(tmp_2);
        }

        const char* tmp_3 = pTableData->mData[2];

        if (tmp_3) {
            pArchiveList->addArchive(tmp_3);
        }

        const char* tmp_4 = pTableData->mData[3];

        if (tmp_4) {
            pArchiveList->addArchive(tmp_4);
        }

        const char* tmp_5 = pTableData->mData[4];

        if (tmp_5) {
            pArchiveList->addArchive(tmp_5);
        }

        const char* pPlanetName = pTableData->mPlanetName;

        for (int i = 0; i < sizeof(sUniquePlanetUniqueArchiveName) / sizeof(*sUniquePlanetUniqueArchiveName); i++) {
            if (MR::isEqualString(pPlanetName, sUniquePlanetUniqueArchiveName[i]._0)) {
                pArchiveList->addArchive(sUniquePlanetUniqueArchiveName[i]._4);
            }
        }
    }
}
#endif

void PlanetMapCreator::createPlanetMapDataTable(JMapInfo* pInfo) {
    for (int i = 0; i < mTableCount; i++) {
        mPlanetMapData[i] = new PlanetMapData();

        addTableData(pInfo, i);
    }
}

void PlanetMapCreator::addTableData(const JMapInfo* pInfo, s32 idx) {
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
        char data[32];
        snprintf(data, sizeof(data), "ForceLowScenarioName%d", i);

        MR::getCsvDataStr(&curData->mForceScenarioData[i], pInfo, data, idx);
    }
}

PlanetMapData* PlanetMapCreator::getTableData(const char* pName) const {
    for (int i = 0; i < mTableCount; i++) {
        if (MR::isEqualString(pName, mPlanetMapData[i]->mPlanetName)) {
            return mPlanetMapData[i];
        }
    }

    return nullptr;
}

bool PlanetMapCreator::isScenarioForceLow(const PlanetMapData* pData) const {
    bool isForce = isDataForceLow(pData);

    if (!isForce) {
        return false;
    }

    char name[64];
    snprintf(name, sizeof(name), "%s_%d", MR::getCurrentStageName(), MR::getCurrentScenarioNo());

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

void PlanetMapCreatorFunction::makeArchiveList(NameObjArchiveListCollector* pArchiveList, const JMapInfoIter& rIter, const char* pName) {
    MR::getSceneObj< PlanetMapCreator >(SceneObj_PlanetMapCreator)->makeArchiveListPlanet(pArchiveList, rIter, pName);
}

// PlanetMapCreatorFunction::getPlanetMapCreator

bool PlanetMapCreatorFunction::isLoadArchiveAfterScenarioSelected(const char* pArchive) {
    bool isExistTableData = MR::getSceneObj< PlanetMapCreator >(SceneObj_PlanetMapCreator)->getTableData(pArchive);

    if (!isExistTableData) {
        return false;
    }

    return isDataForceLow(MR::getSceneObj< PlanetMapCreator >(SceneObj_PlanetMapCreator)->getTableData(pArchive));
}

bool PlanetMapCreatorFunction::isRegisteredObj(const char* pArchive) {
    return MR::getSceneObj< PlanetMapCreator >(SceneObj_PlanetMapCreator)->getTableData(pArchive);
}
