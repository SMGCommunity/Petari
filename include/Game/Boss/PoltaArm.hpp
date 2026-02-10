#pragma once

#include "Game/LiveActor/ModelObj.hpp"

class PoltaArm : public ModelObj {
public:
    PoltaArm(const char* pName, const char* pModelName, MtxPtr pMtx);

    virtual void init(const JMapInfoIter& rIter);
    virtual void appear();
    virtual void makeActorDead();
    virtual void control();

    void initPartsModel();
    void exeDamage();
    void exeBreak();
    void exeBreakBody();
    void exeRepair();
    void endRepair();
    void exeBroken();
    void exeControlled();
    void exeWaitDamageEnd();
    void exeWaitRepairEnd();
    bool isEnableHitSensor() const;
    bool isEnableAttack() const;
    bool isBroken() const;
    void start();
    bool requestBreakBody();
    bool requestDamage();
    bool requestStartControll();
    bool requestControlled(const char*);
    void appearBreakModel();

    /* 0x90 */ ModelObj* mFormationModel;
    /* 0x94 */ ModelObj* mBreakModel;
    TPos3f _98;
    s32 _C8;
    s32 _CC;
    /* 0xD0*/ s32 mBrokenCounter;
    s32 _D4;
    /* 0xD8 */ bool mIsLeftArm;
};
