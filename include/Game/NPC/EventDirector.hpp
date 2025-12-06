#pragma once

#include "Game/NameObj/NameObj.hpp"
#include <JSystem/JGeometry/TVec.hpp>

class CometEventKeeper;
class PowerStarEventKeeper;
class PowerStarHolder;
class StageStateKeeper;
class TimeAttackEventKeeper;

class EventDirector : public NameObj {
public:
    /// @brief Creates a new `EventDirector`.
    EventDirector();

    virtual void init(const JMapInfoIter& rIter);

    /* 0x0C */ PowerStarEventKeeper* mPowerStarEventKeeper;
    /* 0x10 */ StageStateKeeper* mStageStateKeeper;
    /* 0x14 */ PowerStarHolder* mPowerStarHolder;
    /* 0x18 */ CometEventKeeper* mCometEventKeeper;
    /* 0x1C */ TimeAttackEventKeeper* mTimeAttackEventKeeper;
};

namespace EventFunction {
    PowerStarEventKeeper* getPowerStarEventKeeper();
    StageStateKeeper* getStageStateKeeper();
    PowerStarHolder* getPowerStarHolder();
    CometEventKeeper* getCometEventKeeper();
    TimeAttackEventKeeper* getTimeAttackEventKeeper();
    bool isStartCometEvent(const char*);
    void startCometEvent();
    void endCometEvent();
};  // namespace EventFunction

namespace MR {
    void declareEventPowerStar(const char*, s32, bool);
    void initEventSystemAfterPlacement();
    void declareEventPowerStar(const NameObj*, s32, bool);
    bool isSuccessEventPowerStar(const char*, s32);
    bool isGreenEventPowerStar(const char*, s32);
    bool isRedEventPowerStar(const char*, s32);
    bool isGrandEventPowerStar(const char*, s32);
    void appearEventPowerStar(const char*, s32, const TVec3f*, bool, bool);
    bool isEndEventPowerStarAppearDemo(const char*);
};  // namespace MR
