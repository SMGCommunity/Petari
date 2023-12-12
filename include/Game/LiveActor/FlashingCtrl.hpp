#pragma once

#include "Game/LiveActor/LiveActor.h"

class FlashingCtrl : public NameObj {
public:
    FlashingCtrl(LiveActor *, bool);

    virtual ~FlashingCtrl();
    virtual void movement();

    void start(int);
    void end();
    s32 getCurrentInterval() const;
    bool isNowFlashing() const;
    bool isNowOn() const;
    void updateFlashing();

    LiveActor *mActor;      // _C
    bool mToggleDraw;       // _10
    bool mIsEnded;          // _11
    u8 mOverrideInterval;   // _12
    u8 _13;                 // padding
    s32 mTimer;             // _14
    s32 mFlashStartTime;    // _18
};