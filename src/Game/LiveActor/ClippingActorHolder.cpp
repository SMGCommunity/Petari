#include "Game/LiveActor/ClippingActorHolder.hpp"
#include "Game/LiveActor/ClippingActorInfo.hpp"
#include "Game/LiveActor/ClippingGroupHolder.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/ViewGroupCtrl.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"

namespace {
    static int sActorNumMax = 2560;
};  // namespace

ClippingActorHolder::ClippingActorHolder()
    : NameObj("クリッピングアクター保持"), _C(0), _10(nullptr), _14(nullptr), _18(nullptr), _1C(nullptr), mViewGroupCtrl(nullptr) {
    _10 = new ClippingActorInfoList(::sActorNumMax);
    _14 = new ClippingActorInfoList(::sActorNumMax);
    _18 = new ClippingActorInfoList(::sActorNumMax);
    _1C = new ClippingActorInfoList(::sActorNumMax);
    mViewGroupCtrl = new ViewGroupCtrl();
}

void ClippingActorHolder::movement() {
    mViewGroupCtrl->update();

    for (s32 i = 0; i < _10->_4; i++) {
        _10->mClippingActorList[i]->judgeClipping();
    }
}

void ClippingActorHolder::registerActor(LiveActor* pActor) {
    ClippingActorInfo* inf = new ClippingActorInfo(pActor);
    _18->add(inf);
    _C++;
}

void ClippingActorHolder::initSystemInfo(LiveActor* pActor, const JMapInfoIter& rIter) {
    s32 viewGroupID = -1;

    if (MR::getJMapInfoViewGroupID(rIter, &viewGroupID)) {
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

void ClippingActorHolder::entryLodCtrl(LodCtrl* pLod, const JMapInfoIter& rIter) {
    mViewGroupCtrl->entryLodCtrl(pLod, rIter);
}

void ClippingActorHolder::validateClipping(LiveActor* pActor) {
    pActor->mFlag.mIsInvalidClipping = false;
    ClippingActorInfo* inf = _14->remove(pActor);

    if (MR::isDead(pActor)) {
        _18->add(inf);
    } else {
        if (inf->isGroupClipping()) {
            _1C->add(inf);
        } else {
            _10->add(inf);
        }
    }
}

void ClippingActorHolder::invalidateClipping(LiveActor* pActor) {
    pActor->mFlag.mIsInvalidClipping = true;
    ClippingActorInfo* inf;

    if (MR::isDead(pActor)) {
        inf = _18->remove(pActor);
    } else {
        if (_10->isInList(pActor)) {
            inf = _10->remove(pActor);
        } else {
            inf = _1C->remove(pActor);
        }
    }

    _14->add(inf);
    if (MR::isClipped(pActor)) {
        pActor->endClipped();
    }
}

void ClippingActorHolder::addToClippingTarget(LiveActor* pActor) {
    if (MR::isInvalidClipping(pActor) || _10->isInList(pActor) || _1C->isInList(pActor)) {
        return;
    }
    ClippingActorInfo* pActorInfo = _18->remove(pActor);
    if (pActorInfo->isGroupClipping()) {
        _1C->add(pActorInfo);
        return;
    }
    _10->add(pActorInfo);
}

void ClippingActorHolder::removeFromClippingTarget(LiveActor* pActor) {
    if (!MR::isInvalidClipping(pActor) && !_18->isInList(pActor)) {
        ClippingActorInfo* pActorInfo;
        if (_1C->isInList(pActor)) {
            pActorInfo = _1C->remove(pActor);
        } else {
            pActorInfo = _10->remove(pActor);
        }
        _18->add(pActorInfo);
    }
}

ClippingActorInfo* ClippingActorHolder::startGroupClipping(LiveActor* pActor, const JMapInfoIter& rIter) {
    ClippingActorInfo* pActorInfo = nullptr;
    if (_10->isInList(pActor)) {
        pActorInfo = _10->remove(pActor);
        _1C->add(pActorInfo);
    } else if (_18->isInList(pActor)) {
        pActorInfo = _18->find(pActor, 0);
    } else if (_14->isInList(pActor)) {
        pActorInfo = _14->remove(pActor);
        _1C->add(pActorInfo);
        pActor->endClipped();
    }
    pActorInfo->setGroupClippingNo(rIter);
    return pActorInfo;
}

void ClippingActorHolder::setTypeToSphere(LiveActor* pActor, f32 range, const TVec3f* a3) {
    find(pActor)->setTypeToSphere(range, a3);
}

// cast issues
void ClippingActorHolder::setFarClipLevel(LiveActor* pActor, s32 level) {
    find(pActor)->mFarClipLevel = level;
}

ClippingActorInfo* ClippingActorHolder::find(const LiveActor* pActor) const {
    ClippingActorInfo* pActorInfo;

    pActorInfo = _10->findOrNone(pActor);

    if (pActorInfo != nullptr) {
        return pActorInfo;
    }

    pActorInfo = _18->findOrNone(pActor);

    if (pActorInfo != nullptr) {
        return pActorInfo;
    }

    pActorInfo = _1C->findOrNone(pActor);

    if (pActorInfo != nullptr) {
        return pActorInfo;
    }

    return _14->find(pActor, 0);
}

ClippingActorHolder::~ClippingActorHolder() {
}
