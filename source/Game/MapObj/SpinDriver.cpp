#include "Game/MapObj/SpinDriver.hpp"
#include "Game/MapObj/SpinDriverUtil.hpp"
#include <cstdio>
#include <cstring>
#include "math_types.hpp"

SpinDriver::SpinDriver(const char *pName) : LiveActor(pName),
    _8C(nullptr), mShootPath(nullptr), mSpinDriverCamera(nullptr), _98(0, 0, 0, 1), _A8(0, 0, 0, 1), 
    _B8(0, 0, 0), _C4(0, 0, 0), _D0(0, 0, 1), _DC(0, 0, 0), _E8(0, 1, 0), _F4(0, 0, 0), _104(0.0f), _100(40.0f), _108(0.0f), _10C(0, 0, 0) {
    
    _11C = 0.0f;
    _120 = -1.0f;
    _124 = 300;
    _128 = -1;
    _12C = 240;
    _130 = 270;
    _134 = 0.0;
    _138 = 1;
    _13C = 0;
    _140 = 0;
    _141 = 1;
    _142 = 0;
}

void SpinDriver::init(const JMapInfoIter &rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("SpinDriver", nullptr, false);
    MR::connectToSceneNoSilhouettedMapObjStrongLight(this);
    initShootPath(rIter);
    MR::offCalcGravity(this);
    TVec3f gravityVector;
    MR::calcGravityVectorOrZero(this, &gravityVector, nullptr, 0);
    if (MR::isNearZero(gravityVector, 0.001f)) {
        gravityVector.set(0.0f, -1.0f, 0.0f);
    }

    mGravity.set(gravityVector);
    initHitSensor(1);
    TVec3f sensorOffs;
    sensorOffs.x = 0.0f;
    sensorOffs.y = 0.0f;
    sensorOffs.z = 0.0f;
    MR::addHitSensor(this, "body", 0x64, 0x10, 300.0f, sensorOffs);
    initEffectKeeper(0, nullptr, false);
    initSound(6, false);
    initEventCamera(rIter);
    initNerve(&NrvSpinDriver::SpinDriverNrvWait::sInstance);
    MR::initShadowVolumeFlatModel(this, "SpinDriverShadow", MR::getJointMtx(this, "SpinDriver"));
    initParamFromJMapInfo(rIter);
    MR::setClippingFar200m(this);
    
    if (MR::useStageSwitchReadAppear(this, rIter)) {
        MR::syncStageSwitchAppear(this);
        makeActorDead();
        mSpinDriverCamera->initAppearCamera(rIter, this);
        
    }
    else {
        makeActorAppeared();
    }

    MR::useStageSwitchSleep(this, rIter);
}

void SpinDriver::initAfterPlacement() {
    SpinDriverUtil::setShadowAndClipping(this, &_10C, _120, 300.0f, &_118);
}

void SpinDriver::initParamFromJMapInfo(const JMapInfoIter &rIter) {
    if (!mShootPath) {
        MR::getJMapInfoArg0NoInit(rIter, &_100);
    }
    else {
        MR::getJMapInfoArg0NoInit(rIter, &_124);
    }

    _120 = -1.0f;
    MR::getJMapInfoArg1NoInit(rIter, &_120);
    s32 arg2 = 0;
    if (MR::getJMapInfoArg2NoInit(rIter, &arg2)) {
        _138 = arg2 != 0;
    }

    s32 arg4 = 0;
    if (MR::getJMapInfoArg4NoInit(rIter, &arg4)) {
        _142 = !(arg4 - 1); 
    }
}

void SpinDriver::initShootPath(const JMapInfoIter &rIter) {
    if (MR::isConnectedWithRail(rIter)) {
        mShootPath = new SpinDriverShootPath();
        _134 = 0.0f;
        MR::getJMapInfoArg5NoInit(rIter, &_134);
        mShootPath->initUsingParabolic(rIter, mPosition);
        mShootPath->calcInitPose(&_E8, &_D0, &_DC, _134);
    }
    else {
        Mtx mtx;
        MR::makeMtxRotate(mtx, mRotation);
        f32 z = mtx[2][0];
        f32 y = mtx[1][0];
        f32 x = mtx[0][0];
        _DC.set(x, y, z);
        z = mtx[2][1];
        y = mtx[1][1];
        x = mtx[0][1];
        _E8.set(x, y, z);
        z = mtx[2][2];
        y = mtx[1][2];
        x = mtx[0][2];
        _D0.set(x, y, z);
    }
}

