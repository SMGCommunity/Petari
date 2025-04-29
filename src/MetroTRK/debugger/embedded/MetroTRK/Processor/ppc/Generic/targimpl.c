#include "TRK_Types.h"
#include "dolphin.h"
#include "portable/mpc_7xx_603e.h"
#include "portable/msgcmd.h"
#include "portable/msgbuf.h"
#include "portable/nubevent.h"
#include "portable/ppc_reg.h"
#include "portable/ppc_mem.h"
#include "portable/support.h"
#include "portable/dserror.h"
#include "portable/targimpl_ppc.h"
#include "size_t.h"

static void TRKExceptionHandler(ui16);
DSError TRKTargetAccessMemory(void *Data, void *virtualAddress, size_t *memorySize, MemoryAccessOptions accessOptions, s32 read);

typedef struct TRKExceptionStatus{
	StopInfo_PPC exceptionInfo;
	ui8 inTRK;
	ui8 exceptionDetected;
	u8 pad[2];
}TRKExceptionStatus;

typedef struct {
    s32 active;
    DSMessageStepOptions type;
    u32 count;
    u32 rangeStart;
    u32 rangeEnd;
    u8 pad[4];
} PpcStepStatus;

static PpcStepStatus gTRKStepStatus =
{
    0,
    kDSStepIntoCount,
    0,
    0,
    0,
    { 0, 0, 0, 0 }
};

static TRKExceptionStatus gTRKExceptionStatus = {
	{0,0,0},
	true,
	0
};

ProcessorState_PPC gTRKCPUState;

ProcessorRestoreFlags_PPC gTRKRestoreFlags =
{
    0,
    0,
    {
        0, 0, 0, 0, 0, 0, 0
    }
};

TRKState_PPC gTRKState;
static ui16 TRK_saved_exceptionID = 0;
Default_PPC gTRKSaveState;

#define SHIFT_LEFT_X_BY_Y_BITS(x, y)             (x << y)
#define SHIFT_RIGHT_X_BY_Y_BITS(x, y)            (x >> y)

DSError TRKTargetReadInstruction(InstructionType *readData, void *virtualAddress);

/*
** Instruction Builder
*/

/*
** Instruction   :        mfspr    rD, SPR
** (Move from Special Purpose Register)
**
** Bit  0..5     :        31         (decimal)
** Bit  6..10    :        rD
** Bit 11..20    :        SPR
** Bit 21..30    :        339        (decimal)
** Bit 31        :        0
*/
#define INSTR_MFSPR(rD, SPR)    /* Bit  0.. 5 */ (SHIFT_LEFT_X_BY_Y_BITS(31, 26))            |    \
                                /* Bit  6..10 */ (SHIFT_LEFT_X_BY_Y_BITS(rD, 21))            |    \
                                /* Bit 11..20 */ (SHIFT_LEFT_X_BY_Y_BITS((SPR & 0xfe0), 6))  |    \
                                                 (SHIFT_LEFT_X_BY_Y_BITS((SPR & 0x1f), 16))  |    \
                                /* Bit 21..30 */ (SHIFT_LEFT_X_BY_Y_BITS(339, 1))            |    \
                                /* Bit     31 */ 0

/*
** Instruction    :       mtspr    SPR, rS
** (Move to Special Purpose Register)
**
** Bit 0..5      :        31         (decimal)
** Bit 6..10     :        rS
** Bit 11..20    :        SPR
** Bit 21..30    :        467        (decimal)
** Bit 31        :        0
*/
#define INSTR_MTSPR(SPR, rS)    /* Bit  0.. 5 */ (SHIFT_LEFT_X_BY_Y_BITS(31, 26))            |    \
                                /* Bit  6..10 */ (SHIFT_LEFT_X_BY_Y_BITS(rS, 21))            |    \
                                /* Bit 11..20 */ (SHIFT_LEFT_X_BY_Y_BITS((SPR & 0xfe0), 6))  |    \
                                                 (SHIFT_LEFT_X_BY_Y_BITS((SPR & 0x1f), 16))  |    \
                                /* Bit 21..30 */ (SHIFT_LEFT_X_BY_Y_BITS(467, 1))            |    \
                                /* Bit     31 */ 0

/*
** Instruction   :        stw        rS, d(rA)
** (Store Word)
**
** Bit 0..5      :        36         (decimal)
** Bit 6..10     :        rS
** Bit 11..15    :        rA
** Bit 16..31    :        d
*/
#define INSTR_STW(rS, d, rA)    /* Bit  0.. 5 */ (SHIFT_LEFT_X_BY_Y_BITS(36, 26))            |    \
                                /* Bit  6..10 */ (SHIFT_LEFT_X_BY_Y_BITS(rS, 21))            |    \
                                /* Bit 11..15 */ (SHIFT_LEFT_X_BY_Y_BITS(rA, 16))            |    \
                                /* Bit 16..31 */ d

