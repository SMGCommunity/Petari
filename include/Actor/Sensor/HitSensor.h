#ifndef HITSENSOR_H
#define HITSENSOR_H

#include "types.h"

#include "Actor/LiveActor/LiveActor.h"

class HitSensor
{
public:
    HitSensor(u64, u16, f32, LiveActor *);

    bool recieveMessage(u64, HitSensor *);

    u32 _0;
    u32 _4;
    u32 _8;
    u32 _C;
    u32 _10;
    u16 _14;
    u16 _16;
    u32 _18;
    u32 _1C;
    u32 _20;
    LiveActor* mActor; // _24
};

#endif // HITSENSOR_H