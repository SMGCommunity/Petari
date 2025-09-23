#pragma once

#include <revolution.h>

class HitSensor;
class StarPiece;

namespace MR {
    void createStarPiece();

    bool giftStarPieceToTarget(HitSensor *, u32);

    StarPiece* getDeadStarPiece();
};
