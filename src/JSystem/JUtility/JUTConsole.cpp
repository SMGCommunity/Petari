#include "JSystem/JUtility/JUTConsole.hpp"
#include "JSystem/J2DGraph/J2DOrthoGraph.hpp"
#include "JSystem/JKernel/JKRHeap.hpp"
#include "JSystem/JUtility/JUTDirectPrint.hpp"
#include "JSystem/JUtility/JUTFont.hpp"
#include "JSystem/JUtility/JUTVideo.hpp"
#include <cerrno>
#include <cstdarg>
#include <new>

#pragma inline_max_size(250)

JUTConsoleManager* JUTConsoleManager::sManager;
static JUTConsole* sReportConsole;
static JUTConsole* sWarningConsole;

JUTConsole* JUTConsole::create(unsigned int param_0, void* buffer, u32 bufferSize) {
    JUTConsoleManager* pManager = JUTConsoleManager::getManager();

    u32 maxLines = getLineFromObjectSize(bufferSize, param_0);

    JUTConsole* console = new (buffer) JUTConsole(param_0, maxLines, false);
    console->mBuf = static_cast< u8* >(buffer) + sizeof(JUTConsole);
    console->clear();

    pManager->appendConsole(console);
    return console;
}

JUTConsole::JUTConsole(unsigned int param_0, unsigned int maxLines, bool param_2) {
    field_0x2c = param_2;
    field_0x20 = param_0;
    mMaxLines = maxLines;

    mPositionX = 30;
    mPositionY = 50;
    mHeight = 20;

    if (mHeight > mMaxLines) {
        mHeight = mMaxLines;
    }

    mFont = nullptr;
    mVisible = true;
    field_0x69 = false;
    field_0x6a = false;
    field_0x6b = false;
    mOutput = 1;

    field_0x5c = JUtility::TColor(0, 0, 0, 100);
    field_0x60 = JUtility::TColor(0, 0, 0, 230);
    field_0x64 = 8;
}

JUTConsole::~JUTConsole() {
    JUTConsoleManager::getManager()->removeConsole(this);
}

size_t JUTConsole::getLineFromObjectSize(u32 bufferSize, unsigned int param_1) {
    bufferSize -= sizeof(JUTConsole);
    size_t result = (bufferSize) / (param_1 + 2);
    return result;
}

void JUTConsole::clear() {
    field_0x30 = 0;
    field_0x34 = 0;
    field_0x38 = 0;
    field_0x3c = 0;

    for (u32 i = 0; i < mMaxLines; i++) {
        setLineAttr(i, 0);
    }

    setLineAttr(0, -1);
    *getLinePtr(0) = 0;
}

void JUTConsole::doDraw(JUTConsole::EConsoleType consoleType) const {
    f32 font_yOffset;

    if (mVisible && (mFont != nullptr || consoleType == CONSOLE_TYPE_2)) {
        if (mHeight != 0) {
            bool spA = consoleType == CONSOLE_TYPE_0 ? true : false;
            font_yOffset = 2.0f + mFontSizeY;

            if (consoleType != CONSOLE_TYPE_2) {
                if (JUTGetVideoManager() == nullptr) {
                    J2DOrthoGraph ortho(0.0f, 0.0f, 640.0f, 480.0f, -1.0f, 1.0f);
                    ortho.setPort();
                } else {
                    J2DOrthoGraph ortho(0.0f, 0.0f, JUTGetVideoManager()->getFbWidth(), JUTGetVideoManager()->getEfbHeight(), -1.0f, 1.0f);
                    ortho.setPort();
                }

                J2DFillBox(mPositionX - 2, static_cast< int >(mPositionY - font_yOffset), static_cast< int >((mFontSizeX * field_0x20) + 4.0f),
                           static_cast< int >(font_yOffset * mHeight), spA ? field_0x60 : field_0x5c);
                mFont->setGX();

                if (spA) {
                    if (((diffIndex(field_0x30, field_0x38) - static_cast< int >(mHeight)) + 1) <= 0) {
                        mFont->setCharColor(JUtility::TColor(255, 255, 255, 255));
                    } else if (field_0x30 == field_0x34) {
                        mFont->setCharColor(JUtility::TColor(255, 230, 230, 255));
                    } else {
                        mFont->setCharColor(JUtility::TColor(230, 230, 255, 255));
                    }
                } else {
                    mFont->setCharColor(JUtility::TColor(230, 230, 230, 255));
                }
            } else {
                JUTDirectPrint::getManager()->erase(mPositionX - 3, mPositionY - 2, (field_0x20 * 6) + 6,
                                                    static_cast< int >(font_yOffset * mHeight) + 4);
                JUTDirectPrint::getManager()->setCharColor(JUtility::TColor(255, 255, 255, 255));
            }

            s32 curLine = field_0x30;
            s32 yFactor = 0;
            u8* linePtr;

            while (true) {
                linePtr = getLinePtr(curLine);
                u8 sp9 = linePtr[-1];
                if (sp9 == 0) {
                    break;
                }

                if (consoleType != CONSOLE_TYPE_2) {
                    mFont->drawString_scale(mPositionX, ((yFactor * font_yOffset) + mPositionY), mFontSizeX, mFontSizeY,
                                            reinterpret_cast< char* >(linePtr), true);
                } else {
                    JUTDirectPrint::getManager()->drawString(mPositionX, ((yFactor * font_yOffset) + mPositionY), reinterpret_cast< char* >(linePtr));
                }
                curLine = nextIndex(curLine);
                yFactor++;

                if (yFactor >= mHeight || curLine == field_0x34) {
                    break;
                }
            }
        }
    }
}

