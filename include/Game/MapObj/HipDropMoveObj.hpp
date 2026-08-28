#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class HipDropMoveObj : public LiveActor {
public:
    HipDropMoveObj(const char*);

    /* 0x08 */ virtual ~HipDropMoveObj();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x5C */ virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    /* 0x78 */ virtual void moveStart();
    /* 0x7C */ virtual void moving();
    /* 0x80 */ virtual void moveEnd();
    /* 0x84 */ virtual bool isEndMove() const;

    void exeWait();
    void exeMove();
    void exeDone();

    const char* mObjectName;  // 0x8C
};

class HipDropDemoMoveObj : public HipDropMoveObj {
public:
    HipDropDemoMoveObj(const char*);

    /* 0x08 */ virtual ~HipDropDemoMoveObj();
    /* 0x78 */ virtual void moveStart();
    /* 0x7C */ virtual void moving();

    TMtx34f mMtx;  // 0x90
};
