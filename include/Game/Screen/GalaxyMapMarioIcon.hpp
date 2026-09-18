#pragma once

#include "Game/Screen/LayoutActor.hpp"

class GalaxyMapMarioIcon : public LayoutActor {
public:
    GalaxyMapMarioIcon(LayoutActor*, const char*);

    virtual void init(const JMapInfoIter& rIter);
    virtual void control();

    void showBlink();

public:
    /* 0x20 */ LayoutActor* _20;
    /* 0x24 */ const char* _24;
};
