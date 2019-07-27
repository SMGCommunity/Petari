#ifndef ACTORSENSORUTIL_H
#define ACTORSENSORUTIL_H

class LiveActor;

namespace MR
{
    bool tryUpdateHitSensorsAll(LiveActor *);
    void clearHitSensors(LiveActor *);
};

#endif // ACTORSENSORUTIL_H