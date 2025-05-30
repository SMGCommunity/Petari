#pragma once

#include "Game/Map/CollisionParts.hpp"
#include "Game/NPC/NPCActor.hpp"
#include "Game/NameObj/NameObjArchiveListCollector.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/NPCUtil.hpp"
#include "revolution/types.h"

class LuigiNPC : public NPCActor {
public:
    LuigiNPC(const char *);

    virtual ~LuigiNPC();
    virtual void init(const JMapInfoIter &);
    virtual void initAfterPlacement();
    virtual void control();
    virtual void attackSensor(HitSensor *, HitSensor *);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor *, HitSensor *);
    virtual bool receiveOtherMsg(u32, HitSensor *, HitSensor *);

    void makeArchiveList(NameObjArchiveListCollector *, const JMapInfoIter &);
    void branchFunc(u32);
    void eventFunc(u32);
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
    void exeOnTreeFall();
    void exeOnTreeLand();

    TakeOutStar* _15C;
    s32 _160;   
};
