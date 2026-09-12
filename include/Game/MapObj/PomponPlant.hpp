#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class PomponPlant : public LiveActor {
public:
    PomponPlant(const char*);

    virtual ~PomponPlant();
    virtual void init(const JMapInfoIter&) override;
    virtual void attackSensor(HitSensor*, HitSensor*) override;
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*) override;

    bool tryGenItem();
    void exeWait();
    void exeSwingDpd();
    void exeSwingPlayer();

    /* 0x8C */ s32 mItemType;
    /* 0x90 */ s32 mItemCount;
    /* 0x94 */ bool mCanGenItem;
};
