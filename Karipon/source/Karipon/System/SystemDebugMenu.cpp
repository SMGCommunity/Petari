#include "Karipon/System/SystemDebugMenu.hpp"
#include "Game/NameObj/NameObjHolder.hpp"
#include "Game/System/FileLoader.hpp"
#include "Game/System/GameSystem.hpp"
#include "Game/System/GameSystemSceneController.hpp"
#include "Game/System/HeapMemoryWatcher.hpp"
#include "Game/System/WPad.hpp"
#include "Game/System/WPadButton.hpp"
#include "Game/System/WPadHolder.hpp"
#include "Game/Util/DirectDraw.hpp"
#include "Game/Util/ScreenUtil.hpp"
#include "Game/Util/SingletonHolder.hpp"
#include "Game/Util/SystemUtil.hpp"
#include "JSystem/JKernel/JKRExpHeap.hpp"
#include "JSystem/JKernel/JKRHeap.hpp"
#include "JSystem/JKernel/JKRSolidHeap.hpp"
#include "Macros.hpp"
#include "revolution/mtx.h"
#include <wstring.h>

extern "C" {
int vswprintf(wchar_t* s, size_t n, const wchar_t* format, va_list arg);
}

namespace {
    typedef void (*PageUpdateFunc)(SystemDebugMenu* pMenu);

    struct PageUpdateInfo {
        const char* mName;
        const PageUpdateFunc mFunc;
    };

    namespace DiagnosticsPage {
        static OSTick sLastTick;

        static void printHeapInfo(SystemDebugMenu* pMenu, const wchar_t* pHeapName, JKRHeap* pHeap) {
            if (!pHeap) {
                return;
            }

            u32 max = pHeap->mSize;
            u32 used = max - pHeap->getFreeSize();
            f32 usedPercent = static_cast<f32>(used) / static_cast<f32>(max) * 100.0f;

            pMenu->printTextF(false, L"%ls : %.02f%% usage, 0x%08X/0x%08X bytes used\n", pHeapName, usedPercent, used, max);
        }

        static void update(SystemDebugMenu* pMenu) {
            OSTick tick = OSGetTick();
            OSTick span = tick - sLastTick;
            sLastTick = tick;

            f32 fps = span == 0 ? 0.0f : static_cast<f32>(OS_TIMER_CLOCK) / static_cast<f32>(span);
            pMenu->printTextF(false, L"FPS : %.0f\n", fps);

            FileLoader* pFileLoader = SingletonHolder<FileLoader>::get();
            ArchiveHolder* pArchiveHolder = pFileLoader->mArchiveHolder;
            FileHolder* pFileHolder = pFileLoader->mFileHolder;
            pMenu->printTextF(false, L"File Info : %d requests, %d/%d archives, %d/%d files\n", pFileLoader->mRequestedFileCount,
                              pArchiveHolder->mEntries.mCount, pArchiveHolder->mEntries.capacity(), pFileHolder->mEntries.mCount,
                              pFileHolder->mEntries.capacity());

            NameObjHolder* pNameObjHolder = SingletonHolder<GameSystem>::get()->mSceneController->mObjHolder;
            pMenu->printTextF(false, L"Scene Object Info : %d objects | %d objects\n", pNameObjHolder->mObjArray1.mCount,
                              pNameObjHolder->mObjArray2.mCount);

            HeapMemoryWatcher* pWatcher = SingletonHolder<HeapMemoryWatcher>::get();
            printHeapInfo(pMenu, L"SystemHeap", JKRHeap::sSystemHeap);
            printHeapInfo(pMenu, L"StationedHeapNapa", pWatcher->mStationedHeapNapa);
            printHeapInfo(pMenu, L"StationedHeapGDDR", pWatcher->mStationedHeapGDDR);
            printHeapInfo(pMenu, L"GameHeapNapa", pWatcher->mGameHeapNapa);
            printHeapInfo(pMenu, L"GameHeapGDDR", pWatcher->mGameHeapGDDR);
            printHeapInfo(pMenu, L"FileCacheHeap", pWatcher->mFileCacheHeap);
            printHeapInfo(pMenu, L"SceneHeapNapa", pWatcher->mSceneHeapNapa);
            printHeapInfo(pMenu, L"SceneHeapGDDR", pWatcher->mSceneHeapGDDR);
            printHeapInfo(pMenu, L"WPadHeap", pWatcher->mWPadHeap);
            printHeapInfo(pMenu, L"HomeButtonLayoutHeap", pWatcher->mHomeButtonLayoutHeap);
            printHeapInfo(pMenu, L"AudSystemHeap", pWatcher->mAudSystemHeap);
        }
    } // namespace DiagnosticsPage

    static const f32 cFontSizeWidth = 11.0f;
    static const f32 cFontSizeHeight = 13.0f;
    static const f32 cFontLineSpace = 2.0f;
    static const f32 cFontCharSpace = 0.0f;
    static const PageUpdateInfo cPageUpdateFunc[] = {
        {"Diagnostics", ::DiagnosticsPage::update},
    };

