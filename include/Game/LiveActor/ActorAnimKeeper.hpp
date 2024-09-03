#pragma once

#include <revolution.h>

class LiveActor;

struct ActorAnimDataInfo {
    const char* mName;  // 0x0
    f32 mStartFrame;    // 0x4
    u8 mIsKeepAnim;     // 0x8
};

class ActorAnimKeeper {
public:
    ActorAnimKeeper(LiveActor *);

    void start(const char *);
    void update();
    bool isPlaying(const char *) const;

    static ActorAnimKeeper* tryCreate(LiveActor *);

    LiveActor* mActor;  // 0x0
    u32 _4;
    u32 _8;
    u32 _C;
};