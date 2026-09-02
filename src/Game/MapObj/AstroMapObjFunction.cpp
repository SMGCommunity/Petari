#include "Game/MapObj/AstroMapObjFunction.hpp"
#include "Game/Demo/AstroDemoFunction.hpp"
#include "Game/NameObj/NameObjArchiveListCollector.hpp"
#include "Game/Screen/GalaxyNamePlate.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/AreaObjUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/EventUtil.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/MessageUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/StringUtil.hpp"
#include <cstdio>

namespace {
    const char* const cAstroNamePlateArcName = "AstroNamePlateData";
    const char* const cAstroNamePlateFileName = "AstroNamePlateData.bcsv";
    const char* cAstroDomeNameTable[] = {"AstroDomeObservatory", "AstroDomeWell",    "AstroDomeKitchen",
                                         "AstroDomeBedRoom",     "AstroDomeMachine", "AstroDomeTower"};
    const char* cAstroDomeSkyNameTable[] = {"AstroDomeSkyA", "AstroDomeSkyB", "AstroDomeSkyC", "AstroDomeSkyA", "AstroDomeSkyB", "AstroDomeSkyC"};
    const char* cAstroDomeEntranceNameTable[] = {"AstroDomeEntranceObservatory", "AstroDomeEntranceWell",    "AstroDomeEntranceKitchen",
                                                 "AstroDomeEntranceBedRoom",     "AstroDomeEntranceMachine", "AstroDomeEntranceTower"};
    const char* cAstroStarPlateNameTable[] = {"AstroStarPlateObservatory", "AstroStarPlateWell",    "AstroStarPlateKitchen",
                                              "AstroStarPlateBedRoom",     "AstroStarPlateMachine", "AstroStarPlateTower"};
    // const char* sNamePlatePosDataTable = ...;

    s32 getNumGrandStarForRevival(const char* pObjName, s32 domeId) {
        const char* modelName = AstroMapObjFunction::getModelName(pObjName, domeId);

        if (MR::isEqualString(modelName, "AstroDomeEntranceObservatory")) {
            return 1;
        }

        if (MR::isEqualString(modelName, "AstroDomeEntranceWell")) {
            return 1;
        }

        if (MR::isEqualString(modelName, "AstroBaseA")) {
            return 1;
        }

        if (MR::isEqualString(modelName, "AstroBaseCenterB")) {
            return 1;
        }

        if (MR::isEqualString(modelName, "AstroRotateStepA")) {
            return 1;
        }

        if (MR::isEqualString(modelName, "AstroDecoratePartsA")) {
            return 1;
        }

        if (MR::isEqualString(modelName, "AstroDecoratePartsGearA")) {
            return 1;
        }

        if (MR::isEqualString(modelName, "AstroParking")) {
            return 1;
        }

        if (MR::isEqualString(modelName, "AstroDomeEntranceKitchen")) {
            return 3;
        }

        if (MR::isEqualString(modelName, "AstroDomeEntranceBedRoom")) {
            return 3;
        }

        if (MR::isEqualString(modelName, "AstroBaseB")) {
            return 3;
        }

        if (MR::isEqualString(modelName, "AstroBaseKitchen")) {
            return 3;
        }

        if (MR::isEqualString(modelName, "AstroLibrary")) {
            return 3;
        }

        if (MR::isEqualString(modelName, "AstroCountDownPlate")) {
            return 5;
        }

        if (MR::isEqualString(modelName, "AstroDomeEntranceMachine")) {
            return 5;
        }

        if (MR::isEqualString(modelName, "AstroDomeEntranceTower")) {
            return 5;
        }

        if (MR::isEqualString(modelName, "AstroBaseC")) {
            return 5;
        }

        if (MR::isEqualString(modelName, "AstroBaseCenterA")) {
            return 5;
        }

        if (MR::isEqualString(modelName, "AstroBaseCenterTop")) {
            return 5;
        }

        if (MR::isEqualString(modelName, "AstroRotateStepB")) {
            return 5;
        }

        s32 num = -1;

        if (domeId != -1) {
            num = domeId;
        }

        return num;
    }

