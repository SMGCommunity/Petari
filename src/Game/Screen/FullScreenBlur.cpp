#include "Game/Screen/FullScreenBlur.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/Color.hpp"
#include "Game/Util/DirectDraw.hpp"
#include "Game/Util/DrawUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"
#include <JSystem/JUtility/JUTTexture.hpp>

namespace MR {
    void drawFullScreenBlur(f32 param1) {
        drawFullScreenBlur(param1, param1, param1 / 30.0f, (param1 / 30.0f) / 2.0f);
    }

    void drawFullScreenBlur(f32 param1, f32 param2, u8 param3, u8 param4) {
        f32 screenWidth = MR::getScreenWidth();

        TDDraw::setup(1, 1, 2);

        JUTTexture texture = JUTTexture(MR::getScreenResTIMG(), 0);

        GXSetScissorBoxOffset(0, 1584);
        TDDraw::drawTexture(TVec2f(0.0f, 0.0f), &texture, TVec2f((screenWidth / 640.0f) * 128.0f, 64.0f));
        GXSetScissorBoxOffset(0, 0);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_A0);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);

        GXColorS10 color1 = {0, 0, 0, param3};
        GXSetTevColorS10(GX_TEVREG0, color1);

        TDDraw::drawTexture(TVec2f(-param1, -param1), &texture, TVec2f(screenWidth + param1 * 2.0f, 456.0f + param1 * 2.0f));

        GXColorS10 color2 = {0, 0, 0, param4};
        GXSetTevColorS10(GX_TEVREG0, color2);
        TDDraw::drawTexture(TVec2f(-param2, -param2), MR::getFullScreenBlurTexture(), TVec2f(screenWidth + param2 * 2.0f, 456.0f + param2 * 2.0f));
        MR::nonFilteredCapture(MR::getFullScreenBlurTexture(), 0, 464);
        MR::loadProjectionMtx();
        MR::loadViewMtx();
    }
};  // namespace MR
