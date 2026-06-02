#pragma once

#include "Game/Boss/KoopaBattleStairsBase.hpp"

class KoopaFireStairs;

class KoopaBattleStairsVs1 : public KoopaBattleStairsBase {
public:
    KoopaBattleStairsVs1(Koopa*);

    virtual ~KoopaBattleStairsVs1();
    virtual void registerStair(KoopaBattleMapStair*);

    void exeWaitDemo();
    void exeDemo();
    void exeWait();
    void exeAttackFire();
    void exeJumpToNextPosStart();
    void exeJumpToNextPosLoop();
    void exeJumpToNextPosEnd();

    bool tryAttackRequest();
    bool tryRequestedToMoveNextPos();

private:
    /* 0x10 */ KoopaBattleMapStair* _10;
    /* 0x14 */ KoopaBattleMapStair* _14;
    /* 0x18 */ KoopaFireStairs* _18;
    /* 0x1C */ KoopaFireStairs* _1C;
    /* 0x20 */ KoopaBattleMapStair* _20;
    /* 0x24 */ bool _24;
    /* 0x28 */ TVec3f _28;
    /* 0x34 */ TVec3f _34;
    /* 0x40 */ TVec3f _40;
    /* 0x4C */ s32 _4C;
    /* 0x50 */ s32 _50;
    /* 0x54 */ s32 _54;
    /* 0x58 */ s32 _58;
    /* 0x5C */ s32 _5C;
    /* 0x60 */ TVec3f _60;
    /* 0x6C */ TVec3f _6C;
    /* 0x78 */ TVec3f _78;
};
