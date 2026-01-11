#pragma once

#include <revolution.h>

class LiveActor;

struct ActorAnimDataInfo {
    /* 0x0 */ const char* mName;
    /* 0x4 */ f32 mStartFrame;
    /* 0x8 */ u8 mIsKeepAnim;
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

    /* 0x0 */ LiveActor* mActor;
    /* 0x4 */ s32 mNumInfo;
    /* 0x8 */ ActorAnimKeeperInfo* mInfoArray;
    /* 0xC */ ActorAnimKeeperInfo* mCurrentInfo;
};
