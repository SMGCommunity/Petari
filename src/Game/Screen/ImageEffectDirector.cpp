#include "Game/Screen/ImageEffectState.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Screen/BloomEffect.hpp"
#include "Game/Screen/BloomEffectSimple.hpp"
#include "Game/Screen/DepthOfFieldBlur.hpp"
#include "Game/Screen/ImageEffectBase.hpp"
#include "Game/Screen/ImageEffectDirector.hpp"
#include "Game/Screen/ImageEffectLocalUtil.hpp"
#include "Game/AreaObj/BloomArea.hpp"
#include "Game/AreaObj/DepthOfFieldArea.hpp"
#include "Game/AreaObj/SimpleBloomArea.hpp"
#include "Game/AreaObj/ScreenBlurArea.hpp"
#include "Game/Camera/CameraDirector.hpp"
#include "Game/Util.hpp"


ImageEffectDirector::ImageEffectDirector(const char* pName)
	: NameObj(pName), mIsAuto(true) {
	mIsPlayerSync = false;
	_E = false;
	_F = false;
	mPlayerSyncIntensity = 0;
	mDepthOfFieldIntensity = 1.0f;
	_18 = new ImageEffectStateImpl::StateNull(this);
	_1C = new ImageEffectStateImpl::StateBloomNormal(this);
	_20 = new ImageEffectStateImpl::StateBloomSimple(this);
	_24 = new ImageEffectStateImpl::StateScreenBlur(this);
	_28 = new ImageEffectStateImpl::StateDepthOfField(this);
	mState = _18;
	mCurrentEffect = nullptr;
	MR::connectToSceneImageEffectMovement(this);
}

void ImageEffectDirector::movement(){
	if (mIsAuto){
		updateAuto();
	} else {
		updateManual();
	}
	bool isDOF = false;
	if (_F != false && mState == _28) {
		isDOF = true;
	}
	if (_E) {
		if (!isDOF){
			mState->onChange();
		}
	} else {
		if (isDOF){
			mState->forceOff();
			_18->onChange();
		}
	}
	if (isDOF){
		_18->update();
	} else {
		mState->update();
	}
	_E = isDOF;
}

void ImageEffectDirector::setCurrentEffect(ImageEffectBase* pEffect){
	if (mCurrentEffect != pEffect){
		mCurrentEffect = pEffect;
	}
}

void ImageEffectDirector::turnOnNormal(){
	mIsAuto = false;
	setState(_1C);
}

void ImageEffectDirector::turnOnDepthOfField(bool p1){
	mIsAuto = false;
	setState(_28);
	setPlayerSync(p1);
	mDepthOfFieldIntensity = 1.0f;
	_28->setBlurMinDist(-1);
	_28->setBlurMaxDist(-1);
}

void ImageEffectDirector::turnOff(){
	mIsAuto = false;
	setState(_18);
}

void ImageEffectDirector::forceOff(){
	mIsAuto = false;
	mState->forceOff();
	setState(_18);
}

void ImageEffectDirector::setAuto(){
	mIsAuto = true;
}

void ImageEffectDirector::turnOffDOFInSubjective(){
	_F = true;
}

void ImageEffectDirector::turnOnDOFInSubjective(){
	_F = false;
}

void ImageEffectDirector::setNormalBloomIntensity(u8 bloomIntensity){
	_1C->setBloomIntensity(bloomIntensity);
}

void ImageEffectDirector::setNormalBloomThreshold(u8 threshold){
	_1C->setThreshold(threshold);
}

void ImageEffectDirector::setNormalBloomBlurIntensity1(u8 intensity1){
	_1C->setIntensity1(intensity1);
}

void ImageEffectDirector::setNormalBloomBlurIntensity2(u8 intensity2){
	_1C->setIntensity2(intensity2);
}

void ImageEffectDirector::setDepthOfFieldIntensity(f32 intensity){
	mDepthOfFieldIntensity = intensity;
}

void ImageEffectDirector::updateManual(){
	if (mIsPlayerSync != false) {
		incPlayerSyncIntensity();
	} else {
		updateSyncCounter();
	}
	
	f32 intensity = mDepthOfFieldIntensity * (mPlayerSyncIntensity / 60.0f);
	
	if (mState == _24) {
		_24->setIntensity(intensity) ;
	} else if (mState == _28) {
		_28->setIntensity(intensity);
	}
}

void ImageEffectDirector::setState(ImageEffectState* pState){
	if (mState != pState){
		mState = pState;
		pState->onChange();
	}
}

