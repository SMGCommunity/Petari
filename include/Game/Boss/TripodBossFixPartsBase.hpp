#pragma once

#include "Game/Boss/TripodBossBreakMovement.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "JSystem/JGeometry/TMatrix.hpp"

class TripodBossFixPartsBase : public LiveActor {
public:
    TripodBossFixPartsBase(const char*);

    virtual ~TripodBossFixPartsBase();
    virtual void init(const JMapInfoIter&);
    virtual void initAfterPlacement();
    virtual void makeActorDead();
    virtual void calcAndSetBaseMtx();
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);
    virtual void calcTripodLocalMatrix(TPos3f*);
    virtual void activateTripodBoss();

    void initClippingSphere();
    void setClippingSphere(const TVec3f&, f32);
    void startBreakMovement();
    void updateBreakMovementMatrix();
    void updateTripodMatrix();
    void initBreakMovement(s32);

    TPos3f _8C;
    TVec3f _BC;
    TripodBossBreakMovement* mBreakMovement;  // 0xC8
    s32 _CC;
    TVec3f _D0;
    f32 mClippingDistance;  // 0xDC
    u8 _E0;
    u8 _E1;
};
