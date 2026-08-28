#pragma once

#include "Game/Scene/SceneNameObjListExecutor.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/System/NerveExecutor.hpp"

class Scene : public NerveExecutor {
public:
    Scene(const char*);

    /* 0x08 */ virtual ~Scene();
    /* 0x0C */ virtual void init();
    /* 0x10 */ virtual void start();
    /* 0x14 */ virtual void update();
    /* 0x18 */ virtual void draw() const;
    /* 0x1C */ virtual void calcAnim();

    void initNameObjListExecutor();
    void initSceneObjHolder();

    SceneNameObjListExecutor* mListExecutor;  // 0x8
    u32 _C;
    SceneObjHolder* mSceneObjHolder;  // 0x10
};