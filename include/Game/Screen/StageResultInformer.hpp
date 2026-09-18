#pragma once

#include "Game/Screen/LayoutActor.hpp"
#include "Game/System/NerveExecutor.hpp"

class IconAButton;
class StageResultInformer;
class StageResultSequenceChecker;

class DisplayInformationForResult : public NerveExecutor {
public:
    DisplayInformationForResult(StageResultInformer*);

    void exeAppear();
    void exeDisplay();
    void exeDisappear();
    void exeIdle();

    /* 0x08 */ StageResultInformer* mHost;
    /* 0x0C */ const char* mMessageId;
};

class StageResultInformer : public LayoutActor {
public:
    /// @brief Creates a new `StageResultInformer`.
    StageResultInformer();

    virtual void init(const JMapInfoIter& rIter);
    virtual void appear();
    virtual void kill();

    void exeAppearGetPowerStar();
    void exeWaitBeforeCountUpPowerStar();
    void exeCountUpPowerStar();
    void exeDisplayGetPowerStar();
    void exeDisappearGetPowerStar();
    void exeShowGalaxyComplete();
    void exeAppearUpdateBestScore();
    void exeDisplayUpdateBestScore();
    void exeDisappearUpdateBestScore();
    void exeAppearGetStarPiece();
    void exeDisplayGetStarPiece();
    void exeWaitBeforeDisappearGetStarPiece();
    void exeDisappearGetStarPiece();
    void exeShowNewGalaxyDiscover();
    void exeShowGalaxyMap();
    void exeShowGetPictureBook();
    void exeShowTicoGalaxyAppear();
    void exeShowAstroMapForTico();
    void initBestScoreWindow();
    void decideNextNerve();
    bool tryWaitSystemPadTriggerDecide(const Nerve*);
    bool tryWaitIntervalBeforeKeyWait(const Nerve*, int);
    bool tryShowAndKeyWaitInformationWindow(const char*, const Nerve*);
    void setCounterPaneShowHide(bool, bool, bool);

public:
    /* 0x20 */ DisplayInformationForResult* mInformationDisplayer;
    /* 0x24 */ StageResultSequenceChecker* mSequenceChecker;
    /* 0x28 */ IconAButton* mIconAButton;
    /* 0x2C */ s32 mClearedStarPieceNum;
    /* 0x30 */ bool _30;
    /* 0x30 */ bool _31;
};