    static s32 wrap(s32 num, s32 min, s32 max) {
        if (num < min) {
            num = max - 1;
        } else if (num >= max) {
            num = min;
        }

        return num;
    }
} // namespace

SystemDebugMenu::SystemDebugMenu() : mPageIndex(), mSelectIndex(), mSelectCurrentIndex(), mIsVisible(true) {}

SystemDebugMenu::~SystemDebugMenu() {}

void SystemDebugMenu::update() {
    WPadButton* pPadButton = MR::getWPad(0)->mButton;

    if (pPadButton->testTrigger2()) {
        mIsVisible = !mIsVisible;
    }

    if (!mIsVisible) {
        return;
    }

    mSelectCurrentIndex = 0;

    initWriter();
    printTextF(false, L"----- %s [%d/%d] -----\n", ::cPageUpdateFunc[mPageIndex].mName, mPageIndex + 1, ARRAY_LEN(::cPageUpdateFunc));
    ::cPageUpdateFunc[mPageIndex].mFunc(this);

    if (pPadButton->testButton1()) {
        if (pPadButton->testTriggerUp()) {
            mSelectIndex = wrap(mSelectIndex - 1, 0, mSelectCurrentIndex);
        }

        if (pPadButton->testTriggerDown()) {
            mSelectIndex = wrap(mSelectIndex + 1, 0, mSelectCurrentIndex);
        }

        if (pPadButton->testTriggerLeft()) {
            mPageIndex = wrap(mPageIndex - 1, 0, ARRAY_LEN(::cPageUpdateFunc));
            mSelectIndex = 0;
        }

        if (pPadButton->testTriggerRight()) {
            mPageIndex = wrap(mPageIndex + 1, 0, ARRAY_LEN(::cPageUpdateFunc));
            mSelectIndex = 0;
        }
    }
}

void SystemDebugMenu::initWriter() {
    mWriter = nw4r::ut::WideTextWriter(); // reset writer
    mWriter.SetCursor(0.0f, 0.0f);
    mWriter.SetFont(*MR::getFontOnCurrentLanguage());
    mWriter.SetFontSize(::cFontSizeWidth, ::cFontSizeHeight);
    mWriter.SetLineSpace(::cFontLineSpace);
    mWriter.SetCharSpace(::cFontCharSpace);

    nw4r::ut::Rect rect(0.0f, 0.0f, MR::getFrameBufferWidth(), MR::getFrameBufferHeight());

    Mtx mtx;
    PSMTXIdentity(mtx);

    mtx[0][3] = ((rect.left - rect.right) / 2.0f) + (::cFontSizeWidth * 2.0f); // x
    mtx[1][1] = -1.0f;
    mtx[1][3] = ((rect.bottom - rect.top) / 2.0f) - ::cFontSizeHeight; // y

    GXLoadPosMtxImm(mtx, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
}

void SystemDebugMenu::initDraw(u32 color) {
    nw4r::ut::Color col(color);
    mWriter.SetGradationMode(nw4r::ut::CharWriter::GRADMODE_NONE);
    mWriter.SetTextColor(col);

    nw4r::ut::Color minCol(0x00000000);
    nw4r::ut::Color maxCol(color);
    mWriter.SetColorMapping(minCol, maxCol);

    mWriter.SetupGX();
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
}

bool SystemDebugMenu::printText(bool selectable, const wchar_t* pText) {
    if (pText == nullptr) {
        return false;
    }

    u32 len = wcslen(pText);
    if (len == 0) {
        return false;
    }

    nw4r::math::VEC3 cursorPos(mWriter.mCursorPos);
    mWriter.SetCursorX(cursorPos.x + 2.0f);
    mWriter.SetCursorY(cursorPos.y + 2.0f);

    initDraw(0x000000C0);
    mWriter.Print(pText, len);
    mWriter.mCursorPos = cursorPos;

    bool ret = false;

    if (selectable) {
        if (mSelectIndex == mSelectCurrentIndex) {
            initDraw(0x00FFDDFF);

            WPadButton* pPadButton = MR::getWPad(0)->mButton;
            ret = pPadButton->testButton1() && pPadButton->testTriggerA();
        } else {
            initDraw(0xFFFFFFFF);
        }

        mSelectCurrentIndex++;
    } else {
        initDraw(0xFFFFFFFF);
    }

    mWriter.Print(pText, len);
    return ret;
}

bool SystemDebugMenu::printTextF(bool selectable, const wchar_t* pText, ...) {
    va_list list;
    wchar_t text[0x200];

    va_start(list, text);
    vswprintf(text, sizeof(text) / sizeof(*text), pText, list);
    va_end();

    return printText(selectable, text);
}

void SystemDebugMenu::printFillBox(const TVec2f& tl, const TVec2f& br, u32 color) {
    initDraw(0xFFFFFFFF);
    TDDraw::setup(0, 0, 2);
    TDDraw::drawFillBox(tl, br, color);
}
