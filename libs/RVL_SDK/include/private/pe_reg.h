#ifndef PE_REG_H
#define PE_REG_H

#define PE_REFRESH_RID_SIZE     8
#define PE_REFRESH_RID_SHIFT    24
#define PE_REFRESH_RID_MASK     0xff000000
#define PE_REFRESH_GET_RID(pe_refresh) \
        ((((unsigned long)(pe_refresh)) & PE_REFRESH_RID_MASK) >> PE_REFRESH_RID_SHIFT)

#define PE_REFRESH_ENABLE_SIZE  1
#define PE_REFRESH_ENABLE_SHIFT 9
#define PE_REFRESH_ENABLE_MASK  0x00000200
#define PE_REFRESH_GET_ENABLE(pe_refresh) \
        ((((unsigned long)(pe_refresh)) & PE_REFRESH_ENABLE_MASK) >> PE_REFRESH_ENABLE_SHIFT)

#define PE_REFRESH_INTERVAL_SIZE        9
#define PE_REFRESH_INTERVAL_SHIFT       0
#define PE_REFRESH_INTERVAL_MASK        0x000001ff
#define PE_REFRESH_GET_INTERVAL(pe_refresh) \
        ((((unsigned long)(pe_refresh)) & PE_REFRESH_INTERVAL_MASK) >> PE_REFRESH_INTERVAL_SHIFT)

#define PE_REFRESH_TOTAL_SIZE   32
#define PE_REFRESH(interval, enable, rid) \
        ((((unsigned long)(interval)) << PE_REFRESH_INTERVAL_SHIFT) | \
         (((unsigned long)(enable)) << PE_REFRESH_ENABLE_SHIFT) | \
         (((unsigned long)(rid)) << PE_REFRESH_RID_SHIFT))

#define PE_CHICKEN_PIWR_SIZE	1
#define PE_CHICKEN_PIWR_SHIFT	0
#define PE_CHICKEN_PIWR_MASK	0x00000001
#define PE_CHICKEN_GET_PIWR(pe_chicken) \
	((((unsigned long)(pe_chicken)) & PE_CHICKEN_PIWR_MASK) >> PE_CHICKEN_PIWR_SHIFT)

#define PE_CHICKEN_TXCPY_FMT_SIZE	1
#define PE_CHICKEN_TXCPY_FMT_SHIFT	1
#define PE_CHICKEN_TXCPY_FMT_MASK	0x00000002
#define PE_CHICKEN_GET_TXCPY_FMT(pe_chicken) \
    ((((unsigned long)(pe_chicken)) & PE_CHICKEN_TXCPY_FMT_MASK) >> PE_CHICKEN_TXCPY_FMT_SHIFT)

#define PE_CHICKEN_TXCPY_CCV_SIZE	1
#define PE_CHICKEN_TXCPY_CCV_SHIFT	2
#define PE_CHICKEN_TXCPY_CCV_MASK	0x00000004
#define PE_CHICKEN_GET_TXCPY_CCV(pe_chicken) \
    ((((unsigned long)(pe_chicken)) & PE_CHICKEN_TXCPY_CCV_MASK) >> PE_CHICKEN_TXCPY_CCV_SHIFT)

#define PE_CHICKEN_BLENDOP_SIZE	1
#define PE_CHICKEN_BLENDOP_SHIFT	3
#define PE_CHICKEN_BLENDOP_MASK	0x00000008
#define PE_CHICKEN_GET_BLENDOP(pe_chicken) \
	((((unsigned long)(pe_chicken)) & PE_CHICKEN_BLENDOP_MASK) >> PE_CHICKEN_BLENDOP_SHIFT)

#define PE_CHICKEN_RID_SIZE	8
#define PE_CHICKEN_RID_SHIFT	24
#define PE_CHICKEN_RID_MASK	0xff000000
#define PE_CHICKEN_GET_RID(pe_chicken) \
    ((((unsigned long)(pe_chicken)) & PE_CHICKEN_RID_MASK) >> PE_CHICKEN_RID_SHIFT)

#define PE_CMODE0_RID_SIZE      8
#define PE_CMODE0_RID_SHIFT     24
#define PE_CMODE0_RID_MASK      0xff000000
#define PE_CMODE0_GET_RID(pe_cmode0) \
        ((((unsigned long)(pe_cmode0)) & PE_CMODE0_RID_MASK) >> PE_CMODE0_RID_SHIFT)

#define PE_CMODE1_RID_SIZE      8
#define PE_CMODE1_RID_SHIFT     24
#define PE_CMODE1_RID_MASK      0xff000000
#define PE_CMODE1_GET_RID(pe_cmode1) \
        ((((unsigned long)(pe_cmode1)) & PE_CMODE1_RID_MASK) >> PE_CMODE1_RID_SHIFT)

#define PE_ZMODE_RID_SIZE       8
#define PE_ZMODE_RID_SHIFT      24
#define PE_ZMODE_RID_MASK       0xff000000
#define PE_ZMODE_GET_RID(pe_zmode) \
        ((((unsigned long)(pe_zmode)) & PE_ZMODE_RID_MASK) >> PE_ZMODE_RID_SHIFT)

#define PE_CONTROL_RID_SIZE     8
#define PE_CONTROL_RID_SHIFT    24
#define PE_CONTROL_RID_MASK     0xff000000
#define PE_CONTROL_GET_RID(pe_control) \
        ((((unsigned long)(pe_control)) & PE_CONTROL_RID_MASK) >> PE_CONTROL_RID_SHIFT)

#define PE_COPY_CMD_GAMMA_SIZE  2
#define PE_COPY_CMD_GAMMA_SHIFT 7
#define PE_COPY_CMD_GAMMA_MASK  0x00000180
#define PE_COPY_CMD_GET_GAMMA(pe_copy_cmd) \
        ((((unsigned long)(pe_copy_cmd)) & PE_COPY_CMD_GAMMA_MASK) >> PE_COPY_CMD_GAMMA_SHIFT)

#endif // PE_REG_H
