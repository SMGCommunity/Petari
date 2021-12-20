#pragma once

#include "Game/Camera/CameraMan.h"

class CameraSubjective;

class CameraManSubjective : public CameraMan {
public:
    CameraManSubjective(const char *);

    CameraSubjective *mCamera;  // _48
};