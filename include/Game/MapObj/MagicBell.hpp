#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/MapObj/Swinger.hpp"

class MagicBell : public LiveActor {
public:
    MagicBell(const char*);

    /* 0x08 */ virtual ~MagicBell();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x38 */ virtual MtxPtr getBaseMtx() const;
    /* 0x54 */ virtual void attackSensor(HitSensor*, HitSensor*);
    /* 0x5C */ virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);

    void exeWait();
    void exeRing();
    bool tryRing();
    void startRing(const TVec3f&, const TVec3f&);

    Swinger* mBellSwinger;
    MtxPtr mSurface2Mtx;  // 0x90
    Swinger* mBellRodSwinger;
    MtxPtr mSurface1Mtx;  // 0x98
    TVec3f mHitMarkPosition;
};
