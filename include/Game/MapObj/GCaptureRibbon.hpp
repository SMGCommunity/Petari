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

    /* 0x08C */ JUTTexture* mTexture;
    /* 0x090 */ TVec3f _90[0x40];
    /* 0x390 */ TVec3f _390[0x40];
    /* 0x690 */ TVec3f _690[0x40];
    /* 0x990 */ s32 _990;
    /* 0x994 */ s32 _994;
};
