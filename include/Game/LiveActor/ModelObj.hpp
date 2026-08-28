#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class ActorJointCtrl;
class LodCtrl;

class ModelObj : public LiveActor {
public:
    ModelObj(const char*, const char*, MtxPtr, int, int, int, bool);

    /* 0x0C */ virtual void init(const JMapInfoIter&) override;
    /* 0x4C */ virtual void calcAndSetBaseMtx() override;

    /* 0x8C */ MtxPtr mMtx;
};

class ModelObjNpc : public LiveActor {
public:
    ModelObjNpc(const char*, const char*, MtxPtr);

    /* 0x08 */ virtual ~ModelObjNpc();
    /* 0x0C */ virtual void init(const JMapInfoIter&) override;
    /* 0x48 */ virtual void control() override;
    /* 0x4C */ virtual void calcAndSetBaseMtx() override;

    /* 0x8C */ MtxPtr mMtx;
    /* 0x90 */ LodCtrl* mLodCtrl;
    /* 0x94 */ ActorJointCtrl* mJointCtrl;
};
