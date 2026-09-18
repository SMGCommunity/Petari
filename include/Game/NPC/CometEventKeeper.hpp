#pragma once

#include <revolution.h>

class CometEventExecutorTimeLimit;
class GalaxyCometScreenFilter;
class GalaxyTimeLimitInfo;

class CometEventKeeper {
public:
    /// @brief Creates a new `CometEventKeeper`.
    CometEventKeeper();

    void init();
    bool isStartEvent(const char*) const;
    bool isStartTimeLimitEvent() const;
    void startCometEventIfExist();
    void endCometEvent();
    static u32 getTimeLimitFromTable(const GalaxyTimeLimitInfo*, int);
    void initCometStatus();

public:
    /* 0x0 */ CometEventExecutorTimeLimit* mExecutorTimeLimit;
    /* 0x4 */ GalaxyCometScreenFilter* mScreenFilter;
    /* 0x8 */ const char* mCometName;
    /* 0xC */ s32 mCometStateIndex;
};
