#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class GCaptureRibbon : public LiveActor {
public:
    GCaptureRibbon(const char*);

    virtual void init(const JMapInfoIter&);
    virtual void draw() const;

    void reset();
    void lengthen(const TVec3f&, const TVec3f&);
    void shorten(const TVec3f&, const TVec3f&);
    void updateAxis();
    f32 calcLineWidth() const;

    JUTTexture* mTexture;  // 0x8C
    TVec3f _90;
    TVec3f _9C;
    TVec3f _A8[0xBE];
    u32 _990;
    u32 _994;
};
