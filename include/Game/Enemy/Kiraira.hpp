#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class KirairaChain;
class MsgSharedGroup;

class Kiraira : public LiveActor {
public:
    Kiraira(const char*);

    virtual void init(const JMapInfoIter& rIter);
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveMsgEnemyAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

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
