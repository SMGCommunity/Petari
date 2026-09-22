#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class SeaGullGroup;

class SeaGull : public LiveActor {
public:
    SeaGull(SeaGullGroup* pGroup);

    virtual ~SeaGull();
    virtual void init(const JMapInfoIter& rIter);
    virtual void control();
    virtual void calcAndSetBaseMtx();

    void exeHoverFront();
    void exeHoverLeft();
    void exeHoverRight();
    void updateHover();

    /* 0x8C */ SeaGullGroup* mSeaGullGroup;
    /* 0x90 */ s32 mPointIndex;
    /* 0x94 */ bool mIsReverse;
    /* 0x98 */ s32 mTargetUpdateTimer;
    /* 0x9C */ TVec3f* mTargetPosition;
    /* 0xA0 */ TVec3f mUp;
    /* 0xAC */ TVec3f mFront;
    /* 0xB8 */ TVec3f mBankedUp;
    /* 0xC4 */ TVec3f mSide;
    /* 0xD0 */ s32 mHoverTimer;
    /* 0xD4 */ f32 mBankAngle;
    /* 0xD8 */ s32 mGlideTimer;
    /* 0xDC */ s32 mLiftTimer;
    /* 0xE0 */ s32 mChirpTimer;
};

class SeaGullGroup : public LiveActor {
public:
    SeaGullGroup(const char* pName);

    virtual ~SeaGullGroup();
    virtual void init(const JMapInfoIter& rIter);

    TVec3f* updatePosInfo(s32* pPointIndex, bool isReverse) const;

    /* 0x8C */ s32 mPointCount;
    /* 0x90 */ TVec3f* mPoints;
};
