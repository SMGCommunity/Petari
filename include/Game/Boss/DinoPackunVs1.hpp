#pragma once

#include "Game/Boss/DinoPackunBattleEgg.hpp"
#include "Game/Boss/DinoPackunDemo.hpp"
#include "Game/Boss/DinoPackunSequencer.hpp"

class DinoPackunBattleVs1Lv1;
class DinoPackunBattleVs1Lv2;

class DinoPackunVs1 : public DinoPackunSequencer {
public:
    DinoPackunVs1(DinoPackun*);

    virtual ~DinoPackunVs1();
    virtual void start();
    virtual void init();
    virtual bool isUseEggShell() const {
        return true;
    };
    virtual s32 getVsCount() const {
        return 1;
    };

    void exeWaitStart();
    void exeOpeningDemo();
    void exeBattleEgg();
    void exeCryDemo();
    void exeBattleLv1();
    void exeAngryDemo();
    void exeBattleLv2();
    void exeBattleLv3();
    void exeDownDemo();

    /* 0x10 */ DinoPackunDemo* mDemo;
    /* 0x14 */ DinoPackunBattleEgg* mEgg;
    /* 0x18 */ DinoPackunBattleVs1Lv1* mBattleLv1;
    /* 0x1C */ DinoPackunBattleVs1Lv2* mBattleLv2;
};
