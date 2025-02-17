#include <revolution/gx/GXFDLShortcut.h>

#define SC_CP_OPCODE_SET_INDEX(cp_opcode,index) \
	FAST_GPFLAGSET(cp_opcode,index,CP_OPCODE_INDEX)

#define SC_CP_OPCODE_SET_CMD(cp_opcode,cmd) \
	FAST_GPFLAGSET(cp_opcode,cmd,CP_OPCODE_CMD)

#define SC_CP_STREAM_REG_SET_INDEX(cp_stream_reg,index) \
	FAST_GPFLAGSET(cp_stream_reg,index,CP_STREAM_REG_INDEX)

#define SC_CP_STREAM_REG_SET_ADDR(cp_stream_reg,addr) \
	FAST_GPFLAGSET(cp_stream_reg,addr,CP_STREAM_REG_ADDR)

#define SC_CP_REG_STATUS_SET_OVFL(cp_reg_status,ovfl) \
	FAST_GPFLAGSET(cp_reg_status,ovfl,CP_REG_STATUS_OVFL)

#define SC_CP_REG_STATUS_SET_UNFL(cp_reg_status,unfl) \
	FAST_GPFLAGSET(cp_reg_status,unfl,CP_REG_STATUS_UNFL)

#define SC_CP_REG_STATUS_SET_FIFO_RDIDLE(cp_reg_status,fifo_rdidle) \
	FAST_GPFLAGSET(cp_reg_status,fifo_rdidle,CP_REG_STATUS_FIFO_RDIDLE)

#define SC_CP_REG_STATUS_SET_CPIDLE(cp_reg_status,cpidle) \
	FAST_GPFLAGSET(cp_reg_status,cpidle,CP_REG_STATUS_CPIDLE)

#define SC_CP_REG_STATUS_SET_FIFOBRK(cp_reg_status,fifobrk) \
	FAST_GPFLAGSET(cp_reg_status,fifobrk,CP_REG_STATUS_FIFOBRK)

#define SC_CP_REG_ENABLE_SET_FIFORD(cp_reg_enable,fiford) \
	FAST_GPFLAGSET(cp_reg_enable,fiford,CP_REG_ENABLE_FIFORD)

#define SC_CP_REG_ENABLE_SET_FIFOBRK(cp_reg_enable,fifobrk) \
	FAST_GPFLAGSET(cp_reg_enable,fifobrk,CP_REG_ENABLE_FIFOBRK)

#define SC_CP_REG_ENABLE_SET_OVFLINT(cp_reg_enable,ovflint) \
	FAST_GPFLAGSET(cp_reg_enable,ovflint,CP_REG_ENABLE_OVFLINT)

#define SC_CP_REG_ENABLE_SET_UNFLINT(cp_reg_enable,unflint) \
	FAST_GPFLAGSET(cp_reg_enable,unflint,CP_REG_ENABLE_UNFLINT)

#define SC_CP_REG_ENABLE_SET_WRPTRINC(cp_reg_enable,wrptrinc) \
	FAST_GPFLAGSET(cp_reg_enable,wrptrinc,CP_REG_ENABLE_WRPTRINC)

#define SC_CP_REG_ENABLE_SET_FIFOBRKINT(cp_reg_enable,fifobrkint) \
	FAST_GPFLAGSET(cp_reg_enable,fifobrkint,CP_REG_ENABLE_FIFOBRKINT)

#define SC_CP_REG_CLR_SET_OVFLINT(cp_reg_clr,ovflint) \
	FAST_GPFLAGSET(cp_reg_clr,ovflint,CP_REG_CLR_OVFLINT)

#define SC_CP_REG_CLR_SET_UNFLINT(cp_reg_clr,unflint) \
	FAST_GPFLAGSET(cp_reg_clr,unflint,CP_REG_CLR_UNFLINT)

#define SC_CP_REG_CLR_SET_PERFCNT(cp_reg_clr,perfcnt) \
	FAST_GPFLAGSET(cp_reg_clr,perfcnt,CP_REG_CLR_PERFCNT)

