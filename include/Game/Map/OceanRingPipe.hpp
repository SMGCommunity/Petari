#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class OceanRing;
class OceanRingPipeInside;
class OceanRingPipeOutside;

class OceanRingPipe : public LiveActor {
public:
    OceanRingPipe(const OceanRing*, f32, f32);

    virtual ~OceanRingPipe();
    virtual void init(const JMapInfoIter&);
    virtual void movement();

    void initPoints();
    f32 getAngle() {
        f32 f = 180.0f / (_9C - 1);
        return PI_180 * f;
    }
    inline int calcPointIndex(int i, int j) const {
        return (i * _9C) + j;
    }

    const OceanRing* mOceanRing;  // 0x8C
    bool _90;
    u32 _94;
    s32 _98;
    s32 _9C;
    TVec3f* _A0;
    TVec3s* _A4;
    f32 _A8;
    f32 _AC;
    OceanRingPipeInside* mPipeInside; // 0xB0
    OceanRingPipeOutside* mPipeOutside; // 0xB4
};
