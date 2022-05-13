#pragma once

#include "Game/LiveActor/LiveActor.h"
#include "Game/LiveActor/ActorCameraInfo.h"

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
    virtual u32 receiveMsgPlayerAttack(u32, HitSensor *, HitSensor *);
    virtual u32 receiveOtherMsg(u32, HitSensor *, HitSensor *);

    void initKeySwitchByOwner(const JMapInfoIter &);
    void appearKeySwitch(const TVec3f &);
    void exeDemoStart();
    void exeAppear();
    void exeWait();
    bool tryAvoid();

    ActorCameraInfo* mCameraInfo;   // _8C
    s32 mCurDemoFrame;              // _90
};

namespace NrvKeySwitch {
    NERVE(KeySwitchNrvDemoStart);
    NERVE(KeySwitchNrvAppear);
    NERVE(KeySwitchNrvWait);
};
