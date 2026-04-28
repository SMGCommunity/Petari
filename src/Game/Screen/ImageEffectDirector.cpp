#include "Game/Screen/ImageEffectDirector.hpp"
#include "Game/AreaObj/BloomArea.hpp"
#include "Game/AreaObj/DepthOfFieldArea.hpp"
#include "Game/AreaObj/ScreenBlurArea.hpp"
#include "Game/AreaObj/SimpleBloomArea.hpp"
#include "Game/Camera/CameraDirector.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Screen/BloomEffect.hpp"
#include "Game/Screen/BloomEffectSimple.hpp"
#include "Game/Screen/DepthOfFieldBlur.hpp"
#include "Game/Screen/ImageEffectBase.hpp"
#include "Game/Screen/ImageEffectLocalUtil.hpp"
#include "Game/Screen/ImageEffectState.hpp"
#include "Game/Util.hpp"

namespace {
    static const s32 sIntensityCountMax = 60;
    static const f32 sDOFIntensity = 1.0f;
}  // namespace

ImageEffectDirector::ImageEffectDirector(const char* pName)
    : NameObj(pName), mIsAuto(true), mIsPlayerSync(false), _E(false), _F(false), mPlayerSyncIntensity(0), mDepthOfFieldIntensity(sDOFIntensity),
      mStateNull(new ImageEffectStateImpl::StateNull(this)), mStateBloomNormal(new ImageEffectStateImpl::StateBloomNormal(this)),
      mStateBloomSimple(new ImageEffectStateImpl::StateBloomSimple(this)), mStateScreenBlur(new ImageEffectStateImpl::StateScreenBlur(this)),
      mStateDepthOfField(new ImageEffectStateImpl::StateDepthOfField(this)), mState(mStateNull), mCurrentEffect(nullptr) {
    MR::connectToSceneImageEffectMovement(this);
}

void ImageEffectDirector::movement() {
    if (mIsAuto) {
        updateAuto();
    } else {
        updateManual();
    }
    bool isDOF = _F && mState == mStateDepthOfField;
    if (_E) {
        if (!isDOF) {
            mState->onChange();
        }
    } else if (isDOF) {
        mState->forceOff();
        mStateNull->onChange();
    }
    if (isDOF) {
        mStateNull->update();
    } else {
        mState->update();
    }
    _E = isDOF;
}

void ImageEffectDirector::setCurrentEffect(ImageEffectBase* pEffect) {
    if (mCurrentEffect != pEffect) {
        mCurrentEffect = pEffect;
    }
}

void ImageEffectDirector::turnOnNormal() {
    mIsAuto = false;
    setState(mStateBloomNormal);
}

void ImageEffectDirector::turnOnDepthOfField(bool p1) {
    mIsAuto = false;
    setState(mStateDepthOfField);
    setPlayerSync(p1);
    mDepthOfFieldIntensity = sDOFIntensity;
    mStateDepthOfField->setBlurMinDist(-1);
    mStateDepthOfField->setBlurMaxDist(-1);
}

void ImageEffectDirector::turnOff() {
    mIsAuto = false;
    setState(mStateNull);
}

void ImageEffectDirector::forceOff() {
    mIsAuto = false;
    mState->forceOff();
    setState(mStateNull);
}

void ImageEffectDirector::setAuto() {
    mIsAuto = true;
}

void ImageEffectDirector::turnOffDOFInSubjective() {
    _F = true;
}

void ImageEffectDirector::turnOnDOFInSubjective() {
    _F = false;
}

void ImageEffectDirector::setNormalBloomIntensity(u8 bloomIntensity) {
    mStateBloomNormal->setBloomIntensity(bloomIntensity);
}

void ImageEffectDirector::setNormalBloomThreshold(u8 threshold) {
    mStateBloomNormal->setThreshold(threshold);
}

void ImageEffectDirector::setNormalBloomBlurIntensity1(u8 intensity1) {
    mStateBloomNormal->setIntensity1(intensity1);
}

void ImageEffectDirector::setNormalBloomBlurIntensity2(u8 intensity2) {
    mStateBloomNormal->setIntensity2(intensity2);
}

void ImageEffectDirector::setDepthOfFieldIntensity(f32 intensity) {
    mDepthOfFieldIntensity = intensity;
}

void ImageEffectDirector::updateManual() {
    if (mIsPlayerSync != false) {
        incPlayerSyncIntensity();
    } else {
        updateSyncCounter();
    }

    f32 intensity = mDepthOfFieldIntensity * (mPlayerSyncIntensity / 60.0f);

    if (mState == mStateScreenBlur) {
        mStateScreenBlur->setIntensity(intensity);
    } else if (mState == mStateDepthOfField) {
        mStateDepthOfField->setIntensity(intensity);
    }
}

void ImageEffectDirector::setState(ImageEffectState* pState) {
    if (mState != pState) {
        mState = pState;
        pState->onChange();
    }
}

void ImageEffectDirector::setBloomNormalParams(ImageEffectArea* pArea) {
    BloomArea* pBloomArea = static_cast< BloomArea* >(pArea);
    mStateBloomNormal->setBloomIntensity(pBloomArea->_41);
    mStateBloomNormal->setThreshold(pBloomArea->_40);
    if (pBloomArea->_44 >= 0) {
        mStateBloomNormal->setIntensity1(pBloomArea->_44);
    } else {
        mStateBloomNormal->setIntensity1Default();
    }
    if (pBloomArea->_48 >= 0) {
        mStateBloomNormal->setIntensity2(pBloomArea->_48);
    } else {
        mStateBloomNormal->setIntensity2Default();
    }
}

