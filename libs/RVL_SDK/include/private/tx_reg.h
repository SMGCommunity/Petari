#ifndef TX_REG_H
#define TX_REG_H

#define TX_MIN_NEAREST 0x00000000
#define TX_MIN_NEAREST_MIPMAP_NEAREST 0x00000001
#define TX_MIN_NEAREST_MIPMAP_LINEAR 0x00000002
#define TX_MIN_FILTER_UNUSED_3 0x00000003
#define TX_MIN_LINEAR 0x00000004
#define TX_MIN_LINEAR_MIPMAP_NEAREST 0x00000005
#define TX_MIN_LINEAR_MIPMAP_LINEAR 0x00000006
#define TX_MIN_FILTER_UNUSED_7 0x00000007

#define TX_REFRESH_INTERVAL_SIZE 10
#define TX_REFRESH_INTERVAL_SHIFT 0
#define TX_REFRESH_INTERVAL_MASK 0x000003ff
#define TX_REFRESH_GET_INTERVAL(tx_refresh) ((((unsigned long)(tx_refresh)) & TX_REFRESH_INTERVAL_MASK) >> TX_REFRESH_INTERVAL_SHIFT)

#define TX_REFRESH_ENABLE_SIZE 1
#define TX_REFRESH_ENABLE_SHIFT 10
#define TX_REFRESH_ENABLE_MASK 0x00000400
#define TX_REFRESH_GET_ENABLE(tx_refresh) ((((unsigned long)(tx_refresh)) & TX_REFRESH_ENABLE_MASK) >> TX_REFRESH_ENABLE_SHIFT)

#define TX_REFRESH_PAD0_SIZE 13
#define TX_REFRESH_PAD0_SHIFT 11
#define TX_REFRESH_PAD0_MASK 0x00fff800
#define TX_REFRESH_GET_PAD0(tx_refresh) ((((unsigned long)(tx_refresh)) & TX_REFRESH_PAD0_MASK) >> TX_REFRESH_PAD0_SHIFT)
#define TX_REFRESH_SET_PAD0(tx_refresh, pad0)                                                                                                        \
    { tx_refresh = (((unsigned long)(tx_refresh)) & ~TX_REFRESH_PAD0_MASK) | (((unsigned long)(pad0)) << TX_REFRESH_PAD0_SHIFT); }
#define TX_REFRESH_RID_SIZE 8
#define TX_REFRESH_RID_SHIFT 24
#define TX_REFRESH_RID_MASK 0xff000000
#define TX_REFRESH_GET_RID(tx_refresh) ((((unsigned long)(tx_refresh)) & TX_REFRESH_RID_MASK) >> TX_REFRESH_RID_SHIFT)
#define TX_REFRESH_SET_RID(tx_refresh, rid)                                                                                                          \
    { tx_refresh = (((unsigned long)(tx_refresh)) & ~TX_REFRESH_RID_MASK) | (((unsigned long)(rid)) << TX_REFRESH_RID_SHIFT); }
#define TX_REFRESH_TOTAL_SIZE 32
#define TX_REFRESH(interval, enable, rid)                                                                                                            \
    ((((unsigned long)(interval)) << TX_REFRESH_INTERVAL_SHIFT) | (((unsigned long)(enable)) << TX_REFRESH_ENABLE_SHIFT) |                           \
     (((unsigned long)(rid)) << TX_REFRESH_RID_SHIFT))

#define TX_PERFMODE_SEL3_SIZE 5
#define TX_PERFMODE_SEL3_SHIFT 0
#define TX_PERFMODE_SEL3_MASK 0x0000001f
#define TX_PERFMODE_GET_SEL3(tx_perfmode) ((((unsigned long)(tx_perfmode)) & TX_PERFMODE_SEL3_MASK) >> TX_PERFMODE_SEL3_SHIFT)

#define TX_PERFMODE_SEL2_SIZE 5
#define TX_PERFMODE_SEL2_SHIFT 5
#define TX_PERFMODE_SEL2_MASK 0x000003e0
#define TX_PERFMODE_GET_SEL2(tx_perfmode) ((((unsigned long)(tx_perfmode)) & TX_PERFMODE_SEL2_MASK) >> TX_PERFMODE_SEL2_SHIFT)

#define TX_PERFMODE_SEL1_SIZE 5
#define TX_PERFMODE_SEL1_SHIFT 10
#define TX_PERFMODE_SEL1_MASK 0x00007c00
#define TX_PERFMODE_GET_SEL1(tx_perfmode) ((((unsigned long)(tx_perfmode)) & TX_PERFMODE_SEL1_MASK) >> TX_PERFMODE_SEL1_SHIFT)

#define TX_PERFMODE_SEL0_SIZE 5
#define TX_PERFMODE_SEL0_SHIFT 15
#define TX_PERFMODE_SEL0_MASK 0x000f8000
#define TX_PERFMODE_GET_SEL0(tx_perfmode) ((((unsigned long)(tx_perfmode)) & TX_PERFMODE_SEL0_MASK) >> TX_PERFMODE_SEL0_SHIFT)

#define TX_PERFMODE_PAD0_SIZE 4
#define TX_PERFMODE_PAD0_SHIFT 20
#define TX_PERFMODE_PAD0_MASK 0x00f00000
#define TX_PERFMODE_GET_PAD0(tx_perfmode) ((((unsigned long)(tx_perfmode)) & TX_PERFMODE_PAD0_MASK) >> TX_PERFMODE_PAD0_SHIFT)

#define TX_PERFMODE_RID_SIZE 8
#define TX_PERFMODE_RID_SHIFT 24
#define TX_PERFMODE_RID_MASK 0xff000000
#define TX_PERFMODE_GET_RID(tx_perfmode) ((((unsigned long)(tx_perfmode)) & TX_PERFMODE_RID_MASK) >> TX_PERFMODE_RID_SHIFT)

#define TX_PERFMODE_TOTAL_SIZE 32
#define TX_PERFMODE(sel3, sel2, sel1, sel0, rid)                                                                                                     \
    ((((unsigned long)(sel3)) << TX_PERFMODE_SEL3_SHIFT) | (((unsigned long)(sel2)) << TX_PERFMODE_SEL2_SHIFT) |                                     \
     (((unsigned long)(sel1)) << TX_PERFMODE_SEL1_SHIFT) | (((unsigned long)(sel0)) << TX_PERFMODE_SEL0_SHIFT) |                                     \
     (((unsigned long)(rid)) << TX_PERFMODE_RID_SHIFT))

