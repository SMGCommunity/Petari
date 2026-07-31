#include "Game/Enemy/AnimStampController.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/System/NerveExecutor.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/NerveUtil.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "revolution/types.h"

void dummy() {
    // to emit TVec3f init
    TVec3f a(0, 0, 0);
}

namespace NrvAnimStampController {
    NEW_NERVE(HostNrvAnim, AnimStampController, Anim);
    NEW_NERVE(HostNrvAnimEnd, AnimStampController, AnimEnd);
};  // namespace NrvAnimStampController

AnimStampController::AnimStampController() : NerveExecutor("つぶれアニメコントローラ"), _8(0, 0, 1) {
    init(8, 0.2f, 1.5f, 0.125f, 4.0f, 0.12, 4.0f);
}

AnimStampController::~AnimStampController() {
}

void AnimStampController::init(s32 a1, f32 a2, f32 a3, f32 a4, f32 a5, f32 a6, f32 a7) {
    _44 = a1;
    _14[3][1] = a2;
    _14[3][2] = a3;
    _14[3][3] = a4;
    _14[4][0] = a5;
    _14[4][1] = a6;
    _14[4][2] = a7;
    _14.identity();
    initNerve(&NrvAnimStampController::HostNrvAnimEnd::sInstance);
}

void AnimStampController::startAnim(const TVec3f& rVec) {
    _8.set(rVec);
    setNerve(&NrvAnimStampController::HostNrvAnim::sInstance);
    _14.identity();
}

void AnimStampController::exeAnim() {
    TVec3f vib = TVec3f(_14[3][2], _14[3][1], _14[3][2]);
    if (MR::isLessStep(this, _44)) {
        vib.x = MR::getReduceVibrationValue(getNerveStep(), _44, _14[3][2], _14[4][1], _14[4][2]);
        vib.y = MR::getReduceVibrationValue(getNerveStep(), _44, _14[3][1], _14[3][3], _14[4][0]);
        vib.z = MR::getReduceVibrationValue(getNerveStep(), _44, _14[3][2], _14[4][1], _14[4][2]);
    } else {
        setNerve(&NrvAnimStampController::HostNrvAnimEnd::sInstance);
    }

    MR::scaleMtxToDir(&_14, _8, vib);
}

void AnimStampController::exeAnimEnd() {
}

bool AnimStampController::isAnimEnd() const {
    return isNerve(&NrvAnimStampController::HostNrvAnimEnd::sInstance);
}
