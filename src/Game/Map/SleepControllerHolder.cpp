#include "Game/AreaObj/AreaObj.hpp"

void AreaObj::awake() {
    mAwake = true;
}

void AreaObj::sleep() {
    mAwake = false;
}