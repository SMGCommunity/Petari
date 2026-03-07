#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/LiveActorGroup.hpp"
#include "JSystem/JGeometry/TMatrix.hpp"

class DinoPackunTrackFire : public LiveActor {
public:
    DinoPackunTrackFire(const char*);

    virtual ~DinoPackunTrackFire();
    virtual void init(const JMapInfoIter&);
    virtual MtxPtr getBaseMtx() const;
    virtual void control();
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);

    void appearAndSetPos(const TVec3f&);
    void exeWait();
    void exeGround();

    TPos3f _8C;
};

class DinoPackunTrackFireHolder : public DeriveActorGroup< DinoPackunTrackFire > {
public:
    DinoPackunTrackFireHolder(s32);

    virtual ~DinoPackunTrackFireHolder();
};
