#pragma once

#include "Game/Camera/CameraMan.h"

class CameraSubjective;

class CameraManSubjective : public CameraMan {
public:
    CameraManSubjective(const char *);
    virtual ~CameraManSubjective();
    
    virtual void init(const JMapInfoIter &);

    virtual void calc();
    virtual void notifyActivate();
    virtual void notifyDeactivate();

    CameraSubjective *mCamera;  // _48
};