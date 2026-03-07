#include "private/ra_gen.h"
#include "revolution/gx/GXCommandList.h"
#include "revolution/gx/GXTypes.h"
#include "revolution/gx/shortcut_tx_reg.h"
#include <mem.h>
#include <revolution/gx.h>
#include <revolution/gx/GXRegs.h>

#define __GX_FLUSH_TEX_STATE() __GXFlushTextureState()

#define IsCITexFmt(fmt) (fmt == GX_TF_C4 || fmt == GX_TF_C8 || fmt == GX_TF_C14X2)

#define NumLeadingZeros(d) __cntlzw((u32)d)

static u8 GXTexMode0Ids[8] = {
    TX_SETMODE0_I0_ID, TX_SETMODE0_I1_ID, TX_SETMODE0_I2_ID, TX_SETMODE0_I3_ID,
    TX_SETMODE0_I4_ID, TX_SETMODE0_I5_ID, TX_SETMODE0_I6_ID, TX_SETMODE0_I7_ID,
};

static u8 GXTexMode1Ids[8] = {
    TX_SETMODE1_I0_ID, TX_SETMODE1_I1_ID, TX_SETMODE1_I2_ID, TX_SETMODE1_I3_ID,
    TX_SETMODE1_I4_ID, TX_SETMODE1_I5_ID, TX_SETMODE1_I6_ID, TX_SETMODE1_I7_ID,
};

static u8 GXTexImage0Ids[8] = {
    TX_SETIMAGE0_I0_ID, TX_SETIMAGE0_I1_ID, TX_SETIMAGE0_I2_ID, TX_SETIMAGE0_I3_ID,
    TX_SETIMAGE0_I4_ID, TX_SETIMAGE0_I5_ID, TX_SETIMAGE0_I6_ID, TX_SETIMAGE0_I7_ID,
};

static u8 GXTexImage1Ids[8] = {
    TX_SETIMAGE1_I0_ID, TX_SETIMAGE1_I1_ID, TX_SETIMAGE1_I2_ID, TX_SETIMAGE1_I3_ID,
    TX_SETIMAGE1_I4_ID, TX_SETIMAGE1_I5_ID, TX_SETIMAGE1_I6_ID, TX_SETIMAGE1_I7_ID,
};

static u8 GXTexImage2Ids[8] = {
    TX_SETIMAGE2_I0_ID, TX_SETIMAGE2_I1_ID, TX_SETIMAGE2_I2_ID, TX_SETIMAGE2_I3_ID,
    TX_SETIMAGE2_I4_ID, TX_SETIMAGE2_I5_ID, TX_SETIMAGE2_I6_ID, TX_SETIMAGE2_I7_ID,
};

static u8 GXTexImage3Ids[8] = {
    TX_SETIMAGE3_I0_ID, TX_SETIMAGE3_I1_ID, TX_SETIMAGE3_I2_ID, TX_SETIMAGE3_I3_ID,
    TX_SETIMAGE3_I4_ID, TX_SETIMAGE3_I5_ID, TX_SETIMAGE3_I6_ID, TX_SETIMAGE3_I7_ID,
};

static u8 GXTexTlutIds[8] = {
    TX_SETTLUT_I0_ID, TX_SETTLUT_I1_ID, TX_SETTLUT_I2_ID, TX_SETTLUT_I3_ID, TX_SETTLUT_I4_ID, TX_SETTLUT_I5_ID, TX_SETTLUT_I6_ID, TX_SETTLUT_I7_ID,
};

static u8 GX2HWFiltConv[] = {
    TX_MIN_NEAREST,                 // GX_NEAR,
    TX_MIN_LINEAR,                  // GX_LINEAR,
    TX_MIN_NEAREST_MIPMAP_NEAREST,  // GX_NEAR_MIP_NEAR,
    TX_MIN_LINEAR_MIPMAP_NEAREST,   // GX_LIN_MIP_NEAR,
    TX_MIN_NEAREST_MIPMAP_LINEAR,   // GX_NEAR_MIP_LIN,
    TX_MIN_LINEAR_MIPMAP_LINEAR,    // GX_LIN_MIP_LIN
};

static u8 HW2GXFiltConv[] = {
    GX_NEAR,           // TX_MIN_NEAREST
    GX_NEAR_MIP_NEAR,  // TX_MIN_NEAREST_MIPMAP_NEAREST
    GX_NEAR_MIP_LIN,   // TX_MIN_NEAREST_MIPMAP_LINEAR
    0,                 // (NA)
    GX_LINEAR,         // TX_MIN_LINEAR
    GX_LIN_MIP_NEAR,   // TX_MIN_LINEAR_MIPMAP_NEAREST
    GX_LIN_MIP_LIN,    // TX_MIN_LINEAR_MIPMAP_LINEAR
    0,                 // (NA)
};

