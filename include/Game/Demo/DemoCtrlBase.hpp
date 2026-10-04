#pragma once

#include <revolution/types.h>

class ActorCameraInfo;
class JMapInfoIter;
class LiveActor;

class DemoCtrlBase {
public:
    DemoCtrlBase(LiveActor*, const char*);

    void init(const JMapInfoIter&);
    void end();
    void update();
    bool isDone() const;
    bool isExistEndFrame() const;
    bool tryStart();

    /* 0x00 */ LiveActor* mActor;
    /* 0x04 */ ActorCameraInfo* mCameraInfo;
    /* 0x08 */ const char* mDemoName;
    /* 0x0C */ s32 mCurrentFrame;
    /* 0x10 */ bool _10;
};
