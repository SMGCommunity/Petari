#pragma once

#include "Game/AreaObj/AreaObj.hpp"
#include "Game/AreaObj/LightAreaHolder.hpp"
#include "Game/NameObj/NameObj.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "Game/AreaObj/CubeCamera.hpp"
#include "Game/AreaObj/GlaringLightArea.hpp"
#include "Game/AreaObj/ImageEffectArea.hpp"
#include "Game/AreaObj/WarpCube.hpp"
#include "Game/AreaObj/WaterArea.hpp"

struct AreaObjEntry {
    const char* mName;                          // 0x0
    int _4;                                     // 0x4
    AreaObjMgr* (*mFuncPtr)(s32, const char *); // 0x8
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

    AreaObjMgr* mManagerArray[0x50];    // 0xC
    u32 mNumManagers;                   // 0x14C
};

namespace MR {
    AreaObjContainer* getAreaObjContainer();
};
