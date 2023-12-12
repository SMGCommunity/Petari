#pragma once

#include "Game/LiveActor/LiveActor.h"

class SwitchEventListener {
public:
    inline SwitchEventListener() { }

    virtual void refresh(bool useOn) {
        if (useOn) {
            listenSwitchOnEvent();
        }
        else {
            listenSwitchOffEvent();
        }
    }
    virtual void listenSwitchOnEvent() = 0;
    virtual void listenSwitchOffEvent() = 0;
};

class ActorAppearSwitchListener : public SwitchEventListener {
public:
    ActorAppearSwitchListener(LiveActor *, bool, bool);

    virtual void listenSwitchOnEvent();
    virtual void listenSwitchOffEvent();

    LiveActor* mActor;      // _4
    bool mUsesOn;           // _8
    bool mUsesOff;          // _9
};