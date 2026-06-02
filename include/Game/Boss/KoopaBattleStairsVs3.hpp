#pragma once

#include "Game/Boss/KoopaBattleStairsBase.hpp"

class KoopaBattleStairsVs3 : public KoopaBattleStairsBase {
public:
    KoopaBattleStairsVs3(Koopa*);

    virtual ~KoopaBattleStairsVs3();
    virtual void registerStair(KoopaBattleMapStair*);

    void exeWaitDemo();
    void exeDemo();
    void exeWait();
    void tryAttack();

    /* 0x10 */ TVec3f mNamePos;
};