void SpinDriver::initEventCamera(const JMapInfoIter &rIter) {
    mSpinDriverCamera = new SpinDriverCamera();
    mSpinDriverCamera->init(rIter, this);
    MR::isConnectedWithRail(rIter);
}

void SpinDriver::appear() {
    LiveActor::appear();
    MR::invalidateClipping(this);
    if (mSpinDriverCamera->isUseAppearCamera(this)) {
        MR::requestStartDemo(this, "出現", &NrvSpinDriver::SpinDriverNrvAppear::sInstance, &NrvSpinDriver::SpinDriverNrvTryDemo::sInstance);
    }
    else {
        setNerve(&NrvSpinDriver::SpinDriverNrvAppear::sInstance);
    }
}

void SpinDriver::makeActorAppeared() {
    LiveActor::makeActorAppeared();
    setNerve(&NrvSpinDriver::SpinDriverNrvWait::sInstance);
}

void SpinDriver::makeActorDead() {
    if (_8C) {
        cancelBind();
    }

    LiveActor::makeActorDead();
}

void SpinDriver::control() {
    if (!_141) {
        TVec3f* pos = MR::getPlayerPos();
        if (mPosition.squared(*pos) > 122500.0f) {
            _141 = 1;
        }
    }

    if (_13C) {
        _13C--;
    }

    f32 v4 = _104 + _108;
    f32 v5 = ((_104 + _108) - -PI);
    _140 = _13C > 0;
    _104 = v4;
    f32 v6 = fmod((TWO_PI + v5), TWO_PI_D);
    _104 = -PI + v6;
    _108 *= 0.94999999f;
}

void SpinDriver::calcAndSetBaseMtx() {
    TPos3f position;
    MR::makeMtxUpFrontPos(&position, _E8, _D0, mPosition);
    TMtx34f mtx;
    mtx.identity();
    f32 var = _104;
    f32 _sin = sin(var);
    f32 _cos = cos(var);
    mtx.mMtx[0][2] = _sin;
    mtx.mMtx[1][1] = 1.0f;
    mtx.mMtx[0][0] = _cos;
    mtx.mMtx[2][0] = -_sin;
    mtx.mMtx[2][2] = mtx.mMtx[0][0];
    mtx.mMtx[2][1] = 0.0f;
    mtx.mMtx[1][2] = 0.0f;
    mtx.mMtx[1][0] = 0.0f;
    mtx.mMtx[0][1] = 0.0f;
    position.concat(position, mtx);
    MR::setBaseTRMtx(this, position);
}

bool SpinDriver::receiveOtherMsg(u32 msg, HitSensor *a2, HitSensor *a3) {
    if (msg == 152) {
        _13C = 60;
        return canBind(a2);
    }

    if (msg == 146) {
        _13C = 60;
        if (startBind(a2)) {
            return true;
        }
    }

    if (msg == 147) {
        if (isNerve(&NrvSpinDriver::SpinDriverNrvShootStart::sInstance)) {
            return false;
        }

        if (_8C) {
            _8C = 0;
            cancelCamera();
            return true;
        }
    }

    if (msg == 148 && _8C) {
        _8C = 0;
        cancelCamera();
        return true;
    }

    if (msg == 161 && _8C) {
        calcBindActorMatrix();
        return true;
    }

    return false;
}

bool SpinDriver::tryStartShoot() {
    bool isSwingOrPointed = MR::isPadSwing(0) || MR::isPlayerPointedBy2POnTriggerButton();

    if (isSwingOrPointed) {
        MR::startSound(_8C, "SE_PM_SPIN_ATTACK", -1, -1);
        if (MR::isInAreaObj("Water", mPosition)) {
            MR::startSound(this, "SE_PMSPIN_DRV_IN_WATER_1", -1, -1);
        }

        setNerve(&NrvSpinDriver::SpinDriverNrvShootStart::sInstance);
        return true;
    }

    return false;
}

bool SpinDriver::tryEndCapture() {
    if (MR::isGreaterStep(this, 40) && PSVECDistance(_B8.toCVec(), mPosition.toCVec()) < 15.0f) {
        cancelBind();
        _141 = 0;
        setNerve(&NrvSpinDriver::SpinDriverNrvWait::sInstance);
        return true;
    }

    return false;
}

