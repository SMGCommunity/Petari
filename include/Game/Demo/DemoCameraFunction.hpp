#pragma once

#include "Game/Demo/DemoCameraKeeper.hpp"

namespace DemoCameraFunction {
    bool isCameraTargetMario(const DemoCameraInfo *);
    bool setStringNullIfEmpty(const char **);
    void makeAnimCameraName(char *, u32, const DemoCameraInfo *);
};
