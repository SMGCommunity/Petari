#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class BindCone;

class JumpHole : public LiveActor {
public:
    JumpHole(const char*);

    virtual ~JumpHole();
    virtual void init(const JMapInfoIter&);
    virtual void initAfterPlacement();
    virtual void control();
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void exeWait();
    void exeSetCenter();
    void exeSetUp();
    void exeRailMove();
    void bindHole();
    void initParabola(const TVec3f&);

    BindCone* mCone;  // 0x8C
    HitSensor* _90;
    f32 _94;
    TVec3f _98;
    TVec3f _A4;
    TVec3f _B0;
    TVec3f _BC;
    TVec3f _C8;
    u32 _D4;
    f32 _D8;
    f32 _DC;
    f32 _E0;
    u32 _E4;
};