#define SC_CP_REG_MEMPERFSEL_SET_PERFSEL(cp_reg_memperfsel,perfsel) \
	FAST_GPFLAGSET(cp_reg_memperfsel,perfsel,CP_REG_MEMPERFSEL_PERFSEL)

#define SC_CP_REG_FIFO_BASEL_SET_PAD0(cp_reg_fifo_basel,pad0) \
	FAST_GPFLAGSET(cp_reg_fifo_basel,pad0,CP_REG_FIFO_BASEL_PAD0)

#define SC_CP_REG_FIFO_BASEL_SET_ADDR(cp_reg_fifo_basel,addr) \
	FAST_GPFLAGSET(cp_reg_fifo_basel,addr,CP_REG_FIFO_BASEL_ADDR)

#define SC_CP_REG_FIFO_BASEH_SET_ADDR(cp_reg_fifo_baseh,addr) \
	FAST_GPFLAGSET(cp_reg_fifo_baseh,addr,CP_REG_FIFO_BASEH_ADDR)

#define SC_CP_REG_FIFO_TOPL_SET_PAD0(cp_reg_fifo_topl,pad0) \
	FAST_GPFLAGSET(cp_reg_fifo_topl,pad0,CP_REG_FIFO_TOPL_PAD0)

#define SC_CP_REG_FIFO_TOPL_SET_ADDR(cp_reg_fifo_topl,addr) \
	FAST_GPFLAGSET(cp_reg_fifo_topl,addr,CP_REG_FIFO_TOPL_ADDR)

#define SC_CP_REG_FIFO_TOPH_SET_ADDR(cp_reg_fifo_toph,addr) \
	FAST_GPFLAGSET(cp_reg_fifo_toph,addr,CP_REG_FIFO_TOPH_ADDR)

#define SC_CP_REG_FIFO_HICNTL_SET_PAD0(cp_reg_fifo_hicntl,pad0) \
	FAST_GPFLAGSET(cp_reg_fifo_hicntl,pad0,CP_REG_FIFO_HICNTL_PAD0)

#define SC_CP_REG_FIFO_HICNTL_SET_ADDR(cp_reg_fifo_hicntl,addr) \
	FAST_GPFLAGSET(cp_reg_fifo_hicntl,addr,CP_REG_FIFO_HICNTL_ADDR)

#define SC_CP_REG_FIFO_HICNTH_SET_ADDR(cp_reg_fifo_hicnth,addr) \
	FAST_GPFLAGSET(cp_reg_fifo_hicnth,addr,CP_REG_FIFO_HICNTH_ADDR)

#define SC_CP_REG_FIFO_LOCNTL_SET_PAD0(cp_reg_fifo_locntl,pad0) \
	FAST_GPFLAGSET(cp_reg_fifo_locntl,pad0,CP_REG_FIFO_LOCNTL_PAD0)

#define SC_CP_REG_FIFO_LOCNTL_SET_ADDR(cp_reg_fifo_locntl,addr) \
	FAST_GPFLAGSET(cp_reg_fifo_locntl,addr,CP_REG_FIFO_LOCNTL_ADDR)

#define SC_CP_REG_FIFO_LOCNTH_SET_ADDR(cp_reg_fifo_locnth,addr) \
	FAST_GPFLAGSET(cp_reg_fifo_locnth,addr,CP_REG_FIFO_LOCNTH_ADDR)

#define SC_CP_REG_FIFO_COUNTL_SET_PAD0(cp_reg_fifo_countl,pad0) \
	FAST_GPFLAGSET(cp_reg_fifo_countl,pad0,CP_REG_FIFO_COUNTL_PAD0)

#define SC_CP_REG_FIFO_COUNTL_SET_ADDR(cp_reg_fifo_countl,addr) \
	FAST_GPFLAGSET(cp_reg_fifo_countl,addr,CP_REG_FIFO_COUNTL_ADDR)

