#include "Game/System/WPad.hpp"
#include "Game/System/WPadAcceleration.hpp"
#include "Game/System/WPadButton.hpp"
#include "Game/System/WPadHVSwing.hpp"
#include "Game/System/WPadHolder.hpp"
#include "Game/System/WPadPointer.hpp"
#include "Game/System/WPadStick.hpp"
#include "Game/Util/GamePadUtil.hpp"

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
        return MR::getWPad(channel)->mButton->testButtonUp();
    }

    bool testCorePadButtonDown(s32 channel) {
        return MR::getWPad(channel)->mButton->testButtonDown();
    }

    bool testCorePadButtonLeft(s32 channel) {
        return MR::getWPad(channel)->mButton->testButtonLeft();
    }

    bool testCorePadButtonRight(s32 channel) {
        return MR::getWPad(channel)->mButton->testButtonRight();
    }

    bool testCorePadButtonA(s32 channel) {
        return MR::getWPad(channel)->mButton->testButtonA();
    }

    bool testCorePadButtonB(s32 channel) {
        return MR::getWPad(channel)->mButton->testButtonB();
    }

    bool testCorePadButton1(s32 channel) {
        return MR::getWPad(channel)->mButton->testButton1();
    }

    bool testCorePadButton2(s32 channel) {
        return MR::getWPad(channel)->mButton->testButton2();
    }

    bool testCorePadButtonPlus(s32 channel) {
        return MR::getWPad(channel)->mButton->testButtonPlus();
    }

    bool testCorePadButtonMinus(s32 channel) {
        return MR::getWPad(channel)->mButton->testButtonMinus();
    }

    bool testSubPadButtonC(s32 channel) {
        return MR::getWPad(channel)->mButton->testButtonC();
    }

    bool testSubPadButtonZ(s32 channel) {
        return MR::getWPad(channel)->mButton->testButtonZ();
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
        return MR::getWPad(channel)->mButton->testTriggerUp();
    }

    bool testCorePadTriggerDown(s32 channel) {
        return MR::getWPad(channel)->mButton->testTriggerDown();
    }

    bool testCorePadTriggerLeft(s32 channel) {
        return MR::getWPad(channel)->mButton->testTriggerLeft();
    }

    bool testCorePadTriggerRight(s32 channel) {
        return MR::getWPad(channel)->mButton->testTriggerRight();
    }

    bool testCorePadTriggerA(s32 channel) {
        return MR::getWPad(channel)->mButton->testTriggerA();
    }

    bool testCorePadTriggerB(s32 channel) {
        return MR::getWPad(channel)->mButton->testTriggerB();
    }

    bool testCorePadTrigger1(s32 channel) {
        return MR::getWPad(channel)->mButton->testTrigger1();
    }

    bool testCorePadTrigger2(s32 channel) {
        return MR::getWPad(channel)->mButton->testTrigger2();
    }

    bool testCorePadTriggerPlus(s32 channel) {
        return MR::getWPad(channel)->mButton->testTriggerPlus();
    }

    bool testCorePadTriggerMinus(s32 channel) {
        return MR::getWPad(channel)->mButton->testTriggerMinus();
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
        return MR::getWPad(channel)->mButton->testTriggerHome();
    }

    bool testSubPadTriggerC(s32 channel) {
        return MR::getWPad(channel)->mButton->testTriggerC();
    }

    bool testSubPadTriggerZ(s32 channel) {
        return MR::getWPad(channel)->mButton->testTriggerZ();
    }

    bool testSubPadReleaseZ(s32 channel) {
        return MR::getWPad(channel)->mButton->testReleaseZ();
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
        return (MR::getWPad(channel)->mStick->mTrigger & 1) != 0;
    }

    bool testSubPadStickTriggerDown(s32 channel) {
        return (MR::getWPad(channel)->mStick->mTrigger & 2) != 0;
    }

    bool testSubPadStickTriggerLeft(s32 channel) {
        return (MR::getWPad(channel)->mStick->mTrigger & 8) != 0;
    }

    bool testSubPadStickTriggerRight(s32 channel) {
        return (MR::getWPad(channel)->mStick->mTrigger & 4) != 0;
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
        return testCorePadTriggerA(WPAD_CHAN0) != false;
    }

    bool testSystemTriggerA() {
        return testCorePadTriggerA(WPAD_CHAN0) != false;
    }

    bool testSystemTriggerB() {
        return testCorePadTriggerB(WPAD_CHAN0) != false;
    }

    bool testDPDMenuPadDecideTrigger() {
        return testCorePadTriggerA(WPAD_CHAN0);
    }

    bool testFpViewStartTrigger() {
        return testCorePadTriggerUp(WPAD_CHAN0);
    }

    bool testFpViewOutTrigger() {
        return testCorePadTriggerDown(WPAD_CHAN0) || testCorePadTriggerA(WPAD_CHAN0);
    }

    // getPlayerStickX
    // getPlayerStickY

    bool getPlayerTriggerA() {
        return testCorePadTriggerA(WPAD_CHAN0);
    }

    bool getPlayerTriggerB() {
        return testCorePadTriggerB(WPAD_CHAN0);
    }

    bool getPlayerTriggerZ() {
        return testSubPadTriggerZ(WPAD_CHAN0);
    }

    bool getPlayerTriggerC() {
        return testSubPadTriggerC(WPAD_CHAN0);
    }

    bool getPlayerLevelA() {
        return testCorePadButtonA(WPAD_CHAN0);
    }

    bool getPlayerLevelB() {
        return testCorePadButtonB(WPAD_CHAN0);
    }

    bool getPlayerLevelZ() {
        return testSubPadButtonZ(WPAD_CHAN0);
    }

    bool getPlayerLevelC() {
        return testSubPadButtonC(WPAD_CHAN0);
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

    bool isOperatingWPad(s32 channel) {
        WPad* pWPad = MR::getWPad(channel);

        if (!pWPad->mCorePadAccel->isBalanced()) {
            return true;
        }

        if (pWPad->mButton->testButtonA()) {
            return true;
        }

        return pWPad->mButton->testButtonB();
    }
};

namespace WPadFunction {
    WPadRumble* getWPadRumble(s32 channel) {
        return MR::getWPad(channel)->getRumbleInstance();
    }
};
