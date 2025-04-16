#pragma once

#include <revolution.h>

class PauseButtonCheckerInGame {
public:
    PauseButtonCheckerInGame();

    void update();
    bool isPermitToPlusPause() const;
    bool isPermitToMinusPause() const;
    bool isPermitToPause() const NO_INLINE;

    s32 _0;
    s32 _4;
};
