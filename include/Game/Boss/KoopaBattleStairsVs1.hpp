#pragma once

#include "Game/Boss/KoopaBattleStairsBase.hpp"

class KoopaFireStairs;

class KoopaBattleStairsVs1 : public KoopaBattleStairsBase {
public:
    KoopaBattleStairsVs1(Koopa* pKoopa);

    virtual ~KoopaBattleStairsVs1();
    virtual s32 registerStair(KoopaBattleMapStair* pStair);

    void exeWaitDemo();
    void exeDemo();
    void exeWait();
    void exeAttackFire();
    void exeJumpToNextPosStart();
    void exeJumpToNextPosLoop();
    void exeJumpToNextPosEnd();

    bool tryAttackRequest();
    bool tryRequestedToMoveNextPos();

    /* 0x10 */ KoopaBattleMapStair* mFarBattleMapStair;
    /* 0x14 */ KoopaBattleMapStair* mNearBattleMapStair;
    /* 0x18 */ KoopaFireStairs* mFarFireStairs;
    /* 0x1C */ KoopaFireStairs* mNearFireStairs;
    /* 0x20 */ KoopaBattleMapStair* _20;
    /* 0x24 */ bool mCanJump;
    /* 0x28 */ TVec3f mJumpPos0;
    /* 0x34 */ TVec3f mJumpPos1;
    /* 0x40 */ TVec3f mJumpPos2;
    /* 0x4C */ s32 _4C;
    /* 0x50 */ s32 _50;
    /* 0x54 */ s32 _54;
    /* 0x58 */ s32 mAvailableStairs;
    /* 0x5C */ s32 mJumpIdx;
    /* 0x60 */ TVec3f mOldPosition;
    /* 0x6C */ TVec3f mNewPosition;
    /* 0x78 */ TVec3f mNewDirection;
};
