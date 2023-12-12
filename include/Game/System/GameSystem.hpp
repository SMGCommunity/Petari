#pragma once

#include <revolution.h>
#include "Game/System/GameSystemSceneController.h"

class GameSystem {
public:
    GameSystem();

    void init();
    void frameLoop();

    u8 _0[0x24];
    GameSystemSceneController* mSceneController;    // _24
    u32 _28;
    u32 _2C;
    u32 _30;
    u32 _34;
    u32 _38;
};

void main(void);