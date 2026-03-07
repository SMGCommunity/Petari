#pragma once

#include "Game/Util/Color.hpp"
#include "JSystem/JGeometry.hpp"
#include <revolution.h>

class LiveActor;
class DrawBuffer;

namespace MR {
    void initActorLightInfoLightType(LiveActor*, s32);
    
    void initActorLightInfoDrawBuffer(LiveActor*, DrawBuffer*);

    void requestPointLight(const LiveActor*, TVec3f, Color8, f32, s32);

    void loadLightPlayer();
};  // namespace MR
