#include "Game/MapObj/FountainBig.h"

FountainBig::FountainBig(const char *pName) : LiveActor(pName) {
    mClippingRadius.x = 0.0f;
    mClippingRadius.y = 0.0f;
    mClippingRadius.z = 0.0f;
    mSpoutTimer = -1;
}