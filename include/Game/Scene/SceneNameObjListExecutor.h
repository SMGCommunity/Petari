#pragma once

#include "Game/NameObj/NameObjListExecutor.h"

namespace {
    const CategoryListInitialTable cMovementListInitTable = {};
    const CategoryListInitialTable cCalcAnimListInitTable = {};
    const DrawBufferInitialTable cDrawBufferListInitTable = {};
    const CategoryListInitialTable cDrawListInitTable = {};
};

class SceneNameObjListExecutor : public NameObjListExecutor {
public:
    inline SceneNameObjListExecutor() : NameObjListExecutor() {
    }

    virtual ~SceneNameObjListExecutor();
    virtual void initMovementList();
    virtual void initCalcAnimList();
    virtual void initCalcViewAndEntryList();
    virtual void initDrawList();
};