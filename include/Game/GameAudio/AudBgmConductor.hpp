#pragma once

#include "Game/NameObj/NameObj.hpp"

class AudBgmConductor : public NameObj {
public:
    /// @brief Creates a new `AudBgmConductor`.
    AudBgmConductor();

    virtual void init(const JMapInfoIter& rIter);
    virtual void movement();

    void playPinchEventBgmLevel(bool);
    void setStateBit(u32);
    void stopPinchEventBgmSoon();
    void movementPinchEventBgm();

    // TODO: clarify StateBit_1
    enum StateBits {
        StateBit_1 = 0b010,
        StateBit_Delay = 0b100,
    };

    /* 0x0C */ u32 mStateBits;
    /* 0x10 */ s32 mState;
    /* 0x14 */ s32 mStartDelay;
    /* 0x18 */ s32 mFadeInDelay;
    /* 0x1C */ bool mStopImmediately;
};
