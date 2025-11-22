#pragma once

#include "Game/Util/JMapInfo.hpp"
#include <revolution/types.h>

class ActorLightCtrl;
class LiveActor;
class ModelObj;

class LodCtrl {
public:
    LodCtrl(LiveActor*, const JMapInfoIter&);

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
    void setViewCtrlPtr(const bool*, const bool*, const bool*, const bool*);
    void createLodModel(int, int, int);
    void syncMaterialAnimation();
    void syncJointAnimation();
    void initLightCtrl();
    ModelObj* initLodModel(int, int, int, bool) const;

    f32 _0;
    f32 _4;
    LiveActor* _8;
    LiveActor* mActor;  // 0xC
    ModelObj* _10;
    ModelObj* _14;
    u8 _18;
    u8 _19;
    u8 _1A;
    u8 _1B;
    const bool* _1C;
    const bool* _20;
    const bool* _24;
    const bool* _28;
    s16 mViewGroupID;                 // 0x2C
    ActorLightCtrl* mActorLightCtrl;  // 0x30
};

class LodCtrlFunction {
    static bool isExistLodLowModel(const char*);
};
