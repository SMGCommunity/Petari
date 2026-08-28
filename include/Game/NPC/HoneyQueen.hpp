#pragma once

#include "Game/NPC/NPCActor.hpp"

class CollisionParts;
class ModelObj;

class HoneyQueen : public NPCActor {
public:
    /// @brief Creates a new `HoneyQueen`.
    /// @param pName A pointer to the null-terminated name of the object.
    HoneyQueen(const char* pName);

    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x1C */ virtual void calcAnim();
    /* 0x48 */ virtual void control();
    /* 0x54 */ virtual void attackSensor(HitSensor*, HitSensor*);
    /* 0x5C */ virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);

    void fadeOut();
    void fadeIn();
    void talkEntry();
    void switchFunc();

    void exeWait();
    void exeReady();
    void exeDemo();
    void exeFade();
    void exeTalk();
    void exeItch();
    void exeEvent();
    void exeAfter();

    /* 0x15C */ ModelObj* mWing;
    /* 0x160 */ CollisionParts* mCollisionParts[10];
    /* 0x188 */ s32 _188;
};
