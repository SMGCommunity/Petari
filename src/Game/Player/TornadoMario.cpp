#include "Game/Player/TornadoMario.hpp"

TornadoMario::TornadoMario(const char *pName) : LiveActor(pName) {
    _9C.x = 0.0f;
    _9C.y = 1.0f;
    _9C.z = 0.0f;
}

void TornadoMario::init(const JMapInfoIter &rIter) {
    initModelManagerWithAnm(mName, nullptr, true);
    initSound(4, false);
    MR::connectToScene(this, 0x25, 0x9, 0x15, -1);
    MR::initDLMakerFog(this, true);
    MR::newDifferedDLBuffer(this);
    MR::initJointTransform(this);
    MR::initShadowVolumeCylinder(this, 80.0f);
    MR::setShadowDropLength(this, nullptr, 3000.0f);
    MR::onShadowVolumeCutDropLength(this, nullptr);
    MR::setShadowVolumeEndDropOffset(this, nullptr, 50.0f);
    MR::onCalcShadow(this, nullptr);
    MR::invalidateClipping(this);
    appear();
    _8D = false;
    MR::hideModel(this);
}

void TornadoMario::calcAnim() {
    LiveActor::calcAnim();
}

void TornadoMario::calcAndSetBaseMtx() {
    TPos3f mtx;
    MR::makeMtxUpFrontPos(&mtx, _9C, _A8, mPosition);
    MR::setBaseTRMtx(this, mtx);
}

void TornadoMario::hideForce() {
    if (_8D) {
        _8D = false;
        MR::startBck(this, "MarioTornadoStart", nullptr);
        MR::hideModel(this);
    }
}

void TornadoMario::setTrHeight(const TVec3f &a1, const TVec3f &a2, const TVec3f &a3, const TVec3f &a4) {
    mPosition = a1;
    _9C = a3;
    _A8 = a2;
    MR::setShadowDropDirection(this, nullptr, a4);
}

void TornadoMario::show() {
    if (!_8D) {
        _8D = true;
        MR::startBck(this, "MarioTornadoEnd", nullptr);
        MR::startBtk(this, "MarioTornadoEnd");
        _8E = false;
        MR::showModel(this);
        _8C = true;
    }
}

void TornadoMario::hide() {
    if (_8D) {
        _8D = false;
        MR::startBck(this, "MarioTornadoEnd", nullptr);
        MR::startBtk(this, "MarioTornadoEnd");
    }
}

void TornadoMario::control() {
    if (_8D) {
        if (MR::isBckOneTimeAndStopped(this)) {
            MR::startBck(this, "MarioTornadoLoop", nullptr);
            MR::startBck(this, "MarioTornadoLoop", nullptr);
        }
        MR::startLevelSound(this, "SE_PM_LV_TORNADE_FLYING", -1, -1, -1);
    }
    else {
        if (MR::isBckOneTimeAndStopped(this)) {
            MR::hideModel(this);
        }
    }
}
