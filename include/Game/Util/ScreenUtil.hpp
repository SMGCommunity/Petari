#pragma once

#include <revolution.h>
#include "JSystem/JGeometry/TVec.hpp"
#include "JSystem/JUtility/JUTTexture.hpp"

namespace MR {
    void closeSystemWipeCircleWithCaptureScreen(s32);

    void setWipeCircleCenterPos(const TVec3f &);

    void createCenterScreenBlur();

    void createNormalBloom();
    void createSimpleBloom();

    bool isNormalBloomOn();
    void turnOnNormalBloom();
    void setNormalBloomIntensity(u8);
    void setNormalBloomThreshold(u8);
    void setNormalBloomBlurIntensity1(u8);
    void setNormalBloomBlurIntensity2(u8);

    void createScreenBlur();
    void startCenterScreenBlur(s32, f32, u8, s32, s32);

    void createDepthOfFieldBlur();
    void turnOffDOFInSubjective();

    void openWipeCircle(s32);
    void forceOpenWipeCircle();

    void forceOpenWipeFade();
    void openWipeFade(f32);

    void openSystemWipeWhiteFade(s32);

    bool isStageBeginTitleWipe();
    bool isStageBeginFadeWipe();
    bool isStageBeginWithoutWipe();

    bool isActiveMoviePlayer();

    void activateDefaultGameLayout();
    void deactivateDefaultGameLayout();
    bool isActiveDefaultGameLayout();
    void forceAppearDefaultGameLayout();
    void pauseOffLensFlare();
    void tryScreenToFrameCinemaFrame();
    void tryFrameToBlankCinemaFrame();
    void tryBlankToFrameCinemaFrame();
    void tryFrameToScreenCinemaFrame();
    void forceToScreenCinemaFrame();
    void forceToFrameCinemaFrame();
    void forceToBlankCinemaFrame();

    void requestOneUp();
    void requestPowerupHPMeter();


    void startToCaptureScreen(const char *);
    void endToCaptureScreen(const char *);
    void captureScreenIfAllow(const char *);

    ResTIMG* getScreenResTIMG();
    ResTIMG* getGalaxyMapResTIMG();

    s32 getScreenWidth();

    void openWipeWhiteFade(s32);
    void closeWipeWhiteFade(s32);

    void createPurpleCoinCounter();
    void validatePurpleCoinCounter();

    bool isSystemWipeActive();

    void closeSystemWipeWhiteFade(s32);

    void setImageEffectControlAuto();
};
