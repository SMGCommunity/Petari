#pragma once

#include "Game/NameObj/NameObj.h"
#include "Game/NameObj/NameObjGroup.h"

class StopSceneDelayRequest : public NameObj {
public:
    StopSceneDelayRequest();

    virtual ~StopSceneDelayRequest();
    virtual void movement();

    s32 _C;
    u32 _10;
};

class StopSceneController : public NameObj {
public:
    StopSceneController();

    virtual ~StopSceneController();
    virtual void movement();

    void requestStopScene(s32);
    void requestStopSceneDelay(s32, s32);
    bool isSceneStopped() const;

    NameObjGroup* mDelayRequests;       // _C
    s32 _10;
};