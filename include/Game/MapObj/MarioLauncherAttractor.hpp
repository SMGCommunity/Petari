#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class PointGravityCreator;

class MarioLauncherAttractor : public LiveActor {
public:
    MarioLauncherAttractor(const char* pName);

    virtual void init(const JMapInfoIter& rIter);

    void exeWait();

    /* 0x8C */ PointGravityCreator* mGravityCreator;
};
