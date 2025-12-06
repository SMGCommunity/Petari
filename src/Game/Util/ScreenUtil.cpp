#include "Game/Util/ScreenUtil.hpp"
#include "Game/Scene/GameSceneFunction.hpp"
#include "Game/Scene/PlayTimerScene.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Screen/BloomEffect.hpp"
#include "Game/Screen/BloomEffectSimple.hpp"
#include "Game/Screen/CaptureScreenDirector.hpp"
#include "Game/Screen/CenterScreenBlur.hpp"
#include "Game/Screen/CinemaFrame.hpp"
#include "Game/Screen/CounterLayoutController.hpp"
#include "Game/Screen/DepthOfFieldBlur.hpp"
#include "Game/Screen/GameSceneLayoutHolder.hpp"
#include "Game/Screen/ImageEffectDirector.hpp"
#include "Game/Screen/ImageEffectSystemHolder.hpp"
#include "Game/Screen/InformationMessage.hpp"
#include "Game/Screen/LensFlare.hpp"
#include "Game/Screen/MoviePlayerSimple.hpp"
#include "Game/Screen/PurpleCoinCounter.hpp"
#include "Game/Screen/SceneWipeHolder.hpp"
#include "Game/Screen/ScreenBlurEffect.hpp"
#include "Game/Screen/StarCounter.hpp"
#include "Game/Screen/StarPieceCounter.hpp"
#include "Game/Screen/SystemWipeHolder.hpp"
#include "Game/Screen/YesNoController.hpp"
#include "Game/Screen/YesNoLayout.hpp"
#include "Game/SingletonHolder.hpp"
#include "Game/System/GameSystem.hpp"
#include "Game/System/GameSystemObjHolder.hpp"
#include "Game/System/GameSystemSceneController.hpp"
#include "Game/Util/RenderMode.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/SystemUtil.hpp"

namespace {
    CaptureScreenDirector* getCaptureScreenDirector() NO_INLINE { return SingletonHolder< GameSystem >::get()->mObjHolder->mCaptureScreenDir; }

    PlayTimerScene* getPlayTimerScene() NO_INLINE { return SingletonHolder< GameSystem >::get()->mSceneController->mPlayTimerScene; }

    void appearInformationMessage(bool hasButtonLayout, bool isCenter) {
        if (isCenter) {
            MR::getGameSceneLayoutHolder()->mInformationMessage->setCenter(true);
        } else {
            MR::getGameSceneLayoutHolder()->mInformationMessage->setCenter(false);
        }

        if (hasButtonLayout) {
            MR::getGameSceneLayoutHolder()->mInformationMessage->appearWithButtonLayout();
        } else {
            MR::getGameSceneLayoutHolder()->mInformationMessage->appear();
        }
    }

    StarCounter* getStarCounter() NO_INLINE { return MR::getGameSceneLayoutHolder()->mCounterLayoutCtrl->mStarCounter; }

    StarPieceCounter* getStarPieceCounter() NO_INLINE { return MR::getGameSceneLayoutHolder()->mCounterLayoutCtrl->mStarPieceCounter; }

    SystemWipeHolder* getSystemWipeHolder() { return SingletonHolder< GameSystem >::get()->mSystemWipeHolder; }

    BloomEffect* getNormalBloom() { return MR::getSceneObj< BloomEffect >(SceneObj_BloomEffect); }

    CinemaFrame* getCinemaFrame() { return MR::getSceneObj< CinemaFrame >(SceneObj_CinemaFrame); }

    MoviePlayerSimple* getMoviePlayer() { return MR::getSceneObj< MoviePlayerSimple >(SceneObj_MoviePlayerSimple); }
};  // namespace

namespace MR {
    u32 getViWidth() { return SingletonHolder< GameSystem >::get()->mObjHolder->getRenderModeObj()->viWidth; }

    f32 getSafetyVIScreenWidthRatio() {
        if (isScreen16Per9()) {
            return 0.87f;
        } else {
            return 0.844f;
        }
    }

    s32 getScreenWidth() { return isScreen16Per9() ? 832 : 608; }

