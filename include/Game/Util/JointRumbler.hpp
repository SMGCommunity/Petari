#pragma once

#include <JSystem/JGeometry/TVec.hpp>

class LiveActor;
class RumbleCalculatorCosMultLinear;
class XjointTransform;

class JointRumbler {
public:
    enum Flag {
        /* 0x01 */ FLAG_1 = 1 << 0,
        /* 0x02 */ FLAG_LOCK_X = 1 << 1,
        /* 0x04 */ FLAG_LOCK_Y = 1 << 2,
        /* 0x08 */ FLAG_LOCK_Z = 1 << 3,
    };

    JointRumbler(LiveActor* pActor, const char* pJointName, f32, f32, u32, bool, s32);

    void update();
    void start();
    void reset();
    bool isRumbling() const;
    void updateModelScale(f32 scale);
    void updateScale(TVec3f* pVec, f32 scale);

    inline bool testFlag(Flag flag) const {
        return static_cast< u32 >(mFlags & flag) == flag;
    }

    /* 0x00 */ RumbleCalculatorCosMultLinear* mRumbleCalculator;
    /* 0x04 */ XjointTransform* mJointTransform;
    /* 0x08 */ s32 _8;
    /* 0x0C */ s32 _C;
    /* 0x10 */ f32 mModelScale;
    /* 0x14 */ u16 mFlags;
    /* 0x16 */ bool _16[2];
};
