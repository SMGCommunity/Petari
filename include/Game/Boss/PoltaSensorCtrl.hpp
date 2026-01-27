#pragma once

#include "JSystem/JGeometry/TVec.hpp"
#include "revolution/types.h"

class Polta;
class HitSensor;

class PoltaSensorCtrl {
public:
	PoltaSensorCtrl(Polta*);

	s32 getSensorCount() const;
	inline s32 getCoreSensorCount() const;
	inline s32 getBodySensorCount() const;
    inline s32 getArmSensorCount() const;
	void setScreamSensorSize(f32);
	void initSensor();
	bool isCoreSensor(const HitSensor*) const;
	bool isBodySensor(const HitSensor*) const;
	bool isLeftArmSensor(const HitSensor*) const;
	bool isRightArmSensor(const HitSensor*) const;
	bool isHeadSensor(const HitSensor*) const;
	bool isScreamSensor(const HitSensor*) const;

	Polta* mPoltaPtr;					// 0x00
	HitSensor* mScreamSensor;			// 0x04
	HitSensor* mHeadSensor;				// 0x08
	HitSensor** mCoreSensors;			// 0x0C  (1 element)
	HitSensor** mBodySensors;			// 0x10  (3 elements)
	HitSensor** mLeftArmSensors;	// 0x14  (11 elements)
	HitSensor** mRightArmSensors;	// 0x18  (11 elements)
};


struct PoltaSensorListEntry {
    const char* mName;
    const char* mJointName;
    f32 mRadius;
    f32 mOffsetX;
    f32 mOffsetY;
    f32 mOffsetZ;
};
