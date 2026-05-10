#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class SwingRopePoint;
class JUTTexture;

class Flag : public LiveActor {
public:
    class FixPoints {
    public:
        FixPoints() {
        }

        /* 0x00 */ TVec3f mPos;
        /* 0x0C */ SwingRopePoint** mPoints;
    };

    Flag(const char*);

    virtual ~Flag();

    virtual void init(const JMapInfoIter& rIter);
    virtual void movement();
    virtual void draw() const;
    virtual void appear();

    void setInfoPos(const char* pObjName, const TVec3f* pPos, const TVec3f& rSide, f32 stickLength, f32 width, f32 height, s32 numPointsU,
                    s32 numPointsV, f32);
    void updateFlag();
    void loadMaterial() const;

    s32 getNumPointsU() const {
        return mNumPointsU;
    }

    s32 getNumPointsV() const {
        return mNumPointsV;
    }

    const FixPoints& getFixPoint(s32 idx) const {
        return mFixPoints[idx];
    }

    SwingRopePoint* getPoint(s32 idxV, s32 idxU) const {
        return mFixPoints[idxV].mPoints[idxU];
    }

    /* 0x8C */ u32 _8C;  // Note: unused?
    /* 0x90 */ const char* mObjName;
    /* 0x94 */ bool mDisableLighting;
    /* 0x98 */ s32 mNumPointsV;
    /* 0x9C */ s32 mNumPointsU;
    /* 0xA0 */ f32 mPointIntervalU;
    /* 0xA4 */ f32 mPointIntervalV;
    /* 0xA8 */ FixPoints* mFixPoints;
    /* 0xAC */ TVec3f mUp;
    /* 0xB8 */ TVec3f mClipCenter;
    /* 0xC4 */ const TVec3f* mBasePos;
    /* 0xC8 */ TPos3f* mBaseMtx;
    /* 0xCC */ f32 mStickLength;
    /* 0xD0 */ f32 mWavePhase;
    /* 0xD4 */ TVec3f mSide;
    /* 0xE0 */ TVec3f mFront;
    /* 0xEC */ bool mIsVerticalFlag;
    /* 0xF0 */ s32 mWaitTime;
    /* 0xF4 */ f32 mGravityScale;
    /* 0xF8 */ f32 mFrictionRateMin;
    /* 0xFC */ f32 mFrictionRateMax;
    /* 0x100 */ f32 mWindAccelRate;
    /* 0x104 */ f32 mWindAccelMin;
    /* 0x108 */ f32 mRandomWindAccelMin;
    /* 0x10C */ f32 mRandomWindAccelMax;
    /* 0x110 */ f32 mLightColorMin;
    /* 0x114 */ Color8* mColors;
    /* 0x118 */ f32* mTexST;
    /* 0x11C */ JUTTexture* mTexture;
    /* 0x120 */ bool mUseAlpha;
    /* 0x121 */ u8 mAlpha;
    /* 0x124 */ f32 mAlphaDistanceNear;
    /* 0x128 */ f32 mAlphaDistanceFar;
    /* 0x12C */ s32 mSeStep;
    /* 0x130 */ bool mDisableSound;
};
