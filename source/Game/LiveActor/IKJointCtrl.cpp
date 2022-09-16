#include "Game/LiveActor/IKJointCtrl.h"
#include "Game/LiveActor/LiveActor.h"
#include "Game/Util.h"

#include <algorithm>

void IKJointCtrl::setEndPosition(const TVec3f &rPos, f32 a2) {
    TVec3f joint_pos;
    MR::copyJointPos(mActor, _B4, &joint_pos);
    MR::vecBlend(joint_pos, rPos, &joint_pos, a2);
    mJoint->update(MR::getJointMtx(mActor, _B0), MR::getJointMtx(mActor, _B2), MR::getJointMtx(mActor, _B4), joint_pos);
    _A8 = a2;
}

void IKJointCtrl::setEndDirection(const TVec3f &rDirection, f32 a2) {
    s32 val = MR::vecBlendSphere(_78, rDirection, &_78, a2);

    if (!val) {
        MR::turnRandomVector(&_78, _78, 0.0099999998f);
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

bool IKJointCtrl::updateRootJointCallBack(TPos3f *pPos, const JointControllerInfo &rInfo) {
    pPos->concat(mJoint->_30, _4);
    return true;
}

bool IKJointCtrl::updateMiddleJointCallBack(TPos3f *pPos, const JointControllerInfo &rInfo) {
    pPos->concat(mJoint->_60, _34);
    return true;
}
IKJointCtrlHolder::IKJointCtrlHolder(LiveActor *pActor, u32 count) {
    mControls = 0;
    mNumControls = 0;
    _8 = 0;
    mActor = pActor;
    _10 = 1;
    mControls = new IKJointCtrl*[count];
    mNumControls = count;
}

void IKJointCtrlHolder::addCtrl(const IKJointCtrlParam &rParam) {
    if (mNumControls > _8) {
        IKJointCtrl* ctrl = new IKJointCtrl(mActor);
        ctrl->init(rParam);
        u32 cnt = _8;
        _8 = cnt + 1;
        mControls[cnt] = ctrl;
    }
}

void IKJointCtrlHolder::setEndPosition(const char *pName, const TVec3f &rPos, f32 a3) {
    findIKJointCtrl(pName)->setEndPosition(rPos, a3);
}

void IKJointCtrlHolder::setEndDirection(const char *pName, const TVec3f &rDirection, f32 a3) {
    findIKJointCtrl(pName)->setEndDirection(rDirection, a3);
}

#ifdef NON_MATCHING
// for_each inlines
void IKJointCtrlHolder::endUpdate() {
    for_each(mControls, &mControls[mNumControls], mem_fun(&IKJointCtrl::endCtrl));
    _10 = 1;
}
#endif

IKJointCtrl* IKJointCtrlHolder::findIKJointCtrl(const char *pName) {
    for (u32 i = 0; i < mNumControls; i++) {
        const char* ctrlName = mControls[i]->mName;

        if (MR::isEqualString(pName, ctrlName)) {
            return mControls[i];
        }
    }
    
    return 0;
}

void IKJointCtrl::disableCallBack() {
    _B6 = 0;
}

void IKJointCtrl::enableCallBack() {
    _B6 = 1;
}