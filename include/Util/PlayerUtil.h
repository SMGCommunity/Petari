#pragma once

#include "JSystem/JGeometry/TVec3.h"

namespace MR
{
    bool isPlayerSquat();

    JGeometry::TVec3<f32>* getPlayerCenterPos();
    JGeometry::TVec3<f32>* getPlayerPos();
};