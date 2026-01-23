#pragma once

#include "Game/Boss/DinoPackunSequencer.hpp"

class DinoPackunBattleVs1Lv2;

class DinoPackunVs1 : public DinoPackunSequencer {
public:
    DinoPackunVs1(DinoPackun*);

    virtual ~DinoPackunVs1();
    virtual void start();
    virtual void init();
    virtual bool isUseEggShell() const;
    virtual u32 getVsCount() const;

    void exeOpeningDemo();
    void exeBattleEgg();
    void exeCryDemo();
    void exeBattleLv1();
    void exeAngryDemo();
    void exeBattleLv2();
    void exeBattleLv3();
    void exeDownDemo();

    DinoPackunBattleVs1Lv2* mBattleLv2;  // 0x1C
};
