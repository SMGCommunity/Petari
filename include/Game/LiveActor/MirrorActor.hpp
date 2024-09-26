#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class MirrorActor : public LiveActor {
public:
    MirrorActor(LiveActor *, const char *, const char *);

    virtual ~MirrorActor();

    virtual void init(const JMapInfoIter &);
    virtual void movement();
    virtual void calcAnim();
    virtual void calcViewAndEntry();

    bool isHostInTheOtherSideOfMirror() const;
    TVec3f& getHostCenterPos() const;

    LiveActor* _8C;
    u32 _90;
    u32 _94;
    u32 _98;
    f32 _9C;
    u8 _A0;
};