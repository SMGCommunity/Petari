#pragma once

#include "Game/Boss/KoopaSequencer.hpp"

class KoopaSequencerVs2 : public KoopaSequencer {
public:
    KoopaSequencerVs2();

    virtual ~KoopaSequencerVs2();
    virtual void init(Koopa*, const JMapInfoIter&);
    virtual bool attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveMsgEnemyAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

private:
    /* 0x24 */ u32 _24;
    /* 0x28 */ u32 _28;
};
