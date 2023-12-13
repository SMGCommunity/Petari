#include "Game/MapObj/CoinSpot.hpp"

CoinSpot::CoinSpot(const char *pName) : LiveActor(pName) {
    _8C = 8;
    _90 = 0;
    _94 = 4;
    _98 = 0;
}

void CoinSpot::init(const JMapInfoIter &rIter) {
    MR::connectToSceneMapObjMovementCalcAnim(this);
    MR::initDefaultPos(this, rIter);
    s32 arg = 0;
    MR::getJMapInfoArg0NoInit(rIter, &arg);

    if (arg <= 0) {
        _8C = arg;
    }
    else {
        _8C = 8;
    }

    initSound(4, false);
    initSensor();
    initNerve(&NrvCoinSpot::CoinSpotNrvWait::sInstance);
    appear();
}

void CoinSpot::initSensor() {
    f32 scale_y = mScale.y;
    initHitSensor(1);
    TVec3f offs;
    offs.x = 0.0f;
    offs.y = 60.0f * scale_y;
    offs.z = 0.0f;
    MR::addHitSensorMapObj(this, "body", 32, offs.y, offs);
}

void CoinSpot::exeWait() {
    if (MR::isFirstStep(this)) {
        _94 = 0;
        _98 = 0;
        _90 = 0;
    }

    if (_90 == 1) {
        setNerve(&NrvCoinSpot::CoinSpotNrvIsInTornado::sInstance);
    }
}

void CoinSpot::exeIsInTornado() {
    if (MR::isFirstStep(this)) {
        _94 = 0;
        _98 = 0;
    }

    if (_90) {
        s32 v2 = _94;
        _94 = v2 - 1;

        if (v2 - 1 <= 0) {
            MR::appearCoinFix(this, mPosition, 1);
            _94 = 4;
            _98 = _98 + 1;
        }

        _90 = 0;
    }
    else {
        _94 = 0;
    }

    if (_98 >= _8C || MR::isGreaterStep(this, 180)) {
        MR::deleteEffect(this, "光");
        kill();
    }
}

void CoinSpot::attackSensor(HitSensor *, HitSensor *) {

}

CoinSpot::~CoinSpot() {

}

namespace NrvCoinSpot {
    INIT_NERVE(CoinSpotNrvWait);
    INIT_NERVE(CoinSpotNrvIsInTornado);

    void CoinSpotNrvIsInTornado::execute(Spine *pSpine) const {
        CoinSpot* spot = reinterpret_cast<CoinSpot*>(pSpine->mExecutor);
        spot->exeIsInTornado();
    }
    
    void CoinSpotNrvWait::execute(Spine *pSpine) const {
        CoinSpot* spot = reinterpret_cast<CoinSpot*>(pSpine->mExecutor);
        spot->exeWait();
    }
};