#pragma once

#include "Game/LiveActor/ModelObj.hpp"
#include "Game/NameObj/NameObj.hpp"

class SkeletalFishBoss;
class SubmarineVolcanoBigColumn;

class SkeletalFishBossBattleDirector : public NameObj {
public:
    SkeletalFishBossBattleDirector(SkeletalFishBoss*);

    virtual ~SkeletalFishBossBattleDirector();
    virtual void movement();

    void initiate();
    void startPowerUpDemo1();
    void startPowerUpDemo2();
    void playGuardAnim(const char*, s32);
    void tryColumnCollision(HitSensor*);
    void pauseOffCast();
    void endPowerUpDemo1();
    void endPowerUpDemo2();
    void killGuard();
    void appearBirdLouse();
    void killBirdLouse();

    SkeletalFishBoss* mFishBoss;                // 0xC
    SubmarineVolcanoBigColumn* mColumns[0x20];  // 0x10
    s32 _90;
    LiveActor* _94[0x10];
    s32 _D4;
    ModelObj* mGuardModels[4];  // 0xD8
};