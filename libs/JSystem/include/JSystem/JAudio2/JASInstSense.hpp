#pragma once

#include "JSystem/JAudio2/JASInstEffect.hpp"

class JASInstParam;

class JASInstSense : public JASInstEffect {
public:
    enum SenseType {
        SENSE_ZERO = 0,
        SENSE_VELOCITY = 1,
        SENSE_PITCH = 2,
    };

    JASInstSense() : JASInstEffect(), mType(VOLUME), mSenseType(0), mMaxPoint(60), mStartLvl(0.0f), mEndLvl(0.0f){};

    virtual void effect(int pitch, int velocity, JASInstParam* pParams) const;
    void setParams(int senseType, int maxPoint, f32 startLvl, f32 endLvl);

    /* 0x04 */ u32 mType;
    /* 0x08 */ u8 mSenseType;
    /* 0x09 */ u8 mMaxPoint;
    /* 0x0C */ f32 mStartLvl;
    /* 0x10 */ f32 mEndLvl;
};
