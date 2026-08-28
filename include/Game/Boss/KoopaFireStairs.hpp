#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class KoopaBattleMapStair;
class ModelObj;

class KoopaFireStairs : public LiveActor {
public:
    KoopaFireStairs(const char* pName, bool);

    /* 0x08 */ virtual ~KoopaFireStairs();
    /* 0x0C */ virtual void init(const JMapInfoIter& rIter);
    /* 0x24 */ virtual void appear();
    /* 0x30 */ virtual void makeActorDead();
    /* 0x4C */ virtual void calcAndSetBaseMtx();

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
