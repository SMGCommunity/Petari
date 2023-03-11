#ifndef TRK_TYPES_H
#define TRK_TYPES_H

typedef enum {
    TRK_Success = 0,
    TRK_Error100 = 0x100,
    TRK_Error301 = 0x301,
    TRK_Error302 = 0x302
} TRKResult;

typedef struct _TRKMessage {
    unsigned char _0[8];
    unsigned int mMessageLength;    // _8
    unsigned int _C;
    unsigned int mMessage;          // _10
} TRKMessage;

typedef struct TRKEvent {
    int mEventType;         // _0
    int _4;
    int mBufferIdx;         // _8
} TRKEvent;

typedef TRKBuffer {
    unsigned char _0[4];
    unsigned int _4;
    int _8;
    unsigned int _C;
    unsigned int _10;
    unsigned char mBuffer[0x87C];
} TRKBuffer;

#endif // TRK_TYPES_H