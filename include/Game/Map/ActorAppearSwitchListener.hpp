#pragma once

#include <revolution/types.h>

class LiveActor;

class SwitchEventListener {
public:
    /* 0x08 */ virtual void refresh(bool useOn) {
        if (useOn) {
            listenSwitchOnEvent();
        } else {
            listenSwitchOffEvent();
        }
    }

    /* 0x0C */ virtual void listenSwitchOnEvent() = 0;
    /* 0x10 */ virtual void listenSwitchOffEvent() = 0;
};

class ActorAppearSwitchListener : public SwitchEventListener {
public:
    ActorAppearSwitchListener(LiveActor*, bool, bool);

    /* 0x0C */ virtual void listenSwitchOnEvent();
    /* 0x10 */ virtual void listenSwitchOffEvent();

    LiveActor* mActor;  // 0x4
    bool mUsesOn;       // 0x8
    bool mUsesOff;      // 0x9
};
