#pragma once

#include "Game/NameObj/NameObj.hpp"
#include <JSystem/JGeometry/TVec.hpp>

class TripodBossKillerGenerater;

class TripodBossKillerGeneraterCircle : public NameObj {
public:
    /// @brief Creates a new `TripodBossKillerGeneraterCircle`.
    /// @param pName A pointer to the null-terminated name of the object.
    TripodBossKillerGeneraterCircle(const char* pName);

    virtual ~TripodBossKillerGeneraterCircle();
    virtual void init(const JMapInfoIter&);

    void placementGenerater();

    /* 0x0C */ TripodBossKillerGenerater* mGenerater;
    /* 0x10 */ TVec3f mPosition;
    /* 0x1C */ TVec3f mRotation;
    /* 0x28 */ const f32* mAngleTable;
    /* 0x2C */ s32 mAngleNum;
    /* 0x30 */ f32 _30;
    /* 0x34 */ f32 _34;
    /* 0x38 */ f32 _38;
    /* 0x3C */ f32 _3C;
    /* 0x40 */ u32 _40;
};
