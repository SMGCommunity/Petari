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

typedef struct TRKBuffer {
    unsigned char _0[4];
    unsigned int _4;
    int _8;
    unsigned int _C;
    unsigned int _10;
    unsigned char mBuffer[0x87C];
} TRKBuffer;

typedef int bool;

typedef unsigned char ui8;
typedef unsigned short ui16;
typedef unsigned long ui32;
typedef unsigned long long ui64;

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;
typedef double f64;

#endif // TRK_TYPES_H