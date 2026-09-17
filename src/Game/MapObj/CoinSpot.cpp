#include "Game/MapObj/CoinSpot.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "JSystem/JGeometry/TVec.hpp"

namespace NrvCoinSpot {
    NEW_NERVE(CoinSpotNrvWait, CoinSpot, Wait);
    NEW_NERVE(CoinSpotNrvIsInTornado, CoinSpot, IsInTornado);
};  // namespace NrvCoinSpot

CoinSpot::CoinSpot(const char* pName) : LiveActor(pName), _8C(8), mIsInTornado(), _94(4), _98() {
}

void CoinSpot::init(const JMapInfoIter& rIter) {
    MR::connectToSceneMapObjMovementCalcAnim(this);

    MR::initDefaultPos(this, rIter);

    s32 arg = 0;
    MR::getJMapInfoArg0NoInit(rIter, &arg);

    if (arg > 0) {
        _8C = arg;
    } else {
        _8C = 8;
    }

    initSound(4, false);
    initSensor();

    initNerve(GET_NERVE(CoinSpot, CoinSpotNrvWait));

    appear();
}

void CoinSpot::initSensor() {
    f32 scale = mScale.y;
    initHitSensor(1);
    TVec3f offset(0.0f, 60.0f * scale, 0.0f);
    MR::addHitSensorMapObj(this, "body", 32, offset.y, offset);
}

void CoinSpot::exeWait() {
    if (MR::isFirstStep(this)) {
        _94 = 0;
        _98 = 0;
        mIsInTornado = false;
    }

    if (mIsInTornado == true) {
        setNerve(GET_NERVE(CoinSpot, CoinSpotNrvIsInTornado));
    }
}

void CoinSpot::exeIsInTornado() {
    if (MR::isFirstStep(this)) {
        _94 = 0;
        _98 = 0;
    }

    if (mIsInTornado) {
        if (--_94 <= 0) {
            MR::appearCoinFix(this, mPosition, 1);
            _94 = 4;
            _98++;
        }

        mIsInTornado = false;
    } else {
        _94 = 0;
    }

    if (_98 >= _8C || MR::isGreaterStep(this, 180)) {
        MR::deleteEffect(this, "光");
        kill();
    }
}

void CoinSpot::attackSensor(HitSensor*, HitSensor*) {
}
