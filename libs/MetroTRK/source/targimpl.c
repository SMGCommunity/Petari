#include "TRK_Types.h"
#include "dolphin.h"
#include "portable/mpc_7xx_603e.h"
#include "portable/ppc_reg.h"

static void TRKExceptionHandler(ui16);

typedef struct StopInfo_PPC{
	ui32 PC;
	ui32 PCInstruction;
	ui16 exceptionID;
} StopInfo_PPC;

typedef struct TRKExceptionStatus{
	StopInfo_PPC exceptionInfo;
	ui8 inTRK;
	ui8 exceptionDetected;
}TRKExceptionStatus;

static TRKExceptionStatus gTRKExceptionStatus = {
	{0,0,0},
	true,
	0
};

ProcessorState_PPC gTRKCPUState;
TRKState_PPC gTRKState;
static ui16 TRK_saved_exceptionID = 0;
Default_PPC gTRKSaveState;

asm ui32 __TRK_get_MSR(){
	nofralloc
	mfmsr r3
	blr
}

asm void __TRK_set_MSR(ui32 val){
	nofralloc
	mtmsr r3
	blr
}

void TRKPostInterruptEvent(void);

asm void TRKInterruptHandler() {
	nofralloc
	mtsrr0 r2
	mtsrr1 r4
	mfsprg r4, 3
	mfcr r2
	mtsprg 3, r2
	lis r2, gTRKState@h
	ori r2, r2, gTRKState@l
	lwz r2, TRKState_PPC.MSR(r2)
	ori r2, r2, 0x8002
	xori r2, r2, 0x8002
	sync
	mtmsr r2
	sync
	lis r2, TRK_saved_exceptionID@h
	ori r2, r2, TRK_saved_exceptionID@l
	sth r3, 0(r2)
	cmpwi r3, 0x500
	bne L_802CF694
	lis r2, gTRKCPUState@h
	ori r2, r2, gTRKCPUState@l
	mflr r3
	stw r3, ProcessorState_PPC.transport_handler_saved_ra(r2)
	bl TRKUARTInterruptHandler
	lis r2, gTRKCPUState@h
	ori r2, r2, gTRKCPUState@l
	lwz r3, ProcessorState_PPC.transport_handler_saved_ra(r2)
	mtlr r3
	lis r2, gTRKState@h
	ori r2, r2, gTRKState@l
	lwz r2, TRKState_PPC.inputPendingPtr(r2)
	lbz r2, TRKState_PPC.GPR[0](r2)
	cmpwi r2, 0
	beq L_802CF678
	lis r2, gTRKExceptionStatus@h
	ori r2, r2, gTRKExceptionStatus@l
	lbz r2, TRKExceptionStatus.inTRK(r2)
	cmpwi r2, 1
	beq L_802CF678
	lis r2, gTRKState@h
	ori r2, r2, gTRKState@l
	li r3, 1
	stb r3, TRKState_PPC.inputActivated(r2)
	b L_802CF694
L_802CF678:
	lis r2, gTRKSaveState@h
	ori r2, r2, gTRKSaveState@l
	lwz r3, Default_PPC.CR(r2)
	mtcrf 0xff, r3
	lwz r3, Default_PPC.GPR[3](r2)
	lwz r2, Default_PPC.GPR[2](r2)
	rfi 
L_802CF694:
	lis r2, TRK_saved_exceptionID@h
	ori r2, r2, TRK_saved_exceptionID@l
	lhz r3, 0(r2)
	lis r2, gTRKExceptionStatus@h
	ori r2, r2, gTRKExceptionStatus@l
	lbz r2, TRKExceptionStatus.inTRK(r2)
	cmpwi r2, 0
	bne TRKExceptionHandler
	lis r2, gTRKCPUState@h
	ori r2, r2, gTRKCPUState@l
	stw r0, ProcessorState_PPC.Default.GPR[0](r2)
	stw r1, ProcessorState_PPC.Default.GPR[1](r2)
	mfsprg r0, 1
	stw r0, ProcessorState_PPC.Default.GPR[2](r2)
	sth r3, ProcessorState_PPC.Extended1.exceptionID(r2)
	sth r3, (ProcessorState_PPC.Extended1.exceptionID + 2)(r2)
	mfsprg r0, 2
	stw r0, ProcessorState_PPC.Default.GPR[3](r2)
	stmw r4, ProcessorState_PPC.Default.GPR[4](r2)
	mfsrr0 r27
	mflr r28
	mfsprg r29, 3
	mfctr r30
	mfxer r31
	stmw r27, ProcessorState_PPC.Default.PC(r2)
	bl TRKSaveExtended1Block
	lis r2, gTRKExceptionStatus@h
	ori r2, r2, gTRKExceptionStatus@l
	li r3, 1
	stb r3, TRKExceptionStatus.inTRK(r2)
	lis r2, gTRKState@h
	ori r2, r2, gTRKState@l
	lwz r0, TRKState_PPC.MSR(r2)
	sync
	mtmsr r0
	sync
	lwz r0, TRKState_PPC.LR(r2)
	mtlr r0
	lwz r0, TRKState_PPC.CTR(r2)
	mtctr r0
	lwz r0, TRKState_PPC.XER(r2)
	mtxer r0
	lwz r0, TRKState_PPC.DSISR(r2)
	mtdsisr r0
	lwz r0, TRKState_PPC.DAR(r2)
	mtdar r0
	lmw r3, TRKState_PPC.GPR[3](r2)
	lwz r0, TRKState_PPC.GPR[0](r2)
	lwz r1, TRKState_PPC.GPR[1](r2)
	lwz r2, TRKState_PPC.GPR[2](r2)
	b TRKPostInterruptEvent
}
