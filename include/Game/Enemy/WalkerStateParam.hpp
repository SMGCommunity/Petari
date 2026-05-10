#pragma once

#include <revolution.h>

class WalkerStateParam {
public:
    WalkerStateParam();

    /* 0x00 */ f32 mGravityAccel;
    /* 0x04 */ f32 mAirFriction;
    /* 0x08 */ f32 mGroundFriction;
    /* 0x0C */ f32 mPlayerNearDistance;
    /* 0x10 */ f32 mPlayerSightFanDegreeH;
    /* 0x14 */ f32 mPlayerSightFanDegreeV;
};
