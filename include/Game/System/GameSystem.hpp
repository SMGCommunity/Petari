#pragma once

#include <revolution.h>
#include "Game/System/GameSystemSceneController.hpp"
#include "Game/System/NerveExecutor.hpp"

class GameSequenceDirector;
class GameSystemObjHolder;

class GameSystem : public NerveExecutor {
public:
    GameSystem();

    void init();
    void frameLoop();

    u32 _8;
    GameSequenceDirector* mSeqDirector;             // _C
    u32 _10;
    u32 _14;
    u32 _18;
    u32 _1C;
    GameSystemObjHolder* mObjHolder;                // _20
    GameSystemSceneController* mSceneController;    // _24
    u32 _28;
    u32 _2C;
    u32 _30;
    u32 _34;
    u32 _38;
};

void main(void);