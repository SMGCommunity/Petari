#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class HipDropMoveObj : public LiveActor {
public:
    HipDropMoveObj(const char*);

    virtual ~HipDropMoveObj();
    virtual void init(const JMapInfoIter&);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    virtual void moveStart();
    virtual void moving();
    virtual void moveEnd();
    virtual bool isEndMove() const;

    void exeWait();
    void exeMove();
    void exeDone();

    const char* mObjectName;  // 0x8C
};

class HipDropDemoMoveObj : public HipDropMoveObj {
public:
    HipDropDemoMoveObj(const char*);

    virtual ~HipDropDemoMoveObj();
    virtual void moveStart();
    virtual void moving();

    TMtx34f mMtx;  // 0x90
};
