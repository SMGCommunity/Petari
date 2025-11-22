#pragma once

#include "Game/Util.hpp"
#include <revolution.h>

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

    LiveActor* mActor;             // 0x0
    ActorCameraInfo* mCameraInfo;  // 0x04
    const char* mDemoName;         // 0x08
    s32 mCurrentFrame;             // 0x0C
    bool _10;
};
