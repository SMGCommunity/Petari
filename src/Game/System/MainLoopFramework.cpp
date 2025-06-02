#include "Game/System/DrawSyncManager.hpp"
#include "Game/System/MainLoopFramework.hpp"
#include "Game/Util/BothDirList.hpp"
#include "Game/Util/SchedulerUtil.hpp"
#include <JSystem/J2DGraph/J2DOrthoGraph.hpp>
#include <JSystem/JUtility/JUTAssert.hpp>
#include <JSystem/JUtility/JUTConsole.hpp>
#include <JSystem/JUtility/JUTDirectPrint.hpp>
#include <JSystem/JUtility/JUTVideo.hpp>
#include <JSystem/JUtility/JUTXfb.hpp>
#include <runtime.h>

s8 lbl_806B6ED4;
s64 nextTime;
s8 timeInitialized;
u32 nextCount;
s8 countInitialized;

namespace {
    void handleGXAbortAlarm(OSAlarm* alarm, OSContext* context);

    void waitForTick(u32 tickDuration, u16 retraceCount) NO_INLINE {
        if (tickDuration) {
            if (!timeInitialized) {
                nextTime = OSGetTime();
                timeInitialized = 1;
            }
            s64 currentTime = OSGetTime();
            while (currentTime < nextTime) {
                OSSleepTicks(nextTime - currentTime);
                currentTime = OSGetTime();
            }
            nextTime = currentTime + tickDuration;
        }
        else {
            if (!countInitialized) {
                nextCount = VIGetRetraceCount();
                countInitialized = 1;
            }

            u32 count = retraceCount ? retraceCount : 1;
            u32 dummy = 0;

            u32 msg;
            do {
                if (!OSReceiveMessage(&JUTVideo::sManager->mMessageQueue, (OSMessage*)&msg, 1)) {
                    msg = dummy;
                }
            } while((s32)(msg - nextCount) < 0);
            nextCount = msg + count;
        }
    }

    void waitDrawDoneAndSetAlarm() NO_INLINE {
        OSAlarm alarm;
        MR::BothDirPtrList* list;
        MR::BothDirPtrLink link(&alarm);
        {
            MR::ProhibitSchedulerAndInterrupts prohibit(false);
            OSCreateAlarm(&alarm);
            list = &MainLoopFrameworkAlarm::sList;
            list->append(&link);
        }
        OSTime tick = __cvt_dbl_usll(OS_BUS_CLOCK / 4 * 0.5);
        OSSetAlarm(&alarm, tick, &handleGXAbortAlarm);
        GXDrawDone();
        DrawSyncManager::resetIfAborted();
        {
            MR::ProhibitSchedulerAndInterrupts prohibit(false);
            OSCancelAlarm(&alarm);
            list->remove(&link);
        }
    }
}

void callDirectDraw() {
    u16 efbHeight, fbWidth;
    fbWidth = JUTVideo::sManager->mRenderModeObj->fbWidth;
    efbHeight = JUTVideo::sManager->mRenderModeObj->efbHeight;
    void* xfb = JUTXfb::sManager->getDrawingXfb();
    JUTDirectPrint::sDirectPrint->changeFrameBuffer(xfb, fbWidth, efbHeight);
    JUTAssertion::flushMessage();
}

void MainLoopFramework::prepareCopyDisp() {
    u16 fbWidth = JUTVideo::sManager->mRenderModeObj->fbWidth;
    u16 efbHeight = JUTVideo::sManager->mRenderModeObj->efbHeight;
    u16 xfbHeight = JUTVideo::sManager->mRenderModeObj->xfbHeight;
    f32 yscale = GXGetYScaleFactor(efbHeight, xfbHeight);
    u16 nlines = GXGetNumXfbLines(efbHeight, yscale);
    GXSetCopyClear(_0, _4);
    GXSetDispCopySrc(0, 0, fbWidth, efbHeight);
    GXSetDispCopyDst(fbWidth, nlines);
    GXSetDispCopyYScale(yscale);
    VIFlush();
    GXSetCopyFilter(
        JUTVideo::sManager->mRenderModeObj->aa,
        JUTVideo::sManager->mRenderModeObj->sample_pattern,
        _19,
        JUTVideo::sManager->mRenderModeObj->vfilter
    );
    GXSetCopyClamp((GXFBClamp)_A);
    GXSetDispCopyGamma((GXGamma)_8);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
    if (_18) {
        GXSetAlphaUpdate(GX_TRUE);
    }
}

void MainLoopFramework::drawendXfb_single() {
    JUTXfb* xfb = JUTXfb::sManager;
    if (xfb->_14 >= 0) {
        prepareCopyDisp();
        waitDrawDoneAndSetAlarm();
        GXFlush();
        xfb->_16 = xfb->_14;
    }
}