#define TX_LOADBLOCK3_COUNT_SIZE 15
#define TX_LOADBLOCK3_COUNT_SHIFT 0
#define TX_LOADBLOCK3_COUNT_MASK 0x00007fff
#define TX_LOADBLOCK3_GET_COUNT(tx_loadblock3) ((((unsigned long)(tx_loadblock3)) & TX_LOADBLOCK3_COUNT_MASK) >> TX_LOADBLOCK3_COUNT_SHIFT)
#define TX_LOADBLOCK3_SET_COUNT(tx_loadblock3, count)                                                                                                \
    { tx_loadblock3 = (((unsigned long)(tx_loadblock3)) & ~TX_LOADBLOCK3_COUNT_MASK) | (((unsigned long)(count)) << TX_LOADBLOCK3_COUNT_SHIFT); }
#define TX_LOADBLOCK3_FORMAT_SIZE 2
#define TX_LOADBLOCK3_FORMAT_SHIFT 15
#define TX_LOADBLOCK3_FORMAT_MASK 0x00018000
#define TX_LOADBLOCK3_GET_FORMAT(tx_loadblock3) ((((unsigned long)(tx_loadblock3)) & TX_LOADBLOCK3_FORMAT_MASK) >> TX_LOADBLOCK3_FORMAT_SHIFT)
#define TX_LOADBLOCK3_SET_FORMAT(tx_loadblock3, format)                                                                                              \
    { tx_loadblock3 = (((unsigned long)(tx_loadblock3)) & ~TX_LOADBLOCK3_FORMAT_MASK) | (((unsigned long)(format)) << TX_LOADBLOCK3_FORMAT_SHIFT); }
#define TX_LOADBLOCK3_PAD0_SIZE 7
#define TX_LOADBLOCK3_PAD0_SHIFT 17
#define TX_LOADBLOCK3_PAD0_MASK 0x00fe0000
#define TX_LOADBLOCK3_GET_PAD0(tx_loadblock3) ((((unsigned long)(tx_loadblock3)) & TX_LOADBLOCK3_PAD0_MASK) >> TX_LOADBLOCK3_PAD0_SHIFT)
#define TX_LOADBLOCK3_SET_PAD0(tx_loadblock3, pad0)                                                                                                  \
    { tx_loadblock3 = (((unsigned long)(tx_loadblock3)) & ~TX_LOADBLOCK3_PAD0_MASK) | (((unsigned long)(pad0)) << TX_LOADBLOCK3_PAD0_SHIFT); }
#define TX_LOADBLOCK3_RID_SIZE 8
#define TX_LOADBLOCK3_RID_SHIFT 24
#define TX_LOADBLOCK3_RID_MASK 0xff000000
#define TX_LOADBLOCK3_GET_RID(tx_loadblock3) ((((unsigned long)(tx_loadblock3)) & TX_LOADBLOCK3_RID_MASK) >> TX_LOADBLOCK3_RID_SHIFT)
#define TX_LOADBLOCK3_SET_RID(tx_loadblock3, rid)                                                                                                    \
    { tx_loadblock3 = (((unsigned long)(tx_loadblock3)) & ~TX_LOADBLOCK3_RID_MASK) | (((unsigned long)(rid)) << TX_LOADBLOCK3_RID_SHIFT); }
#define TX_LOADBLOCK3_TOTAL_SIZE 32
#define TX_LOADBLOCK3(count, format, rid)                                                                                                            \
    ((((unsigned long)(count)) << TX_LOADBLOCK3_COUNT_SHIFT) | (((unsigned long)(format)) << TX_LOADBLOCK3_FORMAT_SHIFT) |                           \
     (((unsigned long)(rid)) << TX_LOADBLOCK3_RID_SHIFT))

#define TX_MISC_FIELD_LOD_SIZE 1
#define TX_MISC_FIELD_LOD_SHIFT 0
#define TX_MISC_FIELD_LOD_MASK 0x00000001
#define TX_MISC_GET_FIELD_LOD(tx_misc) ((((unsigned long)(tx_misc)) & TX_MISC_FIELD_LOD_MASK) >> TX_MISC_FIELD_LOD_SHIFT)
#define TX_MISC_SET_FIELD_LOD(tx_misc, field_lod)                                                                                                    \
    { tx_misc = (((unsigned long)(tx_misc)) & ~TX_MISC_FIELD_LOD_MASK) | (((unsigned long)(field_lod)) << TX_MISC_FIELD_LOD_SHIFT); }

#define TX_MISC_ABORT_ALL_SIZE 1
#define TX_MISC_ABORT_ALL_SHIFT 1
#define TX_MISC_ABORT_ALL_MASK 0x00000002
#define TX_MISC_GET_ABORT_ALL(tx_misc) ((((unsigned long)(tx_misc)) & TX_MISC_ABORT_ALL_MASK) >> TX_MISC_ABORT_ALL_SHIFT)
#define TX_MISC_SET_ABORT_ALL(tx_misc, abort_all)                                                                                                    \
    { tx_misc = (((unsigned long)(tx_misc)) & ~TX_MISC_ABORT_ALL_MASK) | (((unsigned long)(abort_all)) << TX_MISC_ABORT_ALL_SHIFT); }

#define TX_MISC_RID_SIZE 8
#define TX_MISC_RID_SHIFT 24
#define TX_MISC_RID_MASK 0xff000000
#define TX_MISC_GET_RID(tx_misc) ((((unsigned long)(tx_misc)) & TX_MISC_RID_MASK) >> TX_MISC_RID_SHIFT)
#define TX_MISC_SET_RID(tx_misc, rid)                                                                                                                \
    { tx_misc = (((unsigned long)(tx_misc)) & ~TX_MISC_RID_MASK) | (((unsigned long)(rid)) << TX_MISC_RID_SHIFT); }

#define TX_MISC_TOTAL_SIZE 32
#define TX_MISC(field_lod, abort_all, rid)                                                                                                           \
    ((((unsigned long)(field_lod)) << TX_MISC_FIELD_LOD_SHIFT) | (((unsigned long)(abort_all)) << TX_MISC_ABORT_ALL_SHIFT) |                         \
     (((unsigned long)(rid)) << TX_MISC_RID_SHIFT))

