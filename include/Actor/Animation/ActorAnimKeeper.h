#ifndef ACTORANIMKEEPER_H
#define ACTORANIMKEEPER_H

#include "Actor/LiveActor/LiveActor.h"

class ActorAnimKeeper
{
public:
    ActorAnimKeeper(LiveActor *);

    static ActorAnimKeeper* tryCreate(LiveActor *);
    void start(const char *);
    void update();
    bool isPlaying(const char *) const;
    void initAnimData();
};

#endif // ACTORANIMKEEPER_H