#define SC_CP_REG_FIFO_COUNTH_SET_ADDR(cp_reg_fifo_counth,addr) \
	FAST_GPFLAGSET(cp_reg_fifo_counth,addr,CP_REG_FIFO_COUNTH_ADDR)

#define SC_CP_REG_FIFO_WPTRL_SET_PAD0(cp_reg_fifo_wptrl,pad0) \
	FAST_GPFLAGSET(cp_reg_fifo_wptrl,pad0,CP_REG_FIFO_WPTRL_PAD0)

#define SC_CP_REG_FIFO_WPTRL_SET_ADDR(cp_reg_fifo_wptrl,addr) \
	FAST_GPFLAGSET(cp_reg_fifo_wptrl,addr,CP_REG_FIFO_WPTRL_ADDR)

#define SC_CP_REG_FIFO_WPTRH_SET_ADDR(cp_reg_fifo_wptrh,addr) \
	FAST_GPFLAGSET(cp_reg_fifo_wptrh,addr,CP_REG_FIFO_WPTRH_ADDR)

#define SC_CP_REG_FIFO_RPTRL_SET_PAD0(cp_reg_fifo_rptrl,pad0) \
	FAST_GPFLAGSET(cp_reg_fifo_rptrl,pad0,CP_REG_FIFO_RPTRL_PAD0)

#define SC_CP_REG_FIFO_RPTRL_SET_ADDR(cp_reg_fifo_rptrl,addr) \
	FAST_GPFLAGSET(cp_reg_fifo_rptrl,addr,CP_REG_FIFO_RPTRL_ADDR)

#define SC_CP_REG_FIFO_RPTRH_SET_ADDR(cp_reg_fifo_rptrh,addr) \
	FAST_GPFLAGSET(cp_reg_fifo_rptrh,addr,CP_REG_FIFO_RPTRH_ADDR)

#define SC_CP_REG_FIFO_BRKL_SET_PAD0(cp_reg_fifo_brkl,pad0) \
	FAST_GPFLAGSET(cp_reg_fifo_brkl,pad0,CP_REG_FIFO_BRKL_PAD0)

#define SC_CP_REG_FIFO_BRKL_SET_ADDR(cp_reg_fifo_brkl,addr) \
	FAST_GPFLAGSET(cp_reg_fifo_brkl,addr,CP_REG_FIFO_BRKL_ADDR)

#define SC_CP_REG_FIFO_BRKH_SET_ADDR(cp_reg_fifo_brkh,addr) \
	FAST_GPFLAGSET(cp_reg_fifo_brkh,addr,CP_REG_FIFO_BRKH_ADDR)

#define SC_CP_VCD_REG_LO_SET_PMIDX(cp_vcd_reg_lo,pmidx) \
	FAST_GPFLAGSET(cp_vcd_reg_lo,pmidx,CP_VCD_REG_LO_PMIDX)

#define SC_CP_VCD_REG_LO_SET_T0MIDX(cp_vcd_reg_lo,t0midx) \
	FAST_GPFLAGSET(cp_vcd_reg_lo,t0midx,CP_VCD_REG_LO_T0MIDX)

#define SC_CP_VCD_REG_LO_SET_T1MIDX(cp_vcd_reg_lo,t1midx) \
	FAST_GPFLAGSET(cp_vcd_reg_lo,t1midx,CP_VCD_REG_LO_T1MIDX)

#define SC_CP_VCD_REG_LO_SET_T2MIDX(cp_vcd_reg_lo,t2midx) \
	FAST_GPFLAGSET(cp_vcd_reg_lo,t2midx,CP_VCD_REG_LO_T2MIDX)

#define SC_CP_VCD_REG_LO_SET_T3MIDX(cp_vcd_reg_lo,t3midx) \
	FAST_GPFLAGSET(cp_vcd_reg_lo,t3midx,CP_VCD_REG_LO_T3MIDX)

