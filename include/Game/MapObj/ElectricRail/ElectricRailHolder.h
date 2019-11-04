#ifndef ELECTRICRAILHOLDER_H
#define ELECTRICRAILHOLDER_H

#include "types.h"

class LiveActor;

enum ElectricRailType
{
    ElectricRailType_StationaryYellow = 0,
    ElectricRailType_StationaryRed = 1,
    ElectricRailType_MovingYellow = 2,
    ElectricRailType_MovingRed = 3
};

class ElectricRailFunction
{
public:
    static void registerRail(const LiveActor *, ElectricRailType);

    static f32 getHitSensorRadius();
    static s32 calcDisplayListSize(u32, s32);
};

#endif // ELECTRICRAILHOLDER_H