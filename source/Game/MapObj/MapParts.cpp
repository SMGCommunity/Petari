#include "Game/MapObj/MapParts.h"
#include "math_types.h"

MapParts::MapParts(const char *pName) : LiveActor(pName) {
    _8C.setZero();
}