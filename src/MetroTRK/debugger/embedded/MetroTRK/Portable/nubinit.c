#include "portable/nubinit.h"
#include "portable/dserror.h"
#include "portable/dispatch.h"
#include "portable/serpoll.h"
#include "dolphin.h"

s32 gTRKBigEndian;

void TRKNubWelcome(void) {
	TRK_board_display("MetroTRK for Revolution v0.1");
}

DSError TRKTerminateNub(void) {
	TRKTerminateSerialHandler();
	return kNoError;
}

s32 TRKInitializeEndian(void) {
	ui8 bendian[4];
	s32 result = false;
	gTRKBigEndian = true;

	bendian[0] = 0x12;
	bendian[1] = 0x34;
	bendian[2] = 0x56;
	bendian[3] = 0x78;

	if (*(ui32*)bendian == 0x12345678) {
		gTRKBigEndian = true;
	} 
    else if (*(ui32*)bendian == 0x78563412) {
		gTRKBigEndian = false;
	} 
    else {
		result = true;
	}

    /* v0.1 has this call, v0.4 removes it */
    if (result == false) {
        usr_put_initialize();
    }

	return result;
}

DSError TRKInitializeNub(void) {
	DSError result;
	DSError resultTemp;

	result = TRKInitializeEndian();

	if (result == kNoError) {
		result = TRKInitializeEventQueue();
	}

	if (result == kNoError) {
		result = TRKInitializeMessageBuffers();
	}

    /* v0.1 has this call, v0.4 removes it */
    if (result == kNoError) {
        result = TRKInitializeDispatcher();
    }
    
	InitializeProgramEndTrap();

	if (result == kNoError) {
		result = TRKInitializeSerialHandler();
	}

	if (result == kNoError) {
		result = TRKInitializeTarget();
	}

	if (result == kNoError) {
		/* v0.4 has this line as resultTemp = TRK_InitializeIntDrivenUART(1, 0, &gTRKInputPendingPtr); */
		resultTemp = TRKInitializeIntDrivenUART(0xE100, 1, 0, &gTRKInputPendingPtr);
		TRKTargetSetInputPendingPtr(gTRKInputPendingPtr);
		if (resultTemp != kNoError) {
			result = resultTemp;
		}
	}
	return result;
}