#pragma once

#include "JSystem/JGeometry.hpp"
#include <revolution.h>

class LayoutActor;
class LiveActor;
class StarPointerOnOffController;

namespace {
    class StarPointerTargetInfo;

    typedef bool (*StarPointerFunc1)(StarPointerTargetInfo*, const TVec3f&, const TVec2f&, f32, f32);
    typedef bool (*StarPointerFunc2)(s32);

    StarPointerOnOffController* getStarPointerOnOffController();
    bool always(s32);
    void onReaction(u64, s32, bool, bool, bool);
    bool checkPointingTarget(StarPointerTargetInfo*, const TVec3f&, const TVec2f&, f32, f32);
    bool checkPointingWithoutCheckZ(StarPointerTargetInfo*, const TVec3f&, const TVec2f&, f32, f32);
    bool isStarPointerPointingCore(StarPointerTargetInfo*, const LiveActor*, s32, StarPointerFunc1, StarPointerFunc2, bool, bool, bool);
};  // namespace

namespace MR {
    void initStarPointerGameScene();
    void createStarPointerLayout();
    void onStarPointerSceneOut();
    void setStarPointerModeBase();
    void setStarPointerCameraMtxAtGameScene();
    void initStarPointerTarget(LiveActor*, f32, const TVec3f&);
    void initStarPointerTargetAtPos(LiveActor*, f32, const TVec3f*, const TVec3f&);
    void initStarPointerTargetAtMtx(LiveActor*, f32, MtxPtr, const TVec3f&);
    void initStarPointerTargetAtJoint(LiveActor*, const char*, f32, const TVec3f&);
    void addStarPointerTargetCircle(LayoutActor*, const char*, f32, const TVec2f&, const char*);
    bool isStarPointerValid(s32);
    bool isStarPointerPointing(const LiveActor*, s32, bool, const char*);
    bool tryStarPointerCheck(const LiveActor*, bool, const char*);
    bool tryStarPointerCheckWithoutRumble(LiveActor*, bool);
    bool isStarPointerPointingPane(const LayoutActor*, const char*, s32, bool, const char*);
    bool isStarPointerPointingPaneForMeterLayout(const LayoutActor*, const char*, s32, bool, const char*);
    bool isStarPointerPointingTarget(const LayoutActor*, const char*, s32, bool, const char*);
    bool isStarPointerPointing1P(const LiveActor*, const char*, bool, bool);
    bool isStarPointerPointing1PWithoutCheckZ(const LiveActor*, const char*, bool, bool);
    bool isStarPointerPointing2P(const LiveActor*, const char*, bool, bool);
    bool isStarPointerPointing2POnPressButton(const LiveActor*, const char*, bool, bool);
    bool isStarPointerPointing2POnTriggerButton(const LiveActor*, const char*, bool, bool);
    bool isStarPointerPointingFileSelect(const LiveActor*);
    bool isStarPointerPointing1Por2P(const LiveActor*, const char*, bool, bool);
    bool requestTicoSeedGuidanceForce();
    bool requestPointerGuidanceNoInformation();
    bool isExistStarPointerGuidance();
    bool isExistStarPointerGuidanceFrame1P();
    void activeStarPointerGuidance();
    void deactiveStarPointerGuidance();
    void tryShowTimeoutedStarPointerGuidance();
    bool isExistStarPointerTarget(const LiveActor*);
    void setStarPointerTargetRadius3d(LiveActor*, f32);
    bool isStarPointerPointing(const TVec3f&, f32, s32, bool);
    MtxPtr getStarPointerViewMtx();
    MtxPtr getStarPointerProjMtx();
    f32 getStarPointerRadius(s32);
    TVec2f* getStarPointerScreenPosition(s32);
    TVec2f getStarPointerScreenPositionOrEdge(s32);
    f32 getStarPointerScreenSpeed(s32);
    bool isStarPointerInScreen(s32);
    bool isStarPointerInScreenAnyPort(s32*);
    s32* getStarPointerLastPointedPort(const LiveActor*);
    TVec2f* getStarPointerScreenVelocity(s32);
    void getStarPointerWorldVelocityDirection(TVec3f*, s32);
    TVec3f* getStarPointerWorldPosUsingDepth(s32);
    void calcStarPointerWorldPointingPos(TVec3f*, const TVec3f&, s32);
    void calcStarPointerWorldPointingPosInsideEdge(TVec3f*, const TVec3f&, s32);
    bool calcStarPointerPosOnPlane(TVec3f*, const TVec3f&, const TVec3f&, s32, bool);
    bool calcStarPointerWorldVelocityDirectionOnPlane(TVec3f*, const TVec3f&, const TVec3f&, s32);
    f32 calcPointRadius2D(const TVec3f&, f32);
    bool calcStarPointerStrokeRotateMoment(TVec3f*, const TVec3f&, f32, s32);
    bool calcStarPointerScreenDistanceToTarget(const LiveActor*, f32*, s32);
    bool tryStartStarPointerCommandStream(const LiveActor*, const TVec3f*, s32, bool);
    bool tryEndStarPointerCommandStream(const LiveActor*, s32);
    bool isStarPointerCommandStream(const LiveActor*, s32);
    void startStarPointerModeTitle(void*);
    void startStarPointerModeFileSelect(void*);
    void startStarPointerModeGame(void*);
    void startStarPointerModeDemo(void*);
    void startStarPointerModeDemoWithStarPointer(void*);
    void startStarPointerModeDemoWithHandPointerFinger(void*);
    void startStarPointerModeDemoMarioDeath(void*);
    void startStarPointerModeMarioLauncher(void*);
    void startStarPointerModeHomeButton(void*);
    void startStarPointerModeChooseYesNo(void*);
    void startStarPointerModePauseMenu(void*);
    void startStarPointerModeScenarioSelectScene(void*);
    void startStarPointerModeBlueStar(void*);
    void startStarPointerModePowerStarGetDemo(void*);
    void startStarPointerModeStarPieceTarget(void*);
    void startStarPointerModeSphereSelectorFinger(void*);
    void startStarPointerModeSphereSelectorOnReaction(void*);
    void startStarPointerModeEnding(void*);
    void startStarPointerModeCommandStream(void*);
    void startStarPointerMode1PInvalid2PValid(void*);
    void endStarPointerMode(void*);
    void requestStarPointerModeErrorWindow(void*);
    void requestStarPointerModeSaveLoad(void*);
    void requestStarPointerModePictureBook(void*);
    void requestStarPointerModePauseMenu(void*);
    void requestStarPointerModeBlueStarReady(void*);
    void requestStarPointerModeBigBubble(void*, const TVec3f&);
    bool isStarPointerModeBlueStarReady();
    bool isStarPointerModeStarPieceTarget();
    bool isStarPointerModeMarioLauncher();
    bool isStarPointerModeHomeButton();
    bool isStarPointerModeErrorWindow();
    void enableStarPointerShootStarPiece();
    void disableStarPointerShootStarPiece();
    bool isEnableStarPointerShootStarPiece(s32);
    bool isStarPointer2PTransparencyMode();
    bool isStarPointer1PInvalid2PValidMode();
    void setStarPointerDrawSyncToken();
    bool requestBlueStarGuidance();
    bool requestTicoSeedGuidance(s32);
    bool requestBigBubbleGuidance();
    bool requestMarioLauncherGuidance();
    bool requestFileSelectGuidance();
    bool requestFileSelectCopyGuidance();
    bool requestStarPieceLectureGuidance();
};  // namespace MR
