#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class JumpStand : public LiveActor {
public:
    JumpStand(const char*);

    enum JumpType { Jump_Low = 0, Jump_Middle = 1, Jump_High = 2 };

    virtual void init(const JMapInfoIter&);
    virtual void control();
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void exeWait();
    void exeTrampleBound();
    void exeHipDropBound();
    void exeStarPieceBound();
    bool tryToStartBind(HitSensor*);
    void updateBindActorMtx();
    void endBindAndShootUp(f32, JumpType);

    /* 0x8C */ LiveActor* mBindedActor;
    /* 0x90 */ TPos3f _90;
    /* 0xC0 */ bool mIsMarioJumpingHigh;
};