static void __GXGetTexTileShift(GXTexFmt fmt, u32* rowTileS, u32* colTileS) {
    switch (fmt) {
    case GX_TF_C4:
    case GX_TF_I4:
    case GX_TF_CMPR:
    case GX_CTF_R4:
    case GX_CTF_Z4:
        *rowTileS = 3;
        *colTileS = 3;
        break;

    case GX_TF_C8:
    case GX_TF_I8:
    case GX_TF_IA4:
    case GX_TF_A8:
    case GX_TF_Z8:
    case GX_CTF_RA4:
    case GX_CTF_R8:
    case GX_CTF_G8:
    case GX_CTF_B8:
    case GX_CTF_Z8M:
    case GX_CTF_Z8L:
        *rowTileS = 3;
        *colTileS = 2;
        break;

    // 16-bit+ formats:
    case GX_TF_C14X2:
    case GX_TF_IA8:
    case GX_TF_RGB565:
    case GX_TF_RGB5A3:
    case GX_TF_RGBA8:
    case GX_TF_Z16:
    case GX_TF_Z24X8:
    case GX_CTF_RA8:
    case GX_CTF_RG8:
    case GX_CTF_GB8:
    case GX_CTF_Z16L:
        *rowTileS = 2;
        *colTileS = 2;
        break;
    default:
        *rowTileS = *colTileS = 0;
        break;
    }
    return;
}

u32 GXGetTexBufferSize(u16 width, u16 height, u32 format, GXBool mipmap, u8 max_lod) {
    u32 tileShiftX, tileShiftY, tileBytes;
    u32 bufferSize, nx, ny, level;

    __GXGetTexTileShift((GXTexFmt)format, &tileShiftX, &tileShiftY);

    if (format == GX_TF_RGBA8 || format == GX_TF_Z24X8) {
        tileBytes = 64;
    } else {
        tileBytes = 32;
    }

    if (mipmap == GX_TRUE) {
        bufferSize = 0;

        for (level = 0; level < max_lod; level++) {
            nx = (u32)((width + (1 << tileShiftX) - 1) >> tileShiftX);
            ny = (u32)((height + (1 << tileShiftY) - 1) >> tileShiftY);

            bufferSize += nx * ny * tileBytes;

            if (width == 1 && height == 1)
                break;

            width = (u16)((width > 1) ? (width >> 1) : 1);
            height = (u16)((height > 1) ? (height >> 1) : 1);
        }
    } else {
        nx = (u32)((width + (1 << tileShiftX) - 1) >> tileShiftX);
        ny = (u32)((height + (1 << tileShiftY) - 1) >> tileShiftY);

        bufferSize = nx * ny * tileBytes;
    }

    return bufferSize;
}

void __GetImageTileCount(GXTexFmt fmt, u16 wd, u16 ht, u32* rowTiles, u32* colTiles, u32* cmpTiles) {
    u32 texRowShift, texColShift;

    __GXGetTexTileShift(fmt, &texRowShift, &texColShift);

    if (wd == 0)
        wd = 1;
    if (ht == 0)
        ht = 1;
    *rowTiles = (u32)((wd + (1 << texRowShift) - 1) >> texRowShift);
    *colTiles = (u32)((ht + (1 << texColShift) - 1) >> texColShift);
    *cmpTiles = (fmt == GX_TF_RGBA8 || fmt == GX_TF_Z24X8) ? 2U : 1U;

    return;
}

