#include "Game/Camera/CameraTwistedPassage.h"
#include "Game/Camera/CamTranslatorTwistedPassage.h"

CameraTwistedPassage::CameraTwistedPassage(const char *pName) : Camera(pName) {
    _4C = 0;
    _50 = 500.0f;
    _54 = 1300.0f;
    _58 = 0;
}

CameraTwistedPassage::~CameraTwistedPassage() {

}

CamTranslatorBase *CameraTwistedPassage::createTranslator() {
    return new CamTranslatorTwistedPassage(this);
}
