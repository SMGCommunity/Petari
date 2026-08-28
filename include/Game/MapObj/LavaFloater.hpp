#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class FloaterFloatingForceTypeNormal;
class ProjmapEffectMtxSetter;
class LodCtrl;

class LavaFloater : public LiveActor {
public:
    LavaFloater(const char*);

    virtual ~LavaFloater();
    virtual void init(const JMapInfoIter&);
    virtual void initAfterPlacement();
    virtual void makeActorAppeared();
    virtual void makeActorDead();
    virtual void startClipped();
    virtual void endClipped();
    virtual void control();

    f32 getCurrentSinkDepth() const;

    void exeFloat();
    void exeSink();

    /* 0x8C */ FloaterFloatingForceTypeNormal* mFloatingForce;
    /* 0x90 */ ProjmapEffectMtxSetter* mMtxSetter;
    /* 0x94 */ LodCtrl* mLodCtrlPlanet;
    /* 0x98 */ const char* mObjectName;
    /* 0x9C */ TVec3f _9C;
    /* 0xA8 */ f32 _A8;
};
