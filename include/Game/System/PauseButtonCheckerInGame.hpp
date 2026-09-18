#pragma once

#include <revolution.h>

class PauseButtonCheckerInGame {
public:
    PauseButtonCheckerInGame();

    void update();
    bool isPermitToPlusPause() const;
    bool isPermitToMinusPause() const;
    bool isPermitToPause() const NO_INLINE;

public:
    /* 0x0 */ s32 mPlusHoldFrame;
    /* 0x4 */ s32 mMinusHoldFrame;
};
