#include "Game/MapObj/MapPartsBreaker.hpp"
#include "Game/LiveActor/LiveActor.hpp"

namespace NrvMapPartsBreaker {
    NEW_NERVE(HostTypeWait, MapPartsBreaker, Wait);
    NEW_NERVE(HostTypeReactToTornado, MapPartsBreaker, ReactToTornado);
    NEW_NERVE(HostTypeBreak, MapPartsBreaker, Break);
};  // namespace NrvMapPartsBreaker

void MapPartsBreaker::exeWait() {}

void MapPartsBreaker::exeReactToTornado() {
    if (isFirstStep()) {
        TVec3f* vel = &mHost->mVelocity;
        vel->set< f32 >((_28 * 5.0f) * _34);
    }

    if (getStep() % 3 == 0) {
        mHost->mVelocity.invertInternal();
    }

    if (isStep(36)) {
        if (_38) {
            setNerve(&NrvMapPartsBreaker::HostTypeBreak::sInstance);
        } else {
            mHost->mVelocity.zero();
            mHost->mPosition.set< f32 >(_1C);
            _28.x = 0.0f;
            _28.y = 1.0f;
            _28.z = 0.0f;
            setNerve(&NrvMapPartsBreaker::HostTypeWait::sInstance);
        }
    } else {
        _38 = 0;
    }
}

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

// TODO: this SHOULD NOT be here, however moving this to its proper place causes linkage issues since
// TVec2f::squared is not used in this file... at all.
f32 TVec2f::squared(const TVec2f& rOther) const {
    return (x - rOther.x) * (x - rOther.x) + (y - rOther.y) * (y - rOther.y);
}
