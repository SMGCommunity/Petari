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

    /* 0x0C */ u32 _C;
    /* 0x10 */ u32 _10;
    /* 0x14 */ s32 _14;
    /* 0x18 */ s32 _18;
    /* 0x1C */ u8 _1C;
};
