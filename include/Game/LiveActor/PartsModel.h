#pragma once

#include "Game/LiveActor/LiveActor.h"
#include "Game/Util.h"

class PartsModel : public LiveActor {
public:
    PartsModel(LiveActor *, const char *, const char *, MtxPtr, int, bool);

    virtual ~PartsModel() {}
    virtual void init(const JMapInfoIter &);
    virtual void movement();
    virtual void calcAnim();
    virtual void calcViewAndEntry();
    virtual void makeActorAppeared();
    virtual void makeActorDead();
    virtual void calcAndSetBaseMtx();

    void initFixedPosition(const TVec3f &, const TVec3f &, const char *);
    void initFixedPosition(const char *);
    void initFixedPosition(MtxPtr, const TVec3f &, const TVec3f &);
    void loadFixedPosition(const char *);
    void offFixedPosNormalizeScale();

    LiveActor* mHost;           // _8C
    FixedPosition* mFixedPos;   // _90
    MtxPtr mMtx;                // _94
    bool mCalcOwnMtx;           // _98
    bool _99;
    bool mIsDead;               // _9A
    bool _9B;
};