bool SpinDriver::tryShoot() {
    if (MR::isGreaterStep(this, 20)) {
        setNerve(&NrvSpinDriver::SpinDriverNrvShoot::sInstance);
        return true;
    }

    return false;
}

bool SpinDriver::tryEndShoot() {
    if (!mShootPath || MR::isGreaterStep(this, _124)) {
        MR::endBindAndSpinDriverJump(this, _C4);
        _8C = 0;
        endCamera();
        setNerve(&NrvSpinDriver::SpinDriverNrvCoolDown::sInstance);
        return true;
    }

    return false;
}

bool SpinDriver::tryForceCancel() {
    if (!_8C) {
        setNerve(&NrvSpinDriver::SpinDriverNrvCoolDown::sInstance);
        return true;
    }

    return false;
}

bool SpinDriver::tryEndCoolDown() {
    if (MR::isGreaterStep(this, 60)) {
        setNerve(&NrvSpinDriver::SpinDriverNrvWait::sInstance);
        return true;
    }

    return false;
}

bool SpinDriver::trySwitchOff() {
    if (MR::isValidSwitchAppear(this) && !MR::isOnSwitchAppear(this)) {
        kill();
        return true;
    }
    
    return false;
}

void SpinDriver::exeNonActive() {
    if (MR::isFirstStep(this)) {
        MR::validateClipping(this);
        MR::startBck(this, "Wait", nullptr);
        MR::startBpk(this, "Wait");
    }

    bool isSwingOrPointed = MR::isPadSwing(0) || MR::isPlayerPointedBy2POnTriggerButton();

    if (isSwingOrPointed) {
        f32 v3 = _108;
        _108 += 0.050000001f;
        if (v3 > 0.15000001f) {
            _108 = 0.15000001f;
        }
    }
}

void SpinDriver::exeAppear() {
    if (MR::isFirstStep(this)) {
        if (!_142) {
            MR::startSystemSE("SE_SY_SPIN_DRIVER_APPEAR", -1, -1);
            MR::startSound(this, "SE_OJ_SPIN_DRV_APPEAR", -1, -1);
        }

        MR::startBck(this, "Appear", nullptr);
        mSpinDriverCamera->startAppearCamera(this, _E8, _D0, mPosition);
    }

    if (MR::isBckStopped(this)) {
        if (MR::isGreaterStep(this, mSpinDriverCamera->getAppearCameraFrames())) {
            setNerve(&NrvSpinDriver::SpinDriverNrvWait::sInstance);

            if (mSpinDriverCamera->isUseAppearCamera(this)) {
                MR::endDemoWaitCameraInterpolating(this, "出現");
                mSpinDriverCamera->endAppearCamera(this);
            }
        }
    }
}

void SpinDriver::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::validateClipping(this);
        MR::startBck(this, "Wait", nullptr);
        MR::startBpk(this, "Wait");
    }

    bool isSwingOrPointed = MR::isPadSwing(0) || MR::isPlayerPointedBy2POnTriggerButton();

    if (isSwingOrPointed) {
        _108 += 0.050000001f;
    }

    if (_13C > 0) {
        _108 += 0.0080000004f;
        MR::startLevelSound(this, "SE_OJ_LV_SPIN_DRV_SHINE", -1, -1, -1);
        if (!_140) {
            MR::emitEffect(this, "SpinDriverLight");
            MR::startCSSound("CS_SPIN_BIND", nullptr, 0);
        }
    }
    else {
        MR::deleteEffect(this, "SpinDriverLight");
    }

    if (trySwitchOff()) {
        return;
    }
}

void SpinDriver::exeCapture() {
    if (tryForceCancel()) {
        MR::deleteEffect(this, "SpinDriverLight");
    }
    else {
        if (MR::isFirstStep(this)) {
            MR::startBckPlayer("SpinDriverWait", "SpinDriverCapture");
            MR::startBck(this, "Active", nullptr);
            MR::emitEffect(this, "SpinDriverLight");
        }

        MR::startLevelSound(this, "SE_OJ_LV_SPIN_DRV_SHINE", -1, -1, -1);
        MR::startLevelSound(this, "SE_OJ_LV_SPIN_DRV_CAPTURE", -1, -1, -1);
        moveBindPosToCenter();
        _11C = MR::calcNerveRate(this, 40);
        updateBindActorMatrix(_11C);
        _108 += 0.0080000004f;
        MR::tryRumblePadWeak(this, 0);
        _13C = 60;

        if (!tryStartShoot()) {
            if (tryEndCapture()) {
                return;
            }
        }
    }
}

