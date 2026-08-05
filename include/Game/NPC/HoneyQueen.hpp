#pragma once

#include "Game/NPC/NPCActor.hpp"

class CollisionParts;
class ModelObj;

class HoneyQueen : public NPCActor {
public:
    /// @brief Creates a new `HoneyQueen`.
    /// @param pName A pointer to the null-terminated name of the object.
    HoneyQueen(const char* pName);

    virtual void init(const JMapInfoIter&);
    virtual void calcAnim();
    virtual void control();
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);

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
