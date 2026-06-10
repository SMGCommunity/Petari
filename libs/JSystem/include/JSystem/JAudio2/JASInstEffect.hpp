#pragma once

#include "JSystem/JAudio2/JASBasicInst.hpp"

class JASInstParam;

struct JASInstEffect {
    enum EffectType {
        VOLUME = 0,
        PITCH = 1,
        PAN = 2,
        FXMIX = 3,
        DOLBY = 4,
    };

    JASInstEffect(){};

    virtual void effect(int, int, JASInstParam*) const;
};
