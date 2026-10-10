#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class FlashingCtrl : public NameObj {
public:
    FlashingCtrl(LiveActor*, bool);

    virtual ~FlashingCtrl();
    virtual void movement();

    void start(int);
    void end();
    s32 getCurrentInterval() const;
    bool isNowFlashing() const NO_INLINE;
    bool isNowOn() const NO_INLINE;
    void updateFlashing();

    /* 0x0C */ LiveActor* mActor;
    /* 0x10 */ bool mToggleDraw;
    /* 0x11 */ bool mIsEnded;
    /* 0x12 */ u8 mOverrideInterval;
    /* 0x14 */ s32 mTimer;
    /* 0x18 */ s32 mFlashStartTime;
};
