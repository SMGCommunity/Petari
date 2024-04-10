#include <RVLFaceLibInternal.h>

double atan2(double y, double x);

inline float atan2f(float x, float y) {
    return atan2(x, y);
}

RFLErrcode RFLMakeIcon(void *buf, RFLDataSource source, RFLMiddleDB *middleDB, u16 index, RFLExpression expression, const RFLIconSetting *setting) {
    RFLiCharInfo info;
    RFLErrcode err = RFLiPickupCharInfo(&info, source, middleDB, index);

    if (err == RFLErrcode_Success) {
        RFLiMakeIcon(buf, &info, expression, setting);
    }

    return err;
}

void RFLiMakeIcon(void* buf, RFLiCharInfo* info, RFLExpression expression,
                        const RFLIconSetting* setting) {
    RFLCharModel model;               // sp+218
    void* modelBuf;                   // sp+84
    f32 vp[6];                        // sp+150
    u32 byteSize;                     // sp+80
    Mtx viewMtx;                      // sp+1e8
    u32 scissorOffsetX;               // sp+7C
    u32 scissorOffsetY;               // sp+78
    u32 scissorWidth;                 // sp+74
    u32 scissorHeight;                // sp+70
    RFLiCoordinateData iconCoordData; // sp+118
    RFLiCoordinateData coordData;     // sp+128
    u32 bufSize;                      // sp+6C
    RFLResolution resolution;         // sp+68
    RFLExpFlag expFlag;               // sp+64
    GXColor backColor;                // sp+60
    Mtx44 projMtx;                    // sp+1A8
    f32 fovy;                         // f31
    f32 aspect;                       // f30
    Vec cameraPos;                    // sp+10C
    Vec target;                       // sp+100
    Vec cameraUp;                     // sp+F4
    GXLightObj light;                 // sp+168
    Vec pos;                          // sp+E8
    RFLDrawSetting drawSetting;       // sp+138

    // buf -> r28
    // info -> r29
    // expression -> r27
    // setting -> r30

    iconCoordData = (RFLiCoordinateData){1, 2, 0, FALSE, FALSE, FALSE};
    byteSize = setting->width * setting->height * sizeof(u16);
    coordData = *RFLiGetCoordinateData();
    RFLiSetCoordinateData(&iconCoordData);

    if ((setting->width > 128 || setting->height > 128) && RFLiGetUseDeluxTex()) {
        resolution = RFLResolution_256;
    } else if (setting->width > 64 || setting->height > 64) {
        resolution = RFLResolution_128;
    } else {
        resolution = RFLResolution_64;
    }

    expFlag = 1 << expression;
    bufSize = RFLGetModelBufferSize(resolution, expFlag);
    modelBuf = RFLiAlloc32(bufSize);
    RFLiInitCharModel(&model, info, modelBuf, resolution, expFlag);
    RFLSetExpression(&model, expression);

    if (setting->bgType == RFLIconBG_Direct) {
        backColor = setting->bgColor;
    } else {
        backColor = RFLGetFavoriteColor(info->personal.color);
    }
    backColor.a = 0;

    GXGetScissor(&scissorOffsetX, &scissorOffsetY, &scissorWidth,
                 &scissorHeight);
    GXSetScissor(0, 0, setting->width, setting->height);

    RFLiSetupCopyTex(GX_RGBA8, setting->width, setting->height, buf, backColor);

    GXGetViewportv(vp);
    GXSetViewport(0.0f, 0.0f, setting->width, setting->height, 0.0f, 1.0f);

    aspect = (f32)setting->width / (f32)setting->height;
    if (setting->width < setting->height) {
        fovy = 2 * ((180 / 3.141592653589793f) * atan2f(43.2f / aspect, 500.0f));
    } else {
        fovy = 2 * ((180 / 3.141592653589793f) * atan2f(43.2f, 500.0f));
    }

    C_MTXPerspective(projMtx, fovy, aspect, 500.0f, 700.0f);
    GXSetProjection(projMtx, GX_PERSPECTIVE);

    cameraPos = (Vec){0.0f, 34.5f, 600.0f};
    target = (Vec){0.0f, 34.5f, 0.0f};
    cameraUp = (Vec){0.0f, 1.0f, 0.0f};

    C_MTXLookAt(viewMtx, &cameraPos, &cameraUp, &target);
    GXInitLightColor(&light, (GXColor){255, 255, 255, 255});

    pos = (Vec){1600.0f, 1500.0f, 6000.0f};

    PSMTXMultVec(viewMtx, &pos, &pos);
    GXInitLightPos(&light, pos.x, pos.y, pos.z);
    GXLoadLightObjImm(&light, GX_LIGHT0);
    RFLSetMtx(&model, viewMtx);

    drawSetting.lightEnable = TRUE;
    drawSetting.lightMask = GX_LIGHT0;
    drawSetting.diffuse = GX_DF_CLAMP;
    drawSetting.attn = GX_AF_NONE;
    drawSetting.ambColor = (GXColor){160, 160, 160, 255};
    drawSetting.compLoc = 0;
    RFLLoadDrawSetting(&drawSetting);

    if (!setting->drawXluOnly) {
        GXSetColorUpdate(TRUE);
        GXSetAlphaUpdate(TRUE);
        GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA,
                       GX_LO_COPY);
        RFLDrawOpa(&model);
    }

    GXSetZMode(TRUE, GX_LEQUAL, FALSE);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_COPY);
    GXSetColorUpdate(TRUE);
    GXSetAlphaUpdate(FALSE);
    RFLDrawXlu(&model);

    GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_INVSRCALPHA, GX_LO_COPY);
    GXSetAlphaUpdate(TRUE);
    GXSetColorUpdate(FALSE);
    RFLDrawXlu(&model);

    GXSetZMode(TRUE, GX_LEQUAL, TRUE);
    GXSetColorUpdate(TRUE);
    GXCopyTex(buf, TRUE);
    GXPixModeSync();

    if (RFLiGetManager()->iconDrawCb == NULL) {
        GXDrawDone();
    } else {
        RFLiGetManager()->iconDrawCb();
    }

    RFLiFree(modelBuf);
    GXSetViewportv(vp);
    GXSetScissor(scissorOffsetX, scissorOffsetY, scissorWidth, scissorHeight);
    RFLiSetCoordinateData(&coordData);
}

void RFLSetIconDrawDoneCallback(RFLCallback callback) {
    RFLiGetManager()->iconDrawCb = callback;
}