#pragma once

#include "Game/Boss/TripodBossRailMoveParts.hpp"
#include "Game/Boss/TripodBossRotateParts.hpp"
#include "Game/NameObj/NameObjFactory.hpp"
#include "Game/MapObj/SimpleNormalMapObj.hpp"
#include "Game/Util.hpp"

namespace MR {
    CreationFuncPtr getModelChangableObjCreator(const char *);
    void requestMountModelChangableObjArchives(const char *, s32);
    bool isReadResourceFromDVDAtModelChangableObj(const char *, s32);
};

typedef NameObj* (*CreationFuncPtr)(const char *);

struct Model2CreateFunc {
    const char* _0;
    const char* mArchiveName;                   // 0x4
    NameObj* (*mCreationFunc)(const char *);    // 0x8
};

namespace {
    const Model2CreateFunc cModelChangableObjCreatorTable[12] = {
        { nullptr, "AssemblyBlock", createNameObj<AssemblyBlock> },
        { nullptr, "ClipFieldMapParts", nullptr },
        { nullptr, "FlexibleSphere", nullptr },
        { nullptr, "MercatorFixParts", nullptr },
        { nullptr, "MercatorRailMoveParts", nullptr },
        { nullptr, "MercatorRotateParts", nullptr },
        { nullptr, "TripodBossFixParts", createNameObj<TripodBossFixParts> },
        { nullptr, "TripodBossRailMoveParts", createNameObj<TripodBossRailMoveParts> },
        { nullptr, "TripodBossRotateParts", createNameObj<TripodBossRotateParts> },
        { nullptr, "TripodBossRotateParts", createNameObj<TripodBossRotateParts> },
        { nullptr, "SimpleNormalMapObj", createNameObj<SimpleNormalMapObj> },
        { nullptr, "SunshadeMapParts", nullptr },
    };
};