void GXInitTexObj(GXTexObj* obj, void* image_ptr, u16 width, u16 height, GXTexFmt format, GXTexWrapMode wrap_s, GXTexWrapMode wrap_t, GXBool mipmap) {
    u32 imageBase;
    u32 maxLOD;
    u16 rowT, colT;
    u32 rowC, colC;

    GX_SETUP_TEXOBJ(t, obj)

    memset(t, 0, sizeof(GXTexObjInt));

    SC_TX_SETMODE0_SET_WRAP_S(t->mode0, wrap_s);
    SC_TX_SETMODE0_SET_WRAP_T(t->mode0, wrap_t);
    SC_TX_SETMODE0_SET_MAG_FILTER(t->mode0, 1);

    if (mipmap) {
        u8 lmax;

        t->flags |= 1;
        if (IsCITexFmt(format)) {
            SC_TX_SETMODE0_SET_MIN_FILTER(t->mode0, 5);
        } else {
            SC_TX_SETMODE0_SET_MIN_FILTER(t->mode0, 6);
        }

        if (width > height) {
            maxLOD = (u32)(31 - NumLeadingZeros(width));
        } else {
            maxLOD = (u32)(31 - NumLeadingZeros(height));
        }
        lmax = (u8)(maxLOD * 16.0f);
        SC_TX_SETMODE1_SET_MAXLOD(t->mode1, lmax);
    } else {
        SC_TX_SETMODE0_SET_MIN_FILTER(t->mode0, 4);
    }

    t->fmt = format;
    SC_TX_SETIMAGE0_SET_IMAGE_WIDTH(t->image0, (width - 1));
    SC_TX_SETIMAGE0_SET_IMAGE_HEIGHT(t->image0, (height - 1));
    SC_TX_SETIMAGE0_SET_FORMAT(t->image0, (format & 0xf));

    imageBase = (GX_PHY_ADDR(image_ptr)) >> 5;
    SC_TX_SETIMAGE3_SET_IMAGE_BASE(t->image3, imageBase);

    switch (format & 0xf) {
    case GX_TF_I4:
    case GX_TF_C4:
        t->loadFmt = 1;
        rowT = 3;
        colT = 3;
        break;

    case GX_TF_I8:
    case GX_TF_IA4:
    case GX_TF_C8:
        t->loadFmt = 2;
        rowT = 3;
        colT = 2;
        break;

    case GX_TF_IA8:
    case GX_TF_C14X2:
    case GX_TF_RGB565:
    case GX_TF_RGB5A3:
        t->loadFmt = 2;
        rowT = 2;
        colT = 2;
        break;

    case GX_TF_RGBA8:
        t->loadFmt = 3;
        rowT = 2;
        colT = 2;
        break;

    case GX_TF_CMPR:
        t->loadFmt = 0;
        rowT = 3;
        colT = 3;
        break;

    default:
        t->loadFmt = 2;
        rowT = 2;
        colT = 2;
        break;
    }

    rowC = (u32)((width + (1 << rowT) - 1) >> rowT);
    colC = (u32)((height + (1 << colT) - 1) >> colT);
    t->loadCnt = (u16)((rowC * colC) & TX_LOADBLOCK3_COUNT_MASK);
    t->flags |= 2;
}

void GXInitTexObjCI(GXTexObj* obj, void* image_ptr, u16 width, u16 height, GXCITexFmt format, GXTexWrapMode wrap_s, GXTexWrapMode wrap_t,
                    GXBool mipmap, u32 tlut_name) {
    GX_SETUP_TEXOBJ(t, obj)

    GXInitTexObj(obj, image_ptr, width, height, (GXTexFmt)format, wrap_s, wrap_t, mipmap);

    t->flags &= ~2;
    t->tlutName = tlut_name;
}

void GXInitTexObjLOD(GXTexObj* obj, GXTexFilter min_filt, GXTexFilter mag_filt, f32 min_lod, f32 max_lod, f32 lod_bias, GXBool bias_clamp,
                     GXBool do_edge_lod, GXAnisotropy max_aniso) {
    u8 lbias;
    u8 lmin, lmax;
    GX_SETUP_TEXOBJ(t, obj)

    if (lod_bias < -4.0f)
        lod_bias = -4.0f;
    else if (lod_bias >= 4.0f)
        lod_bias = 3.99f;
    lbias = (u8)((s8)(lod_bias * 32.0f));

    SC_TX_SETMODE0_SET_LODBIAS(t->mode0, lbias);

    SC_TX_SETMODE0_SET_MAG_FILTER(t->mode0, ((mag_filt == GX_LINEAR) ? 1 : 0));
    SC_TX_SETMODE0_SET_MIN_FILTER(t->mode0, GX2HWFiltConv[min_filt]);

    SC_TX_SETMODE0_SET_DIAGLOD_ENABLE(t->mode0, ((do_edge_lod) ? 0 : 1));
    SC_TX_SETMODE0_SET_ROUND(t->mode0, 0);
    SC_TX_SETMODE0_SET_FIELD_PREDICT(t->mode0, 0);
    SC_TX_SETMODE0_SET_MAXANISO(t->mode0, max_aniso);
    SC_TX_SETMODE0_SET_LODCLAMP(t->mode0, bias_clamp);

    if (min_lod < 0)
        min_lod = 0;
    else if (min_lod > 10.0f)
        min_lod = 10.0f;
    lmin = (u8)(min_lod * 16.0f);

    if (max_lod < 0)
        max_lod = 0;
    else if (max_lod > 10.0f)
        max_lod = 10.0f;
    lmax = (u8)(max_lod * 16.0f);

    SC_TX_SETMODE1_SET_MINLOD(t->mode1, lmin);
    SC_TX_SETMODE1_SET_MAXLOD(t->mode1, lmax);
}

void GXInitTexObjWrapMode(GXTexObj* obj, GXTexWrapMode sm, GXTexWrapMode tm) {
    GX_SETUP_TEXOBJ(t, obj)
    SC_TX_SETMODE0_SET_WRAP_S(t->mode0, sm);
    SC_TX_SETMODE0_SET_WRAP_T(t->mode0, tm);
}

