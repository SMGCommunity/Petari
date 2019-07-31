#ifndef LODCTRL_H
#define LODCTRL_H

#include "Actor/LiveActor/LiveActor.h"
#include "JMap/JMapInfoIter.h"

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
    void calcDistanceToCamera() const;
    void setDistanceToLow(f32);
    void setDistanceToMiddleAndLow(f32, f32);

    f32 mDistToMiddle; // _0
    f32 mDistToLow; // _4
    u32 _8;
    const LiveActor* mActor; // _C
    u32 _10;
    u32 _14;
    bool mIsValid; // _18
    u8 _19;
    u8 _1A;
    u8 _1B;
    u8 _1C[0x34-0x1C];
};

#endif // LODCTRL_H