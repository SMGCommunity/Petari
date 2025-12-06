#pragma once

#include "Game/Camera/CameraMan.hpp"

class CameraSubjective;

class CameraManSubjective : public CameraMan {
public:
    CameraManSubjective(const char*);
    virtual ~CameraManSubjective();

    virtual void init(const JMapInfoIter&);

    virtual void calc();
    virtual void notifyActivate();
    virtual void notifyDeactivate();

    CameraSubjective* mCamera;  // 0x48
};