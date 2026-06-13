#pragma once

#include "Game/Boss/KoopaBattleStairsBase.hpp"

class KoopaBattleStairsVs3 : public KoopaBattleStairsBase {
public:
    KoopaBattleStairsVs3(Koopa* pKoopa);

    virtual ~KoopaBattleStairsVs3();
    virtual s32 registerStair(KoopaBattleMapStair* pStair);

    void exeWaitDemo();
    void exeDemo();
    void exeWait();
    void tryAttack();

    /* 0x10 */ TVec3f mNamePos;
};
