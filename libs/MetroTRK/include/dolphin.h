#ifndef DOLPHIN_H
#define DOLPHIN_H

#include "TRK_Types.h"
#include "export/UART.h"

typedef struct OSContext OSContext;

typedef s16 __OSInterrupt;
typedef void (*__OSInterruptHandler)(__OSInterrupt, OSContext *);

typedef int (*DBCommFunc)();
typedef int (*DBCommInitFunc)(void*, __OSInterruptHandler);
typedef int (*DBCommReadFunc)(ui8*, int);
typedef int (*DBCommWriteFunc)(const ui8*, int);

typedef struct DBCommTable{
    DBCommInitFunc initialize_func;
    DBCommFunc initinterrupts_func;
    DBCommFunc shutdown_func;
    DBCommFunc peek_func;
    DBCommReadFunc read_func;
    DBCommWriteFunc write_func;
    DBCommFunc open_func;
    DBCommFunc close_func;
    DBCommFunc pre_continue_func;
    DBCommFunc post_stop_func;
} DBCommTable;

void TKR_board_display(char *);

void TRKUARTInterruptHandler(void);
UARTError TRK_InitializeIntDrivenUART(ui32 r3, ui32 r4, ui32 r5, void* r6);

void EnableEXI2Interrupts(void);
void EnableMetroTRKInterrupts(void);


typedef struct TRKState_PPC {
	ui32 GPR[32]; //0x0
	ui32 LR; //0x80
	ui32 CTR; //0x84
	ui32 XER; //0x88
	ui32 MSR; //0x8c
	ui32 DAR; //0x90
	ui32 DSISR; //0x94
	bool stopped; //0x98
	bool inputActivated; //0x9c
	ui8* inputPendingPtr; //0xA0
} TRKState_PPC;

#endif // DOLPHIN_H