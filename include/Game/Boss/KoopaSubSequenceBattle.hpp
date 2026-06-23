#pragma once

#include "Game/LiveActor/ActorStateBase.hpp"

class Koopa;
class KoopaBattleBase;
class KoopaSequencer;

class KoopaSubSequenceBattle : public ActorStateBase< Koopa > {
public:
    KoopaSubSequenceBattle(Koopa* pKoopa, KoopaSequencer* pSequencer, KoopaBattleBase* pBattleBase);

    virtual ~KoopaSubSequenceBattle();
    virtual void kill();

    bool isDemo() const;

    void exeBattleLv1();
    void exeDemoStartLv2();
    void exeBattleLv2();
    void exeDemoStartLv3();
    void exeBattleLv3();

private:
    /* 0x10 */ KoopaSequencer* mSequencer;
    /* 0x14 */ KoopaBattleBase* mBattleBase;
};
