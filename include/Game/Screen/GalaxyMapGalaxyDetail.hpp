#pragma once

#include "Game/Screen/LayoutActor.hpp"

class GalaxyInfoLayoutSetter;
class IconAButton;

class GalaxyMapGalaxyDetail : public LayoutActor {
public:
    GalaxyMapGalaxyDetail();

    /* 0x0C */ virtual void init(const JMapInfoIter& rIter);
    /* 0x14 */ virtual void movement();
    /* 0x18 */ virtual void draw() const;
    /* 0x1C */ virtual void calcAnim();
    /* 0x24 */ virtual void appear();
    /* 0x28 */ virtual void kill();
    /* 0x2C */ virtual void control();

    void startDisplay(const char*, bool);
    void exeAppear();
    void exeDisplay();
    void exeDisappear();

    GalaxyInfoLayoutSetter* mInfoLayoutSetter;
    IconAButton* mAButtonIcon;
};