void ImageEffectDirector::setBloomNormalParams(ImageEffectArea* pArea){
	BloomArea* pBloomArea = static_cast<BloomArea*>(pArea);
	_1C->setBloomIntensity(pBloomArea->_41);
	_1C->setThreshold(pBloomArea->_40);
	if (pBloomArea->_44 >= 0) {
		_1C->setIntensity1(pBloomArea->_44);
	} else {
		_1C->setIntensity1Default();
	}
	if (pBloomArea->_48 >= 0) {
		_1C->setIntensity2(pBloomArea->_48);
	} else {
		_1C->setIntensity2Default();
	}

}

void ImageEffectDirector::setBloomSimpleParams(ImageEffectArea* pArea){
	SimpleBloomArea* pSimpleBloomArea = static_cast<SimpleBloomArea*>(pArea);
	switch (pSimpleBloomArea->mMaskFilterColor){
		case 0:
			_20->setMaskFilterAll();
			break;
		case 1:
			_20->setMaskFilterRed();
			break;
		case 2:
			_20->setMaskFilterGreen();
			break;
		case 3:
			_20->setMaskFilterBlue();
			break;
	}
	_20->setThreshold(pSimpleBloomArea->_44);
	_20->setIntensity(pSimpleBloomArea->_45 / 255.0f);
}

void ImageEffectDirector::setScreenBlurParams(ImageEffectArea* pArea){
	ScreenBlurArea* pScreenBlurArea = static_cast<ScreenBlurArea*>(pArea);
	updatePlayerSyncIntensity(pScreenBlurArea, _24);
	_24->setIntensity(mPlayerSyncIntensity / 60.0f);
}

void ImageEffectDirector::setDepthOfFieldParams(ImageEffectArea* pArea){
	DepthOfFieldArea* pDepthOfFieldArea = static_cast<DepthOfFieldArea*>(pArea);
	updatePlayerSyncIntensity(pDepthOfFieldArea, _28);
	f32 syncintensity = mPlayerSyncIntensity / 60.0f;
	f32 areaintensity = pDepthOfFieldArea->getIntensity();
	_28->setIntensity(syncintensity * areaintensity);
	_28->setBlurMaxDist(pDepthOfFieldArea->getBlurMaxDist());
	_28->setBlurMinDist(pDepthOfFieldArea->getBlurMinDist());
}

void ImageEffectDirector::initPlayerSyncIntensity(ImageEffectArea* pArea, ImageEffectState* pState){
	if (mState != pState){
		if (pArea->isSyncWithPlayer()){
			mPlayerSyncIntensity = 0;
		} else {
			mPlayerSyncIntensity = 60;
		}
	}
	
}

void ImageEffectDirector::updatePlayerSyncIntensity(ImageEffectArea* pArea, ImageEffectState* pState){
	initPlayerSyncIntensity(pArea, pState);
	if (pArea->isSyncWithPlayer()){
		incPlayerSyncIntensity();
	} else {
		updateSyncCounter();
	}
}

void ImageEffectDirector::updateSyncCounter(){
	CameraDirector* pDirector = MR::getCameraDirector();
    if (pDirector->isRotatingHard()){
		incPlayerSyncIntensity();
	} else {
		decPlayerSyncIntensity();
	}
}

void ImageEffectDirector::incPlayerSyncIntensity(){
	mPlayerSyncIntensity += 1;
	if (mPlayerSyncIntensity > 60){
		mPlayerSyncIntensity = 60;
	}
	
}

void ImageEffectDirector::decPlayerSyncIntensity(){
	mPlayerSyncIntensity -= 1;
	if (mPlayerSyncIntensity < 0){
		mPlayerSyncIntensity = 0;
	}
}

void ImageEffectDirector::setPlayerSync(bool p1){
	mIsPlayerSync = p1;
	if (p1)	{
		mPlayerSyncIntensity = 0;
	} else {
		mPlayerSyncIntensity = 60;
	}
	
}

void ImageEffectDirector::updateAuto(){
	ImageEffectArea* pEffectArea = static_cast<ImageEffectArea*>(MR::getAreaObj("ImageEffectArea", *MR::getPlayerPos()));
	if (!pEffectArea){
		setState(_18);
		return;
	}
	switch (pEffectArea->mEffectType){
		case ImageEffectArea::IMAGE_EFFECT_TYPE_BLOOM:
			setBloomNormalParams(pEffectArea);
			setState(_1C);
			break;
		case ImageEffectArea::IMAGE_EFFECT_TYPE_SIMPLE_BLOOM:
			setBloomSimpleParams(pEffectArea);
			setState(_20);
			break;
		case ImageEffectArea::IMAGE_EFFECT_TYPE_BLUR:
			setScreenBlurParams(pEffectArea);
			setState(_24);
			break;
		case ImageEffectArea::IMAGE_EFFECT_TYPE_DOF:
			setDepthOfFieldParams(pEffectArea);
			setState(_28);
			break;
		default:
			setState(_18);
			break;
			
	}
}