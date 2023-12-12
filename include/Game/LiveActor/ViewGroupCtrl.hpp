#pragma once

#include "Game/LiveActor/ClippingActorInfo.h"
#include "Game/LiveActor/LodCtrl.h"

struct ViewGroupCtrlDataEntry {
    bool _0;
    bool _1;
    bool _2;
    bool _3;
    bool _4;
};

class ViewGroupCtrl {
public:
    ViewGroupCtrl();

    void initActorInfo(ClippingActorInfo *, s32);
    void initViewGroup(ClippingActorInfoList *);
    void endInitViewGroupTable();
    void entryLodCtrl(LodCtrl *, const JMapInfoIter &);
    void update();
    void startInitViewGroupTable();

    AreaObjMgr* mViewCubeMgr;               // _0
    s32 mMaxViewGroupId;                    // _4
    ViewGroupCtrlDataEntry* mViewGroupData; // _8
    u32 mViewCtrlCount;                     // _C
    LodCtrl** mLodCtrls;                    // _10
};