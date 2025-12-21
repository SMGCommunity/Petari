#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/NameObj/NameObjArchiveListCollector.hpp"
#include "Game/Util/Functor.hpp"
#include <JSystem/JGeometry/TVec.hpp>

class GalaxyNamePlate;
class LiveActor;
class JMapInfo;

namespace {
    s32 getNumGrandStarForRevival(const char*, s32) NO_INLINE;
    s32 getNumGrandStarForOpen(const char*, s32) NO_INLINE;

    const char* cAstroDomeNameTable[] = {"AstroDomeObservatory", "AstroDomeWell",    "AstroDomeKitchen",
                                         "AstroDomeBedRoom",     "AstroDomeMachine", "AstroDomeTower"};

    const char* cAstroDomeSkyNameTable[] = {"AstroDomeSkyA", "AstroDomeSkyB", "AstroDomeSkyC", "AstroDomeSkyA", "AstroDomeSkyB", "AstroDomeSkyC"};

    const char* cAstroDomeEntranceNameTable[] = {"AstroDomeEntranceObservatory", "AstroDomeEntranceWell",    "AstroDomeEntranceKitchen",
                                                 "AstroDomeEntranceBedRoom",     "AstroDomeEntranceMachine", "AstroDomeEntranceTower"};

    const char* cAstroStarPlateNameTable[] = {"AstroStarPlateObservatory", "AstroStarPlateWell",    "AstroStarPlateKitchen",
                                              "AstroStarPlateBedRoom",     "AstroStarPlateMachine", "AstroStarPlateTower"};

    const char* cAstroNamePlateArcName = "AstroNamePlateData";
    const char* cAstroNamePlateFileName = "AstroNamePlateData.bcsv";

};  // namespace

class AstroMapObjFunction {
public:
    static void makeArchiveListFromArg0(NameObjArchiveListCollector*, const JMapInfoIter&);
    static void makeArchiveListAstroNamePlate(NameObjArchiveListCollector*, const JMapInfoIter&);
    static bool isAlreadyRevival(const char*, s32);
    static bool isEnableRevival(const char*, s32);
    static bool isAlreadyOpen(const char*, s32);
    static bool isEnableOpen(const char*, s32);
    static s32 getDomeIdFromArg0(const JMapInfoIter&);
    static const char* getModelName(const char*, s32);
    static const JMapInfo* createAstroNamePlateParser();
    static bool trySetAndShowNamePlate(GalaxyNamePlate*, const JMapInfo*, const TVec3f&, const char*, s32, bool);
    static bool tryRegisterMultiDemoAndFunction(const char*, LiveActor*, const JMapInfoIter&, const MR::FunctorBase&);
    static void initilizePeculiar(const char*, LiveActor*, const JMapInfoIter&);
};