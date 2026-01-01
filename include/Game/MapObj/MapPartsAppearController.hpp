#pragma once

#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/MapPartsFunction.hpp"

class PostureHolder;

class MapPartsAppearController : public MapPartsFunction {
public:
    MapPartsAppearController(LiveActor*);

    virtual ~MapPartsAppearController();
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

    s32 mSignMotionType;            // 0x18
    PostureHolder* mPostureHolder;  // 0x1C
    u8 _20;
};
