#pragma once

#include "Game/NameObj/NameObjListExecutor.hpp"

class SceneNameObjListExecutor : public NameObjListExecutor {
public:
    SceneNameObjListExecutor() : NameObjListExecutor() {
    }

    /* 0x0C */ virtual void initMovementList();
    /* 0x10 */ virtual void initCalcAnimList();
    /* 0x14 */ virtual void initCalcViewAndEntryList();
    /* 0x18 */ virtual void initDrawList();
};
