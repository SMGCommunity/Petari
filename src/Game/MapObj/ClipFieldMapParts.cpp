#include "Game/MapObj/ClipFieldMapParts.hpp"

ClipFieldMapParts::ClipFieldMapParts(const char *pName) : GeneralMapParts(pName) {

}

void ClipFieldMapParts::control() {
    if (MR::isValidSwitchA(this)) {
        if (MR::isOnSwitchA(this)) {
            MR::validateCollisionParts(this);
        }
        else {
            MR::invalidateCollisionParts(this);
        }
    }

    GeneralMapParts::control();
}

void ClipFieldMapParts::connectToScene() {
    MR::connectToClippedMapParts(this);
}

ClipFieldMapParts::~ClipFieldMapParts() {

}

void ClipFieldMapParts::init(const JMapInfoIter &rIter) {
    GeneralMapParts::init(rIter);
    MR::needStageSwitchReadA(this, rIter);
    getSensor("body")->setType(0x49);
}