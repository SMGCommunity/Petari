#ifndef GXFDLSHORTCUT_h
#define GXFDLSHORTCUT_H

#if __MWERKS__
#define FAST_GPFLAGSET(regOrg, newFlag, regName) \
do {                                                                                \
    FDL_ASSERT(!((newFlag) & ~((1 << (regName ## _SIZE))-1)));                      \
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


#endif // GXFDLSHORTCUT_H
