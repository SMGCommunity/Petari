#include "Game/Boss/DinoPackunTailNode.hpp"
#include "Game/Util/JointController.hpp"

typedef JointControlDelegator< DinoPackunTailNode > Delegator;

typedef bool (DinoPackunTailNode::*func)(TPos3f*, const JointControllerInfo&);

DinoPackunTailNode::DinoPackunTailNode(const char* pName, DinoPackun* pParent)
    : LiveActor(pName), mParent(pParent), mNodeDirection(0, 0, 0), _9C(0, 0, 0), _A8(0.0f, 0.0f, 0.0f), _B4(nullptr), _B8(nullptr), _BC(nullptr),
      _C0(nullptr), _C4(nullptr), mLinkLength(90.0f), mKeepBendPower(10.0f), _D0(0) {}

void DinoPackunTailNode::createJointController(LiveActor* pHost, const char* pJointName) {
    _C4 = static_cast< Delegator* >(createJointControllerOwn(pHost, pJointName));
    JointController* cnt = MR::createJointDelegator(this, pHost, &DinoPackunTailNode::preCalcJoint, (func) nullptr, pJointName);
    _C0 = static_cast< Delegator* >(cnt);
    _BC = MR::getJointMtx(pHost, pJointName);
    resetJoint();
}

void DinoPackunTailNode::resetJoint() {
    TPos3f pos;
    JMath::gekko_ps_copy12(pos, _BC);
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

// https://decomp.me/scratch/LGK18
bool DinoPackunTailNode::turnJointLocalXDir(TPos3f* pMtx, const JointControllerInfo& rInfo) {
    if (!_B4 && !_B8) {
        return false;
    }

    TPos3f v25;
    JMath::gekko_ps_copy12(&v25, pMtx);
    MR::orthogonalize(&v25);
    v25.getXDir(mNodeDirection);
    _A8.set< f32 >(v25(0, 2), v25(1, 2), v25(2, 2));
    MR::normalize(&mNodeDirection);
    TQuat4f v24;
    v25.getQuat(v24);

    TVec3f v23;

    if (_B8) {
        TVec3f v22;
        v22.set< f32 >(_B8->mPosition);
        v23.set< f32 >(v22 - mPosition);
    } else {
        TVec3f v21;
        v21.set< f32 >(_B4->mPosition);
        v23.set< f32 >(mPosition - v21);
    }

    if (MR::isNearZero(v23)) {
        return false;
    }

    MR::normalize(&v23);
    MR::turnQuatXDirRad(&v24, v24, v23, M_PI);
    pMtx->setRotateQuaternionInlineAndTrans(v24, mPosition);
    return true;
}

// https://decomp.me/scratch/i48pg
bool DinoPackunTailNode::calcJointScale(TPos3f* pMtx, const JointControllerInfo& rInfo) {
    if (!_B8) {
        return false;
    }

    TVec3f v12;
    v12.set< f32 >(_B8->mPosition);
    f32 v6 = PSVECDistance(&v12, &mPosition);
    f32 v7 = 0.2f;
    f32 v8 = (v6 / mLinkLength);

    if (v8 < 0.2f) {
        v8 = v8;
    } else {
        v7 = 5.0f;

        if (v8 > 5.0f) {
            v7 = v7;
        } else {
            v7 = (v6 / mLinkLength);
        }
    }

    f32 v9 = (1.0f / v7);
    f32 v11;
    if (v9 > 0.0f) {
        f32 v10 = __frsqrte(v9);
        v11 = ((-(((v10 * (1.0f / v7)) * v10) - 3.0f) * (v10 * (1.0f / v7))) * 0.5f);
    } else {
        v11 = (1.0f / v7);
    }

    MR::preScaleMtx(*pMtx, v7, v11, v11);
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
    mVelocity.addInline(rVel);
}

void DinoPackunTailNode::requestLockPosition() {}

void DinoPackunTailNode::requestUnLockPosition() {}
