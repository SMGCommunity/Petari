#include "Game/Camera/CameraDPD.h"

CameraDPD::CameraDPD(const char *pName) : Camera(pName) {
    _4C = 0.0f;
    _50 = 0.0f;
    _54 = 0.0f;
    _58 = 0.0f;
    _5C = 0.0f;
    _90 = 1;
    _94 = 0;
    _98 = 0.0f;
    _9C = 40.0f;
    _A0 = 1.5707964f;
    _A4 = 1.5707964f;
    _A8 = 0.0f;
    _AC = 0.05f;
    _B0 = 0.99f;
    _B4 = 0;
    _B8 = 0.0f;
    _60.identity();
}