void JUTConsole::print_f(char const* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    print_f_va(fmt, args);
    va_end(args);
}

void JUTConsole::print(char const* str) {
    if (mOutput & 1) {
        u8* src = reinterpret_cast< u8* >(const_cast< char* >(str));
        u8* dst = getLinePtr(field_0x38) + field_0x3c;
        while (*src != 0) {
            if (field_0x6a && field_0x34 == nextIndex(field_0x38)) {
                break;
            }
            if (*src == '\n') {
                src++;
                field_0x3c = field_0x20;
            } else if (*src == '\t') {
                src++;
                while (field_0x3c < field_0x20) {
                    *dst++ = ' ';
                    field_0x3c++;
                    if (field_0x3c % field_0x64 == 0) {
                        break;
                    }
                }
            } else if (mFont && mFont->isLeadByte(*src)) {
                if (field_0x3c + 1 < field_0x20) {
                    *dst++ = *src++;
                    *dst++ = *src++;
                    field_0x3c++;
                    field_0x3c++;
                } else {
                    *dst++ = '\0';
                    field_0x3c++;
                }
            } else {
                *dst++ = *src++;
                field_0x3c++;
            }

            if (field_0x3c < field_0x20) {
                continue;
            }
            *dst = '\0';
            field_0x38 = nextIndex(field_0x38);
            field_0x3c = 0;
            setLineAttr(field_0x38, 0xff);
            dst = getLinePtr(field_0x38);
            *dst = '\0';
            int local_28 = diffIndex(field_0x30, field_0x38);
            if (local_28 == mHeight) {
                field_0x30 = nextIndex(field_0x30);
            }
            if (field_0x38 == field_0x34) {
                field_0x34 = nextIndex(field_0x34);
            }
            if (field_0x38 == field_0x30) {
                field_0x30 = nextIndex(field_0x30);
            }

            if (field_0x6b) {
                break;
            }
        }
        *dst = '\0';
    }
}

extern "C" void JUTConsole_print_f_va_(JUTConsole* console, const char* fmt, va_list args) {
    char buf[1024];
    int len = vsnprintf(buf, sizeof(buf), fmt, args);
    console->print(buf);
}

void JUTConsole::scroll(int scrollAmnt) {
    if (scrollAmnt < 0) {
        int diff = diffIndex(field_0x34, field_0x30);
        if (scrollAmnt < -diff) {
            scrollAmnt = -diff;
        }
    } else if (scrollAmnt > 0) {
        if (diffIndex(field_0x34, field_0x38) + 1 <= mHeight) {
            scrollAmnt = 0;
        } else {
            int r27 = diffIndex(field_0x30, field_0x38) - mHeight + 1;
            if (scrollAmnt > r27) {
                scrollAmnt = r27;
            }
        }
    }

    field_0x30 += scrollAmnt;
    if (field_0x30 < 0) {
        field_0x30 += mMaxLines;
    }

    if (field_0x30 >= static_cast< u32 >(mMaxLines)) {
        field_0x30 -= mMaxLines;
    }
}