void GXInitTexObjTlut(GXTexObj* obj, u32 tlut_name) {
    GX_SETUP_TEXOBJ(t, obj)
    t->tlutName = tlut_name;
}

void GXInitTexObjUserData(GXTexObj* obj, void* user_data) {
    GX_SETUP_TEXOBJ(t, obj)
    t->userData = user_data;
}

void* GXGetTexObjUserData(const GXTexObj* obj) {
    GX_SETUP_TEXOBJ(t, obj)
    return t->userData;
}

void GXGetTexObjAll(const GXTexObj* obj, void** image_ptr, u16* width, u16* height, GXTexFmt* format, GXTexWrapMode* wrap_s, GXTexWrapMode* wrap_t,
                    GXBool* mipmap) {
    GX_SETUP_TEXOBJ(t, obj)

    *image_ptr = (void*)((TX_SETIMAGE3_GET_IMAGE_BASE(t->image3)) << 5);
    *width = (u16)(TX_SETIMAGE0_GET_IMAGE_WIDTH(t->image0) + 1);
    *height = (u16)(TX_SETIMAGE0_GET_IMAGE_HEIGHT(t->image0) + 1);
    *format = t->fmt;
    *wrap_s = (GXTexWrapMode)TX_SETMODE0_GET_WRAP_S(t->mode0);
    *wrap_t = (GXTexWrapMode)TX_SETMODE0_GET_WRAP_T(t->mode0);
    *mipmap = (GXBool)((t->flags & 1) == 1);
}

u16 GXGetTexObjWidth(const GXTexObj* to) {
    GX_SETUP_TEXOBJ(t, to)
    return (u16)(TX_SETIMAGE0_GET_IMAGE_WIDTH(t->image0) + 1);
}

u16 GXGetTexObjHeight(const GXTexObj* to) {
    GX_SETUP_TEXOBJ(t, to)
    return (u16)(TX_SETIMAGE0_GET_IMAGE_HEIGHT(t->image0) + 1);
}

GXTexFmt GXGetTexObjFmt(const GXTexObj* to) {
    GX_SETUP_TEXOBJ(t, to)
    return (t->fmt);
}

GXTexWrapMode GXGetTexObjWrapS(const GXTexObj* to) {
    GX_SETUP_TEXOBJ(t, to)
    return (GXTexWrapMode)TX_SETMODE0_GET_WRAP_S(t->mode0);
}

GXTexWrapMode GXGetTexObjWrapT(const GXTexObj* to) {
    GX_SETUP_TEXOBJ(t, to)
    return (GXTexWrapMode)TX_SETMODE0_GET_WRAP_T(t->mode0);
}

GXBool GXGetTexObjMipMap(const GXTexObj* to) {
    GX_SETUP_TEXOBJ(t, to)
    return (GXBool)((t->flags & 1) == 1);
}

void GXGetTexObjLODAll(const GXTexObj* tex_obj, GXTexFilter* min_filt, GXTexFilter* mag_filt, f32* min_lod, f32* max_lod, f32* lod_bias,
                       GXBool* bias_clamp, GXBool* do_edge_lod, GXAnisotropy* max_aniso) {
    s16 tmp;
    GX_SETUP_TEXOBJ(t, tex_obj)

    *min_filt = (GXTexFilter)HW2GXFiltConv[TX_SETMODE0_GET_MIN_FILTER(t->mode0)];
    *mag_filt = (GXTexFilter)TX_SETMODE0_GET_MAG_FILTER(t->mode0);
    *min_lod = TX_SETMODE1_GET_MINLOD(t->mode1) / 16.0f;
    *max_lod = TX_SETMODE1_GET_MAXLOD(t->mode1) / 16.0f;
    tmp = (s16)TX_SETMODE0_GET_LODBIAS(t->mode0);
    *lod_bias = ((f32)((s8)tmp) / 32.0f);
    *bias_clamp = (GXBool)TX_SETMODE0_GET_LODCLAMP(t->mode0);
    *do_edge_lod = (GXBool)!TX_SETMODE0_GET_DIAGLOD_ENABLE(t->mode0);
    *max_aniso = (GXAnisotropy)TX_SETMODE0_GET_MAXANISO(t->mode0);
}

u32 GXGetTexObjTlut(const GXTexObj* tex_obj) {
    GX_SETUP_TEXOBJ(t, tex_obj)
    return t->tlutName;
}

