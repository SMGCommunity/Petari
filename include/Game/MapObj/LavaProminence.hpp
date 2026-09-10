#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class PartsModel;

class LavaProminence : public LiveActor {
public:
    /// @brief Creates a new `LavaProminence`.
    /// @param pName A pointer to the null-terminated name of the object.
    LavaProminence(const char* pName);

    virtual void init(const JMapInfoIter& rIter);
    virtual void initAfterPlacement();
    virtual void appear();
    virtual void kill();
    virtual void startClipped();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);

    void moveOnRail();
    void setGravityAndMakeMtx();
    void updateEffectClipping(bool);

    void exeWaitSwitch();
    void exeWait();
    void exeSign();
    void exeMoveStartExtra();
    void exeMoveStart();
    void exeMoveLoop();
    void exeMoveEnd();
    void exeMoveEndExtra();

    bool isNrvMove();

    /* 0x08C */ s32 mWaitTime;
    /* 0x090 */ f32 mMoveSpeed;
    /* 0x094 */ f32 mMoveRailCoord;
    /* 0x098 */ f32 mMoveRailEndOffset;
    /* 0x09C */ TVec3f mRailStartDir;
    /* 0x0A8 */ TVec3f mRailEndDir;
    /* 0x0B4 */ TVec3f mRailStartPos;
    /* 0x0C0 */ TVec3f mRailEndPos;
    /* 0x0CC */ TPos3f mEffectStartMtx;
    /* 0x0FC */ TPos3f mEffectEndMtx;
    /* 0x12C */ TVec3f _12C;
    /* 0x138 */ PartsModel* mBloomModel;
    /* 0x13C */ TPos3f mBloomModelMtx;
    /* 0x16C */ TQuat4f _16C;
    /* 0x17C */ TVec3f mRailDir;
    /* 0x188 */ bool mEmitDropEffect;
    /* 0x18C */ TPos3f _18C;
};
