#include "Game/MapObj/AstroMapObjFunction.hpp"
#include "Game/Demo/AstroDemoFunction.hpp"
#include <cstdio>

namespace {
    s32 getNumGrandStarForRevival(const char *pName, s32 domeId) {
        const char* modelName = AstroMapObjFunction::getModelName(pName, domeId);

        if ( MR::isEqualString(modelName, "AstroDomeEntranceObservatory") )
            return 1;
        if ( MR::isEqualString(modelName, "AstroDomeEntranceWell") )
            return 1;
        if ( MR::isEqualString(modelName, "AstroBaseA") )
            return 1;
        if ( MR::isEqualString(modelName, "AstroBaseCenterB") )
            return 1;
        if ( MR::isEqualString(modelName, "AstroRotateStepA") )
            return 1;
        if ( MR::isEqualString(modelName, "AstroDecoratePartsA") )
            return 1;
        if ( MR::isEqualString(modelName, "AstroDecoratePartsGearA") )
            return 1;
        if ( MR::isEqualString(modelName, "AstroParking") )
            return 1;
        if ( MR::isEqualString(modelName, "AstroDomeEntranceKitchen") )
            return 3;
        if ( MR::isEqualString(modelName, "AstroDomeEntranceBedRoom") )
            return 3;
        if ( MR::isEqualString(modelName, "AstroBaseB") )
            return 3;
        if ( MR::isEqualString(modelName, "AstroBaseKitchen") )
            return 3;
        if ( MR::isEqualString(modelName, "AstroLibrary") )
            return 3;
        if ( MR::isEqualString(modelName, "AstroCountDownPlate") )
            return 5;
        if ( MR::isEqualString(modelName, "AstroDomeEntranceMachine") )
            return 5;
        if ( MR::isEqualString(modelName, "AstroDomeEntranceTower") )
            return 5;
        if ( MR::isEqualString(modelName, "AstroBaseC") )
            return 5;
        if ( MR::isEqualString(modelName, "AstroBaseCenterA") )
            return 5;
        if ( MR::isEqualString(modelName, "AstroBaseCenterTop") )
            return 5;
        if ( MR::isEqualString(modelName, "AstroRotateStepB") )
            return 5;

        s32 res = -1;
        if ( domeId != -1 )
            res = domeId;
        return res;
    }
 
    s32 getNumGrandStarForOpen(const char *pName, s32 domeId) {
        s32 num = -1;

        if (domeId != -1) {
            num = domeId;
        }

        return num;
    }
};

void AstroMapObjFunction::makeArchiveListFromArg0(NameObjArchiveListCollector *pList, const JMapInfoIter &rIter) {
    s32 domeId = -1;
    MR::getJMapInfoArg0NoInit(rIter, &domeId);
    s32 domeIdx = domeId;
    const char* objName;
    MR::getObjectName(&objName, rIter);
    const char* modelName = AstroMapObjFunction::getModelName(objName, domeIdx);
    pList->addArchive(modelName);

    if (MR::isEqualString(modelName, "AstroDomeEntrance")) {
        pList->addArchive("GalaxyNamePlate");
        pList->addArchive(cAstroNamePlateArcName);
    }
}

void AstroMapObjFunction::makeArchiveListAstroNamePlate(NameObjArchiveListCollector *pList, const JMapInfoIter &rIter) {
    pList->addArchive("GalaxyNamePlate");
    pList->addArchive(cAstroNamePlateArcName);
}

bool AstroMapObjFunction::isAlreadyRevival(const char *pName, s32 domeId) {
    if (MR::isEqualString(pName, "AstroChildRoom")) {
        return MR::isOnGameEventFlagChildsRoom();
    }

    if (MR::isEqualString(pName, "TridentPlanet")) {
        return MR::isOnGameEventFlagGreenDriver();
    }

    s32 openNum = AstroDemoFunction::getOpenedAstroDomeNum();
    return openNum >= ::getNumGrandStarForRevival(pName, domeId);
}

bool AstroMapObjFunction::isEnableRevival(const char *pName, s32 domeId) {
    if (MR::isEqualString(pName, "AstroChildRoom")) {
        return true;
    }

    s32 openNum = AstroDemoFunction::getOpenedAstroDomeNum();
    return ::getNumGrandStarForRevival(pName, domeId) == openNum + 1;
}

bool AstroMapObjFunction::isAlreadyOpen(const char *pName, s32 domeId) {
    if (MR::isEqualString(pName, "AstroLibrary")) {
        return MR::isOnGameEventFlagLibraryRoom();
    }

    s32 openNum = AstroDemoFunction::getOpenedAstroDomeNum();

    return openNum >= ::getNumGrandStarForOpen(pName, domeId);
}

bool AstroMapObjFunction::isEnableOpen(const char *pName, s32 domeId) {
    s32 openNum = AstroDemoFunction::getOpenedAstroDomeNum();
    return ::getNumGrandStarForOpen(pName, domeId) == openNum + 1;
}

s32 AstroMapObjFunction::getDomeIdFromArg0(const JMapInfoIter &rIter) {
    s32 domeId = -1;
    MR::getJMapInfoArg0NoInit(rIter, &domeId);
    return domeId;
}

const char* AstroMapObjFunction::getModelName(const char *pName, s32 domeIdx) {
    const char** tbl = nullptr;

    if (MR::isEqualString(pName, "AstroDome")) {
        tbl = cAstroDomeNameTable;
    }
    else if (MR::isEqualString(pName, "AstroDomeSky")) {
        tbl = cAstroDomeSkyNameTable;
    }
    else if (MR::isEqualString(pName, "AstroDomeEntrance")) {
        tbl = cAstroDomeEntranceNameTable;
    }
    else if (MR::isEqualString(pName, "AstroStarPlate")) {
        tbl = cAstroStarPlateNameTable;
    }

    if (tbl != nullptr) {
        return tbl[domeIdx - 1];
    }
    else {
        return pName;
    }
}

#ifndef NON_MATCHING
const JMapInfo* AstroMapObjFunction::createAstroNamePlateParser() {
    char buf[0x40];
    snprintf(buf, 0x40, "%s.arc", cAstroNamePlateArcName);
    return MR::createCsvParser(buf, cAstroNamePlateFileName);
}
#endif

// AstroMapObjFunction::trySetAndShowNamePlate
// AstroMapObjFunction::tryRegisterMultiDemoAndFunction
// AstroMapObjFunction::initilizePeculiar