#pragma once

#include <revolution/types.h>

class LiveActor;

class SwitchEventListener {
public:
    virtual void refresh(bool useOn) {
        if (useOn) {
            listenSwitchOnEvent();
        } else {
            listenSwitchOffEvent();
        }
    }

    virtual void listenSwitchOnEvent() = 0;
    virtual void listenSwitchOffEvent() = 0;
};

class ActorAppearSwitchListener : public SwitchEventListener {
public:
    ActorAppearSwitchListener(LiveActor*, bool, bool);

    virtual void listenSwitchOnEvent();
    virtual void listenSwitchOffEvent();

    /* 0x4 */ LiveActor* mActor;
    /* 0x8 */ bool mUsesOn;
    /* 0x9 */ bool mUsesOff;
};
