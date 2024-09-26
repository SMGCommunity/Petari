#include "Game/LiveActor/ClippingActorHolder.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Util.hpp"

ClippingActorHolder::ClippingActorHolder() : NameObj("クリッピングアクター保持") {
    _C = 0;
    _10 = 0;
    _14 = 0;
    _18 = 0;
    _1C = 0;
    mViewGroupCtrl = 0;

    _10 = new ClippingActorInfoList(sActorNumMax);
    _14 = new ClippingActorInfoList(sActorNumMax);
    _18 = new ClippingActorInfoList(sActorNumMax);
    _1C = new ClippingActorInfoList(sActorNumMax);
    mViewGroupCtrl = new ViewGroupCtrl();
}

void ClippingActorHolder::movement() {
    mViewGroupCtrl->update();

    for (s32 i = 0; i < _10->_4; i++) {
        _10->mClippingActorList[i]->judgeClipping();
    }
}

void ClippingActorHolder::registerActor(LiveActor *pActor) {
    ClippingActorInfo* inf = new ClippingActorInfo(pActor);
    _18->add(inf);
    _C++;
}

void ClippingActorHolder::initSystemInfo(LiveActor *pActor, const JMapInfoIter &rIter) {
    s32 viewGroupID = -1;
    bool res = MR::getJMapInfoViewGroupID(rIter, &viewGroupID);

    if (res) {
        mViewGroupCtrl->initActorInfo(find(pActor), viewGroupID);
    }
}

void ClippingActorHolder::initViewGroupTable() {
    mViewGroupCtrl->startInitViewGroupTable();
    mViewGroupCtrl->initViewGroup(_10);
    mViewGroupCtrl->initViewGroup(_18);
    mViewGroupCtrl->initViewGroup(_1C);
    mViewGroupCtrl->initViewGroup(_14);
    mViewGroupCtrl->endInitViewGroupTable();
}

void ClippingActorHolder::entryLodCtrl(LodCtrl *pLod, const JMapInfoIter &rIter) {
    mViewGroupCtrl->entryLodCtrl(pLod, rIter);
}

void ClippingActorHolder::validateClipping(LiveActor *pActor) {
    pActor->mFlags.mIsInvalidClipping = false;
    ClippingActorInfo* inf = _14->remove(pActor);

    if (MR::isDead(pActor)) {
        _18->add(inf);
    }
    else {
        if (inf->isGroupClipping()) {
            _10->add(inf);
        }
        else {
            _1C->add(inf);
        }  
    }
}

void ClippingActorHolder::invalidateClipping(LiveActor *pActor) {
    pActor->mFlags.mIsInvalidClipping = true;
    ClippingActorInfo* inf;

    if (MR::isDead(pActor)) {
        inf = _18->remove(pActor);
    }
    else {
        if (_10->isInList(pActor)) {
            inf = _10->remove(pActor);
        }
        else {
            inf = _1C->remove(pActor);
        }
    }

    _14->add(inf);
    if (MR::isClipped(pActor)) {
        pActor->endClipped();
    }
}

void ClippingActorHolder::setTypeToSphere(LiveActor *pActor, f32 range, const TVec3f *a3) {
    find(pActor)->setTypeToSphere(range, a3);
}

#ifdef NON_MATCHING
// cast issues
void ClippingActorHolder::setFarClipLevel(LiveActor *pActor, s32 level) {
    find(pActor)->mFarClipLevel = level;
}
#endif