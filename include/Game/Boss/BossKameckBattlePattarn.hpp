#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class BossKameckBattlePattarn {
public:
    BossKameckBattlePattarn(s32*, bool);

    void initPattarn(s32*);
    s32 goNextPattarn();

    s32* _0;
    s32 _4;
    u32 _8;
    s32 _C;
    bool _10;
};