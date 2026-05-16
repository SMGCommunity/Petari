#pragma once

#include "Game/Map/RaceManager.hpp"
#include "Game/NPC/NPCActor.hpp"

class TurnJointCtrl;
class LiveActorGroup;
class SurfRayTutorial;

class PenguinCoach : public NPCActor, public AbstractAudience {
public:
    enum Behavior { Behavior_Default = 0, Behavior_Star = 1, Behavior_Tutorial = 2, Behavior_Race = 3 };
    PenguinCoach(const char*);

    virtual void init(const JMapInfoIter& rIter);
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual void prepAudience();
    virtual void resetAudience();

    void initAfterPlacement();
    bool branchFunc(u32);
    bool eventFunc(u32);

    void exeTakeOutStar();
    void exeRace();
    void exeReaction();
    void exePrep();
    void exeWait();
    void exeFade();
    void exeTutorial();
    void exePraise();

    void setTutorialPos();
    void setFinishPos();
    void setPlayerNoSelectionPos();

    /* 0x160 */ bool mIsRaceComplete;
    /* 0x164 */ TakeOutStar* mTakeOutStar;
    /* 0x168 */ s32 mBehavior;
    /* 0x16C */ SurfRayTutorial* mTutorial;
    /* 0x170 */ LiveActor* mActor;
    /* 0x174 */ ActorCameraInfo* mCameraInfo;
    /* 0x178 */ TurnJointCtrl* mTurnJointCtrl;
};
