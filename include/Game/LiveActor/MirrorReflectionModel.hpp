#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class MirrorReflectionModel : public LiveActor {
public:
    MirrorReflectionModel(const LiveActor*, const char*, const char*, MtxPtr);

    virtual ~MirrorReflectionModel();
    virtual void init(const JMapInfoIter&) override;
    virtual void movement() override;
    virtual void calcAnim() override;
    virtual void calcViewAndEntry() override;
    virtual void calcAndSetBaseMtx() override;

    const LiveActor* _8C;
    MtxPtr _90;
    u8 _94;
};
