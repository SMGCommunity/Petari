#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Util/SpringValue.hpp"

class TripodBossBreakMovement : public LiveActor {
public:
    TripodBossBreakMovement(const char*);

    virtual ~TripodBossBreakMovement();
    virtual void movement();

    void start(const TPos3f&, s32);
    void setBreakDownLevel(s32);
    void exeStartBreak();
    void exeBreak();
    void exeWait() {}

    TPos3f _8C;
    SpringValue* mSpring;  // 0xBC
    TVec3f _C0;
    TVec3f _CC;
    TVec3f _D8;
    TVec3f _E4;
    f32 _F0;
    f32 _F4;
    f32 _F8;
    u32 _FC;
    s32 _100;
    u32 _104;
};
