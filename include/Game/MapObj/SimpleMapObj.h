#pragma once

#include "Game/MapObj/MapObjActor.h"

struct SoundEffectDataEntry {
    const char* objectName; // _0
    const char* soundName;  // _4
    f32 _4;
};

namespace {
    static SoundEffectDataEntry sSeDataTable[8] = {
        { "KoopaShipSeesawStep", "SE_OJ_LV_KOOPA_SHIP_SEESAW", 0.001f },
        { "IceVolcanoBalanceStepA", "SE_OJ_LV_VOL_BAL_STEP_ROT", 0.001f },
        { "IceVolcanoBalanceStepB", "SE_OJ_LV_VOL_BAL_STEP_ROT", 0.001f },
        { "IceMountainSeesawSlipA", "SE_OJ_LV_ICE_M_SEESAW_S", 0.001f },
        { "IceMountainSeesawSlipB", "SE_OJ_LV_ICE_M_SEESAW_M", 0.001f },
        { "IceMountainSeesawSlipC", "SE_OJ_LV_ICE_M_SEESAW_L", 0.001f },
        { "IceMountainSeesawSlipD", "SE_OJ_LV_ICE_M_SEESAW_L", 0.001f },
        { "IceMountainSeesawNoSlipA", "SE_OJ_LV_ICE_M_SEESAW_S", 0.001f },
    };

    SoundEffectDataEntry* getSeParam(const char *pName) NO_INLINE {
        for (int i = 0; i < 8; i++) {
            if (MR::isEqualString(pName, sSeDataTable[i].objectName)) {
                return &sSeDataTable[i];
            }
        }

        return NULL;
    }
};

class SimpleMapObj : public MapObjActor {
public:
    SimpleMapObj(const char *pName) : MapObjActor(pName) {

    }

    virtual ~SimpleMapObj() {

    }

    virtual void init(const JMapInfoIter &);
};

class SimpleEnvironmentObj : public MapObjActor {
public:
    inline SimpleEnvironmentObj(const char *pName) : MapObjActor(pName) {

    }

    virtual ~SimpleEnvironmentObj();
    virtual void init(const JMapInfoIter &);
    virtual void connectToScene(const MapObjActorInitInfo &);
};

class SimpleMapObjFarMax : public SimpleMapObj {
public:
    inline SimpleMapObjFarMax(const char *pName) : SimpleMapObj(pName) {

    }

    virtual ~SimpleMapObjFarMax() {

    }

    virtual void init(const JMapInfoIter &);
};

class SimpleMapObjPush : public SimpleMapObj {
public:
    inline SimpleMapObjPush(const char *pName) : SimpleMapObj(pName) {

    }

    virtual ~SimpleMapObjPush();
    virtual void attackSensor(HitSensor *, HitSensor *);
};

class SimpleMapObjNoSilhouetted : public SimpleMapObj {
public:
    inline SimpleMapObjNoSilhouetted(const char *pName) : SimpleMapObj(pName) {

    }

    virtual ~SimpleMapObjNoSilhouetted();
    virtual void connectToScene(const MapObjActorInitInfo &);
};

class SimpleSeesawObj : public MapObjActor {
public:
    SimpleSeesawObj(const char *);

    virtual ~SimpleSeesawObj();
    virtual void init(const JMapInfoIter &);
    virtual void control();

    TMtx34f _C4;
};

class SimpleMirrorObj : public MapObjActor {
public:
    SimpleMirrorObj(const char *, const char *, MtxPtr);

    virtual ~SimpleMirrorObj();
    virtual void init(const JMapInfoIter &);
    virtual void calcAndSetBaseMtx();

    MtxPtr mMtx;    // _C4
};

class SimpleMirrorReflectionObj : public SimpleMapObj {
public:
    inline SimpleMirrorReflectionObj(const char *pName) : SimpleMapObj(pName) {

    }

    virtual ~SimpleMirrorReflectionObj();
    virtual void init(const JMapInfoIter &);
    virtual void connectToScene(const MapObjActorInitInfo &);
};