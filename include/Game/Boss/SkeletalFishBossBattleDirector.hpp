#pragma once

#include "Game/LiveActor/ModelObj.hpp"

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

    /* 0x0C */ SkeletalFishBoss* mFishBoss;
    /* 0x10 */ SubmarineVolcanoBigColumn* mColumns[0x20];
    /* 0x90 */ s32 _90;
    /* 0x94 */ LiveActor* _94[0x10];
    /* 0xD4 */ s32 _D4;
    /* 0xD8 */ ModelObj* mGuardModels[4];
};
