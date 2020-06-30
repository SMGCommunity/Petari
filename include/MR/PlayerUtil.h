#pragma once

#include "JGeometry/TVec3.h"

namespace MR
{
    bool isPlayerSquat();

    JGeometry::TVec3<f32>* getPlayerCenterPos();
    JGeometry::TVec3<f32>* getPlayerPos();
};