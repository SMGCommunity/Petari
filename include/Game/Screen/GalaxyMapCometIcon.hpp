#pragma once

#include "Game/Screen/LayoutActor.hpp"

class GalaxyMapCometIcon : public LayoutActor {
public:
    GalaxyMapCometIcon(const char*, LayoutActor*, const char*);

    virtual ~GalaxyMapCometIcon();
    virtual void init(const JMapInfoIter&);
    virtual void calcAnim();
    virtual void appear();
    virtual void control();

    void calcAnimForCapture(const nw4r::lyt::DrawInfo&);
    void syncStatus();

private:
    /* 0x20 */ const char* _20;
    /* 0x24 */ LayoutActor* _24;
    /* 0x28 */ const char* _28;
};
