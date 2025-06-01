#pragma once

#include "Game/System/NerveExecutor.hpp"

class GameSystemErrorWatcher : public NerveExecutor {
public:
    GameSystemErrorWatcher();

    u32 _8;
    u32 _C;
    u32 _10;
    u32 _14;
    u32 _18;
    u32 _1C;
    u32 _20;
    u32 _24;
    u32 _28;
    u32 _2C;
    u8 _30;
};
