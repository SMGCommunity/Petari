#pragma once

#include "Game/LiveActor/LiveActor.h"

namespace {
    static const char* cSwitchJointName = "Switch";
    static const char* cMoveJointName = "Move";
    static const char* cMoveAnimName = "Move";
};

class HipDropMoveObj : public LiveActor {
public:
    HipDropMoveObj(const char *);

    virtual ~HipDropMoveObj();
    virtual void init(const JMapInfoIter &);
    virtual u32 receiveMsgPlayerAttack(u32, HitSensor *, HitSensor *);
    virtual void moveStart();
    virtual void moving();
    virtual void moveEnd();
    virtual bool isEndMove() const;

    void exeMove();

    const char* mObjectName;    // _8C
};

class HipDropDemoMoveObj : public HipDropMoveObj {
public:
    HipDropDemoMoveObj(const char *);

    virtual ~HipDropDemoMoveObj();
    virtual void moveStart();
    virtual void moving();

    TMtx34f mMtx;   // _90
};

namespace NrvHipDropMoveObj {
    NERVE(HostTypeWait);
    NERVE(HostTypeMove);
    NERVE(HostTypeDone);
};