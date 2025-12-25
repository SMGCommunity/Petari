#include "Game/Enemy/MoguStone.hpp"

namespace {
    static f32 hAddHeight = 200.0f;
};  // namespace

namespace NrvMoguStone {
    NEW_NERVE(MoguStoneNrvTaken, MoguStone, Taken);
    NEW_NERVE(MoguStoneNrvThrow, MoguStone, Throw);
    NEW_NERVE(MoguStoneNrvFall, MoguStone, Fall);
};  // namespace NrvMoguStone
