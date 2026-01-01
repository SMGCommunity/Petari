#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/MapObj/Coin.hpp"

class CoinGroup : public LiveActor {
public:
    CoinGroup(const char*);

    virtual ~CoinGroup() {}

    virtual void init(const JMapInfoIter&);
    virtual void appear();

    virtual void initCoinArray(const JMapInfoIter&) = 0;
    virtual void placementCoin();
    virtual const char* getCoinName() const;

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
