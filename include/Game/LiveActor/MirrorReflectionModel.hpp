#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class MirrorReflectionModel : public LiveActor {
public:
    MirrorReflectionModel(const LiveActor*, const char*, const char*, MtxPtr);

    /* 0x08 */ virtual ~MirrorReflectionModel();
    /* 0x0C */ virtual void init(const JMapInfoIter&) override;
    /* 0x14 */ virtual void movement() override;
    /* 0x1C */ virtual void calcAnim() override;
    /* 0x20 */ virtual void calcViewAndEntry() override;
    /* 0x4C */ virtual void calcAndSetBaseMtx() override;

    const LiveActor* _8C;
    MtxPtr _90;
    u8 _94;
};
