#pragma once

#include "Game/System/NerveExecutor.h"

class Scene : public NerveExecutor {
public:
    Scene(const char *);

    virtual ~Scene();
    virtual void init();
    virtual void start();
    virtual void update();
    virtual void draw() const;
    virtual void calcAnim();

    u32 _8;
    u32 _C;
    u32 _10;
};