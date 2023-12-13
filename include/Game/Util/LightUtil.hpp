#pragma once

#include <revolution.h>
#include "Game/Util/Color.hpp"
#include "JSystem/JGeometry.hpp"

class LiveActor;

namespace MR {
    void initActorLightInfoLightType(LiveActor *, s32);

    void requestPointLight(const LiveActor *, TVec3f, Color8, f32, s32);

    void loadLightPlayer();
};