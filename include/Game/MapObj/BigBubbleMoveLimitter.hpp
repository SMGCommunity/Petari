#pragma once

#include "Game/NameObj/NameObj.hpp"
#include <JSystem/JGeometry/TVec.hpp>

class BigBubbleMoveLimitter : public NameObj {
public:
    enum LimitterType {
        LimitterType_Cylinder = 0,
        LimitterType_Plane = 1,
    };

    enum LimitterMarginType {
        LimitterMarginType_Center = -1,
        LimitterMarginType_Outside = 0,
        LimitterMarginType_Inside = 1,
    };

    BigBubbleMoveLimitter(const char*, s32);

    virtual void init(const JMapInfoIter&);

    bool limitPosition(TVec3f*, f32) const;
    bool limitVelocity(TVec3f*, const TVec3f&, f32) const;

    s32 getLimitterID() const {
        return mID;
    }

    inline f32 getLimitterOffset(f32 offset) const {
        switch (mLimitMarginType) {
        case LimitterMarginType_Outside:
            return offset;
        case LimitterMarginType_Inside:
            return -offset;
        default:
            return 0.0f;
        }
    }

    /* 0x0C */ s32 mID;
    /* 0x10 */ TVec3f mPosition;
    /* 0x1C */ TVec3f mAxis;
    /* 0x28 */ TVec3f mSide;
    /* 0x34 */ TVec3f mUp;
    /* 0x40 */ TVec3f mFront;
    /* 0x4C */ f32 mCylinderRadius;
    /* 0x50 */ f32 mCylinderHeight;
    /* 0x54 */ f32 mPlaneWidth;
    /* 0x58 */ f32 mPlaneHeight;
    /* 0x5C */ s32 mLimitMarginType;
    /* 0x60 */ s32 mType;
};

namespace MR {
    NameObj* createBigBubbleMoveLimitterCylinder(const char*);
    NameObj* createBigBubbleMoveLimitterPlane(const char*);
};  // namespace MR