#define SC_CP_VCD_REG_LO_SET_T4MIDX(cp_vcd_reg_lo,t4midx) \
	FAST_GPFLAGSET(cp_vcd_reg_lo,t4midx,CP_VCD_REG_LO_T4MIDX)

#define SC_CP_VCD_REG_LO_SET_T5MIDX(cp_vcd_reg_lo,t5midx) \
	FAST_GPFLAGSET(cp_vcd_reg_lo,t5midx,CP_VCD_REG_LO_T5MIDX)

#define SC_CP_VCD_REG_LO_SET_T6MIDX(cp_vcd_reg_lo,t6midx) \
	FAST_GPFLAGSET(cp_vcd_reg_lo,t6midx,CP_VCD_REG_LO_T6MIDX)

#define SC_CP_VCD_REG_LO_SET_T7MIDX(cp_vcd_reg_lo,t7midx) \
	FAST_GPFLAGSET(cp_vcd_reg_lo,t7midx,CP_VCD_REG_LO_T7MIDX)

#define SC_CP_VCD_REG_LO_SET_POS(cp_vcd_reg_lo,pos) \
	FAST_GPFLAGSET(cp_vcd_reg_lo,pos,CP_VCD_REG_LO_POS)

#define SC_CP_VCD_REG_LO_SET_NRM(cp_vcd_reg_lo,nrm) \
	FAST_GPFLAGSET(cp_vcd_reg_lo,nrm,CP_VCD_REG_LO_NRM)

#define SC_CP_VCD_REG_LO_SET_COL0(cp_vcd_reg_lo,col0) \
	FAST_GPFLAGSET(cp_vcd_reg_lo,col0,CP_VCD_REG_LO_COL0)

#define SC_CP_VCD_REG_LO_SET_COL1(cp_vcd_reg_lo,col1) \
	FAST_GPFLAGSET(cp_vcd_reg_lo,col1,CP_VCD_REG_LO_COL1)

#define SC_CP_VCD_REG_HI_SET_TEX0(cp_vcd_reg_hi,tex0) \
	FAST_GPFLAGSET(cp_vcd_reg_hi,tex0,CP_VCD_REG_HI_TEX0)

#define SC_CP_VCD_REG_HI_SET_TEX1(cp_vcd_reg_hi,tex1) \
	FAST_GPFLAGSET(cp_vcd_reg_hi,tex1,CP_VCD_REG_HI_TEX1)

#define SC_CP_VCD_REG_HI_SET_TEX2(cp_vcd_reg_hi,tex2) \
	FAST_GPFLAGSET(cp_vcd_reg_hi,tex2,CP_VCD_REG_HI_TEX2)

#define SC_CP_VCD_REG_HI_SET_TEX3(cp_vcd_reg_hi,tex3) \
	FAST_GPFLAGSET(cp_vcd_reg_hi,tex3,CP_VCD_REG_HI_TEX3)

#define SC_CP_VCD_REG_HI_SET_TEX4(cp_vcd_reg_hi,tex4) \
	FAST_GPFLAGSET(cp_vcd_reg_hi,tex4,CP_VCD_REG_HI_TEX4)

#define SC_CP_VCD_REG_HI_SET_TEX5(cp_vcd_reg_hi,tex5) \
	FAST_GPFLAGSET(cp_vcd_reg_hi,tex5,CP_VCD_REG_HI_TEX5)

#define SC_CP_VCD_REG_HI_SET_TEX6(cp_vcd_reg_hi,tex6) \
	FAST_GPFLAGSET(cp_vcd_reg_hi,tex6,CP_VCD_REG_HI_TEX6)

#define SC_CP_VCD_REG_HI_SET_TEX7(cp_vcd_reg_hi,tex7) \
	FAST_GPFLAGSET(cp_vcd_reg_hi,tex7,CP_VCD_REG_HI_TEX7)

#define SC_CP_VAT_REG_A_SET_POSCNT(cp_vat_reg_a,posCnt) \
	FAST_GPFLAGSET(cp_vat_reg_a,posCnt,CP_VAT_REG_A_POSCNT)

