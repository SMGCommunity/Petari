#pragma once

#include <revolution.h>
#include "Game/NameObj/NameObj.hpp"

class HitSensor;

class MarioMessenger : public NameObj {
public:
    MarioMessenger(HitSensor *);

    HitSensor *_C;

    HitSensor **requests;
    u32 *_14;

    u32 size;
};
