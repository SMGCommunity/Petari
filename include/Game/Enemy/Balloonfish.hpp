#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class Balloonfish : public LiveActor {
public:
    Balloonfish(const char*);
    virtual ~Balloonfish();

    virtual void init(const JMapInfoIter& rIter);
    virtual void appear();
    virtual void kill();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);

    void exeWait();
    void exeDash();
    void exeDashEnd();
    void exeStarPointerBind();

    /* _8C */ u32 _8C;
    /* _90 */ u32 _90;
    /* _94 */ u32 _94;
    /* _98 */ TQuat4f _98;
    /* _A8 */ f32 _A8;
};
