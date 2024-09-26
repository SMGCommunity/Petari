#pragma once

#include "Game/MapObj/MapObjActor.hpp"
#include "Game/NameObj/NameObjArchiveListCollector.hpp"

struct SoundEffectDataEntry {
    const char* objectName; // 0x0
    const char* soundName;  // 0x4
    f32 _8;
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
    SimpleMirrorObj(const char *pName, const char *a2 = NULL, MtxPtr = NULL);

    virtual ~SimpleMirrorObj();
    virtual void init(const JMapInfoIter &);
    virtual void calcAndSetBaseMtx();

    MtxPtr mMtx;    // 0xC4
};

class SimpleMirrorReflectionObj : public SimpleMapObj {
public:
    inline SimpleMirrorReflectionObj(const char *pName) : SimpleMapObj(pName) {

    }

    virtual ~SimpleMirrorReflectionObj();
    virtual void init(const JMapInfoIter &);
    virtual void connectToScene(const MapObjActorInitInfo &);
};

class SimpleTextureSwitchChangeObj : public SimpleMapObj {
    inline SimpleTextureSwitchChangeObj(const char *pName) : SimpleMapObj(pName) {

    }

    virtual ~SimpleTextureSwitchChangeObj();
    virtual void initCaseUseSwitchA(const MapObjActorInitInfo &);
    virtual s32 getChangedTextureNo() const;

    void changeTexture();
};

class UFOKinokoUnderConstruction : public MapObjActor {
    inline UFOKinokoUnderConstruction(const char *pName) : MapObjActor(pName) {

    }

    virtual ~UFOKinokoUnderConstruction();
    virtual void init(const JMapInfoIter &);
};

class GlaringLight : public SimpleMapObj {
public:
    inline GlaringLight(const char *pName) : SimpleMapObj(pName) {

    }

    virtual ~GlaringLight();
    virtual void connectToScene(const MapObjActorInitInfo &);
};

namespace MR {
    void makeArchiveListUFOKinokoUnderConstruction(NameObjArchiveListCollector *, const JMapInfoIter &);
};
