#include "Game/Map/PlanetMapCreator.hpp"
#include "Game/Map.hpp"
#include "Game/MapObj.hpp"
#include "Game/NameObj/NameObjArchiveListCollector.hpp"
#include "Game/NameObj/NameObjFactory.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Util/ModelUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SceneUtil.hpp"
#include "Game/Util/StringUtil.hpp"
#include <cstdio>

namespace {
    static const char* const sArcName = "PlanetMapDataTable.arc";
    static const char* const sFileName = "PlanetMapDataTable.bcsv";

    void makeSubModelName(const char** ppDst, const JMapInfo* pIter, s32 idx, const char* pKey, const char* pSuffix) {
        s32 data;
        MR::getCsvDataS32(&data, pIter, pKey, idx);

        const char* pPlanetName;
        MR::getCsvDataStr(&pPlanetName, pIter, "PlanetName", idx);

        if (data != 0) {
            s32 size = strlen(pPlanetName) + strlen(pSuffix) + 1;
            char* buf = new char[size];
            snprintf(buf, size, "%s%s", pPlanetName, pSuffix);

            *ppDst = buf;
        } else {
            *ppDst = nullptr;
        }
    }

    static const PlanetEntry sUniquePlanetCreateFuncTable[] = {
        {
            "BeamGoRoundPlanet",
            createNameObj< BeamGoRoundPlanet >,
        },
        {
            "BumpAppearPlanet",
            createNameObj< BumpAppearPlanet >,
        },
        {
            "ChoConveyorPlanetB",
            createNameObj< RailPlanetMap >,
        },
        {
            "ChoConveyorPlanetD",
            createNameObj< RotateMoveObj >,
        },
        {
            "DinoPackunBattlePlanet",
            createNameObj< FurPlanetMap >,
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
            createNameObj< PlanetMapFarClippable >,
        },
        {
            "FlagDiscPlanetC",
            createNameObj< PlanetMapFarClippable >,
        },
        {
            "FlagDiscPlanetD",
            createNameObj< RotateMoveObj >,
        },
        {
            "FlagDiscPlanetE",
            createNameObj< PlanetMapFarClippable >,
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
            createNameObj< FurPlanetMap >,
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
            createNameObj< FlexibleSphere >,
        },
        {
            "SnowCapsulePlanet",
            createNameObj< SnowCapsulePlanet >,
        },
        {
            "TeresaRoomPlanet",
            createNameObj< PlanetMapAnimLow >,
        },
        {
            "TridentPlanet",
            createNameObj< AstroSimpleObj >,
        },
        {
            "QuestionBoxPlanetA",
            createNameObj< QuestionBoxGalleryObj >,
        },
        {
            "QuestionBoxPlanetB",
            createNameObj< QuestionBoxGalleryObj >,
        },
        {
            "QuestionBoxPlanetC",
            createNameObj< QuestionBoxGalleryObj >,
        },
        {
            "QuestionBoxPlanetD",
            createNameObj< QuestionBoxGalleryObj >,
        },
        {
            "QuestionBoxPlanetE",
            createNameObj< QuestionBoxGalleryObj >,
        },
        {
            "Quicksand2DPlanet",
            createNameObj< RailPlanetMap >,
        },
        {
            "ReverseGravityRoomPlanet",
            createNameObj< ReverseGravityRoomPlanet >,
        },
        {
            "SandStreamHighTowerPlanet",
            createNameObj< RailPlanetMap >,
        },
        {
            "SandStreamJointPlanetA",
            createNameObj< RailPlanetMap >,
        },
        {
            "SandStreamJointPlanetB",
            createNameObj< RailPlanetMap >,
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

    const PlanetEntry* findUniquePlanetEntry(const char* pName) {
        for (u32 i = 0; i < ARRAY_SIZE(::sUniquePlanetCreateFuncTable); i++) {
            const PlanetEntry* pEntry = &::sUniquePlanetCreateFuncTable[i];
            if (MR::isEqualString(pName, pEntry->mName)) {
                return pEntry;
            }
        }

        return nullptr;
    }

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
PlanetMapCreator::PlanetMapCreator(const char* pName) : NameObj(pName), mPlanetMapData() {
    JMapInfo* pIter = MR::createCsvParser(::sArcName, ::sFileName);

    mTableCount = MR::getCsvDataElementNum(pIter);
    mPlanetMapData = new PlanetMapData*[mTableCount];
    createPlanetMapDataTable(pIter);
}

CreatorFuncPtr PlanetMapCreator::getCreateFunc(const char* pParam1) {
    if (isScenarioForceLow(getTableData(pParam1))) {
        return createNameObj< PlanetMapWithoutHighModel >;
    }

    const PlanetEntry* pEntry = ::findUniquePlanetEntry(pParam1);

    if (pEntry != nullptr) {
        return pEntry->mCreateFunc;
    }

    return createNameObj< PlanetMap >;
}

void PlanetMapCreator::makeArchiveListPlanet(NameObjArchiveListCollector* pCollector, const JMapInfoIter& rIter, const char* pName) {
    PlanetMapData* pTableData = getTableData(pName);
    if (isScenarioForceLow(pTableData)) {
        pCollector->addArchive(pTableData->mData[0]);
        return;
    }

    pCollector->addArchive(pTableData->mPlanetName);

    const char* pData0 = pTableData->mData[0];
    if (pData0 != nullptr) {
        pCollector->addArchive(pData0);
    }

    const char* pData1 = pTableData->mData[1];
    if (pData1 != nullptr) {
        pCollector->addArchive(pData1);
    }

    const char* pData2 = pTableData->mData[2];
    if (pData2 != nullptr) {
        pCollector->addArchive(pData2);
    }

    const char* pData3 = pTableData->mData[3];
    if (pData3 != nullptr) {
        pCollector->addArchive(pData3);
    }

    const char* pData4 = pTableData->mData[4];
    if (pData4 != nullptr) {
        pCollector->addArchive(pData4);
    }

    const char* pPlanetName = pTableData->mPlanetName;
    for (u32 i = 0; i < ARRAY_SIZE(::sUniquePlanetUniqueArchiveName); i++) {
        const UniqueEntry* pUniqueEntry = &::sUniquePlanetUniqueArchiveName[i];
        if (MR::isEqualString(pPlanetName, pUniqueEntry->_0)) {
            pCollector->addArchive(pUniqueEntry->_4);
        }
    }
}

void PlanetMapCreator::createPlanetMapDataTable(JMapInfo* pInfo) {
    for (int i = 0; i < mTableCount; i++) {
        mPlanetMapData[i] = new PlanetMapData();

        addTableData(pInfo, i);
    }
}

void PlanetMapCreator::addTableData(const JMapInfo* pInfo, s32 idx) {
    PlanetMapData* curData = mPlanetMapData[idx];
    MR::getCsvDataStr(&curData->mPlanetName, pInfo, "PlanetName", idx);
    ::makeSubModelName(&curData->mData[0], pInfo, idx, "LowFlag", "Low");
    ::makeSubModelName(&curData->mData[1], pInfo, idx, "MiddleFlag", "Middle");
    ::makeSubModelName(&curData->mData[2], pInfo, idx, "BloomFlag", "Bloom");
    ::makeSubModelName(&curData->mData[3], pInfo, idx, "WaterFlag", "Water");
    ::makeSubModelName(&curData->mData[4], pInfo, idx, "IndirectFlag", "Indirect");

    for (int i = 0; i < 5; i++) {
        if (curData->mData[i] != nullptr) {
            if (!MR::isExistModel(curData->mData[i])) {
                curData->mData[i] = nullptr;
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
    bool isForce = ::isDataForceLow(pData);

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

void PlanetMapCreatorFunction::makeArchiveList(NameObjArchiveListCollector* pCollector, const JMapInfoIter& rIter, const char* pName) {
    MR::getSceneObj< PlanetMapCreator >(SceneObj_PlanetMapCreator)->makeArchiveListPlanet(pCollector, rIter, pName);
}

CreatorFuncPtr PlanetMapCreatorFunction::getPlanetMapCreator(const char* pParam1) {
    return MR::getSceneObj< PlanetMapCreator >(SceneObj_PlanetMapCreator)->getCreateFunc(pParam1);
}

bool PlanetMapCreatorFunction::isLoadArchiveAfterScenarioSelected(const char* pArchive) {
    bool isExistTableData = MR::getSceneObj< PlanetMapCreator >(SceneObj_PlanetMapCreator)->getTableData(pArchive) != nullptr;

    if (!isExistTableData) {
        return false;
    }

    return ::isDataForceLow(MR::getSceneObj< PlanetMapCreator >(SceneObj_PlanetMapCreator)->getTableData(pArchive));
}

bool PlanetMapCreatorFunction::isRegisteredObj(const char* pArchive) {
    return MR::getSceneObj< PlanetMapCreator >(SceneObj_PlanetMapCreator)->getTableData(pArchive) != nullptr;
}
