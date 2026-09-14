#pragma once

#include "Game/LiveActor/ModelObj.hpp"

class SmallStoneMember;

class SmallStone : public LiveActor {
public:
    SmallStone(const char* pName);

    virtual ~SmallStone();
    virtual void init(const JMapInfoIter& rIter);
    virtual void initAfterPlacement();
    virtual void control();
    virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual void startClipped();
    virtual void endClipped();

    bool isAllMemberBreak();
    bool tryBreak();
    void initMember(const char* pModelName, bool useGravity);

    /* 0x8C */ SmallStoneMember** mMembers;
    /* 0x90 */ s32 mMemberCount;
    /* 0x94 */ f32 mPlacementOffset;
    /* 0x98 */ s32 mStoneType;
    /* 0x9C */ bool mUseGravity;
    /* 0x9D */ bool mIsStarPiece;
};

class SmallStoneMember : public ModelObj {
public:
    SmallStoneMember(const char* pModelName);

    virtual ~SmallStoneMember();

    bool tryShake();
    bool tryPush(HitSensor* pSender, HitSensor* pReceiver);
    void movementByHost(SmallStone* pHost);
    void animControl();
    void doBreak(SmallStone* pHost);

    /* 0x90 */ f32 mAnimRate;
    /* 0x94 */ u8 mShakeRequested;
    /* 0x95 */ bool mAnimStopped;
    /* 0x98 */ s32 mBreakTimer;
    /* 0x9C */ const char* mBreakEffectName;
    /* 0xA0 */ bool mUseGravity;
};
