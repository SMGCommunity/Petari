#pragma once

#include "Game/Boss/TripodBossKillerGenerator.hpp"
#include "Game/NameObj/NameObj.hpp"

class TripodBossKillerGeneraterCircle : public NameObj {
public:
    TripodBossKillerGeneraterCircle(const char*);

    virtual ~TripodBossKillerGeneraterCircle();
    virtual void init(const JMapInfoIter&);

    void placementGenerater();

    TripodBossKillerGenerater* mGenerators;  // 0xC
    TVec3f mPosition;                        // 0x10
    TVec3f mRotation;                        // 0x1C
    const f32* mAngleTable;                  // 0x28
    s32 mNumAngles;                          // 0x2C
    f32 _30;
    f32 _34;
    f32 _38;
    f32 _3C;
    u32 _40;
};
