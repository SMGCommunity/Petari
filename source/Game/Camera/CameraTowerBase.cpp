#include "Game/Camera/CameraTowerBase.hpp"

CameraTowerBase::CameraTowerBase(const char *pName) : Camera(pName) {
    _4C = 0.0f;
    _50 = 0.0f;
    _54 = 0.0f;
    _58 = 0.0f;
    _5C = 1.0f;
    _60 = 0.0f;
    _64 = 0.0f;
    _68 = 2000.0f;
}

CameraTowerBase::~CameraTowerBase() {

}