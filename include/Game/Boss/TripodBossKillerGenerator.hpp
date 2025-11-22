#pragma once

#include "Game/Boss/TripodBossFixPartsBase.hpp"
#include "Game/LiveActor/HitSensor.hpp"

class HomingKiller;

class TripodBossKillerGenerater : public TripodBossFixPartsBase {
public:
    TripodBossKillerGenerater(const char* pName = "三脚ボスキラー発生源");

    virtual ~TripodBossKillerGenerater();
    virtual void init(const JMapInfoIter&);
    virtual void kill();
    virtual bool receiveMsgEnemyAttack(u32, HitSensor*, HitSensor*);
    virtual void calcTripodLocalMatrix(TPos3f*);
    virtual void activateTripodBoss();

    void setLocalMatrix(const TPos3f&);
    void setActiveLebel(s32);
    void setHasCollision(bool);
    bool tryShootSetting();
    bool tryShoot();
    bool tryCoolDown();
    bool tryReady();
    bool tryBreak();
    bool tryAbort();
    bool requestBreak();
    void exeShowDemo();
    void exeReady();
    void exeShootSetting();
    void exeShoot();
    void exeCoolDown();
    void exeStop();
    void exeRestart();
    void exeBreak();
    inline void exeHide();
    inline void exeNonActive();

    TPos3f _E4;
    HomingKiller* mKiller;  // 0x114
    f32 _118;
    s32 mActiveLabel;    // 0x11C
    bool mHasCollision;  // 0x120
    u8 _121;
};
