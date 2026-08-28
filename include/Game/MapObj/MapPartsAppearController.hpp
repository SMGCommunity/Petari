#pragma once

#include "Game/MapObj/MapPartsFunction.hpp"

class PostureHolder;

class MapPartsAppearController : public MapPartsFunction {
public:
    MapPartsAppearController(LiveActor*);

    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x28 */ virtual void start();
    /* 0x2C */ virtual void end();
    /* 0x30 */ virtual bool receiveMsg(u32);

    void storeCurrentPosture();
    void initSwitchMessenger(const JMapInfoIter&);
    void startAppear();
    void startKill();
    void appearHost();
    void killHost();
    void exeWait();
    void exeDisappear();

    s32 mSignMotionType;            // 0x18
    PostureHolder* mPostureHolder;  // 0x1C
    u8 _20;
};