void GXLoadTexObjPreLoaded(const GXTexObj* obj, const GXTexRegion* region, GXTexMapID id) {
    GXTlutRegionInt* tlr;
    u32 m0, m1, img0, img1, img2, img3;
    GX_SETUP_ALL_TEXOBJS(t, obj, r, region)

    m0 = t->mode0;
    m1 = t->mode1;
    img0 = t->image0;
    img1 = r->image1;
    img2 = r->image2;
    img3 = t->image3;

    SC_TX_SETMODE0_SET_RID(t->mode0, GXTexMode0Ids[id]);
    SC_TX_SETMODE1_SET_RID(t->mode1, GXTexMode1Ids[id]);
    SC_TX_SETIMAGE0_SET_RID(t->image0, GXTexImage0Ids[id]);
    SC_TX_SETIMAGE1_SET_RID(r->image1, GXTexImage1Ids[id]);
    SC_TX_SETIMAGE2_SET_RID(r->image2, GXTexImage2Ids[id]);
    SC_TX_SETIMAGE3_SET_RID(t->image3, GXTexImage3Ids[id]);
    GX_WRITE_RA_REG(t->mode0);
    GX_WRITE_RA_REG(t->mode1);
    GX_WRITE_RA_REG(t->image0);
    GX_WRITE_RA_REG(r->image1);
    GX_WRITE_RA_REG(r->image2);
    GX_WRITE_RA_REG(t->image3);

    if (!(t->flags & 2)) {
        tlr = (GXTlutRegionInt*)(gx->tlutRegionCallback)(t->tlutName);

        SC_TX_SETTLUT_SET_RID(tlr->tlutObj.tlut, GXTexTlutIds[id]);
        GX_WRITE_RA_REG(tlr->tlutObj.tlut);
    }

    gx->tImage0[id] = t->image0;
    gx->tMode0[id] = t->mode0;
    gx->dirtyState |= 1;
    BP_SENT();
}

void GXLoadTexObj(const GXTexObj* obj, GXTexMapID id) {
    GXTexRegion* r;
    r = (gx->texRegionCallback)(obj, id);
    GXLoadTexObjPreLoaded(obj, r, id);
}

void GXInitTlutObj(GXTlutObj* tlut_obj, void* lut, GXTlutFmt fmt, u16 n_entries) {
    GX_SETUP_TLUTOBJ(t, tlut_obj);
    t->tlut = 0;
    SC_TX_SETTLUT_SET_FORMAT(t->tlut, fmt);
    SC_TX_LOADTLUT0_SET_TLUT_BASE(t->loadTlut0, (GX_PHY_ADDR(lut) >> 5));
    SC_TX_LOADTLUT0_SET_RID(t->loadTlut0, 0x64);
    t->numEntries = n_entries;
}

void GXLoadTlut(const GXTlutObj* tlut_obj, u32 tlut_name) {
    GXTlutRegionInt* r;
    u32 tlut_offset;
    GX_SETUP_TLUTOBJ(t, tlut_obj);

    r = (GXTlutRegionInt*)(gx->tlutRegionCallback)(tlut_name);

    __GX_FLUSH_TEX_STATE();

    GX_WRITE_RA_REG(t->loadTlut0);
    GX_WRITE_RA_REG(r->loadTlut1);

    __GX_FLUSH_TEX_STATE();

    tlut_offset = TX_LOADTLUT1_GET_TMEM_OFFSET(r->loadTlut1);
    SC_TX_SETTLUT_SET_TMEM_OFFSET(t->tlut, tlut_offset);
    r->tlutObj = *t;
}

void GXInitTexCacheRegion(GXTexRegion* region, GXBool is_32b_mipmap, u32 tmem_even, GXTexCacheSize size_even, u32 tmem_odd, GXTexCacheSize size_odd) {
    u32 WidthExp2;
    GX_SETUP_TREGOBJ(t, region)

    switch (size_even) {
    case GX_TEXCACHE_32K:
        WidthExp2 = 3;
        break;
    case GX_TEXCACHE_128K:
        WidthExp2 = 4;
        break;
    case GX_TEXCACHE_512K:
        WidthExp2 = 5;
        break;
    default:

        break;
    }

    t->image1 = 0;
    SC_TX_SETIMAGE1_SET_TMEM_OFFSET(t->image1, ((u32)tmem_even >> 5));
    SC_TX_SETIMAGE1_SET_CACHE_WIDTH(t->image1, WidthExp2);
    SC_TX_SETIMAGE1_SET_CACHE_HEIGHT(t->image1, WidthExp2);
    SC_TX_SETIMAGE1_SET_IMAGE_TYPE(t->image1, 0);

    switch (size_odd) {
    case GX_TEXCACHE_32K:
        WidthExp2 = 3;
        break;
    case GX_TEXCACHE_128K:
        WidthExp2 = 4;
        break;
    case GX_TEXCACHE_512K:
        WidthExp2 = 5;
        break;
    case GX_TEXCACHE_NONE:
        WidthExp2 = 0;
        break;
    default:
        break;
    }

    t->image2 = 0;
    SC_TX_SETIMAGE2_SET_TMEM_OFFSET(t->image2, ((u32)tmem_odd >> 5));
    SC_TX_SETIMAGE2_SET_CACHE_WIDTH(t->image2, WidthExp2);
    SC_TX_SETIMAGE2_SET_CACHE_HEIGHT(t->image2, WidthExp2);

    t->is32bMipmap = (u8)is_32b_mipmap;
    t->isCached = GX_TRUE;
}

