#include "Game/Boss/DinoPackunTailNode.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/JointController.hpp"
#include "Game/Util/JointUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"

void DinoPackunTailNode_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)0.5f;
    (void)3.0f;
    (void)3.1415927f;
    (void)2.0f;
    (void)90.0f;
    (void)10.0f;
    (void)0.001f;
    (void)0.2f;
    (void)5.0f;
}

typedef JointControlDelegator< DinoPackunTailNode > Delegator;

typedef bool (DinoPackunTailNode::*func)(TPos3f*, const JointControllerInfo&);

DinoPackunTailNode::DinoPackunTailNode(const char* pName, DinoPackun* pParent)
    : LiveActor(pName), mParent(pParent), mNodeDirection(0, 0, 0), _9C(0, 0, 0), _A8(0.0f, 0.0f, 0.0f), _B4(), _B8(), _BC(), _C0(), _C4(),
      mLinkLength(90.0f), mKeepBendPower(10.0f), _D0() {
}

void DinoPackunTailNode::createJointController(LiveActor* pHost, const char* pJointName) {
    _C4 = static_cast< Delegator* >(createJointControllerOwn(pHost, pJointName));
    JointController* cnt = MR::createJointDelegator(this, pHost, &DinoPackunTailNode::preCalcJoint, (func) nullptr, pJointName);
    _C0 = static_cast< Delegator* >(cnt);
    _BC = MR::getJointMtx(pHost, pJointName);
    resetJoint();
}

void DinoPackunTailNode::resetJoint() {
    TPos3f pos(_BC);
    pos.getXDir(mNodeDirection);
    pos.getTrans(mPosition);
    MR::zeroVelocity(this);
}

f32 DinoPackunTailNode::getLinkLength() const {
    return mLinkLength;
}

f32 DinoPackunTailNode::getKeepBendPower() const {
    return mKeepBendPower;
}

bool DinoPackunTailNode::preCalcJoint(TPos3f* pMtx, const JointControllerInfo& rInfo) {
    pMtx->getTrans(_9C);
    return false;
}

bool DinoPackunTailNode::turnJointLocalXDir(TPos3f* pMtx, const JointControllerInfo& rInfo) {
    if (!_B4 && !_B8) {
        return false;
    }

    TPos3f v25(pMtx);
    MR::orthogonalize(&v25);
    v25.getXDir(mNodeDirection);
    v25.getZDir(_A8);
    MR::normalize(&mNodeDirection);
    TQuat4f v24;
    v25.getQuat(v24);

    TVec3f v23;

    if (_B8) {
        TVec3f v22;
        v22.set(_B8->mPosition);
        v23.set(v22 - mPosition);
    } else {
        TVec3f v21;
        v21.set(_B4->mPosition);
        v23.set(mPosition - v21);
    }

    if (MR::isNearZero(v23)) {
        return false;
    }

    MR::normalize(&v23);
    MR::turnQuatXDirRad(&v24, v24, v23, M_PI);
    pMtx->setQuat(v24);
    pMtx->setTrans(mPosition);
    return true;
}

// https://decomp.me/scratch/i48pg
bool DinoPackunTailNode::calcJointScale(TPos3f* pMtx, const JointControllerInfo& rInfo) {
    if (!_B8) {
        return false;
    }

    TVec3f v12;
    v12.set(_B8->mPosition);
    f32 v6 = v12.distance(mPosition);
    f32 stretch = MR::clamp(v6 / mLinkLength, 0.2f, 5.0f);
    f32 shrink = MR::sqrt(1.0f / stretch);
    MR::preScaleMtx(*pMtx, stretch, shrink, shrink);
    return true;
}

void DinoPackunTailNode::registerPreCalcJointCallBack() {
    _C0->registerCallBack();
}

void DinoPackunTailNode::registerJointCallBack() {
    _C4->registerCallBack();
}

void DinoPackunTailNode::lockPosition() {
    if (!_D0) {
        requestLockPosition();
        _D0 = 1;
    }
}

void DinoPackunTailNode::unLockPosition() {
    if (_D0) {
        requestUnLockPosition();
        _D0 = 0;
    }
}

void DinoPackunTailNode::addNodeVelocityHost(const TVec3f& rVelocity) {
    if (!_D0) {
        addNodeVelocity(rVelocity);
    }
}

const TVec3f* DinoPackunTailNode::getNodeDirection() const {
    return &mNodeDirection;
}

void DinoPackunTailNode::addNodeVelocity(const TVec3f& rVel) {
    mVelocity.add(rVel);
}

void DinoPackunTailNode::requestLockPosition() {
}

void DinoPackunTailNode::requestUnLockPosition() {
}
