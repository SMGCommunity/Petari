#pragma once

#include <JSystem/JGeometry/TMatrix.hpp>

class CameraRotChecker {
public:
    CameraRotChecker();

    void update();

    /* 0x00 */ TPos3f mMtx;
    /* 0x30 */ bool mIsRotatingHard;
};