void GXInitTexPreLoadRegion(GXTexRegion* region, u32 tmem_even, u32 size_even, u32 tmem_odd, u32 size_odd) {
    GX_SETUP_TREGOBJ(t, region)

    t->image1 = 0;
    SC_TX_SETIMAGE1_SET_TMEM_OFFSET(t->image1, ((u32)tmem_even >> 5));
    SC_TX_SETIMAGE1_SET_CACHE_WIDTH(t->image1, 0);
    SC_TX_SETIMAGE1_SET_CACHE_HEIGHT(t->image1, 0);
    SC_TX_SETIMAGE1_SET_IMAGE_TYPE(t->image1, 1);

    t->image2 = 0;
    SC_TX_SETIMAGE2_SET_TMEM_OFFSET(t->image2, ((u32)tmem_odd >> 5));
    SC_TX_SETIMAGE2_SET_CACHE_WIDTH(t->image2, 0);
    SC_TX_SETIMAGE2_SET_CACHE_HEIGHT(t->image2, 0);

    t->is32bMipmap = 0;
    t->isCached = GX_FALSE;
    t->sizeEven = (u16)(size_even >> 5);
    t->sizeOdd = (u16)(size_odd >> 5);
}

void GXInitTlutRegion(GXTlutRegion* region, u32 tmem_addr, GXTlutSize tlut_size) {
    GX_SETUP_TLUTREGOBJ(t, region)

    t->loadTlut1 = 0;
    tmem_addr -= 0x80000;
    SC_TX_LOADTLUT1_SET_TMEM_OFFSET(t->loadTlut1, (tmem_addr >> 9));
    SC_TX_LOADTLUT1_SET_COUNT(t->loadTlut1, tlut_size);
    SC_TX_LOADTLUT1_SET_RID(t->loadTlut1, 0x65);
}

void GXInvalidateTexAll(void) {
    u32 reg0, reg1;
    reg0 = TX_INVALTAGS(0, 8, 0x66);
    reg1 = TX_INVALTAGS(256, 8, 0x66);
    __GX_FLUSH_TEX_STATE();
    GX_WRITE_RA_REG(reg0);
    GX_WRITE_RA_REG(reg1);
    __GX_FLUSH_TEX_STATE();
}

GXTexRegionCallback GXSetTexRegionCallback(GXTexRegionCallback f) {
    GXTexRegionCallback oldcb = gx->texRegionCallback;
    gx->texRegionCallback = f;
    return oldcb;
}

GXTlutRegionCallback GXSetTlutRegionCallback(GXTlutRegionCallback f) {
    GXTlutRegionCallback oldcb = gx->tlutRegionCallback;
    gx->tlutRegionCallback = f;
    return oldcb;
}

void GXSetTexCoordScaleManually(GXTexCoordID coord, GXBool enable, u16 ss, u16 ts) {
    gx->tcsManEnab = gx->tcsManEnab & ~(1 << coord) | (enable << coord);

    if (enable) {
        SC_SU_TS0_SET_SSIZE(gx->suTs0[coord], (u16)(ss - 1));
        SC_SU_TS1_SET_TSIZE(gx->suTs1[coord], (u16)(ts - 1));

        GX_WRITE_RA_REG(gx->suTs0[coord]);
        GX_WRITE_RA_REG(gx->suTs1[coord]);
        BP_SENT();
    }
}

void __SetSURegs(u32 tmap, u32 tcoord) NO_INLINE {
    u32 w, h;
    GXBool s_bias, t_bias;

    w = ((gx->tImage0[tmap] & 0x3FF));
    h = ((gx->tImage0[tmap] & 0xFFC00) >> 10);
    FAST_FLAG_SET(gx->suTs0[tcoord], w, 0, 16);
    FAST_FLAG_SET(gx->suTs1[tcoord], h, 0, 16);

    s_bias = ((gx->tMode0[tmap] & 0x3)) == GX_REPEAT;
    t_bias = ((gx->tMode0[tmap] & 0xC) >> 2) == GX_REPEAT;
    FAST_FLAG_SET(gx->suTs0[tcoord], s_bias, 16, 1);
    FAST_FLAG_SET(gx->suTs1[tcoord], t_bias, 16, 1);

    GX_WRITE_RA_REG(gx->suTs0[tcoord]);
    GX_WRITE_RA_REG(gx->suTs1[tcoord]);
    gx->bpSentNot = GX_FALSE;
}