#define SC_CP_VAT_REG_A_SET_POSFMT(cp_vat_reg_a,posFmt) \
	FAST_GPFLAGSET(cp_vat_reg_a,posFmt,CP_VAT_REG_A_POSFMT)

#define SC_CP_VAT_REG_A_SET_POSSHFT(cp_vat_reg_a,posShft) \
	FAST_GPFLAGSET(cp_vat_reg_a,posShft,CP_VAT_REG_A_POSSHFT)

#define SC_CP_VAT_REG_A_SET_NRMCNT(cp_vat_reg_a,nrmCnt) \
	FAST_GPFLAGSET(cp_vat_reg_a,nrmCnt,CP_VAT_REG_A_NRMCNT)

#define SC_CP_VAT_REG_A_SET_NRMFMT(cp_vat_reg_a,nrmFmt) \
	FAST_GPFLAGSET(cp_vat_reg_a,nrmFmt,CP_VAT_REG_A_NRMFMT)

#define SC_CP_VAT_REG_A_SET_COL0CNT(cp_vat_reg_a,Col0Cnt) \
	FAST_GPFLAGSET(cp_vat_reg_a,Col0Cnt,CP_VAT_REG_A_COL0CNT)

#define SC_CP_VAT_REG_A_SET_COL0FMT(cp_vat_reg_a,Col0Fmt) \
	FAST_GPFLAGSET(cp_vat_reg_a,Col0Fmt,CP_VAT_REG_A_COL0FMT)

#define SC_CP_VAT_REG_A_SET_COL1CNT(cp_vat_reg_a,Col1Cnt) \
	FAST_GPFLAGSET(cp_vat_reg_a,Col1Cnt,CP_VAT_REG_A_COL1CNT)

#define SC_CP_VAT_REG_A_SET_COL1FMT(cp_vat_reg_a,Col1Fmt) \
	FAST_GPFLAGSET(cp_vat_reg_a,Col1Fmt,CP_VAT_REG_A_COL1FMT)

#define SC_CP_VAT_REG_A_SET_TEX0CNT(cp_vat_reg_a,tex0Cnt) \
	FAST_GPFLAGSET(cp_vat_reg_a,tex0Cnt,CP_VAT_REG_A_TEX0CNT)

#define SC_CP_VAT_REG_A_SET_TEX0FMT(cp_vat_reg_a,tex0Fmt) \
	FAST_GPFLAGSET(cp_vat_reg_a,tex0Fmt,CP_VAT_REG_A_TEX0FMT)

#define SC_CP_VAT_REG_A_SET_TEX0SHFT(cp_vat_reg_a,tex0Shft) \
	FAST_GPFLAGSET(cp_vat_reg_a,tex0Shft,CP_VAT_REG_A_TEX0SHFT)

#define SC_CP_VAT_REG_A_SET_BYTEDEQUANT(cp_vat_reg_a,byteDequant) \
	FAST_GPFLAGSET(cp_vat_reg_a,byteDequant,CP_VAT_REG_A_BYTEDEQUANT)

#define SC_CP_VAT_REG_A_SET_NORMALINDEX3(cp_vat_reg_a,normalIndex3) \
	FAST_GPFLAGSET(cp_vat_reg_a,normalIndex3,CP_VAT_REG_A_NORMALINDEX3)

#define SC_CP_VAT_REG_B_SET_TEX1CNT(cp_vat_reg_b,tex1Cnt) \
	FAST_GPFLAGSET(cp_vat_reg_b,tex1Cnt,CP_VAT_REG_B_TEX1CNT)

#define SC_CP_VAT_REG_B_SET_TEX1FMT(cp_vat_reg_b,tex1Fmt) \
	FAST_GPFLAGSET(cp_vat_reg_b,tex1Fmt,CP_VAT_REG_B_TEX1FMT)

