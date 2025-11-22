#pragma once

#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "JSystem/JGeometry/TMatrix.hpp"

class AudAnmSoundObject;
class IKJoint;
class TripodBoss;
class TripodBossMovableArea;
class TripodBossStepPoint;

class TripodBossLeg : public LiveActor {
public:
    TripodBossLeg(const char*);

    virtual ~TripodBossLeg();
    virtual void init(const JMapInfoIter&);
    virtual void makeActorAppeared();
    virtual void makeActorDead();
    virtual void control();
    virtual void attackSensor(HitSensor*, HitSensor*);

    void initShadow(const char*);
    void setBody(TripodBoss*);
    void setMovableArea(TripodBossMovableArea*);
    void setIKParam(f32, f32, const TVec3f&, const TVec3f&, const TVec3f&);
    void setStepTarget(TripodBossStepPoint*);
    void setWait();
    void setForceEndPoint(const TVec3f&);
    void setDemoEffectTiming(bool);
    const TPos3f& getRootJointMatrix() const;
    const TPos3f& getMiddleJointMatrix() const;
    const TPos3f& getEndJointMatrix() const;
    const TPos3f& getRootLocalYMatrix() const;
    const TPos3f& getRootLocalYZMatrix() const;
    const TPos3f& getAnkleLocalXMatrix() const;
    const TPos3f& getAnkleLocalXZMatrix() const;
    void requestStepTarget(TripodBossStepPoint*);
    void requestLeaveOut();
    void requestBreak();
    void requestStartDemo();
    void requestEndDemo();
    bool isLanding() const;
    bool isDamage() const;
    bool isStop() const;
    bool isBroken() const;
    bool isPressPlayer() const;
    bool canStep() const;
    bool canCancelStep() const;
    bool canWeighting() const;
    void exeDemo();
    void exeUp();
    void exeLeaveOut();
    void exeMove();
    void exeMoveToLandingPos();
    void exeStampSign();
    void exeLanding();
    void exeDamageVibration();
    void exeBreak();
    inline void exeDamage();
    inline void exeHold();
    void addToTargetPower(const TVec3f&, f32);
    void addIKLimitPower();
    void addAccelUpLeg(TripodBossStepPoint*, f32);
    f32 calcLegHeight(TripodBossStepPoint*) const NO_INLINE;
    void updatePose();
    void updateIKPose();
    bool bindEndPosition();
    void separateLocalAxisRootJoint(const TPos3f&);
    void separateAnkleJointLocalAxis();
    void updateAnkleUp(f32);
    void updateAnkleSlerpToBasePose();
    void updateAnkleLanding();
    void updateAnkleShadowMatrix();

    TripodBoss* mBoss;                 // 0x8C
    TripodBossMovableArea* mMoveArea;  // 0x90
    TripodBossStepPoint* _94;
    TripodBossStepPoint* _98;
    IKJoint* mJoint;  // 0x9C
    TPos3f _A0;
    TPos3f mRootLocalYMtx;    // 0xD0
    TPos3f mRootLocalYZMtx;   // 0x100
    TPos3f mAnkleLocalXMtx;   // 0x130
    TPos3f mAnkleLocalXZMtx;  // 0x160
    TPos3f mEndJointMtx;      // 0x190
    TPos3f _1C0;
    TQuat4f _1F0;
    TQuat4f _200;
    TVec3f _210;
    TVec3f _21C;
    TVec3f mForceEndPoint;  // 0x228
    TVec3f _234;
    TVec3f _240;
    u32 _24C;
    f32 _250;
    u8 _254;
    bool mIsPressPlayer;  // 0x255
    u8 _256;
    u8 _257;
    s32 mDemoTiming;  // 0x258
    AudAnmSoundObject* _25C;
    AudAnmSoundObject* _260;
};

namespace MR {
    void separateMatrixRotateYZX(TPos3f*, TPos3f*, const TPos3f&, const TPos3f&);
};
