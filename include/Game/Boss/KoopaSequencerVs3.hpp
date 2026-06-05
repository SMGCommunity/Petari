#pragma once

#include "Game/Boss/KoopaSequencer.hpp"

class KoopaSequencerVs3 : public KoopaSequencer {
public:
    KoopaSequencerVs3();
    
    virtual ~KoopaSequencerVs3();
    virtual void init(Koopa*, const JMapInfoIter&);
    virtual void calcAndSetBaseMtx();
    virtual bool attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveMsgEnemyAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

private:
    /* 0x24 */ u32 _24;
    /* 0x28 */ u32 _28;
    /* 0x2C */ u32 _2C;
    /* 0x30 */ u32 _30;
    /* 0x34 */ u32 _34;
};
