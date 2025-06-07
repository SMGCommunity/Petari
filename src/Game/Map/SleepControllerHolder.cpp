#include "Game/AreaObj/AreaObj.hpp"

void AreaObj::awake() {
    mIsAwake = true;
}

void AreaObj::sleep() {
    mIsAwake = false;
}