    // FIXME: Source register swap in division instruction.
    s32 getSafetyFrameWidth() {
        s32 viWidthMax = static_cast< u16 >(RenderModeObj::getViWidthMax());
        f32 safetyFrameWidthRatio = viWidthMax * getSafetyVIScreenWidthRatio();
        s32 screenWidth = getScreenWidth();
        s32 viWidth = getViWidth();
        f32 viWidthRatio = static_cast< f32 >(viWidth) / screenWidth;

        return safetyFrameWidthRatio * viWidthRatio + 0.5f;
    }

    s32 getSafetyFrameLeft() { return (getScreenWidth() - getSafetyFrameWidth()) / 2; }

    s32 getSafetyFrameRight() { return (getScreenWidth() + getSafetyFrameWidth()) / 2; }

    void convertFrameBufferPosToScreenPos(TVec2f* pScreenPos, const TVec2f& rFrameBufferPos) {
        f32 posRatio = rFrameBufferPos.x / getFrameBufferWidth();

        pScreenPos->set(posRatio * getScreenWidth(), rFrameBufferPos.y);
    }

    void convertScreenPosToFrameBufferPos(TVec2f* pFrameBufferPos, const TVec2f& rScreenPos) {
        f32 posRatio = rScreenPos.x / getScreenWidth();

        pFrameBufferPos->set(posRatio * getFrameBufferWidth(), rScreenPos.y);
    }

    void startToCaptureScreen(const char* pParam1) { getCaptureScreenDirector()->requestCaptureTiming(pParam1); }

    void endToCaptureScreen(const char* pParam1) { getCaptureScreenDirector()->invalidateCaptureTiming(pParam1); }

    void captureScreenIfAllow(const char* pParam1) { getCaptureScreenDirector()->captureIfAllow(pParam1); }

    ResTIMG* getScreenResTIMG() { return getCaptureScreenDirector()->getResTIMG(); }

    u8* getScreenTexImage() { return getCaptureScreenDirector()->getTexImage(); }

    void closeWipeCircle(s32 param1) { SceneWipeHolderFunction::closeWipe("円ワイプ", param1); }

    void forceOpenWipeCircle() { SceneWipeHolderFunction::forceOpenWipe("円ワイプ"); }

    void forceCloseWipeCircle() { SceneWipeHolderFunction::forceCloseWipe("円ワイプ"); }

    void closeWipeFade(s32 param1) { SceneWipeHolderFunction::closeWipe("フェードワイプ", param1); }

    void forceOpenWipeFade() { SceneWipeHolderFunction::forceOpenWipe("フェードワイプ"); }

    void forceCloseWipeFade() { SceneWipeHolderFunction::forceCloseWipe("フェードワイプ"); }

    void closeWipeWhiteFade(s32 param1) { SceneWipeHolderFunction::closeWipe("白フェードワイプ", param1); }

    void forceOpenWipeWhiteFade() { SceneWipeHolderFunction::forceOpenWipe("白フェードワイプ"); }

    void forceCloseWipeWhiteFade() { SceneWipeHolderFunction::forceCloseWipe("白フェードワイプ"); }

    bool isWipeActive() {
        return SceneWipeHolderFunction::getSceneWipeHolder()->isWipeIn() || SceneWipeHolderFunction::getSceneWipeHolder()->isWipeOut();
    }

    bool isWipeBlank() { return SceneWipeHolderFunction::getSceneWipeHolder()->isClose(); }

    bool isWipeOpen() { return SceneWipeHolderFunction::getSceneWipeHolder()->isOpen(); }

    void closeSystemWipeCircle(s32 param1) {
        getSystemWipeHolder()->forceOpen("円ワイプ");
        getSystemWipeHolder()->wipe(nullptr, param1);
    }

    void openSystemWipeFade(s32 param1) {
        getSystemWipeHolder()->forceClose("フェードワイプ");
        getSystemWipeHolder()->wipe(nullptr, param1);
    }

    void closeSystemWipeFade(s32 param1) {
        getSystemWipeHolder()->forceOpen("フェードワイプ");
        getSystemWipeHolder()->wipe(nullptr, param1);
    }