void SpinDriver::exeShootStart() {
    if (!tryForceCancel()) {
        if (MR::isFirstStep(this)) {
            _F4 = _B8;
            MR::deleteEffect(this, "SpinDriverLight");
            MR::emitEffect(this, "SpinDriverStart");
            MR::startBckPlayer("Spin", "SpinDriverShoot");
            MR::startBck(this, "Active", nullptr);
            MR::startBpk(this, "Active");
            startCamera();
        }

        if (MR::isStep(this, 4)) {
            MR::startCSSound("CS_SPIN_DRIVE_SHORT", "SE_SY_CS_SPIN_DRV_START", 0);
        }

        f32 clamp = MR::clamp(getNerveStep() / 20.0f, 0.0f, 1.0f);
        f32 v3 = ((2.0f * (clamp - 0.5f)) * (2.0f * (clamp - 0.5f)));
        f32 dot = v3 * v3;
        TVec3f upVec;
        MR::calcUpVec(&upVec, this);
        TVec3f stack_50;
        stack_50.set(mPosition - MR::multVec(MR::multVec(upVec, 200.0f), 1.0f - dot));
        f32 v5 = MR::clamp((2.0f * clamp), 0.0f, 1.0f);
        _B8.set(MR::multAndAddVec(stack_50, _F4, v5, 1.0f - v5));
        updateBindActorMatrix((v5 + (_11C * (1.0f - v5))));
        _108 += 0.039999999f;
        MR::tryRumblePadMiddle(this, 0);
        if (tryShoot()) { 
            return;
        } 
    }
}

void SpinDriver::exeShoot() {
    if (!tryForceCancel()) {
        if (MR::isFirstStep(this)) {
            calcShootMotionTime();

            if (MR::hasME()) {
                MR::startSystemME("ME_MAGIC");
            }
            else {
                MR::startSystemSE("SE_SY_S_SPIN_DRV_ME_ALT", -1, -1);
            }

            MR::startSound(this, "SE_OJ_SPIN_DRV_JUMP", -1, -1);
            MR::startSound(_8C, "SE_PV_JUMP_JOY", -1, -1);

            if (MR::isInAreaObj("Water", mPosition)) {
                MR::startSound(this, "SE_PM_SIN_DIV_IN_WATER_2", -1, -1);
            }

            MR::startBckPlayer("SpaceFlyShortStart", "SpinDriverFlyStart");
            MR::shakeCameraNormal();
            MR::tryRumblePadVeryStrong(this, 0);
        }
        
        if (mShootPath) {
            updateBindPosition();
            updateCamera();
            calcParabolicBindPose();

            if (MR::isStep(this, _128)) {
                MR::startBckPlayer("SpaceFlyShort", "SpinDriverFlyLoop");
            }

            if (MR::isStep(this, _12C)) {
                MR::startBckPlayer("SpaceFlyEnd", "SpinDriverFlyEnd");
            }

            if (MR::isLessStep(this, _130)) {
                MR::startLevelSound(_8C, "SE_PM_LV_SPIN_DRV_FLY", -1, -1, -1);
            }

            if (MR::isStep(this, _130)) {
                MR::startBckPlayer("Fall", "SpinDriverFall");
            }
        }
        else {
            f32 v3 = _100;
            TVec3f v5(_E8);
            v5 *= v3;
            _C4 = v5;
        }

        if (tryEndShoot()) {
            return;
        }
    }
}

void SpinDriver::exeCoolDown() {
    if (MR::isFirstStep(this)) {
        MR::startBpk(this, "Wait");
    }

    if (!tryEndCoolDown()) {
        if (trySwitchOff()) {
            return;
        }
    }
}

