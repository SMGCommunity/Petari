#pragma once

#include "Game/Screen/LayoutActor.hpp"

class SimpleLayout;

class PlayerActionGuidance : public LayoutActor {
public:
    /// @brief Creates a new `PlayerActionGuidance`.
    PlayerActionGuidance();

    virtual void init(const JMapInfoIter& rIter);
    virtual void control();

    void createSpinLayout();
    void createTamakoroLayout();
    void exeWaitFocusIn();
    void exeFadein();
    void startWaitAnimTamakoro();
    void exeDisplay();
    void exeSuspend();
    void exeFadeout();
    void startAnimAllLayout(const char*);
    void startWaitAnimAllLayout();
    void setAnimFrameAndStopAllLayout(f32);
    void hideAllLayout();
    bool isInVolumePlayer(const char*, bool) const;

    /* 0x20 */ SimpleLayout* mSpinLayout;
    /* 0x24 */ SimpleLayout* mTamakoroLayout;
    /* 0x28 */ LayoutActor* mCurrentLayout;
    /* 0x2C */ s32 mGuidanceState;
    /* 0x30 */ s32 mGuidancePrevState;
    /* 0x34 */ bool _34;
    /* 0x35 */ bool _35;
    /* 0x36 */ bool _36;
    /* 0x37 */ bool _37;
};

namespace MR {
    PlayerActionGuidance* getPlayerActionGuidance();
};
