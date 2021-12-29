#include "Game/Camera/CameraTripodBoss.h"
#include "Game/Camera/CamTranslatorTripodBoss.h"

CameraTripodBoss::CameraTripodBoss(const char *pName) : CameraTower(pName) {
    upZ = 0.0f;
}

CamTranslatorBase *CameraTripodBoss::createTranslator() {
    return new CamTranslatorTripodBoss(this);
}
