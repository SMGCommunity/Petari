#pragma once

#include "JSystem/JGeometry/TVec.hpp"

class WPadRumble;

namespace MR {
    void getCorePadPointingPosBasedOnScreen(TVec2f*, s32);
    void getCorePadPointingPos(TVec2f*, s32);
    void getCorePadPastPointingPos(TVec2f*, s32, s32);
    s32 getCorePadEnablePastCount(s32);
    bool isCorePadPointInScreen(s32);
    f32 getCorePadDistanceToDisplay(s32);
    void getCorePadAcceleration(TVec3f*, s32);
    bool testCorePadButtonUp(s32);
    bool testCorePadButtonDown(s32);
    bool testCorePadButtonA(s32);
    bool testCorePadButtonB(s32);
    bool testCorePadButtonPlus(s32);
    bool testCorePadButtonMinus(s32);
    bool testSubPadButtonC(s32);
    bool testSubPadButtonZ(s32);
    bool testPadButtonAnyWithoutHome(s32);
    bool testCorePadTriggerUp(s32);
    bool testCorePadTriggerDown(s32);
    bool testCorePadTriggerLeft(s32);
    bool testCorePadTriggerRight(s32);
    bool testCorePadTriggerA(s32);
    bool testCorePadTriggerB(s32);
    bool testCorePadTriggerPlus(s32);
    bool testCorePadTriggerMinus(s32);
    bool testCorePadTriggerAnyWithoutHome(s32);
    bool testCorePadTriggerHome(s32);
    bool testSubPadTriggerC(s32);
    bool testSubPadTriggerZ(s32);
    bool testSubPadReleaseZ(s32);
    bool isCorePadSwing(s32);
    bool isCorePadSwingTrigger(s32);
    f32 getSubPadStickX(s32);
    f32 getSubPadStickY(s32);
    bool testSubPadStickTriggerUp(s32);
    bool testSubPadStickTriggerDown(s32);
    bool testSubPadStickTriggerLeft(s32);
    bool testSubPadStickTriggerRight(s32);
    void getSubPadAcceleration(TVec3f*, s32);
    bool isSubPadSwing(s32);
    bool isPadSwing(s32);
    bool testSystemPadTriggerDecide();
    bool testSystemTriggerA();
    bool testSystemTriggerB();
    bool testDPDMenuPadDecideTrigger();
    bool testFpViewStartTrigger();
    bool testFpViewOutTrigger();
    f32 getPlayerStickX();
    f32 getPlayerStickY();
    bool getPlayerTriggerA();
    bool getPlayerTriggerB();
    bool getPlayerTriggerZ();
    bool getPlayerTriggerC();
    bool getPlayerLevelA();
    bool getPlayerLevelB();
    bool getPlayerLevelZ();
    bool getPlayerLevelC();
    bool isGamePadStickOperated(s32);
    void calcWorldStickDirectionXZ(f32*, f32*, s32);
    void calcWorldStickDirectionXZ(TVec3f*, s32);
    u32 getWPadMaxCount();
    bool isConnectedWPad(s32);
    bool isOperatingWPad(s32);
};

namespace WPadFunction {
    WPadRumble* getWPadRumble(s32);
};
