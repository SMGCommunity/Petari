#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class JUTTexture;

class GCaptureRibbon : public LiveActor {
public:
    GCaptureRibbon(const char*);

    virtual ~GCaptureRibbon();
    virtual void init(const JMapInfoIter&);
    virtual void draw() const;

    void reset();
    void lengthen(const TVec3f&, const TVec3f&);
    void shorten(const TVec3f&, const TVec3f&);
    void updateAxis();
    f32 calcLineWidth() const;

    JUTTexture* mTexture;  // 0x8C
    TVec3f _90[0x40];
    TVec3f _390[0x40];
    TVec3f _690[0x40];
    s32 _990;
    s32 _994;
};
