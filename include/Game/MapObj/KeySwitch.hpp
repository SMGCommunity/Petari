#pragma once

#include "Game/LiveActor/ActorCameraInfo.hpp"
#include "Game/LiveActor/LiveActor.hpp"

class KeySwitch : public LiveActor {
public:
    KeySwitch(const char*);

    /* 0x08 */ virtual ~KeySwitch();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x24 */ virtual void appear();
    /* 0x2C */ virtual void kill();
    /* 0x48 */ virtual void control();
    /* 0x4C */ virtual void calcAndSetBaseMtx();
    /* 0x5C */ virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    /* 0x74 */ virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void initKeySwitchByOwner(const JMapInfoIter&);
    void appearKeySwitch(const TVec3f&);
    void exeDemoStart();
    void exeAppear();
    void exeWait();
    bool tryAvoid();

    ActorCameraInfo* mCameraInfo;  // 0x8C
    s32 mCurDemoFrame;             // 0x90
};