/*
** Instruction   :        lwz        rD, d(rA)
** (Load Word and Zero)
**
** Bit 0..5      :        32         (decimal)
** Bit 6..10     :        rD
** Bit 11..15    :        rA
** Bit 16..31    :        d
*/
#define INSTR_LWZ(rD, d, rA)    /* Bit  0.. 5 */ (SHIFT_LEFT_X_BY_Y_BITS(32, 26))            |    \
                                /* Bit  6..10 */ (SHIFT_LEFT_X_BY_Y_BITS(rD, 21))            |    \
                                /* Bit 11..15 */ (SHIFT_LEFT_X_BY_Y_BITS(rA, 16))            |    \
                                /* Bit 16..31 */ d

/*
** Instruction   :        stfd       frS, d(rA)
** (Store Floating-Point Double)
**
** Bit 0..5      :        54         (decimal)
** Bit 6..10     :        frS
** Bit 11..15    :        rA
** Bit 16..31    :        d
*/
#define INSTR_STFD(frS, d, rA)  /* Bit  0.. 5 */ (SHIFT_LEFT_X_BY_Y_BITS(54,  26))           |    \
                                /* Bit  6..10 */ (SHIFT_LEFT_X_BY_Y_BITS(frS, 21))           |    \
                                /* Bit 11..15 */ (SHIFT_LEFT_X_BY_Y_BITS(rA,  16))           |    \
                                /* Bit 16..31 */ d

/*
** Instruction   :        lfd        frD, d(rA)
** (Load Floating-Point Double)
**
** Bit 0..5      :        50         (decimal)
** Bit 6..10     :        frD
** Bit 11..15    :        rA
** Bit 16..31    :        d
*/
#define INSTR_LFD(frD, d, rA)   /* Bit  0.. 5 */ (SHIFT_LEFT_X_BY_Y_BITS(50,  26))           |    \
                                /* Bit  6..10 */ (SHIFT_LEFT_X_BY_Y_BITS(frD, 21))           |    \
                                /* Bit 11..15 */ (SHIFT_LEFT_X_BY_Y_BITS(rA,  16))           |    \
                                /* Bit 16..31 */ d

/*
** Instruction   :        mffs       frD
** (Move from FPSCR)
**
** Bit 0..5      :        63         (decimal)
** Bit 6..10     :        frD
** Bit 11..20    :        0
** Bit 21..30    :        583        (decimal)
** Bit 31        :        0
*/
#define INSTR_MFFS(frD)         /* Bit  0.. 5 */ (SHIFT_LEFT_X_BY_Y_BITS(63, 26))            |    \
                                /* Bit  6..10 */ (SHIFT_LEFT_X_BY_Y_BITS(frD, 21))           |    \
                                /* Bit 11..20 */ 0                                           |    \
                                /* Bit 21..30 */ (SHIFT_LEFT_X_BY_Y_BITS(583, 1))            |    \
                                /* Bit     31 */ 0

/*
** Instruction   :        mtfsf      FM, frB
** (Move to FPSCR fields)
**
** Bit 0..5      :        63         (decimal)
** Bit 6         :        0
** Bit 7..14     :        FM
** Bit 15        :        0
** Bit 16..20    :        frB
** Bit 21..30    :        711        (decimal)
** Bit 31        :        0
*/
#define INSTR_MTFSF(FM,frB)     /* Bit  0.. 5 */ (SHIFT_LEFT_X_BY_Y_BITS(63, 26))            |    \
                                /* Bit  6     */ 0                                           |    \
                                /* Bit  7..14 */ (SHIFT_LEFT_X_BY_Y_BITS(FM, 17))            |    \
                                /* Bit  15    */ 0                                           |    \
                                /* Bit 16..20 */ (SHIFT_LEFT_X_BY_Y_BITS(frB, 11))           |    \
                                /* Bit 21..30 */ (SHIFT_LEFT_X_BY_Y_BITS(711, 1))            |    \
                                /* Bit     31 */ 0

