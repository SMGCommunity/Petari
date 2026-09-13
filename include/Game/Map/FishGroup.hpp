#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class FishGroup;

class Fish : public LiveActor {
public:
    Fish(const char*, FishGroup*, int, bool);

    virtual ~Fish();

    virtual void init(const JMapInfoIter& rIter);
    virtual void control();
    virtual void calcAndSetBaseMtx();

    void exeWander();
    void exeApproach();
    void exeEscape();

    bool tryEscapeFromPlayer();
    void updateFollowPointPos();

    /* 0x8C */ FishGroup* mGroup;
    /* 0x90 */ int mFishType;
    /* 0x94 */ bool mCanEscape;
    /* 0x98 */ TVec3f mFollowPos;
    /* 0xA4 */ TVec3f mFollowPosOffset;
    /* 0xB0 */ TVec3f mFront;
    /* 0xBC */ f32 mDistanceToApproach;
    /* 0xC0 */ s32 mTimeToSearchPlayer;
    /* 0xC4 */ s32 mStepToMove;
    /* 0xC8 */ TVec3f* mEscapePos;
};

class FishGroup : public LiveActor {
public:
    FishGroup(const char*);

    virtual ~FishGroup();

    virtual void init(const JMapInfoIter& rIter);
    virtual void startClipped();
    virtual void endClipped();
    virtual void control();

    /* 0x8C */ TVec3f mUp;
    /* 0x98 */ s32 mFishNum;
    /* 0x9C */ Fish** mFish;
    /* 0xA0 */ f32 mSpeed;
    /* 0xA4 */ TVec3f mClippingCenter;
};
