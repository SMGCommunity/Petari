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

    /* 0x08 */ virtual ~TeresaWater();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x2C */ virtual void kill();
    /* 0x48 */ virtual void control();
    /* 0x4C */ virtual void calcAndSetBaseMtx();
    /* 0x54 */ virtual void attackSensor(HitSensor*, HitSensor*);
    /* 0x5C */ virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);

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
