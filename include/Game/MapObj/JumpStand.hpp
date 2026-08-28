#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "JSystem/JGeometry/TMatrix.hpp"

class JumpStand : public LiveActor {
public:
    JumpStand(const char*);

    enum JumpType { UNK_0 = 0, UNK_1 = 1, UNK_2 = 2 };

    /* 0x08 */ virtual ~JumpStand();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x48 */ virtual void control();
    /* 0x5C */ virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    /* 0x74 */ virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void exeWait();
    void exeTrampleBound();
    void exeHipDropBound();
    void exeStarPieceBound();
    bool tryToStartBind(HitSensor*);
    void updateBindActorMtx();
    void endBindAndShootUp(f32, JumpType);

    LiveActor* mBindedActor;  // 0x8C
    TPos3f _90;
    u8 _C0;
};
