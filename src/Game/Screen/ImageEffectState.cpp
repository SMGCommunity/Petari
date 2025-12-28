#include "Game/Screen/ImageEffectState.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Screen/BloomEffect.hpp"
#include "Game/Screen/BloomEffectSimple.hpp"
#include "Game/Screen/DepthOfFieldBlur.hpp"
#include "Game/Screen/ImageEffectBase.hpp"
#include "Game/Screen/ImageEffectDirector.hpp"
#include "Game/Screen/ScreenBlurEffect.hpp"

void ImageEffectState::update() {
    ImageEffectBase* pBase;

    bool b = mHost->mCurrentEffect == getEffect();

    if (b) {
        if (getEffect() == nullptr) {
            return;
        }

        if (getEffect()->_C) {
            return;
        }

        pBase = getEffect();
        pBase->_C = true;
        pBase->notifyTurnOn();
        return;
    }

    pBase = mHost->mCurrentEffect;

    if (pBase != nullptr) {
        if (pBase->_C) {
            pBase->_C = false;
            pBase->notifyTurnOff();
            return;
        }

        if (pBase->isSomething()) {
            return;
        }
    }

    if (getEffect() != nullptr) {
        pBase = getEffect();
        pBase->_C = true;
        pBase->notifyTurnOn();
    }

    mHost->setCurrentEffect(getEffect());
}

void ImageEffectState::forceOff() {
    ImageEffectBase* pBase;

    if (getEffect() == nullptr) {
        return;
    }

    pBase = getEffect();
    pBase->_D = false;
    pBase->_C = false;
    pBase->_10 = 0.0f;
    pBase->notifyForceOff();
}

namespace ImageEffectStateImpl {
    StateBloomNormal::StateBloomNormal(ImageEffectDirector* pHost)
        : ImageEffectState(pHost), _8(true), _C(0), _10(0.0f), mBloomIntensity(0), _18(0.0f), mThreshold(0), _20(0.0f), mIntensity1(0), _28(0.0f),
          mIntensity2(0) {}

    void StateBloomNormal::update() {
        if (_8) {
            _10 = mBloomIntensity;
            _18 = mThreshold;
            _20 = mIntensity1;
            _28 = mIntensity2;
            _8 = false;
        } else {
            _10 += ((mBloomIntensity + 0.5f) - _10) * 0.1f;
            _18 += ((mThreshold + 0.5f) - _18) * 0.1f;
            _20 += ((mIntensity1 + 0.5f) - _20) * 0.1f;
            _28 += ((mIntensity2 + 0.5f) - _28) * 0.1f;

            if (_10 > 255.0f) {
                _10 = 255.0f;
            }

            if (_18 > 255.0f) {
                _18 = 255.0f;
            }

            if (_20 > 255.0f) {
                _20 = 255.0f;
            }

            if (_28 > 255.0f) {
                _28 = 255.0f;
            }
        }

        static_cast< BloomEffect* >(StateBloomNormal::getEffect())->_14 = _10;
        static_cast< BloomEffect* >(StateBloomNormal::getEffect())->_20 = _18;
        static_cast< BloomEffect* >(StateBloomNormal::getEffect())->_18 / 255.0f;
        static_cast< BloomEffect* >(StateBloomNormal::getEffect())->_1C / 255.0f;

        ImageEffectState::update();
    }

    bool StateBloomNormal::doesEffectExist() const {
        return MR::isExistSceneObj(SceneObj_BloomEffect);
    }

    ImageEffectBase* StateBloomNormal::getEffect() const {
        return MR::getSceneObj< ImageEffectBase >(SceneObj_BloomEffect);
    }

    void StateBloomNormal::onChange() {
        _8 = true;
    }

    void StateBloomNormal::setBloomIntensity(u8 bloomIntensity) {
        mBloomIntensity = bloomIntensity;
    }

    void StateBloomNormal::setThreshold(u8 threshold) {
        mThreshold = threshold;
    }

