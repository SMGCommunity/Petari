#include "portable/msgbuf.h"
#include "portable/msgcmd.h"
#include "portable/msghndlr.h"
#include "portable/dserror.h"
#include "TRK_Types.h"

DSError TRKDispatchMessage(MessageBuffer* buffer) {
    DSError result = kDispatchError;
    MessageBuffer* temp = buffer;
    ui8 command;
    
    TRKSetBufferPosition(buffer, 0);
    
    command = temp->fData[4];
    
    switch(command){
        case kDSConnect:
            result = TRKDoConnect(temp); 
        	break;
        case kDSDisconnect:
            result = TRKDoDisconnect(temp);
        	break;
        case kDSReset:
            result = TRKDoReset(temp);
        	break;
        case kDSOverride:
            result = TRKDoOverride(temp);
        	break;
        /* v0.4 removes these two cases */
        case kDSVersions:
            result = TRKDoVersions(temp);
            break;
        case kDSSupportMask:
            result = TRKDoSupportMask(temp);
            break;
        case kDSReadMemory:
            result = TRKDoReadMemory(temp);
        	break;
        case kDSWriteMemory:
            result = TRKDoWriteMemory(temp);
        	break;
        case kDSReadRegisters:
            result = TRKDoReadRegisters(temp);
        	break;
        case kDSWriteRegisters:
            result = TRKDoWriteRegisters(temp);
        	break;
        case kDSContinue:
            result = TRKDoContinue(temp);
        	break;
        case kDSStep:
            result = TRKDoStep(temp);
        	break;
        case kDSStop:
            result = TRKDoStop(temp);
        	break;
        case kDSSetOption:
            result = TRKDoSetOption(temp);
        	break;
    }
    return result;
}

DSError TRKInitializeDispatcher(void) {
    return kNoError;
}