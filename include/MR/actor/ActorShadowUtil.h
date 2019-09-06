#ifndef ACTORSHADOWUTIL_H
#define ACTORSHADOWUTIL_H

class LiveActor;

namespace MR
{
    void onShadowVisibleSyncHostAll(LiveActor *);
    void offShadowVisibleSyncHostAll(LiveActor *);
};

#endif // ACTORSHADOWUTIL_H