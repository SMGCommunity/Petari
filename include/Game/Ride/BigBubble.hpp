#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>
#include <JSystem/JGeometry/TVec.hpp>

class OctahedronBezierSurface;
class BigBubbleMoveLimitter;

class BigBubble : public LiveActor {
public:
    enum ShapeType { ShapeType_Sphere = -1, ShapeType_Cube = 0, ShapeType_Octahedron = 1 };

    BigBubble(const char*);

    virtual void init(const JMapInfoIter&);
    virtual void makeActorAppeared();
    virtual void makeActorDead();
    virtual void control();
    virtual void calcAnim();
    virtual void draw() const;
    virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgPush(HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual MtxPtr getBaseMtx() const {
        return (MtxPtr)&mBaseMtx;
    }

    void initSensor();
    void generate(const TVec3f&, const TVec3f&, f32, bool, s32, s32, const BigBubbleMoveLimitter*);
    void setActionType(s32);
    void setShapeType(s32);
    f32 getBaseRadius() const;
    void setHost(LiveActor*);
    void setScale(f32);
    bool requestBind(HitSensor*);
    bool requestMerged(HitSensor* pSender, HitSensor* pReceiver);
    bool requestAssimilate(HitSensor* pSender, HitSensor* pReceiver);
    bool requestCancelBind();
    bool tryAppearEnd();
    bool tryBreak();
    bool tryAutoBreak();
    bool tryBreakEnd();
    bool tryEscape();
    bool tryEscapeEnd();
    bool tryMergedCancel();
    bool tryMergedEnd();
    bool tryGoal();

    void exeAppear();
    void exeWait();
    void exeCapture();
    void exeMerged();
    void exeBreak();
    void exeEscape();
    void exeGoal();

    void endCapture();

    void addDeformVelocityOuter(const TVec3f&, bool);
    void addDeformVelocityInternalOressure();
    void addDeformVelocityRebound();
    void addCoriolisAccel();
    void addAccelMoment(s32 padChannel);
    bool addAccelPointing(s32 padChannel);
    void doMoveLimit();
    void updatePose();
    void updateBindActorMatrix();
    void updateNormalVelocity();
    void resetDeformVelocity();
    void updateDeformVelocity();
    void updateMeshPoint();
    void updateCaptureReduceVolume();
    void updateCaptureWarningColor();
    void calcMergePosition(TVec3f*, TVec3f*, const BigBubble*) const;
    void calcLocalDirection(TVec3f*, s32) const;
    s32 getNearAxisIndex(const TVec3f&) const;
    s32 getMergeIndex(const TVec3f&) const;

    bool isPushable() const;
    bool isBindMario() const;
    bool isDraw() const;
    bool isMerged() const;
    bool isEnemyAttackBreakable() const;

    inline f32 getSize() {
        f32 radius = getBaseRadius();
        return radius * mScale.x;
    }

    inline f32 getRadius(f32 volume) {
        return pow(volume, 1.0f / 3.0f);
    }

    /* 0x8C */ OctahedronBezierSurface* mSurface;
    /* 0x90 */ const BigBubbleMoveLimitter* mMoveLimitter;
    /* 0x94 */ LiveActor* mHost;
    /* 0x98 */ LiveActor* mRider;
    /* 0x9C */ BigBubble* mMergeBubble;
    /* 0xA0 */ BigBubble* mMergeBubbles[6];
    /* 0xB8 */ TPos3f mEffectMtx;
    /* 0xE8 */ TPos3f _E8;  // unused
    /* 0x118 */ TPos3f mBaseMtx;
    /* 0x148 */ TQuat4f mBubbleQuat;
    /* 0x158 */ TQuat4f mRiderQuat;
    /* 0x168 */ TVec3f mRiderBasePos;
    /* 0x174 */ TVec3f mRiderPos;
    /* 0x180 */ TVec3f mSpawnPosition;
    /* 0x18C */ TVec3f _18C;  // unused?
    /* 0x198 */ TVec3f mPointerPos;
    /* 0x1A4 */ TVec3f _1A4;  // deform direction for something? not implemented
    /* 0x1B0 */ TVec3f mMoment;
    /* 0x1BC */ TVec3f mCoriolisAccel;
    /* 0x1C8 */ f32 mDeformCoeff[6];
    /* 0x1E0 */ f32 mDeformVelocity[6];
    /* 0x1F8 */ f32 mInterpolateTime;
    /* 0x1FC */ f32 mBlowForce;
    /* 0x200 */ s32 _200;  // some timer
    /* 0x204 */ s32 mReduceVolumeTimer;
    /* 0x208 */ s32 _208;  // some timer
    /* 0x20C */ s32 mMergeIndex;
    /* 0x210 */ s32 mShapeType;
    /* 0x214 */ s32 _214;  // unused
    /* 0x218 */ f32 mAppearRadius;
    /* 0x21C */ f32 mVolume;
    /* 0x220 */ f32 _220;  // ????????? camZ.dot(mPosition) + mScale.x
    /* 0x224 */ f32 mBinderRadius;
    /* 0x228 */ Color8 mWarningColor;
    /* 0x22C */ bool _22C;  // detached?????
    /* 0x22D */ bool _22D;  // unused
    /* 0x22E */ bool mIsHidden;
    /* 0x22F */ bool _22F;  // related to breaking?
    /* 0x230 */ bool mIsBroken;
    /* 0x231 */ bool mIsShrinkable;
    /* 0x232 */ bool mIsAttached;  // disables addCoriolisAccel
};
