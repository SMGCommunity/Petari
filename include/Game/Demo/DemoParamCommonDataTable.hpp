#pragma once

namespace MR {
    namespace Demo {
        float getCameraShakeIntensity(const char *);
        float getCameraShakeSpeed(const char *);
        const char* getPadRumble(const char *);
        bool isExistCameraShaking(const char *);
        bool isExistPadRumble(const char *);
        const char* getStartEffect(const char *);
    };
};
