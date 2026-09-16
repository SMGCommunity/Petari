#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class RosettaChair : public LiveActor {
public:
    RosettaChair(const char*);

    virtual void init(const JMapInfoIter&);

    void setDefaultPose();
    void startDemo();

    /* 0x8C */ TVec3f mDefaultPosition;
    /* 0x98 */ TVec3f mDefaultRotation;
    /* 0xA0 */ TMtx34f mScaleMtx;
};
