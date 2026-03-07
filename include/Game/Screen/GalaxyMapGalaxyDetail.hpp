#pragma once

#include "Game/Screen/LayoutActor.hpp"

class GalaxyInfoLayoutSetter;
class IconAButton;

class GalaxyMapGalaxyDetail : public LayoutActor {
public:
    GalaxyMapGalaxyDetail();

    virtual void init(const JMapInfoIter& rIter);
    virtual void movement();
    virtual void draw() const;
    virtual void calcAnim();
    virtual void appear();
    virtual void kill();
    virtual void control();

    void startDisplay(const char*, bool); 
    void exeAppear();
    void exeDisplay();
    void exeDisappear();

    GalaxyInfoLayoutSetter* mInfoLayoutSetter;
    IconAButton* mAButtonIcon;
};
