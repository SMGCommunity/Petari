#pragma once

class LiveActor;

namespace MR {
    bool isDead(const LiveActor *);

    bool isClipped(const LiveActor *);
    bool isNoEntryDrawBuffer(const LiveActor *);

    void onEntryDrawBuffer(LiveActor *);
    void offEntryDrawBuffer(LiveActor *);

    const char* getModelResName(const LiveActor *);
}
