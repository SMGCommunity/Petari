#include <revolution/os.h>
#include <revolution/exi.h>
#include <revolution/os/OSBootInfo.h>
#include <revolution/vi.h>
#include <private/flipper.h>
#include <cstring>

typedef struct OSFatalParam {
    GXColor fg;
    GXColor bg;
    const char* msg;
} OSFatalParam;

static OSFatalParam FatalParam;
static OSContext FatalContext;

extern void OSDefaultExceptionHandler(__OSException, OSContext *);
static void Halt(void);

static void ScreenClear(void* xfb, u16 xfbW, u16 xfbH, GXColor yuv) {
    int i, j;
    u8* ptr;

    ptr = xfb;
    for (i = 0; i < xfbH; i++) {
        for (j = 0; j < xfbW; j += 2) {
            *ptr++ = yuv.r;
            *ptr++ = yuv.g;
            *ptr++ = yuv.r;
            *ptr++ = yuv.b;
        }
    }
}

static void ScreenReport(void* xfb, u16 xfbW, u16 xfbH, GXColor yuv,
                         s32 x, s32 y, s32 leading, const char* string)
{

    u8* ptr;
    s32 width;
    u32 i, j;
    u32 image[24 / 8 * 24];   // 24 x 24
    u32 k, l;
    u8  Y;
    u32 pixel;
    s32 col;

NextLine:
    if (xfbH - 24 < y) {
        return;
    }

    ptr = (u8*) xfb + (y * xfbW + x) * 2;
    col = x;

    while (*string) {
        if (*string == '\n') {
            ++string;
            y += leading;
            goto NextLine;
        }

        if (xfbW - 48 < col) {
            y += leading;
            goto NextLine;
        }

        for (i = 0; i < 24; i++) {
            j = 24 * (i / 8) + (i % 8);
            image[j] = 0;
            image[j + 32 / sizeof(u32)] = 0;
            image[j + 64 / sizeof(u32)] = 0;
        }

        string = OSGetFontTexel(string, image, 0, 24 / 4, &width);
        for (i = 0; i < 24; i++)
        {
            j = 24 * (i / 8) + (i % 8);
            for (k = 0; k < 24; k++)
            {
                l = j + 8 * (k / 8);
                Y = (u8) ((image[l] >> ((7 - (k % 8)) * 4)) & 0xf);
                if (0 < Y)
                {
                    Y = (u8) (16 + ((255 - 16) * Y * yuv.r) / 255 / 15);

                    pixel = i * xfbW + k;
                    ptr[pixel * 2] = Y;

                    if ((col + k) & 1) {
                        ptr[pixel * 2 - 1] = yuv.g;
                        ptr[pixel * 2 + 1] = yuv.b;
                    }
                    else {
                        ptr[pixel * 2 - 1] = yuv.b;
                        ptr[pixel * 2 + 1] = yuv.g;
                    }
                }
            }
        }

        ptr += width * 2;
        col += width;
    }
}

static void ConfigureVideo(u16 xfbW, u16 xfbH) {
    GXRenderModeObj mode;

    mode.fbWidth = xfbW;
    mode.efbHeight = 480;
    mode.xfbHeight = xfbH;
    mode.viXOrigin = (720 - 640) / 2;
    mode.viWidth = 640;
    mode.viHeight = xfbH;

    switch (VIGetTvFormat())
    {
      case VI_NTSC:
      case VI_MPAL:
        if (((((u32)(__VIRegs[0x36])) & 1))) {
            mode.viTVmode = VI_TVMODE_NTSC_PROG;
            mode.viYOrigin = (480 - 480) / 2;
            mode.xFBmode = VI_XFBMODE_SF;
        }
        else {
            mode.viTVmode = VI_TVMODE_NTSC_INT;
            mode.viYOrigin = (480 - 480) / 2;
            mode.xFBmode = VI_XFBMODE_DF;
        }
        break;

      case VI_EURGB60:
        mode.viTVmode = VI_TVMODE_EURGB60_INT;
        mode.viYOrigin = (480 - 480) / 2;
        mode.xFBmode = VI_XFBMODE_DF;
        break;

      case VI_PAL:
        mode.viTVmode = VI_TVMODE_PAL_INT;
        mode.viYOrigin = (574 - 480) / 2;
        mode.xFBmode = VI_XFBMODE_DF;
        break;

      default:
        break;
    }

    VIConfigure(&mode);
    VIConfigurePan(0, 0, 640, 480);
}

