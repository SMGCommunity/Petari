#pragma once

#include "Game/LiveActor/LiveActor.h"
#include "Game/LiveActor/FlashingCtrl.h"
#include "Game/LiveActor/PartsModel.h"
#include "Game/NameObj/NameObjArchiveListCollector.h"

class CoinHostInfo {
public:
    CoinHostInfo() {
        _0 = 0;
        _4 = 0;
        _8 = 0;
        _C = 0;
    }

    u32 _0;
    u32 _4;
    u32 _8;
    u32 _C;
};

class Coin : public LiveActor {
public:
    Coin(const char *);

    virtual ~Coin();
    virtual void init(const JMapInfoIter &);
    virtual void initAfterPlacement();
    virtual void appear();
    virtual void makeActorAppeared();
    virtual void makeActorDead();
    virtual void calcAndSetBaseMtx();
    virtual bool receiveOtherMsg(u32, HitSensor *, HitSensor *);

    void initShadow(const JMapInfoIter &);
    void setShadowAndPoseModeFromJMapIter(const JMapInfoIter &);
    void exeNonActive();
    void exeFix();
    void exeFixTimer();
    void exeMove();
    void exeControled();
    void exeSpinDrained();
    void exeHop();
    void appearFixInit();
    void appearFix();
    void appearControlPose();
    void appearNonActive();
    void appearFixTimer(s32, s32);
    void appearMove(const TVec3f &, const TVec3f &, s32, s32);
    void appearHop(const TVec3f &, const TVec3f &);
    bool requestActive();
    bool requestActiveWithGravity();
    bool requestDeactive();
    bool requestStartControl();
    bool requestEndControl();
    bool requestSpinDrain();
    bool requestShow();
    bool requestHide();
    void noticeGetCoin();
    void setLife(s32);
    void setHostInfo(CoinHostInfo *);
    void setCalcShadowMode();
    void setCannotTime(s32);
    void calcRebouond();
    void attenuateVelocity();
    static bool isNeedBubble(const JMapInfoIter &);
    static void makeArchiveList(NameObjArchiveListCollector *, const JMapInfoIter &);

    CoinHostInfo* mHostInfo;    // _8C
    FlashingCtrl* mFlashCtrl;   // _90
    PartsModel* mAirBubble;    // _94
    TVec3f mDropPosition;       // _98
    TVec3f mClippingRange;      // _A4
    int _B0;
    s32 mCannotTime;            // _B4
    bool mIsInWater;            // _B8
    bool _B9;                   // obj_arg3
    bool _BA;                   // obj_arg4
    bool _BB;
    bool mIsPurpleCoin;         // _BC
    bool mIsNeedBubble;         // _BD
};

namespace MR {
    Coin* createDirectSetCoin(const char *);
    Coin* createCoin(const NameObj *, const char *);
    Coin* createDirectSetPurpleCoin(const char *);
    Coin* createPurpleCoin(const NameObj *, const char *);
};

namespace NrvCoin {
    NERVE(CoinNrvNonActive);
    NERVE(CoinNrvFix);
    NERVE(CoinNrvFixHide);
    NERVE(CoinNrvFixTimer);
    NERVE(CoinNrvControled);
    NERVE(CoinNrvSpinDrained);
    NERVE(CoinNrvMove);
    NERVE(CoinNrvHop);
};