bool SpinDriver::startBind(HitSensor *pSensor) {
    if (!canStartBind()) {
        return false;
    }

    bool isSwingOrPointed = MR::isPadSwing(0) || MR::isPlayerPointedBy2POnTriggerButton();

    if (isSwingOrPointed) {
        if (MR::hasME()) {
            MR::startSystemME("ME_MAGIC");
        }
        else {
            MR::startSystemSE("SE_SY_S_SPIN_DRV_ME_ALT", -1, -1);
        }

        MR::startSound(pSensor->mActor, "SE_PM_SPIN_ATTACK", -1, -1);

        if (MR::isInAreaObj("Water", mPosition)) {
            MR::startSound(this, "SE_PM_SPIN_DRV_IN_WATER_1", -1, -1);
        }

        setNerve(&NrvSpinDriver::SpinDriverNrvShootStart::sInstance);
    }
    else {
        if (_138 && _141 && mPosition.squared(pSensor->mPosition) < 57600.0f) {
            setNerve(&NrvSpinDriver::SpinDriverNrvCapture::sInstance);
        }
        else {
            return false;
        }
    }

    _8C = pSensor->mActor;
    _B8 = mPosition;
    _C4 = *MR::getPlayerLastMove();
    f32 mag = PSVECMag(_C4.toCVec());
    if (mag > 40.0f) {
        _C4 *= 40.0f / mag;
    }

    _11C = 0.0f;
    MtxPtr mtx = _8C->getBaseMtx();
    TRot3f rotation;
    rotation.setInline(mtx);
    rotation.getQuat(_98);
    _A8 = _98;
    cSpaceFlyStartFrame = MR::getBckFrameMax(_8C, "SpaceFlyStart");
    cSpaceFlyEndFrame = MR::getBckFrameMax(_8C, "SpaceFlyEnd");
    MR::validateClipping(this);
    return true;
}

void SpinDriver::cancelBind() {
    if (_8C) {
        MR::endBindAndPlayerJump(this, _C4, 0);
        _8C = nullptr;
        endCamera();
    }
}

void SpinDriver::updateBindPosition() {
    f32 step = getNerveStep() / (f32)_124;
    TVec3f stack_20(_B8);
    TVec3f position;
    mShootPath->calcPosition(&position, step);
    _B8 = position;
    _C4 = position - stack_20;
}

void SpinDriver::moveBindPosToCenter() {
    _B8 += _C4;
    TVec3f stack_24(mPosition - _B8);
    f32 scalar;
    MR::separateScalarAndDirection(&scalar, &stack_24, stack_24);
    f32 v3 = scalar / 120.0f;
    _C4 += MR::multVec(stack_24, 1.5f, v3);
    _C4.x *= 0.80000001f;
    _C4.y *= 0.80000001f;
    _C4.z *= 0.80000001f;
}

void SpinDriver::updateBindActorMatrix(f32 a1) {
    TPos3f rotation;
    MR::makeMtxUpFrontPos(&rotation, _E8, _D0, mPosition);
    TQuat4f quat;
    rotation.makeQuat(quat);
    _A8.x = _98.x;
    _A8.y = _98.y;
    _A8.z = _98.z;
    _A8.w = _98.w;
    _A8.slerp(quat, a1);
}

/*
void SpinDriver::calcBindActorMatrix() {
    f32 v3 = (2.0f * _A8.y);
    f32 v4 = _A8.z;
    f32 v5 = (2.0f * _A8.x);
    f32 v6 = (2.0f * _A8.y) * _A8.y;
    f32 v7 = (2.0f * _A8.w);
    f32 v8 = (1.0f - ((2.0f * _A8.x) * _A8.x));
    f32 v9 = ((1.0f - ((2.0f * _A8.x) * _A8.x)) - ((2.0f * _A8.z) * _A8.z));
    f32 v10 = (((2.0f * _A8.x) * _A8.y) - ((2.0f * _A8.w) * _A8.z));
    f32 v11 = (((2.0f * _A8.x) * _A8.y) + ((2.0f * _A8.w) * _A8.z));
    TPos3f pos;
    pos.mMtx[0][0] = (1.0f - ((2.0f * _A8.y) * _A8.y)) - ((2.0f * _A8.z) * _A8.z);
    pos.mMtx[0][1] = v10;
    f32 v12 = (v7 * _A8.y);
    pos.mMtx[1][1] = v9;
    f32 v13 = (v7 * _A8.x);
    pos.mMtx[1][0] = v11;
    pos.mMtx[2][2] = v8 - v6;
    pos.mMtx[0][2] = (v5 * v4) + v12;
    pos.mMtx[1][2] = (v3 * v4) - v13;
    pos.mMtx[2][0] = (v5 * v4) - v12;
    pos.mMtx[2][1] = (v3 * v4) + v13;
    pos.mMtx[0][3] = _B8.x;
    pos.mMtx[1][3] = _B8.y;
    pos.mMtx[2][3] = _B8.z;
    MR::setBaseTRMtx(_8C, pos);
}
*/

