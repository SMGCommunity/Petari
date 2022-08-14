#include "Game/MapObj/AirBubble.h"

AirBubble::AirBubble(const char *pName) : LiveActor(pName) {
    _A4.x = 0.0f;
    _A4.y = 0.0f;
    _A4.z = 0.0f;
    _B0 = 0.0f;
    _C4 = 360;
}