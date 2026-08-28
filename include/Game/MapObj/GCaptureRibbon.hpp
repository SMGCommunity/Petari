#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class JUTTexture;

class GCaptureRibbon : public LiveActor {
public:
    GCaptureRibbon(const char*);

    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x18 */ virtual void draw() const;

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