void MainLoopFramework::exchangeXfb_double() {
    JUTXfb* xfb = JUTXfb::sManager;
    if (xfb->_16 == xfb->_18) {
        if (xfb->_14 >= 0) {
            if (_38) {
                _38();
            }
            prepareCopyDisp();
            GXCopyDisp(xfb->getDrawingXfb(), GX_TRUE);
            if (!_C) {
                xfb->_16 = xfb->_14;
                GXDrawDone();
                JUTVideo::dummyNoDrawWait();
            }
            else {
                JUTVideo::drawDoneStart();
            }
            if (!_C) {
                callDirectDraw();
            }
        }
        s16 temp = xfb->_14;
        xfb->_16 = temp;
        xfb->_14 = temp >= 0 ? temp ^ 1 : 0;
    }
    else {
        clearEfb(_0);
        if (xfb->_14 < 0) {
            xfb->_14 = 0;
        }
    }
}

void MainLoopFramework::preGX() {
    GXInvalidateTexAll();
    GXInvalidateVtxCache();
    if (JUTVideo::sManager->mRenderModeObj->aa) {
        GXSetPixelFmt(GX_PF_RGB565_Z16, GX_ZC_LINEAR);
        GXSetDither(GX_TRUE);
    }
    else if (_18) {
        GXSetPixelFmt(GX_PF_RGBA6_Z24, GX_ZC_LINEAR);
        GXSetDither(GX_TRUE);
    }
    else {
        GXSetPixelFmt(GX_PF_RGB8_Z24, GX_ZC_LINEAR);
        GXSetDither(GX_FALSE);
    }
}

void MainLoopFramework::endGX() {
    J2DOrthoGraph ortho(
        0f, 0f, 
        JUTVideo::sManager->mRenderModeObj->fbWidth, 
        JUTVideo::sManager->mRenderModeObj->efbHeight, 
        -1f, 1f
    );
    if (JUTConsoleManager::sManager) {
        ortho.setPort();
        JUTConsoleManager::sManager->draw();
    }
    if (_C || JUTXfb::sManager->_10 == 1) {
        JUTAssertion::flushMessage_dbPrint();
    }
    GXFlush();
}

void MainLoopFramework::waitForRetrace() {
    waitForTick(mTickDuration, mRetraceCount);
    JUTVideo::sManager->waitRetraceIfNeed();
}

void MainLoopFramework::beginRender() {
    OSTick currentTick = OSGetTick();
    _24 = currentTick - mTick;
    mTick = currentTick;

    _28 = currentTick - JUTVideo::sVideoLastTick;

    if (_34) {
        JUTXfb* xfb = JUTXfb::sManager;
        switch (xfb->_10) {
            case 1:
                if (xfb->_1C != 2) {
                    xfb->_1C = 1;
                    clearEfb(_0);
                }
                else {
                    xfb->_1C = 1;
                }
                xfb->_14 = _3C;
                break;
            case 2:
                exchangeXfb_double();
                break;
            case 3:
                exchangeXfb_triple();
                break;
        }
    }

    if ((_34 = ++_30 >= _2C)) {
        _30 = 0;
    }
    if (_34) {
        clearEfb(_0);
        preGX();
    }
}

void MainLoopFramework::endRender() {
    endGX();
    if (_34) {
        JUTXfb* xfb = JUTXfb::sManager;
        switch (xfb->_10) {
            case 1:
                drawendXfb_single();
                break;
            case 2:
                break;
            case 3:
                copyXfb_triple();
                break;
        }
    }
    calcCombinationRatio();
}

void MainLoopFramework::endFrame() {
    if (_34) {
        JUTXfb* xfb = JUTXfb::sManager;
        switch (xfb->_10) {
            case 1:
                break;
            case 2:
                waitDrawDoneAndSetAlarm();
                GXFlush();
                break;
            case 3:
                waitDrawDoneAndSetAlarm();
                GXFlush();
                break;
        }
    }
    
    if (_34) {
        if (!lbl_806B6ED4) {
            VIGetRetraceCount();
            lbl_806B6ED4 = 1;
        }
        VIGetRetraceCount();
    }
}

void MainLoopFramework::waitBlanking(int tickCount) {
    while (tickCount-- > 0) {
        waitForTick(mTickDuration, mRetraceCount);
    }
}

void MainLoopFramework::setTickRateFromFrame(u16 frame) {
    mTickDuration = frameToTick(frame);
    mRetraceCount = 0;
}

void MainLoopFramework::clearEfb(GXColor color) {
    u16 fbWidth = JUTVideo::sManager->mRenderModeObj->fbWidth;
    u16 efbHeight = JUTVideo::sManager->mRenderModeObj->efbHeight;
    clearEfb(0, 0, fbWidth, efbHeight, color);
}

// void MainLoopFramework::clearEfb(int, int, int, int, GXColor color) {
// }

void MainLoopFramework::calcCombinationRatio() {
    s32 videoInterval = JUTVideo::sVideoInterval;
    s32 var1 = videoInterval;
    while (var1 < 2 * _24) {
        var1 += videoInterval;
    }
    s32 var2 = var1 - 2 * _24 - _28;
    if (var2 < 0) {
        var2 += videoInterval;
    }

    mCombinationRatio = (f32)var2 / (u32)_24;
    if (mCombinationRatio > 1f) {
        mCombinationRatio = 1f;
    }
}

u32 MainLoopFramework::frameToTick(f32 frame) {
    s32 tick = (s32)((OS_BUS_CLOCK / 4) * frame / 59.94) - 100;
    return (tick < 0) ? 0 : tick;
}
