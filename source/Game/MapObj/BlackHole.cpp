#include "Game/MapObj/BlackHole.hpp"
#include "Game/LiveActor/ModelObj.hpp"
#include "Game/Util.hpp"

// BlackHole::BlackHole

void BlackHole::init(const JMapInfoIter &rIter) {
    initMapToolInfo(rIter);
    initModel();
    MR::connectToSceneMapObj(this);
    initHitSensor(1);
    MR::addHitSensorEye(this, "body", 0x10, _A0, TVec3f(0.0f, 0.0f, 0.0f));
    initEffectKeeper(0, 0, false);
    MR::setEffectHostMtx(this, "BlackHoleSuction", (MtxPtr)&_D8);
    f32 radius = _A0;
    f32 val = 500.0f * _9C;
    if (radius >= val) {
        radius = radius;
    }
    else {
        radius = val;
    }

    f32 clippingRadius = 500.0f * radius;
    MR::setClippingTypeSphere(this, clippingRadius);
    MR::setClippingTypeSphere(mBlackHoleModel, clippingRadius);
    MR::setClippingFarMax(this);
    MR::setClippingFarMax(mBlackHoleModel);
    initSound(4, false);

    if (MR::tryRegisterDemoCast(this, rIter)) {
        MR::registerDemoActionNerve(this, &NrvBlackHole::BlackHoleNrvDisappear::sInstance, 0);
    }

    bool isCreated = MR::createActorCameraInfoIfExist(rIter, &mCameraInfo);
    if (isCreated) {
        MR::initActorCamera(this, rIter, &mCameraInfo);
    }

    initNerve(&NrvBlackHole::BlackHoleNrvWait::sInstance);
    bool uses = MR::useStageSwitchReadAppear(this, rIter);
    if (uses) {
        MR::syncStageSwitchAppear(this);
        makeActorDead();
    }
    else {
        makeActorAppeared();
    }
}

void BlackHole::makeActorAppeared() {
    bool isOnSwitch = false;
    if (MR::isValidSwitchA(this) && MR::isOnSwitchA(this)) {
        isOnSwitch = true;
    }

    if (isOnSwitch) {
        LiveActor::makeActorAppeared();
        mBlackHoleModel->makeActorAppeared();
    }
}

void BlackHole::kill() {
    LiveActor::kill();
    mBlackHoleModel->kill();
}

bool BlackHole::tryStartDemoCamera() {
    if (mCameraInfo) {
        MR::startActorCameraTargetSelf(this, mCameraInfo, -1);
        return true;
    }

    return false;
}

void BlackHole::attackSensor(HitSensor *a1, HitSensor *a2) {
    if (isNerve(&NrvBlackHole::BlackHoleNrvWait::sInstance)) {
        if (!_A4 || isInCubeBox(a2->mPosition)) {
            if (MR::sendArbitraryMsg(0x73, a2, a1)) {
                if (MR::isSensorPlayer(a2)) {
                    setNerve(&NrvBlackHole::BlackHoleNrvDemo::sInstance);
                }
            }
        }
    }
}

#ifdef NON_MATCHING
// shrug
void BlackHole::initMapToolInfo(const JMapInfoIter &rIter) {
    MR::initDefaultPos(this, rIter);
    MR::useStageSwitchReadA(this, rIter);
    MR::useStageSwitchReadAppear(this, rIter);

    if (MR::isEqualObjectName(rIter, "BlackHoleCube")) {
        initCubeBox();
        setName("ブラックホール[キューブ指定]");
    }

    if (_A4 == 0) {
        _A0 = 500.0f * mScale.z;
    }
    else {
        TVec3f stack_C;
        stack_C.setInlineXYPS(mScale);
        _A0 = PSVECMag(stack_C.toCVec());
    }

    f32 arg0;
    bool ret = MR::getJMapInfoArg0NoInit(rIter, &arg0);

    if (ret) {
        _9C = arg0 / 1000.0f;
    }
    else if (_A4) {
        _9C = 1.0f;
    }
    else {
        _9C = mScale.x;
    }
}
#endif

