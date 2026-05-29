#pragma once

#include "Game/System/NerveExecutor.hpp"
#include "JSystem/JGeometry/TVec.hpp"

class Koopa;
class HitSensor;
class KoopaBattleMapStair;
class LiveActorGroup;

class KoopaBattleStairsBase : public NerveExecutor {
public:
    KoopaBattleStairsBase(Koopa*);

    virtual ~KoopaBattleStairsBase();

    void calcFireStartPos(TVec3f*, const char*) const;
    void update();
    void end();
    bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    s32 calcFireAttackStep(KoopaBattleMapStair*, f32, s32, const TVec3f&) const;

private:
    /* 0x8 */ Koopa* mKoopa;
    /* 0xC */ LiveActorGroup* _C;
};
