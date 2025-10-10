#include "Game/Util/GamePadUtil.hpp"
#include "Game/System/WPad.hpp"
#include "Game/System/WPadAcceleration.hpp"
#include "Game/System/WPadButton.hpp"
#include "Game/System/WPadHolder.hpp"
#include "Game/System/WPadHVSwing.hpp"
#include "Game/System/WPadPointer.hpp"
#include "Game/System/WPadStick.hpp"

namespace MR {
    void getCorePadPointingPosBasedOnScreen(TVec2f* pPos, s32 channel) {
        MR::getWPad(channel)->mPointer->getPointingPosBasedOnScreen(pPos);
    }

    void getCorePadPointingPos(TVec2f* pPos, s32 channel) {
        MR::getWPad(channel)->mPointer->getPointingPos(pPos);
    }

    void getCorePadPastPointingPos(TVec2f* pPos, s32 idx, s32 channel) {
        MR::getWPad(channel)->mPointer->getPastPointingPos(pPos, idx);
    }

    s32 getCorePadEnablePastCount(s32 channel) {
        return MR::getWPad(channel)->mPointer->getEnablePastCount();
    }

    bool isCorePadPointInScreen(s32 channel) {
        return MR::getWPad(channel)->mPointer->mIsPointInScreen;
    }

    f32 getCorePadDistanceToDisplay(s32 channel) {
        return MR::getWPad(channel)->mPointer->mDistDisplay;
    }

    void getCorePadAcceleration(TVec3f* pAccel, s32 channel) {
        MR::getWPad(channel)->mCorePadAccel->getAcceleration(pAccel);
    }

    bool testCorePadButtonUp(s32 channel) {
        return MR::getWPad(channel)->mButtons->testButtonUp();
    }

    bool testCorePadButtonDown(s32 channel) {
        return MR::getWPad(channel)->mButtons->testButtonDown();
    }

    bool testCorePadButtonLeft(s32 channel) {
        return MR::getWPad(channel)->mButtons->testButtonLeft();
    }

    bool testCorePadButtonRight(s32 channel) {
        return MR::getWPad(channel)->mButtons->testButtonRight();
    }

    bool testCorePadButtonA(s32 channel) {
        return MR::getWPad(channel)->mButtons->testButtonA();
    }

    bool testCorePadButtonB(s32 channel) {
        return MR::getWPad(channel)->mButtons->testButtonB();
    }

    bool testCorePadButton1(s32 channel) {
        return MR::getWPad(channel)->mButtons->testButton1();
    }

    bool testCorePadButton2(s32 channel) {
        return MR::getWPad(channel)->mButtons->testButton2();
    }

    bool testCorePadButtonPlus(s32 channel) {
        return MR::getWPad(channel)->mButtons->testButtonPlus();
    }

    bool testCorePadButtonMinus(s32 channel) {
        return MR::getWPad(channel)->mButtons->testButtonMinus();
    }

    bool testSubPadButtonC(s32 channel) {
        return MR::getWPad(channel)->mButtons->testButtonC();
    }

    bool testSubPadButtonZ(s32 channel) {
        return MR::getWPad(channel)->mButtons->testButtonZ();
    }

    bool testPadButtonAnyWithoutHome(s32 channel) {
        return testCorePadButtonUp(channel)
            || testCorePadButtonDown(channel)
            || testCorePadButtonLeft(channel)
            || testCorePadButtonRight(channel)
            || testCorePadButtonA(channel)
            || testCorePadButtonB(channel)
            || testCorePadButton1(channel)
            || testCorePadButton2(channel)
            || testCorePadButtonPlus(channel)
            || testCorePadButtonMinus(channel)
            || testSubPadButtonC(channel)
            || testSubPadButtonZ(channel);
    }

    bool testCorePadTriggerUp(s32 channel) {
        return MR::getWPad(channel)->mButtons->testTriggerUp();
    }

    bool testCorePadTriggerDown(s32 channel) {
        return MR::getWPad(channel)->mButtons->testTriggerDown();
    }

    bool testCorePadTriggerLeft(s32 channel) {
        return MR::getWPad(channel)->mButtons->testTriggerLeft();
    }

    bool testCorePadTriggerRight(s32 channel) {
        return MR::getWPad(channel)->mButtons->testTriggerRight();
    }

    bool testCorePadTriggerA(s32 channel) {
        return MR::getWPad(channel)->mButtons->testTriggerA();
    }

    bool testCorePadTriggerB(s32 channel) {
        return MR::getWPad(channel)->mButtons->testTriggerB();
    }

    bool testCorePadTrigger1(s32 channel) {
        return MR::getWPad(channel)->mButtons->testTrigger1();
    }

    bool testCorePadTrigger2(s32 channel) {
        return MR::getWPad(channel)->mButtons->testTrigger2();
    }

    bool testCorePadTriggerPlus(s32 channel) {
        return MR::getWPad(channel)->mButtons->testTriggerPlus();
    }

