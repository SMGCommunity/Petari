#pragma once

#include "Game/LiveActor/ActorJointCtrl.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/LodCtrl.hpp"

class ModelObj : public LiveActor {
public:
    ModelObj(const char *, const char *, MtxPtr, int, int, int, bool);

    inline virtual ~ModelObj();
    virtual void init(const JMapInfoIter &);

    virtual void calcAndSetBaseMtx();

    MtxPtr mMtx;    // 0x8C
};

class ModelObjNpc : public LiveActor {
public:
    ModelObjNpc(const char *, const char *, MtxPtr);

    virtual ~ModelObjNpc();
    virtual void init(const JMapInfoIter &);
    virtual void control();
    virtual void calcAndSetBaseMtx();

    MtxPtr mMtx;                // 0x8C
    LodCtrl* mLODCtrl;          // 0x90
    ActorJointCtrl* mJointCtrl; // 0x94
};
