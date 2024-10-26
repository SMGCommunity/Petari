#include "Game/Player/Mario.hpp"

void Mario::clearSlope()
{
    _8F0 = 0.0f;
    mMovementStates._23 = false;
    _8F8.zero();
    _280 = 0.0f;
    _910.zero();
    _284.zero();
    _3C2 = 0;
    _2C4.zero();
    _3C4 = 0;
    _904.zero();
}
