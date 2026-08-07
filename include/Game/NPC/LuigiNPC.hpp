#pragma once

#include "Game/NPC/NPCActor.hpp"

class TakeOutStar;

class LuigiNPC : public NPCActor {
public:
    enum Type {
        Type_Invalid = -1,
        Type_Afraid,
        Type_Normal,
        Type_Event,
        Type_OnTree,
        Type_Arrested,
        Type_Count,
    };

    /// @brief Creates a new `LuigiNPC`.
    /// @param pName A pointer to the null-terminated name of the object.
    LuigiNPC(const char* pName);

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

    /* 0x15C */ TakeOutStar* mTakeOutStar;
    /* 0x160 */ s32 mType;
};
