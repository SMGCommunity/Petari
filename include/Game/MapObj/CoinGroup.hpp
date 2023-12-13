#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/MapObj/Coin.hpp"

class CoinGroup : public LiveActor {
public:
    CoinGroup(const char *);

    virtual ~CoinGroup() {

    }

    virtual void init(const JMapInfoIter &);
    virtual void appear();
    
    virtual void initCoinArray(const JMapInfoIter &) = 0;
    virtual void placementCoin();
    virtual const char* getCoinName() const;

    void killCoinAll();
    void appearCoinAll();
    void appearCoinFix();
    void appearCoinAllTimer();
    void setCoinTrans(s32, const TVec3f &);
    void exeAppear();
    void exeDemoAppear();

    Coin** mCoinArray;              // _8C
    ActorCameraInfo* mCameraInfo;   // _90
    u32 mCoinCount;                 // _94
    s32 mTimeLimit;                 // _98
    bool mIsPurpleCoinGroup;        // _9C
};

namespace NrvCoinGroup {
    NERVE(CoinGroupNrvAppear);
    NERVE(CoinGroupNrvTryStartDemo);
    NERVE(CoinGroupNrvDemoAppear);
    NERVE(CoinGroupNrvKill);
};