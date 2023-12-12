#pragma once

#include "Game/NameObj/NameObj.h"
#include "Game/NameObj/NameObjArchiveListCollector.h"

class GalaxyNamePlate;
class LiveActor;

namespace {
    s32 getNumGrandStarForRevival(const char *, s32) NO_INLINE;
    s32 getNumGrandStarForOpen(const char *, s32) NO_INLINE;

    const char* cAstroDomeNameTable[6] = {
        "AstroDomeObservatory",
        "AstroDomeWell",
        "AstroDomeKitchen",
        "AstroDomeBedRoom",
        "AstroDomeMachine",
        "AstroDomeTower"
    };

    const char* cAstroDomeSkyNameTable[6] = {
        "AstroDomeSkyA",
        "AstroDomeSkyB",
        "AstroDomeSkyC",
        "AstroDomeSkyA",
        "AstroDomeSkyB",
        "AstroDomeSkyC"
    };

    const char* cAstroDomeEntranceNameTable[6] = {
        "AstroDomeEntranceObservatory",
        "AstroDomeEntranceWell",
        "AstroDomeEntranceKitchen",
        "AstroDomeEntranceBedRoom",
        "AstroDomeEntranceMachine",
        "AstroDomeEntranceTower"
    };

    const char* cAstroStarPlateNameTable[6] = {
        "AstroStarPlateObservatory",
        "AstroStarPlateWell",
        "AstroStarPlateKitchen",
        "AstroStarPlateBedRoom",
        "AstroStarPlateMachine",
        "AstroStarPlateTower"
    };

    const char* cAstroNamePlateArcName = "AstroNamePlateData";
    const char* cAstroNamePlateFileName = "AstroNamePlateData.bcsv";

};

class AstroMapObjFunction {
public:
    static void makeArchiveListFromArg0(NameObjArchiveListCollector *, const JMapInfoIter &);
    static void makeArchiveListAstroNamePlate(NameObjArchiveListCollector *, const JMapInfoIter &);
    static bool isAlreadyRevival(const char *, s32);
    static bool isEnableRevival(const char *, s32);
    static bool isAlreadyOpen(const char *, s32);
    static bool isEnableOpen(const char *, s32);
    static s32 getDomeIdFromArg0(const JMapInfoIter &);
    static const char* getModelName(const char *, s32);
    static JMapInfo* createAstroNamePlateParser();
    static bool trySetAndShowNamePlate(GalaxyNamePlate *, const JMapInfo *, const TVec3f &, const char *, s32, bool);
    static bool tryRegisterMultiDemoAndFunction(const char *, LiveActor *, const JMapInfoIter &, const MR::FunctorBase &);
    static void initilizePeculiar(const char *, LiveActor *, const JMapInfoIter &);
};