#pragma once

#include "Game/LiveActor/ActorJointCtrl.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/LodCtrl.hpp"

class ModelObj : public LiveActor {
public:
    ModelObj(const char*, const char*, MtxPtr, int, int, int, bool);

    virtual void init(const JMapInfoIter&) override;
    virtual void calcAndSetBaseMtx() override;

    /* 0x8C */ MtxPtr mMtx;
};

class ModelObjNpc : public LiveActor {
public:
    ModelObjNpc(const char*, const char*, MtxPtr);

    virtual ~ModelObjNpc();
    virtual void init(const JMapInfoIter&) override;
    virtual void control() override;
    virtual void calcAndSetBaseMtx() override;

    /* 0x8C */ MtxPtr mMtx;
    /* 0x8C */ LodCtrl* mLodCtrl;
    /* 0x8C */ ActorJointCtrl* mJointCtrl;
};
