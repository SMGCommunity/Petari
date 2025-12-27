#pragma once

#include <revolution.h>

namespace MR {
    namespace Demo {
        f32 getCameraShakeIntensity(const char*);
        f32 getCameraShakeSpeed(const char*);
        const char* getPadRumble(const char*);
        bool isExistCameraShaking(const char*);
        bool isExistPadRumble(const char*);
        const char* getStartEffect(const char*);
    };  // namespace Demo
};  // namespace MR
