#pragma once

#include <revolution.h>

class HitSensor;

namespace MR {
    void createStarPiece();

    bool giftStarPieceToTarget(HitSensor*, u32);
}; // namespace MR
