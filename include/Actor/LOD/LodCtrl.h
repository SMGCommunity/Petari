#pragma once

#include "Actor/Model/ModelObj.h"
#include "JMap/JMapInfoIter.h"

class ActorLightCtrl;

class LodCtrl
{
public:
    LodCtrl(LiveActor *, const JMapInfoIter &);

    void offSyncShadowHost();
    void appear();
    void kill();
    void validate();
    void invalidate();
    void update();
    bool isShowLowModel() const;
    void setDistanceToMiddle(f32);
    f32 calcDistanceToCamera() const;
    void setDistanceToLow(f32);
    void setDistanceToMiddleAndLow(f32, f32);
    void setClippingTypeSphereContainsModelBoundingBox(f32);
    void setFarClipping(f32);
    void invalidateClipping();
    void showHighModel();
    void showMiddleModel();
    void showLowModel();
    void hideAllModel();
    void setViewCtrlPtr(const bool *, const bool *, const bool *, const bool *);
    void createLodModel(s32, s32, s32);
    void syncMaterialAnimation();
    void syncJointAnimation();
    void initLightCtrl();
    ModelObj* initLodModel(s32, s32, s32, bool) const;

    f32 mDistToMiddle; // _0
    f32 mDistToLow; // _4
    ModelObj* mCurrentActiveObj; // _8
    LiveActor* mModelActor; // _C
    ModelObj* mModelObjMiddle; // _10
    ModelObj* mModelObjLow; // // _14
    bool mIsValid; // _18
    u8 _19;
    u8 mIsShadowVisible; // _1A
    u8 _1B;
    const bool* _1C;
    const bool* _20;
    const bool* _24;
    const bool* _28;
    s16 _2C;
    u16 _2E;
    ActorLightCtrl* mLightCtrl; // _30
};

class LodCtrlFunction
{
public:
    static bool isExistLodLowModel(const char *);
};

template<typename T>
void LodFuntionCall(LodCtrl *, void(*)(LiveActor *, f32), T);

void LodFuntionCall(LodCtrl *, void(*)(LiveActor *));

static const bool* val_zero = 0;