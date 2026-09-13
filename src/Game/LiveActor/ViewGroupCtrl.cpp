#include "Game/LiveActor/ViewGroupCtrl.hpp"
#include "Game/AreaObj/AreaObj.hpp"
#include "Game/LiveActor/ClippingActorInfo.hpp"
#include "Game/LiveActor/LodCtrl.hpp"
#include "Game/Util/AreaObjUtil.hpp"
#include "Game/Util/Array.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"

namespace {
    // sLodCtrlNumMax
    const u32 sAllLodCtrlNumMax = 0x100;
}  // namespace

ViewGroupCtrl::ViewGroupCtrl() : mViewCubeMgr(), mMaxViewGroupId(), mViewGroupData(), mViewCtrlCount(), mLodCtrls() {
    mLodCtrls = new LodCtrl*[sAllLodCtrlNumMax];

    for (u32 i = 0; i < sAllLodCtrlNumMax; i++) {
        mLodCtrls[i] = nullptr;
    }
}

void ViewGroupCtrl::initActorInfo(ClippingActorInfo* pInfo, s32 groupID) {
    pInfo->_12 = groupID;

    if (groupID >= mMaxViewGroupId) {
        mMaxViewGroupId = groupID + 1;
    }
}

void ViewGroupCtrl::initViewGroup(ClippingActorInfoList* pList) {
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

void ViewGroupCtrl::entryLodCtrl(LodCtrl* pCtrl, const JMapInfoIter& rIter) {
    s32 groupID = -1;
    if (MR::getJMapInfoViewGroupID(rIter, &groupID)) {
        pCtrl->mViewGroupID = groupID;
        mLodCtrls[mViewCtrlCount] = pCtrl;
        mViewCtrlCount++;
    }
}

void ViewGroupCtrl::update() {
    ViewGroupCtrlDataEntry* entry;
    if (!mViewCubeMgr) {
        return;
    }

    for (s32 i = 0; i < mMaxViewGroupId; i++) {
        entry = &mViewGroupData[i];
        entry->_0 = false;
        entry->_1 = false;
        entry->_2 = false;
        entry->_3 = false;
        entry->_4 = false;
    }

    for (s32 i = 0; i < mViewCubeMgr->getNumAreaObj(); i++) {
        AreaObj* area = mViewCubeMgr->getAreaObj(i);
        if (area->isInVolume(*MR::getPlayerPos())) {
            ViewGroupCtrlDataEntry* entry = &mViewGroupData[area->mObjArg0];
            entry->_0 = true;
            if (area->mObjArg1 == 1) {
                entry->_1 = true;
            } else if (area->mObjArg1 == 2) {
                entry->_2 = true;
            } else if (area->mObjArg1 == 3) {
                entry->_3 = true;
            } else if (area->mObjArg1 == 4) {
                entry->_0 = false;
                entry->_4 = true;
            }
        }
    }
}

void ViewGroupCtrl::startInitViewGroupTable() {
    mViewCubeMgr = MR::getAreaObjManager("ViewGroupCtrlCube");
    if (mViewCubeMgr) {
        for (s32 i = 0; i < mViewCubeMgr->getNumAreaObj(); i++) {
            s32 groupID = mViewCubeMgr->getAreaObj(i)->mObjArg0;
            if (groupID >= mMaxViewGroupId) {
                mMaxViewGroupId = groupID + 1;
            }
        }
    }

    mViewGroupData = new ViewGroupCtrlDataEntry[mMaxViewGroupId + 1];
    for (s32 i = 0; i < mMaxViewGroupId + 1; i++) {
        ViewGroupCtrlDataEntry* entry = &mViewGroupData[i];
        entry->_0 = false;
        entry->_1 = false;
        entry->_2 = false;
        entry->_3 = false;
        entry->_4 = false;
    }

    for (s32 i = 0; i < mViewCtrlCount; i++) {
        LodCtrl* ctrl = mLodCtrls[i];
        s32 groupID = ctrl->mViewGroupID;
        if (groupID < 0) {
            groupID = mMaxViewGroupId;
        }

        ViewGroupCtrlDataEntry* entry = &mViewGroupData[groupID];
        ctrl->setViewCtrlPtr(&entry->_1, &entry->_2, &entry->_3, &entry->_4);
    }
}
