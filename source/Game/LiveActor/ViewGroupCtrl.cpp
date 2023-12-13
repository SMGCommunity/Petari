#include "Game/LiveActor/ViewGroupCtrl.hpp"
#include "Game/Util/Array.hpp"

ViewGroupCtrl::ViewGroupCtrl() {
    mViewCubeMgr = nullptr;
    mMaxViewGroupId = 0;
    mViewGroupData = nullptr;
    mViewCtrlCount = 0;
    mLodCtrls = nullptr;
    mLodCtrls = new LodCtrl*[0x100];

    for (u32 i = 0; i < 0x100; i++) {
        mLodCtrls[i] = nullptr;
    }
}

void ViewGroupCtrl::initActorInfo(ClippingActorInfo *pInfo, s32 groupID) {
    pInfo->_12 = groupID;

    if (groupID >= mMaxViewGroupId) {
        mMaxViewGroupId = groupID + 1;
    }
}

void ViewGroupCtrl::initViewGroup(ClippingActorInfoList *pList) {
    for (s32 i = 0; i < pList->_4; i++) {
        s32 id = pList->mClippingActorList[i]->_12;

        if (id < 0) {
            id = mMaxViewGroupId;
        }

        pList->mClippingActorList[i]->_14 = &mViewGroupData[id];
    }
}

void ViewGroupCtrl::endInitViewGroupTable() {

}

void ViewGroupCtrl::entryLodCtrl(LodCtrl *pCtrl, const JMapInfoIter &rIter) {
    s32 groupID = -1;
    if (MR::getJMapInfoViewGroupID(rIter, &groupID)) {
        pCtrl->mViewGroupID = groupID;
        mLodCtrls[mViewCtrlCount] = pCtrl;
        mViewCtrlCount++;
    }
}
