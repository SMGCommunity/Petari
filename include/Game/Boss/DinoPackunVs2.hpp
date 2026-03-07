#pragma once

#include "Game/Boss/DinoPackunBattleEggVs2.hpp"
#include "Game/Boss/DinoPackunBattleVs2Lv1.hpp"
#include "Game/Boss/DinoPackunDemo.hpp"
#include "Game/Boss/DinoPackunFire.hpp"
#include "Game/Boss/DinoPackunSequencer.hpp"
#include "Game/Boss/DinoPackunTrackFire.hpp"

class DinoPackunVs2 : public DinoPackunSequencer {
public:
    DinoPackunVs2(DinoPackun*);

    virtual ~DinoPackunVs2();
    virtual void start();
    virtual void init();
    virtual bool isUseEggShell() const;
    virtual s32 getVsCount() const;

    void exeWaitStart();
    void exeOpeningDemo();
    void exeBattleEgg();
    void exeCryDemo();
    void exeBattleLv1();
    void exeAngryDemo();
    void exeBattleLv2();
    void exeBattleLv3();
    void exeDownDemo();

    DinoPackunDemo* mDemo;                        // 0x10
    DinoPackunBattleEggVs2* mEgg;                 // 0x14
    DinoPackunBattleVs2Lv1* mBattleLv1;           // 0x18
    DinoPackunTrackFireHolder* mTrackFireHolder;  // 0x1C
    DinoPackunFireHolder* mFireHolder;            // 0x20
};
