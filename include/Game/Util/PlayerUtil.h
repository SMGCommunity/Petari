#pragma once

#include "JSystem/JGeometry/TVec.h"

namespace MR {
    TVec3f* getPlayerPos();
    TVec3f* getPlayerCenterPos();

    bool isPlayerDead();

    bool isPlayerInBind();

    bool isPlayerBindedBigBubble();

    void forceKillPlayerByAbyss();
};
