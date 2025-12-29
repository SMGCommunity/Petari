/**
 * dispatch.c
 * Description:
 */

#include "MetroTRK/Portable/dispatch.h"
#include "MetroTRK/Portable/msgbuf.h"

DSError TRKInitializeDispatcher() {
    return DS_NoError;
}

BOOL TRKDispatchMessage(TRKBuffer* msg) {
    u32 err;

    err = DS_DispatchError;
    TRKSetBufferPosition(msg, 0);

    switch (msg->data[4]) {
    case DSMSG_Connect:
        err = TRKDoConnect(msg);
        break;
    case DSMSG_Disconnect:
        err = TRKDoDisconnect(msg);
        break;
    case DSMSG_Reset:
        err = TRKDoReset(msg);
        break;
    case DSMSG_Override:
        err = TRKDoOverride(msg);
        break;
    case DSMSG_Versions:
        err = TRKDoVersions(msg);
        break;
    case DSMSG_SupportMask:
        err = TRKDoSupportMask(msg);
        break;
    case DSMSG_ReadMemory:
        err = TRKDoReadMemory(msg);
        break;
    case DSMSG_WriteMemory:
        err = TRKDoWriteMemory(msg);
        break;
    case DSMSG_ReadRegisters:
        err = TRKDoReadRegisters(msg);
        break;
    case DSMSG_WriteRegisters:
        err = TRKDoWriteRegisters(msg);
        break;
    case DSMSG_Continue:
        err = TRKDoContinue(msg);
        break;
    case DSMSG_Step:
        err = TRKDoStep(msg);
        break;
    case DSMSG_Stop:
        err = TRKDoStop(msg);
        break;
    case DSMSG_SetOption:
        err = TRKDoSetOption(msg);
        break;
    }

    return err;
}
