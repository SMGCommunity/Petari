#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Util.hpp"

class PartsModel : public LiveActor {
public:
    PartsModel(LiveActor*, const char*, const char*, MtxPtr, int, bool);

    virtual ~PartsModel() {}
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

    LiveActor* mHost;          // 0x8C
    FixedPosition* mFixedPos;  // 0x90
    MtxPtr mMtx;               // 0x94
    bool mCalcOwnMtx;          // 0x98
    bool _99;
    bool mIsDead;  // 0x9A
    bool _9B;
};
