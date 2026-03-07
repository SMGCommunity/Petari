#pragma once

#include "Game/LiveActor/FlashingCtrl.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/PartsModel.hpp"
#include "Game/NameObj/NameObjArchiveListCollector.hpp"

class CoinHostInfo {
public:
    CoinHostInfo() {
        mHostActor = nullptr;
        _4 = 0;
        _8 = 0;
        _C = 0;
    }

    const NameObj* mHostActor;  // 0x0
    s32 _4;
    s32 _8;
    u32 _C;
};

class Coin : public LiveActor {
public:
    Coin(const char*);

    virtual ~Coin();
    virtual void init(const JMapInfoIter&);
    virtual void initAfterPlacement();
    virtual void appear();
    virtual void makeActorAppeared();
    virtual void makeActorDead();
    virtual void calcAndSetBaseMtx();
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void initShadow(const JMapInfoIter&);
    void setShadowAndPoseModeFromJMapIter(const JMapInfoIter&);
    void exeNonActive();
    void exeFix();
    void exeFixHide();
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
    void appearMove(const TVec3f&, const TVec3f&, s32, s32);
    void appearHop(const TVec3f&, const TVec3f&);
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
    void setHostInfo(CoinHostInfo*);
    void setCalcShadowMode();
    void setCannotTime(s32);
    void calcRebouond();
    void attenuateVelocity();
    static bool isNeedBubble(const JMapInfoIter&);
    static void makeArchiveList(NameObjArchiveListCollector*, const JMapInfoIter&);

    CoinHostInfo* mHostInfo;   // 0x8C
    FlashingCtrl* mFlashCtrl;  // 0x90
    PartsModel* mAirBubble;    // 0x94
    TVec3f mDropPosition;      // 0x98
    TVec3f mClippingRange;     // 0xA4
    int _B0;
    s32 mCannotTime;  // 0xB4
    bool mIsInWater;  // 0xB8
    bool _B9;         // obj_arg3
    bool _BA;         // obj_arg4
    bool _BB;
    bool mIsPurpleCoin;  // 0xBC
    bool mIsNeedBubble;  // 0xBD
};

namespace MR {
    NameObj* createDirectSetCoin(const char*);
    NameObj* createCoin(const NameObj*, const char*);
    NameObj* createDirectSetPurpleCoin(const char*);
    NameObj* createPurpleCoin(const NameObj*, const char*);
};  // namespace MR
