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

    LiveActor* mActor;     // 0xC
    bool mToggleDraw;      // 0x10
    bool mIsEnded;         // 0x11
    u8 mOverrideInterval;  // 0x12
    s32 mTimer;            // 0x14
    s32 mFlashStartTime;   // 0x18
};
