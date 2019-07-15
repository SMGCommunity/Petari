#ifndef HITSENSOR_H
#define HITSENSOR_H

#include "types.h"

class LiveActor;

class HitSensor
{
public:
    HitSensor(u64, u16, f32, LiveActor *);

    bool recieveMessage(u64, HitSensor *);
};

#endif // HITSENSOR_H