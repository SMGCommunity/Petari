#include "Game/LiveActor/DynamicJointCtrl.hpp"
#include "Game/Util.hpp"
#include <JSystem/J3DGraphAnimator/J3DJoint.hpp>

namespace {
    DynamicJointCtrlParam sDefaultParam = {0.3f, 0.98f, 0.01f, 45.0f, -1};
    const s32 sDefaultCtrlTime = 10;
    const char* sTextOutFileName = "DynamicJointCtrl";

    s32 calcJointChildNum(J3DJoint* pJoint) {
        if (pJoint->mChild == nullptr) {
            return 0;
        } else {
            return calcJointChildNum(pJoint->mChild) + 1;
        }
    }
}  // namespace

JointCtrlRate::JointCtrlRate() : _0(1.0f), _4(), _8(-1), _C(-1) {
}

void JointCtrlRate::update() {
    if (_8 >= 0) {
        _4--;
        if (_4 < 0) {
            _8 = -1;
            _0 = 1.0f;
        } else {
            _0 = static_cast< f32 >(_8 - _4) / static_cast< f32 >(_8);
        }
    } else if (_C >= 0) {
        _4--;
        if (_4 < 0) {
            _C = -1;
            _0 = 0.0f;
        } else {
            _0 = static_cast< f32 >(_4) / static_cast< f32 >(_C);
        }
    }
}

void JointCtrlRate::startCtrl(s32 time) {
    if (time < 0) {
        time = sDefaultCtrlTime;
    }

    _8 = time;
    _C = -1;
    _4 = time;
    _0 = 0.0f;
}

void JointCtrlRate::endCtrl(s32 time) {
    if (time < 0) {
        time = sDefaultCtrlTime;
    }

    _C = time;
    _8 = -1;
    _4 = time;
    _0 = 1.0f;
}

DynamicJointCtrlNode::DynamicJointCtrlNode(const DynamicJointCtrl* pControl, MtxPtr pMtx, f32 length)
    : _0(length), _4(0.0f, 0.0f, 0.0f), _10(0.0f, 0.0f, 0.0f), _1C(0.0f, 0.0f, 1.0f), _28(pMtx), mJointController(), mChild(),
      mParentControl(pControl) {
    if (pMtx) {
        _4.set< f32 >(pMtx[0][3], pMtx[1][3], pMtx[2][3]);
        _1C.set< f32 >(pMtx[0][0], pMtx[1][0], pMtx[2][0]);
    }
}

void DynamicJointCtrlNode::update(LiveActor* pActor, const DynamicJointCtrlNode* pParent) {
    _10.scale(mParentControl->mParams->mFriction);
    TVec3f gravity(0.0f, 0.0f, 0.0f);
    MR::calcGravityVector(pActor, &gravity, nullptr, 0);
    gravity.scale(mParentControl->mParams->mGravity);
    _10.add(gravity);

    TVec3f targetPos(pParent->_1C);
    targetPos.scale(_0);
    targetPos.add(pParent->_4);
    TVec3f accel(targetPos);
    accel.sub(_4);
    accel.sub(_10);
    accel.scale(mParentControl->mParams->mAccelRatetoBckPos);
    _10.add(accel);

    TVec3f offset(_4);
    offset.add(_10);
    offset.sub(pParent->_4);
    TVec3f direction(offset);

    if (MR::isNearZero(direction)) {
        direction.set< f32 >(pParent->_1C);
    } else {
        MR::normalize(&direction);
    }

    if (offset.length() >= _0) {
        TVec3f limitedOffset(direction);
        limitedOffset.scale(_0);
        TVec3f correction(offset);
        correction.sub(limitedOffset);
        _10.sub(correction);
    }

    _4.add(_10);
}

void DynamicJointCtrlNode::reset() {
    _4.set< f32 >(_28[0][3], _28[1][3], _28[2][3]);
    _10.zero();
}

bool DynamicJointCtrlNode::updateJointMtxCallBack(TPos3f* pMtx, const JointControllerInfo& rInfo) {
    TVec3f axis;
    pMtx->getXDir(axis);

    if (MR::isNearZero(axis)) {
        return false;
    }

    _1C.set< f32 >(axis);
    MR::normalize(&_1C);

    f32 rate = mParentControl->mControlRate->_0;
    if (rate == 0.0f) {
        return false;
    }

    TVec3f direction(mChild->_4);
    direction.sub(_4);

    if (MR::isNearZero(direction)) {
        return false;
    }

    MR::normalize(&direction);

    f32 blendRate = mParentControl->mControlRate->_0;
    MR::turnMtxToXDirDegree(pMtx, direction, mParentControl->mParams->mBendMaxDegree * blendRate);
    return true;
}

DynamicJointCtrl::DynamicJointCtrl(LiveActor* pActor, const char* pName, const DynamicJointCtrlParam* pParam)
    : mActor(pActor), mName(pName), _8(), mCtrlNodes(), mParams(pParam), mControlRate(new JointCtrlRate) {
}

