#pragma once

#include "Game/NPC/NPCActor.hpp"
#include "Game/Util/Color.hpp"
#include "Game/Util/NPCUtil.hpp"

class TicoDemoGetPower;

class Tico : public NPCActor {
public:
    Tico(const char*);

    /* 0x08 */ virtual ~Tico();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x48 */ virtual void control();
    /* 0x4C */ virtual void calcAndSetBaseMtx();
    /* 0x5C */ virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);

    static void makeArchiveList(NameObjArchiveListCollector*, const JMapInfoIter&);

    void initBase(s32);
    void initBase(const JMapInfoIter&, s32);
    void initMessage(const JMapInfoIter&, const char*);
    void initMessage(const char*);
    bool tryReaction();
    void setNerveMeta();
    void setNerveWait();
    bool killFunc(u32);
    void exeReaction();
    void exeDelight();
    void exeAppear();
    void exeNoReaction();
    void exeWait();
    void exeMeta();
    void exeBlue0();
    void exeBlue1();
    void exeRed0();
    void exeRed1();
    void exeRed2();
    void exeGuide0();
    void exeGuide1();
    void exeGuide2();
    void exeGuide3();
    void exeLead0();
    inline void exeTalk();
    inline void exeSpin0();

    u32 _15C;
    TVec3f _160;
    f32 _16C;
    TicoDemoGetPower* mDemoGetPower;  // 0x170
    TalkMessageCtrl* _174;
    MtxPtr _178;
    Color8 _17C;
    const Nerve* _180;
    DemoStarter mDemoStarter;  // 0x184
};
