#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class AnimScaleController;
class WalkerStateBindStarPointer;

struct TeresaWaterMoveParam {
    f32 _0;
    f32 _4;
    f32 _8;
    f32 _C;
};

class TeresaWater : public LiveActor {
public:
    TeresaWater(const char*);

    virtual ~TeresaWater();
    virtual void init(const JMapInfoIter&);
    virtual void kill();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);

    void exeMove();
    void exeHit();
    void exeStun();
    void exeDPDSwoon();
    void endDPDSwoon();
    bool tryDPDSwoon();

    AnimScaleController* _8C;
    WalkerStateBindStarPointer* _90;
    TVec3f _94;
    TVec3f _A0;
    TVec3f _AC;
    TVec3f* _B8;
};
