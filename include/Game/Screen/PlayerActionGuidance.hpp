#pragma once

#include "Game/Screen/LayoutActor.h"

class PlayerActionGuidance : public LayoutActor {
public:
    PlayerActionGuidance();

    virtual ~PlayerActionGuidance();
    virtual void init(const JMapInfoIter &);
    virtual void control();

    void createSpinLayout();
    void createTamakoroLayout();
    void exeWaitFocusIn();
    void exeFadein();
    void startWaitAnimTamakoro();
    void exeDisplay();
    void exeSuspend();
    void exeFadeout();
    void startAnimAllLayout(const char *);
    void startWaitAnimAllLayout();
    void setAnimFrameAndStopAllLayout(f32);
    void hideAllLayout();
    bool isInVolumePlayer(const char *, bool) const;
};

namespace MR {
    PlayerActionGuidance* getPlayerActionGuidance();
};