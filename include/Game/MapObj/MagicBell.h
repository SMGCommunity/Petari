#pragma once

#include "Game/LiveActor/LiveActor.h"
#include "Game/MapObj/Swinger.h"

class MagicBell : public LiveActor {
public:
    MagicBell(const char *);

    virtual ~MagicBell();
    virtual void init(const JMapInfoIter &);
    virtual MtxPtr getBaseMtx() const;
    virtual void attackSensor(HitSensor *, HitSensor *);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor *, HitSensor *);

    void exeWait();
    void exeRing();
    bool tryRing();
    void startRing(const TVec3f &, const TVec3f &);

    Swinger* _8C;
    MtxPtr mSurface2Mtx;        // _90
    Swinger* _94;
    MtxPtr mSurface1Mtx;        // _98
    TVec3f _9C;
};

namespace NrvMagicBell {
    NERVE_DECL(MagicBellNrvWait, MagicBell, MagicBell::exeWait);
    NERVE_DECL(MagicBellNrvRing, MagicBell, MagicBell::exeRing);
};