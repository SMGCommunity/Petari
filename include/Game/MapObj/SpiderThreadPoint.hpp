#pragma once

#include <JSystem/JGeometry/TVec.hpp>

class SpiderThreadPoint {
public:
    SpiderThreadPoint(const TVec3f&, f32);

    bool updateSpring();
    void updateDrawVec(const TVec3f*, const TVec3f*);
    void updateWind(f32);
    void updatePos();
    void restrict(const TVec3f*, f32);
    void updateHang(const TVec3f&);
    bool tryTouch(f32, const TVec3f&, s32);
    bool tryPush(const TVec3f&, f32);
    void startThreadLevelSound();

    inline void setPos(const TVec3f& rPos) {
        mPosition.set(rPos);
        mVelocity.zero();
    }

    inline f32 getPosZDiff(const TVec3f& rPos) const { return rPos.z - mPosition.z; }

    // TODO: this should be removed when the appropriate inline in JAudio is made/found
    inline s32 systemLevelParam(f32 a, f32 b) { return static_cast< s16 >(a) << 16 | static_cast< s16 >(b) & 0xFFFF; }

    /* 0x00 */ TVec3f mBasePos;
    /* 0x0C */ TVec3f mPosition;
    /* 0x18 */ TVec3f mVelocity;
    /* 0x24 */ f32 mSpringFactor;
    /* 0x28 */ TVec3f mUp;
    /* 0x34 */ TVec3f mFront;
    /* 0x40 */ s32 mWindStartTime;
    /* 0x44 */ s32 mWindTime;
    /* 0x48 */ f32 mPrevStretchDist;
};
