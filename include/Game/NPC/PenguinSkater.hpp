#pragma once

#include "Game/NPC/NPCActor.hpp"

class ActorCameraInfo;
class NPCSupportRail;

class PenguinSkater : public NPCActor {
public:
    PenguinSkater(const char*);

    virtual void init(const JMapInfoIter& rIter);
    virtual void initAfterPlacement();
    virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    f32 calcLead() const;
    void moveRail(f32, f32);
    void stopRail(f32);
    bool inProvokeRangeIn(f32) const;
    bool inProvokeRangeOut(f32) const;
    bool inSwitchRange(f32) const;
    bool trySwitchRail();
    void endSwitchRail();
    void setCaughtStartMarioPose();
    void blendBaseMatrixToMario(f32) const;

    void exeReaction();
    void exeTalk();
    void exeWait();
    void exeDemo();
    void exeAway();
    void exeTurn();
    void exeProvoke();
    void exeCaught();
    void exeFadeOut();
    void exeFadeIn();
    void exeGiveUp();
    void exeSitDown();
    void exeSwitch();

    bool isAttackable() const;

    /* 0x15C */ ActorCameraInfo* mCameraInfo;
    /* 0x160 */ NPCSupportRail* mCurrentRail;
    /* 0x164 */ NPCSupportRail* mSwitchRail;
    /* 0x168 */ NPCSupportRail* mSwitchRails[16];
    /* 0x1A8 */ NPCSupportRail* mRail;
    /* 0x1AC */ s32 mNumRails;
    /* 0x1B0 */ s32 mCrossPoint;
    /* 0x1B4 */ TQuat4f mMarioQuat;
    /* 0x1C4 */ TVec3f mMarioPos;
};
