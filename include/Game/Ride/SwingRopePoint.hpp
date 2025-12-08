#pragma once

#include <JSystem/JGeometry/TVec.hpp>

class SwingRopePoint {
public:
    SwingRopePoint(const TVec3f&);

    virtual void updatePos(f32);

    void addAccel(const TVec3f&);
    bool restrict(const TVec3f&, f32, const TVec3f*);
    void strain(const TVec3f&, f32);
    void updateAxis(const TVec3f&);
    void updatePosAndAxis(const TVec3f&, f32);
    void stop();
    void setInfo(const TVec3f&, const TVec3f&, const TVec3f&, f32);
    void setPosAndAxis(const TVec3f&, const TVec3f&, const TVec3f&, const TVec3f&);
    void setAndUpdatePosAndAxis(const TVec3f&, const TVec3f&, const TVec3f&);

    /* 0x04 */ TVec3f mPosition;
    /* 0x10 */ TVec3f mVelocity;
    /* 0x1C */ TVec3f mSide;
    /* 0x28 */ TVec3f mUp;
    /* 0x34 */ TVec3f mFront;
};