/*
** Instruction   :        stvx       vS, rA, rB
** (Store Vector Indexed)
**
** Bit 0..5      :        31         (decimal)
** Bit 6..10     :        vS
** Bit 11..15    :        rA
** Bit 16..20    :        rB
** Bit 21..30    :        231
** Bit 31        :        0
*/
#define INSTR_STVX(vS, rA, rB)  /* Bit  0.. 5 */ (SHIFT_LEFT_X_BY_Y_BITS(31, 26))            |    \
                                /* Bit  6..10 */ (SHIFT_LEFT_X_BY_Y_BITS(vS, 21))            |    \
                                /* Bit 11..15 */ (SHIFT_LEFT_X_BY_Y_BITS(rA, 16))            |    \
                                /* Bit 16..20 */ (SHIFT_LEFT_X_BY_Y_BITS(rB, 11))            |    \
                                /* Bit 21..30 */ (SHIFT_LEFT_X_BY_Y_BITS(231, 1))            |    \
                                /* Bit 31     */ 0

/*
** Instruction   :        lvx        vD, rA, rB
** (Load Vector Indexed)
**
** Bit 0..5      :        31         (decimal)
** Bit 6..10     :        vS
** Bit 11..15    :        rA
** Bit 16..20    :        rB
** Bit 21..30    :        103
** Bit 31        :        0
*/
#define INSTR_LVX(vD, rA, rB)   /* Bit  0.. 5 */ (SHIFT_LEFT_X_BY_Y_BITS(31, 26))            |    \
                                /* Bit  6..10 */ (SHIFT_LEFT_X_BY_Y_BITS(vD, 21))            |    \
                                /* Bit 11..15 */ (SHIFT_LEFT_X_BY_Y_BITS(rA, 16))            |    \
                                /* Bit 16..20 */ (SHIFT_LEFT_X_BY_Y_BITS(rB, 11))            |    \
                                /* Bit 21..30 */ (SHIFT_LEFT_X_BY_Y_BITS(103, 1))            |    \
                                /* Bit 31     */ 0

/*
** Instruction   :        mfvscr     vD
** (Move from Vector Status and Control Register)
**
** Bit 0..5      :        4          (decimal)
** Bit 6..10     :        vD
** Bit 11..20    :        0
** Bit 21..30    :        1540       (decimal)
** Bit 31        :        0
*/
#define INSTR_MFVSCR(vD)        /* Bit  0.. 5 */ (SHIFT_LEFT_X_BY_Y_BITS( 4,  26))           |    \
                                /* Bit  6..10 */ (SHIFT_LEFT_X_BY_Y_BITS(vD,  21))           |    \
                                /* Bit 11..20 */ (SHIFT_LEFT_X_BY_Y_BITS( 0,   0))           |    \
                                /* Bit 21..30 */ (SHIFT_LEFT_X_BY_Y_BITS(1540, 0))           |    \
                                /* Bit 31     */ 0

/*
** Instruction   :        mtvscr     vB
** (Move to Vector Status and Control Register)
**
** Bit 0..5      :        4          (decimal)
** Bit 6..10     :        vB
** Bit 11..20    :        0
** Bit 21..30    :        1604       (decimal)
** Bit 31        :        0
*/
#define INSTR_MTVSCR(vB)        /* Bit  0.. 5 */ (SHIFT_LEFT_X_BY_Y_BITS( 4,  26))           |    \
                                /* Bit  6..10 */ (SHIFT_LEFT_X_BY_Y_BITS(vB,  21))           |    \
                                /* Bit 11..20 */ (SHIFT_LEFT_X_BY_Y_BITS( 0,   0))           |    \
                                /* Bit 21..30 */ (SHIFT_LEFT_X_BY_Y_BITS(1604, 0))           |    \
                                /* Bit 31     */ 0

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