int JUTConsole::getUsedLine() const {
    int result = diffIndex(field_0x34, field_0x38);
    return result;
}

int JUTConsole::getLineOffset() const {
    int result = diffIndex(field_0x34, field_0x30);
    return result;
}

JUTConsoleManager::JUTConsoleManager() {
    mActiveConsole = nullptr;
    mDirectConsole = nullptr;
}

JUTConsoleManager* JUTConsoleManager::createManager(JKRHeap* pHeap) {
    if (pHeap == nullptr) {
        pHeap = JKRGetCurrentHeap();
    }

    sManager = new (pHeap, 0) JUTConsoleManager();
    return sManager;
}

void JUTConsoleManager::appendConsole(JUTConsole* console) {
    mList.Insert(mList.end(), console);

    if (mActiveConsole == nullptr) {
        mActiveConsole = console;
    }
}

void JUTConsoleManager::removeConsole(JUTConsole* console) {
    if (mActiveConsole == console) {
        if (mList.size() <= 1) {
            mActiveConsole = nullptr;
        } else {
            ConsoleList::iterator last = mList.end();
            --last;
            mActiveConsole = console != *last ? mList.Element_toValue(console->mListNode.getNext()) : *mList.begin();
        }
    }

    if (JUTGetWarningConsole() == console)
        sWarningConsole = nullptr;
    if (JUTGetReportConsole() == console)
        sReportConsole = nullptr;

    mList.Remove(console);
}

void JUTConsoleManager::draw() const {
    ConsoleList& consoles = const_cast< ConsoleList& >(mList);
    ConsoleList::iterator it = consoles.begin();
    ConsoleList::iterator end = consoles.end();
    for (; it != end; ++it) {
        if (*it != mActiveConsole) {
            (*it)->doDraw(JUTConsole::CONSOLE_TYPE_1);
        }
    }
    if (mActiveConsole != nullptr) {
        mActiveConsole->doDraw(JUTConsole::CONSOLE_TYPE_0);
    }
}

void JUTConsoleManager::drawDirect(bool a1) const {
    if (mDirectConsole != nullptr) {
        if (a1) {
            BOOL en = OSEnableInterrupts();
            u32 startRetraceCount = VIGetRetraceCount();
            u32 curRetraceCount = startRetraceCount;
            do {
                curRetraceCount = VIGetRetraceCount();
            } while (startRetraceCount == curRetraceCount);

            OSRestoreInterrupts(en);
        }

        mDirectConsole->doDraw(JUTConsole::CONSOLE_TYPE_2);
    }
}

void JUTConsoleManager::setDirectConsole(JUTConsole* pConsole) {
    if (mDirectConsole != nullptr) {
        appendConsole(mDirectConsole);
    }

    if (pConsole != nullptr) {
        removeConsole(pConsole);
    }

    mDirectConsole = pConsole;
}

extern "C" JUTConsole* JUTGetReportConsole() {
    return sReportConsole;
}

extern "C" JUTConsole* JUTGetWarningConsole() {
    return sWarningConsole;
}

extern "C" void JUTReportConsole_f_va(const char* fmt, va_list args) {
    char buf[256];
    int len;

    if (JUTGetReportConsole() == nullptr) {
        len = vsnprintf(buf, sizeof(buf), fmt, args);
    } else if (JUTGetReportConsole()->getOutput() & (JUTConsole::OUTPUT_CONSOLE | JUTConsole::OUTPUT_OSREPORT)) {
        len = vsnprintf(buf, sizeof(buf), fmt, args);
        JUTGetReportConsole()->print(buf);
    }
}

extern "C" void JUTReportConsole_f(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    JUTReportConsole_f_va(fmt, args);
    va_end(args);
}

extern "C" void JUTReportConsole(const char* message) {
    JUTReportConsole_f("%s", message);
}

extern "C" void JUTWarningConsole_f_va(const char* fmt, va_list args) {
    char buf[256];
    int len;

    if (JUTGetWarningConsole() == nullptr) {
        len = vsnprintf(buf, sizeof(buf), fmt, args);
    } else if (JUTGetWarningConsole()->getOutput() & (JUTConsole::OUTPUT_CONSOLE | JUTConsole::OUTPUT_OSREPORT)) {
        len = vsnprintf(buf, sizeof(buf), fmt, args);
        JUTGetWarningConsole()->print(buf);
    }
}
