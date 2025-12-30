#ifndef GDTEXTURE_H
#define GDTEXTURE_H

#define TX_SETMODE0_I0_ID 0x80
#define TX_SETMODE0_I1_ID 0x81
#define TX_SETMODE0_I2_ID 0x82
#define TX_SETMODE0_I3_ID 0x83
#define TX_SETMODE0_I4_ID 0xa0
#define TX_SETMODE0_I5_ID 0xa1
#define TX_SETMODE0_I6_ID 0xa2
#define TX_SETMODE0_I7_ID 0xa3

#define TX_SETMODE1_I0_ID 0x84
#define TX_SETMODE1_I1_ID 0x85
#define TX_SETMODE1_I2_ID 0x86
#define TX_SETMODE1_I3_ID 0x87
#define TX_SETMODE1_I4_ID 0xa4
#define TX_SETMODE1_I5_ID 0xa5
#define TX_SETMODE1_I6_ID 0xa6
#define TX_SETMODE1_I7_ID 0xa7

#define TX_SETIMAGE0_I0_ID 0x88
#define TX_SETIMAGE0_I1_ID 0x89
#define TX_SETIMAGE0_I2_ID 0x8a
#define TX_SETIMAGE0_I3_ID 0x8b
#define TX_SETIMAGE0_I4_ID 0xa8
#define TX_SETIMAGE0_I5_ID 0xa9
#define TX_SETIMAGE0_I6_ID 0xaa
#define TX_SETIMAGE0_I7_ID 0xab

#define TX_SETIMAGE3_I0_ID 0x94
#define TX_SETIMAGE3_I1_ID 0x95
#define TX_SETIMAGE3_I2_ID 0x96
#define TX_SETIMAGE3_I3_ID 0x97
#define TX_SETIMAGE3_I4_ID 0xb4
#define TX_SETIMAGE3_I5_ID 0xb5
#define TX_SETIMAGE3_I6_ID 0xb6
#define TX_SETIMAGE3_I7_ID 0xb7

#define TX_SETMODE0_WRAP_S_SHIFT 0
#define TX_SETMODE0_WRAP_T_SHIFT 2
#define TX_SETMODE0_MAG_FILTER_SHIFT 4
#define TX_SETMODE0_MIN_FILTER_SHIFT 5
#define TX_SETMODE0_DIAGLOD_SHIFT 8
#define TX_SETMODE0_LODBIAS_SHIFT 9
#define TX_SETMODE0_MAXANISO_SHIFT 19
#define TX_SETMODE0_LODCLAMP_SHIFT 21
#define TX_SETMODE0_RID_SHIFT 24
#define TX_SETMODE0(wrap_s, wrap_t, mag_filter, min_filter, diaglod, lodbias, maxaniso, lodclamp, rid)                                               \
    ((((u32)(wrap_s)) << TX_SETMODE0_WRAP_S_SHIFT) | (((u32)(wrap_t)) << TX_SETMODE0_WRAP_T_SHIFT) |                                                 \
     (((u32)(mag_filter)) << TX_SETMODE0_MAG_FILTER_SHIFT) | (((u32)(min_filter)) << TX_SETMODE0_MIN_FILTER_SHIFT) |                                 \
     (((u32)(diaglod)) << TX_SETMODE0_DIAGLOD_SHIFT) | (((u32)(lodbias)) << TX_SETMODE0_LODBIAS_SHIFT) |                                             \
     (((u32)(maxaniso)) << TX_SETMODE0_MAXANISO_SHIFT) | (((u32)(lodclamp)) << TX_SETMODE0_LODCLAMP_SHIFT) |                                         \
     (((u32)(rid)) << TX_SETMODE0_RID_SHIFT))

#define TX_SETMODE1_MINLOD_SHIFT 0
#define TX_SETMODE1_MAXLOD_SHIFT 8
#define TX_SETMODE1_RID_SHIFT 24
#define TX_SETMODE1(minlod, maxlod, rid)                                                                                                             \
    ((((u32)(minlod)) << TX_SETMODE1_MINLOD_SHIFT) | (((u32)(maxlod)) << TX_SETMODE1_MAXLOD_SHIFT) | (((u32)(rid)) << TX_SETMODE1_RID_SHIFT))

#define TX_SETIMAGE0_WIDTH_SHIFT 0
#define TX_SETIMAGE0_HEIGHT_SHIFT 10
#define TX_SETIMAGE0_FORMAT_SHIFT 20
#define TX_SETIMAGE0_RID_SHIFT 24
#define TX_SETIMAGE0(width, height, format, rid)                                                                                                     \
    ((((u32)(width)) << TX_SETIMAGE0_WIDTH_SHIFT) | (((u32)(height)) << TX_SETIMAGE0_HEIGHT_SHIFT) |                                                 \
     (((u32)(format)) << TX_SETIMAGE0_FORMAT_SHIFT) | (((u32)(rid)) << TX_SETIMAGE0_RID_SHIFT))

#define TX_SETIMAGE3_IMAGE_BASE_SHIFT 0
#define TX_SETIMAGE3_RID_SHIFT 24
#define TX_SETIMAGE3(image_base, rid) ((((u32)(image_base)) << TX_SETIMAGE3_IMAGE_BASE_SHIFT) | (((u32)(rid)) << TX_SETIMAGE3_RID_SHIFT))

#endif  // GDTEXTURE_H