void ImageEffectDirector::setBloomSimpleParams(ImageEffectArea* pArea) {
    SimpleBloomArea* pSimpleBloomArea = static_cast< SimpleBloomArea* >(pArea);
    switch (pSimpleBloomArea->mMaskFilterColor) {
    case SimpleBloomArea::MASK_FILTER_ALL:
        mStateBloomSimple->setMaskFilterAll();
        break;
    case SimpleBloomArea::MASK_FILTER_RED:
        mStateBloomSimple->setMaskFilterRed();
        break;
    case SimpleBloomArea::MASK_FILTER_GREEN:
        mStateBloomSimple->setMaskFilterGreen();
        break;
    case SimpleBloomArea::MASK_FILTER_BLUE:
        mStateBloomSimple->setMaskFilterBlue();
        break;
    }
    mStateBloomSimple->setThreshold(pSimpleBloomArea->_44);
    mStateBloomSimple->setIntensity(pSimpleBloomArea->_45 / 255.0f);
}

void ImageEffectDirector::setScreenBlurParams(ImageEffectArea* pArea) {
    ScreenBlurArea* pScreenBlurArea = static_cast< ScreenBlurArea* >(pArea);
    updatePlayerSyncIntensity(pScreenBlurArea, mStateScreenBlur);
    mStateScreenBlur->setIntensity(mPlayerSyncIntensity / 60.0f);
}

void ImageEffectDirector::setDepthOfFieldParams(ImageEffectArea* pArea) {
    DepthOfFieldArea* pDepthOfFieldArea = static_cast< DepthOfFieldArea* >(pArea);
    updatePlayerSyncIntensity(pDepthOfFieldArea, mStateDepthOfField);
    f32 syncIntensity = mPlayerSyncIntensity / 60.0f;
    f32 areaIntensity = pDepthOfFieldArea->getIntensity();
    mStateDepthOfField->setIntensity(syncIntensity * areaIntensity);
    mStateDepthOfField->setBlurMaxDist(pDepthOfFieldArea->getBlurMaxDist());
    mStateDepthOfField->setBlurMinDist(pDepthOfFieldArea->getBlurMinDist());
}

void ImageEffectDirector::initPlayerSyncIntensity(ImageEffectArea* pArea, ImageEffectState* pState) {
    if (mState != pState) {
        if (pArea->isSyncWithPlayer()) {
            mPlayerSyncIntensity = 0;
        } else {
            mPlayerSyncIntensity = sIntensityCountMax;
        }
    }
}

void ImageEffectDirector::updatePlayerSyncIntensity(ImageEffectArea* pArea, ImageEffectState* pState) {
    initPlayerSyncIntensity(pArea, pState);
    if (pArea->isSyncWithPlayer()) {
        incPlayerSyncIntensity();
    } else {
        updateSyncCounter();
    }
}

void ImageEffectDirector::updateSyncCounter() {
    CameraDirector* pDirector = MR::getCameraDirector();
    if (pDirector->isRotatingHard()) {
        incPlayerSyncIntensity();
    } else {
        decPlayerSyncIntensity();
    }
}

void ImageEffectDirector::incPlayerSyncIntensity() {
    mPlayerSyncIntensity++;
    if (mPlayerSyncIntensity > sIntensityCountMax) {
        mPlayerSyncIntensity = sIntensityCountMax;
    }
}

void ImageEffectDirector::decPlayerSyncIntensity() {
    mPlayerSyncIntensity--;
    if (mPlayerSyncIntensity < 0) {
        mPlayerSyncIntensity = 0;
    }
}

void ImageEffectDirector::setPlayerSync(bool playerSync) {
    mIsPlayerSync = playerSync;
    if (playerSync) {
        mPlayerSyncIntensity = 0;
    } else {
        mPlayerSyncIntensity = sIntensityCountMax;
    }
}

void ImageEffectDirector::updateAuto() {
    ImageEffectArea* pEffectArea = static_cast< ImageEffectArea* >(MR::getAreaObj("ImageEffectArea", *MR::getPlayerPos()));
    if (!pEffectArea) {
        setState(mStateNull);
        return;
    }
    switch (pEffectArea->mEffectType) {
    case ImageEffectArea::IMAGE_EFFECT_TYPE_BLOOM:
        setBloomNormalParams(pEffectArea);
        setState(mStateBloomNormal);
        break;
    case ImageEffectArea::IMAGE_EFFECT_TYPE_SIMPLE_BLOOM:
        setBloomSimpleParams(pEffectArea);
        setState(mStateBloomSimple);
        break;
    case ImageEffectArea::IMAGE_EFFECT_TYPE_BLUR:
        setScreenBlurParams(pEffectArea);
        setState(mStateScreenBlur);
        break;
    case ImageEffectArea::IMAGE_EFFECT_TYPE_DOF:
        setDepthOfFieldParams(pEffectArea);
        setState(mStateDepthOfField);
        break;
    default:
        setState(mStateNull);
        break;
    }
}
