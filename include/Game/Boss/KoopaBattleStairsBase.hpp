#pragma once

#include "Game/System/NerveExecutor.hpp"
#include "JSystem/JGeometry/TVec.hpp"

class Koopa;
class HitSensor;
class KoopaBattleMapStair;
class LiveActorGroup;

class KoopaBattleStairsBase : public NerveExecutor {
public:
    KoopaBattleStairsBase(Koopa* pKoopa);

    virtual ~KoopaBattleStairsBase() {};

    void calcFireStartPos(TVec3f* pVec, const char* pDemoName) const;
    void update();
    void end();
    bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    s32 calcFireAttackStep(KoopaBattleMapStair* pBattleMapStair, f32, s32, const TVec3f& rVec) const;

    /* 0x8 */ Koopa* mKoopa;
    /* 0xC */ LiveActorGroup* mStairsGroup;
};
