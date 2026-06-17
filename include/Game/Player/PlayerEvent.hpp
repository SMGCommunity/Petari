#pragma once

#include "Game/NameObj/NameObj.hpp"

class EventSequence;
class HashSortTable;

typedef void (EventSequence::*EventFunc1)(u16, u16);
typedef bool (EventSequence::*EventFunc2)(u16);

class EventSequencer : public NameObj {
public:
    /// @brief Creates a new `EventSequencer`.
    EventSequencer();

    virtual ~EventSequencer();
    virtual void init(const JMapInfoIter& rIter);
    virtual void movement();

    void startEvent(const char*);

    /* 0x0C */ HashSortTable* _C;
    /* 0x10 */ EventSequence* _10;
    /* 0x14 */ u32 _14;
};

class EventSequence {
private:
    /* 0x00 */ u32 _0;
    /* 0x04 */ u16 _4;
    /* 0x06 */ u16 _6;
    /* 0x08 */ void* _8;
    /* 0x0C */ void* _C;
    /* 0x10 */ void* _10;
    /* 0x14 */ void* _14;
    /* 0x18 */ void* _18;
    /* 0x1C */ bool _1C;
    /* 0x1D */ bool _1D;

public:
    EventSequence(u32);

    virtual void updateBefore();
    virtual void updateAfter();

    void clearFlag();
    void addEventOnTime(const char*, EventFunc1, u16);
    void addEventInTime(const char*, EventFunc1, u16, u16);
    void addEventInStatus(const char*, EventFunc1, EventFunc2);
    void addEventInPhase(const char*, EventFunc1, u16);
    bool checkAndRun(u32);
    void playSound(const char*);
    void playAnimation(const char*);
    bool isMissLayoutClosed(u16);
    void setSpot(f32, u32);
    void decLeft(u16, u16);
    void closeDefaultLayout(u16, u16);
    void doCloseWipe(u16, u16);
    void doWaitAfterWipe(u16, u16);
};
