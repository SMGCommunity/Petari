#include "Game/MapObj/WaveFloatingForce.hpp"
#include <JSystem/JMath/JMATrigonometric.hpp>

WaveFloatingForce::WaveFloatingForce(const LiveActor *pActor, s32 a2, f32 a3, bool a4) {
    mActor = pActor;
    _4 = a2;
    _8 = a3;
    _C = 0.0f;
    _10 = false;
    _C = 6.2831855f * MR::getRandom();
}

/*
void WaveFloatingForce::update() {
    if (_10 || !MR::isOnPlayer(mActor)) {
        updateOnPlayer();
    }
    else {
        updateNormal();
    }
}
*/

#ifdef NON_MATCHING
f32 WaveFloatingForce::getCurrentValue() const {
    return _8 * JMath::sSinCosTable.sinLap(_C);
}
#endif

#define TWO_PI 6.2831855f
#define TWO_PI_APPROX 6.283185482025146
#define TWO_POW_52 4.503601774854144e15

/*
void WaveFloatingForce::updateNormal() {
    f32 v2 = _4;
    f32 v1 = (_C + (TWO_PI / (v2 - TWO_POW_52)));
    _C = _C + (TWO_PI / (v2 - TWO_POW_52));
    _C += MR::normalizePhase(v1, 0.0f, TWO_PI, TWO_PI_APPROX);
}

void WaveFloatingForce::updateOnPlayer() {

}
*/