#define TX_SETMODE0_WRAP_S_SIZE 2
#define TX_SETMODE0_WRAP_S_SHIFT 0
#define TX_SETMODE0_WRAP_S_MASK 0x00000003
#define TX_SETMODE0_GET_WRAP_S(tx_setmode0) ((((unsigned long)(tx_setmode0)) & TX_SETMODE0_WRAP_S_MASK) >> TX_SETMODE0_WRAP_S_SHIFT)
#define TX_SETMODE0_SET_WRAP_S(tx_setmode0, wrap_s)                                                                                                  \
    { tx_setmode0 = (((unsigned long)(tx_setmode0)) & ~TX_SETMODE0_WRAP_S_MASK) | (((unsigned long)(wrap_s)) << TX_SETMODE0_WRAP_S_SHIFT); }
#define TX_SETMODE0_WRAP_T_SIZE 2
#define TX_SETMODE0_WRAP_T_SHIFT 2
#define TX_SETMODE0_WRAP_T_MASK 0x0000000c
#define TX_SETMODE0_GET_WRAP_T(tx_setmode0) ((((unsigned long)(tx_setmode0)) & TX_SETMODE0_WRAP_T_MASK) >> TX_SETMODE0_WRAP_T_SHIFT)
#define TX_SETMODE0_SET_WRAP_T(tx_setmode0, wrap_t)                                                                                                  \
    { tx_setmode0 = (((unsigned long)(tx_setmode0)) & ~TX_SETMODE0_WRAP_T_MASK) | (((unsigned long)(wrap_t)) << TX_SETMODE0_WRAP_T_SHIFT); }

#define TX_SETMODE0_MAG_FILTER_SIZE 1
#define TX_SETMODE0_MAG_FILTER_SHIFT 4
#define TX_SETMODE0_MAG_FILTER_MASK 0x00000010
#define TX_SETMODE0_GET_MAG_FILTER(tx_setmode0) ((((unsigned long)(tx_setmode0)) & TX_SETMODE0_MAG_FILTER_MASK) >> TX_SETMODE0_MAG_FILTER_SHIFT)
#define TX_SETMODE0_SET_MAG_FILTER(tx_setmode0, mag_filter)                                                                                          \
    {                                                                                                                                                \
        tx_setmode0 =                                                                                                                                \
            (((unsigned long)(tx_setmode0)) & ~TX_SETMODE0_MAG_FILTER_MASK) | (((unsigned long)(mag_filter)) << TX_SETMODE0_MAG_FILTER_SHIFT);       \
    }

#define TX_SETMODE0_MIN_FILTER_SIZE 3
#define TX_SETMODE0_MIN_FILTER_SHIFT 5
#define TX_SETMODE0_MIN_FILTER_MASK 0x000000e0
#define TX_SETMODE0_GET_MIN_FILTER(tx_setmode0) ((((unsigned long)(tx_setmode0)) & TX_SETMODE0_MIN_FILTER_MASK) >> TX_SETMODE0_MIN_FILTER_SHIFT)
#define TX_SETMODE0_SET_MIN_FILTER(tx_setmode0, min_filter)                                                                                          \
    {                                                                                                                                                \
        tx_setmode0 =                                                                                                                                \
            (((unsigned long)(tx_setmode0)) & ~TX_SETMODE0_MIN_FILTER_MASK) | (((unsigned long)(min_filter)) << TX_SETMODE0_MIN_FILTER_SHIFT);       \
    }

#define TX_SETMODE1_MAXLOD_SIZE 8
#define TX_SETMODE1_MAXLOD_SHIFT 8
#define TX_SETMODE1_MAXLOD_MASK 0x0000ff00
#define TX_SETMODE1_GET_MAXLOD(tx_setmode1) ((((unsigned long)(tx_setmode1)) & TX_SETMODE1_MAXLOD_MASK) >> TX_SETMODE1_MAXLOD_SHIFT)
#define TX_SETMODE1_SET_MAXLOD(tx_setmode1, maxlod)                                                                                                  \
    { tx_setmode1 = (((unsigned long)(tx_setmode1)) & ~TX_SETMODE1_MAXLOD_MASK) | (((unsigned long)(maxlod)) << TX_SETMODE1_MAXLOD_SHIFT); }

#define TX_SETIMAGE0_IMAGE_WIDTH_SIZE 10
#define TX_SETIMAGE0_IMAGE_WIDTH_SHIFT 0
#define TX_SETIMAGE0_IMAGE_WIDTH_MASK 0x000003ff
#define TX_SETIMAGE0_GET_IMAGE_WIDTH(tx_setimage0)                                                                                                   \
    ((((unsigned long)(tx_setimage0)) & TX_SETIMAGE0_IMAGE_WIDTH_MASK) >> TX_SETIMAGE0_IMAGE_WIDTH_SHIFT)
#define TX_SETIMAGE0_SET_IMAGE_WIDTH(tx_setimage0, image_width)                                                                                      \
    {                                                                                                                                                \
        tx_setimage0 =                                                                                                                               \
            (((unsigned long)(tx_setimage0)) & ~TX_SETIMAGE0_IMAGE_WIDTH_MASK) | (((unsigned long)(image_width)) << TX_SETIMAGE0_IMAGE_WIDTH_SHIFT); \
    }

#define TX_SETIMAGE0_IMAGE_HEIGHT_SIZE 10
#define TX_SETIMAGE0_IMAGE_HEIGHT_SHIFT 10
#define TX_SETIMAGE0_IMAGE_HEIGHT_MASK 0x000ffc00
#define TX_SETIMAGE0_GET_IMAGE_HEIGHT(tx_setimage0)                                                                                                  \
    ((((unsigned long)(tx_setimage0)) & TX_SETIMAGE0_IMAGE_HEIGHT_MASK) >> TX_SETIMAGE0_IMAGE_HEIGHT_SHIFT)
#define TX_SETIMAGE0_SET_IMAGE_HEIGHT(tx_setimage0, image_height)                                                                                    \
    {                                                                                                                                                \
        tx_setimage0 = (((unsigned long)(tx_setimage0)) & ~TX_SETIMAGE0_IMAGE_HEIGHT_MASK) |                                                         \
                       (((unsigned long)(image_height)) << TX_SETIMAGE0_IMAGE_HEIGHT_SHIFT);                                                         \
    }

