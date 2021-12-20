#pragma once

#include "Game/AreaObj/AreaObj.h"
#include "Game/AreaObj/LightAreaHolder.h"
#include "Game/NameObj/NameObj.h"
#include "JSystem/JGeometry/TVec.h"

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

    static AreaObjEntry cCreateTable[8] = {
        { "SwitchArea", 0x40, createManager<AreaObjMgr> },
        { "RaceJudgeCube", 0x40, createManager<AreaObjMgr> },
        { "NinForceWindCube", 0x40, createManager<AreaObjMgr> },
        { "NinAbyssCube", 0x40, createManager<AreaObjMgr> },
        //{ "CubeCamera", 0xA0, createManager<AreaObjMgr> },
        { "BindEndCube", 0x40, createManager<AreaObjMgr> },
        { "EffectCylinder", 0x40, createManager<AreaObjMgr> },
        { "DeathArea", 0x40, createManager<AreaObjMgr> },
        /* todo -- finish me */

        { "LightArea", 0x80, createManager<LightAreaHolder> }
    };
};

class AreaObjContainer : public NameObj {
public:
    AreaObjContainer(const char *);

    virtual ~AreaObjContainer();
    virtual void init(const JMapInfoIter &);

    AreaObjMgr* getManager(const char *) const;
    AreaObj* getAreaObj(const char *, const TVec3f &) const;
};

namespace MR {
    AreaObjContainer* getAreaObjContainer();
};
