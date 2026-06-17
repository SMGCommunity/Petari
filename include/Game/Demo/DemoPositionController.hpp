#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class ActorCameraInfo;

class DemoPositionController : public LiveActor {
public:
    DemoPositionController(const char*, const JMapInfoIter&);

    virtual void control();

    void initAnimCamera(const char*);
    void startDemo(const char*);
    void endDemo(const char*);

    ActorCameraInfo* pCameraInfo;  // 0x8C
};
