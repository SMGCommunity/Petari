#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class SnowplowSwitch : public LiveActor {
public:
    /// @brief Creates a new `SnowplowSwitch`.
    /// @param pName A pointer to the null-terminated name of the object.
    SnowplowSwitch(const char* pName);

    /* 0x08 */ virtual ~SnowplowSwitch();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x74 */ virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    /* 0x8C */ bool mIsDiscovered;
};