#define TX_SETIMAGE0_FORMAT_SIZE 4
#define TX_SETIMAGE0_FORMAT_SHIFT 20
#define TX_SETIMAGE0_FORMAT_MASK 0x00f00000
#define TX_SETIMAGE0_GET_FORMAT(tx_setimage0) ((((unsigned long)(tx_setimage0)) & TX_SETIMAGE0_FORMAT_MASK) >> TX_SETIMAGE0_FORMAT_SHIFT)
#define TX_SETIMAGE0_SET_FORMAT(tx_setimage0, format)                                                                                                \
    { tx_setimage0 = (((unsigned long)(tx_setimage0)) & ~TX_SETIMAGE0_FORMAT_MASK) | (((unsigned long)(format)) << TX_SETIMAGE0_FORMAT_SHIFT); }

#define TX_SETIMAGE3_IMAGE_BASE_SIZE 24
#define TX_SETIMAGE3_IMAGE_BASE_SHIFT 0
#define TX_SETIMAGE3_IMAGE_BASE_MASK 0x00ffffff
#define TX_SETIMAGE3_GET_IMAGE_BASE(tx_setimage3) ((((unsigned long)(tx_setimage3)) & TX_SETIMAGE3_IMAGE_BASE_MASK) >> TX_SETIMAGE3_IMAGE_BASE_SHIFT)
#define TX_SETIMAGE3_SET_IMAGE_BASE(tx_setimage3, image_base)                                                                                        \
    {                                                                                                                                                \
        tx_setimage3 =                                                                                                                               \
            (((unsigned long)(tx_setimage3)) & ~TX_SETIMAGE3_IMAGE_BASE_MASK) | (((unsigned long)(image_base)) << TX_SETIMAGE3_IMAGE_BASE_SHIFT);    \
    }

#define TX_SETMODE0_DIAGLOD_ENABLE_SIZE 1
#define TX_SETMODE0_DIAGLOD_ENABLE_SHIFT 8
#define TX_SETMODE0_DIAGLOD_ENABLE_MASK 0x00000100
#define TX_SETMODE0_GET_DIAGLOD_ENABLE(tx_setmode0)                                                                                                  \
    ((((unsigned long)(tx_setmode0)) & TX_SETMODE0_DIAGLOD_ENABLE_MASK) >> TX_SETMODE0_DIAGLOD_ENABLE_SHIFT)
#define TX_SETMODE0_SET_DIAGLOD_ENABLE(tx_setmode0, diaglod_enable)                                                                                  \
    {                                                                                                                                                \
        tx_setmode0 = (((unsigned long)(tx_setmode0)) & ~TX_SETMODE0_DIAGLOD_ENABLE_MASK) |                                                          \
                      (((unsigned long)(diaglod_enable)) << TX_SETMODE0_DIAGLOD_ENABLE_SHIFT);                                                       \
    }

#define TX_SETMODE0_LODBIAS_SIZE 8
#define TX_SETMODE0_LODBIAS_SHIFT 9
#define TX_SETMODE0_LODBIAS_MASK 0x0001fe00
#define TX_SETMODE0_GET_LODBIAS(tx_setmode0) ((((unsigned long)(tx_setmode0)) & TX_SETMODE0_LODBIAS_MASK) >> TX_SETMODE0_LODBIAS_SHIFT)
#define TX_SETMODE0_SET_LODBIAS(tx_setmode0, lodbias)                                                                                                \
    { tx_setmode0 = (((unsigned long)(tx_setmode0)) & ~TX_SETMODE0_LODBIAS_MASK) | (((unsigned long)(lodbias)) << TX_SETMODE0_LODBIAS_SHIFT); }

#define TX_SETMODE0_ROUND_SIZE 1
#define TX_SETMODE0_ROUND_SHIFT 17
#define TX_SETMODE0_ROUND_MASK 0x00020000
#define TX_SETMODE0_GET_ROUND(tx_setmode0) ((((unsigned long)(tx_setmode0)) & TX_SETMODE0_ROUND_MASK) >> TX_SETMODE0_ROUND_SHIFT)
#define TX_SETMODE0_SET_ROUND(tx_setmode0, round)                                                                                                    \
    { tx_setmode0 = (((unsigned long)(tx_setmode0)) & ~TX_SETMODE0_ROUND_MASK) | (((unsigned long)(round)) << TX_SETMODE0_ROUND_SHIFT); }

#define TX_SETMODE0_FIELD_PREDICT_SIZE 1
#define TX_SETMODE0_FIELD_PREDICT_SHIFT 18
#define TX_SETMODE0_FIELD_PREDICT_MASK 0x00040000
#define TX_SETMODE0_GET_FIELD_PREDICT(tx_setmode0)                                                                                                   \
    ((((unsigned long)(tx_setmode0)) & TX_SETMODE0_FIELD_PREDICT_MASK) >> TX_SETMODE0_FIELD_PREDICT_SHIFT)
#define TX_SETMODE0_SET_FIELD_PREDICT(tx_setmode0, field_predict)                                                                                    \
    {                                                                                                                                                \
        tx_setmode0 = (((unsigned long)(tx_setmode0)) & ~TX_SETMODE0_FIELD_PREDICT_MASK) |                                                           \
                      (((unsigned long)(field_predict)) << TX_SETMODE0_FIELD_PREDICT_SHIFT);                                                         \
    }

#define TX_SETMODE0_MAXANISO_SIZE 2
#define TX_SETMODE0_MAXANISO_SHIFT 19
#define TX_SETMODE0_MAXANISO_MASK 0x00180000
#define TX_SETMODE0_GET_MAXANISO(tx_setmode0) ((((unsigned long)(tx_setmode0)) & TX_SETMODE0_MAXANISO_MASK) >> TX_SETMODE0_MAXANISO_SHIFT)
#define TX_SETMODE0_SET_MAXANISO(tx_setmode0, maxaniso)                                                                                              \
    { tx_setmode0 = (((unsigned long)(tx_setmode0)) & ~TX_SETMODE0_MAXANISO_MASK) | (((unsigned long)(maxaniso)) << TX_SETMODE0_MAXANISO_SHIFT); }
