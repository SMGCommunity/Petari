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

    /* 0x00 */ f32 _0;
    /* 0x04 */ f32 _4;
    /* 0x08 */ LiveActor* _8;
    /* 0x0C */ LiveActor* mActor;
    /* 0x10 */ ModelObj* _10;
    /* 0x14 */ ModelObj* _14;
    /* 0x18 */ u8 _18;
    /* 0x19 */ u8 _19;
    /* 0x1A */ u8 _1A;
    /* 0x1B */ u8 _1B;
    /* 0x1C */ const bool* _1C;
    /* 0x20 */ const bool* _20;
    /* 0x24 */ const bool* _24;
    /* 0x28 */ const bool* _28;
    /* 0x2C */ s16 mViewGroupID;
    /* 0x30 */ ActorLightCtrl* mActorLightCtrl;
};

class LodCtrlFunction {
public:
    static bool isExistLodLowModel(const char*);
};
