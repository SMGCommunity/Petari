#pragma once

#include "JSystem/JGeometry/TMatrix.h"

class CameraRotChecker {
public:
    CameraRotChecker();

    void update();

    TMtx34f _0;
    bool _30;
    u8 _31[3];
};