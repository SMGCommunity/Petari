#include "JSystem/JUtility/JUTAssert.hpp"
#include "JSystem/JUtility/JUTConsole.hpp"
#include "JSystem/JUtility/JUTDbPrint.hpp"
#include "JSystem/JUtility/JUTDirectPrint.hpp"
#include "JSystem/JUtility/JUTFont.hpp"
#include <cstdio>
#include <revolution/vi.h>

bool sAssertVisible = true;

namespace JUTAssertion {

    namespace {
        static u32 sMessageLife;
        static char sMessageFileLine[64];
        static char sMessageString[256];
        static s32 sDisplayTime = -1;
        static s32 sDevice = 3;
        static bool mVisible = true;

    };  // namespace

    void create() {}

    u32 flush_subroutine() {
        if (sMessageLife == 0) {
            return 0;
        }

        if (sMessageLife != -1) {
            sMessageLife--;
        }

        if (sMessageLife >= 5) {
            return sMessageLife;
        }

        return 0;
    }

    void flushMessage() {
        if (flush_subroutine() && sAssertVisible == true) {
            JUTDirectPrint* manager = JUTDirectPrint::getManager();
            JUtility::TColor color = manager->getCharColor();
            manager->setCharColor(JUtility::TColor(255, 200, 200, 255));
            manager->drawString(16, 16, sMessageFileLine);
            manager->drawString(16, 24, sMessageString);
            manager->setCharColor(color);
        }
    }

    void flushMessage_dbPrint() {
        if (flush_subroutine() && sAssertVisible == true && JUTDbPrint::getManager() != NULL) {
            JUTFont* font = JUTDbPrint::getManager()->getFont();
            if (font != NULL) {
                u8 tmp = ((VIGetRetraceCount() & 0x3C) << 2) | 0xF;
                font->setGX();
                font->setCharColor(JUtility::TColor(255, tmp, tmp, 255));
                font->drawString(30, 36, sMessageFileLine, true);
                font->drawString(30, 54, sMessageString, true);
            }
        }
    }

    void changeDisplayTime(u32 time) {
        sDisplayTime = time;
    }

    void changeDevice(u32 device) {
        sDevice = device;
    }

    void setVisible(bool visible) {
        sAssertVisible = visible;
    }

};  // namespace JUTAssertion
