#pragma once

#include "LiveActor/Nerve/NerveExecutor.h"

class GalaxyCometState : public NerveExecutor
{
public:
    GalaxyCometState();

    virtual ~GalaxyCometState();

    void update();
    bool isLand() const;
    bool isHide() const;
    void forceToNext();
    s32 getStateIndex() const;
    s32 getPastSecond() const;
    void setStateAndPastSecond(s32, u16);

    bool mNeedsUpdate; // _8
};

class GalaxyCometStateLand : public Nerve
{
public:
    GalaxyCometStateLand();

    static GalaxyCometStateLand* sInstance;
};

class GalaxyCometStateHide : public Nerve
{
public:
    GalaxyCometStateHide();

    static GalaxyCometStateHide* sInstance;
};