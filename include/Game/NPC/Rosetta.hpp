#pragma once

#include "Game/NPC/NPCActor.hpp"
#include "Game/Util/NPCUtil.hpp"

class NameObjArchiveListCollector;
class NerveExecutor;
class TurnJointCtrl;

class Rosetta : public NPCActor {
public:
    /// @brief Creates a new `Rosetta`.
    /// @param pName The pointer to the null-terminated name of the object.
    Rosetta(const char* pName);

    virtual void init(const JMapInfoIter& rIter);
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor*, HitSensor*);

    bool branchFunc(u32);
    bool eventFunc(u32);

    static void makeArchiveList(NameObjArchiveListCollector*, const JMapInfoIter&);
    void startDemo(NerveExecutor*);
    void endDemo();
    void endDemoWithInterpole();
    bool canUpdateStarePos() const;
    void exeDemo();
    void exeReaction();

private:
    /* 0x15C */ FadeStarter _15C;
    /* 0x170 */ NerveExecutor* _170;
    /* 0x174 */ NerveExecutor* _174;
    /* 0x178 */ TurnJointCtrl* _178;
    /* 0x17C */ s32 _17C;
};
