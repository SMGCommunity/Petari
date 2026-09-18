#pragma once

#include "Game/Screen/LayoutActor.hpp"

class ButtonPaneController;
class Nerve;

class GalaxyMapSelectButton : public LayoutActor {
public:
    /// @brief Creates a new `GalaxyMapSelectButton`.
    GalaxyMapSelectButton();

    virtual void init(const JMapInfoIter& rIter);
    virtual void appear();
    virtual void control();

    void startAstroMap();
    bool isPointingAnything() const;
    bool isDecidedList() const;
    void changeToStarList();
    void exeHide();
    void exeAstroMapWait();
    void exeStarListWait();
    void exeBetweenAstroMapAndGalaxyMapFadein();
    void exeBetweenListToMapFadeout();
    void exeBetweenListToMapFadein();
    void exeBetweenMapToListFadeout();
    void exeBetweenMapToListFadein();

public:
    /* 0x20 */ Nerve* _20;
    /* 0x24 */ ButtonPaneController* mPaneCtrl;
};
