#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class SpinDriverUtil {
public:
    static void setShadowAndClipping(LiveActor*, TVec3f*, f32, f32, f32*);
};