#pragma once

#include "Game/Map/CollisionParts.hpp"
#include "Game/NPC/NPCActor.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/NPCUtil.hpp"
#include "revolution/types.h"

class HoneyQueen : public NPCActor {
public:
    HoneyQueen(const char*);

    virtual ~HoneyQueen();
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
    inline void exeReady();
    inline void exeDemo();
    inline void exeFade();
    void exeTalk();
    void exeItch();
    void exeEvent();
    inline void exeAfter();

    ModelObj* mNpcModel;
    CollisionParts* mCenterPart;
    CollisionParts* mCenterFurPart;
    CollisionParts* mFacePart;
    CollisionParts* mLArm01Part;
    CollisionParts* mLArm02Part;
    CollisionParts* mLFoot01Part;
    CollisionParts* mRArm01Part;
    CollisionParts* mRArm02Part;
    CollisionParts* mRFoot01Part;
    CollisionParts* mTactilePart;
    s32 _188;
};
