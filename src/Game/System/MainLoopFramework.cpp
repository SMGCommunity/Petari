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
#include <revolution/gx/GXRegs.h>
#include <runtime.h>

MainLoopFramework* MainLoopFramework::sManager;

MR::BothDirList<MainLoopFrameworkAlarm> MainLoopFrameworkAlarm::sList(false);

GXTexObj clear_z_tobj;

Mtx e_mtx = { { 1f, 0f, 0f, 0f },
              { 0f, 1f, 0f, 0f },
              { 0f, 0f, 1f, 0f } };

u32 lbl_805DBAC0[] = { 0x00FF00FF, 0x00FF00FF, 0x00FF00FF, 0x00FF00FF,
                       0x00FF00FF, 0x00FF00FF, 0x00FF00FF, 0x00FF00FF,
                       0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
                       0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF };

namespace {
    s32 getDisplayingXfbIndex() NO_INLINE;

    void waitForTick(u32, u16);

    void waitDrawDoneAndSetAlarm();

    void handleGXAbortAlarm(OSAlarm*, OSContext*);
}

void MainLoopFramework::ctor_subroutine(bool param1) {
    _0 = (GXColor){0, 0, 0, 0};
    _4 = 0xffffff;
    _8 = 0;
    _A = 3;
    _C = 1;
    mRetraceCount = 1;
    mTickDuration = 0;
    _18 = param1;
    _19 = true;
    mCombinationRatio = 0f;
    mTick = OSGetTick();
    _24 = 0;
    _28 = 0;
    _3C = 0;
    _3E = 0;
    GXInitTexObj(
        &clear_z_tobj,
        lbl_805DBAC0,
        4, 4,
        GX_TF_Z24X8,
        GX_REPEAT,
        GX_REPEAT,
        GX_FALSE
    );
    GXInitTexObjLOD(
        &clear_z_tobj,
        GX_NEAR,
        GX_NEAR,
        0f, 0f, 0f,
        GX_FALSE,
        GX_FALSE,
        GX_ANISO_1
    );
    _2C = 1;
    _30 = 0;
    _34 = false;
    _38 = nullptr;
}

MainLoopFramework::~MainLoopFramework() {
    if (JUTVideo::sManager) {
        waitBlanking(2);
    }
    JUTXfb::destroyManager();
}

MainLoopFramework* MainLoopFramework::createManager(
    const GXRenderModeObj* pRenderModeObj, void* param2, void* param3, void* param4, bool param5
) {
    if (pRenderModeObj) {
        JUTVideo::sManager->setRenderMode(pRenderModeObj);
    }
    if (!sManager) {
        sManager = new MainLoopFramework(param2, param3, param4, param5);
    }
    return sManager;
}

void callDirectDraw() {
    u16 efbHeight, fbWidth;
    fbWidth = JUTVideo::sManager->mRenderModeObj->fbWidth;
    efbHeight = JUTVideo::sManager->mRenderModeObj->efbHeight;
    void* pXfb = JUTXfb::sManager->getDrawingXfb();
    JUTDirectPrint::sDirectPrint->changeFrameBuffer(pXfb, fbWidth, efbHeight);
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
    JUTXfb* pXfbMgr = JUTXfb::sManager;
    if (pXfbMgr->_14 >= 0) {
        prepareCopyDisp();
        waitDrawDoneAndSetAlarm();
        GXFlush();
        pXfbMgr->_16 = pXfbMgr->_14;
    }
}

