#pragma once
#include "Game/LiveActor/LiveActor.hpp"
class JUTTexture;
class SnowFloor : public LiveActor {
public:
    SnowFloor(const char*);

    virtual ~SnowFloor();
    virtual void init(const JMapInfoIter&) override;
    virtual void draw() const override;
    virtual void control() override;
    virtual void attackSensor(HitSensor*, HitSensor*) override;
    void drawSetup();
    void initLocalPoint(const JMapInfoIter&);
    bool calcLocalPoint(const TVec3f&, TVec3f*);
    void createReduceMap(u16);
    void digPlayerWalk(const TVec3f&);
    void digDpd(const TVec3f&);
    bool doErase(const TVec3f&, f32, f32);
    f32 doMove(TVec2f, TVec2f, f32, f32);
    void doErase(TVec2f, f32, f32);
    void createTexture();
    void addCheckList(HitSensor*);
    void updateCheckList();
    f32 calcCoverSnow(const TVec2f&, f32);
    void drawLayer(s32) const;
    /* 0x8C */ f32 mHalfLength;
    /* 0x90 */ f32 mHalfAngle;
    /* 0x94 */ f32 mRadius;
    /* 0x98 */ TVec3f mUp;
    /* 0xA4 */ TVec3f mAxis;
    /* 0xB0 */ TVec2f mLastPlayerPoint;
    /* 0xB8 */ f32* mTexCoords;
    /* 0xBC */ TVec3f* mDirections;
    /* 0xC0 */ u16 mDirtyFrames;
    /* 0xC4 */ u8* mImage;
    /* 0xC8 */ JUTTexture* mTexture;
    /* 0xCC */ f32* mSnowDepth;
    /* 0xD0 */ u8* mBaseDepth;
    /* 0xD4 */ f32* mReduceMap;
    /* 0xD8 */ u16 mReduceMapSize;
    /* 0xDC */ TVec2f mLastDpdPoint;
    u16 _E4;
    f32 _E8;
    /* 0xEC */ f32 mWalkingResist;
    /* 0xF0 */ u16 mResistFrames;
    /* 0xF2 */ u16 mCheckCount;
    /* 0xF4 */ HitSensor* mCheckList[8];
    /* 0x114 */ JUTTexture* mTextures[2];
    /* 0x11C */ u8 mTextureIndex;
};