    bool testCorePadTriggerMinus(s32 channel) {
        return MR::getWPad(channel)->mButtons->testTriggerMinus();
    }

    bool testCorePadTriggerAnyWithoutHome(s32 channel) {
        return testCorePadTriggerUp(channel)
            || testCorePadTriggerDown(channel)
            || testCorePadTriggerLeft(channel)
            || testCorePadTriggerRight(channel)
            || testCorePadTriggerA(channel)
            || testCorePadTriggerB(channel)
            || testCorePadTrigger1(channel)
            || testCorePadTrigger2(channel)
            || testCorePadTriggerPlus(channel)
            || testCorePadTriggerMinus(channel);
    }

    bool testCorePadTriggerHome(s32 channel) {
        return MR::getWPad(channel)->mButtons->testTriggerHome();
    }

    bool testSubPadTriggerC(s32 channel) {
        return MR::getWPad(channel)->mButtons->testTriggerC();
    }

    bool testSubPadTriggerZ(s32 channel) {
        return MR::getWPad(channel)->mButtons->testTriggerZ();
    }

    bool testSubPadReleaseZ(s32 channel) {
        return MR::getWPad(channel)->mButtons->testReleaseZ();
    }

    bool isCorePadSwing(s32 channel) {
        return MR::getWPad(channel)->mCorePadSwing->mIsSwing;
    }

    bool isCorePadSwingTrigger(s32 channel) {
        return MR::getWPad(channel)->mCorePadSwing->mIsTriggerSwing;
    }

    f32 getSubPadStickX(s32 channel) {
        return MR::getWPad(channel)->mStick->mStick.x;
    }

    f32 getSubPadStickY(s32 channel) {
        return MR::getWPad(channel)->mStick->mStick.y;
    }

    bool testSubPadStickTriggerUp(s32 channel) {
        return (MR::getWPad(channel)->mStick->_14 >> 0) & 1;
    }

    bool testSubPadStickTriggerDown(s32 channel) {
        return (MR::getWPad(channel)->mStick->_14 >> 1) & 1;
    }

    bool testSubPadStickTriggerLeft(s32 channel) {
        return (MR::getWPad(channel)->mStick->_14 >> 3) & 1;
    }

    bool testSubPadStickTriggerRight(s32 channel) {
        return (MR::getWPad(channel)->mStick->_14 >> 2) & 1;
    }

    void getSubPadAcceleration(TVec3f* pAccel, s32 channel) {
        MR::getWPad(channel)->mSubPadAccel->getAcceleration(pAccel);
    }

    bool isSubPadSwing(s32 channel) {
        return MR::getWPad(channel)->mSubPadSwing->mIsSwing;
    }

    bool isPadSwing(s32 channel) {
        return isCorePadSwing(channel) || isSubPadSwing(channel);
    }

    bool testSystemPadTriggerDecide() {
        return testCorePadTriggerA(0) != false;
    }

    bool testSystemTriggerA() {
        return testCorePadTriggerA(0) != false;
    }

    bool testSystemTriggerB() {
        return testCorePadTriggerB(0) != false;
    }

    bool testDPDMenuPadDecideTrigger() {
        return testCorePadTriggerA(0);
    }

    bool testFpViewStartTrigger() {
        return testCorePadTriggerUp(0);
    }

    bool testFpViewOutTrigger() {
        return testCorePadTriggerDown(0) || testCorePadTriggerA(0);
    }

    // getPlayerStickX
    // getPlayerStickY

    bool getPlayerTriggerA() {
        return testCorePadTriggerA(0);
    }

    bool getPlayerTriggerB() {
        return testCorePadTriggerB(0);
    }

    bool getPlayerTriggerZ() {
        return testSubPadTriggerZ(0);
    }

    bool getPlayerTriggerC() {
        return testSubPadTriggerC(0);
    }

    bool getPlayerLevelA() {
        return testCorePadButtonA(0);
    }

    bool getPlayerLevelB() {
        return testCorePadButtonB(0);
    }

    bool getPlayerLevelZ() {
        return testSubPadButtonZ(0);
    }

    bool getPlayerLevelC() {
        return testSubPadButtonC(0);
    }

    bool isGamePadStickOperated(s32 channel) {
        f32 x = getSubPadStickX(channel);
        f32 y = getSubPadStickY(channel);

        return __fabsf(x) + __fabsf(y) > 0.0f;
    }

    // calcWorldStickDirectionXZ

    void calcWorldStickDirectionXZ(TVec3f* pDir, s32 channel) {
        pDir->y = 0.0f;

        calcWorldStickDirectionXZ(&pDir->x, &pDir->z, channel);
    }

    u32 getWPadMaxCount() {
        return 2;
    }

    bool isConnectedWPad(s32 channel) {
        return MR::getWPad(channel)->mIsConnected;
    }

    // isOperatingWPad
}

namespace WPadFunction {
    WPadRumble* getWPadRumble(s32 channel) {
        return MR::getWPad(channel)->getRumbleInstance();
    }
};
