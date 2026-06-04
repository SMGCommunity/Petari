#pragma once

#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/ModelObj.hpp"

class SmallStoneMember;
class SmallStone : public LiveActor {
public:
    SmallStone(const char*);

    virtual void init(const JMapInfoIter&);
    virtual void initAfterPlacement();
    virtual void startClipped();
    virtual void endClipped();
    virtual void control();
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveMsgPlayerAttack(unsigned long, HitSensor*, HitSensor*);
    virtual bool receiveOtherMsg(unsigned long, HitSensor*, HitSensor*);

    void initMember(const char*, bool);

    /* 0x8C */ SmallStoneMember** mArray;
    /* 0x90*/ u32 mArrayCount;
    /* 0x94 */ f32 _94;
    /* 0x98 */ s32 mMemberType;
    /* 0x9C */ bool _9C;
    /* 0x9D */ bool _9D;
};

class SmallStoneMember : public ModelObj {
public:
    SmallStoneMember(const char*);

    void movementByHost(SmallStone*);
    void animControl();
    void doBreak(SmallStone*);

    /* 0x90 */ f32 _90;
    /* 0x94 */ bool _94;
    /* 0x95 */ bool _95;
    /* 0x98 */ s32 _98;
    /* 0x9C */ const char* mBreakStr;
    /* 0xA0 */ bool _A0;  
};
