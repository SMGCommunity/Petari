#pragma once

#include "JSystem/JGeometry/TVec.hpp"

class ModelManager;

class ActorPadAndCameraCtrl {
public:
    static ActorPadAndCameraCtrl* tryCreate(const ModelManager*, const TVec3f*);
    void update();
};