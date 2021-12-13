#pragma once

#include "revolution.h"
#include "Game/AreaObj/AstroChangeStageCube.h"
#include "Game/AreaObj/AudioEffectArea.h"
#include "Game/AreaObj/BigBubbleCameraArea.h"
#include "Game/AreaObj/BgmProhibitArea.h"
#include "Game/AreaObj/ChangeBgmCube.h"
#include "Game/AreaObj/CubeCamera.h"
#include "Game/AreaObj/DeathArea.h"

class AreaObj;
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
    template<typename T>
    NameObj* createCenterOriginCube(const char *pName) {
        return new T(0, pName);
    }

    template<typename T>
    NameObj* createBaseOriginCube(const char *pName) {
        return new T(1, pName);
    }

    template<typename T>
    NameObj* createSphere(const char *pName) {
        return new T(2, pName);
    }

    template<typename T>
    NameObj* createBaseOriginCylinder(const char *pName) {
        return new T(3, pName);
    }

    template<typename T>
    NameObj* createBowl(const char *pName) {
        return new T(4, pName);
    }

    template<typename T>
    NameObj* createNameObj(const char *pName) {
        return new T(pName);
    }

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

    const NameObjFactory::Name2CreateFunc cCreateTable[17] = {
        { "CubeCameraBox", createCenterOriginCube<CubeCameraArea>, 0 },
        { "CubeCameraCylinder", createBaseOriginCylinder<CubeCameraArea>, 0 },
        { "CubeCameraSphere", createSphere<CubeCameraArea>, 0 },
        { "CubeCameraBowl", createBowl<CubeCameraArea>, 0 },

        { "DeathCube", createBaseOriginCube<DeathArea>, 0 },
        { "DeathSphere", createSphere<DeathArea>, 0 },
        { "DeathCylinder", createBaseOriginCylinder<DeathArea>, 0 },

        { "ChangeBgmCube", createBaseOriginCube<ChangeBgmCube>, 0 },
        { "BgmProhibitArea", createSphere<BgmProhibitArea>, 0 },

        { "BigBubbleCameraBox", createCenterOriginCube<BigBubbleCameraArea>, 0 },
        { "BigBubbleCameraCylinder", createBaseOriginCylinder<BigBubbleCameraArea>, 0 },
        { "BigBubbleCameraSphere", createSphere<BigBubbleCameraArea>, 0 },

        { "AudioEffectCube", createBaseOriginCube<AudioEffectArea>, 0 },
        { "AudioEffectSphere", createSphere<AudioEffectArea>, 0 },
        { "AudioEffectCylinder", createBaseOriginCylinder<AudioEffectArea>, 0 },

        { "AstroChangeStageCube", createBaseOriginCube<AstroChangeStageCube>, 0 }
    };

    const char* cName2ArchiveNamesTable;
};