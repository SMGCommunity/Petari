#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class SpringValue;

class TripodBossBreakMovement : public LiveActor {
public:
    /// @brief Creates a new `TripodBossBreakMovement`.
    /// @param pName A pointer to the null-terminated name of the object.
    TripodBossBreakMovement(const char* pName);

    virtual void movement();

    void start(const TPos3f&, s32);
    void setBreakDownLevel(s32);

    void exeWait();
    void exeStartBreak();
    void exeBreak();

    /* 0x08C */ TPos3f _8C;
    /* 0x0BC */ SpringValue* mSpring;
    /* 0x0C0 */ TVec3f _C0;
    /* 0x0CC */ TVec3f _CC;
    /* 0x0D8 */ TVec3f _D8;
    /* 0x0E4 */ TVec3f _E4;
    /* 0x0F0 */ f32 _F0;
    /* 0x0F4 */ f32 _F4;
    /* 0x0F8 */ f32 _F8;
    /* 0x0FC */ u32 _FC;
    /* 0x100 */ s32 _100;
    /* 0x104 */ u32 _104;
};
