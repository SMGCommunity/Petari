#ifndef LODCTRL_H
#define LODCTRL_H

#include "Actor/LiveActor/LiveActor.h"
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

    void showHighModel();
    void showMiddleModel();
    void showLowModel();
    void hideAllModel();

    f32 mDistToMiddle; // _0
    f32 mDistToLow; // _4
    LiveActor* _8;
    LiveActor* _C;
    LiveActor* mModelObjMiddle; // ModelObj* (_10)
    LiveActor* mModelObjLow; // ModelObj* (_14)
    bool mIsValid; // _18
    u8 _19;
    u8 _1A;
    u8 _1B;
    const bool* _1C;
    const bool* _20;
    const bool* _24;
    const bool* _28;
    s16 _2C;
    u16 _2E;
    ActorLightCtrl* mLightCtrl; // _30
};

static const bool* val_zero = 0;

#endif // LODCTRL_H