#pragma once

#include "Game/Util.h"
#include <revolution.h>

class LiveActor;

class DemoCtrlBase {
public:
    DemoCtrlBase(LiveActor *, const char *);

    void init(const JMapInfoIter &);
    void end();
    void update();
    bool isDone() const;
    bool isExistEndFrame() const;
    bool tryStart();

    LiveActor* mActor;  // _0
    u32 _4;
    const char* _8;
    s32 _C;
    s8 _10;
};