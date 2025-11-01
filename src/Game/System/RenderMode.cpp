#include "Game/System/RenderMode.hpp"
#include <revolution/gx.h>
#include <revolution/vi.h>
#include <revolution/sc.h>

const GXRenderModeObj GXNtscIntDf[] = {
    {
        VI_TVMODE_NTSC_INT,
        640,
        456,
        456,
        (VI_MAX_WIDTH_NTSC - 670) / 2,
        (VI_MAX_HEIGHT_NTSC - 456) / 2,
        670,
        456,
        VI_XFBMODE_DF,
        GX_FALSE,
        GX_FALSE,
        {
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
        },
        {
            32,
            0,
            32,
            0,
            0,
            0,
            0
        },
    },
    {
        VI_TVMODE_NTSC_INT,
        640,
        456,
        456,
        (VI_MAX_WIDTH_NTSC - 686) / 2,
        (VI_MAX_HEIGHT_NTSC - 456) / 2,
        686,
        456,
        VI_XFBMODE_DF,
        GX_FALSE,
        GX_FALSE,
        {
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
        },
        {
            32,
            0,
            32,
            0,
            0,
            0,
            0
        },
    },

};

const GXRenderModeObj GXNtscProg[] = {
    {
        VI_TVMODE_NTSC_PROG,
        640,
        456,
        456,
        (VI_MAX_WIDTH_NTSC - 670) / 2,
        (VI_MAX_HEIGHT_NTSC - 456) / 2,
        670,
        456,
        VI_XFBMODE_SF,
        GX_FALSE,
        GX_FALSE,
        {
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
        },
        {
            32,
            0,
            32,
            0,
            0,
            0,
            0
        },
    },
    {
        VI_TVMODE_NTSC_PROG,
        640,
        456,
        456,
        (VI_MAX_WIDTH_NTSC - 686) / 2,
        (VI_MAX_HEIGHT_NTSC - 456) / 2,
        686,
        456,
        VI_XFBMODE_SF,
        GX_FALSE,
        GX_FALSE,
        {
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
        },
        {
            32,
            0,
            32,
            0,
            0,
            0,
            0
        },
    },

};

const GXRenderModeObj GXPalIntDf[] = {
    {
        VI_TVMODE_PAL_INT,
        640,
        456,
        542,
        (VI_MAX_WIDTH_PAL - 666) / 2,
        (VI_MAX_HEIGHT_PAL - 542) / 2,
        666,
        542,
        VI_XFBMODE_DF,
        GX_FALSE,
        GX_FALSE,
        {
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
        },
        {
            32,
            0,
            32,
            0,
            0,
            0,
            0
        },
    },
    {
        VI_TVMODE_PAL_INT,
        640,
        456,
        542,
        (VI_MAX_WIDTH_PAL - 682) / 2,
        (VI_MAX_HEIGHT_PAL - 542) / 2,
        682,
        542,
        VI_XFBMODE_DF,
        GX_FALSE,
        GX_FALSE,
        {
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
        },
        {
            32,
            0,
            32,
            0,
            0,
            0,
            0
        },
    },

};

const GXRenderModeObj GXEurgb60HzIntDf[] = {
    {
        VI_TVMODE_EURGB60_INT,
        640,
        456,
        456,
        (VI_MAX_WIDTH_EURGB60 - 670) / 2,
        (VI_MAX_HEIGHT_EURGB60 - 456) / 2,
        670,
        456,
        VI_XFBMODE_DF,
        GX_FALSE,
        GX_FALSE,
        {
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
        },
        {
            32,
            0,
            32,
            0,
            0,
            0,
            0
        },
    },
    {
        VI_TVMODE_EURGB60_INT,
        640,
        456,
        456,
        (VI_MAX_WIDTH_EURGB60 - 686) / 2,
        (VI_MAX_HEIGHT_EURGB60 - 456) / 2,
        686,
        456,
        VI_XFBMODE_DF,
        GX_FALSE,
        GX_FALSE,
        {
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
        },
        {
            32,
            0,
            32,
            0,
            0,
            0,
            0
        },
    },

};

const GXRenderModeObj GXEurgb60HzProg[] = {
    {
        VI_TVMODE_EURGB60_PROG,
        640,
        456,
        456,
        (VI_MAX_WIDTH_EURGB60 - 670) / 2,
        (VI_MAX_HEIGHT_EURGB60 - 456) / 2,
        670,
        456,
        VI_XFBMODE_SF,
        GX_FALSE,
        GX_FALSE,
        {
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
        },
        {
            32,
            0,
            32,
            0,
            0,
            0,
            0
        },
    },
    {
        VI_TVMODE_EURGB60_PROG,
        640,
        456,
        456,
        (VI_MAX_WIDTH_EURGB60 - 686) / 2,
        (VI_MAX_HEIGHT_EURGB60 - 456) / 2,
        686,
        456,
        VI_XFBMODE_SF,
        GX_FALSE,
        GX_FALSE,
        {
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
            { 6, 6 },
        },
        {
            32,
            0,
            32,
            0,
            0,
            0,
            0
        },
    },

};

namespace MR {
    const GXRenderModeObj *getSuitableRenderMode() {
        const GXRenderModeObj *result = nullptr;
        bool is16Per9 = isAspectRatioFlag16Per9();

        if (VIGetDTVStatus() == 1 && isProgressiveModeOn()) {
            switch (VIGetTvFormat()) {
                case VI_NTSC:
                case VI_MPAL:
                    result = &GXNtscProg[is16Per9];
                    break;
                case VI_PAL:
                case VI_EURGB60:
                    result = &GXEurgb60HzProg[is16Per9];
                    break;
            }
        } else {
            switch (VIGetTvFormat()) {
                case VI_NTSC:
                case VI_MPAL:
                    return &GXNtscIntDf[is16Per9];
                case VI_PAL:
                    if (!isEuRgb60ModeOn()) { 
                        return &GXPalIntDf[is16Per9];
                    }
                case VI_EURGB60:
                    return &GXEurgb60HzIntDf[is16Per9];
            }
        }

        return result;
    }

    bool isProgressiveModeOn() {
        return SCGetProgressiveMode() == SC_PROGRESSIVE_MODE_ON;
    }

    bool isEuRgb60ModeOn() {
        return SCGetEuRgb60Mode() == SC_EURGB60_MODE_ON;
    }

    bool isAspectRatioFlag16Per9() {
        return SCGetAspectRatio() == SC_ASPECT_RATIO_16x9;
    }

    u32 getRequiredExternalFrameBufferSize() {
        return 0xA9600;
    }
}

u32 RenderModeObj::getViWidthMax() {
    switch (VIGetTvFormat()) {
        case VI_PAL:
            return VI_MAX_WIDTH_PAL;
        case VI_MPAL:
            return VI_MAX_WIDTH_MPAL;
        case VI_EURGB60:
            return VI_MAX_WIDTH_EURGB60;
        case VI_NTSC:
        default:
            return VI_MAX_WIDTH_NTSC;
    }
}
