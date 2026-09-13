#pragma once

#include <revolution/types.h>

class AreaObjMgr;
class ClippingActorInfo;
class ClippingActorInfoList;
class JMapInfoIter;
class LodCtrl;

struct ViewGroupCtrlDataEntry {
    /* 0x00 */ bool _0;
    /* 0x01 */ bool _1;
    /* 0x02 */ bool _2;
    /* 0x03 */ bool _3;
    /* 0x04 */ bool _4;
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

    /* 0x00 */ AreaObjMgr* mViewCubeMgr;
    /* 0x04 */ s32 mMaxViewGroupId;
    /* 0x08 */ ViewGroupCtrlDataEntry* mViewGroupData;
    /* 0x0C */ s32 mViewCtrlCount;
    /* 0x10 */ LodCtrl** mLodCtrls;
};
