#include "Game/Enemy/AnimScaleController.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/NerveUtil.hpp"

namespace NrvAnimScaleController {
    NEW_NERVE(AnimScaleControllerNrvAnim, AnimScaleController, Anim);
    NEW_NERVE(AnimScaleControllerNrvStop, AnimScaleController, Stop);
    NEW_NERVE(AnimScaleControllerNrvDpdVibration, AnimScaleController, DpdVibration);
    NEW_NERVE(AnimScaleControllerNrvHitReaction, AnimScaleController, HitReaction);
    NEW_NERVE(AnimScaleControllerNrvCrush, AnimScaleController, Crush);
}  // namespace NrvAnimScaleController

namespace {

    class AnimScaleDefaultParam : public AnimScaleParam {
    public:
        AnimScaleDefaultParam();

        AnimScaleParam tightParam;  // 0x34;
    };

    AnimScaleDefaultParam::AnimScaleDefaultParam() {
        tightParam._0 = 0.5f;
        tightParam._4 = 0.85f;
        tightParam._10 = 8.0f;
        tightParam._14 = 0.8f;
        tightParam._18 = 0.08f;
        tightParam._1C = 2.2f;
    };

    AnimScaleDefaultParam sStaticAnimParam;
}  // namespace

AnimScaleParam::AnimScaleParam() {
    _0 = 0.2f;
    _4 = 0.91f;
    _8 = 0.2f;
    _C = 1.8f;
    _10 = 12.0f;
    _14 = 0.55f;
    _18 = 0.12f;
    _1C = 2.0f;
    _20 = 0.06f;
    _24 = 0.12f;
    _28 = 0.91f;
    _2C = 0x14;
    _30 = 0.25f;
}

AnimScaleController::AnimScaleController(AnimScaleParam* pScaleParam) : NerveExecutor("スケールアニメコントロール\00") {
    _8 = pScaleParam;
    _C.set(1.0f);
    _18 = 0.0f;

    if (pScaleParam == nullptr) {
        _8 = &::sStaticAnimParam;
    }
    initNerve(&NrvAnimScaleController::AnimScaleControllerNrvStop::sInstance);
}

void AnimScaleController::setParamTight() {
    _8 = &::sStaticAnimParam.tightParam;
}

void AnimScaleController::startAnim() {
    setNerve(&NrvAnimScaleController::AnimScaleControllerNrvAnim::sInstance);
}

void AnimScaleController::startDpdHitVibration() {
    setNerve(&NrvAnimScaleController::AnimScaleControllerNrvDpdVibration::sInstance);
}

void AnimScaleController::startHitReaction() {
    setNerve(&NrvAnimScaleController::AnimScaleControllerNrvHitReaction::sInstance);
}

void AnimScaleController::startAndAddScaleVelocityY(f32 f1) {
    _18 += f1;
    setNerve(&NrvAnimScaleController::AnimScaleControllerNrvAnim::sInstance);
}

void AnimScaleController::startCrush() {
    setNerve(&NrvAnimScaleController::AnimScaleControllerNrvCrush::sInstance);
}

void AnimScaleController::stopAndReset() {
    resetScale();
    // resetScale already sets _18 to 0, so this is completely unnecessary
    _18 = 0.0f;
    setNerve(&NrvAnimScaleController::AnimScaleControllerNrvStop::sInstance);
}

void AnimScaleController::resetScale() {
    _18 = 0.0f;
    _C.set(1.0f);
}

void AnimScaleController::exeAnim() {
    updateScale(_8->_0, _8->_4);
    if (tryStop()) {
        return;
    }
}

void AnimScaleController::exeStop() {}

void AnimScaleController::exeHitReaction() {
    if (MR::isFirstStep(this)) {
        resetScale();
        _18 = _8->_20;
    }

    updateScale(_8->_24, _8->_28);
    if (tryStop()) {
        return;
    }
}

void AnimScaleController::exeDpdVibration() {
    f32 nerveStep = getNerveStep();
    f32 dpdVibrationValue = MR::getReduceVibrationValue(nerveStep, _8->_10, _8->_14, _8->_18, _8->_1C);

    _C.y = dpdVibrationValue;
    _C.x = MR::sqrt(1.0f / _C.y);
    _C.z = _C.x;
}

void AnimScaleController::exeCrush() {
    _18 = 0.0f;

    f32 nerveRate = MR::calcNerveRate(this, _8->_2C);
    f32 crushValue = MR::getConvergeVibrationValue(nerveRate, 1.0f, _8->_30, 0.3f, 4.0f);

    _C.y = crushValue;
    _C.x = MR::sqrt(1.0f / _C.y);
    _C.z = _C.x;

    if (MR::isGreaterStep(this, _8->_2C)) {
        setNerve(&NrvAnimScaleController::AnimScaleControllerNrvStop::sInstance);
    }
}

bool AnimScaleController::isHitReaction(s32 a1) const {
    if (!isNerve(&NrvAnimScaleController::AnimScaleControllerNrvHitReaction::sInstance)) {
        return false;
    }

    if (a1 > 0) {
        return MR::isLessStep(this, a1);
    }
    return true;
}

bool AnimScaleController::tryStop() {
    // currently not matching, probably inlined MR::isNearZero(f32);
    if (__fabsf(1.0f - _C.y) < 0.001f && __fabsf(_18) < 0.001f) {
        resetScale();
        setNerve(&NrvAnimScaleController::AnimScaleControllerNrvStop::sInstance);
        return true;
    }
    return false;
}

void AnimScaleController::update() {
    updateNerve();
}

void AnimScaleController::updateScale(f32 f1, f32 f2) {
    f32 updateValue = ((1.0f - _C.y) * f1 + _18);
    updateValue = updateValue * f2;
    _18 = updateValue;
    _C.y += updateValue;

    f32 updateValue2 = _C.y;

    f32 clampMin = _8->_8;
    f32 clampMax = _8->_C;
    if (_C.y < clampMin) {
        updateValue2 = clampMin;
    } else if (_C.y > clampMax) {
        updateValue2 = clampMax;
    } else {
        updateValue2 = _C.y;
    }

    _C.y = updateValue2;
    _C.x = MR::sqrt(1.0f / _C.y);
    _C.z = _C.x;
}

AnimScaleController::~AnimScaleController() {};
