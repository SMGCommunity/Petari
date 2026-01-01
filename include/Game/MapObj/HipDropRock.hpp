#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/ModelObj.hpp"

class HipDropRock : public LiveActor {
public:
    HipDropRock(const char*);

    virtual void init(const JMapInfoIter&);
    virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void exeWait();
    void exeBreak();
    void exeWreck();

    void initBreakModel();
    void initItem(const JMapInfoIter&);
    void appearItem();

    ModelObj* mModel;  //_8C
    TMtx34f _90;
    s32 _C0;
    s32 _C4;
    bool _C8;
};
