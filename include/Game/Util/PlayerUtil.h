#pragma once

#include "JSystem/JGeometry/TVec.h"

namespace MR {
    TVec3f* getPlayerPos();
    TVec3f* getPlayerCenterPos();

    bool isPlayerInBind();

    bool isPlayerBindedBigBubble();
};
