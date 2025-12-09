#pragma once

#include "Game/Ride/SwingRopePoint.hpp"
#include <JSystem/JGeometry/TVec.hpp>

class Binder;

class SledRopePoint : public SwingRopePoint {
public:
    SledRopePoint(const TVec3f&, f32);

    virtual void updatePos(f32);

    bool bindToMapCollision(f32);

    /* 0x40 */ Binder* mBinder;
    /* 0x44 */ bool mNoBind;
};
