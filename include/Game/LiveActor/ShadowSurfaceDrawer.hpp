#pragma once

#include "Game/LiveActor/ShadowDrawer.hpp"

class ShadowSurfaceDrawInit : public NameObj {
public:
    ShadowSurfaceDrawInit(const char*);

    /* 0x08 */ virtual ~ShadowSurfaceDrawInit();

    static void initDraw();
};

class ShadowSurfaceDrawer : public ShadowDrawer {
public:
    ShadowSurfaceDrawer(const char*);

    /* 0x08 */ virtual ~ShadowSurfaceDrawer();
};