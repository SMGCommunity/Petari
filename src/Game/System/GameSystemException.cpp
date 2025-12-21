#include "Game/System/GameSystemException.hpp"
#include "Game/SingletonHolder.hpp"
#include "Game/System/GameSystem.hpp"
#include "Game/System/GameSystemObjHolder.hpp"
#include "Game/Util/GamePadUtil.hpp"
#include <JSystem/JUtility/JUTAssert.hpp>
#include <JSystem/JUtility/JUTConsole.hpp>
#include <JSystem/JUtility/JUTDirectPrint.hpp>
#include <JSystem/JUtility/JUTException.hpp>
#include <JSystem/JUtility/JUTVideo.hpp>

void* GameSystemException::sMapFileUsingBuffer;

namespace {
    bool isBootWPAD() {
        bool ret = false;
        GameSystemObjHolder* objHolder = SingletonHolder< GameSystem >::get()->mObjHolder;
        if (objHolder != nullptr && objHolder->mWPadHolder != nullptr) {
            ret = true;
        }

        return ret;
    }

    const s32 cDispExceptionCommand[] = {8, 4, 1, 2, 0x100, 0x10, 0, 0};
};  // namespace

void GameSystemException::init() {
    JUTDirectPrint* print = JUTDirectPrint::start();
    JUTAssertion::create();
    JUTAssertion::changeDisplayTime(600);
    JUTAssertion::changeDevice(3);
    JUTConsoleManager::createManager(nullptr);
    JUTException::create(print);
    JUTException::createConsole(new u8[0x24FC], 0x24FC);
    JUTException::setPreUserCallback(GameSystemException::handleException);
    JUTException::sErrorManager->mPrintWaitTime1 = 0;
    JUTException::sErrorManager->mPrintWaitTime0 = 0;
    JUTConsole* console = JUTException::getConsole();
    console->mPositionX = 0xF;
    console->mPositionY = 0x30;
    console = JUTException::getConsole();
    console->mHeight = 0x16;

    if (console->mHeight > console->mMaxLines) {
        console->mHeight = console->mMaxLines;
    }

    GameSystemException::sMapFileUsingBuffer = new u8[0x10];
}

void GameSystemException::handleException(u16 a1, OSContext* pContext, u32 a3, u32 a4) {
    if (!JUTVideo::sManager) {
        JUTException::sConsole->mOutput = 2;
        JUTException::sConsole->mVisible = false;
        JUTAssertion::setVisible(false);
    }

    VIFlush();
    JUTAssertion::flushMessage();

    if (::isBootWPAD()) {
        for (u32 i = 0; i < MR::getWPadMaxCount(); i++) {
            WPADControlMotor(i, 0);
        }

        JUTException* exception = JUTException::sErrorManager;
        JUTException::sErrorManager->_84 = -1;
        exception->mGamePadPort = JUTGamePad::Port_Unknown;
    }

    AIRegisterDMACallback(0);

    if (::isBootWPAD()) {
        s32 v3 = 0;
        u32 v6 = 0;
        u32 v5 = 0;
        s32 v4;
        while (cDispExceptionCommand[v3]) {
            JUTException::waitTime(100);
            JUTException::sErrorManager->readPad(&v6, &v5);

            if (v6 != 0) {
                if ((cDispExceptionCommand[v3] & v6) == cDispExceptionCommand[v3]) {
                    v3++;
                } else {
                    v3 = 0;
                }
            }
        }
    }
}
