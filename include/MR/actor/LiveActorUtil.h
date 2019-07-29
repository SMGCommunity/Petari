#ifndef LIVEACTORUTIL_H
#define LIVEACTORUTIL_H

#include "types.h"

class LiveActor;

namespace MR
{
    bool isDead(const LiveActor *);
    bool isHiddenModel(const LiveActor *);
    void showModel(LiveActor *);
    void hideModel(LiveActor *);
    void hideModelAndOnCalcAnim(LiveActor *);

    bool isClipped(const LiveActor *);
    bool isNoEntryDrawBuffer(const LiveActor *);

    void setCollisionMtx(LiveActor *);
    void validateCollisionParts(LiveActor *);
    void invalidateCollisionParts(LiveActor *);
    
    bool isCalcGravity(const LiveActor *);

    void calcGravity(const LiveActor *);

    void updateLightCtrl(LiveActor *);
    void setBaseTRMtx(LiveActor *, Mtx);
};

#endif // LIVEACTORUTIL_H