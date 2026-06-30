#pragma once

#include "Game/Screen/LayoutActor.hpp"

class BackButton;
class GalaxyMap;
class GalaxyMapBackground;
class GalaxyMapSelectButton;
class JUTTexture;
class PowerStarList;
class ResTIMG;
class WipeFade;

class GalaxyMapController : public LayoutActor {
public:
    enum Mode {
        Mode_AstroMap,
        Mode_GalaxyMap,
        Mode_PowerStarList,
        Mode_NewGalaxyDiscover,
        Mode_NewTicoGalaxyDiscover,
        Mode_NewDomeDiscover,
        Mode_TicoCometPray,
        Mode_ChallengeGalaxyDiscover,
    };

    /// @brief Creates a new `GalaxyMapController`.
    GalaxyMapController();

    virtual void init(const JMapInfoIter& rIter);
    virtual void movement();
    virtual void draw() const;
    virtual void calcAnim();
    virtual void appear();
    virtual void kill();

    void drawForCapture();
    void startGalaxyMap();
    void startAstroMap();
    void startNewGalaxyDiscover();
    void startNewTicoGalaxyDiscover();
    void startNewDomeDiscover();
    void startTicoCometPray();
    void startChallengeGalaxyDiscover();
    void killAllLayout();
    bool tryPushBackButton();
    bool tryChangeMode();
    bool tryAppearGalaxyMap();
    bool tryDisplayAndKeyWait();
    void setNerveAccordingToNextModeFromMap();
    void setFaderParam(GXColor, int);
    void capture();

    void exeReadyModeMap();
    void exeFadeinModeMap();
    void exeAppearGalaxyMap();
    void exeDisplayGalaxyMap();
    void exeAppearAstroMap();
    void exeDisplayAstroMap();
    void exeReadyPowerStarList();
    void exeAppearPowerStarList();
    void exeDisplayPowerStarList();
    void exeDisappearPowerStarList();
    void exeNewGalaxyDiscoverPhaseAstroMap();
    void exeNewGalaxyDiscoverPhaseAppearGalaxyMap();
    void exeNewGalaxyDiscoverPhaseDisplayGalaxyMap();
    void exeNewTicoGalaxyDiscoverPhaseAstroMap();
    void exeNewDomeDiscoverPhaseAstroMap();
    void exeTicoCometFadeoutWhite();
    void exeTicoCometWaitWhite();
    void exeTicoCometPray();
    void exeChallengeGalaxyDiscoverPhaseAstroMap();
    void exeFadeout();
    void exeReadyGame();

    /* 0x20 */ GalaxyMap* mMap;
    /* 0x24 */ GalaxyMapSelectButton* mMapSelectButton;
    /* 0x28 */ GalaxyMapBackground* mMapBackground;
    /* 0x2C */ PowerStarList* mPowerStarList;
    /* 0x30 */ BackButton* mBackButton;
    /* 0x34 */ WipeFade* mWipe;
    /* 0x38 */ JUTTexture* _38;
    /* 0x3C */ s32 mMode;
    /* 0x40 */ s32 mWipeFrame;
};

namespace MR {
    void startGalaxyMapLayout();
    void startAstroMapLayout();
    void startAstroMapLayoutForNewGalaxyDiscover();
    void startAstroMapLayoutForNewTicoGalaxyDiscover();
    void startAstroMapLayoutForNewDomeDiscover();
    void startGalaxyMapLayoutForTicoComet();
    void startAstroMapLayoutForChallengeGalaxyDiscover();
    bool isActiveGalaxyMapLayout();
    const ResTIMG* getGalaxyMapResTIMG();
};  // namespace MR
