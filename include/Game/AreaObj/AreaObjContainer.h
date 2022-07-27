#pragma once

#include "Game/AreaObj/AreaObj.h"
#include "Game/AreaObj/LightAreaHolder.h"
#include "Game/NameObj/NameObj.h"
#include "JSystem/JGeometry/TVec.h"
#include "Game/AreaObj/CubeCamera.h"
#include "Game/AreaObj/GlaringLightArea.h"
#include "Game/AreaObj/ImageEffectArea.h"
#include "Game/AreaObj/WarpCube.h"
#include "Game/AreaObj/WaterArea.h"

struct AreaObjEntry {
    const char* mName;                          // _0
    int _4;                                     // _4
    AreaObjMgr* (*mFuncPtr)(s32, const char *); // _8
};

namespace {
    template<typename T>
    AreaObjMgr* createManager(s32 type, const char *pName) {
        return new T(type, pName);
    }

    static AreaObjEntry cCreateTable[0x43] = {
        { "SwitchArea", 0x40, createManager<AreaObjMgr> },
        { "RaceJudgeCube", 0x40, createManager<AreaObjMgr> },
        { "NinForceWindCube", 0x40, createManager<AreaObjMgr> },
        { "NinAbyssCube", 0x40, createManager<AreaObjMgr> },
        { "CubeCamera", 0xA0, createManager<CubeCameraMgr> },
        { "BindEndCube", 0x40, createManager<AreaObjMgr> },
        { "EffectCylinder", 0x40, createManager<AreaObjMgr> },
        { "DeathArea", 0x40, createManager<AreaObjMgr> },
        /* todo -- finish me */

        { "ImageEffectArea", 0x20, createManager<ImageEffectAreaMgr> },

        { "GlaringLightArea", 0x40, createManager<GlaringLightAreaMgr> },

        { "WarpCube", 0x40, createManager<WarpCubeMgr> },

        { "LightArea", 0x80, createManager<LightAreaHolder> },

        { "Water", 0x40, createManager<WaterAreaMgr> }
    };
};

class AreaObjContainer : public NameObj {
public:
    AreaObjContainer(const char *);

    virtual ~AreaObjContainer();
    virtual void init(const JMapInfoIter &);

    AreaObjMgr* getManager(const char *) const;
    AreaObj* getAreaObj(const char *, const TVec3f &) const;

    inline AreaObjMgr* getFirstMgr() const {
        return mManagerArray[0];
    }

    inline AreaObjMgr* getLastMgr() const {
        return mManagerArray[mNumManagers];
    }

    AreaObjMgr* mManagerArray[0x50];    // _C
    u32 mNumManagers;                   // _14C
};

namespace MR {
    AreaObjContainer* getAreaObjContainer();
};
