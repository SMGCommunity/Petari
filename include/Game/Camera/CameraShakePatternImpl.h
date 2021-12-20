#pragma once

#include "JSystem/JGeometry/TVec.h"

class CameraShakePattern {
public:
    inline CameraShakePattern() {
        _4 = 0;
    }

    virtual inline ~CameraShakePattern() {

    }

    virtual bool isEnd() const = 0;
    virtual void getOffset(TVec2f *) const = 0;
    virtual void start() = 0;
    virtual void update() = 0;
    
    u32 _4;
};

class CameraShakePatternSingly : public CameraShakePattern {
public:
    CameraShakePatternSingly(float);
    virtual ~CameraShakePatternSingly();

    virtual bool isEnd() const;
    virtual void getOffset(TVec2f *) const;
    virtual void start();
    virtual void update();

    void setDirection(const TVec2f &);

    f32 _8;
    TVec2f mOffset;     // _C
    TVec2f mDirection;  // _14
};

class CameraShakePatternVerticalSin : public CameraShakePattern {
public:
    CameraShakePatternVerticalSin(float, float);
    virtual ~CameraShakePatternVerticalSin();

    virtual bool isEnd() const;
    virtual void getOffset(TVec2f *) const;
    virtual void start();
    virtual void update();

    f32 _8;
    f32 _C;
    TVec2f mOffset; // _10
};