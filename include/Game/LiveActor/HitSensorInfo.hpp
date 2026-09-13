#pragma once

#include "JSystem/JGeometry/TVec.hpp"
#include <revolution.h>

class HitSensor;

class HitSensorInfo {
public:
    HitSensorInfo(const char*, HitSensor*, const TVec3f*, MtxPtr, const TVec3f&, bool);

    void update();
    void doObjCol();

    void setOffset(const TVec3f& rOffset) {
        _C = rOffset;
    }

    /* 0x00 */ const char* mName;
    /* 0x04 */ s32 mHashCode;
    /* 0x08 */ HitSensor* mSensor;
    /* 0x0C */ TVec3f _C;
    /* 0x18 */ const TVec3f* _18;
    /* 0x1C */ MtxPtr _1C;
    /* 0x20 */ bool _20;
    /* 0x21 */ u8 _21;
    /* 0x22 */ u8 _22;
    /* 0x23 */ u8 _23;
};
