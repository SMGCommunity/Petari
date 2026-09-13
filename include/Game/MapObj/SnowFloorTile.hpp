#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class JUTTexture;

class SnowFloorTile : public LiveActor {
public:
    SnowFloorTile(const char*);

    virtual ~SnowFloorTile();
    virtual void init(const JMapInfoIter&) override;
    virtual void draw() const override;
    virtual void control() override;
    virtual void attackSensor(HitSensor*, HitSensor*) override;

    void initLocalPoint(const JMapInfoIter&);
    bool calcLocalPoint(const TVec3f&, TVec3f*, f32);
    void createReduceMap(u16);
    void digPlayerWalk(const TVec3f&);
    void digDpd(const TVec3f&);
    bool doErase(const TVec3f&, f32, f32);
    f32 doMove(TVec2f, TVec2f, f32, f32);
    void doErase(TVec2f, f32, f32);
    void createTexture();
    void drawLayer(s32) const;

    /* 0x8C */ u8 _8C[12];
    /* 0x98 */ TVec3f mRight;
    /* 0xA4 */ TVec3f mUp;
    /* 0xB0 */ TVec3f mForward;
    /* 0xBC */ f32 mCellSize;
    /* 0xC0 */ s32 mTextureShift;
    /* 0xC4 */ s32 mTextureSize;
    /* 0xC8 */ TVec2f mLastPlayerPoint;
    /* 0xD0 */ u8 _D0[8];
    /* 0xD8 */ u16 mDirtyFrames;
    /* 0xDC */ u8* mImage;
    /* 0xE0 */ JUTTexture* mTexture;
    /* 0xE4 */ f32* mSnowDepth;
    /* 0xE8 */ f32* mReduceMap;
    /* 0xEC */ u16 mReduceMapSize;
    /* 0xF0 */ TVec2f mLastDpdPoint;
    /* 0xF8 */ u16 _F8;
    /* 0xFC */ f32 _FC;
    /* 0x100 */ JUTTexture* mTextures[2];
    /* 0x108 */ u8 mTextureIndex;
};
