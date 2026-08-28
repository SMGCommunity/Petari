#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class BombBirdBomb : public LiveActor {
public:
    /// @brief Creates a new `BombBirdBomb`.
    /// @param pName A pointer to the null-terminated name of the object.
    BombBirdBomb(const char* pName);

    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x2C */ virtual void kill();
    /* 0x48 */ virtual void control();
    /* 0x4C */ virtual void calcAndSetBaseMtx();
    /* 0x54 */ virtual void attackSensor(HitSensor*, HitSensor*);
    /* 0x58 */ virtual bool receiveMsgPush(HitSensor*, HitSensor*);
    /* 0x5C */ virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);

    void hold(const TVec3f&);
    void start(const TVec3f&, const TVec3f&);

    void exeHold();
    void exeWait();
    void exeExplosion();

    TQuat4f _8C;
    TVec3f _9C;
};