void MainLoopFramework::exchangeXfb_double() {
    JUTXfb* pXfbMgr = JUTXfb::sManager;
    if (pXfbMgr->_16 == pXfbMgr->_18) {
        if (pXfbMgr->_14 >= 0) {
            if (_38) {
                _38();
            }
            prepareCopyDisp();
            GXCopyDisp(pXfbMgr->getDrawingXfb(), GX_TRUE);
            if (!_C) {
                pXfbMgr->_16 = pXfbMgr->_14;
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
        s16 temp = pXfbMgr->_14;
        pXfbMgr->_16 = temp;
        pXfbMgr->_14 = temp >= 0 ? temp ^ 1 : 0;
    }
    else {
        clearEfb(_0);
        if (pXfbMgr->_14 < 0) {
            pXfbMgr->_14 = 0;
        }
    }
}

namespace {
    s32 getDisplayingXfbIndex() {
        return JUTXfb::getManager()->getDisplayingXfbIndex();
    }
}

void MainLoopFramework::exchangeXfb_triple() {
    JUTXfb* xfbMgr = JUTXfb::sManager;
    if (xfbMgr->_14 >= 0) {
        callDirectDraw();
    }
    xfbMgr->_16 = xfbMgr->_14;
    s16 var1 = xfbMgr->_14 + 1;
    do {
        if (var1 >= 3 || var1 < 0) {
            var1 = 0;
        }
    } while (var1 == getDisplayingXfbIndex());
    xfbMgr->_14 = var1;
}

void MainLoopFramework::copyXfb_triple() {
    JUTXfb* xfbMgr = JUTXfb::sManager;
    if (xfbMgr->_14 >= 0) {
        if (_38) {
            _38();
        }
        prepareCopyDisp();
        GXCopyDisp(xfbMgr->getDrawingXfb(), GX_FALSE);
        GXPixModeSync();
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
        static s8 lbl_806B6ED4;
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

void MainLoopFramework::clearEfb(int param1, int param2, int param3, int param4, GXColor color) {
    u16 fbWidth = JUTVideo::sManager->mRenderModeObj->fbWidth;
    u16 efbHeight = JUTVideo::sManager->mRenderModeObj->efbHeight;
    Mtx44 proj;
    C_MTXOrtho(proj, 0f, efbHeight, 0f, fbWidth, 0f, 1f);
    GXSetProjection(proj, GX_ORTHOGRAPHIC);
    GXSetViewport(0f, 0f, fbWidth, efbHeight, 0f, 1f);
    GXSetScissor(0, 0, fbWidth, efbHeight);
    GXLoadPosMtxImm(e_mtx, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XY, GX_U16, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_POS_XYZ, GX_U8, 0);
    GXSetNumChans(0);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetChanCtrl(GX_COLOR1A1, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXLoadTexObj(&clear_z_tobj, GX_TEXMAP0);
    GXSetNumTevStages(1);
    GXSetTevColor(GX_TEVREG0, color);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C0);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_A0);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);
    GXSetZTexture(GX_ZT_REPLACE, GX_TF_Z24X8, 0);
    GXSetZCompLoc(GX_FALSE);
    GXSetBlendMode(GX_BM_NONE, GX_BL_ZERO, GX_BL_ZERO, GX_LO_NOOP);
    if (_18) {
        GXSetAlphaUpdate(GX_TRUE);
        GXSetDstAlpha(GX_TRUE, color.a);
    }
    GXSetZMode(GX_TRUE, GX_ALWAYS, GX_TRUE);
    GXSetCullMode(GX_CULL_BACK);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GX_WRITE_U16(param1);
    GX_WRITE_U16(param2);
    GX_WRITE_U8(0);
    GX_WRITE_U8(0);
    GX_WRITE_U16(param1 + param3);
    GX_WRITE_U16(param2);
    GX_WRITE_U8(1);
    GX_WRITE_U8(0);
    GX_WRITE_U16(param1 + param3);
    GX_WRITE_U16(param2 + param4);
    GX_WRITE_U8(1);
    GX_WRITE_U8(1);
    GX_WRITE_U16(param1);
    GX_WRITE_U16(param2 + param4);
    GX_WRITE_U8(0);
    GX_WRITE_U8(1);
    GXSetZTexture(GX_ZT_DISABLE, GX_TF_Z24X8, 0);
    GXSetZCompLoc(GX_TRUE);
    if (_18) {
        GXSetDstAlpha(GX_FALSE, color.a);
    }
}

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

void MainLoopFramework::setForOSResetSystem() {
    MR::BothDirPtrLink* pLink = MainLoopFrameworkAlarm::sList.mHead;
    while (pLink) {
        OSCancelAlarm((MainLoopFrameworkAlarm*)pLink->mValue);
        pLink = pLink->mNextLink;
    }
    JUTVideo::destroyManager();
    VISetBlack(TRUE);
    VIFlush();
    VIWaitForRetrace();
    if (sManager) {
        delete sManager;
        sManager = nullptr;
    }
}

namespace {
    void waitForTick(u32 tickDuration, u16 retraceCount) {
        if (tickDuration) {
            static s64 lbl_8060B380;
            static s8 timeInitialized;
            if (!timeInitialized) {
                lbl_8060B380 = OSGetTime();
                timeInitialized = 1;
            }
            s64 currentTime = OSGetTime();
            while (currentTime < lbl_8060B380) {
                OSSleepTicks(lbl_8060B380 - currentTime);
                currentTime = OSGetTime();
            }
            lbl_8060B380 = currentTime + tickDuration;
        }
        else {
            static u32 nextCount;
            static s8 lbl_806B6EDC;
            if (!lbl_806B6EDC) {
                nextCount = VIGetRetraceCount();
                lbl_806B6EDC = 1;
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

    void waitDrawDoneAndSetAlarm() {
        MainLoopFrameworkAlarm alarm;
        MR::BothDirLink<MainLoopFrameworkAlarm> link(&alarm);
        {
            MR::ProhibitSchedulerAndInterrupts prohibit(false);
            OSCreateAlarm(&alarm);
            MainLoopFrameworkAlarm::sList.append(&link);
        }
        OSTime tick = __cvt_dbl_usll(OS_BUS_CLOCK / 4 * 0.5);
        OSSetAlarm(&alarm, tick, &handleGXAbortAlarm);
        GXDrawDone();
        DrawSyncManager::resetIfAborted();
        {
            MR::ProhibitSchedulerAndInterrupts prohibit(false);
            OSCancelAlarm(&alarm);
            MainLoopFrameworkAlarm::sList.remove(&link);
        }
    }

    void handleGXAbortAlarm(OSAlarm* alarm, OSContext* context) {
        u32 ras_busy2, clocks2, xf_wait_in2, xf_wait_out2;
        u32 ras_busy, clocks, xf_wait_in, xf_wait_out;
        GXReadXfRasMetric(&xf_wait_in, &xf_wait_out, &ras_busy, &clocks);
        GXReadXfRasMetric(&xf_wait_in2, &xf_wait_out2, &ras_busy2, &clocks2);
        GXBool brkpt, cmdIdle, readIdle, underlow, overhi;
        GXGetGPStatus(&overhi, &underlow, &readIdle, &cmdIdle, &brkpt);
        GXDisableBreakPt();
        GXAbortFrame();
        DrawSyncManager::sInstance->clearFifo();
        GX_WRITE_U8(0x61);
        GX_WRITE_U32(0x5800000f);
        GXSetDrawDone();
        DrawSyncManager::prepareReset();
    }
}