#define TX_SETMODE0_LODCLAMP_SIZE 1
#define TX_SETMODE0_LODCLAMP_SHIFT 21
#define TX_SETMODE0_LODCLAMP_MASK 0x00200000
#define TX_SETMODE0_GET_LODCLAMP(tx_setmode0) ((((unsigned long)(tx_setmode0)) & TX_SETMODE0_LODCLAMP_MASK) >> TX_SETMODE0_LODCLAMP_SHIFT)
#define TX_SETMODE0_SET_LODCLAMP(tx_setmode0, lodclamp)                                                                                              \
    { tx_setmode0 = (((unsigned long)(tx_setmode0)) & ~TX_SETMODE0_LODCLAMP_MASK) | (((unsigned long)(lodclamp)) << TX_SETMODE0_LODCLAMP_SHIFT); }

#define TX_SETMODE1_MINLOD_SIZE 8
#define TX_SETMODE1_MINLOD_SHIFT 0
#define TX_SETMODE1_MINLOD_MASK 0x000000ff
#define TX_SETMODE1_GET_MINLOD(tx_setmode1) ((((unsigned long)(tx_setmode1)) & TX_SETMODE1_MINLOD_MASK) >> TX_SETMODE1_MINLOD_SHIFT)
#define TX_SETMODE1_SET_MINLOD(tx_setmode1, minlod)                                                                                                  \
    { tx_setmode1 = (((unsigned long)(tx_setmode1)) & ~TX_SETMODE1_MINLOD_MASK) | (((unsigned long)(minlod)) << TX_SETMODE1_MINLOD_SHIFT); }
#define TX_SETMODE1_MAXLOD_SIZE 8
#define TX_SETMODE1_MAXLOD_SHIFT 8
#define TX_SETMODE1_MAXLOD_MASK 0x0000ff00
#define TX_SETMODE1_GET_MAXLOD(tx_setmode1) ((((unsigned long)(tx_setmode1)) & TX_SETMODE1_MAXLOD_MASK) >> TX_SETMODE1_MAXLOD_SHIFT)

#define TX_SETMODE0_RID_SIZE 8
#define TX_SETMODE0_RID_SHIFT 24
#define TX_SETMODE0_RID_MASK 0xff000000
#define TX_SETMODE0_GET_RID(tx_setmode0) ((((unsigned long)(tx_setmode0)) & TX_SETMODE0_RID_MASK) >> TX_SETMODE0_RID_SHIFT)
#define TX_SETMODE0_SET_RID(tx_setmode0, rid)                                                                                                        \
    { tx_setmode0 = (((unsigned long)(tx_setmode0)) & ~TX_SETMODE0_RID_MASK) | (((unsigned long)(rid)) << TX_SETMODE0_RID_SHIFT); }

#define TX_SETMODE1_RID_SIZE 8
#define TX_SETMODE1_RID_SHIFT 24
#define TX_SETMODE1_RID_MASK 0xff000000
#define TX_SETMODE1_GET_RID(tx_setmode1) ((((unsigned long)(tx_setmode1)) & TX_SETMODE1_RID_MASK) >> TX_SETMODE1_RID_SHIFT)
#define TX_SETMODE1_SET_RID(tx_setmode1, rid)                                                                                                        \
    { tx_setmode1 = (((unsigned long)(tx_setmode1)) & ~TX_SETMODE1_RID_MASK) | (((unsigned long)(rid)) << TX_SETMODE1_RID_SHIFT); }

#define TX_SETIMAGE0_RID_SIZE 8
#define TX_SETIMAGE0_RID_SHIFT 24
#define TX_SETIMAGE0_RID_MASK 0xff000000
#define TX_SETIMAGE0_GET_RID(tx_setimage0) ((((unsigned long)(tx_setimage0)) & TX_SETIMAGE0_RID_MASK) >> TX_SETIMAGE0_RID_SHIFT)
#define TX_SETIMAGE0_SET_RID(tx_setimage0, rid)                                                                                                      \
    { tx_setimage0 = (((unsigned long)(tx_setimage0)) & ~TX_SETIMAGE0_RID_MASK) | (((unsigned long)(rid)) << TX_SETIMAGE0_RID_SHIFT); }

#define TX_SETIMAGE1_RID_SIZE 8
#define TX_SETIMAGE1_RID_SHIFT 24
#define TX_SETIMAGE1_RID_MASK 0xff000000
#define TX_SETIMAGE1_GET_RID(tx_setimage1) ((((unsigned long)(tx_setimage1)) & TX_SETIMAGE1_RID_MASK) >> TX_SETIMAGE1_RID_SHIFT)
#define TX_SETIMAGE1_SET_RID(tx_setimage1, rid)                                                                                                      \
    { tx_setimage1 = (((unsigned long)(tx_setimage1)) & ~TX_SETIMAGE1_RID_MASK) | (((unsigned long)(rid)) << TX_SETIMAGE1_RID_SHIFT); }

#define TX_SETIMAGE2_RID_SIZE 8
#define TX_SETIMAGE2_RID_SHIFT 24
#define TX_SETIMAGE2_RID_MASK 0xff000000
#define TX_SETIMAGE2_GET_RID(tx_setimage2) ((((unsigned long)(tx_setimage2)) & TX_SETIMAGE2_RID_MASK) >> TX_SETIMAGE2_RID_SHIFT)
#define TX_SETIMAGE2_SET_RID(tx_setimage2, rid)                                                                                                      \
    { tx_setimage2 = (((unsigned long)(tx_setimage2)) & ~TX_SETIMAGE2_RID_MASK) | (((unsigned long)(rid)) << TX_SETIMAGE2_RID_SHIFT); }

#define TX_SETIMAGE3_RID_SIZE 8
#define TX_SETIMAGE3_RID_SHIFT 24
#define TX_SETIMAGE3_RID_MASK 0xff000000
#define TX_SETIMAGE3_GET_RID(tx_setimage3) ((((unsigned long)(tx_setimage3)) & TX_SETIMAGE3_RID_MASK) >> TX_SETIMAGE3_RID_SHIFT)
#define TX_SETIMAGE3_SET_RID(tx_setimage3, rid)                                                                                                      \
    { tx_setimage3 = (((unsigned long)(tx_setimage3)) & ~TX_SETIMAGE3_RID_MASK) | (((unsigned long)(rid)) << TX_SETIMAGE3_RID_SHIFT); }

