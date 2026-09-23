#include "Game/Util/BaseMatrixFollowTargetHolder.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Util/JMapLinkInfo.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/ObjUtil.hpp"

namespace {
    BaseMatrixFollowTargetHolder* getBaseMatrixFollowTargetHolder() {
        return MR::getSceneObj< BaseMatrixFollowTargetHolder >(SceneObj_BaseMatrixFollowTargetHolder);
    }
}  // namespace

BaseMatrixFollower::BaseMatrixFollower(NameObj* pObj, const JMapInfoIter& rIter) {
    mFollowerObj = pObj;
    mLinkInfo = nullptr;
    mFollowTarget = nullptr;
    mFollowID = -1;
    MR::getJMapInfoFollowID(rIter, &mFollowID);
    mLinkInfo = new JMapLinkInfo(rIter, false);
}

NameObj* BaseMatrixFollower::getFollowTargetActor() const {
    return mFollowTarget->mActor;
}

void BaseMatrixFollower::calcFollowMatrix(TPos3f* pOut) const {
    pOut->set(mFollowTarget->getHostBaseMtx());
    pOut->concat(*pOut, mFollowTarget->_0);
}

bool BaseMatrixFollower::isEnableFollow() const {
    BaseMatrixFollowTarget* pTarget = mFollowTarget;

    if (pTarget == nullptr) {
        return false;
    }

    return pTarget->mActor;
}

bool BaseMatrixFollower::isValid() const {
    return mFollowTarget->isValid(mFollowID);
}

BaseMatrixFollowTarget::BaseMatrixFollowTarget(const JMapLinkInfo* pInfo) {
    mLinkInfo = pInfo;
    _30 = nullptr;
    mActor = nullptr;
    mValidater = nullptr;
    _0.identity();
}

void BaseMatrixFollowTarget::set(LiveActor* pActor, const TPos3f& rMtx, const TPos3f* pBaseMtx, BaseMatrixFollowValidater* pValidator) {
    mValidater = pValidator;
    mActor = pActor;
    _0.set(rMtx.mMtx);
    _0.invert(_0);

    if (pBaseMtx) {
        _30 = pBaseMtx;
    }
}

const MtxPtr BaseMatrixFollowTarget::getHostBaseMtx() const {
    if (!_30) {
        return mActor->getBaseMtx();
    }

    return const_cast< MtxPtr >(_30->mMtx);
}

bool BaseMatrixFollowTarget::isValid(s32 followId) const {
    if (mValidater) {
        return mValidater->isValid(followId);
    }

    return true;
}

BaseMatrixFollowTargetHolder::BaseMatrixFollowTargetHolder(const char* pName, s32 targetCapacity, s32 followerCapacity) : NameObj(pName) {
    mTargets.init(targetCapacity);
    mFollowers.init(followerCapacity);
    MR::connectToSceneMapObjMovement(this);
}

template MR::Vector< MR::AssignableArray< BaseMatrixFollowTarget* > >::~Vector();
template MR::Vector< MR::AssignableArray< BaseMatrixFollower* > >::~Vector();

void BaseMatrixFollowTargetHolder::initAfterPlacement() {
    for (u32 i = 0; i < mFollowers.size(); i++) {
        BaseMatrixFollower* pFollower = mFollowers[i];
        BaseMatrixFollowTarget* pTarget = findFollowTarget(pFollower);
        if (pTarget->mActor) {
            pFollower->setGravityFollowHost(pTarget->mActor);
        }
    }
}

void BaseMatrixFollowTargetHolder::movement() {
    for (u32 i = 0; i < mFollowers.size(); i++) {
        if (mFollowers[i]->isEnableFollow()) {
            mFollowers[i]->update();
        }
    }
}

void BaseMatrixFollowTargetHolder::addFollower(BaseMatrixFollower* pFollower) {
    mFollowers.push_back(pFollower);

    BaseMatrixFollowTarget* pTarget = findFollowTarget(pFollower);
    if (!pTarget) {
        pTarget = new BaseMatrixFollowTarget(pFollower->mLinkInfo);
        mTargets.push_back(pTarget);
    }

    pFollower->mFollowTarget = pTarget;
}

void BaseMatrixFollowTargetHolder::setFollowTargetInfo(LiveActor* pActor, const JMapInfoIter& rIter, const TPos3f* pBaseMtx,
                                                       BaseMatrixFollowValidater* pValidater) {
    JMapLinkInfo linkInfo(rIter, true);
    BaseMatrixFollowTarget* pTarget = findFollowTarget(&linkInfo);
    if (pTarget) {
        TPos3f mtx;
        MR::getJMapInfoMatrixFromRT(rIter, &mtx);
        pTarget->set(pActor, mtx, pBaseMtx, pValidater);
    }
}

BaseMatrixFollowTarget* BaseMatrixFollowTargetHolder::findFollowTarget(const JMapLinkInfo* pLinkInfo) {
    if (!pLinkInfo) {
        return nullptr;
    }

    for (u32 i = 0; i < mTargets.size(); i++) {
        if (*mTargets[i]->mLinkInfo == *pLinkInfo) {
            return mTargets[i];
        }
    }

    return nullptr;
}

BaseMatrixFollowTarget* BaseMatrixFollowTargetHolder::findFollowTarget(const BaseMatrixFollower* pFollower) {
    if (pFollower == nullptr) {
        return nullptr;
    }

    return findFollowTarget(pFollower->mLinkInfo);
}

namespace MR {
    bool isValidFollowID(const JMapInfoIter& rIter) {
        if (!isValidInfo(rIter)) {
            return false;
        }

        JMapLinkInfo linkInfo(rIter, false);
        return linkInfo.isValid();
    }

    void addBaseMatrixFollower(BaseMatrixFollower* pFollower) {
        createSceneObj(SceneObj_BaseMatrixFollowTargetHolder);
        ::getBaseMatrixFollowTargetHolder()->addFollower(pFollower);
    }

    void addBaseMatrixFollowTarget(LiveActor* pActor, const JMapInfoIter& rIter, const TPos3f* pBaseMtx, BaseMatrixFollowValidater* pValidater) {
        if (isValidInfo(rIter) && isExistSceneObj(SceneObj_BaseMatrixFollowTargetHolder)) {
            ::getBaseMatrixFollowTargetHolder()->setFollowTargetInfo(pActor, rIter, pBaseMtx, pValidater);
        }
    }
}  // namespace MR

void BaseMatrixFollower::update() {
}

template MR::AssignableArray< BaseMatrixFollowTarget* >::~AssignableArray();
template MR::AssignableArray< BaseMatrixFollower* >::~AssignableArray();

BaseMatrixFollowTargetHolder::~BaseMatrixFollowTargetHolder() {
}
