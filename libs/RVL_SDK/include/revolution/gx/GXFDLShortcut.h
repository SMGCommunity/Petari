#ifndef GXFDLSHORTCUT_h
#define GXFDLSHORTCUT_H

#if __MWERKS__
#define FAST_GPFLAGSET(regOrg, newFlag, regName) \
do {                                                                                \
    (regOrg) = (u32) __rlwimi((int) (regOrg),                                       \
                              (int) (newFlag),                                      \
                              (regName ## _SHIFT),                                  \
                              (32-(regName ## _SHIFT)-(regName ## _SIZE)),          \
                              (31-(regName ## _SHIFT)));                            \
} while (0)
#else
#define FAST_GPFLAGSET(regOrg, newFlag, regName)
#endif

#define CP_VCD_REG_HI_TEXALL_MASK   ((1UL<<CP_VCD_REG_HI_TOTAL_SIZE)-1)
#define CP_VCD_REG_HI_TEXALL_SHIFT  CP_VCD_REG_HI_TEX0_SHIFT
#define CP_VCD_REG_LO_COLALL_MASK   (CP_VCD_REG_LO_COL0_MASK|CP_VCD_REG_LO_COL1_MASK)
#define CP_VCD_REG_LO_COLALL_SHIFT  CP_VCD_REG_LO_COL0_SHIFT

#define TEV_REGISTERL_R8_SIZE   TEV_KREGISTERL_R_SIZE
#define TEV_REGISTERL_R8_SHIFT  TEV_KREGISTERL_R_SHIFT
#define TEV_REGISTERL_R8_MASK   TEV_KREGISTERL_R_MASK
#define SC_TEV_REGISTERL_SET_R8(tev_registerl, r8) \
    FAST_GPFLAGSET(tev_registerl, r8, TEV_REGISTERL_R8)

#define TEV_REGISTERL_A8_SIZE   TEV_KREGISTERL_A_SIZE
#define TEV_REGISTERL_A8_SHIFT  TEV_KREGISTERL_A_SHIFT
#define TEV_REGISTERL_A8_MASK   TEV_KREGISTERL_A_MASK
#define SC_TEV_REGISTERL_SET_A8(tev_registerl, a8) \
    FAST_GPFLAGSET(tev_registerl, a8, TEV_REGISTERL_A8)

#define TEV_REGISTERH_B8_SIZE   TEV_KREGISTERH_B_SIZE
#define TEV_REGISTERH_B8_SHIFT  TEV_KREGISTERH_B_SHIFT
#define TEV_REGISTERH_B8_MASK   TEV_KREGISTERH_B_MASK
#define SC_TEV_REGISTERH_SET_B8(tev_registerh, b8) \
    FAST_GPFLAGSET(tev_registerh, b8, TEV_REGISTERH_B8)

#define TEV_REGISTERH_G8_SIZE   TEV_KREGISTERH_G_SIZE
#define TEV_REGISTERH_G8_SHIFT  TEV_KREGISTERH_G_SHIFT
#define TEV_REGISTERH_G8_MASK   TEV_KREGISTERH_G_MASK
#define SC_TEV_REGISTERH_SET_G8(tev_registerh, g8) \
    FAST_GPFLAGSET(tev_registerh, g8, TEV_REGISTERH_G8)

#define TEV_REGISTERL_R8_SIZE   TEV_KREGISTERL_R_SIZE
#define TEV_REGISTERL_R8_SHIFT  TEV_KREGISTERL_R_SHIFT
#define TEV_REGISTERL_R8_MASK   TEV_KREGISTERL_R_MASK
#define SC_TEV_REGISTERL_SET_R8(tev_registerl, r8) \
    FAST_GPFLAGSET(tev_registerl, r8, TEV_REGISTERL_R8)

#define TEV_REGISTERL_A8_SIZE   TEV_KREGISTERL_A_SIZE
#define TEV_REGISTERL_A8_SHIFT  TEV_KREGISTERL_A_SHIFT
#define TEV_REGISTERL_A8_MASK   TEV_KREGISTERL_A_MASK
#define SC_TEV_REGISTERL_SET_A8(tev_registerl, a8) \
    FAST_GPFLAGSET(tev_registerl, a8, TEV_REGISTERL_A8)

#define TEV_REGISTERH_B8_SIZE   TEV_KREGISTERH_B_SIZE
#define TEV_REGISTERH_B8_SHIFT  TEV_KREGISTERH_B_SHIFT
#define TEV_REGISTERH_B8_MASK   TEV_KREGISTERH_B_MASK
#define SC_TEV_REGISTERH_SET_B8(tev_registerh, b8) \
    FAST_GPFLAGSET(tev_registerh, b8, TEV_REGISTERH_B8)

#define TEV_REGISTERH_G8_SIZE   TEV_KREGISTERH_G_SIZE
#define TEV_REGISTERH_G8_SHIFT  TEV_KREGISTERH_G_SHIFT
#define TEV_REGISTERH_G8_MASK   TEV_KREGISTERH_G_MASK
#define SC_TEV_REGISTERH_SET_G8(tev_registerh, g8) \
    FAST_GPFLAGSET(tev_registerh, g8, TEV_REGISTERH_G8)

#endif // GXFDLSHORTCUT_H
