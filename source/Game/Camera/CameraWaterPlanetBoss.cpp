#include "Game/Camera/CameraWaterPlanetBoss.hpp"
#include "Game/Camera/CamTranslatorWaterPlanetBoss.hpp"

CameraWaterPlanetBoss::CameraWaterPlanetBoss(const char *pName) : Camera(pName) {
    mAxisY = 300.0f;
    mAxisX = 1200.0f;
    mDist = 0.01f;
    _58 = 0.01f;
    _5C = 0;
    _60 = 0.0f;
    _64 = 0.0f;
    _68 = 0.0f;
    _6C = false;
    mWPointX = 0.0f;
    mWPointY = 0.0f;
    mWPointZ = 0.0f;
    mAxisZ = 0.0f;
    mUpY = 0.0f;
    mUpX = 0.0f;
    mUpZ = 0.0f;
    _8C = false;
    _90 = 0;
    _94 = 1.0f;
    _98 = 0.0f;
    _9C = 0.0f;
}

CameraWaterPlanetBoss::~CameraWaterPlanetBoss() {

}

CamTranslatorBase *CameraWaterPlanetBoss::createTranslator() {
    return new CamTranslatorWaterPlanetBoss(this);
}