void __GXSetSUTexRegs(void) {
    u32 nStages, nIndStages, i, map, tmap, coord;
    u32* ptref;

    if (gx->tcsManEnab == 0xFF) {
        return;
    }

    nStages = ((gx->genMode & 0x3C00) >> 10) + 1;
    nIndStages = ((gx->genMode & 0x70000) >> 16);

    for (i = 0; i < nIndStages; i++) {
        switch (i + GX_INDTEXSTAGE0) {
        case GX_INDTEXSTAGE0:
            tmap = (gx->iref) & 0x7;
            coord = ((gx->iref & 0x38) >> 3);
            break;
        case GX_INDTEXSTAGE1:
            tmap = ((gx->iref & 0x1C0) >> 6);
            coord = ((gx->iref & 0xE00) >> 9);
            break;
        case GX_INDTEXSTAGE2:
            tmap = ((gx->iref & 0x7000) >> 12);
            coord = ((gx->iref & 0x38000) >> 15);
            break;
        case GX_INDTEXSTAGE3:
            tmap = ((gx->iref & 0x1C0000) >> 18);
            coord = ((gx->iref & 0xE00000) >> 21);
            break;
        }

        if (!(gx->tcsManEnab & (1 << coord))) {
            __SetSURegs(tmap, coord);
        }
    }

    for (i = 0; i < nStages; i++) {
        ptref = &gx->tref[i / 2];
        map = gx->texmapId[i];
        tmap = map & ~256;

        if (i & 1) {
            coord = ((*ptref & 0x38000) >> 15);
        } else {
            coord = ((*ptref & 0x38) >> 3);
        }

        if ((tmap != GX_TEXMAP_NULL) && !(gx->tcsManEnab & (1 << coord)) && (gx->tevTcEnab & (1 << i))) {
            __SetSURegs(tmap, coord);
        }
    }
}

