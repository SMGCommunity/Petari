#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class FixedPosition;

class PartsModel : public LiveActor {
public:
    PartsModel(LiveActor*, const char*, const char*, MtxPtr, int, bool);

    /* 0x0C */ virtual void init(const JMapInfoIter&) override;
    /* 0x14 */ virtual void movement() override;
    /* 0x1C */ virtual void calcAnim() override;
    /* 0x20 */ virtual void calcViewAndEntry() override;
    /* 0x28 */ virtual void makeActorAppeared() override;
    /* 0x30 */ virtual void makeActorDead() override;
    /* 0x4C */ virtual void calcAndSetBaseMtx() override;

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
