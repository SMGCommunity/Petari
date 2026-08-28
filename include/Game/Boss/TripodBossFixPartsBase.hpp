#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class TripodBossBreakMovement;

class TripodBossFixPartsBase : public LiveActor {
public:
    TripodBossFixPartsBase(const char*);

    /* 0x08 */ virtual ~TripodBossFixPartsBase();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x10 */ virtual void initAfterPlacement();
    /* 0x30 */ virtual void makeActorDead();
    /* 0x4C */ virtual void calcAndSetBaseMtx();
    /* 0x74 */ virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);
    /* 0x78 */ virtual void calcTripodLocalMatrix(TPos3f*);
    /* 0x7C */ virtual void activateTripodBoss();

    void initClippingSphere();
    void setClippingSphere(const TVec3f&, f32);
    void startBreakMovement();
    void updateBreakMovementMatrix();
    void updateTripodMatrix();
    void initBreakMovement(s32);

    /* 0x8C */ TPos3f _8C;
    /* 0xBC */ TVec3f _BC;
    /* 0xC8 */ TripodBossBreakMovement* mBreakMovement;
    /* 0xCC */ s32 _CC;
    /* 0xD0 */ TVec3f _D0;
    /* 0xDC */ f32 mClippingDistance;
    /* 0xE0 */ u8 _E0;
    /* 0xE1 */ u8 _E1;
};
