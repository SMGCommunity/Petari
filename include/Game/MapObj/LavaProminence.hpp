#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "JSystem/JGeometry/TMatrix.hpp"
#include "JSystem/JGeometry/TVec.hpp"

class PartsModel;

class LavaProminence : public LiveActor {
public:
    LavaProminence(const char*);

    virtual void init(const JMapInfoIter&);
    virtual void initAfterPlacement();
    virtual void appear();
    virtual void kill();
    virtual void startClipped();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor*, HitSensor*);

    void moveOnRail();
    void setGravityAndMakeMtx();
    void updateEffectClipping(bool);
    bool isNrvExtra();
    f32 getRadius(const char*) const;

    void exeWaitSwitch();
    void exeWait();
    void exeSign();
    void exeMoveStartExtra();
    void exeMoveStart();
    void exeMoveLoop();
    void exeMoveEnd();
    void exeMoveEndExtra();

    /* 0x8C */ s32 mStepAppearance;
    /* 0x90 */ f32 mMovementSpeed;
    /* 0x94 */ f32 _94;
    /* 0x98 */ f32 mEndPathOffset;
    /* 0x9C */ TVec3f mRailDir;
    /* 0xA8 */ TVec3f mRailDirEnd;
    /* 0xB4 */ TVec3f _B4;
    /* 0xC0 */ TVec3f _C0;
    /* 0xCC */ TPos3f _CC;
    /* 0xFC */ TPos3f _FC;
    /* 0x12C */ TVec3f _12C;
    /* 0x138 */ PartsModel* mBloomModel;
    /* 0x13C */ TPos3f _13C;
    /* 0x16C */ TQuat4f _16C;
    /* 0x17C */ TVec3f _17C;
    /* 0x188 */ bool mEmitDropEffect;
    /* 0x18C */ TPos3f _18C;
};
