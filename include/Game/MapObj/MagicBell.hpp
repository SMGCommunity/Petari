#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/MapObj/Swinger.hpp"

class MagicBell : public LiveActor {
public:
    MagicBell(const char*);

    virtual ~MagicBell();
    virtual void init(const JMapInfoIter&);
    virtual MtxPtr getBaseMtx() const;
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);

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
