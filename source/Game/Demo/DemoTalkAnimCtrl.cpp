#include "Game/Demo/DemoTalkAnimCtrl.hpp"
#include <cstdio>
#include "Game/Demo/DemoExecutor.hpp"
#include "Game/Demo/DemoFunction.hpp"
#include "Game/System/NerveExecutor.hpp"
#include "Game/System/ResourceHolder.hpp"
#include "Game/System/ResourceInfo.hpp"
#include "Game/Util/ActorCameraUtil.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"

DemoTalkAnimCtrl::DemoTalkAnimCtrl(LiveActor* pActor, const char* a3, const char* a4)
    : NerveExecutor(a3), mActor(pActor), _C(a3), _10(a4), _14(nullptr), _18(""), _1C(""), mCameraInfo(nullptr), _24(0), _28(0), _2C(0), _30(0),
      _34(true), _35(false), _36(false), _38(0), _3C(0), _40(0), _44(0), _48(false), mHaveCamera(false), _4A(false), mHaveBtk(false), mHaveBpk(false),
      mHaveBtp(false), mHaveBrk(false), mHaveBva(false) {}

DemoTalkAnimCtrl::~DemoTalkAnimCtrl() {}

void DemoTalkAnimCtrl::initForScene(const char* a1, const char* a2, const JMapInfoIter& rIter) {
    char buf[0x100];
    snprintf(buf, sizeof(buf), "%s.canm", a2);
    initForActor(a1);
    _35 = true;
    _34 = _10 == NULL;
    mHaveCamera = MR::getResourceHolder(mActor)->mFileInfoTable->isExistRes(buf);
    _48 = MR::isExistBck(MR::getPlayerDemoActor(), a1);
    if (mHaveCamera) {
        _1C = a2;
        mCameraInfo = MR::createActorCameraInfo(rIter);
        MR::initAnimCamera(mActor, mCameraInfo, _1C);
    }
}

void DemoTalkAnimCtrl::updateCamera() {
    char buf[0x100];
    snprintf(buf, sizeof(buf), "%s%s", mActor->mName, _1C);
    if (DemoFunction::isPauseTimeKeepDemo(_C)) {
        MR::pauseOnAnimCamera(mCameraInfo, buf);
    } else {
        MR::pauseOffAnimCamera(mCameraInfo, buf);
    }
}

void DemoTalkAnimCtrl::createBckCtrlData(BckCtrlData* pBck, s32 totalSteps) const {
    s16 temp_r0 = MR::max(_24 - 1, 0);
    pBck->mLoopMode = 2;
    pBck->mStartFrame = temp_r0;
    pBck->mRepeatFrame = temp_r0 + 1;
    pBck->mInterpole = _38;
    pBck->mEndFrame = temp_r0 + totalSteps + 1;

    if (_36) {
        s32 var_r0 = DemoFunction::findDemoExecutor(_C)->getSubPartStep(_14);
        if (var_r0 > 0) {
            pBck->mRepeatFrame = temp_r0 + var_r0;
        }
    }
}

void DemoTalkAnimCtrl::initForActor(const char* a1) {
    _35 = false;
    _34 = true;
    mHaveCamera = false;
    _48 = false;
    _18 = a1;
    _14 = nullptr;
    _24 = 0;
    _28 = 0;
    _2C = 0;
    _30 = 0;
    _36 = false;
    mCameraInfo = nullptr;
    _1C = "";
    _4A = MR::isExistBck(mActor, a1);
    mHaveBtk = MR::isExistBtk(mActor, _18);
    mHaveBpk = MR::isExistBpk(mActor, _18);
    mHaveBtp = MR::isExistBtp(mActor, _18);
    mHaveBrk = MR::isExistBrk(mActor, _18);
    mHaveBva = MR::isExistBva(mActor, _18);
}

void DemoTalkAnimCtrl::startDemo() {
    _14 = nullptr;
    _24 = 0;
    _28 = 0;
    _2C = 0;
    _30 = 0;
    _36 = false;
    if (mHaveCamera && _34) {
        startCamera();
    }
}

