#pragma once

#include "Game/NPC/NPCActor.hpp"

class ActorCameraInfo;
class BombTimerLayout;
class MsgSharedGroup;
class TakeOutStar;

class Caretaker : public NPCActor {
public:
    /// @brief Creates a new `Caretaker`.
    /// @param pName A pointer to the null-terminated name of the object.
    Caretaker(const char* pName);

    virtual void init(const JMapInfoIter& rIter);
    virtual void initAfterPlacement();
    virtual void kill();
    virtual void control();
    virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    static void makeArchiveList(NameObjArchiveListCollector*, const JMapInfoIter&);

    bool branchFuncStar(u32);
    bool branchFuncComet(u32);
    bool eventFuncComet(u32);
    bool animeFunc(u32);

    void setAnim(s32);
    void updateCounterSE();

    void exeTakeOutStar();
    void exeReaction();
    void exeTalk();
    void exeWait();
    void exePreTalk();
    void exePreWipeOut();
    void exePreWipeIn();
    void exePreWait();
    void exeCountDown();
    void exeTidy();
    void exeRunaway();
    void exePstWipeOut();
    void exePstWipeIn();
    void exePstWait();
    void exePstTalk();
    void exeAppearedStar();

private:
    /* 0x15C */ TakeOutStar* mTakeOutStar;
    /* 0x160 */ ActorCameraInfo* mCameraInfo;
    /* 0x164 */ BombTimerLayout* mBombTimerLayout;
    /* 0x168 */ MsgSharedGroup* _168;
    /* 0x16C */ s32 mObjArg0;
    /* 0x170 */ s32 mObjArg4;
    /* 0x174 */ s32 mBodyColorFrame;
    /* 0x178 */ s32 mTidyTimeLimit;
    /* 0x17C */ s32 _17C;
};
