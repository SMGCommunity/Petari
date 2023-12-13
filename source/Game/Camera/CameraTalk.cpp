#include "Game/Camera/CameraTalk.hpp"
#include "Game/Camera/CamTranslatorTalk.hpp"

CameraTalk::CameraTalk(const char *pName) : Camera(pName) {
    _4C = 0.0f;
    _50 = 0.0f;
    _54 = 0.0f;
    _58 = 0.0f;
    _5C = 1.0f;
    _60 = 0.0f;
    _64 = 120.0f;
    _68 = 450.0f;
    _6C = 0;
}

CameraTalk::~CameraTalk() {

}

CamTranslatorBase *CameraTalk::createTranslator() {
    return new CamTranslatorTalk(this);
}