#define CLAMP(x,l,h) ((x > h) ? h : ((x < l) ? l : x))

static GXColor RGB2YUV(GXColor rgb) {
    f32 Y, Cb, Cr;
    GXColor yuv;

    Y =  0.257f * rgb.r + 0.504f * rgb.g + 0.098f * rgb.b +  16.0f + 0.5f;
    Cb = -0.148f * rgb.r - 0.291f * rgb.g + 0.439f * rgb.b + 128.0f + 0.5f;
    Cr =  0.439f * rgb.r - 0.368f * rgb.g - 0.071f * rgb.b + 128.0f + 0.5f;

    yuv.r = (u8)CLAMP(Y , 16, 235);
    yuv.g = (u8)CLAMP(Cb, 16, 240);
    yuv.b = (u8)CLAMP(Cr, 16, 240);
    yuv.a = 0;
    return yuv;
}

void OSFatal(GXColor fg, GXColor bg, const char* msg) {
    OSBootInfo* bootInfo = (OSBootInfo*)OSPhysicalToCached(0);
    u32 count;
    OSTime t;

    OSDisableInterrupts();
    OSDisableScheduler();
    OSClearContext(&FatalContext);
    OSSetCurrentContext(&FatalContext);
    __OSStopAudioSystem();
    VIInit();
    __OSUnmaskInterrupts(128);
    VISetBlack(TRUE);
    VIFlush();
    VISetPreRetraceCallback(NULL);
    VISetPostRetraceCallback(NULL);
    OSEnableInterrupts();

    count = VIGetRetraceCount();
    while ((s32)VIGetRetraceCount() - (s32)count < 1) {

    }

    t = OSGetTime();
    while (!__OSCallShutdownFunctions(FALSE, 0) && (OSGetTime() - t) < OSMillisecondsToTicks(1000));

    OSDisableInterrupts();
    __OSCallShutdownFunctions(TRUE, 0);

    EXISetExiCallback(0, NULL);
    EXISetExiCallback(2, NULL);
    
    while (!EXILock(0, 1, NULL)) {
        EXISync(0);
        EXIDeselect(0);
        EXIUnlock(0);
    }

    EXIUnlock(0);

    while (((((u32)(__EXIRegs[3])) & 1)) == 1);

    __OSSetExceptionHandler(8, OSDefaultExceptionHandler);
    GXAbortFrame();
    OSSetArenaLo((void*)0x81400000);

    if (!bootInfo->FSTLocation) {
        OSSetArenaHi(*(void**)OSPhysicalToCached(0x3110));
    }
    else {
        OSSetArenaHi(bootInfo->FSTLocation);
    }

    FatalParam.fg = fg;
    FatalParam.bg = bg;
    FatalParam.msg = msg;
    OSSwitchFiber((u32)Halt, (u32)OSGetArenaHi());
}

static void Halt(void) {
    u32 count;
    OSFontHeader* fontData;
    void* xfb;
    u32 len;
    OSFatalParam* fp;

    OSEnableInterrupts();
    fp = &FatalParam;
    len = strlen(fp->msg) + 1;
    fp->msg = memmove(OSAllocFromMEM1ArenaLo(len, 32), fp->msg, len);
    fontData = OSAllocFromMEM1ArenaLo(0xA1004, 32);
    OSLoadFont(fontData, OSGetArenaLo());
    xfb = OSAllocFromMEM1ArenaLo((u32)(640 * 480 * 2), 32);
    ScreenClear(xfb, 640, 480, RGB2YUV(fp->bg));
    VISetNextFrameBuffer(xfb);

    ConfigureVideo(640, 480);
    VIFlush();
    count = VIGetRetraceCount();

    while ((s32)VIGetRetraceCount() - (s32)count < 2);

    ScreenReport(xfb, 640, 480, RGB2YUV(fp->fg), 48, 100, fontData->leading, fp->msg);
    DCFlushRange(xfb, 640 * 480 * 2);
    VISetBlack(FALSE);
    VIFlush();
    count = VIGetRetraceCount();

    while ((s32)VIGetRetraceCount() - (s32)count < 1);
    OSDisableInterrupts();
    OSReport("%s\n", fp->msg);
    PPCHalt();
}