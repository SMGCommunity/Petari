#pragma once

#define EXI_0CPR_IDX	            0x00000000

#define EXI_0CPR_EXIINTMSK_MASK	    0x00000001
#define EXI_0CPR_EXIINT_MASK	    0x00000002
#define EXI_0CPR_TCINTMSK_MASK	    0x00000004
#define EXI_0CPR_TCINT_MASK	        0x00000008
#define EXI_0CPR_CLK_MASK	        0x00000070
#define EXI_0CPR_CS0B_MASK	        0x00000080
#define EXI_0CPR_CS1B_MASK	        0x00000100
#define EXI_0CPR_CS2B_MASK	        0x00000200
#define EXI_0CPR_EXTINTMSK_MASK	    0x00000400
#define EXI_0CPR_EXTINT_MASK	    0x00000800

typedef struct {
    unsigned long tcint : 1;
    unsigned long tcintmsk : 1;
    unsigned long comerr : 1;
    unsigned long rdstint : 1;
    unsigned long rdstintmsk : 1;
    unsigned long pad2 : 4;
    unsigned long outlngth : 7;
    unsigned long pad1 : 1;
    unsigned long inlngth : 7;
    unsigned long pad0 : 5;
    unsigned long channel : 2;
    unsigned long tstart : 1;
} si_comcsr_t;

typedef union {
    unsigned long val;
    si_comcsr_t f;
} si_comcsr_u;