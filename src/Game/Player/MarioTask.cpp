#include "Game/Player/Mario.hpp"

void Mario::initTask()
{
    for (int i = 0; i < 0xb; i++) {
        _984[i] = nullptr;
    }
    _974 = 0;
}
