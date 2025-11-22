#pragma once

#include "Game/NameObj/NameObjListExecutor.hpp"

class SceneNameObjListExecutor : public NameObjListExecutor {
public:
    SceneNameObjListExecutor() : NameObjListExecutor() {}

    virtual void initMovementList();
    virtual void initCalcAnimList();
    virtual void initCalcViewAndEntryList();
    virtual void initDrawList();
};