void BlackHole::initModel() {
    initModelManagerWithAnm("BlackHoleRange", 0, false);
    mBlackHoleModel = MR::createModelObjMapObj("コアモデル", "BlackHole", getBaseMtx());
    mBlackHoleModel->makeActorDead();
    updateModelScale(_9C, _9C);
}

#ifdef NON_MATCHING
void BlackHole::initCubeBox() {
    MR::makeMtxRotate((MtxPtr)&_A8, mRotation.x, mRotation.y, mRotation.z);
    _A8.mMtx[0][3] = mPosition.x;
    _A8.mMtx[1][3] = mPosition.y;
    _A8.mMtx[2][3] = mPosition.z;
    _A4 = new TBox3f();
    TVec3f stack_8(0.5f * (1000.0f * -mScale.x), 0.5f * (1000.0f * -mScale.y), 0.5f * (1000.0f * -mScale.z));
    TVec3f stack_14(0.5f * (1000.0f * mScale.x), 0.5f * (1000.0f, mScale.z),  0.5f * (1000.0f * mScale.y));
    _A4->mMin.set(stack_8);
    _A4->mMax.set(stack_14);
}
#endif

bool BlackHole::isInCubeBox(const TVec3f &rVec) const {
    TVec3f stack_8;
    _A8.multTranspose(rVec, stack_8);
    bool ret = false;
    TBox3f* box = _A4;
    if (stack_8.x >= box->mMin.x && stack_8.y >= box->mMin.y && stack_8.z >= box->mMin.z && stack_8.x < box->mMax.x && stack_8.y < box->mMax.y && stack_8.z < box->mMax.z) {
        ret = true;
    }

    return ret;
}

void BlackHole::updateModelScale(f32 a1, f32 a2) {
    mScale.setAll<f32>(a1);
    mBlackHoleModel->mScale.setAll<f32>(0.5f * a2);
}

void BlackHole::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "BlackHoleRange", 0);
        MR::startBtk(this, "BlackHoleRange");
        MR::startBtk(mBlackHoleModel, "BlackHole");
    }

    MR::startLevelSound(this, "SE_OJ_LV_BLACK_HOLE", -1, -1, -1);
    bool isOnSwitch = false;
    if (MR::isValidSwitchA(this) && MR::isOnSwitchA(this)) {
        isOnSwitch = true;
    }

    if (isOnSwitch) {
        kill();
    }
}

void BlackHole::exeDisappear() {
    if (MR::isFirstStep(this)) {
        MR::startSound(this, "SE_OJ_BLACK_HOLE_DISAPPEAR", -1, -1);
    }

    f32 nerveEaseIn = MR::calcNerveEaseInValue(this, 0x5A, _9C, 0.0f);
    f32 blackHoleEase = MR::calcNerveEaseInValue(this, 0x1E, 0x5A, _9C, 0.0f);
    mScale.setAll<f32>(nerveEaseIn);
    mBlackHoleModel->mScale.setAll<f32>(0.5f * blackHoleEase);

    if (MR::isStep(this, 0x5A)) { 
        kill();
    }
}

BlackHole::~BlackHole() {

}

namespace NrvBlackHole {
    BlackHoleNrvWait BlackHoleNrvWait::sInstance;
    BlackHoleNrvDemo BlackHoleNrvDemo::sInstance;
    BlackHoleNrvDisappear BlackHoleNrvDisappear::sInstance;

    void BlackHoleNrvDisappear::execute(Spine *pSpine) const {
        BlackHole* blackHole = reinterpret_cast<BlackHole*>(pSpine->mExecutor);
        blackHole->exeDisappear();
    }

    void BlackHoleNrvDemo::execute(Spine *pSpine) const {
        BlackHole* blackHole = reinterpret_cast<BlackHole*>(pSpine->mExecutor);
        MR::startLevelSound(blackHole, "SE_OJ_LV_BLACK_HOLE", -1, -1, -1);
    }

    void BlackHoleNrvWait::execute(Spine *pSpine) const {
        BlackHole* blackHole = reinterpret_cast<BlackHole*>(pSpine->mExecutor);
        blackHole->exeWait();
    }

};