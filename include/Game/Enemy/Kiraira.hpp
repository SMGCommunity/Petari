#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class KirairaChain;
class MsgSharedGroup;

class Kiraira : public LiveActor {
public:
    Kiraira(const char*);

    /* 0x0C */ virtual void init(const JMapInfoIter& rIter);
    /* 0x4C */ virtual void calcAndSetBaseMtx();
    /* 0x54 */ virtual void attackSensor(HitSensor*, HitSensor*);
    /* 0x5C */ virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    /* 0x60 */ virtual bool receiveMsgEnemyAttack(u32, HitSensor*, HitSensor*);
    /* 0x74 */ virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void openEyes();
    void closeEyes();
    void drift();
    void driftOnRail();
    void explode();

    bool isDown() const;

    void exeWait();
    void exeFaceToMario();
    void exeFaceToMarioAndStare();
    void exeExplode();
    void exeBeExploded();
    void exeDead();
    void exeRecoverSign();
    void exeRecover();

private:
    /* 0x8C */ f32 mRailCoordSpeed;
    /* 0x90 */ TVec3f mRailVec;
    /* 0x9C */ TVec3f mFront;
    /* 0xA8 */ f32 _A8;
    /* 0xAC */ bool mIsRail;
    /* 0xAD */ bool mIsForceDetonated;
    /* 0xAE */ bool mEyesOpen;
    /* 0xB0 */ MsgSharedGroup* mSharedGroup;
    /* 0xB4 */ KirairaChain* mChain;
};
