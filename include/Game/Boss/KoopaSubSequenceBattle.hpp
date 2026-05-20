#pragma once

#include "Game/Boss/Koopa.hpp"
#include "Game/LiveActor/ActorStateBase.hpp"

class KoopaSequencer;
class KoopaBattleBase;

class KoopaSubSequenceBattle : public ActorStateBase< Koopa > {
public:
    KoopaSubSequenceBattle(Koopa*, KoopaSequencer*, KoopaBattleBase*);
    
    virtual ~KoopaSubSequenceBattle();
    virtual void kill();

    bool isDemo();

    void exeBattleLv1();
    void exeDemoStartLv2();
    void exeBattleLv2();
    void exeDemoStartLv3();
    void exeBattleLv3();

private:
    /* 0x10 */ KoopaSequencer* mSequencer;
    /* 0x14 */ KoopaBattleBase* mBattleBase;
};
