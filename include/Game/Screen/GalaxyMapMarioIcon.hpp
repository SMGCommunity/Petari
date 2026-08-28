#pragma once

#include "Game/Screen/LayoutActor.hpp"

class GalaxyMapMarioIcon : public LayoutActor {
public:
    GalaxyMapMarioIcon(LayoutActor*, const char*);

    /* 0x0C */ virtual void init(const JMapInfoIter& rIter);
    /* 0x2C */ virtual void control();

    void showBlink();

private:
    /* 0x20 */ LayoutActor* _20;
    /* 0x24 */ const char* _24;
};
