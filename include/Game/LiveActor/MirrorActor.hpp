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
    TVec3f getHostCenterPos() const;

    /* 0x8C */ LiveActor* mHost;
    /* 0x90 */ TVec3f mCenter;
    /* 0x9C */ f32 mRadius;
    /* 0xA0 */ bool mIsDisconnected;
};
