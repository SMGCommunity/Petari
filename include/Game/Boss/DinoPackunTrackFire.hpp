#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/LiveActorGroup.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class DinoPackunTrackFire : public LiveActor {
public:
    DinoPackunTrackFire(const char*);

    /* 0x08 */ virtual ~DinoPackunTrackFire();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x38 */ virtual MtxPtr getBaseMtx() const;
    /* 0x48 */ virtual void control();
    /* 0x54 */ virtual void attackSensor(HitSensor*, HitSensor*);
    /* 0x5C */ virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);

    void appearAndSetPos(const TVec3f&);
    void exeWait();
    void exeGround();

    TPos3f _8C;
};

class DinoPackunTrackFireHolder : public DeriveActorGroup< DinoPackunTrackFire > {
public:
    DinoPackunTrackFireHolder(s32);

    /* 0x08 */ virtual ~DinoPackunTrackFireHolder();
};
