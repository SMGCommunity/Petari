#pragma once

#include <revolution.h>
#include "Game/System/GameSystemSceneController.hpp"
#include "Game/System/NerveExecutor.hpp"

class GameSequenceDirector;
class GameSystemObjHolder;
class NameObjListExecutor;

class GameSystem : public NerveExecutor {
public:
    GameSystem();

    void init();
    void frameLoop();

    u32 _8;
    GameSequenceDirector* mSeqDirector;  // 0xC
    u32 _10;
    u32 _14;
    u32 _18;
    u32 _1C;
    GameSystemObjHolder* mObjHolder;              // 0x20
    GameSystemSceneController* mSceneController;  // 0x24
    u32 _28;
    u32 _2C;
    u32 _30;
    u32 _34;
    u32 _38;
};

void main(void);