#ifndef PPC_REG_H
#define PPC_REG_H

typedef struct Default_PPC{
	ui32 GPR[32];
	ui32 PC;
	ui32 LR;
	ui32 CR;
	ui32 CTR;
	ui32 XER;
} Default_PPC;

typedef struct Float_PPC{
	ui64 FPR[32];
	ui64 FPSCR;
	ui64 FPECR;
} Float_PPC;

typedef struct Extended1_PPC_6xx_7xx{
	ui32 SR[16];
	ui32 TBL;
	ui32 TBU;
	ui32 HID0;
	ui32 HID1;
	ui32 MSR;
	ui32 PVR;
	ui32 IBAT0U;
	ui32 IBAT0L;
	ui32 IBAT1U;
	ui32 IBAT1L;
	ui32 IBAT2U;
	ui32 IBAT2L;
	ui32 IBAT3U;
	ui32 IBAT3L;
	ui32 DBAT0U;
	ui32 DBAT0L;
	ui32 DBAT1U;
	ui32 DBAT1L;
	ui32 DBAT2U;
	ui32 DBAT2L;
	ui32 DBAT3U;
	ui32 DBAT3L;
	ui32 DMISS;
	ui32 DCMP;
	ui32 HASH1;
	ui32 HASH2;
	ui32 IMISS;
	ui32 ICMP;
	ui32 RPA;
	ui32 SDR1;
	ui32 DAR;
	ui32 DSISR;
	ui32 SPRG0;
	ui32 SPRG1;
	ui32 SPRG2;
	ui32 SPRG3;
	ui32 DEC;
	ui32 IABR;
	ui32 EAR;
	ui32 DABR;
	ui32 PMC1;
	ui32 PMC2;
	ui32 PMC3;
	ui32 PMC4;
	ui32 SIA;
	ui32 MMCR0;
	ui32 MMCR1;
	ui32 THRM1;
	ui32 THRM2;
	ui32 THRM3;
	ui32 ICTC;
	ui32 L2CR;
	ui32 UMMCR2;
	ui32 UBAMR;
	ui32 UMMCR0;
	ui32 UPMC1;
	ui32 UPMC2;
	ui32 USIA;
	ui32 UMMCR1;
	ui32 UPMC3;
	ui32 UPMC4;
	ui32 USDA;
	ui32 MMCR2;
	ui32 BAMR;
	ui32 SDA;
	ui32 MSSCR0;
	ui32 MSSCR1;
	ui32 PIR;
	ui32 exceptionID;
	ui32 GQR[8];
	ui32 HID_G;
	ui32 WPAR;
	ui32 DMA_U;
	ui32 DMA_L;
} Extended1_PPC_6xx_7xx;

typedef struct Extended2_PPC_6xx_7xx{
	ui32 PSR[32][2];
} Extended2_PPC_6xx_7xx;

typedef struct ProcessorState_PPC_6xx_7xx{
	Default_PPC Default;
	Float_PPC Float;
	Extended1_PPC_6xx_7xx Extended1;
	Extended2_PPC_6xx_7xx Extended2;
	ui32 transport_handler_saved_ra;
} ProcessorState_PPC_6xx_7xx;


typedef ProcessorState_PPC_6xx_7xx ProcessorState_PPC;

#endif // PPC_REG_H