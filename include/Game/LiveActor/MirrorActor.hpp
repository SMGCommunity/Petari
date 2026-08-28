#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class MirrorActor : public LiveActor {
public:
    MirrorActor(LiveActor*, const char*, const char*);

    /* 0x08 */ virtual ~MirrorActor();

    /* 0x0C */ virtual void init(const JMapInfoIter&) override;
    /* 0x14 */ virtual void movement() override;
    /* 0x1C */ virtual void calcAnim() override;
    /* 0x20 */ virtual void calcViewAndEntry() override;

    bool isHostInTheOtherSideOfMirror() const;
    TVec3f getHostCenterPos() const;

    /* 0x8C */ LiveActor* mHost;
    /* 0x90 */ TVec3f mCenter;
    /* 0x9C */ f32 mRadius;
    /* 0xA0 */ bool mIsDisconnected;
};
