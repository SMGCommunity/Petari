#include "Game/MapObj/BlackHole.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/ModelObj.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util.hpp"

void BlackHole_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)0.5f;
    (void)500.0f;
    (void)0.009999999776482582f;
    (void)1000.0f;
}

namespace NrvBlackHole {
    NEW_NERVE(BlackHoleNrvWait, BlackHole, Wait);
    NEW_NERVE(BlackHoleNrvDemo, BlackHole, Demo);
    NEW_NERVE(BlackHoleNrvDisappear, BlackHole, Disappear);
};  // namespace NrvBlackHole

BlackHole::BlackHole(const char* pName) : LiveActor(pName), mBlackHoleModel(), _90(gZeroVec), _9C(1.0f), _A0(500.0f), _A4(), mCameraInfo() {
    _A8.identity();
    _D8.identity();
}

void BlackHole::init(const JMapInfoIter& rIter) {
    initMapToolInfo(rIter);
    initModel();
    MR::connectToSceneMapObj(this);
    initHitSensor(1);
    MR::addHitSensorEye(this, "body", 16, _A0, TVec3f(0.0f, 0.0f, 0.0f));
    initEffectKeeper(0, nullptr, false);
    MR::setEffectHostMtx(this, "BlackHoleSuction", (MtxPtr)&_D8);
    f32 radius = _A0;
    f32 val = 500.0f * _9C;

    if (radius >= val) {
        radius = radius;
    } else {
        radius = val;
    }

    f32 clippingRadius = 500.0f + radius;
    MR::setClippingTypeSphere(this, clippingRadius);
    MR::setClippingTypeSphere(mBlackHoleModel, clippingRadius);
    MR::setClippingFarMax(this);
    MR::setClippingFarMax(mBlackHoleModel);
    initSound(4, false);

    if (MR::tryRegisterDemoCast(this, rIter)) {
        MR::registerDemoActionNerve(this, GET_NERVE(BlackHole, BlackHoleNrvDisappear), 0);
    }

    bool isCreated = MR::createActorCameraInfoIfExist(rIter, &mCameraInfo);

    if (isCreated) {
        MR::initActorCamera(this, rIter, &mCameraInfo);
    }

    initNerve(GET_NERVE(BlackHole, BlackHoleNrvWait));
    bool uses = MR::useStageSwitchReadAppear(this, rIter);

    if (uses) {
        MR::syncStageSwitchAppear(this);
        makeActorDead();
    } else {
        makeActorAppeared();
    }
}

void BlackHole::makeActorAppeared() {
    bool isOnSwitch = MR::isValidSwitchA(this) && MR::isOnSwitchA(this);

    if (!isOnSwitch) {
        LiveActor::makeActorAppeared();
        mBlackHoleModel->makeActorAppeared();
    }
}

void BlackHole::kill() {
    LiveActor::kill();
    mBlackHoleModel->kill();
}

bool BlackHole::tryStartDemoCamera() {
    if (mCameraInfo != nullptr) {
        MR::startActorCameraTargetSelf(this, mCameraInfo, -1);
        return true;
    }

    return false;
}

void BlackHole::calcAndSetBaseMtx() {
    LiveActor::calcAndSetBaseMtx();
    TVec3f dirToCam;
    dirToCam.sub(MR::getCamPos(), mPosition);
    TVec3f camYDir;
    camYDir.set(MR::getCamYdir());

    if (MR::normalizeOrZero(&dirToCam)) {
        return;
    }

    if (MR::isSameDirection(dirToCam, camYDir)) {
        return;
    }

    MR::makeMtxFrontUpPos(&_D8, dirToCam, camYDir, mPosition);

    _D8.scaleXYZ(mScale.x);
}

void BlackHole::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (!isNerve(GET_NERVE(BlackHole, BlackHoleNrvWait))) {
        return;
    }

    if (_A4 != nullptr && !isInCubeBox(pReceiver->mPosition)) {
        return;
    }

    if (!MR::sendArbitraryMsg(ACTMES_INHALE_BLACK_HOLE, pReceiver, pSender)) {
        return;
    }

    if (!MR::isSensorPlayer(pReceiver)) {
        return;
    }

    setNerve(GET_NERVE(BlackHole, BlackHoleNrvDemo));
}

void BlackHole::initMapToolInfo(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    MR::useStageSwitchReadA(this, rIter);
    MR::useStageSwitchReadAppear(this, rIter);

    if (MR::isEqualObjectName(rIter, "BlackHoleCube")) {
        initCubeBox();
        setName("ブラックホール[キューブ指定]");
    }

    if (_A4 == nullptr) {
        _A0 = 500.0f * mScale.x;
    } else {
        TVec3f size(mScale);
        size.scale(500.0f);

        _A0 = size.length();
    }

    f32 arg0;

    if (MR::getJMapInfoArg0NoInit(rIter, &arg0)) {
        _9C = arg0 / 1000.0f;
    } else if (_A4 == nullptr) {
        _9C = mScale.x;
    } else {
        _9C = 1.0f;
    }
}

void BlackHole::initModel() {
    initModelManagerWithAnm("BlackHoleRange", nullptr, false);
    mBlackHoleModel = MR::createModelObjMapObj("コアモデル", "BlackHole", getBaseMtx());
    mBlackHoleModel->makeActorDead();
    updateModelScale(_9C, _9C);
}

void BlackHole::initCubeBox() {
    MR::makeMtxRotate(_A8, mRotation.x, mRotation.y, mRotation.z);
    _A8.setTrans(mPosition);

    _A4 = new TBox3f();
    TVec3f vecStart(0.5f * (1000.0f * -mScale.x), 0.5f * (1000.0f * -mScale.y), 0.5f * (1000.0f * -mScale.z));
    TVec3f vecEnd(0.5f * (1000.0f * mScale.x), 0.5f * (1000.0f * mScale.y), 0.5f * (1000.0f * mScale.z));
    _A4->set(vecStart, vecEnd);
}

bool BlackHole::isInCubeBox(const TVec3f& rVec) const {
    TVec3f pos;
    _A8.multTranspose(rVec, pos);
    return _A4->intersectsPoint(pos);
}

void BlackHole::updateModelScale(f32 a1, f32 a2) {
    mScale.setAll< f32 >(a1);
    mBlackHoleModel->mScale.setAll< f32 >(0.5f * a2);
}

void BlackHole::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "BlackHoleRange");
        MR::startBtk(this, "BlackHoleRange");
        MR::startBtk(mBlackHoleModel, "BlackHole");
    }

    MR::startLevelSound(this, "SE_OJ_LV_BLACK_HOLE");
    bool isOnSwitch = false;

    if (MR::isValidSwitchA(this) && MR::isOnSwitchA(this)) {
        isOnSwitch = true;
    }

    if (isOnSwitch) {
        kill();
    }
}

void BlackHole::exeDemo() {
    MR::startLevelSound(this, "SE_OJ_LV_BLACK_HOLE");
}

void BlackHole::exeDisappear() {
    if (MR::isFirstStep(this)) {
        MR::startSound(this, "SE_OJ_BLACK_HOLE_DISAPPEAR");
    }

    f32 nerveEaseIn = MR::calcNerveEaseInValue(this, 90, _9C, 0.0f);
    f32 blackHoleEase = MR::calcNerveEaseInValue(this, 30, 90, _9C, 0.0f);
    mScale.setAll< f32 >(nerveEaseIn);
    mBlackHoleModel->mScale.setAll< f32 >(0.5f * blackHoleEase);

    if (MR::isStep(this, 90)) {
        kill();
    }
}

BlackHole::~BlackHole() {
}
