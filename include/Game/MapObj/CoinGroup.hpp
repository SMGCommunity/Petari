#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class ActorCameraInfo;
class Coin;

class CoinGroup : public LiveActor {
public:
    CoinGroup(const char*);

    /* 0x08 */ virtual ~CoinGroup() {
    }

    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x24 */ virtual void appear();

    /* 0x78 */ virtual void initCoinArray(const JMapInfoIter&) = 0;
    /* 0x7C */ virtual void placementCoin();
    /* 0x80 */ virtual const char* getCoinName() const;

    void killCoinAll();
    void appearCoinAll();
    void appearCoinFix();
    void appearCoinAllTimer();
    void setCoinTrans(s32, const TVec3f&);
    void exeAppear();
    void exeTryStartDemo();
    void exeDemoAppear();
    void exeKill();

    Coin** mCoinArray;             // 0x8C
    ActorCameraInfo* mCameraInfo;  // 0x90
    u32 mCoinCount;                // 0x94
    s32 mTimeLimit;                // 0x98
    bool mIsPurpleCoinGroup;       // 0x9C
};
