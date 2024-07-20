#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class BossKameckBattlePattarn {
public:
    BossKameckBattlePattarn(s32 *, bool);

    void initPattarn(s32 *);
    s32 goNextPattarn();
};