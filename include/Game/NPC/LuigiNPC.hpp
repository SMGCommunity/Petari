#pragma once

#include "Game/NPC/NPCActor.hpp"

class TakeOutStar;

class LuigiNPC : public NPCActor {
public:
    LuigiNPC(const char*);

    virtual ~LuigiNPC();
    virtual void init(const JMapInfoIter&);
    virtual void initAfterPlacement();
    virtual void control();
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    static void makeArchiveList(NameObjArchiveListCollector*, const JMapInfoIter&);
    bool branchFunc(u32);
    bool eventFunc(u32);
    bool trySetNerveAfraid();
    void setWaitAction();
    void setAfraidAction();
    bool trySetNerveArrested();
    bool isDeclarePowerStarType();
    void exeWait();
    void exeReaction();
    void exeTakeOutStar();
    void exeAfraidWait();
    void exeAfraidSquat();
    void exeAfraidRise();
    void exeArrestedWait();
    void exeArrestedJump();
    void exeOnTreeWait();
    void exeOnTreeTouch();
    void exeOnTreeFall();
    void exeOnTreeLand();

    TakeOutStar* mTakeOutStar;
    s32 _160;
};