#define SC_CP_VAT_REG_B_SET_TEX1SHFT(cp_vat_reg_b,tex1Shft) \
	FAST_GPFLAGSET(cp_vat_reg_b,tex1Shft,CP_VAT_REG_B_TEX1SHFT)

#define SC_CP_VAT_REG_B_SET_TEX2CNT(cp_vat_reg_b,tex2Cnt) \
	FAST_GPFLAGSET(cp_vat_reg_b,tex2Cnt,CP_VAT_REG_B_TEX2CNT)

#define SC_CP_VAT_REG_B_SET_TEX2FMT(cp_vat_reg_b,tex2Fmt) \
	FAST_GPFLAGSET(cp_vat_reg_b,tex2Fmt,CP_VAT_REG_B_TEX2FMT)

#define SC_CP_VAT_REG_B_SET_TEX2SHFT(cp_vat_reg_b,tex2Shft) \
	FAST_GPFLAGSET(cp_vat_reg_b,tex2Shft,CP_VAT_REG_B_TEX2SHFT)

#define SC_CP_VAT_REG_B_SET_TEX3CNT(cp_vat_reg_b,tex3Cnt) \
	FAST_GPFLAGSET(cp_vat_reg_b,tex3Cnt,CP_VAT_REG_B_TEX3CNT)

#define SC_CP_VAT_REG_B_SET_TEX3FMT(cp_vat_reg_b,tex3Fmt) \
	FAST_GPFLAGSET(cp_vat_reg_b,tex3Fmt,CP_VAT_REG_B_TEX3FMT)

#define SC_CP_VAT_REG_B_SET_TEX3SHFT(cp_vat_reg_b,tex3Shft) \
	FAST_GPFLAGSET(cp_vat_reg_b,tex3Shft,CP_VAT_REG_B_TEX3SHFT)

#define SC_CP_VAT_REG_B_SET_TEX4CNT(cp_vat_reg_b,tex4Cnt) \
	FAST_GPFLAGSET(cp_vat_reg_b,tex4Cnt,CP_VAT_REG_B_TEX4CNT)

#define SC_CP_VAT_REG_B_SET_TEX4FMT(cp_vat_reg_b,tex4Fmt) \
	FAST_GPFLAGSET(cp_vat_reg_b,tex4Fmt,CP_VAT_REG_B_TEX4FMT)

#define SC_CP_VAT_REG_B_SET_VCACHE_ENHANCE(cp_vat_reg_b,vcache_enhance) \
	FAST_GPFLAGSET(cp_vat_reg_b,vcache_enhance,CP_VAT_REG_B_VCACHE_ENHANCE)

#define SC_CP_VAT_REG_C_SET_TEX4SHFT(cp_vat_reg_c,tex4Shft) \
	FAST_GPFLAGSET(cp_vat_reg_c,tex4Shft,CP_VAT_REG_C_TEX4SHFT)

#define SC_CP_VAT_REG_C_SET_TEX5CNT(cp_vat_reg_c,tex5Cnt) \
	FAST_GPFLAGSET(cp_vat_reg_c,tex5Cnt,CP_VAT_REG_C_TEX5CNT)

#define SC_CP_VAT_REG_C_SET_TEX5FMT(cp_vat_reg_c,tex5Fmt) \
	FAST_GPFLAGSET(cp_vat_reg_c,tex5Fmt,CP_VAT_REG_C_TEX5FMT)

#define SC_CP_VAT_REG_C_SET_TEX5SHFT(cp_vat_reg_c,tex5Shft) \
	FAST_GPFLAGSET(cp_vat_reg_c,tex5Shft,CP_VAT_REG_C_TEX5SHFT)

#define SC_CP_VAT_REG_C_SET_TEX6CNT(cp_vat_reg_c,tex6Cnt) \
	FAST_GPFLAGSET(cp_vat_reg_c,tex6Cnt,CP_VAT_REG_C_TEX6CNT)

