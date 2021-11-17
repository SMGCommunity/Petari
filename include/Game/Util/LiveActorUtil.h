#pragma once

class LiveActor;
class ResourceHolder;

namespace MR {
    void copyTransRotateScale(const LiveActor *, LiveActor *);
    bool isDead(const LiveActor *);

    bool isClipped(const LiveActor *);

    ResourceHolder* getResourceHolder(const LiveActor *);

    bool isNoEntryDrawBuffer(const LiveActor *);

    void onEntryDrawBuffer(LiveActor *);
    void offEntryDrawBuffer(LiveActor *);

    const char* getModelResName(const LiveActor *);
}
