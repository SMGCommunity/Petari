#pragma once

#include <JSystem/JGeometry/TMatrix.hpp>

class BrightInfo {
public:
    /// @brief Creates a new `BrightInfo`.
    BrightInfo();

    void write(const TVec2f&, const TVec2f&, f32);
    void endRead();
    void reset();

    /* 0x00 */ u32 _0;
    /* 0x04 */ u32 _4;
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

    // void write(u16, const TPos3f&, const JGeometry::TProjection3<JGeometry::TMatrix44<JGeometry::SMatrix44C<f32>>>&, const TVec3f&, const TVec3f&)

    /* 0x00 */ TMtx34f mViewMtx[2];
    /* 0x60 */ Mtx44 mProjectionMtx[2];
    /* 0xE0 */ TVec3f mCameraDir[2];
    /* 0xF8 */ TVec3f mCameraPos[2];
};
