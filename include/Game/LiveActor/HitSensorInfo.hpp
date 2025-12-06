#pragma once

#include "JSystem/JGeometry/TVec.hpp"
#include <revolution.h>

class HitSensor;

class HitSensorInfo {
public:
    HitSensorInfo(const char*, HitSensor*, const TVec3f*, MtxPtr, const TVec3f&, bool);

    void update();
    void doObjCol();

    const char* mName;   // 0x0
    s32 mHashCode;       // 0x4
    HitSensor* mSensor;  // 0x8
    TVec3f _C;
    const TVec3f* _18;
    MtxPtr _1C;
    bool _20;
    u8 _21;  // 0xPadding
    u8 _22;
    u8 _23;
};
