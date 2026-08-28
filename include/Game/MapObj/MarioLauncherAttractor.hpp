#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class PointGravityCreator;

class MarioLauncherAttractor : public LiveActor {
public:
    MarioLauncherAttractor(const char* pName);

    /* 0x0C */ virtual void init(const JMapInfoIter& rIter);

    void exeWait();

    /* 0x8C */ PointGravityCreator* mGravityCreator;
};