#define TX_SETTLUT_RID_SIZE 8
#define TX_SETTLUT_RID_SHIFT 24
#define TX_SETTLUT_RID_MASK 0xff000000
#define TX_SETTLUT_GET_RID(tx_settlut) ((((unsigned long)(tx_settlut)) & TX_SETTLUT_RID_MASK) >> TX_SETTLUT_RID_SHIFT)
#define TX_SETTLUT_SET_RID(tx_settlut, rid)                                                                                                          \
    { tx_settlut = (((unsigned long)(tx_settlut)) & ~TX_SETTLUT_RID_MASK) | (((unsigned long)(rid)) << TX_SETTLUT_RID_SHIFT); }

#define TX_SETTLUT_FORMAT_SIZE 2
#define TX_SETTLUT_FORMAT_SHIFT 10
#define TX_SETTLUT_FORMAT_MASK 0x00000c00
#define TX_SETTLUT_GET_FORMAT(tx_settlut) ((((unsigned long)(tx_settlut)) & TX_SETTLUT_FORMAT_MASK) >> TX_SETTLUT_FORMAT_SHIFT)
#define TX_SETTLUT_SET_FORMAT(tx_settlut, format)                                                                                                    \
    { tx_settlut = (((unsigned long)(tx_settlut)) & ~TX_SETTLUT_FORMAT_MASK) | (((unsigned long)(format)) << TX_SETTLUT_FORMAT_SHIFT); }

#define TX_LOADTLUT0_TLUT_BASE_SIZE 24
#define TX_LOADTLUT0_TLUT_BASE_SHIFT 0
#define TX_LOADTLUT0_TLUT_BASE_MASK 0x00ffffff
#define TX_LOADTLUT0_GET_TLUT_BASE(tx_loadtlut0) ((((unsigned long)(tx_loadtlut0)) & TX_LOADTLUT0_TLUT_BASE_MASK) >> TX_LOADTLUT0_TLUT_BASE_SHIFT)
#define TX_LOADTLUT0_SET_TLUT_BASE(tx_loadtlut0, tlut_base)                                                                                          \
    {                                                                                                                                                \
        tx_loadtlut0 =                                                                                                                               \
            (((unsigned long)(tx_loadtlut0)) & ~TX_LOADTLUT0_TLUT_BASE_MASK) | (((unsigned long)(tlut_base)) << TX_LOADTLUT0_TLUT_BASE_SHIFT);       \
    }

#define TX_LOADTLUT0_RID_SIZE 8
#define TX_LOADTLUT0_RID_SHIFT 24
#define TX_LOADTLUT0_RID_MASK 0xff000000
#define TX_LOADTLUT0_GET_RID(tx_loadtlut0) ((((unsigned long)(tx_loadtlut0)) & TX_LOADTLUT0_RID_MASK) >> TX_LOADTLUT0_RID_SHIFT)
#define TX_LOADTLUT0_SET_RID(tx_loadtlut0, rid)                                                                                                      \
    { tx_loadtlut0 = (((unsigned long)(tx_loadtlut0)) & ~TX_LOADTLUT0_RID_MASK) | (((unsigned long)(rid)) << TX_LOADTLUT0_RID_SHIFT); }

#define TX_SETTLUT_TMEM_OFFSET_SIZE 10
#define TX_SETTLUT_TMEM_OFFSET_SHIFT 0
#define TX_SETTLUT_TMEM_OFFSET_MASK 0x000003ff
#define TX_SETTLUT_GET_TMEM_OFFSET(tx_settlut) ((((unsigned long)(tx_settlut)) & TX_SETTLUT_TMEM_OFFSET_MASK) >> TX_SETTLUT_TMEM_OFFSET_SHIFT)
#define TX_SETTLUT_SET_TMEM_OFFSET(tx_settlut, tmem_offset)                                                                                          \
    {                                                                                                                                                \
        tx_settlut =                                                                                                                                 \
            (((unsigned long)(tx_settlut)) & ~TX_SETTLUT_TMEM_OFFSET_MASK) | (((unsigned long)(tmem_offset)) << TX_SETTLUT_TMEM_OFFSET_SHIFT);       \
    }

#define TX_LOADTLUT1_TMEM_OFFSET_SIZE 10
#define TX_LOADTLUT1_TMEM_OFFSET_SHIFT 0
#define TX_LOADTLUT1_TMEM_OFFSET_MASK 0x000003ff
#define TX_LOADTLUT1_GET_TMEM_OFFSET(tx_loadtlut1)                                                                                                   \
    ((((unsigned long)(tx_loadtlut1)) & TX_LOADTLUT1_TMEM_OFFSET_MASK) >> TX_LOADTLUT1_TMEM_OFFSET_SHIFT)
#define TX_LOADTLUT1_SET_TMEM_OFFSET(tx_loadtlut1, tmem_offset)                                                                                      \
    {                                                                                                                                                \
        tx_loadtlut1 =                                                                                                                               \
            (((unsigned long)(tx_loadtlut1)) & ~TX_LOADTLUT1_TMEM_OFFSET_MASK) | (((unsigned long)(tmem_offset)) << TX_LOADTLUT1_TMEM_OFFSET_SHIFT); \
    }
#define TX_SETIMAGE1_TMEM_OFFSET_SIZE 15
#define TX_SETIMAGE1_TMEM_OFFSET_SHIFT 0
#define TX_SETIMAGE1_TMEM_OFFSET_MASK 0x00007fff
#define TX_SETIMAGE1_GET_TMEM_OFFSET(tx_setimage1)                                                                                                   \
    ((((unsigned long)(tx_setimage1)) & TX_SETIMAGE1_TMEM_OFFSET_MASK) >> TX_SETIMAGE1_TMEM_OFFSET_SHIFT)
#define TX_SETIMAGE1_SET_TMEM_OFFSET(tx_setimage1, tmem_offset)                                                                                      \
    {                                                                                                                                                \
        tx_setimage1 =                                                                                                                               \
            (((unsigned long)(tx_setimage1)) & ~TX_SETIMAGE1_TMEM_OFFSET_MASK) | (((unsigned long)(tmem_offset)) << TX_SETIMAGE1_TMEM_OFFSET_SHIFT); \
    }

