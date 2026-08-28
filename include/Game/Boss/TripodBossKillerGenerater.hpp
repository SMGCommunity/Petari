#pragma once

#include "Game/Boss/TripodBossFixPartsBase.hpp"

class HomingKiller;

class TripodBossKillerGenerater : public TripodBossFixPartsBase {
public:
    TripodBossKillerGenerater(const char* pName = "三脚ボスキラー発生源");

    /* 0x08 */ virtual ~TripodBossKillerGenerater();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x2C */ virtual void kill();
    /* 0x60 */ virtual bool receiveMsgEnemyAttack(u32, HitSensor*, HitSensor*);

    /* 0x78 */ virtual void calcTripodLocalMatrix(TPos3f*);
    /* 0x7C */ virtual void activateTripodBoss();

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
