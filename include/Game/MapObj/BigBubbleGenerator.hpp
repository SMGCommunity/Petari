#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class BigBubble;

class BigBubbleGenerator : public LiveActor {
public:
    BigBubbleGenerator(const char*);

    virtual void init(const JMapInfoIter&);
    virtual void initAfterPlacement();
    virtual void control();

    void initFromJMapParam(const JMapInfoIter&);

    void onActive();
    void offActive();
    bool tryActive();
    bool tryNonActive();

    bool isActiveRange();
    bool isNonActiveRange();

    void exeNonActive();
    void exeActive();

    /* 0x8C */ s32 mSpawnTimer;
    /* 0x90 */ f32 mMinSize;
    /* 0x94 */ f32 mMaxSize;
    /* 0x98 */ s32 mIsObstruct;  // NOTE: is a bool but stored as s32
    /* 0x9C */ s32 mShape;
    /* 0xA0 */ s32 mLimitterID;
    /* 0xA4 */ BigBubble* mAttachedBubble;
};

namespace MR {
    NameObj* createBigBubbleGenerator(const char*);
    NameObj* createBigObstructBubbleGenerator(const char*);
};  // namespace MR
