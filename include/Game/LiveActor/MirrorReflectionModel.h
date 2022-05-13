#pragma once

#include "Game/LiveActor/LiveActor.h"

class MirrorReflectionModel : public LiveActor {
public:
    MirrorReflectionModel(const LiveActor *, const char *, const char *, MtxPtr);
    ~MirrorReflectionModel() {};

    void init(JMapInfoIter const &);
    void movement();
    void calcAnim();
    void calcViewAndEntry();
    void calcAndSetBaseMtx();

    const LiveActor* _8C;
    MtxPtr _90;
    u8 _94;
};