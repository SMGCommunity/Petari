#pragma once

#include "JSystem/JAudio2/JASInstEffect.hpp"

class JASInstParam;

class JASInstRand : public JASInstEffect {
public:
    JASInstRand() : JASInstEffect(), mType(VOLUME), mBaseValue(0.0f), mRange(1.0f){};

    virtual void effect(int pitch, int velocity, JASInstParam* pParams) const;

    inline void setBase(f32 base) {
        mBaseValue = base;
    }

    inline void setRange(f32 range) {
        mRange = range;
    }

    /* 0x04 */ u32 mType;
    /* 0x08 */ f32 mBaseValue;
    /* 0x0C */ f32 mRange;
};
