#pragma once

#include <revolution.h>

class LiveActor;

struct ActorAnimDataInfo {
    const char* mName;  // 0x0
    f32 mStartFrame;    // 0x4
    u8 mIsKeepAnim;     // 0x8
};

struct ActorAnimKeeperInfo {
    /* 0x0 */ const char* mName;
    /* 0x4 */ ActorAnimDataInfo mBckInfo;
    /* 0x10 */ ActorAnimDataInfo mBtkInfo;
    /* 0x1C */ ActorAnimDataInfo mBrkInfo;
    /* 0x28 */ ActorAnimDataInfo mBpkInfo;
    /* 0x34 */ ActorAnimDataInfo mBtpInfo;
    /* 0x40 */ ActorAnimDataInfo mBvaInfo;
};

class ActorAnimKeeper {
public:
    ActorAnimKeeper(LiveActor*);

    bool start(const char*);
    void update();
    bool isPlaying(const char*) const;
    bool initAnimData();
    ActorAnimKeeperInfo* findAnimInfo(const char*) const;
    static ActorAnimKeeper* tryCreate(LiveActor*);

    LiveActor* mActor;                // 0x0
    s32 mNumInfo;                     // 0x4
    ActorAnimKeeperInfo* mInfoArray;  // 0x8
    ActorAnimKeeperInfo* mCurrentInfo;
};
