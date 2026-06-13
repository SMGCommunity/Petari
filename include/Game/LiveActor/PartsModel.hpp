#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class FixedPosition;

class PartsModel : public LiveActor {
public:
    PartsModel(LiveActor*, const char*, const char*, MtxPtr, int, bool);

    virtual void init(const JMapInfoIter&) override;
    virtual void movement() override;
    virtual void calcAnim() override;
    virtual void calcViewAndEntry() override;
    virtual void makeActorAppeared() override;
    virtual void makeActorDead() override;
    virtual void calcAndSetBaseMtx() override;

    void initFixedPosition(const TVec3f&, const TVec3f&, const char*);
    void initFixedPosition(const char*);
    void initFixedPosition(MtxPtr, const TVec3f&, const TVec3f&);
    void loadFixedPosition(const char*);
    void offFixedPosNormalizeScale();

    /* 0x8C */ LiveActor* mHost;
    /* 0x90 */ FixedPosition* mFixedPos;
    /* 0x94 */ MtxPtr mMtx;
    /* 0x98 */ bool mCalcOwnMtx;
    /* 0x99 */ bool _99;
    /* 0x9A */ bool mIsDead;
    /* 0x9B */ bool _9B;
};
