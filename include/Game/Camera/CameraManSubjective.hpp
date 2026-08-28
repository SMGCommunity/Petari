#pragma once

#include "Game/Camera/CameraMan.hpp"

class CameraSubjective;

class CameraManSubjective : public CameraMan {
public:
    CameraManSubjective(const char*);

    /* 0x0C */ virtual void init(const JMapInfoIter&);

    /* 0x24 */ virtual void calc();
    /* 0x28 */ virtual void notifyActivate();
    /* 0x2C */ virtual void notifyDeactivate();

    CameraSubjective* mCamera;  // 0x48
};
