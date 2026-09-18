#pragma once

#include "Game/Screen/LayoutActor.hpp"

class GalaxyMapTitle : public LayoutActor {
public:
    /// @brief Creates a new `GalaxyMapTitle`.
    GalaxyMapTitle();

    virtual void init(const JMapInfoIter& rIter);
    virtual void appear();

    void setModeNormal();
    void setModeHideInformation();
    void startAstroMap();
    void startGalaxyMap();
    void changeToAstroMap();
    void changeToGalaxyMap();
    void exeHide();
    void exeDisplay();
    void exeDisappear();
    void exeAppearToAnother();
    void exeDisappearToAnother() NO_INLINE;
    bool tryDisappear();

public:
    /* 0x20 */ const char* mMessageID;
};