#define TX_SETIMAGE1_CACHE_WIDTH_SIZE 3
#define TX_SETIMAGE1_CACHE_WIDTH_SHIFT 15
#define TX_SETIMAGE1_CACHE_WIDTH_MASK 0x00038000
#define TX_SETIMAGE1_GET_CACHE_WIDTH(tx_setimage1)                                                                                                   \
    ((((unsigned long)(tx_setimage1)) & TX_SETIMAGE1_CACHE_WIDTH_MASK) >> TX_SETIMAGE1_CACHE_WIDTH_SHIFT)
#define TX_SETIMAGE1_SET_CACHE_WIDTH(tx_setimage1, cache_width)                                                                                      \
    {                                                                                                                                                \
        tx_setimage1 =                                                                                                                               \
            (((unsigned long)(tx_setimage1)) & ~TX_SETIMAGE1_CACHE_WIDTH_MASK) | (((unsigned long)(cache_width)) << TX_SETIMAGE1_CACHE_WIDTH_SHIFT); \
    }

#define TX_SETIMAGE1_CACHE_HEIGHT_SIZE 3
#define TX_SETIMAGE1_CACHE_HEIGHT_SHIFT 18
#define TX_SETIMAGE1_CACHE_HEIGHT_MASK 0x001c0000
#define TX_SETIMAGE1_GET_CACHE_HEIGHT(tx_setimage1)                                                                                                  \
    ((((unsigned long)(tx_setimage1)) & TX_SETIMAGE1_CACHE_HEIGHT_MASK) >> TX_SETIMAGE1_CACHE_HEIGHT_SHIFT)
#define TX_SETIMAGE1_SET_CACHE_HEIGHT(tx_setimage1, cache_height)                                                                                    \
    {                                                                                                                                                \
        tx_setimage1 = (((unsigned long)(tx_setimage1)) & ~TX_SETIMAGE1_CACHE_HEIGHT_MASK) |                                                         \
                       (((unsigned long)(cache_height)) << TX_SETIMAGE1_CACHE_HEIGHT_SHIFT);                                                         \
    }

#define TX_SETIMAGE1_IMAGE_TYPE_SIZE 1
#define TX_SETIMAGE1_IMAGE_TYPE_SHIFT 21
#define TX_SETIMAGE1_IMAGE_TYPE_MASK 0x00200000
#define TX_SETIMAGE1_GET_IMAGE_TYPE(tx_setimage1) ((((unsigned long)(tx_setimage1)) & TX_SETIMAGE1_IMAGE_TYPE_MASK) >> TX_SETIMAGE1_IMAGE_TYPE_SHIFT)
#define TX_SETIMAGE1_SET_IMAGE_TYPE(tx_setimage1, image_type)                                                                                        \
    {                                                                                                                                                \
        tx_setimage1 =                                                                                                                               \
            (((unsigned long)(tx_setimage1)) & ~TX_SETIMAGE1_IMAGE_TYPE_MASK) | (((unsigned long)(image_type)) << TX_SETIMAGE1_IMAGE_TYPE_SHIFT);    \
    }

#define TX_SETIMAGE2_TMEM_OFFSET_SIZE 15
#define TX_SETIMAGE2_TMEM_OFFSET_SHIFT 0
#define TX_SETIMAGE2_TMEM_OFFSET_MASK 0x00007fff
#define TX_SETIMAGE2_GET_TMEM_OFFSET(tx_setimage2)                                                                                                   \
    ((((unsigned long)(tx_setimage2)) & TX_SETIMAGE2_TMEM_OFFSET_MASK) >> TX_SETIMAGE2_TMEM_OFFSET_SHIFT)
#define TX_SETIMAGE2_SET_TMEM_OFFSET(tx_setimage2, tmem_offset)                                                                                      \
    {                                                                                                                                                \
        tx_setimage2 =                                                                                                                               \
            (((unsigned long)(tx_setimage2)) & ~TX_SETIMAGE2_TMEM_OFFSET_MASK) | (((unsigned long)(tmem_offset)) << TX_SETIMAGE2_TMEM_OFFSET_SHIFT); \
    }
#define TX_SETIMAGE2_CACHE_WIDTH_SIZE 3
#define TX_SETIMAGE2_CACHE_WIDTH_SHIFT 15
#define TX_SETIMAGE2_CACHE_WIDTH_MASK 0x00038000
#define TX_SETIMAGE2_GET_CACHE_WIDTH(tx_setimage2)                                                                                                   \
    ((((unsigned long)(tx_setimage2)) & TX_SETIMAGE2_CACHE_WIDTH_MASK) >> TX_SETIMAGE2_CACHE_WIDTH_SHIFT)
#define TX_SETIMAGE2_SET_CACHE_WIDTH(tx_setimage2, cache_width)                                                                                      \
    {                                                                                                                                                \
        tx_setimage2 =                                                                                                                               \
            (((unsigned long)(tx_setimage2)) & ~TX_SETIMAGE2_CACHE_WIDTH_MASK) | (((unsigned long)(cache_width)) << TX_SETIMAGE2_CACHE_WIDTH_SHIFT); \
    }
#define TX_SETIMAGE2_CACHE_HEIGHT_SIZE 3
#define TX_SETIMAGE2_CACHE_HEIGHT_SHIFT 18
#define TX_SETIMAGE2_CACHE_HEIGHT_MASK 0x001c0000
#define TX_SETIMAGE2_GET_CACHE_HEIGHT(tx_setimage2)                                                                                                  \
    ((((unsigned long)(tx_setimage2)) & TX_SETIMAGE2_CACHE_HEIGHT_MASK) >> TX_SETIMAGE2_CACHE_HEIGHT_SHIFT)
#define TX_SETIMAGE2_SET_CACHE_HEIGHT(tx_setimage2, cache_height)                                                                                    \
    {                                                                                                                                                \
        tx_setimage2 = (((unsigned long)(tx_setimage2)) & ~TX_SETIMAGE2_CACHE_HEIGHT_MASK) |                                                         \
                       (((unsigned long)(cache_height)) << TX_SETIMAGE2_CACHE_HEIGHT_SHIFT);                                                         \
    }

