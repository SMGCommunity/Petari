#pragma once

#include "Game/MapObj/MapPartsFunction.hpp"

class PostureHolder;

class MapPartsAppearController : public MapPartsFunction {
public:
    MapPartsAppearController(LiveActor*);

    virtual void init(const JMapInfoIter&);
    virtual void start();
    virtual void end();
    virtual bool receiveMsg(u32);

    void storeCurrentPosture();
    void initSwitchMessenger(const JMapInfoIter&);
    void startAppear();
    void startKill();
    void appearHost();
    void killHost();
    void exeWait();
    void exeDisappear();

    /* 0x18 */ s32 mSignMotionType;
    /* 0x1C */ PostureHolder* mPostureHolder;
    /* 0x20 */ u8 _20;
};
