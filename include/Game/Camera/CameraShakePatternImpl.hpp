#pragma once

#include "JSystem/JGeometry/TVec.hpp"

class CameraShakePattern {
public:
    CameraShakePattern() : mFrame() {
    }

    virtual ~CameraShakePattern() {
    }

    virtual bool isEnd() const = 0;
    virtual void getOffset(TVec2f*) const = 0;
    virtual void start() = 0;
    virtual void update() = 0;

    /* 0x4 */ u32 mFrame;
};

class CameraShakePatternSingly : public CameraShakePattern {
public:
    CameraShakePatternSingly(f32);

    virtual bool isEnd() const;
    virtual void getOffset(TVec2f*) const;
    virtual void start();
    virtual void update();

    void setDirection(const TVec2f&);

    /* 0x08 */ f32 mIntensity;
    /* 0x0C */ TVec2f mOffset;
    /* 0x14 */ TVec2f mDirection;
};

class CameraShakePatternVerticalSin : public CameraShakePattern {
public:
    CameraShakePatternVerticalSin(f32, f32);

    virtual bool isEnd() const;
    virtual void getOffset(TVec2f*) const;
    virtual void start();
    virtual void update();

    /* 0x08 */ f32 mIntensity;
    /* 0x0C */ f32 mSpeed;
    /* 0x10 */ TVec2f mOffset;
};
