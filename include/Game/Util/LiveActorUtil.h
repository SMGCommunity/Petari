#pragma once

#include "JSystem/JGeometry/TVec.h"
#include <revolution.h>

class LiveActor;
class ResourceHolder;
class ActorLightCtrl;

namespace MR {
    void copyTransRotateScale(const LiveActor *, LiveActor *);
    bool isDead(const LiveActor *);

    bool isValidDraw(const LiveActor *);
    bool isClipped(const LiveActor *);
    bool isInvalidClipping(const LiveActor *);

    bool isHiddenModel(const LiveActor *);

    ResourceHolder* getResourceHolder(const LiveActor *);

    bool isNoEntryDrawBuffer(const LiveActor *);

    void onEntryDrawBuffer(LiveActor *);
    void offEntryDrawBuffer(LiveActor *);

    const char* getModelResName(const LiveActor *);

    void calcAnimDirect(LiveActor *);

    void initLightCtrl(LiveActor *);
    void initLightCtrlForPlayer(LiveActor *);
    void initLightCtrlNoDrawEnemy(LiveActor *);
    void initLightCtrlNoDrawMapObj(LiveActor *);
    void updateLightCtrl(LiveActor *);
    void updateLightCtrlDirect(LiveActor *);
    void loadActorLight(const LiveActor *);
    void calcLightPos0(TVec3f *, const LiveActor *);
    void calcLightPos1(TVec3f *, const LiveActor *);
    const GXColor* getLightAmbientColor(const LiveActor *);
    ActorLightCtrl* getLightCtrl(const LiveActor *);
}
