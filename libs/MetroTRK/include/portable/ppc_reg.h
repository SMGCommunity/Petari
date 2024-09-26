#ifndef PPC_REG_H
#define PPC_REG_H

#include "TRK_Types.h"

typedef u32       DefaultType;
typedef u32       Extended1Type;
typedef u64       FloatType;

typedef struct Default_PPC{
	ui32 GPR[32];
	ui32 PC;
	ui32 LR;
	ui32 CR;
	ui32 CTR;
	ui32 XER;
} Default_PPC;

typedef DefaultType    PCType;
typedef u32            InstructionType;
typedef u16            ExceptionCauseType;

typedef struct StopInfo_PPC
{
    PCType PC;
    InstructionType PCInstruction;
    ExceptionCauseType exceptionID;
    u8 pad[2];
} StopInfo_PPC;

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

#define SPR_PIR                      1023

#define SPR_XER                      1

#define SPR_LR                       8
#define SPR_CTR                      9

#define SPR_DSISR                    18
#define SPR_DAR                      19

#define SPR_DEC                      22

#define SPR_SDR1                     25
#define SPR_SRR0                     26
#define SPR_SRR1                     27

#define SPR_CSRR0                    58
#define SPR_CSRR1                    59

#define SPR_VRSAVE                   256

#define TBR_TBL_READ                 268
#define TBR_TBU_READ                 269

#define SPR_SPRG0                    272
#define SPR_SPRG1                    273
#define SPR_SPRG2                    274
#define SPR_SPRG3                    275

#define SPR_EAR                      282

#define TBR_TBL_WRITE                284
#define TBR_TBU_WRITE                285

#define SPR_PVR                      287

#define SPR_IBAT0U                   528
#define SPR_IBAT0L                   529
#define SPR_IBAT1U                   530
#define SPR_IBAT1L                   531
#define SPR_IBAT2U                   532
#define SPR_IBAT2L                   533
#define SPR_IBAT3U                   534
#define SPR_IBAT3L                   535

#define SPR_DBAT0U                   536
#define SPR_DBAT0L                   537
#define SPR_DBAT1U                   538
#define SPR_DBAT1L                   539
#define SPR_DBAT2U                   540
#define SPR_DBAT2L                   541
#define SPR_DBAT3U                   542
#define SPR_DBAT3L                   543

#define SPR_IBAT4U                   560
#define SPR_IBAT4L                   561
#define SPR_IBAT5U                   562
#define SPR_IBAT5L                   563
#define SPR_IBAT6U                   564
#define SPR_IBAT6L                   565
#define SPR_IBAT7U                   566
#define SPR_IBAT7L                   567

#define SPR_DBAT4U                   568
#define SPR_DBAT4L                   569
#define SPR_DBAT5U                   570
#define SPR_DBAT5L                   571
#define SPR_DBAT6U                   572
#define SPR_DBAT6L                   573
#define SPR_DBAT7U                   574
#define SPR_DBAT7L                   575

#define SPR_GQR0                     912
#define SPR_GQR1                     913
#define SPR_GQR2                     914
#define SPR_GQR3                     915
#define SPR_GQR4                     916
#define SPR_GQR5                     917
#define SPR_GQR6                     918
#define SPR_GQR7                     919

#define SPR_HID2                     920
#define SPR_WPAR                     921
#define SPR_DMAU                     922
#define SPR_DMAL                     923

#define SPR_UMMCR2                   928
#define SPR_UBAMR                    935

#define SPR_UMMCR0                   936
#define SPR_UPMC1                    937
#define SPR_UPMC2                    938
#define SPR_USIA                     939
#define SPR_UMMCR1                   940
#define SPR_UPMC3                    941
#define SPR_UPMC4                    942
#define SPR_USDA                     943

#define SPR_MMCR2                    944
#define SPR_BAMR                     951

#define SPR_MMCR0                    952
#define SPR_PMC1                     953
#define SPR_PMC2                     954
#define SPR_SIA                      955
#define SPR_MMCR1                    956
#define SPR_PMC3                     957
#define SPR_PMC4                     958
#define SPR_SDA                      959

#define SPR_DMISS                    976
#define SPR_DCMP                     977
#define SPR_HASH1                    978
#define SPR_HASH2                    979
#define SPR_IMISS                    980
#define SPR_ICMP                     981
#define SPR_RPA                      982

#define SPR_HID0                     1008
#define SPR_HID1                     1009
#define SPR_IABR                     1010
#define SPR_HID4                     1011
#define SPR_TDCL                     1012
#define SPR_DABR                     1013

#define SPR_MSSCR0                   1014
#define SPR_MSSCR1                   1015

#define SPR_L2CR                     1017
#define SPR_TDCH                     1018
#define SPR_ICTC                     1019

#define SPR_THRM1                    1020
#define SPR_THRM2                    1021
#define SPR_THRM3                    1022


#endif // PPC_REG_H