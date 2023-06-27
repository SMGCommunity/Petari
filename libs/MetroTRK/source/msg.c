#include "export/UART.h"
#include "portable/msgbuf.h"

int TRKMessageSend(MessageBuffer *buffer) {
    TRKWriteUARTN(buffer->fData, buffer->fLength);
    return 0;
}