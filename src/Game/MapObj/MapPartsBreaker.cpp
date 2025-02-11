#include "Game/MapObj/MapPartsBreaker.hpp"
#include "Game/LiveActor/LiveActor.hpp"

/*
void MapPartsBreaker::exeReactToTornado() {
    if (isFirstStep()) {
        TVec3f* vel = &mHost->mVelocity;
        vel->set<f32>((_28 * 5.0f) * _34);
    }

    if (getStep() % 3) {
        mHost->mVelocity.negateSelf();
    }

    if (isStep(36)) {
        if (_38) {
            setNerve(&NrvMapPartsBreaker::HostTypeBreak::sInstance);
        }
        else {
            mHost->mVelocity.zero();
            mHost->mPosition.set<f32>(_1C);
            _28.x = 0.0f;
            _28.y = 1.0f;
            _28.z = 0.0f;
            setNerve(&NrvMapPartsBreaker::HostTypeWait::sInstance);
        }
    }
    else {
        _38 = 0;
    }
}
*/

void MapPartsBreaker::exeBreak() {
    if (isFirstStep()) {
        mHost->mVelocity.zero();
        MR::emitEffect(mHost, "SmokeSphere");
        MR::hideModel(mHost);
        MR::invalidateCollisionParts(mHost);
    }

    if (isStep(15)) {
        sendMsgToHost(206);
        mHost->kill();
    }
}

namespace NrvMapPartsBreaker {
    INIT_NERVE(HostTypeWait);
    INIT_NERVE(HostTypeReactToTornado);
    INIT_NERVE(HostTypeBreak);

    void HostTypeBreak::execute(Spine *pSpine) const {
        MapPartsBreaker* breaker = reinterpret_cast<MapPartsBreaker*>(pSpine->mExecutor);
        breaker->exeBreak();
    }

    void HostTypeReactToTornado::execute(Spine *pSpine) const {
        MapPartsBreaker* breaker = reinterpret_cast<MapPartsBreaker*>(pSpine->mExecutor);
        breaker->exeReactToTornado();
    }

    void HostTypeWait::execute(Spine *pSpine) const {

    }
};
