#ifndef SOUNDUTIL_H
#define SOUNDUTIL_H

#include "Actor/LiveActor/LiveActor.h"

namespace MR
{
    void startSound(const LiveActor *, const char *, s32, s32);
    void actorSoundMovement(LiveActor *);

    void startLevelSound(const LiveActor *, const char *, s32, s32, s32);
};

#endif // SOUNDUTIL_H