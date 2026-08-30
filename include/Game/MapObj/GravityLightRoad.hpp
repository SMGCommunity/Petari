#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class GravityLightRoad : public LiveActor {
public:
    GravityLightRoad(const char*);
    
    virtual void init(const JMapInfoIter&);
    virtual void updateHitSensor(HitSensor*);
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void updateBindActorMtx();
    void endBind();
    void startDisappear();
    bool tryStartBind(LiveActor*);
    bool isPlayerInCylinder() const;

    void exeWait();
    void exeDraw();
    void exeDisappear();

    /* 0x8C */ LiveActor* mHost;
    /* 0x90 */ f32 mSpeed;
    /* 0x94 */ f32 mRadius;
    /* 0x98 */ TVec3f mEndPos;
};
