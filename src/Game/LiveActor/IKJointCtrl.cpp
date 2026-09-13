#include "Game/LiveActor/IKJointCtrl.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Util.hpp"
#include <JSystem/J3DGraphAnimator/J3DJoint.hpp>

#include <algorithm>

namespace {
    const char* sTextOutFileName = "IKJointCtrl";
}

void IKJointCtrl_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
}

IKJointCtrl::IKJointCtrl(LiveActor* pActor)
    : mName(), mActor(pActor), mJoint(), mRootCtrl(), mMiddleCtrl(), mEndCtrl(), _78(0.0f, 1.0f, 0.0f), mEndLocalDir(0.0f, 1.0f, 0.0f),
      _90(0.0f, 1.0f, 0.0f), mNearLimitRate(), mFarLimitRate(1.0f), mEndDirMaxDegree(45.0f), _A8(), _AC(), _B0(0xFFFF), _B2(0xFFFF), _B4(0xFFFF),
      _B6() {
    _4.identity();
    _34.identity();
}

void IKJointCtrl::init(const IKJointCtrlParam& rParam) {
    mName = rParam.mName;
    mNearLimitRate = rParam.mNearLimitRate;
    mFarLimitRate = rParam.mFarLimitRate;
    mEndLocalDir = rParam.mEndLocalDir;
    MR::normalize(&mEndLocalDir);
    mEndDirMaxDegree = rParam.mEndDirMaxDegree;

    J3DJoint* endJoint = MR::getJoint(mActor, mName);
    J3DJoint* middleJoint = MR::getParentJoint(mActor, endJoint);
    J3DJoint* rootJoint = MR::getParentJoint(mActor, middleJoint);
    mJoint = new IKJoint;
    _B0 = rootJoint->getJntNo();
    _B2 = middleJoint->getJntNo();
    _B4 = endJoint->getJntNo();
    mRootCtrl = MR::createJointController< IKJointCtrl >(this, mActor, _B0, &IKJointCtrl::updateRootJointCallBack, nullptr);
    mMiddleCtrl = MR::createJointController< IKJointCtrl >(this, mActor, _B2, &IKJointCtrl::updateMiddleJointCallBack, nullptr);
    mEndCtrl = MR::createJointController< IKJointCtrl >(this, mActor, _B4, &IKJointCtrl::updateEndJointCallBack, nullptr);
}

void IKJointCtrl::setEndPosition(const TVec3f& rPos, f32 a2) {
    TVec3f joint_pos;
    MR::copyJointPos(mActor, _B4, &joint_pos);
    MR::vecBlend(joint_pos, rPos, &joint_pos, a2);
    mJoint->update(MR::getJointMtx(mActor, _B0), MR::getJointMtx(mActor, _B2), MR::getJointMtx(mActor, _B4), joint_pos);
    _A8 = a2;
}

void IKJointCtrl::setEndDirection(const TVec3f& rDirection, f32 a2) {
    s32 val = MR::vecBlendSphere(_78, rDirection, &_78, a2);

    if (!val) {
        MR::turnRandomVector(&_78, _78, 0.01f);
    }

    _AC = a2;
}

void IKJointCtrl::endCtrl() {
    _A8 = 0.0f;
    _AC = 0.0f;
}

void IKJointCtrl::setCallBackFunction() {
    if (_B6) {
        if (_A8 > 0.0f) {
            mRootCtrl->registerCallBack();
            mMiddleCtrl->registerCallBack();
        }
    }

    if (_B6) {
        if (_AC > 0.0f) {
            mEndCtrl->registerCallBack();
        }
    }
}

bool IKJointCtrl::updateRootJointCallBack(TPos3f* pPos, const JointControllerInfo& rInfo) {
    pPos->concat(mJoint->_30, _4);
    return true;
}

bool IKJointCtrl::updateMiddleJointCallBack(TPos3f* pPos, const JointControllerInfo& rInfo) {
    pPos->concat(mJoint->_60, _34);
    return true;
}

bool IKJointCtrl::updateEndJointCallBack(TPos3f* pMtx, const JointControllerInfo& rInfo) {
    f32 angle = mEndDirMaxDegree * _AC;
    MR::turnMtxToLocalDirDegree(pMtx, mEndLocalDir, _78, angle);
    pMtx->mult33(mEndLocalDir, _90);
    return true;
}

IKJointCtrlHolder::IKJointCtrlHolder(LiveActor* pActor, u32 count) : mControls(), mNumControls(), _8(), mActor(pActor), _10(true) {
    mControls = new IKJointCtrl*[count];
    mNumControls = count;
}

void IKJointCtrlHolder::addCtrl(const IKJointCtrlParam& rParam) {
    if (mNumControls > _8) {
        IKJointCtrl* ctrl = new IKJointCtrl(mActor);
        ctrl->init(rParam);
        u32 cnt = _8;
        _8 = cnt + 1;
        mControls[cnt] = ctrl;
    }
}

void IKJointCtrlHolder::startUpdate() {
    _10 = false;
    std::for_each(mControls, mControls + _8, std::mem_fun(&IKJointCtrl::disableCallBack));
}

void IKJointCtrlHolder::setEndPosition(const char* pName, const TVec3f& rPos, f32 a3) {
    findIKJointCtrl(pName)->setEndPosition(rPos, a3);
}

void IKJointCtrlHolder::setEndDirection(const char* pName, const TVec3f& rDirection, f32 a3) {
    findIKJointCtrl(pName)->setEndDirection(rDirection, a3);
}

void IKJointCtrlHolder::endUpdate() {
    std::for_each(mControls, mControls + _8, std::mem_fun(&IKJointCtrl::enableCallBack));
    _10 = true;
}

void IKJointCtrlHolder::endCtrlAll() {
    std::for_each(mControls, mControls + _8, std::mem_fun(&IKJointCtrl::endCtrl));
}

void IKJointCtrlHolder::setCallBackFunction() {
    if (_10) {
        std::for_each(mControls, mControls + _8, std::mem_fun(&IKJointCtrl::setCallBackFunction));
    }
}

IKJointCtrl* IKJointCtrlHolder::findIKJointCtrl(const char* pName) {
    for (u32 i = 0; i < _8; i++) {
        const char* ctrlName = mControls[i]->mName;

        if (MR::isEqualString(pName, ctrlName)) {
            return mControls[i];
        }
    }

    return nullptr;
}

IKJointCtrlHolder* ActorJoint::createIKJointCtrlHolder(LiveActor* pActor) {
    const ResourceHolder* resourceHolder = MR::getResourceHolder(pActor);
    JMapInfo* csv = MR::tryCreateCsvParser(resourceHolder, "%s.bcsv", sTextOutFileName);
    if (!csv) {
        return nullptr;
    }

    s32 count = csv->getNumEntries();

    if (count <= 0) {
        return nullptr;
    }

    IKJointCtrlHolder* holder = new IKJointCtrlHolder(pActor, count);

    for (s32 i = 0; i < count; i++) {
        IKJointCtrlParam param;
        csv->getValue(i, "JointName", &param.mName);
        csv->getValue(i, "NearLimitRate", &param.mNearLimitRate);
        csv->getValue(i, "FarLimitRate", &param.mFarLimitRate);
        csv->getValue(i, "EndLocalDirX", &param.mEndLocalDir.x);
        csv->getValue(i, "EndLocalDirY", &param.mEndLocalDir.y);
        csv->getValue(i, "EndLocalDirZ", &param.mEndLocalDir.z);
        csv->getValue(i, "EndDirMaxDegree", &param.mEndDirMaxDegree);
        holder->addCtrl(param);
    }

    return holder;
}
