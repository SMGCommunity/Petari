#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class JUTTexture;

class TrampleStar : public LiveActor {
public:
    class Surface {
    public:
        Surface(){};

        void set(u16 vtx1, u16 vtx2, u16 vtx3) {
            mVtxs[0] = vtx1;
            mVtxs[1] = vtx2;
            mVtxs[2] = vtx3;
        }

        /* 0x00 */ u16 mVtxs[3];
    };

    TrampleStar(const char*);

    virtual ~TrampleStar();

    virtual void init(const JMapInfoIter& rIter);
    virtual void draw() const;
    virtual void calcAnim();
    virtual void control();
    virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void toSphere(f32);
    void initCheckFlag(u32);
    u16 readCheckFlag(u16, u16);
    void writeCheckFlag(u16, u16, u16);
    void divide();
    void moveVtx(f32, u32, u32, f32, u32);
    void calcSurface(bool);
    void calcLodSurface();
    void onLod();
    void offLod();

    void exeWait();
    void exeBindingCapture();
    void exeBindingShoot();
    u32 calcNearestVtxIndex(const TVec3f&);
    void drawSelf() const;

    /* 0x8C */ u32 _8C;  // NOTE: unused
    /* 0x90 */ u16* mCheckFlags;
    /* 0x94 */ u16 mNumFlags;
    /* 0x96 */ u16 mNumSurfaces;
    /* 0x98 */ u16 mNumVtxs;
    /* 0x9C */ TVec3f* mVtxs;
    /* 0xA0 */ f32* mDeformCoeff;
    /* 0xA4 */ f32* mDeformSpeed;
    /* 0xA8 */ u8* _A8;  // NOTE: allocated but unused?
    /* 0xAC */ Surface* mSurfaces;
    /* 0xB0 */ Surface* mLodSurfaces;
    /* 0xB4 */ u32 _B4;  // NOTE: unused
    /* 0xB8 */ JUTTexture* mTexture;
    /* 0xBC */ TVec2f* mTexST;
    /* 0xC0 */ s16* mVtxOverlap;
    /* 0xC4 */ u16 mNumDrawVtxs;
    /* 0xC8 */ TVec3f* mDrawPos;
    /* 0xCC */ TVec3f* mDrawNorm;
    /* 0xD0 */ TVec2f* mDrawTex;
    /* 0xD4 */ f32* mDrawDeformCoeffs;
    /* 0xD8 */ bool mIsLod;
    /* 0xDA */ u16 mCaptureVtx;
    /* 0xDC */ f32 mDeformRate;
    /* 0xE0 */ f32 mJumpVel;
    /* 0xE4 */ f32 mSpringRate;
    /* 0xE8 */ u16 mCaptureFrames;
};

class TrampleStarShare : public LiveActor {
public:
    TrampleStarShare(TrampleStar*);
    void copy(TrampleStar*) const;

    /* 0x8C */ TrampleStar* mTrampleStar;
};