#define SC_CP_VAT_REG_C_SET_TEX6FMT(cp_vat_reg_c,tex6Fmt) \
	FAST_GPFLAGSET(cp_vat_reg_c,tex6Fmt,CP_VAT_REG_C_TEX6FMT)

#define SC_CP_VAT_REG_C_SET_TEX6SHFT(cp_vat_reg_c,tex6Shft) \
	FAST_GPFLAGSET(cp_vat_reg_c,tex6Shft,CP_VAT_REG_C_TEX6SHFT)

#define SC_CP_VAT_REG_C_SET_TEX7CNT(cp_vat_reg_c,tex7Cnt) \
	FAST_GPFLAGSET(cp_vat_reg_c,tex7Cnt,CP_VAT_REG_C_TEX7CNT)

#define SC_CP_VAT_REG_C_SET_TEX7FMT(cp_vat_reg_c,tex7Fmt) \
	FAST_GPFLAGSET(cp_vat_reg_c,tex7Fmt,CP_VAT_REG_C_TEX7FMT)

#define SC_CP_VAT_REG_C_SET_TEX7SHFT(cp_vat_reg_c,tex7Shft) \
	FAST_GPFLAGSET(cp_vat_reg_c,tex7Shft,CP_VAT_REG_C_TEX7SHFT)

#define SC_CP_MATIDX_REG_A_SET_POSIDX(cp_matidx_reg_a,posIdx) \
	FAST_GPFLAGSET(cp_matidx_reg_a,posIdx,CP_MATIDX_REG_A_POSIDX)

#define SC_CP_MATIDX_REG_A_SET_TEX0IDX(cp_matidx_reg_a,tex0Idx) \
	FAST_GPFLAGSET(cp_matidx_reg_a,tex0Idx,CP_MATIDX_REG_A_TEX0IDX)

#define SC_CP_MATIDX_REG_A_SET_TEX1IDX(cp_matidx_reg_a,tex1Idx) \
	FAST_GPFLAGSET(cp_matidx_reg_a,tex1Idx,CP_MATIDX_REG_A_TEX1IDX)

#define SC_CP_MATIDX_REG_A_SET_TEX2IDX(cp_matidx_reg_a,tex2Idx) \
	FAST_GPFLAGSET(cp_matidx_reg_a,tex2Idx,CP_MATIDX_REG_A_TEX2IDX)

#define SC_CP_MATIDX_REG_A_SET_TEX3IDX(cp_matidx_reg_a,tex3Idx) \
	FAST_GPFLAGSET(cp_matidx_reg_a,tex3Idx,CP_MATIDX_REG_A_TEX3IDX)

#define SC_CP_MATIDX_REG_B_SET_TEX4IDX(cp_matidx_reg_b,tex4Idx) \
	FAST_GPFLAGSET(cp_matidx_reg_b,tex4Idx,CP_MATIDX_REG_B_TEX4IDX)

#define SC_CP_MATIDX_REG_B_SET_TEX5IDX(cp_matidx_reg_b,tex5Idx) \
	FAST_GPFLAGSET(cp_matidx_reg_b,tex5Idx,CP_MATIDX_REG_B_TEX5IDX)

#define SC_CP_MATIDX_REG_B_SET_TEX6IDX(cp_matidx_reg_b,tex6Idx) \
	FAST_GPFLAGSET(cp_matidx_reg_b,tex6Idx,CP_MATIDX_REG_B_TEX6IDX)

#define SC_CP_MATIDX_REG_B_SET_TEX7IDX(cp_matidx_reg_b,tex7Idx) \
	FAST_GPFLAGSET(cp_matidx_reg_b,tex7Idx,CP_MATIDX_REG_B_TEX7IDX)

#define SC_CP_ARRAY_BASE_REG_SET_BASE(cp_array_base_reg,base) \
	FAST_GPFLAGSET(cp_array_base_reg,base,CP_ARRAY_BASE_REG_BASE)

#define SC_CP_ARRAY_BASE_REG_SET_PAD0(cp_array_base_reg,pad0) \
	FAST_GPFLAGSET(cp_array_base_reg,pad0,CP_ARRAY_BASE_REG_PAD0)