    void StateBloomNormal::setIntensity1(u8 intensity1) {
        mIntensity1 = intensity1;
    }

    void StateBloomNormal::setIntensity1Default() {
        mIntensity1 = static_cast< BloomEffect* >(StateBloomNormal::getEffect())->getIntensity1Default();
    }

    void StateBloomNormal::setIntensity2(u8 intensity2) {
        mIntensity2 = intensity2;
    }

    void StateBloomNormal::setIntensity2Default() {
        mIntensity2 = static_cast< BloomEffect* >(StateBloomNormal::getEffect())->getIntensity2Default();
    }

    StateBloomSimple::StateBloomSimple(ImageEffectDirector* pHost) : ImageEffectState(pHost) {}

    bool StateBloomSimple::doesEffectExist() const {
        return MR::isExistSceneObj(SceneObj_BloomEffectSimple);
    }

    ImageEffectBase* StateBloomSimple::getEffect() const {
        return MR::getSceneObj< ImageEffectBase >(SceneObj_BloomEffectSimple);
    }

    void StateBloomSimple::setMaskFilterAll() {
        static_cast< BloomEffectSimple* >(StateBloomSimple::getEffect())->mMaskFilter = 0;
    }

    void StateBloomSimple::setMaskFilterRed() {
        static_cast< BloomEffectSimple* >(StateBloomSimple::getEffect())->mMaskFilter = 1;
    }

    void StateBloomSimple::setMaskFilterGreen() {
        static_cast< BloomEffectSimple* >(StateBloomSimple::getEffect())->mMaskFilter = 2;
    }

    void StateBloomSimple::setMaskFilterBlue() {
        static_cast< BloomEffectSimple* >(StateBloomSimple::getEffect())->mMaskFilter = 3;
    }

    void StateBloomSimple::setThreshold(u8 threshold) {
        static_cast< BloomEffectSimple* >(StateBloomSimple::getEffect())->mThreshold = threshold;
    }

    void StateBloomSimple::setIntensity(f32 intensity) {
        static_cast< BloomEffectSimple* >(StateBloomSimple::getEffect())->mIntensity = intensity;
    }

    StateScreenBlur::StateScreenBlur(ImageEffectDirector* pHost) : ImageEffectState(pHost) {}

    bool StateScreenBlur::doesEffectExist() const {
        return MR::isExistSceneObj(SceneObj_ScreenBlurEffect);
    }

    ImageEffectBase* StateScreenBlur::getEffect() const {
        return MR::getSceneObj< ImageEffectBase >(SceneObj_ScreenBlurEffect);
    }

    void StateScreenBlur::setIntensity(f32 intensity) {
        static_cast< ScreenBlurEffect* >(StateScreenBlur::getEffect())->mIntensity = intensity;
    }

    StateDepthOfField::StateDepthOfField(ImageEffectDirector* pHost) : ImageEffectState(pHost) {}

    bool StateDepthOfField::doesEffectExist() const {
        return MR::isExistSceneObj(SceneObj_DepthOfFieldBlur);
    }

    ImageEffectBase* StateDepthOfField::getEffect() const {
        return MR::getSceneObj< ImageEffectBase >(SceneObj_DepthOfFieldBlur);
    }

    void StateDepthOfField::setIntensity(f32 intensity) {
        static_cast< DepthOfFieldBlur* >(StateDepthOfField::getEffect())->mIntensity = intensity;
    }

    void StateDepthOfField::setBlurMaxDist(s32 blurMaxDist) {
        static_cast< DepthOfFieldBlur* >(StateDepthOfField::getEffect())->mBlurMaxDist = blurMaxDist;
    }

    void StateDepthOfField::setBlurMinDist(s32 blurMinDist) {
        static_cast< DepthOfFieldBlur* >(StateDepthOfField::getEffect())->mBlurMinDist = blurMinDist;
    }
};  // namespace ImageEffectStateImpl