    s32 getNumGrandStarForOpen(const char* pObjName, s32 domeId) {
        s32 num = -1;

        if (domeId != -1) {
            num = domeId;
        }

        return num;
    }
};  // namespace

void AstroMapObjFunction::makeArchiveListFromArg0(NameObjArchiveListCollector* pCollector, const JMapInfoIter& rIter) {
    s32 domeId = getDomeIdFromArg0(rIter);

    const char* objName;
    MR::getObjectName(&objName, rIter);

    const char* modelName = getModelName(objName, domeId);

    pCollector->addArchive(modelName);

    if (MR::isEqualString(modelName, "AstroDomeEntrance")) {
        makeArchiveListAstroNamePlate(pCollector, rIter);
    }
}

void AstroMapObjFunction::makeArchiveListAstroNamePlate(NameObjArchiveListCollector* pCollector, const JMapInfoIter& rIter) {
    pCollector->addArchive("GalaxyNamePlate");
    pCollector->addArchive(::cAstroNamePlateArcName);
}

bool AstroMapObjFunction::isAlreadyRevival(const char* pObjName, s32 domeId) {
    if (MR::isEqualString(pObjName, "AstroChildRoom")) {
        return MR::isOnGameEventFlagChildsRoom();
    }

    if (MR::isEqualString(pObjName, "TridentPlanet")) {
        return MR::isOnGameEventFlagGreenDriver();
    }

    s32 openedNum = AstroDemoFunction::getOpenedAstroDomeNum();

    return openedNum >= ::getNumGrandStarForRevival(pObjName, domeId);
}

bool AstroMapObjFunction::isEnableRevival(const char* pObjName, s32 domeId) {
    if (MR::isEqualString(pObjName, "AstroChildRoom")) {
        return true;
    }

    s32 openedNum = AstroDemoFunction::getOpenedAstroDomeNum();

    return ::getNumGrandStarForRevival(pObjName, domeId) == openedNum + 1;
}

bool AstroMapObjFunction::isAlreadyOpen(const char* pObjName, s32 domeId) {
    if (MR::isEqualString(pObjName, "AstroLibrary")) {
        return MR::isOnGameEventFlagLibraryRoom();
    }

    s32 openedNum = AstroDemoFunction::getOpenedAstroDomeNum();

    return openedNum >= ::getNumGrandStarForOpen(pObjName, domeId);
}

bool AstroMapObjFunction::isEnableOpen(const char* pObjName, s32 domeId) {
    s32 openedNum = AstroDemoFunction::getOpenedAstroDomeNum();

    return ::getNumGrandStarForOpen(pObjName, domeId) == openedNum + 1;
}

s32 AstroMapObjFunction::getDomeIdFromArg0(const JMapInfoIter& rIter) {
    s32 arg0 = -1;
    MR::getJMapInfoArg0NoInit(rIter, &arg0);

    return arg0;
}

const char* AstroMapObjFunction::getModelName(const char* pObjName, s32 domeId) {
    const char** nameTable = nullptr;

    if (MR::isEqualString(pObjName, "AstroDome")) {
        nameTable = ::cAstroDomeNameTable;
    } else if (MR::isEqualString(pObjName, "AstroDomeSky")) {
        nameTable = ::cAstroDomeSkyNameTable;
    } else if (MR::isEqualString(pObjName, "AstroDomeEntrance")) {
        nameTable = ::cAstroDomeEntranceNameTable;
    } else if (MR::isEqualString(pObjName, "AstroStarPlate")) {
        nameTable = ::cAstroStarPlateNameTable;
    }

    if (nameTable != nullptr) {
        return nameTable[domeId - 1];
    }

    return pObjName;
}

const JMapInfo* AstroMapObjFunction::createAstroNamePlateParser() {
    char arcName[64];
    snprintf(arcName, sizeof(arcName), "%s.arc", ::cAstroNamePlateArcName);

    return MR::createCsvParser(arcName, ::cAstroNamePlateFileName);
}

