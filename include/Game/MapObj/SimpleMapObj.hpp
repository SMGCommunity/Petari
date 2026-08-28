#pragma once

#include "Game/MapObj/MapObjActor.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class NameObjArchiveListCollector;

struct SoundEffectDataEntry {
    const char* objectName;  // 0x0
    const char* soundName;   // 0x4
    f32 _8;
};

class SimpleMapObj : public MapObjActor {
public:
    SimpleMapObj(const char* pName) : MapObjActor(pName) {
    }

    /* 0x08 */ virtual ~SimpleMapObj() {
    }

    /* 0x0C */ virtual void init(const JMapInfoIter&);
};

class SimpleEnvironmentObj : public MapObjActor {
public:
    inline SimpleEnvironmentObj(const char* pName) : MapObjActor(pName) {
    }

    /* 0x08 */ virtual ~SimpleEnvironmentObj();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x78 */ virtual void connectToScene(const MapObjActorInitInfo&);
};

class SimpleMapObjFarMax : public SimpleMapObj {
public:
    inline SimpleMapObjFarMax(const char* pName) : SimpleMapObj(pName) {
    }

    /* 0x08 */ virtual ~SimpleMapObjFarMax() {
    }

    /* 0x0C */ virtual void init(const JMapInfoIter&);
};

class SimpleMapObjPush : public SimpleMapObj {
public:
    inline SimpleMapObjPush(const char* pName) : SimpleMapObj(pName) {
    }

    /* 0x08 */ virtual ~SimpleMapObjPush();
    /* 0x54 */ virtual void attackSensor(HitSensor*, HitSensor*);
};

class SimpleMapObjNoSilhouetted : public SimpleMapObj {
public:
    inline SimpleMapObjNoSilhouetted(const char* pName) : SimpleMapObj(pName) {
    }

    /* 0x08 */ virtual ~SimpleMapObjNoSilhouetted();
    /* 0x78 */ virtual void connectToScene(const MapObjActorInitInfo&);
};

class SimpleSeesawObj : public MapObjActor {
public:
    SimpleSeesawObj(const char*);

    /* 0x08 */ virtual ~SimpleSeesawObj();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x48 */ virtual void control();

    TMtx34f _C4;
};

class SimpleMirrorObj : public MapObjActor {
public:
    SimpleMirrorObj(const char* pName, const char* a2 = nullptr, MtxPtr = nullptr);

    /* 0x08 */ virtual ~SimpleMirrorObj();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x4C */ virtual void calcAndSetBaseMtx();

    MtxPtr mMtx;  // 0xC4
};

class SimpleMirrorReflectionObj : public SimpleMapObj {
public:
    inline SimpleMirrorReflectionObj(const char* pName) : SimpleMapObj(pName) {
    }

    /* 0x08 */ virtual ~SimpleMirrorReflectionObj();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x78 */ virtual void connectToScene(const MapObjActorInitInfo&);
};

class SimpleTextureSwitchChangeObj : public SimpleMapObj {
public:
    inline SimpleTextureSwitchChangeObj(const char* pName) : SimpleMapObj(pName) {
    }

    /* 0x08 */ virtual ~SimpleTextureSwitchChangeObj();
    /* 0x7C */ virtual void initCaseUseSwitchA(const MapObjActorInitInfo&);
    virtual s32 getChangedTextureNo() const;

    void changeTexture();
};

class UFOKinokoUnderConstruction : public MapObjActor {
public:
    inline UFOKinokoUnderConstruction(const char* pName) : MapObjActor(pName) {
    }

    /* 0x08 */ virtual ~UFOKinokoUnderConstruction();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
};

class GlaringLight : public SimpleMapObj {
public:
    inline GlaringLight(const char* pName) : SimpleMapObj(pName) {
    }

    /* 0x08 */ virtual ~GlaringLight();
    /* 0x78 */ virtual void connectToScene(const MapObjActorInitInfo&);
};

namespace MR {
    void makeArchiveListUFOKinokoUnderConstruction(NameObjArchiveListCollector*, const JMapInfoIter&);
};  // namespace MR
