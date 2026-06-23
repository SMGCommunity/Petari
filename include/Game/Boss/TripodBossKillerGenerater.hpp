#pragma once

#include "Game/Boss/TripodBossFixPartsBase.hpp"

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

    void exeNonActive();
    void exeHide();
    void exeShowDemo();
    void exeReady();
    void exeShootSetting();
    void exeShoot();
    void exeCoolDown();
    void exeStop();
    void exeRestart();
    void exeBreak();

    /* 0x0E4 */ TPos3f _E4;
    /* 0x114 */ HomingKiller* mKiller;
    /* 0x118 */ f32 _118;
    /* 0x11C */ s32 mActiveLabel;
    /* 0x120 */ bool mHasCollision;
    /* 0x121 */ u8 _121;
};
