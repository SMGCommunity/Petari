#include "Game/MapObj/MapParts.h"
#include "math_types.h"

MapParts::~MapParts() {

}

MapParts::MapParts(const char *pName) : LiveActor(pName) {
    _8C.setZero();
}

void MapParts::appear() {
    LiveActor::appear();
    MR::tryStartAllAnim(this, MR::getModelResName(this));
}

s32 MapParts::getSensorNumMax() const {
    return 0;
}

s32 MapParts::getMoveStartSignalTime() {
    return 0x32;
}

void MapParts::connectToScene() {
    MR::connectToSceneMapParts(this);
}