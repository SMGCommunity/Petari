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
    unsigned int mMessageLength;    // 0x8
    unsigned int _C;
    unsigned int mMessage;          // 0x10
} TRKMessage;

typedef struct TRKEvent {
    int mEventType;         // 0x0
    int _4;
    int mBufferIdx;         // 0x8
} TRKEvent;

typedef struct TRKBuffer {
    unsigned char _0[4];
    unsigned int _4;
    int _8;
    unsigned int _C;
    unsigned int _10;
    unsigned char mBuffer[0x87C];
} TRKBuffer;

#define true    1
#define false   0

#ifndef NULL
#define NULL    0
#endif

typedef unsigned char ui8;
typedef unsigned short ui16;
typedef unsigned long ui32;

typedef unsigned long long ui64;

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;
typedef unsigned long long u64;
typedef double f64;


typedef char s8;
typedef short s16;
typedef int s32;
typedef long long s64;

#define DSFetch_u8(_p_)     (*((u8 *)_p_))
#define DSFetch_u16(_p_)    (*((u16 *)_p_))
#define DSFetch_u32(_p_)    (*((u32 *)_p_))
#define DSFetch_u64(_p_)    (*((u64 *)_p_))


#endif // TRK_TYPES_H