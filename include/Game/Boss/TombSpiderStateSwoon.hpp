#pragma once

#include "Game/LiveActor/ActorStateBase.hpp"

class TombSpider;

class TombSpiderStateSwoon : public ActorStateBase< TombSpider > {
public:
    TombSpiderStateSwoon(TombSpider*);

    /* 0x0C */ virtual void init();
    /* 0x10 */ virtual void appear();

    void exeStart();
    void exeLoop();
    void exeEnd();
};