bool AstroMapObjFunction::trySetAndShowNamePlate(GalaxyNamePlate* pNamePlate, const JMapInfo* pInfo, const JGeometry::TVec3< float >& rPos,
                                                 const char* pObjName, s32 domeId, bool param6) {
    AreaObj* astroOverlookAreaObj = MR::getCurrentAstroOverlookAreaObj();

    if (astroOverlookAreaObj == nullptr) {
        return false;
    }

    if (!param6 && !isAlreadyRevival(pObjName, domeId)) {
        return false;
    }

    if (param6 && !isAlreadyOpen(pObjName, domeId)) {
        return false;
    }

    char offsetYId[16];
    snprintf(offsetYId, sizeof(offsetYId), "OffsetY%d", MR::getAreaObjArg(astroOverlookAreaObj, 0));

    char upperFlagId[16];
    snprintf(upperFlagId, sizeof(upperFlagId), "UpperFlag%d", MR::getAreaObjArg(astroOverlookAreaObj, 0));

    const char* modelName = getModelName(pObjName, domeId);
    s32 i;
    s32 num = MR::getCsvDataElementNum(pInfo);

    for (i = 0; i < num; i++) {
        const char* csvModelName;
        MR::getCsvDataStr(&csvModelName, pInfo, "ModelName", i);

        if (MR::isEqualString(modelName, csvModelName)) {
            break;
        }
    }

    f32 offsetY;
    MR::getCsvDataF32(&offsetY, pInfo, offsetYId, i);

    bool upperFlag;
    MR::getCsvDataBool(&upperFlag, pInfo, upperFlagId, i);

    TVec3f pos = rPos;
    pos.y += offsetY;

    pNamePlate->setPos3D(pos);

    const wchar_t* msg;

    if (MR::isEqualString(pObjName, "AstroDomeEntrance")) {
        msg = MR::getScenarioNameOnCurrentLanguage("AstroDome", domeId);
    } else if (MR::isEqualString(pObjName, "AstroLibrary")) {
        msg = MR::getGameMessageDirect("AstroName_LibraryRoom");
    } else if (MR::isEqualString(pObjName, "AstroChildRoom")) {
        msg = MR::getGameMessageDirect("AstroName_ChildRoom");
    } else if (MR::isEqualString(pObjName, "AstroParking")) {
        msg = MR::getGameMessageDirect("AstroName_ParkingLot");
    } else if (MR::isEqualString(pObjName, "TridentPlanet")) {
        msg = MR::getGameMessageDirect("AstroName_Trident");
    } else {
        msg = nullptr;
    }

    pNamePlate->show(msg, upperFlag);

    return true;
}

bool AstroMapObjFunction::tryRegisterMultiDemoAndFunction(const char* pObjName, LiveActor* pActor, const JMapInfoIter& rIter,
                                                          const MR::FunctorBase& rFunctor) {
    const char* modelName = getModelName(pObjName, getDomeIdFromArg0(rIter));
    const char* demoName = nullptr;

    if (MR::isEqualString(modelName, ::cAstroDomeEntranceNameTable[1])) {
        demoName = "グランドスター１帰還";
    } else if (MR::isEqualString(modelName, ::cAstroDomeEntranceNameTable[3])) {
        demoName = "グランドスター３帰還";
    } else if (MR::isEqualString(modelName, ::cAstroDomeEntranceNameTable[5])) {
        demoName = "グランドスター５帰還";
    }

    if (demoName != nullptr && MR::isDemoExist(demoName) && MR::tryRegisterDemoCast(pActor, demoName, rIter)) {
        MR::tryRegisterDemoActionFunctorDirect(pActor, rFunctor, demoName, nullptr);

        return true;
    }

    return false;
}

void AstroMapObjFunction::initilizePeculiar(const char* pObjName, LiveActor* pActor, const JMapInfoIter& rIter) {
    s32 domeId = getDomeIdFromArg0(rIter);

    if (!MR::isEqualString(getModelName(pObjName, domeId), ::cAstroDomeEntranceNameTable[0])) {
        return;
    }

    MR::needStageSwitchWriteA(pActor, rIter);

    if (MR::isKinopioExplorerRescued()) {
        return;
    }

    MR::onSwitchA(pActor);
}
