#include "Game/Util/BaseMatrixFollowTargetHolder.hpp"

BaseMatrixFollower::BaseMatrixFollower(NameObj *pObj, const JMapInfoIter &rIter) {
    mFollowerObj = pObj;
    mLinkInfo = 0;
    mFollowTarget = 0;
    mFollowID = -1;
    MR::getJMapInfoFollowID(rIter, &mFollowID);
    mLinkInfo = new JMapLinkInfo(rIter, false);
}

NameObj* BaseMatrixFollower::getFollowTargetActor() const {
    return mFollowTarget->mActor;
}

/*
void BaseMatrixFollower::calcFollowMatrix(TPos3f *pOut) const {
    MtxPtr baseMtx = mFollowTarget->getHostBaseMtx();
    pOut->set((const TMtx34f)&baseMtx);
    pOut->concat((const TMtx34f)&pOut, mFollowTarget->_0);
}*/

bool BaseMatrixFollower::isEnableFollow() const {
    BaseMatrixFollowTarget* targ = mFollowTarget;

    if (targ == 0) {
        return false;
    }

    return targ->mActor;
}

bool BaseMatrixFollower::isValid() const {
    return mFollowTarget->isValid(mFollowID);
}

BaseMatrixFollowTarget::BaseMatrixFollowTarget(const JMapLinkInfo *pInfo) {
    mLinkInfo = pInfo;
    _30 = 0;
    mActor = 0;
    mValidater = 0;
    _0.identity();
}

void BaseMatrixFollowTarget::set(LiveActor *pActor, const TPos3f &a2, const TPos3f *a3, BaseMatrixFollowValidater *pValidator) {
    mValidater = pValidator;
    mActor = pActor;
    _0.set(a2);
    _0.invert(_0);

    if (a3) {
        _30 = a3;
    }
}

const MtxPtr BaseMatrixFollowTarget::getHostBaseMtx() const {
    if (!_30) {
        return mActor->getBaseMtx();
    }

    // MtxPtr and TPos3f are both 3 x 4 matricies, so this is fine
    return (const MtxPtr)_30;
}

bool BaseMatrixFollowTarget::isValid(s32 followId) const {
    if (mValidater) {
        return mValidater->isValid(followId);
    }

    return true;
}

BaseMatrixFollowTargetHolder::~BaseMatrixFollowTargetHolder() {

}