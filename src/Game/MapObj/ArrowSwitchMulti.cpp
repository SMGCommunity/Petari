#include "Game/MapObj/ArrowSwitchMulti.hpp"
#include "Game/MapObj/ArrowSwitchMultiHolder.hpp"
#include "Game/Map/StageSwitch.hpp"
#include "math_types.hpp"

ArrowSwitchTarget::ArrowSwitchTarget(const char *pName) : NameObj(pName) {
    mJMapIDInfo = nullptr;
    mStageSwitchCtrl = nullptr;
    mTargetIdx = -1;
    MR::createArrowSwitchMultiHolder();
}

void ArrowSwitchTarget::init(const JMapInfoIter &rIter) {
    s32 arg;
    MR::getJMapInfoArg0WithInit(rIter, &arg);
    mJMapIDInfo = new JMapIdInfo(arg, rIter);
    MR::getJMapInfoArg1WithInit(rIter, &mTargetIdx);
    mStageSwitchCtrl = MR::createStageSwitchCtrl(this, rIter);
}

void ArrowSwitchTarget::initAfterPlacement() {
    MR::registerArrowSwitchTarget(this);
}

void ArrowSwitchTarget::onTarget() {
    if (mStageSwitchCtrl->isValidSwitchA()) {
        mStageSwitchCtrl->onSwitchA();
    }

    if (mStageSwitchCtrl->isValidSwitchB()) {
        mStageSwitchCtrl->offSwitchB();
    }
}

void ArrowSwitchTarget::offTarget() {
    if (mStageSwitchCtrl->isValidSwitchA()) {
        mStageSwitchCtrl->offSwitchA();
    }

    if (mStageSwitchCtrl->isValidSwitchB()) {
        mStageSwitchCtrl->onSwitchB();
    }
}


ArrowSwitchMulti::ArrowSwitchMulti(const char *pName) : LiveActor(pName) {
    mIDInfo = nullptr;
    _A0 = 0.0f;
    _A4 = 0.0f;
    _A8 = 0;
    _AC = 0;
    _B0 = 1;
    MR::createArrowSwitchMultiHolder();

    for (u32 i = 0; i < 4; i++) {
        mTargetArray[i] = nullptr;
    }
}

void ArrowSwitchMulti::registerTarget(ArrowSwitchTarget *pTarget) {
    mTargetArray[pTarget->mTargetIdx] = pTarget;
}

void ArrowSwitchMulti::init(const JMapInfoIter &rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("ArrowSwitch", nullptr, false);
    MR::connectToSceneNoSilhouettedMapObjStrongLight(this);
    s32 arg;
    MR::getJMapInfoArg0WithInit(rIter, &arg);
    mIDInfo = new JMapIdInfo(arg, rIter);

    if (MR::isInAreaObj("PlaneModeCube", mPosition)) {
        initHitSensor(1);
        MR::addHitSensorMapObj(this, "body", 8, 100.0f, TVec3f(0.0f, 50.0f, 250.0f));
    }
    else {
        initHitSensor(2);
        MR::addHitSensorMapObj(this, "body", 8, 100.0f, TVec3f(0.0f, 50.0f, 250.0f));
        MR::addHitSensorMapObjMoveCollision(this, "collision", 8, 0.0f, TVec3f(0.0f, 0.0f, 0.0f));
        MR::initCollisionParts(this, "ArrowSwitch", getSensor("collision"), nullptr);
    }

    initSound(4, false);
    MR::registerArrowSwitchMulti(this);
    initNerve(&NrvArrowSwitchMulti::ArrowSwitchMultiNrvWait::sInstance);
    makeActorAppeared();
}

void ArrowSwitchMulti::control() {

}

void ArrowSwitchMulti::calcAndSetBaseMtx() {
    LiveActor::calcAndSetBaseMtx();
    MtxPtr baseMtx = getBaseMtx();
    TPos3f pos;
    pos.setInline(baseMtx);
    TMtx34f tr_mtx;
    tr_mtx.identity();
    f32 v11 = _A0;
    v11 = v11 * PI_180;
    f32 v12 = sin(v11);
    f32 v13 = cos(v11);
    tr_mtx.mMtx[0][2] = v12;
    tr_mtx.mMtx[1][1] = 1.0f;
    tr_mtx.mMtx[0][0] = v13;
    tr_mtx.mMtx[2][0] = -v12;
    tr_mtx.mMtx[2][2] = v13;
    tr_mtx.mMtx[2][1] = 0.0f;
    tr_mtx.mMtx[1][2] = 0.0f;
    tr_mtx.mMtx[1][0] = 0.0f;
    tr_mtx.mMtx[0][1] = 0.0f;
    pos.concat(pos, tr_mtx);
    MR::setBaseTRMtx(this, pos);
}

bool ArrowSwitchMulti::receiveMsgPlayerAttack(u32 msg, HitSensor *a2, HitSensor *a3) {
    if (MR::isMsgPlayerHitAll(msg)) {
        return requestPunch(a2, a3);
    }   

    return false;
}

bool ArrowSwitchMulti::requestPunch(HitSensor *a1, HitSensor *a2) {
    if (getSensor("body") != a2) {
        return false;
    }

    if (!isNerve(&NrvArrowSwitchMulti::ArrowSwitchMultiNrvWait::sInstance)) {
        return false;
    }

    if (_B0) {
        _AC++;
        _A4 = 6.0f;
    }
    else {
        _AC--;
        _A4 = -6.0f;
    }

    _AC = (_AC + 4) % 4;
    MR::invalidateClipping(this);
    setNerve(&NrvArrowSwitchMulti::ArrowSwitchMultiNrvRotate::sInstance);
    return true; 
}

void ArrowSwitchMulti::exeWait() {
    if (MR::isFirstStep(this)) {
        if (_AC % 2 == 1) {
            MR::startBtk(this, "On");
        } 
        else {
            MR::startBtk(this, "Off");
        }

        MR::validateClipping(this);
    }
}

// ArrowSwitchMulti::exeRotate

ArrowSwitchMulti::~ArrowSwitchMulti() {

}

ArrowSwitchTarget::~ArrowSwitchTarget() {

}

namespace NrvArrowSwitchMulti {
    INIT_NERVE(ArrowSwitchMultiNrvWait);
    INIT_NERVE(ArrowSwitchMultiNrvRotate);

    void ArrowSwitchMultiNrvRotate::execute(Spine *pSpine) const {
        ArrowSwitchMulti* sw = reinterpret_cast<ArrowSwitchMulti*>(pSpine->mExecutor);
        sw->exeRotate();
    }

    void ArrowSwitchMultiNrvWait::execute(Spine *pSpine) const {
        ArrowSwitchMulti* sw = reinterpret_cast<ArrowSwitchMulti*>(pSpine->mExecutor);
        sw->exeWait();
    }
};