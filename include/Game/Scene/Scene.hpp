#pragma once

#include "Game/Scene/SceneNameObjListExecutor.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/System/NerveExecutor.hpp"

class Scene : public NerveExecutor {
public:
    Scene(const char*);

    virtual ~Scene();
    virtual void init();
    virtual void start();
    virtual void update();
    virtual void draw() const;
    virtual void calcAnim();

    void initNameObjListExecutor();
    void initSceneObjHolder();

    SceneNameObjListExecutor* mListExecutor;  // 0x8
    u32 _C;
    SceneObjHolder* mSceneObjHolder;  // 0x10
};