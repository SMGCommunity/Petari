#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/JGeometry.hpp>

class PartsModel;

class LavaProminenceTriple : public LiveActor {
public:
    LavaProminenceTriple(const char*);

    /* 0x08 */ virtual ~LavaProminenceTriple();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x24 */ virtual void appear();
    /* 0x48 */ virtual void control();
    /* 0x4C */ virtual void calcAndSetBaseMtx();
    /* 0x50 */ virtual void updateHitSensor(HitSensor*);
    /* 0x54 */ virtual void attackSensor(HitSensor*, HitSensor*);

    void updateEffectClipping();
    void updateEffectClippingIndividual(bool*, const char*, const char*);

    PartsModel* mBloomModel;
    TPos3f _90;
    TQuat4f _C0;
    TQuat4f _D0;
    TVec3f _E0;
    TVec3f _EC;
    /* 0xF8 */ f32 mArg0;
    bool _FC;
    bool _FD;
    bool _FE;
    u8 _FF;
    /* 0x100 */ s32 mArg1;
    s32 _104;
};
