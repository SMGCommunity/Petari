#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class MirrorActor : public LiveActor {
public:
    MirrorActor(LiveActor*, const char*, const char*);

    virtual ~MirrorActor();

    virtual void init(const JMapInfoIter&) override;
    virtual void movement() override;
    virtual void calcAnim() override;
    virtual void calcViewAndEntry() override;

    bool isHostInTheOtherSideOfMirror() const;
    TVec3f& getHostCenterPos() const;

    LiveActor* _8C;
    u32 _90;
    u32 _94;
    u32 _98;
    f32 _9C;
    bool _A0;
};
