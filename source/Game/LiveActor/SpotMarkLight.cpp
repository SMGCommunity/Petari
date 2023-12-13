#include "Game/LiveActor/SpotMarkLight.hpp"

SpotMarkLight::SpotMarkLight(LiveActor *pActor, f32 a3, f32 a4, MtxPtr mtx) : PartsModel(pActor, "マーク光柱", "SpotMarkLight", mtx, -1, false) {
    _9B = false;

    if (!mtx) {
        initFixedPosition(pActor->getBaseMtx(), TVec3f(0.0f, 0.0f, 0.0f), TVec3f(0.0f, 0.0f, 0.0f));
    }

    mScale.z = a3 / 50.0f;
    mScale.x = a3 / 50.0f;
    mScale.y = a4 / 1000.0f;
    MR::startBrk(this, "Appear");
    MR::registerDemoSimpleCastAll(this);
}

void SpotMarkLight::control() {
    if (MR::isNearPlayerAnyTime(this, 1000.0f)) {
        if (!_9B) {
            if (MR::isBrkStopped(this)) {
                MR::startBrk(this, "Disappear");
                _9B = true;
            }
        }
    }
    else if (_9B && MR::isBrkStopped(this)) {
        MR::startBrk(this, "Appear");
        _9B = false;
    }
}

void SpotMarkLight::calcAndSetBaseMtx() {
    PartsModel::calcAndSetBaseMtx();
}

SpotMarkLight::~SpotMarkLight() {

}
