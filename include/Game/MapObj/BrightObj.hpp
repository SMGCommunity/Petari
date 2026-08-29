#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class Sun;

class BrightInfo {
public:
    /// @brief Creates a new `BrightInfo`.
    BrightInfo();

    void write(const TVec2f&, const TVec2f&, f32);
    void endRead();
    void reset();

    /* 0x00 */ u32 mIndex;
    /* 0x04 */ u32 mCurrIndex;
    /* 0x08 */ TVec2f mBrightnessCenter[3];
    /* 0x20 */ TVec2f mRealCenter[3];
    /* 0x38 */ f32 mBright[3];
};

class BrightDrawInfo {
public:
    /// @brief Creates a new `BrightDrawInfo`.
    BrightDrawInfo();

    void write(u16, const TVec3f&, f32);

    /* 0x00 */ TVec3f mPosition[2];
    /* 0x18 */ f32 mRadius[2];
};

class BrightCamInfo {
public:
    /// @brief Creates a new `BrightCamInfo`.
    BrightCamInfo();

    void write(u16, const TPos3f&, const TProj3f&, const TVec3f&, const TVec3f&);

    /* 0x00 */ TPos3f mViewMtx[2];
    /* 0x60 */ TProj3f mProjectionMtx[2];
    /* 0xE0 */ TVec3f mCameraDir[2];
    /* 0xF8 */ TVec3f mCameraPos[2];
};

class BrightObjBase {
public:
    struct CheckArg {
        CheckArg() : mMaxBrightness(), mCurrBrightness(), mObjCenter(0.0f, 0.0f), mCenter(0.0f, 0.0f) {};

        /* 0x00 */ u32 mMaxBrightness;
        /* 0x04 */ u32 mCurrBrightness;
        /* 0x08 */ TVec2f mObjCenter;
        /* 0x10 */ TVec2f mCenter;
    };

    BrightObjBase();

    virtual ~BrightObjBase();
    virtual void calcBrightInfo(u16, const BrightCamInfo&) = nullptr;
    virtual f32 getBright() const;
    virtual const TVec2f* getBrightCenter() const;
    virtual const TVec2f* getCenter() const;
    virtual void endRead();
    virtual void getNowCenter(TVec2f*) const;

    void checkVisibilityOfSphere(u16, const BrightCamInfo&);
    void checkVisible(CheckArg*, const TVec3f&, const TPos3f&, const TProj3f&);
    void setResult(const CheckArg&);
    void drawSphere(const TVec3f&, float) const;

    /* 0x04 */ BrightInfo mInfo;
    /* 0x48 */ TVec2f mBrightCenter;
    /* 0x50 */ TVec2f mNowCenter;
    /* 0x58 */ f32 mBrightness;
    /* 0x5C */ bool mIsNotVisible;
    /* 0x60 */ BrightDrawInfo mDrawInfo;
};

class BrightObj : public LiveActor, public BrightObjBase {
public:
    BrightObj(const char*);

    virtual ~BrightObj();
    virtual void init(const JMapInfoIter&);
    virtual void draw() const;
    virtual void control();
    
    virtual void calcBrightInfo(u16, const BrightCamInfo&);
    virtual void getNowCenter(TVec2f*) const;

    void checkVisibilityOfSphere(u16, const BrightCamInfo&);

    /* 0x10C */ f32 mRadius;
};

class BrightSun : public LiveActor, public BrightObjBase {
public:
    BrightSun(const char*);

    virtual ~BrightSun();
    virtual void init(const JMapInfoIter&);
    virtual void draw() const;
    virtual void control();

    virtual void calcBrightInfo(u16, const BrightCamInfo&);
    virtual void getNowCenter(TVec2f*) const;

    void controlSunModel();

    /* 0x10C */ Sun* mSun;
};
