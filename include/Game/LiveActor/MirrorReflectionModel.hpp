#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class MirrorReflectionModel : public LiveActor {
public:
    MirrorReflectionModel(const LiveActor *, const char *, const char *, MtxPtr);

    virtual ~MirrorReflectionModel();
    virtual void init(const JMapInfoIter &);
    virtual void movement();
    virtual void calcAnim();
    virtual void calcViewAndEntry();
    virtual void calcAndSetBaseMtx();

    const LiveActor* _8C;
    MtxPtr _90;
    u8 _94;
};