#define SC_CP_ARRAY_STRIDE_REG_SET_STRIDE(cp_array_stride_reg,stride) \
	FAST_GPFLAGSET(cp_array_stride_reg,stride,CP_ARRAY_STRIDE_REG_STRIDE)

#define SC_CP_ARRAY_STRIDE_REG_SET_PAD0(cp_array_stride_reg,pad0) \
	FAST_GPFLAGSET(cp_array_stride_reg,pad0,CP_ARRAY_STRIDE_REG_PAD0)

#define SC_CP_STAT_ENABLE_REG_SET_VC_STAT(cp_stat_enable_reg,vc_stat) \
	FAST_GPFLAGSET(cp_stat_enable_reg,vc_stat,CP_STAT_ENABLE_REG_VC_STAT)

#define SC_CP_STAT_ENABLE_REG_SET_PAD0(cp_stat_enable_reg,pad0) \
	FAST_GPFLAGSET(cp_stat_enable_reg,pad0,CP_STAT_ENABLE_REG_PAD0)

#define SC_CP_STAT_ENABLE_REG_SET_FRCLK(cp_stat_enable_reg,frclk) \
	FAST_GPFLAGSET(cp_stat_enable_reg,frclk,CP_STAT_ENABLE_REG_FRCLK)

#define SC_CP_STAT_SEL_REG_SET_ATTR_SEL(cp_stat_sel_reg,attr_sel) \
	FAST_GPFLAGSET(cp_stat_sel_reg,attr_sel,CP_STAT_SEL_REG_ATTR_SEL)

#define SC_CP_STAT_SEL_REG_SET_STALLPERF_SEL(cp_stat_sel_reg,stallperf_sel) \
	FAST_GPFLAGSET(cp_stat_sel_reg,stallperf_sel,CP_STAT_SEL_REG_STALLPERF_SEL)

#define SC_CP_XF_LOADREGS_SET_ADDR(cp_xf_loadregs,addr) \
	FAST_GPFLAGSET(cp_xf_loadregs,addr,CP_XF_LOADREGS_ADDR)

#define SC_CP_XF_LOADREGS_SET_CNT(cp_xf_loadregs,cnt) \
	FAST_GPFLAGSET(cp_xf_loadregs,cnt,CP_XF_LOADREGS_CNT)

#define SC_CP_XF_LOADINDEX_SET_ADDR(cp_xf_loadindex,addr) \
	FAST_GPFLAGSET(cp_xf_loadindex,addr,CP_XF_LOADINDEX_ADDR)

#define SC_CP_XF_LOADINDEX_SET_CNT(cp_xf_loadindex,cnt) \
	FAST_GPFLAGSET(cp_xf_loadindex,cnt,CP_XF_LOADINDEX_CNT)

#define SC_CP_XF_LOADINDEX_SET_IDX(cp_xf_loadindex,idx) \
	FAST_GPFLAGSET(cp_xf_loadindex,idx,CP_XF_LOADINDEX_IDX)

#define SC_CP_CALLOBJ_D1_SET_PAD0(cp_callobj_d1,pad0) \
	FAST_GPFLAGSET(cp_callobj_d1,pad0,CP_CALLOBJ_D1_PAD0)

#define SC_CP_CALLOBJ_D1_SET_ADDR(cp_callobj_d1,addr) \
	FAST_GPFLAGSET(cp_callobj_d1,addr,CP_CALLOBJ_D1_ADDR)

#define SC_CP_CALLOBJ_D2_SET_PAD0(cp_callobj_d2,pad0) \
	FAST_GPFLAGSET(cp_callobj_d2,pad0,CP_CALLOBJ_D2_PAD0)

#define SC_CP_CALLOBJ_D2_SET_CNT(cp_callobj_d2,cnt) \
	FAST_GPFLAGSET(cp_callobj_d2,cnt,CP_CALLOBJ_D2_CNT)
