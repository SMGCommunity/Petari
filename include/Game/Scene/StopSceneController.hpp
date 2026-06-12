#pragma once

#include "Game/NameObj/NameObj.hpp"

class NameObjGroup;

class StopSceneDelayRequest : public NameObj {
public:
    /// @brief Creates a new `StopSceneDelayRequest`.
    StopSceneDelayRequest();

    virtual void movement();

    /* 0x0C */ s32 mFrame;
    /* 0x10 */ s32 mDelay;
};

class StopSceneController : public NameObj {
public:
    /// @brief Creates a new `StopSceneController`.
    StopSceneController();

    virtual void movement();

    void requestStopScene(s32);
    void requestStopSceneDelay(s32, s32);
    bool isSceneStopped() const;

    /* 0x0C */ NameObjGroup* mDelayRequestArray;
    /* 0x10 */ s32 mFrame;
};
