#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class KoopaBattleMapStair;
class ModelObj;

class KoopaFireStairs : public LiveActor {
public:
    KoopaFireStairs(const char* pName, bool);

    virtual ~KoopaFireStairs();
    virtual void init(const JMapInfoIter& rIter);
    virtual void appear();
    virtual void makeActorDead();
    virtual void calcAndSetBaseMtx();

    void setInfo(const KoopaBattleMapStair*, const TVec3f*);
    void exeFly();
    void exeBreak();

    /* 0x8C */ bool mIsKoopaJr;
    /* 0x90 */ const KoopaBattleMapStair* mStair;
    /* 0x94 */ TVec3f _94;
    /* 0xA0 */ TVec3f _A0;
    /* 0xAC */ TVec3f _AC;
    /* 0xB8 */ ModelObj* mBreakModel;
};
