#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/ActorCameraInfo.hpp"

namespace {
    static const char* cDemoName = "カギ出現";
};

class KeySwitch : public LiveActor {
public:
    KeySwitch(const char *);

    virtual ~KeySwitch();
    virtual void init(const JMapInfoIter &);
    virtual void appear();
    virtual void kill();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual bool receiveMsgPlayerAttack(u32, HitSensor *, HitSensor *);
    virtual bool receiveOtherMsg(u32, HitSensor *, HitSensor *);

    void initKeySwitchByOwner(const JMapInfoIter &);
    void appearKeySwitch(const TVec3f &);
    void exeDemoStart();
    void exeAppear();
    void exeWait();
    bool tryAvoid();

    ActorCameraInfo* mCameraInfo;   // 0x8C
    s32 mCurDemoFrame;              // 0x90
};

namespace NrvKeySwitch {
    NERVE(KeySwitchNrvDemoStart);
    NERVE(KeySwitchNrvAppear);
    NERVE(KeySwitchNrvWait);
};
