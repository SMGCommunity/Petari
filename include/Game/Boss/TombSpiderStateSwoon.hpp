#pragma once

#include "Game/LiveActor/ActorStateBase.hpp"

class TombSpider;

class TombSpiderStateSwoon : public ActorStateBase< TombSpider > {
public:
    TombSpiderStateSwoon(TombSpider*);

    virtual void init();
    virtual void appear();

    void exeStart();
    void exeLoop();
    void exeEnd();

    /* 0x0C */ TombSpider* mParent;
};
