#pragma once

#include "Game/System/NerveExecutor.h"
#include "Game/Scene/SceneNameObjListExecutor.h"
#include "Game/Scene/SceneObjHolder.h"

class Scene : public NerveExecutor {
public:
    Scene(const char *);

    virtual ~Scene();
    virtual void init();
    virtual void start();
    virtual void update();
    virtual void draw() const;
    virtual void calcAnim();

    void initNameObjListExecutor();
    void initSceneObjHolder();

    SceneNameObjListExecutor* mListExecutor;    // _8
    u32 _C;
    SceneObjHolder* mSceneObjHolder;            // _10
};