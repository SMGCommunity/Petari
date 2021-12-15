#include "Game/Camera/CameraParamChunk.h"
#include "Game/Camera/CameraTargetObj.h"

const char *CameraParamChunk::getClassName() const {
    return "Base";
}

bool CameraTargetObj::isWaterMode() const {
    return false;
}

bool CameraTargetObj::isOnWaterSurface() const {
    return false;
}

bool CameraTargetObj::isFooFighterMode() const {
    return false;
}