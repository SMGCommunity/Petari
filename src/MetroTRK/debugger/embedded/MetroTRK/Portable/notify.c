#include "portable/dserror.h"
#include "portable/msgbuf.h"
#include "portable/msgcmd.h"

DSError TRKDoNotifyStopped(MessageCommandID command) {
    DSError err;
    MessageBufferID replyID, bufferID;
    MessageBuffer* buffer;

    err = TRKGetFreeBuffer(&bufferID, &buffer);

    if (err == 0) {
        if (err == 0) {
            if (command == 0x90) {
                TRKTargetAddStopInfo(buffer);
            }
            else {
                TRKTargetAddExceptionInfo(buffer);
            }
        }

        err = TRKRequestSend(buffer, &replyID, 2, 3, 1);
        
        if (err == 0) {
            TRKReleaseBuffer(replyID);
        }

        TRKReleaseBuffer(bufferID);
    }
    
    return err;
}