    void forceOpenSystemWipeFade() { getSystemWipeHolder()->forceOpen("フェードワイプ"); }

    void openSystemWipeWhiteFade(s32 param1) {
        getSystemWipeHolder()->forceClose("白フェードワイプ");
        getSystemWipeHolder()->wipe(nullptr, param1);
    }

    void closeSystemWipeWhiteFade(s32 param1) {
        getSystemWipeHolder()->forceOpen("白フェードワイプ");
        getSystemWipeHolder()->wipe(nullptr, param1);
    }

    void forceCloseSystemWipeWhiteFade() { getSystemWipeHolder()->forceClose("白フェードワイプ"); }

    bool isSystemWipeActive() { return getSystemWipeHolder()->isWipeIn() || getSystemWipeHolder()->isWipeOut(); }

    void closeSystemWipeCircleWithCaptureScreen(s32 param1) {
        closeSystemWipeCircle(param1);
        getSystemWipeHolder()->startGameScreenCapture();
    }

    void closeSystemWipeFadeWithCaptureScreen(s32 param1) {
        closeSystemWipeFade(param1);
        getSystemWipeHolder()->startGameScreenCapture();
    }

    void setWipeCircleCenterPos(const TVec3f& rCenterPos) { getSystemWipeHolder()->setWipeRingCenter(rCenterPos); }

    void requestMovementOnImageEffect() {
        if (isExistSceneObj(SceneObj_ImageEffectSystemHolder)) {
            getImageEffectSystemHolder()->pauseOff();
        }

        if (isExistSceneObj(SceneObj_BloomEffect)) {
            requestMovementOn(getNormalBloom());
        }

        if (isExistSceneObj(SceneObj_BloomEffectSimple)) {
            requestMovementOn(getSceneObj< BloomEffectSimple >(SceneObj_BloomEffectSimple));
        }

        if (isExistSceneObj(SceneObj_ScreenBlurEffect)) {
            requestMovementOn(getSceneObj< ScreenBlurEffect >(SceneObj_ScreenBlurEffect));
        }

        if (isExistSceneObj(SceneObj_DepthOfFieldBlur)) {
            requestMovementOn(getSceneObj< DepthOfFieldBlur >(SceneObj_DepthOfFieldBlur));
        }
    }

    void setImageEffectControlAuto() { getImageEffectDirector()->setAuto(); }

    void turnOffDOFInSubjective() {
        if (isExistImageEffectDirector()) {
            getImageEffectDirector()->turnOffDOFInSubjective();
        }
    }

    void turnOnDOFInSubjective() {
        if (isExistImageEffectDirector()) {
            getImageEffectDirector()->turnOnDOFInSubjective();
        }
    }

    void turnOnNormalBloom() { getImageEffectDirector()->turnOnNormal(); }

    void turnOnDepthOfField(bool param1) { getImageEffectDirector()->turnOnDepthOfField(param1); }

    void turnOffImageEffect() { getImageEffectDirector()->turnOff(); }

    void forceOffImageEffect() { getImageEffectDirector()->forceOff(); }

    void createNormalBloom() { createSceneObj(SceneObj_BloomEffect); }

    bool isNormalBloomOn() { return isExistSceneObj(SceneObj_BloomEffect) && getNormalBloom()->isSomething(); }

    bool isNormalBloomForEachModel() { return isExistSceneObj(SceneObj_BloomEffect) && getNormalBloom()->isForEachModel(); }

    void setNormalBloomIntensity(u8 intensity) { getImageEffectDirector()->setNormalBloomIntensity(intensity); }

    void setNormalBloomThreshold(u8 threshold) { getImageEffectDirector()->setNormalBloomThreshold(threshold); }

    void setNormalBloomBlurIntensity1(u8 intensity1) { getImageEffectDirector()->setNormalBloomBlurIntensity1(intensity1); }

    void setNormalBloomBlurIntensity2(u8 intensity2) { getImageEffectDirector()->setNormalBloomBlurIntensity2(intensity2); }

