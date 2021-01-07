#pragma once

#include "LiveActor/Nerve/NerveExecutor.h"
#include "Scene/SceneNameObjListExecutor.h"
#include "Scene/SceneObjHolder.h"

class Scene : public NerveExecutor
{
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

    SceneNameObjListExecutor* mExecutor; // _8
    u32 _C;
    SceneObjHolder* mObjHolder; // _10
};