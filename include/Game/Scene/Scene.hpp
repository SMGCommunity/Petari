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

    /* 0x08 */ SceneNameObjListExecutor* mListExecutor;
    /* 0x0C */ u32 _C;
    /* 0x10 */ SceneObjHolder* mSceneObjHolder;
};