static asm void TRK_ppc_memcpy(void *dst, const void *src, u32 length, u32 dst_MSR, u32 src_MSR) {
	nofralloc

	mfmsr	r8
	li		r10, 0

cond:
	cmpw	r10, r5
	beq		end

loop:
	mtmsr	r7
	sync
	lbzx	r9, r10, r4
	mtmsr	r6
	sync
	stbx	r9, r10, r3
	addi 	r10, r10, 1
	b		loop

end:
	mtmsr	r8
	sync
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

asm void TRKExceptionHandler(ui16) {
	nofralloc

	addis     	r2, r0, gTRKExceptionStatus@h
	ori       	r2, r2, gTRKExceptionStatus@l
	sth       	r3, TRKExceptionStatus.exceptionInfo.exceptionID(r2)
	mfspr		r3, 26
	stw       	r3, TRKExceptionStatus.exceptionInfo.PC(r2)
	lhz       	r3, TRKExceptionStatus.exceptionInfo.exceptionID(r2)
	cmpwi 		r3, 0x200
	bt			2, skip_instr
	cmpwi 		r3, 0x300
	bt			2, skip_instr
	cmpwi 		r3, 0x400
	bt			2, skip_instr
	cmpwi 		r3, 0x600
	bt			2, skip_instr
	cmpwi 		r3, 0x700
	bt			2, skip_instr
	cmpwi 		r3, 0x800
	bt			2, skip_instr
	cmpwi 		r3, 0x1000
	bt			2, skip_instr
	cmpwi 		r3, 0x1100
	bt			2, skip_instr
	cmpwi 		r3, 0x1200
	bt			2, skip_instr
	cmpwi 		r3, 0x1300
	bt			2, skip_instr
	b			set

skip_instr:
	mfspr		r3, 26
	addi 		r3, r3, 4
	mtspr		26, r3

set:
	addis     	r2, r0, gTRKExceptionStatus@h
	ori       	r2, r2, gTRKExceptionStatus@l
	addi      	r3, r0, 1
	stb       	r3, TRKExceptionStatus.exceptionDetected(r2)

	mfspr		r3, 275
	mtcrf		0xFF, r3

	mfspr		r2, 273
	mfspr		r3, 274
	rfi
}

 void TRKInterruptHandlerEnableInterrupts(void);

asm void TRKSwapAndGo() {
	nofralloc

	addis     r3, r0, gTRKState@h
    ori       r3, r3, gTRKState@l

    stmw      r0, TRKState_PPC.GPR[0](r3)

    mfmsr     r0
    stw       r0, TRKState_PPC.MSR(r3)

    mflr      r0
    stw       r0, TRKState_PPC.LR(r3)

    mfctr     r0
    stw       r0, TRKState_PPC.CTR(r3)

    mfxer     r0
    stw       r0, TRKState_PPC.XER(r3)

    mfspr     r0, 18
    stw       r0, TRKState_PPC.DSISR(r3)

    mfspr     r0, 19
    stw       r0, TRKState_PPC.DAR(r3)
	
    addi      r1, r0, 0x8002
    nor       r1, r1, r1
    mfmsr     r3
    and       r3, r3, r1
    mtmsr     r3

    addis     r2, r0, gTRKState@h
    ori       r2, r2, gTRKState@l
    lwz       r2, TRKState_PPC.inputPendingPtr(r2)
    lbz       r2, 0(r2)
    cmpwi     r2, 0
    beq       NoOutgoingInput

    addis     r2, r0, gTRKState@h
    ori       r2, r2, gTRKState@l
    li        r3, 1
    stb       r3, TRKState_PPC.inputActivated(r2)

    b         TRKInterruptHandlerEnableInterrupts

NoOutgoingInput:
    addis     r2, r0, gTRKExceptionStatus@h
    ori       r2, r2, gTRKExceptionStatus@l
    addi      r3, r0, 0
    stb       r3, TRKExceptionStatus.inTRK(r2)
    bl        TRKRestoreExtended1Block

    addis     r2, r0, gTRKCPUState@h
    ori       r2, r2, gTRKCPUState@l

    lmw       r27, ProcessorState_PPC.Default.PC(r2)
    mtspr     26, r27
    mtspr     8, r28
    mtcr      r29
    mtspr     9, r30
    mtspr     1, r31

    // Restore all GPR's

    lmw       r3, ProcessorState_PPC.Default.GPR[3](r2)     // restore GPR3-31
    lwz       r0, ProcessorState_PPC.Default.GPR[0](r2)
    lwz       r1, ProcessorState_PPC.Default.GPR[1](r2)
    lwz       r2, ProcessorState_PPC.Default.GPR[2](r2)

    rfi
}

asm void TRKInterruptHandlerEnableInterrupts(void) {
	nofralloc

    addis     r2, r0, gTRKState@h
    ori       r2, r2, gTRKState@l
    lwz       r0, TRKState_PPC.MSR(r2)
    sync
    mtmsr     r0
    sync

    lwz       r0, TRKState_PPC.LR(r2)
    mtlr      r0

    lwz       r0, TRKState_PPC.CTR(r2)
    mtctr     r0

    lwz       r0, TRKState_PPC.XER(r2)
    mtxer     r0

    lwz       r0, TRKState_PPC.DSISR(r2)
    mtspr     18, r0

    lwz       r0, TRKState_PPC.DAR(r2)
    mtspr     19, r0

    lmw       r3, TRKState_PPC.GPR[3](r2)
    lwz       r0, TRKState_PPC.GPR[0](r2)
    lwz       r1, TRKState_PPC.GPR[1](r2)
    lwz       r2, TRKState_PPC.GPR[2](r2)

    b         TRKPostInterruptEvent
}

asm s32 ReadFPSCR() {
    nofralloc

    stwu      r1, -0x40(r1)
    stfd      f31, 0x10(r1)
    psq_st    f31, 0x20(r1), 0, 0
    mffs      f31
    stfd      f31, 0(r3)
    psq_l     f31, 0x20(r1), 0, 0
    lfd       f31, 0x10(r1)
    addi      r1, r1, 0x40
    blr
}

asm void WriteFPSCR()
{
    nofralloc

    stwu      r1, -0x40(r1)
    stfd      f31, 0x10(r1)
    psq_st    f31, 0x20(r1), 0, 0
    lfd       f31, 0(r3)
    mtfsf     0xff, f31
    psq_l     f31, 0x20(r1), 0, 0
    lfd       f31, 0x10(r1)
    addi      r1, r1, 0x40
    blr
}

void TRKTargetSetInputPendingPtr(volatile u8* inputPendingPtr) {
	gTRKState.inputPendingPtr = inputPendingPtr;
}

s32 TRKTargetStop() {
	gTRKState.stopped = 1; 
	return 0;
}

void TRKTargetSetStopped(s32 stopped) {
	gTRKState.stopped = stopped;
}

s32 TRKTargetStopped() {
	return gTRKState.stopped;
}

DSError TRKTargetSupportRequest(void) {
    size_t *length;
    DSIOResult io_result;
    s32 msg_length;
    MessageCommandID command;
    NubEvent event;
	DSError err = kNoError;

	command = (MessageCommandID)gTRKCPUState.Default.GPR[3];

	if ((command != kDSReadFile) && (command != kDSWriteFile) && (command != kDSOpenFile) && (command != kDSCloseFile) && (command != kDSPositionFile))
    {
        TRKConstructEvent(&event, kExceptionEvent);
        TRKPostEvent(&event);
        return err;
    }

    if (command == kDSOpenFile) {
        err = HandleOpenFileSupportRequest((DSFileHandle)gTRKCPUState.Default.GPR[4], (u8)gTRKCPUState.Default.GPR[5], (u32 *)gTRKCPUState.Default.GPR[6], &io_result);

           if (io_result == kDSIONoError && err != kNoError) {
            io_result = kDSIOError;
		   }

        gTRKCPUState.Default.GPR[3] = (DefaultType)io_result;
    }
    else if (command == kDSCloseFile) {
        err = HandleCloseFileSupportRequest((DSFileHandle)gTRKCPUState.Default.GPR[4], &io_result);

        if (io_result == kDSIONoError && err != kNoError) {
            io_result = kDSIOError;
		}

        gTRKCPUState.Default.GPR[3] = (DefaultType)io_result;
    }
    else if (command == kDSPositionFile) {
        msg_length = DSFetch_u32(gTRKCPUState.Default.GPR[5]);
        err = HandlePositionFileSupportRequest((DSFileHandle)gTRKCPUState.Default.GPR[4], &msg_length, (u8)gTRKCPUState.Default.GPR[6], &io_result);

           if (io_result == kDSIONoError && err != kNoError)
            io_result = kDSIOError;

        gTRKCPUState.Default.GPR[3] = (DefaultType)io_result;
		DSFetch_s32(gTRKCPUState.Default.GPR[5]) = msg_length;
    }
	else{
        length = (size_t *)gTRKCPUState.Default.GPR[5];

        err = TRK_SuppAccessFile((DSFileHandle)gTRKCPUState.Default.GPR[4],
                              (u8*)gTRKCPUState.Default.GPR[6],
                              (s32*)length, &io_result, 1, (command == kDSReadFile));

        if ((io_result == kDSIONoError) && (err != kNoError)) {
            io_result = kDSIOError;
		}

        gTRKCPUState.Default.GPR[3] = (DefaultType)io_result;

        if (command == kDSReadFile) {
            TRK_flush_cache((u32)gTRKCPUState.Default.GPR[6], *length);
        }
    }

	gTRKCPUState.Default.PC += 4;
	return err;
}

PCType TRKTargetGetPC(void) {
    return (PCType)gTRKCPUState.Default.PC;
}

static DSError TRKTargetEnableTrace(s32 enable)
{
    if (enable)
        gTRKCPUState.Extended1.MSR |= 0x400;
    else
        gTRKCPUState.Extended1.MSR = ((gTRKCPUState.Extended1.MSR & (0xFFFFFFFF ^ 0x400)));

    return kNoError;
}

static DSError TRKTargetDoStep(void)
{
    DSError err = kNoError;
    gTRKStepStatus.active = 1;
    TRKTargetEnableTrace(1);

    if ((gTRKStepStatus.type == kDSStepIntoCount) ||
        (gTRKStepStatus.type == kDSStepOverCount))
        gTRKStepStatus.count--;

    TRKTargetSetStopped(0);

    return err;
}

DSError TRKTargetStepOutOfRange(u32 start, u32 end, s32 stepOver) {
    DSError err = kNoError;

    if (stepOver) {
        return kUnsupportedError;
    }
    else {
        gTRKStepStatus.type = kDSStepIntoRange;
        gTRKStepStatus.rangeStart = start;
        gTRKStepStatus.rangeEnd = end;

        err = TRKTargetDoStep();
    }

    return err;
}

DSError TRKTargetSingleStep(u32 count, s32 stepOver)
{
    DSError err = kNoError;

    if (stepOver) {
        err = kUnsupportedError;
    }
    else
    {
        gTRKStepStatus.type = kDSStepIntoCount;
        gTRKStepStatus.count = count;

        err = TRKTargetDoStep();
    }

    return err;
}

DSError TRKTargetAddExceptionInfo(MessageBuffer *buffer) {
	u32 instruction;
    DSError err;
    u32 buf[16];

    err = kNoError;

	TRK_memset(&buf, 0, sizeof(buf));

	buf[0] = sizeof(buf);
    DSFetch_u8(&buf[1]) = kDSNotifyException;
    buf[2] = gTRKExceptionStatus.exceptionInfo.PC;
	err = TRKTargetReadInstruction((void *)&instruction, (void *)gTRKExceptionStatus.exceptionInfo.PC);
	buf[3] = instruction;
    buf[4] = gTRKExceptionStatus.exceptionInfo.exceptionID;

    err = TRKAppendBuffer_ui8(buffer, (const u8 *)&buf[0], sizeof(buf));

    return err;
}

DSError TRKTargetAddStopInfo(MessageBuffer *buffer) {
    u32 instruction;
    DSError err;
    u32 buf[16];
    u32 v12;
    u32 v13;

    err = kNoError;

	TRK_memset(&buf[0], 0, 0x40);
	buf[0] = 0x40;
	DSFetch_u8(&buf[1]) = kDSNotifyStopped;
	buf[2] = gTRKCPUState.Default.PC;
	TRKTargetReadInstruction((void *)&instruction, (void *)gTRKCPUState.Default.PC);
	buf[3] = instruction;
    buf[4] = (gTRKCPUState.Extended1.exceptionID & 0xffff);
	return TRKAppendBuffer_ui8(buffer, (const u8 *)buf, 0x40);
}

s32 TRKTargetCheckStep(void)
{
    s32 err;
    if (gTRKStepStatus.active)
    {
        TRKTargetEnableTrace(0);

        err = 1;

        if (gTRKStepStatus.active &&
            ((u16)gTRKCPUState.Extended1.exceptionID == 0x0D00))
        {
            switch (gTRKStepStatus.type)
            {

                case kDSStepIntoCount:
                    if (gTRKStepStatus.count > 0)
                        err = 0;

                    break;


                case kDSStepIntoRange:
                    if ((gTRKCPUState.Default.PC >= gTRKStepStatus.rangeStart) &&
                        (gTRKCPUState.Default.PC <= gTRKStepStatus.rangeEnd))
                        err = 0;

                    break;

                default:
                    break;
            }
        }

        if (err)
            gTRKStepStatus.active = 0;
        else
            TRKTargetDoStep();
    }

    return gTRKStepStatus.active;
}

DSError TRKTargetInterrupt(NubEvent *event) {
	DSError err = kNoError;

	switch (event->fType) {
		case kBreakpointEvent:
        case kExceptionEvent:
			if (!TRKTargetCheckStep()) {
				TRKTargetSetStopped(1);
				err = TRKDoNotifyStopped(kDSNotifyStopped);
			}
			break;
		default:
			break;
	}

	return err;
}

void TRKPostInterruptEvent(void) {
    NubEvent        event;
    NubEventType    eventType;
    u32 inst;
    s32 post;

	if (gTRKState.inputActivated)  {
        gTRKState.inputActivated = 0;
        return;
    }

	post = 1;

	switch ((u16)gTRKCPUState.Extended1.exceptionID & 0xffff) {
        case 0x700:
        case 0xD00:

            TRKTargetReadInstruction((void *)&inst, (void *)gTRKCPUState.Default.PC);

            if (inst == 0xFE00000) {
                eventType = kSupportEvent;
			}
            else {
                eventType = kBreakpointEvent;
			}
            break;

        default:
            eventType = kExceptionEvent;
            break;
    }

    if (post) {
        TRKConstructEvent(&event, eventType);
        TRKPostEvent(&event);
    }
}

u8 TRKvalue128_temp[16];

DSError TRKPPCAccessSpecialReg(void *value, u32 *access_func, s32 read) {
    typedef void (*asm_access_type)(void *, void *);
	asm_access_type asm_access;

    access_func[9] = 0x4E800020;
    asm_access = (asm_access_type)access_func;

    TRK_flush_cache((u32)access_func, (sizeof(access_func) * 10));
    (*asm_access)(value, (void *)&TRKvalue128_temp);

    return kNoError;
}

DSError TRKPPCAccessSPR(void *value, u32 spr_register_num, s32 read) {
    InstructionType access_func[10] = { 0x60000000,
                                       0x60000000,
                                       0x60000000,
                                       0x60000000,
                                       0x60000000,
                                       0x60000000,
                                       0x60000000,
                                       0x60000000,
                                       0x60000000,
                                       0x60000000};

    if (read) {
        access_func[0] = INSTR_MFSPR(4, spr_register_num);
        access_func[1] = (u32)INSTR_STW(4, 0, 3);
    }
    else
    {
        access_func[0] = (u32)INSTR_LWZ(4, 0, 3);
        access_func[1] = INSTR_MTSPR(spr_register_num, 4);
    }

    return TRKPPCAccessSpecialReg(value, access_func, read);
}

DSError TRKPPCAccessPairedSingleRegister(void *value, u32 register_num, s32 read) {
    InstructionType access_func[10] = { 0x60000000,
                                       0x60000000,
                                       0x60000000,
                                       0x60000000,
                                       0x60000000,
                                       0x60000000,
                                       0x60000000,
                                       0x60000000,
                                       0x60000000,
                                       0x60000000};

    if (read)
        access_func[0] = (register_num << 21 | 0xF0030000);
    else
        access_func[0] = (register_num << 21 | 0xE0030000);

    return TRKPPCAccessSpecialReg(value, access_func, read);
}

DSError TRKTargetAccessExtended2(u32 firstRegister, u32 lastRegister, MessageBuffer *b, size_t *registerStorageSize, s32 read) {
    DSError err;
    FloatType value64;
    u32 value32[2];
    TRKExceptionStatus savedException;

	if (lastRegister > 31) {
		return kInvalidRegister;
	}

	savedException = gTRKExceptionStatus;
    gTRKExceptionStatus.exceptionDetected = 0;

    TRKPPCAccessSPR((void *)value32, 920, 1);

    value32[0] |= 0xA0000000;
    TRKPPCAccessSPR((void *)value32, 920, 0);

    value32[0] = 0;
    TRKPPCAccessSPR((void *)value32, 912, 0);

    *registerStorageSize = 0;
    err = kNoError;

	while ((firstRegister <= lastRegister) && !err)
    {
        if (read)
        {
            TRKPPCAccessPairedSingleRegister((void *)&value64, firstRegister, read);
            err = TRKAppendBuffer1_ui64(b, value64);
        }
        else
        {
            TRKReadBuffer1_ui64(b, &value64);
            err = TRKPPCAccessPairedSingleRegister((void *)&value64, firstRegister, read);
        }

        ++firstRegister;
        *registerStorageSize += sizeof(FloatType);
    }

	if (gTRKExceptionStatus.exceptionDetected)
    {
        err = kCWDSException;
        *registerStorageSize = 0;
    }

	gTRKExceptionStatus = savedException;

    return err;
}

// scheduler issues but not really concerned about them
DSError TRKTargetAccessExtended1(u32 firstRegister, u32 lastRegister, MessageBuffer *b, size_t *registerStorageSize, s32 read) {
    DSError err;
    DefaultType *start;
    s32 count;
    TRKExceptionStatus savedException;

    if (lastRegister > 0x60)
        return kInvalidRegister;

    savedException = gTRKExceptionStatus;
    gTRKExceptionStatus.exceptionDetected = 0;

    *registerStorageSize = 0;

    if (firstRegister <= lastRegister)
    {
        start = (((u32 *)&gTRKCPUState.Extended1) + firstRegister);
        count = (s32)(lastRegister - firstRegister + 1);

        *registerStorageSize += (count * 1);

        if (read)
        {
            err = TRKAppendBuffer_ui32(b, start, (s32)count);
        }
        else
        {
            if ((start <= (u32 *)&gTRKCPUState.Extended1.TBU) &&   // TBU
                ((start + count - 1) >=
                    (u32 *)&gTRKCPUState.Extended1.TBL))  // TBL
                gTRKRestoreFlags.TBR = 1;

            if ((start <= (u32 *)&gTRKCPUState.Extended1.DEC) && ((start + count - 1) >= (u32 *)&gTRKCPUState.Extended1.DEC)) 
                gTRKRestoreFlags.DEC = 1;

            err = TRKReadBuffer_ui32(b, start, (s32)count);
        }
    }

    if (gTRKExceptionStatus.exceptionDetected)
    {
        err = kCWDSException;
        *registerStorageSize = 0;
    }

    gTRKExceptionStatus = savedException;

    return err;
}


DSError TRKTargetAccessFP(u32 firstRegister, u32 lastRegister,
                          MessageBuffer *b, size_t *registerStorageSize,
                          s32 read)
{
    DSError err;
    u32 current;
    FloatType value;
    TRKExceptionStatus savedException;

    if (lastRegister > 33)
        return kInvalidRegister;

    savedException = gTRKExceptionStatus;
    gTRKExceptionStatus.exceptionDetected = 0;

    __TRK_set_MSR(__TRK_get_MSR() | 0x2000);

    for (current = firstRegister, *registerStorageSize = 0, err = kNoError;
         (current <= lastRegister) && (err == kNoError);
         *registerStorageSize += sizeof(FloatType), current++)
    {

        if (read)
        {
            err = TRKPPCAccessFPRegister((void *)&value, current, read);

            err = TRKAppendBuffer1_ui64(b, value);
        }
        else
        {
            err = TRKReadBuffer1_ui64(b, &value);

            err = TRKPPCAccessFPRegister((void *)&value, current, read);
        }
    }

    if (gTRKExceptionStatus.exceptionDetected)
    {
        err = kCWDSException;
        *registerStorageSize = 0;
    }

    gTRKExceptionStatus = savedException;

    return err;
}


DSError TRKTargetAccessDefault(u32 firstRegister, u32 lastRegister,
                          MessageBuffer *b, size_t *registerStorageSize,
                          s32 read)
{
    DSError err;
    DefaultType *start;
    u32 count;
    TRKExceptionStatus savedException;

    if (lastRegister > 36)
        return kInvalidRegister;

    savedException = gTRKExceptionStatus;
    gTRKExceptionStatus.exceptionDetected = 0;

    start = (((DefaultType *)&gTRKCPUState.Default) + firstRegister);
    count = (lastRegister - firstRegister + 1);
    *registerStorageSize = (count * sizeof(DefaultType));

    if (read)
        err = TRKAppendBuffer_ui32(b, start, (s32)count);
    else
        err = TRKReadBuffer_ui32(b, start, (s32)count);

    if (gTRKExceptionStatus.exceptionDetected)
    {
        err = kCWDSException;
        *registerStorageSize = 0;
    }

    gTRKExceptionStatus = savedException;

    return err;
}

DSError TRKTargetReadInstruction(InstructionType *readData,
                                void *virtualAddress)
{
    DSError err = kNoError;
    size_t length = sizeof(InstructionType);


    err = TRKTargetAccessMemory(readData, virtualAddress, &length, 0, 1);

    if ((err == kNoError) && (length != sizeof(InstructionType)))
        err = kInvalidMemory;

    return err;
}

void* TRKTargetTranslate(u32*);


DSError TRKTargetAccessMemory(void *Data,
                        void *virtualAddress,
                        size_t *memorySize,
                        MemoryAccessOptions accessOptions,
                        s32 read)
{
    DSError err;
    TRKExceptionStatus savedException;
    void *transAddress;

    savedException = gTRKExceptionStatus;
    gTRKExceptionStatus.exceptionDetected = 0;


    transAddress = TRKTargetTranslate((u32 *)virtualAddress);

    err = TRKValidMemory32(transAddress, *memorySize,
                        read ? 0 : 1);

    if (err != kNoError)
    {
        *memorySize = 0;
    }
    else
    {

        u32 trk_msr = __TRK_get_MSR();
        u32 target_msr = (trk_msr | (gTRKCPUState.Extended1.MSR & 0x10));  // MSR

        if (read)
        {
            TRK_ppc_memcpy(Data, (const void *)transAddress, (size_t)*memorySize,
                        trk_msr, target_msr);
        }
        else
        {
            TRK_ppc_memcpy(transAddress, (const void *)Data, (u32)*memorySize,
                        target_msr, trk_msr);

            TRK_flush_cache((u32)transAddress, (u32)*memorySize);

            if (virtualAddress != transAddress)
                TRK_flush_cache((u32)virtualAddress, (u32)(*memorySize));
        }

        if (accessOptions == 0)
        {

        }
	}

    if (gTRKExceptionStatus.exceptionDetected)
    {
        err = kCWDSException;
        *memorySize = 0;
    }

    gTRKExceptionStatus = savedException;

    return err;
}


// TRKValidMemory32