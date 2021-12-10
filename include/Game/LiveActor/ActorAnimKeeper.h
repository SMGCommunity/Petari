#pragma once

#include <revolution.h>

class LiveActor;

struct ActorAnimDataInfo {
    const char* mName;  // _0
    f32 mStartFrame;    // _4
    u8 mIsKeepAnim;     // _8
};

class ActorAnimKeeper {
public:
    ActorAnimKeeper(LiveActor *);

    void start(const char *);
    void update();
    bool isPlaying(const char *) const;

    static ActorAnimKeeper* tryCreate(LiveActor *);

    LiveActor* mActor;  // _0
    u32 _4;
    u32 _8;
    u32 _C;
};