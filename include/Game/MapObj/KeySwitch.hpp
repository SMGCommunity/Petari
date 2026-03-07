#pragma once

#include "Game/LiveActor/ActorCameraInfo.hpp"
#include "Game/LiveActor/LiveActor.hpp"

class KeySwitch : public LiveActor {
public:
    KeySwitch(const char*);

    virtual ~KeySwitch();
    virtual void init(const JMapInfoIter&);
    virtual void appear();
    virtual void kill();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void initKeySwitchByOwner(const JMapInfoIter&);
    void appearKeySwitch(const TVec3f&);
    void exeDemoStart();
    void exeAppear();
    void exeWait();
    bool tryAvoid();

    ActorCameraInfo* mCameraInfo;  // 0x8C
    s32 mCurDemoFrame;             // 0x90
};
