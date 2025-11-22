#pragma once

#include "Game/LiveActor/ClippingActorInfo.hpp"
#include "Game/LiveActor/LodCtrl.hpp"

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

    void initActorInfo(ClippingActorInfo*, s32);
    void initViewGroup(ClippingActorInfoList*);
    void endInitViewGroupTable();
    void entryLodCtrl(LodCtrl*, const JMapInfoIter&);
    void update();
    void startInitViewGroupTable();

    AreaObjMgr* mViewCubeMgr;                // 0x0
    s32 mMaxViewGroupId;                     // 0x4
    ViewGroupCtrlDataEntry* mViewGroupData;  // 0x8
    u32 mViewCtrlCount;                      // 0xC
    LodCtrl** mLodCtrls;                     // 0x10
};