void __GXSetTmemConfig(u32 config) {
    switch (config) {
    case 2:
        GX_WRITE_RA_REG(TX_SETIMAGE1(0x00000 >> 5, 3, 3, 0, TX_SETIMAGE1_I0_ID));
        GX_WRITE_RA_REG(TX_SETIMAGE2(0x80000 >> 5, 3, 3, TX_SETIMAGE2_I0_ID));
        GX_WRITE_RA_REG(TX_SETIMAGE1(0x10000 >> 5, 3, 3, 0, TX_SETIMAGE1_I1_ID));
        GX_WRITE_RA_REG(TX_SETIMAGE2(0x90000 >> 5, 3, 3, TX_SETIMAGE2_I1_ID));
        GX_WRITE_RA_REG(TX_SETIMAGE1(0x20000 >> 5, 3, 3, 0, TX_SETIMAGE1_I2_ID));
        GX_WRITE_RA_REG(TX_SETIMAGE2(0xA0000 >> 5, 3, 3, TX_SETIMAGE2_I2_ID));
        GX_WRITE_RA_REG(TX_SETIMAGE1(0x30000 >> 5, 3, 3, 0, TX_SETIMAGE1_I3_ID));
        GX_WRITE_RA_REG(TX_SETIMAGE2(0xB0000 >> 5, 3, 3, TX_SETIMAGE2_I3_ID));
        GX_WRITE_RA_REG(TX_SETIMAGE1(0x40000 >> 5, 3, 3, 0, TX_SETIMAGE1_I4_ID));
        GX_WRITE_RA_REG(TX_SETIMAGE2(0x88000 >> 5, 3, 3, TX_SETIMAGE2_I4_ID));
        GX_WRITE_RA_REG(TX_SETIMAGE1(0x50000 >> 5, 3, 3, 0, TX_SETIMAGE1_I5_ID));
        GX_WRITE_RA_REG(TX_SETIMAGE2(0x98000 >> 5, 3, 3, TX_SETIMAGE2_I5_ID));
        GX_WRITE_RA_REG(TX_SETIMAGE1(0x60000 >> 5, 3, 3, 0, TX_SETIMAGE1_I6_ID));
        GX_WRITE_RA_REG(TX_SETIMAGE2(0xA8000 >> 5, 3, 3, TX_SETIMAGE2_I6_ID));
        GX_WRITE_RA_REG(TX_SETIMAGE1(0x70000 >> 5, 3, 3, 0, TX_SETIMAGE1_I7_ID));
        GX_WRITE_RA_REG(TX_SETIMAGE2(0xB8000 >> 5, 3, 3, TX_SETIMAGE2_I7_ID));
        break;
    case 1:
        GX_WRITE_RA_REG(TX_SETIMAGE1(0x00000 >> 5, 3, 3, 0, TX_SETIMAGE1_I0_ID));
        GX_WRITE_RA_REG(TX_SETIMAGE2(0x80000 >> 5, 3, 3, TX_SETIMAGE2_I0_ID));
        GX_WRITE_RA_REG(TX_SETIMAGE1(0x10000 >> 5, 3, 3, 0, TX_SETIMAGE1_I1_ID));
        GX_WRITE_RA_REG(TX_SETIMAGE2(0x90000 >> 5, 3, 3, TX_SETIMAGE2_I1_ID));
        GX_WRITE_RA_REG(TX_SETIMAGE1(0x20000 >> 5, 3, 3, 0, TX_SETIMAGE1_I2_ID));
        GX_WRITE_RA_REG(TX_SETIMAGE2(0xa0000 >> 5, 3, 3, TX_SETIMAGE2_I2_ID));
        GX_WRITE_RA_REG(TX_SETIMAGE1(0x30000 >> 5, 3, 3, 0, TX_SETIMAGE1_I3_ID));
        GX_WRITE_RA_REG(TX_SETIMAGE2(0xb0000 >> 5, 3, 3, TX_SETIMAGE2_I3_ID));
        GX_WRITE_RA_REG(TX_SETIMAGE1(0x40000 >> 5, 3, 3, 0, TX_SETIMAGE1_I4_ID));
        GX_WRITE_RA_REG(TX_SETIMAGE2(0xc0000 >> 5, 3, 3, TX_SETIMAGE2_I4_ID));
        GX_WRITE_RA_REG(TX_SETIMAGE1(0x50000 >> 5, 3, 3, 0, TX_SETIMAGE1_I5_ID));
        GX_WRITE_RA_REG(TX_SETIMAGE2(0xd0000 >> 5, 3, 3, TX_SETIMAGE2_I5_ID));
        GX_WRITE_RA_REG(TX_SETIMAGE1(0x60000 >> 5, 3, 3, 0, TX_SETIMAGE1_I6_ID));
        GX_WRITE_RA_REG(TX_SETIMAGE2(0xe0000 >> 5, 3, 3, TX_SETIMAGE2_I6_ID));
        GX_WRITE_RA_REG(TX_SETIMAGE1(0x70000 >> 5, 3, 3, 0, TX_SETIMAGE1_I7_ID));
        GX_WRITE_RA_REG(TX_SETIMAGE2(0xf0000 >> 5, 3, 3, TX_SETIMAGE2_I7_ID));
        break;

    default:
    case 0:
        GX_WRITE_RA_REG(TX_SETIMAGE1(0x00000 >> 5, 3, 3, 0, TX_SETIMAGE1_I0_ID));
        GX_WRITE_RA_REG(TX_SETIMAGE2(0x80000 >> 5, 3, 3, TX_SETIMAGE2_I0_ID));
        GX_WRITE_RA_REG(TX_SETIMAGE1(0x08000 >> 5, 3, 3, 0, TX_SETIMAGE1_I1_ID));
        GX_WRITE_RA_REG(TX_SETIMAGE2(0x88000 >> 5, 3, 3, TX_SETIMAGE2_I1_ID));
        GX_WRITE_RA_REG(TX_SETIMAGE1(0x10000 >> 5, 3, 3, 0, TX_SETIMAGE1_I2_ID));
        GX_WRITE_RA_REG(TX_SETIMAGE2(0x90000 >> 5, 3, 3, TX_SETIMAGE2_I2_ID));
        GX_WRITE_RA_REG(TX_SETIMAGE1(0x18000 >> 5, 3, 3, 0, TX_SETIMAGE1_I3_ID));
        GX_WRITE_RA_REG(TX_SETIMAGE2(0x98000 >> 5, 3, 3, TX_SETIMAGE2_I3_ID));
        GX_WRITE_RA_REG(TX_SETIMAGE1(0x20000 >> 5, 3, 3, 0, TX_SETIMAGE1_I4_ID));
        GX_WRITE_RA_REG(TX_SETIMAGE2(0xa0000 >> 5, 3, 3, TX_SETIMAGE2_I4_ID));
        GX_WRITE_RA_REG(TX_SETIMAGE1(0x28000 >> 5, 3, 3, 0, TX_SETIMAGE1_I5_ID));
        GX_WRITE_RA_REG(TX_SETIMAGE2(0xa8000 >> 5, 3, 3, TX_SETIMAGE2_I5_ID));
        GX_WRITE_RA_REG(TX_SETIMAGE1(0x30000 >> 5, 3, 3, 0, TX_SETIMAGE1_I6_ID));
        GX_WRITE_RA_REG(TX_SETIMAGE2(0xb0000 >> 5, 3, 3, TX_SETIMAGE2_I6_ID));
        GX_WRITE_RA_REG(TX_SETIMAGE1(0x38000 >> 5, 3, 3, 0, TX_SETIMAGE1_I7_ID));
        GX_WRITE_RA_REG(TX_SETIMAGE2(0xb8000 >> 5, 3, 3, TX_SETIMAGE2_I7_ID));
        break;
    }
}
