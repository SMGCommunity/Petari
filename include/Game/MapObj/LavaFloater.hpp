#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class FloaterFloatingForceTypeNormal;
class ProjmapEffectMtxSetter;
class LodCtrl;

class LavaFloater : public LiveActor {
public:
    LavaFloater(const char*);

    /* 0x08 */ virtual ~LavaFloater();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x10 */ virtual void initAfterPlacement();
    /* 0x28 */ virtual void makeActorAppeared();
    /* 0x30 */ virtual void makeActorDead();
    /* 0x40 */ virtual void startClipped();
    /* 0x44 */ virtual void endClipped();
    /* 0x48 */ virtual void control();

    void getCurrentSinkDepth() const;

    void exeFloat();
    void exeSink();

    /* 0x8C */ FloaterFloatingForceTypeNormal* mFloatingForce;
    /* 0x90 */ ProjmapEffectMtxSetter* mMtxSetter;
    /* 0x94 */ LodCtrl* mLodCtrlPlanet;
    /* 0x98 */ const char* mObjectName;
    /* 0x9C */ TVec3f _9C;
    /* 0xA4 */ f32 _A4;
    /* 0xA8 */ f32 _A8;
};
