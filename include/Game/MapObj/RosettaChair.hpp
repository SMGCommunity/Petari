#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class RosettaChair : public LiveActor {
public:
    RosettaChair(const char*);

    virtual void init(const JMapInfoIter&);

    void setDefaultPose();
    void startDemo();

    TVec3f _8C;
    TVec3f _98;
    TMtx34f _A0;
};
