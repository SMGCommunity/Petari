#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class DemoPositionController : LiveActor {
public:
    DemoPositionController(const char*, const JMapInfoIter&);
    virtual ~DemoPositionController();

    virtual void control();

    void initAnimCamera(const char*);
    void startDemo(const char*);
    void endDemo(const char*);

    ActorCameraInfo* pCameraInfo;  // 0x8C
};