bool DemoTalkAnimCtrl::updateDemo() {
    if (!_34) {
        return false;
    }

    const char* name = MR::getCurrentDemoPartNameMain(_C);
    if (_14 == nullptr) {
        startAnim();
        _14 = "";
    }

    bool ok = MR::isDemoPartFirstStep(name);
    if (MR::isEqualStringCase(_14, "")) {
        ok = MR::isDemoPartStep(name, 1);
    }

    if (ok) {
        s32 totalsteps = MR::getDemoPartTotalStep(name);
        BckCtrlData bck;
        _36 = MR::isDemoPartTalk(name);
        _14 = name;
        createBckCtrlData(&bck, totalsteps);
        updateAnim(bck);
        if (_48) {
            MR::startBckPlayer(_18, bck);
        }
        _28 = 0;
        _2C = 0;
        _24 += totalsteps;
    }

    if (_35 && _36) {
        updatePause();
    }
    if (mHaveCamera) {
        updateCamera();
    }

    if (DemoFunction::isDemoSuspendOrLastPartLastStep()) {
        if (mHaveCamera) {
            endCamera();
        }
        return false;
    } else {
        if (!DemoFunction::isPauseTimeKeepDemo(_C)) {
            _30 += 1;
            _28 += 1;
        }
        _2C += 1;
        return true;
    }
}

void DemoTalkAnimCtrl::startAnim() {
    if (_4A) {
        MR::startBckNoInterpole(mActor, _18);
    }
    if (mHaveBtk) {
        MR::startBtk(mActor, _18);
    }
    if (mHaveBpk) {
        MR::startBpk(mActor, _18);
    }
    if (mHaveBtp) {
        MR::startBtp(mActor, _18);
    }
    if (mHaveBrk) {
        MR::startBrk(mActor, _18);
    }
    if (mHaveBva) {
        MR::startBva(mActor, _18);
    }
}

void DemoTalkAnimCtrl::startCamera() {
    MR::startAnimCameraTargetSelf(mActor, mCameraInfo, _1C, _40, 1.0f);
}

void DemoTalkAnimCtrl::updateAnim(const BckCtrlData& rBck) {
    J3DFrameCtrl* ctrls[5];

    ctrls[0] = mHaveBtk ? MR::getBtkCtrl(mActor) : nullptr;
    ctrls[1] = mHaveBpk ? MR::getBpkCtrl(mActor) : nullptr;
    ctrls[2] = mHaveBtp ? MR::getBtpCtrl(mActor) : nullptr;
    ctrls[3] = mHaveBrk ? MR::getBrkCtrl(mActor) : nullptr;
    ctrls[4] = mHaveBva ? MR::getBvaCtrl(mActor) : nullptr;

    for (int i = 0; i < 5; i++) {
        J3DFrameCtrl* ctrl = ctrls[i];
        if (ctrl != nullptr) {
            ctrl->mLoopMode = rBck.mLoopMode;
            ctrl->mStartFrame = rBck.mStartFrame;
            ctrl->mCurrentFrame = (float)rBck.mStartFrame;
            ctrl->mLoopFrame = rBck.mRepeatFrame;
            ctrl->mCurrentFrame = (float)rBck.mStartFrame;
            ctrl->mEndFrame = rBck.mEndFrame;
            ctrl->mSpeed = 1.0f;
        }
    }

    if (_4A) {
        MR::startBck(mActor, _18, nullptr);
        MR::reflectBckCtrlData(mActor, rBck);
    }
}

void DemoTalkAnimCtrl::updatePause() {
    const char* name = MR::getCurrentDemoPartNameMain(_C);
    if (DemoFunction::isPauseTimeKeepDemo(_C)) {
        if (!MR::isSystemTalking()) {
            DemoFunction::resumeTimeKeepDemo(_C);
        }
    } else {
        if (MR::isDemoPartStep(name, MR::getDemoPartTotalStep(name) - 2)) {
            DemoFunction::pauseTimeKeepDemo(_C);
        }
    }
}

void DemoTalkAnimCtrl::endCamera() {
    MR::endAnimCamera(mActor, mCameraInfo, _1C, _44, true);
}

void DemoTalkAnimCtrl::setupStartDemoPart(const char* pPartName) {
    if (_10 != nullptr) {
        _34 = MR::isEqualStringCase(pPartName, _10);
    }
}
