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

    bool testCorePadButtonA(s32 channel) {
        return MR::getWPad(channel)->mButtons->testButtonA();
    }

    bool testCorePadButtonB(s32 channel) {
        return MR::getWPad(channel)->mButtons->testButtonB();
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

    bool testCorePadTriggerPlus(s32 channel) {
        return MR::getWPad(channel)->mButtons->testTriggerPlus();
    }

    bool testCorePadTriggerMinus(s32 channel) {
        return MR::getWPad(channel)->mButtons->testTriggerMinus();
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
        return MR::getWPad(channel)->mStick->_14 & 1;
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
        return MR::getWPad(channel)->mCorePadSwing->mIsSwing || MR::getWPad(channel)->mSubPadSwing->mIsSwing;
    }

    bool testSystemPadTriggerDecide() {
        return MR::getWPad(0)->mButtons->testTriggerA() != 0;
    }

    bool testSystemTriggerA() {
        return MR::getWPad(0)->mButtons->testTriggerA() != 0;
    }

    bool testSystemTriggerB() {
        return MR::getWPad(0)->mButtons->testTriggerB() != 0;
    }

    bool testDPDMenuPadDecideTrigger() {
        return MR::getWPad(0)->mButtons->testTriggerA();
    }

    bool testFpViewStartTrigger() {
        return MR::getWPad(0)->mButtons->testTriggerUp();
    }

    bool testFpViewOutTrigger() {
        return MR::getWPad(0)->mButtons->testTriggerDown() || MR::getWPad(0)->mButtons->testTriggerA();
    }

    bool getPlayerTriggerA() {
        return MR::getWPad(0)->mButtons->testTriggerA();
    }

    bool getPlayerTriggerB() {
        return MR::getWPad(0)->mButtons->testTriggerB();
    }

    bool getPlayerTriggerZ() {
        return MR::getWPad(0)->mButtons->testTriggerZ();
    }

    bool getPlayerTriggerC() {
        return MR::getWPad(0)->mButtons->testTriggerC();
    }

    bool getPlayerLevelA() {
        return MR::getWPad(0)->mButtons->testButtonA();
    }

    bool getPlayerLevelB() {
        return MR::getWPad(0)->mButtons->testButtonB();
    }

    bool getPlayerLevelZ() {
        return MR::getWPad(0)->mButtons->testButtonZ();
    }

    bool getPlayerLevelC() {
        return MR::getWPad(0)->mButtons->testButtonC();
    }

    u32 getWPadMaxCount() {
        return 2;
    }

    bool isConnectedWPad(s32 channel) {
        return MR::getWPad(channel)->mIsConnected;
    }
}

namespace WPadFunction {
    WPadRumble* getWPadRumble(s32 channel) {
        return MR::getWPad(channel)->getRumbleInstance();
    }
};
