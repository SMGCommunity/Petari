#include "Game/Camera/CameraTripodBoss.hpp"
#include "Game/Camera/CamTranslatorTripodBoss.hpp"

CameraTripodBoss::CameraTripodBoss(const char *pName) : CameraTower(pName) {
    upZ = 0.0f;
}

CameraTripodBoss::~CameraTripodBoss() {

}

CamTranslatorBase *CameraTripodBoss::createTranslator() {
    return new CamTranslatorTripodBoss(this);
}
