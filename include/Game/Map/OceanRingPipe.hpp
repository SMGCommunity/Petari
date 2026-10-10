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

    /* 0x8C */ const OceanRing* mOceanRing;
    /* 0x90 */ bool _90;
    /* 0x94 */ u32 _94;
    /* 0x98 */ s32 _98;
    /* 0x9C */ s32 _9C;
    /* 0xA0 */ TVec3f* _A0;
    /* 0xA4 */ TVec3s* _A4;
    /* 0xA8 */ f32 _A8;
    /* 0xAC */ f32 _AC;
    /* 0xB0 */ OceanRingPipeInside* mPipeInside;
    /* 0xB4 */ OceanRingPipeOutside* mPipeOutside;
};
