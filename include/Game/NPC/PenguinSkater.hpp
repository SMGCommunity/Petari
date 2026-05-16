#pragma once

#include "Game/NPC/NPCActor.hpp"
#include "Game/NameObj/NameObj.hpp"

class PenguinSkater : public NPCActor {
public:
    PenguinSkater(const char*);
    virtual ~PenguinSkater();

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

private:
    /* 0x15C */ ActorCameraInfo* mCameraInfo;
    /* 0x160 */ LiveActor* _160;
    /* 0x164 */ LiveActor* _164;
    /* 0x168 */ u8 _168[0x40];
    /* 0x1A8 */ LiveActor* mRail;
    /* 0x1AC */ s32 _1AC;
    /* 0x1B0*/ s32 _1B0;
    /* 0x1B4 */ TQuat4f _1B4;
    /* 0x1C4 */ TVec3f _1C4;
};
