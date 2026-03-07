#pragma once

#include "JSystem/JGeometry/TVec.hpp"
#include "revolution/types.h"

class Polta;
class HitSensor;

class PoltaSensorCtrl {
public:
    PoltaSensorCtrl(Polta*);

    s32 getSensorCount() const;
    inline s32 getCoreSensorCount() const { return 1; };
    inline s32 getBodySensorCount() const { return 3; };
    inline s32 getArmSensorCount() const { return 11; };
    void setScreamSensorSize(f32);
    void initSensor();
    bool isCoreSensor(const HitSensor*) const;
    bool isBodySensor(const HitSensor*) const;
    bool isLeftArmSensor(const HitSensor*) const;
    bool isRightArmSensor(const HitSensor*) const;
    bool isHeadSensor(const HitSensor*) const;
    bool isScreamSensor(const HitSensor*) const;

    /* 0x00 */ Polta* mPoltaPtr;
    /* 0x04 */ HitSensor* mScreamSensor;
    /* 0x08 */ HitSensor* mHeadSensor;
    /* 0x0C */ HitSensor** mCoreSensors;      // (1 element)
    /* 0x10 */ HitSensor** mBodySensors;      // (3 elements)
    /* 0x14 */ HitSensor** mLeftArmSensors;   // (11 elements)
    /* 0x18 */ HitSensor** mRightArmSensors;  // (11 elements)
};

struct PoltaSensorListEntry {
    /* 0x00 */ const char* mName;
    /* 0x04 */ const char* mJointName;
    /* 0x08 */ f32 mRadius;
    /* 0x0C */ f32 mOffsetX;
    /* 0x10 */ f32 mOffsetY;
    /* 0x14 */ f32 mOffsetZ;
};
