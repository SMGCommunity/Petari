#pragma once

#include "Game/MapObj/MapPartsFunction.hpp"
#include "Game/Util/PostureHolder.hpp"

class MapPartsAppearController : public MapPartsFunction {
public:
    MapPartsAppearController(LiveActor *);

    virtual ~MapPartsAppearController();
    virtual void init(const JMapInfoIter &);
    virtual void start();
    virtual void end();
    virtual bool receiveMsg(u32);

    void storeCurrentPosture();
    void initSwitchMessenger(const JMapInfoIter &);
    void startAppear();
    void startKill();
    void appearHost();
    void killHost();
    void exeDisappear();

    s32 mSignMotionType;            // 0x18
    PostureHolder* mPostureHolder;  // 0x1C
    u8 _20;
};

namespace NrvMapPartsAppearController {
    NERVE(HostTypeWait);
    NERVE(HostTypeDisappear);
};