void DynamicJointCtrl::init() {
    MR::getJointMtx(mActor, mName);
    J3DJoint* joint = MR::getJoint(mActor, mName);
    if (mParams->mCtrlJointNumMax > 0) {
        _8 = mParams->mCtrlJointNumMax;
    } else {
        _8 = calcJointChildNum(joint) + 2;
    }

    mCtrlNodes = new DynamicJointCtrlNode*[_8];
    TVec3f previousPos(0.0f, 0.0f, 0.0f);
    f32 length = 100.0f;

    for (s32 i = 0; i < _8; i++) {
        MtxPtr mtx = nullptr;
        if (i < _8 - 1) {
            mtx = MR::getJointMtx(mActor, joint->getJntNo());
        }

        if (i < _8 - 1) {
            TVec3f pos(mtx[0][3], mtx[1][3], mtx[2][3]);
            if (i > 0) {
                length = pos.distance(previousPos);
            }

            previousPos.set< f32 >(pos);
        }

        mCtrlNodes[i] = new DynamicJointCtrlNode(this, mtx, length);
        if (i < _8 - 1) {
            const char* name = MR::getJointName(mActor, joint->getJntNo());
            mCtrlNodes[i]->mJointController =
                MR::createJointDelegator< DynamicJointCtrlNode >(mCtrlNodes[i], mActor, &DynamicJointCtrlNode::updateJointMtxCallBack, nullptr, name);
            joint = joint->mChild;
        }
    }

    for (s32 i = 0; i < _8 - 1; i++) {
        mCtrlNodes[i]->mChild = mCtrlNodes[i + 1];
    }
}

void DynamicJointCtrl::update() {
    mCtrlNodes[0]->reset();

    for (s32 i = 1; i < _8; i++) {
        mCtrlNodes[i]->update(mActor, mCtrlNodes[i - 1]);
    }

    mControlRate->update();
}

void DynamicJointCtrl::reset() {
    for (s32 i = 0; i < _8 - 1; i++) {
        mCtrlNodes[i]->reset();
    }

    int endIndex = _8 - 1;
    int parentIndex = _8 - 2;
    mCtrlNodes[endIndex]->update(mActor, mCtrlNodes[parentIndex]);
}

void DynamicJointCtrl::setCallBackFunction() {
    for (s32 i = 0; i < _8 - 1; i++) {
        mCtrlNodes[i]->mJointController->registerCallBack();
    }
}

DynamicJointCtrlKeeper::DynamicJointCtrlKeeper(LiveActor* pActor) : mActor(pActor), _4(), mControls() {
    const ResourceHolder* resourceHolder = MR::getResourceHolder(pActor);
    JMapInfo* csv = MR::tryCreateCsvParser(resourceHolder, "%s.bcsv", sTextOutFileName);
    if (!csv) {
        return;
    }

    _4 = csv->getNumEntries();
    mControls = new DynamicJointCtrl*[_4];

    for (s32 i = 0; i < _4; i++) {
        DynamicJointCtrlParam* param = new DynamicJointCtrlParam;
        *param = sDefaultParam;
        const char* name = nullptr;
        csv->getValue(i, "JointName", &name);
        csv->getValue(i, "Gravity", &param->mGravity);
        csv->getValue(i, "Friction", &param->mFriction);
        csv->getValue(i, "AccelRateToBckPos", &param->mAccelRatetoBckPos);
        csv->getValue(i, "BendMaxDegree", &param->mBendMaxDegree);
        csv->getValue(i, "CtrlJointNumMax", &param->mCtrlJointNumMax);
        mControls[i] = new DynamicJointCtrl(pActor, name, param);
        mControls[i]->init();
    }
}

void DynamicJointCtrlKeeper::update() {
    for (s32 i = 0; i < _4; i++) {
        mControls[i]->update();
    }
}

void DynamicJointCtrlKeeper::setCallBackFunction() {
    for (s32 i = 0; i < _4; i++) {
        mControls[i]->setCallBackFunction();
    }
}

void DynamicJointCtrlKeeper::startCtrl(const char* pName, s32 time) {
    findJointCtrl(pName)->mControlRate->startCtrl(time);
}

void DynamicJointCtrlKeeper::endCtrl(const char* pName, s32 time) {
    findJointCtrl(pName)->mControlRate->endCtrl(time);
}

void DynamicJointCtrlKeeper::reset() {
    for (s32 i = 0; i < _4; i++) {
        mControls[i]->reset();
    }
}

DynamicJointCtrl* DynamicJointCtrlKeeper::findJointCtrl(const char* pName) {
    for (s32 i = 0; i < _4; i++) {
        if (MR::isEqualString(mControls[i]->mName, pName)) {
            return mControls[i];
        }
    }

    return mControls[0];
}
