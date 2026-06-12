#pragma once

#include <JSystem/JGeometry/TVec.hpp>

class GalaxyNamePlate;
class JMapInfo;
class JMapInfoIter;
class LiveActor;
class NameObjArchiveListCollector;

namespace MR {
    class FunctorBase;
};  // namespace MR

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