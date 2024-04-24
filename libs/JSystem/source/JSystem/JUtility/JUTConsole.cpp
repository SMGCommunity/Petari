#include "JSystem/JUtility/JUTConsole.hpp"
#include <cerrno>

void JUTConsoleManager::drawDirect(bool a1) const {
    if (mDirectConsole != nullptr) {
        if (a1) { 
            BOOL en = OSEnableInterrupts();
            u32 startRetraceCount = VIGetRetraceCount();
            u32 curRetraceCount = startRetraceCount;
            do {
                curRetraceCount = VIGetRetraceCount();
            } while(startRetraceCount == curRetraceCount);

            OSRestoreInterrupts(en);
        }

        mDirectConsole->doDraw(JUTConsole::CONSOLE_TYPE_2);
    }
}

void JUTConsoleManager::setDirectConsole(JUTConsole *pConsole) {
    if (mDirectConsole != nullptr) {
        appendConsole(mDirectConsole);
    }

    if (pConsole != nullptr) {
        removeConsole(pConsole);
    }

    mDirectConsole = pConsole;
}