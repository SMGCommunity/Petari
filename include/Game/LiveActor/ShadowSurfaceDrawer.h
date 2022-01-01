#pragma once

#include "Game/LiveActor/ShadowDrawer.h"

class ShadowSurfaceDrawInit : public NameObj {
public:
    ShadowSurfaceDrawInit(const char *);

    virtual ~ShadowSurfaceDrawInit();

    void initDraw();
};

class ShadowSurfaceDrawer : public ShadowDrawer {
public:
    ShadowSurfaceDrawer(const char *);

    virtual ~ShadowSurfaceDrawer();


};