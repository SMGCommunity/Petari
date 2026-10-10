#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class ItemBlock : public LiveActor {
public:
    ItemBlock(const char*);

    virtual void init(const JMapInfoIter&);
    virtual void calcAnim();
    virtual void appear();
    virtual void kill();
    virtual void startClipped();
    virtual void endClipped();
    virtual void control();
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);

    void checkKind(const JMapInfoIter&);
    void initBlock();
    bool tryStartJumpPunch();
    void exeStandby();
    void exeWait();
    void exeAppearItem();
    void exeAppearItemSplash();
    void decTimer();

    /* 0x8C */ s32 mKind;
    /* 0x90 */ s32 mTimer;
    /* 0x94 */ s32 mItemCount;
    /* 0x98 */ u8 _98;
    /* 0x99 */ bool mCalcShadow;
    /* 0x9A */ bool mNoCalcAnim;
};