// SpinDriver::calcParabolicBindPose
// SpinDriver::turnBindHead

/*
void SpinDriver::calcShootMotionTime() {
    s32 v1 = _124;
    if (v1 < cSpaceFlyEndFrame) {
        _128 = -1;
        _12C = 0;
        _130 = v1;
    }
    else {
        _12C = v1 - 70;
        _130 = v1 - 20;
        _128 = (v1 - 70) / 5;
    }
}
*/

void SpinDriver::startCamera() {
    if (mSpinDriverCamera) {
        mSpinDriverCamera->start(_E8, _D0, mPosition);
    }
}

void SpinDriver::updateCamera() {
    if (mSpinDriverCamera) {
        mSpinDriverCamera->update(_C4, _B8);
    }
}

void SpinDriver::cancelCamera() {
    if (mSpinDriverCamera) {
        mSpinDriverCamera->cancel();
    }
}

void SpinDriver::endCamera() {
    if (mSpinDriverCamera) {
        mSpinDriverCamera->end();
    }
}

bool SpinDriver::canStartBind() const {
    return isNerve(&NrvSpinDriver::SpinDriverNrvWait::sInstance);
} 

bool SpinDriver::canBind(HitSensor *pSensor) const {
    if (!canStartBind()) {
        return false;
    }

    bool isSwingOrPointed = MR::isPadSwing(0) || MR::isPlayerPointedBy2POnTriggerButton();

    if (isSwingOrPointed) {
        return true;
    }

    if (_138 && _141 && mPosition.squared(pSensor->mPosition) < 457600.0f) {
        return true;
    }

    return false;
}

SpinDriver::~SpinDriver() {

}

namespace NrvSpinDriver {
    INIT_NERVE(SpinDriverNrvTryDemo);
    INIT_NERVE(SpinDriverNrvNonActive);
    INIT_NERVE(SpinDriverNrvAppear);
    INIT_NERVE(SpinDriverNrvWait);
    INIT_NERVE(SpinDriverNrvCapture);
    INIT_NERVE(SpinDriverNrvShootStart);
    INIT_NERVE(SpinDriverNrvShoot);
    INIT_NERVE(SpinDriverNrvCoolDown);

    void SpinDriverNrvCoolDown::execute(Spine *pSpine) const {
        SpinDriver* spinDriver = reinterpret_cast<SpinDriver*>(pSpine->mExecutor);
        spinDriver->exeCoolDown();
    }

    void SpinDriverNrvShoot::execute(Spine *pSpine) const {
        SpinDriver* spinDriver = reinterpret_cast<SpinDriver*>(pSpine->mExecutor);
        spinDriver->exeShoot();
    }

    void SpinDriverNrvShootStart::execute(Spine *pSpine) const {
        SpinDriver* spinDriver = reinterpret_cast<SpinDriver*>(pSpine->mExecutor);
        spinDriver->exeShootStart();
    }

    void SpinDriverNrvCapture::execute(Spine *pSpine) const {
        SpinDriver* spinDriver = reinterpret_cast<SpinDriver*>(pSpine->mExecutor);
        spinDriver->exeCapture();
    }

    void SpinDriverNrvWait::execute(Spine *pSpine) const {
        SpinDriver* spinDriver = reinterpret_cast<SpinDriver*>(pSpine->mExecutor);
        spinDriver->exeWait();
    }

    void SpinDriverNrvAppear::execute(Spine *pSpine) const {
        SpinDriver* spinDriver = reinterpret_cast<SpinDriver*>(pSpine->mExecutor);
        spinDriver->exeAppear();
    }

    void SpinDriverNrvNonActive::execute(Spine *pSpine) const {
        SpinDriver* spinDriver = reinterpret_cast<SpinDriver*>(pSpine->mExecutor);
        spinDriver->exeNonActive();
    }

    void SpinDriverNrvTryDemo::execute(Spine *pSpine) const {

    }
};