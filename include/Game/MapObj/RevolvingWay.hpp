#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/JGeometry/TQuat.hpp>

class RevolvingWay : public LiveActor {
public:
    RevolvingWay(const char*);

    /* 0x08 */ virtual ~RevolvingWay();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x48 */ virtual void control();
    /* 0x4C */ virtual void calcAndSetBaseMtx();

    void exeWait();
    void addAccelMoment();

    /* 0x0x8C */ TQuat4f mRotateQuat;
    /* 0x0x9C */ TVec3f mFriction;
    /* 0x0xA8 */ f32 mRadius;
};