#define TX_LOADTLUT1_COUNT_SIZE 11
#define TX_LOADTLUT1_COUNT_SHIFT 10
#define TX_LOADTLUT1_COUNT_MASK 0x001ffc00
#define TX_LOADTLUT1_GET_COUNT(tx_loadtlut1) ((((unsigned long)(tx_loadtlut1)) & TX_LOADTLUT1_COUNT_MASK) >> TX_LOADTLUT1_COUNT_SHIFT)
#define TX_LOADTLUT1_SET_COUNT(tx_loadtlut1, count)                                                                                                  \
    { tx_loadtlut1 = (((unsigned long)(tx_loadtlut1)) & ~TX_LOADTLUT1_COUNT_MASK) | (((unsigned long)(count)) << TX_LOADTLUT1_COUNT_SHIFT); }

#define TX_LOADTLUT1_RID_SIZE 8
#define TX_LOADTLUT1_RID_SHIFT 24
#define TX_LOADTLUT1_RID_MASK 0xff000000
#define TX_LOADTLUT1_GET_RID(tx_loadtlut1) ((((unsigned long)(tx_loadtlut1)) & TX_LOADTLUT1_RID_MASK) >> TX_LOADTLUT1_RID_SHIFT)
#define TX_LOADTLUT1_SET_RID(tx_loadtlut1, rid)                                                                                                      \
    { tx_loadtlut1 = (((unsigned long)(tx_loadtlut1)) & ~TX_LOADTLUT1_RID_MASK) | (((unsigned long)(rid)) << TX_LOADTLUT1_RID_SHIFT); }

#define TX_INVALTAGS_TMEM_OFFSET_SIZE 9
#define TX_INVALTAGS_TMEM_OFFSET_SHIFT 0
#define TX_INVALTAGS_TMEM_OFFSET_MASK 0x000001ff
#define TX_INVALTAGS_GET_TMEM_OFFSET(tx_invaltags)                                                                                                   \
    ((((unsigned long)(tx_invaltags)) & TX_INVALTAGS_TMEM_OFFSET_MASK) >> TX_INVALTAGS_TMEM_OFFSET_SHIFT)
#define TX_INVALTAGS_SET_TMEM_OFFSET(tx_invaltags, tmem_offset)                                                                                      \
    {                                                                                                                                                \
        tx_invaltags =                                                                                                                               \
            (((unsigned long)(tx_invaltags)) & ~TX_INVALTAGS_TMEM_OFFSET_MASK) | (((unsigned long)(tmem_offset)) << TX_INVALTAGS_TMEM_OFFSET_SHIFT); \
    }
#define TX_INVALTAGS_COUNT_SIZE 4
#define TX_INVALTAGS_COUNT_SHIFT 9
#define TX_INVALTAGS_COUNT_MASK 0x00001e00
#define TX_INVALTAGS_GET_COUNT(tx_invaltags) ((((unsigned long)(tx_invaltags)) & TX_INVALTAGS_COUNT_MASK) >> TX_INVALTAGS_COUNT_SHIFT)
#define TX_INVALTAGS_SET_COUNT(tx_invaltags, count)                                                                                                  \
    { tx_invaltags = (((unsigned long)(tx_invaltags)) & ~TX_INVALTAGS_COUNT_MASK) | (((unsigned long)(count)) << TX_INVALTAGS_COUNT_SHIFT); }
#define TX_INVALTAGS_PAD0_SIZE 11
#define TX_INVALTAGS_PAD0_SHIFT 13
#define TX_INVALTAGS_PAD0_MASK 0x00ffe000
#define TX_INVALTAGS_GET_PAD0(tx_invaltags) ((((unsigned long)(tx_invaltags)) & TX_INVALTAGS_PAD0_MASK) >> TX_INVALTAGS_PAD0_SHIFT)
#define TX_INVALTAGS_SET_PAD0(tx_invaltags, pad0)                                                                                                    \
    { tx_invaltags = (((unsigned long)(tx_invaltags)) & ~TX_INVALTAGS_PAD0_MASK) | (((unsigned long)(pad0)) << TX_INVALTAGS_PAD0_SHIFT); }
#define TX_INVALTAGS_RID_SIZE 8
#define TX_INVALTAGS_RID_SHIFT 24
#define TX_INVALTAGS_RID_MASK 0xff000000
#define TX_INVALTAGS_GET_RID(tx_invaltags) ((((unsigned long)(tx_invaltags)) & TX_INVALTAGS_RID_MASK) >> TX_INVALTAGS_RID_SHIFT)
#define TX_INVALTAGS_SET_RID(tx_invaltags, rid)                                                                                                      \
    { tx_invaltags = (((unsigned long)(tx_invaltags)) & ~TX_INVALTAGS_RID_MASK) | (((unsigned long)(rid)) << TX_INVALTAGS_RID_SHIFT); }
#define TX_INVALTAGS_TOTAL_SIZE 32
#define TX_INVALTAGS(tmem_offset, count, rid)                                                                                                        \
    ((((unsigned long)(tmem_offset)) << TX_INVALTAGS_TMEM_OFFSET_SHIFT) | (((unsigned long)(count)) << TX_INVALTAGS_COUNT_SHIFT) |                   \
     (((unsigned long)(rid)) << TX_INVALTAGS_RID_SHIFT))

#define TX_SETIMAGE1_TOTAL_SIZE 32
#define TX_SETIMAGE1(tmem_offset, cache_width, cache_height, image_type, rid)                                                                        \
    ((((unsigned long)(tmem_offset)) << TX_SETIMAGE1_TMEM_OFFSET_SHIFT) | (((unsigned long)(cache_width)) << TX_SETIMAGE1_CACHE_WIDTH_SHIFT) |       \
     (((unsigned long)(cache_height)) << TX_SETIMAGE1_CACHE_HEIGHT_SHIFT) | (((unsigned long)(image_type)) << TX_SETIMAGE1_IMAGE_TYPE_SHIFT) |       \
     (((unsigned long)(rid)) << TX_SETIMAGE1_RID_SHIFT))
#define TX_SETIMAGE2_TOTAL_SIZE 32
#define TX_SETIMAGE2(tmem_offset, cache_width, cache_height, rid)                                                                                    \
    ((((unsigned long)(tmem_offset)) << TX_SETIMAGE2_TMEM_OFFSET_SHIFT) | (((unsigned long)(cache_width)) << TX_SETIMAGE2_CACHE_WIDTH_SHIFT) |       \
     (((unsigned long)(cache_height)) << TX_SETIMAGE2_CACHE_HEIGHT_SHIFT) | (((unsigned long)(rid)) << TX_SETIMAGE2_RID_SHIFT))

#endif  // TX_REG_H
