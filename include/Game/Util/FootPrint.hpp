#pragma once

#include "Game/NameObj/NameObj.hpp"
#include <JSystem/JGeometry/TVec.hpp>

class ResTIMG;
class JUTTexture;
class FootPrintInfo;

class FootPrintInfo {
public:
    FootPrintInfo();

    /* 0x00 */ TVec3f mPos;
    /* 0x0C */ TVec3f _C;
    /* 0x18 */ TVec3f _18;

    /// @brief Frames left before becoming invalid. Also controls the footprint's alpha.
    /* 0x24 */ s32 mTimeLeft;

    /// @brief Invalid prints will no longer be updated nor drawn.
    /* 0x28 */ bool mIsValid;

    /* 0x29 */ bool _29;
};

class FootPrint : public NameObj {
public:
    FootPrint(const char*, s32);
    FootPrint(const char*, s32, s32);

    virtual void movement();
    virtual void draw() const;

    void setTexture(ResTIMG*);
    bool addPrint(const TVec3f&, const TVec3f&, const TVec3f&, bool);
    void clear();
    void initMember(s32, s32);
    void clearForce();
    TVec3f* getPrintPos(u32) const;
    void invalidate(u32);
    bool isValid(u32) const;

    /* 0x0C */ JUTTexture* _C;

    /// @brief Array of footprints.
    /* 0x10 */ FootPrintInfo* mPrints;

    /* 0x14 */ TVec3f mLastPrintPos;

    /// @brief Size of the mPrints array.
    /* 0x20 */ s32 mPrintMaxNum;

    /// @brief Amount of active footprints (counting invalid ones). Caps at mPrintMaxNum.
    /* 0x24 */ s32 mPrintNum;

    /// @brief The index used when adding a new footprint. Will wrap around and overwrite the oldest footprints.
    /* 0x28 */ s32 mRotatingPrintIndex;

    /* 0x2C */ f32 _2C;
    /* 0x30 */ f32 _30;
    /* 0x34 */ f32 _34;

    /// @brief The minimum distance for a new print to be registered.
    /* 0x38 */ f32 mMinPrintDistance;

    /* 0x3C */ bool _3C;
};
