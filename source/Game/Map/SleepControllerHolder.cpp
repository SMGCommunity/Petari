#include "Game/AreaObj/AreaObj.h"

void AreaObj::awake() {
    mAwake = true;
}

void AreaObj::sleep() {
    mAwake = false;
}