    void createSimpleBloom() { createSceneObj(SceneObj_BloomEffectSimple); }

    void createScreenBlur() { createSceneObj(SceneObj_ScreenBlurEffect); }

    void createDepthOfFieldBlur() { createSceneObj(SceneObj_DepthOfFieldBlur); }

    void setDepthOfFieldBlurIntensity(f32 intensity) { getImageEffectDirector()->setDepthOfFieldIntensity(intensity); }

    void createCenterScreenBlur() { createSceneObj(SceneObj_CenterScreenBlur); }

    void startCenterScreenBlur(s32 param1, f32 param2, u8 param3, s32 param4, s32 param5) {
        getSceneObj< CenterScreenBlur >(SceneObj_CenterScreenBlur)->start(param1, param2, param3, param4, param5);
    }

    void startGlobalTimer() {}

    void resetGlobalTimer() { getPlayTimerScene()->stop(); }

    bool isGlobalTimerEnd() { return getPlayTimerScene()->isEndGlobalTimer(); }

    void startGlobalTimerTimeUp() { getPlayTimerScene()->startTimeUp(); }

    void requestPowerUpHPMeter() { getGameSceneLayoutHolder()->requestPowerUpHPMeter(); }

    void requestForceAppearHPMeter() { getGameSceneLayoutHolder()->requestForceAppearHPMeter(); }

    void startMissLayout() { getGameSceneLayoutHolder()->startMiss(); }

    bool isMissLayoutAnimEnd() { return getGameSceneLayoutHolder()->isMissAnimEnd(); }

    void activateDefaultGameLayout() { getGameSceneLayoutHolder()->activateDefaultGameLayout(); }

    void deactivateDefaultGameLayout() { getGameSceneLayoutHolder()->deactivateDefaultGameLayout(); }

    bool isActiveDefaultGameLayout() { return getGameSceneLayoutHolder()->mIsActiveDefaultGameLayout; }

    void forceAppearDefaultGameLayout() {
        getGameSceneLayoutHolder()->activateDefaultGameLayout();
        getGameSceneLayoutHolder()->mCounterLayoutCtrl->forceAppear();
    }

    void pauseOffLensFlare() {
        if (!isExistSceneObj(SceneObj_LensFlareDirector)) {
            return;
        }

        getSceneObj< LensFlareDirector >(SceneObj_LensFlareDirector)->pauseOff();
    }

    void tryScreenToFrameCinemaFrame() { getCinemaFrame()->tryScreenToFrame(); }

    void tryFrameToBlankCinemaFrame() { getCinemaFrame()->tryFrameToBlank(); }

    void tryBlankToFrameCinemaFrame() { getCinemaFrame()->tryBlankToFrame(); }

    void tryFrameToScreenCinemaFrame() { getCinemaFrame()->tryFrameToScreen(); }

    void forceToScreenCinemaFrame() { getCinemaFrame()->forceToScreen(); }

    void forceToFrameCinemaFrame() { getCinemaFrame()->forceToFrame(); }

    void forceToBlankCinemaFrame() { getCinemaFrame()->forceToBlank(); }

    bool isStopCinemaFrame() { return getCinemaFrame()->isStop(); }

    void appearInformationMessage(const char* pMessageId, bool hasButtonLayout) {
        getGameSceneLayoutHolder()->mInformationMessage->setMessage(pMessageId);
        ::appearInformationMessage(hasButtonLayout, false);
    }

    void appearInformationMessageCenter(const char* pMessageId, bool hasButtonLayout) {
        getGameSceneLayoutHolder()->mInformationMessage->setMessage(pMessageId);
        ::appearInformationMessage(hasButtonLayout, true);
    }

    void appearInformationMessage(const wchar_t* pMessage, bool hasButtonLayout) {
        getGameSceneLayoutHolder()->mInformationMessage->setMessage(pMessage);
        ::appearInformationMessage(hasButtonLayout, false);
    }

    void setInformationMessageReplaceString(const wchar_t* pMessage, s32 param2) {
        getGameSceneLayoutHolder()->mInformationMessage->setReplaceString(pMessage, param2);
    }

