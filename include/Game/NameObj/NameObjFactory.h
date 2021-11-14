#pragma once

#include "revolution.h"

class NameObj;

class NameObjFactory {
public:
    struct Name2CreateFunc {
        const char* mName;                          // _0
        NameObj* (*mCreactionFunc)(const char *);   // _4
        const char* _8;
    };

    static Name2CreateFunc* getName2CreateFunc(const char *, const Name2CreateFunc *);
    static bool isPlayerArchiveLoaderObj(const char *);
};

namespace {
    const char* cPlayerArchiveLoaderObjTable[0x8] =
    {
        "Hopper",
        "BenefitItemInvincible",
        "MorphItemNeoBee",
        "MorphItemNeoFire",
        "MorphItemNeoFoo",
        "MorphItemNeoHopper",
        "MorphItemNeoIce",
        "MorphItemNeoTeresa"
    };

    const NameObjFactory::Name2CreateFunc* cCreateTable;

    const char* cName2ArchiveNamesTable;
};