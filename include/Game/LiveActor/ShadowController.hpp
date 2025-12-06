#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Util.hpp"
#include "Game/Util/Array.hpp"
#include <revolution.h>

class CollisionPartsFilterBase;
class ShadowDrawer;
class LiveActor;
class ShadowController;

class ShadowControllerList {
public:
    ShadowControllerList(LiveActor*, u32);

    void addController(ShadowController*);
    u32 getControllerCount() const;
    ShadowController* getController(u32) const;
    ShadowController* getController(const char*) const;
    void resetCalcCount();
    void requestCalc();

    MR::Vector< MR::AssignableArray< ShadowController* > > mShadowList;  // 0x0
    LiveActor* mHost;                                                    // 0xC
};

class ShadowControllerHolder : public NameObj {
public:
    ShadowControllerHolder();

    virtual ~ShadowControllerHolder();
    virtual void initAfterPlacement() override;
    virtual void movement() override;

    void updateController();

    MR::Vector< MR::AssignableArray< ShadowController* > > _C;
    MR::Vector< MR::AssignableArray< ShadowController* > > _18;
    bool _24;
    f32 mFarClip;  // 0x28
};

class ShadowController {
public:
    ShadowController(LiveActor*, const char*);

    void requestCalc();
    void update();
    void updateFarClipping(f32);
    void updateDirection();
    void upateProjection();
    ShadowDrawer* getShadowDrawer();
    LiveActor* getHost() const;
    void setShadowDrawer(ShadowDrawer*);
    void getDropPos(TVec3f*) const;
    void getDropDir(TVec3f*) const;
    f32 getDropLength() const;
    void getProjectionPos(TVec3f*) const;
    void getProjectionNormal(TVec3f*) const;
    f32 getProjectionLength() const;
    bool isProjected() const;
    bool isDraw() const;
    bool isCalcCollision() const;
    bool isCalcGravity() const;
    bool isCalcShadowGravity() const;
    void setGroupName(const char*);
    void setDropPosPtr(const TVec3f*);
    void setDropPosMtxPtr(MtxPtr, const TVec3f&);
    void setDropPosFix(const TVec3f&);
    void setDropDirPtr(const TVec3f*);
    void setProjectionPtr(const TVec3f*, const TVec3f*);
    void setDropDirFix(const TVec3f&);
    void setDropLength(f32);
    void setDropStartOffset(f32);
    void setDropTypeNormal();
    void setDropTypeSurface();
    void setProjectionFix(const TVec3f&, const TVec3f&, bool);
    void onCalcCollision();
    void offCalcCollision();
    void onCalcCollisionOneTime();
    void onCalcDropGravity();
    void onCalcDropGravityOneTime();
    void offCalcDropGravity();
    void onCalcDropPrivateGravity();
    void onCalcDropPrivateGravityOneTime();
    void offCalcDropPrivateGravity();
    void setCollisionPartsFilter(CollisionPartsFilterBase*);
    void onFollowHostScale();
    void offFollowHostScale();
    bool isFollowHostScale() const;
    void onVisibleSyncHost();
    void offVisibleSyncHost();
    void validate();
    void invalidate();

    void updateProjection();

    inline void appendToHolder() { MR::getSceneObj< ShadowControllerHolder >(SceneObj_ShadowControllerHolder)->_18.push_back(this); }

    LiveActor* mActor;                                // 0x0
    const char* mName;                                // 0x4
    const char* mGroupName;                           // 0x8
    ShadowDrawer* mDrawer;                            // 0xC
    HitSensor* mProjectedSensor;                      // 0x10
    CollisionPartsFilterBase* mCollisionPartsFilter;  // 0x14
    MtxPtr _18;
    MtxPtr _1C;
    const TVec3f* mDropPos;  // 0x20
    const TVec3f* mDropDir;  // 0x24
    TVec3f* mProjPos;        // 0x28
    TVec3f* mProjNorm;       // 0x2C
    TVec3f _30;
    TVec3f _3C;
    TVec3f _48;
    TVec3f _54;
    u8 _60;
    u8 _61;
    u8 _62;
    u8 _63;
    u8 _64;
    u8 _65;
    u8 _66;
    u8 _67;
    f32 mStartOffset;  // 0x68
    f32 mDropLength;   // 0x6C
    u8 _70;
    u8 _71;
    u8 _72;
};

namespace MR {
    void addShadowController(ShadowController*);
    void requestCalcActorShadowAppear(LiveActor*);
    void requestCalcActorShadow(LiveActor*);
};  // namespace MR
