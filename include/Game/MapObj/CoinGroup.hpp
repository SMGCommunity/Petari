#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class ActorCameraInfo;
class Coin;

class CoinGroup : public LiveActor {
public:
    CoinGroup(const char*);

    virtual void init(const JMapInfoIter&);
    virtual void appear();

    virtual void initCoinArray(const JMapInfoIter&) = 0;
    virtual void placementCoin() {
    }
    virtual const char* getCoinName() const {
        return "コイン(グループ配置)";
    }

    void killCoinAll();
    void appearCoinAll();
    void appearCoinFix();
    void appearCoinAllTimer();
    void setCoinTrans(s32, const TVec3f&);
    void exeAppear();
    void exeTryStartDemo();
    void exeDemoAppear();
    void exeKill();

    /* 0x8C */ Coin** mCoinArray;
    /* 0x90 */ ActorCameraInfo* mCameraInfo;
    /* 0x94 */ u32 mCoinCount;
    /* 0x98 */ s32 mTimeLimit;
    /* 0x9C */ bool mIsPurpleCoinGroup;
};
