#pragma once

#include <revolution.h>
#include "JSystem/JGeometry/TVec.h"
#include "JSystem/JUtility/JUTTexture.h"

namespace MR {
    void closeSystemWipeCircleWithCaptureScreen(s32);

    void setWipeCircleCenterPos(const TVec3f &);

    void createCenterScreenBlur();

    void createNormalBloom();
    void createSimpleBloom();

    void createScreenBlur();
    void startCenterScreenBlur(long, float, unsigned char, long, long);

    void createDepthOfFieldBlur();
    void turnOffDOFInSubjective();

    void openWipeCircle(s32);
    void forceOpenWipeCircle();

    void forceOpenWipeFade();
    void openWipeFade(f32);

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

    void openWipeWhiteFade(s32);
    void closeWipeWhiteFade(s32);

    void createPurpleCoinCounter();
    void validatePurpleCoinCounter();
};
