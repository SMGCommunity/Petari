#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/JGeometry.hpp>

class PartsModel;

class LavaProminenceTriple : public LiveActor {
public:
    LavaProminenceTriple(const char*);

    virtual ~LavaProminenceTriple();
    virtual void init(const JMapInfoIter&);
    virtual void appear();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void updateHitSensor(HitSensor*);
    virtual void attackSensor(HitSensor*, HitSensor*);

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
