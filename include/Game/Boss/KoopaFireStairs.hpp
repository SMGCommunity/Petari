#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/ModelObj.hpp"

class KoopaBattleMapStair;

class KoopaFireStairs : public LiveActor {
public:
    KoopaFireStairs(const char*, bool);

    virtual ~KoopaFireStairs();
    virtual void init(const JMapInfoIter&);
    virtual void appear();
    virtual void makeActorDead();
    virtual void calcAndSetBaseMtx();

    void setInfo(const KoopaBattleMapStair*, const TVec3f*);
    void exeFly();
    void exeBreak();

    bool _8C;
    const KoopaBattleMapStair* mStair;  // 0x90
    TVec3f _94;
    TVec3f _A0;
    TVec3f _AC;
    ModelObj* mBreakModel;  // 0xB8
};