    void disappearInformationMessage() { getGameSceneLayoutHolder()->mInformationMessage->disappear(); }

    bool isDeadInformationMessage() { return isDead(getGameSceneLayoutHolder()->mInformationMessage); }

    void appearYesNoSelector(const char* pRightMessageId, const char* pLeftMessageId, const char* pInformationMessageId) {
        getGameSceneLayoutHolder()->mYesNoLayout->appear();
        getGameSceneLayoutHolder()->mYesNoLayout->setSelectMessage(pRightMessageId, pLeftMessageId);

        if (pInformationMessageId == nullptr) {
            return;
        }

        getGameSceneLayoutHolder()->mYesNoLayout->setInformationMessage(pInformationMessageId);
    }

    bool isYesNoSelected() { return getGameSceneLayoutHolder()->mYesNoLayout->mController->isSelected(); }

    bool isYesNoSelectedYes() { return getGameSceneLayoutHolder()->mYesNoLayout->mController->isSelectedYes(); }

    void setYesNoSelectorSE(const char* pCursorSE, const char* pYesSE, const char* pNoSE) {
        getGameSceneLayoutHolder()->mYesNoLayout->mController->setSE(pCursorSE, pYesSE, pNoSE);
    }

    void resetYesNoSelectorSE() { getGameSceneLayoutHolder()->mYesNoLayout->mController->setSE(nullptr, nullptr, nullptr); }

    void startMoviePlayer(const char* pName) {
        getMoviePlayer()->startMovie(pName, false);
        GameSceneFunction::requestPlayMovieDemo();
    }

    void stopMoviePlayer() { getMoviePlayer()->stopMovie(); }

    bool isActiveMoviePlayer() {
        if (!isExistSceneObj(SceneObj_MoviePlayerSimple)) {
            return false;
        }

        return getMoviePlayer()->isMovieActive();
    }

    bool isMoviePlayerPlaying() {
        if (!isActiveMoviePlayer()) {
            return false;
        }

        return getMoviePlayer()->isMoviePlaying();
    }

    s32 getMovieCurrentFrame() {
        if (!isActiveMoviePlayer()) {
            return -1;
        }

        return getMoviePlayer()->getCurrentFrame();
    }

    u32 getMovieTotalFrame() { return getMoviePlayer()->getTotalFrame(); }

    void setMovieVolume(f32 volume, s32 step) { getMoviePlayer()->setVolume(volume, step); }

    void appearStarCounter() { getStarCounter()->appear(); }

    void disappearStarCounter() { getStarCounter()->disappear(); }

    void forceSyncStarPieceCounter() { getStarPieceCounter()->forceSync(); }

    void forceAppearStarPieceCounterForTicoFat() { getStarPieceCounter()->forceAppear(true); }

    void disappearStarPieceCounterForTicoFat() { getStarPieceCounter()->disappear(true); }

    void createPurpleCoinCounter() { getGameSceneLayoutHolder()->createPurpleCoinCounter(); }

    void validatePurpleCoinCounter() { getGameSceneLayoutHolder()->getPurpleCoinCounter()->validate(); }

    void requestCounterLayoutAppearanceForTicoEat(bool param1) { getGameSceneLayoutHolder()->mCounterLayoutCtrl->requestedTicoEat(param1); }

    void openWipeCircle(s32 param1) { SceneWipeHolderFunction::openWipe("円ワイプ", param1); }

    void openWipeFade(s32 param1) { SceneWipeHolderFunction::openWipe("フェードワイプ", param1); }

    void openWipeWhiteFade(s32 param1) { SceneWipeHolderFunction::openWipe("白フェードワイプ", param1); }

    void startGameOverWipe() { SceneWipeHolderFunction::getSceneWipeHolder()->wipe("ゲームオーバー", -1); }

    void startDownWipe() { SceneWipeHolderFunction::getSceneWipeHolder()->wipe("クッパ", -1); }

    void requestOneUp() {
        startSystemSE("SE_SY_1UP", -1, -1);
        getGameSceneLayoutHolder()->requestOneUp(1);
    }
};  // namespace MR
