#pragma once

#include "JSystem/JGeometry/TVec.h"

class ModelManager;

class ActorPadAndCameraCtrl {
public:
    static ActorPadAndCameraCtrl* tryCreate(const ModelManager *, const TVec3f *);
};