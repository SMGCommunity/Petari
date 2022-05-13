#pragma once

#include "Game/LiveActor/ActorJointCtrl.h"
#include "Game/LiveActor/LiveActor.h"
#include "Game/LiveActor/LodCtrl.h"

class ModelObj : public LiveActor {
public:
    ModelObj(const char *, const char *, MtxPtr, int, int, int, bool);

    virtual ~ModelObj();
    virtual void init(const JMapInfoIter &);

    virtual void calcAndSetBaseMtx();

    MtxPtr mMtx;    // _8C
};

class ModelObjNpc : public LiveActor {
public:
    ModelObjNpc(const char *, const char *, MtxPtr);

    virtual ~ModelObjNpc();
    virtual void init(const JMapInfoIter &);
    virtual void control();
    virtual void calcAndSetBaseMtx();

    MtxPtr mMtx;                // _8C
    LodCtrl* mLODCtrl;          // _90
    ActorJointCtrl* mJointCtrl; // _94
};