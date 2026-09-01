#pragma once

#include "Game/NPC/NPCActor.hpp"
#include "Game/Util/Color.hpp"
#include "Game/Util/NPCUtil.hpp"

class TicoDemoGetPower;

/// @brief Luma
class Tico : public NPCActor {
public:
    /// @brief Creates a new `Tico`.
    /// @param pName A pointer to the null-terminated name of the object.
    Tico(const char* pName);

    virtual void init(const JMapInfoIter& rIter);
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

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
    void exeTalk();
    void exeMeta();
    void exeBlue0();
    void exeBlue1();
    void exeRed0();
    void exeRed1();
    void exeRed2();
    void exeSpin0();
    void exeGuide0();
    void exeGuide1();
    void exeGuide2();
    void exeGuide3();
    void exeLead0();

    static f32 sFloatSeMinVolume;

    /* 0x15C */ u32 _15C;
    /* 0x160 */ TVec3f _160;
    /* 0x16C */ f32 _16C;
    /* 0x170 */ TicoDemoGetPower* mDemoGetPower;
    /* 0x174 */ TalkMessageCtrl* _174;
    /* 0x178 */ MtxPtr _178;
    /* 0x17C */ Color8 _17C;
    /* 0x180 */ const Nerve* _180;
    /* 0x184 */ DemoStarter mDemoStarter;
};
